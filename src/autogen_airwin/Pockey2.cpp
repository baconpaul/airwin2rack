/* ========================================
 *  Pockey2 - Pockey2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Pockey2_H
#include "Pockey2.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Pockey2 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Pockey2(audioMaster);}

Pockey2::Pockey2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.66;
	C = 1.0;
	
	lastSampleL = 0.0;
	heldSampleL = 0.0;
	previousHeldL = 0.0;
	
	lastSampleR = 0.0;
	heldSampleR = 0.0;
	previousHeldR = 0.0;
	
	position = 0.0;

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

Pockey2::~Pockey2() {}
VstInt32 Pockey2::getVendorVersion () {return 1000;}
void Pockey2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Pockey2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Pockey2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Pockey2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Pockey2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "DeFreq", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "DeRez", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Pockey2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((4.0+(B*12.0)), text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Pockey2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Pockey2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Pockey2::getEffectName(char* name) {
    vst_strncpy(name, "Pockey2", kVstMaxProductStrLen); return true;
}

VstPlugCategory Pockey2::getPlugCategory() {return kPlugCategEffect;}

bool Pockey2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Pockey2", kVstMaxProductStrLen); return true;
}

bool Pockey2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Pockey2::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = std::clamp( (value - 4.0) / 12.0, 0., 1. ); } return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Pockey2::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
