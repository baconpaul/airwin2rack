/* ========================================
 *  StoneFireComp - StoneFireComp.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __StoneFireComp_H
#include "StoneFireComp.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::StoneFireComp {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new StoneFireComp(audioMaster);}

StoneFireComp::StoneFireComp(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 0.5;
	C = 0.5;
	D = 0.5;
	E = 1.0;
	F = 0.5;
	G = 0.5;
	H = 0.5;
	I = 0.5;
	J = 1.0;
	
	for (int x = 0; x < kal_total; x++) kal[x] = 0.0;
	fireCompL = 1.0;
	fireCompR = 1.0;
	stoneCompL = 1.0;
	stoneCompR = 1.0;
	
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

StoneFireComp::~StoneFireComp() {}
VstInt32 StoneFireComp::getVendorVersion () {return 1000;}
void StoneFireComp::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void StoneFireComp::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void StoneFireComp::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        case kParamG: G = value; break;
        case kParamH: H = value; break;
        case kParamI: I = value; break;
        case kParamJ: J = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float StoneFireComp::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        case kParamG: return G; break;
        case kParamH: return H; break;
        case kParamI: return I; break;
        case kParamJ: return J; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void StoneFireComp::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Fire Th", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Attack", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Release", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Fire", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "StoneTh", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Attack", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "Release", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "Stone", kVstMaxParamStrLen); break;
		case kParamI: vst_strncpy (text, "Range", kVstMaxParamStrLen); break;
		case kParamJ: vst_strncpy (text, "Ratio", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void StoneFireComp::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        case kParamG: float2string (G, text, kVstMaxParamStrLen); break;
        case kParamH: float2string (H, text, kVstMaxParamStrLen); break;
        case kParamI: float2string (I, text, kVstMaxParamStrLen); break;
        case kParamJ: float2string (J, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void StoneFireComp::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamI: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamJ: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 StoneFireComp::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool StoneFireComp::getEffectName(char* name) {
    vst_strncpy(name, "StoneFireComp", kVstMaxProductStrLen); return true;
}

VstPlugCategory StoneFireComp::getPlugCategory() {return kPlugCategEffect;}

bool StoneFireComp::getProductString(char* text) {
  	vst_strncpy (text, "airwindows StoneFireComp", kVstMaxProductStrLen); return true;
}

bool StoneFireComp::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool StoneFireComp::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }
    case kParamF: { auto b = string2float(text, value); return b; break; }
    case kParamG: { auto b = string2float(text, value); return b; break; }
    case kParamH: { auto b = string2float(text, value); return b; break; }
    case kParamI: { auto b = string2float(text, value); return b; break; }
    case kParamJ: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool StoneFireComp::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;
        case kParamE: return true;
        case kParamF: return true;
        case kParamG: return true;
        case kParamH: return true;
        case kParamI: return true;
        case kParamJ: return true;

    }
    return false;
}
} // end namespace
