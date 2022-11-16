//
// Created by Paul Walker on 11/16/22.
//

#ifndef AIRWIN2_RACK_AIRWIN2RACKBASE_H
#define AIRWIN2_RACK_AIRWIN2RACKBASE_H

#include <cstdint>
#include <rack.hpp>


typedef int32_t audioMasterCallback;
typedef int32_t VstInt32;
typedef int32_t VstPlugCategory;
static constexpr int32_t kPlugCategEffect{1};
static constexpr int32_t kVstMaxProgNameLen{64};
static constexpr int32_t kVstMaxParamStrLen{32};
static constexpr int32_t kVstMaxProductStrLen{32};
static constexpr int32_t kVstMaxVendorStrLen{32};

struct Airwin2RackBase {
    Airwin2RackBase(audioMasterCallback m, int, int) {}
    virtual ~Airwin2RackBase() = default;

    float getSampleRate() { return APP->engine->getSampleRate(); }

    virtual void setNumInputs(int) {}
    virtual void setNumOutputs(int) {}
    virtual void setUniqueID(int) {}
    virtual bool canProcessReplacing() { return true; }
    virtual bool canDoubleReplacing() { return false; }
    virtual void programsAreChunks(bool) {}

    virtual void processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) = 0;

    virtual float getParameter(VstInt32 index) = 0;                   // get the parameter value at the specified index
    virtual void setParameter(VstInt32 index, float value) = 0;       // set the parameter at index to value
    virtual void getParameterLabel(VstInt32 index, char *text) = 0;  // label for the parameter (eg dB)
    virtual void getParameterDisplay(VstInt32 index, char *text) = 0; // text description of the current value

};

typedef Airwin2RackBase AudioEffectX;
typedef AudioEffectX AudioEffect;

inline void vst_strncpy(char *dst, const char *src, int n) {
   strncpy(dst, src, n);
}
inline void float2string(float, char *, int) {}

#endif //AIRWIN2_RACK_HACK_AIRWIN2RACKBASE_H
