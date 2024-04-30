/* ========================================
 *  BiquadNonLin - BiquadNonLin.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BiquadNonLin_H
#include "BiquadNonLin.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::BiquadNonLin {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new BiquadNonLin(audioMaster);}

BiquadNonLin::BiquadNonLin(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.5;
	C = 0.7071;
	D = 0.0;
	E = 1.0;
	
	for (int x = 0; x < biq_total; x++) {biquad[x] = 0.0;}
	
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

BiquadNonLin::~BiquadNonLin() {}
VstInt32 BiquadNonLin::getVendorVersion () {return 1000;}
void BiquadNonLin::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void BiquadNonLin::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void BiquadNonLin::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float BiquadNonLin::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void BiquadNonLin::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Type", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Freq", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Q", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "NonLin", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Inv/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void BiquadNonLin::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B*B*B*0.9999)+0.0001, text, kVstMaxParamStrLen); break;
        case kParamC: float2string ((C*C*C*29.99)+0.01, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string ((E*2.0)-1.0, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void BiquadNonLin::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 BiquadNonLin::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool BiquadNonLin::getEffectName(char* name) {
    vst_strncpy(name, "BiquadNonLin", kVstMaxProductStrLen); return true;
}

VstPlugCategory BiquadNonLin::getPlugCategory() {return kPlugCategEffect;}

bool BiquadNonLin::getProductString(char* text) {
  	vst_strncpy (text, "airwindows BiquadNonLin", kVstMaxProductStrLen); return true;
}

bool BiquadNonLin::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool BiquadNonLin::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = std::cbrt((value - 0.0001) / (0.9999)); } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = std::cbrt((value - 0.01) / (29.99)); } return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }

    }
    return false;
}
bool BiquadNonLin::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;
        case kParamE: return true;

    }
    return false;
}
} // end namespace
