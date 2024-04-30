/* ========================================
 *  TapeFat - TapeFat.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TapeFat_H
#include "TapeFat.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::TapeFat {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new TapeFat(audioMaster);}

TapeFat::TapeFat(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 0.0;
	
	for(int count = 0; count < 257; count++) {pL[count] = 0;pR[count] = 0;}
	gcount = 0;
	
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

TapeFat::~TapeFat() {}
VstInt32 TapeFat::getVendorVersion () {return 1000;}
void TapeFat::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void TapeFat::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void TapeFat::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float TapeFat::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void TapeFat::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Lean/Ft", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Depth", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void TapeFat::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (((A*2.0)-1.0), text, kVstMaxParamStrLen); break;
        case kParamB: float2string (floor((B*29.0)+3.0), text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void TapeFat::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "taps", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 TapeFat::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool TapeFat::getEffectName(char* name) {
    vst_strncpy(name, "TapeFat", kVstMaxProductStrLen); return true;
}

VstPlugCategory TapeFat::getPlugCategory() {return kPlugCategEffect;}

bool TapeFat::getProductString(char* text) {
  	vst_strncpy (text, "airwindows TapeFat", kVstMaxProductStrLen); return true;
}

bool TapeFat::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool TapeFat::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = std::clamp( (std::round(value) + 0.1 - (3))/29, 0., 1. ); } return b; break; }

    }
    return false;
}
bool TapeFat::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;

    }
    return false;
}
} // end namespace
