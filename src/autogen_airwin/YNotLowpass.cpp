/* ========================================
 *  YNotLowpass - YNotLowpass.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __YNotLowpass_H
#include "YNotLowpass.h"
#endif
namespace airwinconsolidated::YNotLowpass {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new YNotLowpass(audioMaster);}

YNotLowpass::YNotLowpass(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.1;
	B = 0.5;
	C = 0.1;
	D = 0.1;
	E = 1.0;
	F = 1.0;
	
	for (int x = 0; x < biq_total; x++) {biquad[x] = 0.0;}
	for (int x = 0; x < fix_total; x++) {fixA[x] = 0.0; fixB[x] = 0.0;}
	
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

YNotLowpass::~YNotLowpass() {}
VstInt32 YNotLowpass::getVendorVersion () {return 1000;}
void YNotLowpass::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void YNotLowpass::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void YNotLowpass::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float YNotLowpass::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void YNotLowpass::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Gain", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Freq", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Reson8", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "ResEdge", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void YNotLowpass::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void YNotLowpass::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 YNotLowpass::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool YNotLowpass::getEffectName(char* name) {
    vst_strncpy(name, "YNotLowpass", kVstMaxProductStrLen); return true;
}

VstPlugCategory YNotLowpass::getPlugCategory() {return kPlugCategEffect;}

bool YNotLowpass::getProductString(char* text) {
  	vst_strncpy (text, "airwindows YNotLowpass", kVstMaxProductStrLen); return true;
}

bool YNotLowpass::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool YNotLowpass::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }
    case kParamF: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool YNotLowpass::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;
        case kParamE: return true;
        case kParamF: return true;

    }
    return false;
}
} // end namespace
