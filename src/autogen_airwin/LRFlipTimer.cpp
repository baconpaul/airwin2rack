/* ========================================
 *  LRFlipTimer - LRFlipTimer.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __LRFlipTimer_H
#include "LRFlipTimer.h"
#endif
namespace airwin2rack::LRFlipTimer {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new LRFlipTimer(audioMaster);}

LRFlipTimer::LRFlipTimer(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	tick = 0;
	//this is reset: values being initialized only once. Startup values, whatever they are.
	
    _canDo.insert("plugAsChannelInsert"); // plug-in can be used as a channel insert effect.
    _canDo.insert("plugAsSend"); // plug-in can be used as a send effect.
    _canDo.insert("x2in2out"); 
    setNumInputs(kNumInputs);
    setNumOutputs(kNumOutputs);
    setUniqueID(kUniqueId);
    canProcessReplacing();     // supports output replacing
    canDoubleReplacing();      // supports double precision processing
	programsAreChunks(true);
    vst_strncpy (_programName, "Default", kVstMaxProgNameLen); // default program name
}

LRFlipTimer::~LRFlipTimer() {}
VstInt32 LRFlipTimer::getVendorVersion () {return 1000;}
void LRFlipTimer::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void LRFlipTimer::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void LRFlipTimer::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float LRFlipTimer::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void LRFlipTimer::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Minutes", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void LRFlipTimer::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A*9.0)+1.0, text, kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void LRFlipTimer::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 LRFlipTimer::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool LRFlipTimer::getEffectName(char* name) {
    vst_strncpy(name, "LRFlipTimer", kVstMaxProductStrLen); return true;
}

VstPlugCategory LRFlipTimer::getPlugCategory() {return kPlugCategEffect;}

bool LRFlipTimer::getProductString(char* text) {
  	vst_strncpy (text, "airwindows LRFlipTimer", kVstMaxProductStrLen); return true;
}

bool LRFlipTimer::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool LRFlipTimer::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value - 1.0) / (9.0); } return b; break; }

    }
    return false;
}
bool LRFlipTimer::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;

    }
    return false;
}
} // end namespace
