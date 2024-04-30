/* ========================================
 *  DeBess - DeBess.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DeBess_H
#include "DeBess.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::DeBess {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new DeBess(audioMaster);}

DeBess::DeBess(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.5;
	C = 0.5;
	D = 0.5;
	E = 0.0;
	for (int x = 0; x < 41; x++) {
		sL[x] = 0.0; mL[x] = 0.0; cL[x] = 0.0;
		sR[x] = 0.0; mR[x] = 0.0; cR[x] = 0.0;
	}
	ratioAL = ratioBL = 1.0;
	iirSampleAL = 0.0;
	iirSampleBL = 0.0;
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

DeBess::~DeBess() {}
VstInt32 DeBess::getVendorVersion () {return 1000;}
void DeBess::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void DeBess::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void DeBess::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float DeBess::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void DeBess::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Intense", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Sharp", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Depth", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Filter", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Sense", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void DeBess::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void DeBess::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: 
			if (E > 0.499999) vst_strncpy (text, "Vox", kVstMaxParamStrLen);
			else vst_strncpy (text, "Ess", kVstMaxParamStrLen);
			break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 DeBess::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool DeBess::getEffectName(char* name) {
    vst_strncpy(name, "DeBess", kVstMaxProductStrLen); return true;
}

VstPlugCategory DeBess::getPlugCategory() {return kPlugCategEffect;}

bool DeBess::getProductString(char* text) {
  	vst_strncpy (text, "airwindows DeBess", kVstMaxProductStrLen); return true;
}

bool DeBess::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool DeBess::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool DeBess::canConvertParameterTextToValue(VstInt32 index) {
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
