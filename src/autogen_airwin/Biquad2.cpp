/* ========================================
 *  Biquad2 - Biquad2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Biquad2_H
#include "Biquad2.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Biquad2 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Biquad2(audioMaster);}

Biquad2::Biquad2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for (int x = 0; x < 15; x++) {biquad[x] = 0.0;}
	for (int x = 0; x < 11; x++) {bL[x] = 0.0; bR[x] = 0.0; f[x] = 0.0;}
	frequencychase = 0.0015;
	resonancechase = 0.001;
	outputchase = 1.0;
	wetchase = 1.0;
	
	frequencysetting = -1.0;
	resonancesetting = -1.0;
	outputsetting = -1.0;
	wetsetting = -2.0; //-1.0 is a possible setting here and this forces an update on chasespeed
	
	chasespeed = 500.0;
	A = 1.0;
	B = 0.5;
	C = 0.5;
	D = 1.0;
	E = 1.0;
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

Biquad2::~Biquad2() {}
VstInt32 Biquad2::getVendorVersion () {return 1000;}
void Biquad2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Biquad2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Biquad2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Biquad2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Biquad2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Type", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Freq", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Q", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Inv/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Biquad2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((float)ceil((A*3.999)+0.00001), text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B*B*0.9999)+0.0001, text, kVstMaxParamStrLen); break;
        case kParamC: float2string ((C*C*49.99)+0.01, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string ((E*2.0)-1.0, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Biquad2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Biquad2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Biquad2::getEffectName(char* name) {
    vst_strncpy(name, "Biquad2", kVstMaxProductStrLen); return true;
}

VstPlugCategory Biquad2::getPlugCategory() {return kPlugCategEffect;}

bool Biquad2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Biquad2", kVstMaxProductStrLen); return true;
}

bool Biquad2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Biquad2::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = std::clamp(std::round(value) * 0.25 - 0.245, 0., 1.); } return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = sqrt(std::max((value - 0.0001) / (0.9999), 0.)); } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = sqrt(std::max((value - 0.01) / (49.99), 0.)); } return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }

    }
    return false;
}
bool Biquad2::canConvertParameterTextToValue(VstInt32 index) {
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
