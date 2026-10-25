/*
 * airwindows-individual - native LV2 wrapper for the Airwindows effect suite.
 *
 * Exposes one LV2 plugin descriptor per effect from a single shared binary,
 * with exact per-effect control ports matching each effect's parameter
 * count.  No JUCE involvement; links only against airwin-registry.
 *
 * Port layout (same for every effect):
 *   0   - Audio Input  L  (lv2:AudioPort lv2:InputPort)
 *   1   - Audio Input  R  (lv2:AudioPort lv2:InputPort)
 *   2   - Audio Output L  (lv2:AudioPort lv2:OutputPort)
 *   3   - Audio Output R  (lv2:AudioPort lv2:OutputPort)
 *   4.. - Control Inputs  (lv2:ControlPort lv2:InputPort, nParams of them)
 *
 * This source released under the MIT License.
 */

#include <lv2/core/lv2.h>
#include "AirwinRegistry.h"

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <cstdint>

// ---------------------------------------------------------------------------
// Port index constants
// ---------------------------------------------------------------------------
static constexpr uint32_t PORT_AUDIO_IN_L  = 0;
static constexpr uint32_t PORT_AUDIO_IN_R  = 1;
static constexpr uint32_t PORT_AUDIO_OUT_L = 2;
static constexpr uint32_t PORT_AUDIO_OUT_R = 3;
static constexpr uint32_t PORT_CTRL_BASE   = 4;

// ---------------------------------------------------------------------------
// Forward declarations of LV2 callback functions
// ---------------------------------------------------------------------------
static LV2_Handle  aw_instantiate    (const LV2_Descriptor*, double, const char*, const LV2_Feature* const*);
static void        aw_connect_port   (LV2_Handle, uint32_t, void*);
static void        aw_activate       (LV2_Handle);
static void        aw_run            (LV2_Handle, uint32_t);
static void        aw_deactivate     (LV2_Handle);
static void        aw_cleanup        (LV2_Handle);
static const void* aw_extension_data (const char*);

// ---------------------------------------------------------------------------
// Per-descriptor metadata
//
// One .so binary, hundreds of plugins.
//
// Each effect gets a static LV2_Descriptor that the host sees as a separate
// plugin to load. The trick: AwDescriptorEntry embeds the descriptor as its
// FIRST MEMBER, so we can cast an LV2_Descriptor* directly back to
// AwDescriptorEntry* to recover the registryIndex without a lookup table.
//
// HOW IT WORKS:
// 1. Host loads the descriptor for eg. plugin #5 (Galactic)
// 2. lv2_descriptor(5) returns &g_descriptors[5].descriptor
// 3. Host calls instantiate(&descriptor, ...)
// 4. We cast: entry = (AwDescriptorEntry*)descriptor
// 5. Look up: reg = registry[entry->registryIndex]  (which is 5)
// 6. Call: effect = reg.generator()  (creates Galactic object)
// 7. Return: handle pointing to AwLv2Instance with that Galactic inside
//
// When host simultaneously loads plugin #0 (ADClip7):
// - Separate entry, separate registryIndex, separate effect object (ADClip7)
// - Same .so, totally independent processing chains
// - Each has its own handle, state, and effect instance
// ---------------------------------------------------------------------------
struct AwDescriptorEntry {
    LV2_Descriptor descriptor;   // FIRST MEMBER — enables container_of cast
    int            registryIndex; // which effect in AirwinRegistry
    std::string    uriStorage;   // owns the heap memory for descriptor.URI
};

// ---------------------------------------------------------------------------
// Per-instance runtime data
// ---------------------------------------------------------------------------
struct AwLv2Instance {
    int    registryIndex;
    int    nParams;
    std::unique_ptr<AirwinConsolidatedBase> effect;

    // Pointers set by connect_port; may be null until first block
    float* audioInL  = nullptr;
    float* audioInR  = nullptr;
    float* audioOutL = nullptr;
    float* audioOutR = nullptr;

    // One pointer per control parameter; null means not yet connected
    std::vector<float*> ctrlPorts;
};

// ---------------------------------------------------------------------------
// Global descriptor table (lazily built once)
// ---------------------------------------------------------------------------
static std::vector<AwDescriptorEntry> g_descriptors;
static std::once_flag                 g_initFlag;

static void buildDescriptors()
{
    // Set a sensible default for effects that read it before their first
    // instantiation (e.g. in constructor-side assertions).
    AirwinConsolidatedBase::defaultSampleRate = 44100.0f;

    // Size the vector once so entries never move after this point.
    // The uriStorage.c_str() pointer must remain stable for the lifetime
    // of the process — this single resize + in-place fill guarantees that.
    g_descriptors.resize(AirwinRegistry::registry.size());

    for (size_t i = 0; i < AirwinRegistry::registry.size(); ++i)
    {
        const auto& reg = AirwinRegistry::registry[i];
        auto&       e   = g_descriptors[i];

        e.registryIndex = static_cast<int>(i);
        e.uriStorage    = "https://airwindows.com/lv2/" + reg.name;

        e.descriptor.URI            = e.uriStorage.c_str();
        e.descriptor.instantiate    = aw_instantiate;
        e.descriptor.connect_port   = aw_connect_port;
        e.descriptor.activate       = aw_activate;
        e.descriptor.run            = aw_run;
        e.descriptor.deactivate     = aw_deactivate;
        e.descriptor.cleanup        = aw_cleanup;
        e.descriptor.extension_data = aw_extension_data;
    }
}

// ---------------------------------------------------------------------------
// LV2 callbacks
// ---------------------------------------------------------------------------
static LV2_Handle aw_instantiate(
    const LV2_Descriptor*       descriptor,
    double                      sample_rate,
    const char*                 /*bundle_path*/,
    const LV2_Feature* const*   /*features*/)
{
    // The host is passing us a descriptor for ONE specific effect (e.g., Galactic).
    // We need to figure out which effect it is and instantiate the right one.
    //
    // The descriptor is actually the first member of AwDescriptorEntry (our struct).
    // By casting it, we recover the original entry and its registryIndex.
    // Then we use that index to instantiate the exact right effect.
    const auto* entry = reinterpret_cast<const AwDescriptorEntry*>(descriptor);
    const auto& reg   = AirwinRegistry::registry[entry->registryIndex];

    // Now create an instance of THIS specific effect (Galactic, ADClip7, etc.)
    auto* inst       = new AwLv2Instance();
    inst->registryIndex = entry->registryIndex;
    inst->nParams       = reg.nParams;
    inst->effect        = reg.generator();  // <-- Create Galactic, or Baxandall, or...
    inst->effect->setSampleRate(static_cast<float>(sample_rate));
    inst->ctrlPorts.assign(reg.nParams, nullptr);

    // Each handle is independent. If the host loads Galactic AND Baxandall
    // at the same time, they'll each get their own handle with their own effect instance.
    return inst;
}

static void aw_connect_port(LV2_Handle handle, uint32_t port, void* data)
{
    auto* inst = static_cast<AwLv2Instance*>(handle);
    switch (port)
    {
        case PORT_AUDIO_IN_L:  inst->audioInL  = static_cast<float*>(data); return;
        case PORT_AUDIO_IN_R:  inst->audioInR  = static_cast<float*>(data); return;
        case PORT_AUDIO_OUT_L: inst->audioOutL = static_cast<float*>(data); return;
        case PORT_AUDIO_OUT_R: inst->audioOutR = static_cast<float*>(data); return;
        default: break;
    }
    int p = static_cast<int>(port) - static_cast<int>(PORT_CTRL_BASE);
    if (p >= 0 && p < inst->nParams)
        inst->ctrlPorts[p] = static_cast<float*>(data);
}

static void aw_activate(LV2_Handle /*handle*/) {}

static void aw_run(LV2_Handle handle, uint32_t sample_count)
{
    auto* inst = static_cast<AwLv2Instance*>(handle);
    if (!inst->effect
        || !inst->audioInL || !inst->audioInR
        || !inst->audioOutL || !inst->audioOutR)
        return;

    // Update parameters from the host
    for (int p = 0; p < inst->nParams; ++p)
    {
        if (inst->ctrlPorts[p])
            inst->effect->setParameter(p, *inst->ctrlPorts[p]);
    }

    // Process audio using instance's effect object
    float* inputs[2]  = { inst->audioInL,  inst->audioInR  };
    float* outputs[2] = { inst->audioOutL, inst->audioOutR };
    inst->effect->processReplacing(inputs, outputs, static_cast<int32_t>(sample_count));
}

static void aw_deactivate(LV2_Handle /*handle*/) {}

static void aw_cleanup(LV2_Handle handle)
{
    delete static_cast<AwLv2Instance*>(handle);
}

static const void* aw_extension_data(const char* /*uri*/) { return nullptr; }

// ---------------------------------------------------------------------------
// LV2 entry point
// ---------------------------------------------------------------------------
LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    std::call_once(g_initFlag, buildDescriptors);
    if (index >= static_cast<uint32_t>(g_descriptors.size()))
        return nullptr;
    return &g_descriptors[index].descriptor;
}
