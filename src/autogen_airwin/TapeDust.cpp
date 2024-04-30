/* ========================================
 *  TapeDust - TapeDust.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TapeDust_H
#include "TapeDust.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::TapeDust {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new TapeDust(audioMaster);}

TapeDust::TapeDust(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 1.0;
	
	for(int count = 0; count < 11; count++) {
		bL[count] = 0.0;
		fL[count] = 0.0;
		bR[count] = 0.0;
		fR[count] = 0.0;
	}
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	fpFlip = true;
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

TapeDust::~TapeDust() {}
VstInt32 TapeDust::getVendorVersion () {return 1000;}
void TapeDust::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void TapeDust::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void TapeDust::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float TapeDust::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void TapeDust::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Dust", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void TapeDust::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	}
}

void TapeDust::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 TapeDust::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool TapeDust::getEffectName(char* name) {
    vst_strncpy(name, "TapeDust", kVstMaxProductStrLen); return true;
}

VstPlugCategory TapeDust::getPlugCategory() {return kPlugCategEffect;}

bool TapeDust::getProductString(char* text) {
  	vst_strncpy (text, "airwindows TapeDust", kVstMaxProductStrLen); return true;
}

bool TapeDust::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool TapeDust::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool TapeDust::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;

    }
    return false;
}
} // end namespace
