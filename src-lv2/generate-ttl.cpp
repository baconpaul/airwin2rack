/*
 * generate-ttl — standalone executable that writes the LV2 bundle TTL files
 * for airwindows-individual.
 *
 * Usage: generate-ttl <bundle-dir>
 *
 * Outputs:
 *   <bundle-dir>/manifest.ttl              — lists all plugin URIs
 *   <bundle-dir>/airwindows-individual.ttl — full plugin descriptions
 *
 * The binary links against airwin-registry, so the registry static
 * initializers in ModuleAdd.h populate AirwinRegistry::registry before
 * main() runs.
 *
 * This source released under the MIT License.
 */

#include "AirwinRegistry.h"

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <cctype>
#include <memory>

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Produce a valid LV2 port symbol (C-identifier-like) from a human name.
static std::string makeSymbol(const std::string& name)
{
    std::string sym;
    for (unsigned char c : name)
    {
        if (std::isalnum(c))
            sym += static_cast<char>(std::tolower(c));
        else if (c == ' ' || c == '-' || c == '_' || c == '.')
            sym += '_';
        // other punctuation / special chars dropped
    }
    // Collapse repeated underscores
    std::string out;
    bool lastWasUnderscore = false;
    for (char c : sym)
    {
        if (c == '_' && lastWasUnderscore) continue;
        lastWasUnderscore = (c == '_');
        out += c;
    }
    // Strip leading/trailing underscores
    while (!out.empty() && out.front() == '_') out.erase(out.begin());
    while (!out.empty() && out.back()  == '_') out.pop_back();
    // A symbol must not start with a digit
    if (!out.empty() && std::isdigit(static_cast<unsigned char>(out[0])))
        out = "p" + out;
    if (out.empty()) out = "ctrl";
    return out;
}

// Escape a string for use inside a Turtle string literal ("...").
static std::string ttlEscape(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    for (unsigned char c : s)
    {
        switch (c)
        {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': /* skip */     break;
            case '\t': out += " ";    break;
            default:
                if (c < 0x20) break; // skip other control chars
                out += static_cast<char>(c);
        }
    }
    return out;
}

// Map an Airwindows category string to a suitable lv2: plugin type URI.
static const char* categoryToLv2Type(const std::string& cat)
{
    if (cat == "Reverb" || cat == "Ambience")                     return "lv2:ReverbPlugin";
    if (cat == "Dynamics")                                        return "lv2:CompressorPlugin";
    if (cat == "Clipping")                                        return "lv2:LimiterPlugin";
    if (cat == "Distortion" || cat == "Lo-Fi" || cat == "Noise") return "lv2:DistortionPlugin";
    if (cat == "Filter" || cat == "Biquads"
        || cat == "XYZ Filters" || cat == "Bass")                 return "lv2:FilterPlugin";
    if (cat == "Brightness" || cat == "Tone Color")               return "lv2:EQPlugin";
    if (cat == "Saturation" || cat == "Tape")                     return "lv2:WaveshaperPlugin";
    if (cat == "Stereo")                                          return "lv2:SpatialPlugin";
    if (cat == "Amp Sims")                                        return "lv2:SimulatorPlugin";
    if (cat == "Consoles" || cat == "Utility" || cat == "Dithers") return "lv2:UtilityPlugin";
    // Subtlety, Effects, Unclassified, etc.
    return "lv2:FilterPlugin";
}

// ---------------------------------------------------------------------------
// TTL generation
// ---------------------------------------------------------------------------

static bool writeManifest(const std::string& bundleDir,
                          const std::vector<AirwinRegistry::awReg>& registry)
{
    std::ofstream f(bundleDir + "manifest.ttl");
    if (!f) { std::cerr << "Cannot write manifest.ttl\n"; return false; }

    f << "@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .\n";
    f << "@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .\n\n";

    for (const auto& r : registry)
    {
        f << "<https://airwindows.com/lv2/" << r.name << ">\n";
        f << "    a lv2:Plugin ;\n";
        f << "    lv2:binary <airwindows-individual.so> ;\n";
        f << "    rdfs:seeAlso <airwindows-individual.ttl> .\n\n";
    }
    return true;
}

static bool writePluginTtl(const std::string& bundleDir,
                           const std::vector<AirwinRegistry::awReg>& registry)
{
    std::ofstream f(bundleDir + "airwindows-individual.ttl");
    if (!f) { std::cerr << "Cannot write airwindows-individual.ttl\n"; return false; }

    f << "@prefix doap: <http://usefulinc.com/ns/doap#> .\n";
    f << "@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .\n";
    f << "@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .\n\n";

    for (const auto& r : registry)
    {
        // Instantiate the effect to read parameter names and default values.
        auto fx = r.generator();
        fx->setSampleRate(44100.0f);

        // Collect parameter symbols, deduplicating within this plugin.
        std::set<std::string> usedSymbols;
        auto uniqueSymbol = [&](const std::string& base) -> std::string
        {
            std::string sym = base;
            int suffix = 2;
            while (usedSymbols.count(sym))
                sym = base + "_" + std::to_string(suffix++);
            usedSymbols.insert(sym);
            return sym;
        };

        // ---- Plugin header ------------------------------------------------
        f << "<https://airwindows.com/lv2/" << r.name << ">\n";
        f << "    a lv2:Plugin, " << categoryToLv2Type(r.category) << " ;\n";
        f << "    doap:name \"" << ttlEscape(r.name) << "\" ;\n";
        f << "    doap:license <https://opensource.org/licenses/MIT> ;\n";
        if (!r.whatText.empty())
            f << "    rdfs:comment \"" << ttlEscape(r.whatText) << "\" ;\n";
        f << "    lv2:minorVersion 0 ;\n";
        f << "    lv2:microVersion 0 ;\n";

        // ---- Port list ----------------------------------------------------
        // Helper lambdas that emit port separators correctly.
        int portEmitted = 0;
        int totalPorts  = 4 + r.nParams;

        auto beginPort = [&]()
        {
            if (portEmitted == 0) f << "    lv2:port [\n";
            else                  f << "    ] , [\n";
            ++portEmitted;
        };
        auto endLastPort = [&]()
        {
            f << "    ] .\n\n";
        };

        // Audio in L
        beginPort();
        f << "        a lv2:AudioPort, lv2:InputPort ;\n";
        f << "        lv2:index 0 ;\n";
        f << "        lv2:symbol \"in_l\" ;\n";
        f << "        lv2:name \"In L\"\n";

        // Audio in R
        beginPort();
        f << "        a lv2:AudioPort, lv2:InputPort ;\n";
        f << "        lv2:index 1 ;\n";
        f << "        lv2:symbol \"in_r\" ;\n";
        f << "        lv2:name \"In R\"\n";

        // Audio out L
        beginPort();
        f << "        a lv2:AudioPort, lv2:OutputPort ;\n";
        f << "        lv2:index 2 ;\n";
        f << "        lv2:symbol \"out_l\" ;\n";
        f << "        lv2:name \"Out L\"\n";

        // Audio out R
        beginPort();
        f << "        a lv2:AudioPort, lv2:OutputPort ;\n";
        f << "        lv2:index 3 ;\n";
        f << "        lv2:symbol \"out_r\" ;\n";
        f << "        lv2:name \"Out R\"\n";

        // Control ports
        for (int p = 0; p < r.nParams; ++p)
        {
            char nameBuf[kVstMaxParamStrLen];
            nameBuf[0] = '\0';
            fx->getParameterName(p, nameBuf);
            std::string pname(nameBuf);

            std::string sym = uniqueSymbol(
                makeSymbol(pname.empty() ? "ctrl" : pname));

            float def = fx->getParameter(p);

            beginPort();
            f << "        a lv2:ControlPort, lv2:InputPort ;\n";
            f << "        lv2:index " << (4 + p) << " ;\n";
            f << "        lv2:symbol \"" << sym << "\" ;\n";
            f << "        lv2:name \"" << ttlEscape(pname) << "\" ;\n";
            f << "        lv2:default " << def << " ;\n";
            f << "        lv2:minimum 0.0 ;\n";
            f << "        lv2:maximum 1.0\n";
        }

        (void)totalPorts; // used only for the conceptual count
        endLastPort();
    }
    return true;
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <bundle-directory>\n";
        return 1;
    }

    std::string bundleDir = argv[1];
    if (bundleDir.back() != '/') bundleDir += '/';

    // registry is populated by static initializers in ModuleAdd.h
    // before main() is entered; no explicit init call required.
    AirwinConsolidatedBase::defaultSampleRate = 44100.0f;

    const auto& registry = AirwinRegistry::registry;

    if (!writeManifest(bundleDir, registry))   return 1;
    if (!writePluginTtl(bundleDir, registry))  return 1;

    std::cout << "Wrote " << registry.size()
              << " plugin descriptors to " << bundleDir << "\n";
    return 0;
}
