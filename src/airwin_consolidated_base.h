/*
 * AirwinConslidated - an adaptation of the airwindows effect suite for
 * open source clients
 *
 * This source released under the MIT License, found in ~/LICENSE.md.
 *
 * Copyright 2023 by the authors as described in the github transaction log
 */

#ifndef AIRWIN_CONSOLIDATED_AIRWINBASE_H
#define AIRWIN_CONSOLIDATED_AIRWINBASE_H

#include <cstdint>
#include <cassert>
#include <cstring>
#include <stdio.h>
#include <cmath>

typedef int32_t audioMasterCallback;
typedef int32_t VstInt32;
typedef int32_t VstPlugCategory;
static constexpr int32_t kPlugCategEffect{1};
static constexpr int32_t kVstMaxProgNameLen{64};
static constexpr int32_t kVstMaxParamStrLen{32};
static constexpr int32_t kVstMaxProductStrLen{32};
static constexpr int32_t kVstMaxVendorStrLen{32};

struct AirwinConsolidatedBase
{
    AirwinConsolidatedBase(audioMasterCallback m, int, int) {}
    virtual ~AirwinConsolidatedBase() = default;

    static float defaultSampleRate;
    float sampleRate{defaultSampleRate};
    void setSampleRate(float sr) { sampleRate = sr; }
    float getSampleRate()
    {
        assert(sampleRate > 2000);
        return sampleRate;
    }

    virtual bool getEffectName(char *name) { return false; }
    virtual void setNumInputs(int) {}
    virtual void setNumOutputs(int) {}
    virtual void setUniqueID(int) {}
    virtual bool canProcessReplacing() { return true; }
    virtual bool canDoubleReplacing() { return false; }
    virtual void programsAreChunks(bool) {}

    virtual void processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) = 0;

    virtual float
    getParameter(VstInt32 index) = 0; // get the parameter value at the specified index
    virtual void setParameter(VstInt32 index,
                              float value) = 0; // set the parameter at index to value
    virtual void getParameterLabel(VstInt32 index,
                                   char *text) = 0; // label for the parameter (eg dB)
    virtual void getParameterName(VstInt32 index,
                                  char *text) = 0; // label for the parameter (eg dB)
    virtual void getParameterDisplay(VstInt32 index,
                                     char *text) = 0; // text description of the current value

    virtual bool parameterTextToValue(VstInt32 index, const char *text, float &value)
    {
        return false;
    }
    virtual bool canConvertParameterTextToValue(VstInt32 index) { return false; }
};

typedef AirwinConsolidatedBase AudioEffectX;
typedef AudioEffectX AudioEffect;

inline void vst_strncpy(char *dst, const char *src, int n) { strncpy(dst, src, n); }
inline void float2string(float f, char *txt, int n) { snprintf(txt, n, "%8.4f", f); }
bool string2float(const char *txt, float &f);

inline void int2string(int i, char *t, size_t num) { snprintf(t, num, "%d", i); }
inline void dB2string(float value, char *t, size_t num)
{
    if (value <= 0.00001) // -100 dB, show -inf from that point onwards
        vst_strncpy(t, "-inf", num);
    else
        float2string((float)(20.0 * log10(value)), t, num);
}
bool string2dBNorm(const char *txt, float &f);

#endif
