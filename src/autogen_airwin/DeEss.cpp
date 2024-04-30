/* ========================================
 *  DeEss - DeEss.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DeEss_H
#include "DeEss.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::DeEss {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new DeEss(audioMaster);}

DeEss::DeEss(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.5; //-48.0 to 0.0
	C = 0.5;
	
	s1L = s2L = s3L = s4L = s5L = s6L= s7L = 0.0;
	m1L = m2L = m3L = m4L = m5L = m6L = 0.0;
	c1L = c2L = c3L = c4L = c5L = 0.0;
	ratioAL = ratioBL = 1.0;
	iirSampleAL = 0.0;
	iirSampleBL = 0.0;
	
	s1R = s2R = s3R = s4R = s5R = s6R = s7R = 0.0;
	m1R = m2R = m3R = m4R = m5R = m6R = 0.0;
	c1R = c2R = c3R = c4R = c5R = 0.0;
	ratioAR = ratioBR = 1.0;
	iirSampleAR = 0.0;
	iirSampleBR = 0.0;
	
	flip = false;	
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
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

DeEss::~DeEss() {}
VstInt32 DeEss::getVendorVersion () {return 1000;}
void DeEss::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void DeEss::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void DeEss::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float DeEss::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void DeEss::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Intense", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Max DS", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Freq", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void DeEss::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B-1.0)*48.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void DeEss::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 DeEss::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool DeEss::getEffectName(char* name) {
    vst_strncpy(name, "DeEss", kVstMaxProductStrLen); return true;
}

VstPlugCategory DeEss::getPlugCategory() {return kPlugCategEffect;}

bool DeEss::getProductString(char* text) {
  	vst_strncpy (text, "airwindows DeEss", kVstMaxProductStrLen); return true;
}

bool DeEss::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool DeEss::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value / 48.0) + 1.0; } return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool DeEss::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
