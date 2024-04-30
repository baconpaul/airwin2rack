/* ========================================
 *  Capacitor2 - Capacitor2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Capacitor2_H
#include "Capacitor2.h"
#endif
namespace airwinconsolidated::Capacitor2 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Capacitor2(audioMaster);}

Capacitor2::Capacitor2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 0.0;
	C = 0.0;
	D = 1.0;
	iirHighpassAL = 0.0;
	iirHighpassBL = 0.0;
	iirHighpassCL = 0.0;
	iirHighpassDL = 0.0;
	iirHighpassEL = 0.0;
	iirHighpassFL = 0.0;
	iirLowpassAL = 0.0;
	iirLowpassBL = 0.0;
	iirLowpassCL = 0.0;
	iirLowpassDL = 0.0;
	iirLowpassEL = 0.0;
	iirLowpassFL = 0.0;
	
	iirHighpassAR = 0.0;
	iirHighpassBR = 0.0;
	iirHighpassCR = 0.0;
	iirHighpassDR = 0.0;
	iirHighpassER = 0.0;
	iirHighpassFR = 0.0;
	iirLowpassAR = 0.0;
	iirLowpassBR = 0.0;
	iirLowpassCR = 0.0;
	iirLowpassDR = 0.0;
	iirLowpassER = 0.0;
	iirLowpassFR = 0.0;
	count = 0;
	lowpassChase = 0.0;
	highpassChase = 0.0;
	wetChase = 0.0;
	lowpassBaseAmount = 1.0;
	highpassBaseAmount = 0.0;
	wet = 1.0;
	lastLowpass = 1000.0;
	lastHighpass = 1000.0;
	lastWet = 1000.0;
	
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

Capacitor2::~Capacitor2() {}
VstInt32 Capacitor2::getVendorVersion () {return 1000;}
void Capacitor2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Capacitor2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Capacitor2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Capacitor2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Capacitor2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Lowpass", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Highpass", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "NonLin", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Capacitor2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Capacitor2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Capacitor2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Capacitor2::getEffectName(char* name) {
    vst_strncpy(name, "Capacitor2", kVstMaxProductStrLen); return true;
}

VstPlugCategory Capacitor2::getPlugCategory() {return kPlugCategEffect;}

bool Capacitor2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Capacitor2", kVstMaxProductStrLen); return true;
}

bool Capacitor2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Capacitor2::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Capacitor2::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;

    }
    return false;
}
} // end namespace
