/* ========================================
 *  Air4 - Air4.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Air4_H
#include "Air4.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::Air4 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Air4(audioMaster);}

Air4::Air4(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.52;
	D = 0.0;

	for (int x = 0; x < air_total; x++) air[x] = 0.0;

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

Air4::~Air4() {}
VstInt32 Air4::getVendorVersion () {return 1000;}
void Air4::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Air4::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Air4::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Air4::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Air4::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Air", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Gnd", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "DarkF", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Ratio", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Air4::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Air4::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Air4::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Air4::getEffectName(char* name) {
    vst_strncpy(name, "Air4", kVstMaxProductStrLen); return true;
}

VstPlugCategory Air4::getPlugCategory() {return kPlugCategEffect;}

bool Air4::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Air4", kVstMaxProductStrLen); return true;
}

bool Air4::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Air4::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Air4::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;

    }
    return false;
}
} // end namespace
