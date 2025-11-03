/* ========================================
 *  AngleEQ - AngleEQ.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __AngleEQ_H
#include "AngleEQ.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::AngleEQ {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new AngleEQ(audioMaster);}

AngleEQ::AngleEQ(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.4;
	C = 0.5;
	D = 0.8;
	E = 0.8;
	F = 0.2;
	G = 0.2;
	H = 1.0;

	for(int count = 0; count < 36; count++) {
		iirHPositionL[count] = 0.0;
		iirHAngleL[count] = 0.0;
		iirMHPositionL[count] = 0.0;
		iirMHAngleL[count] = 0.0;
		iirMLPositionL[count] = 0.0;
		iirMLAngleL[count] = 0.0;
		iirLPositionL[count] = 0.0;
		iirLAngleL[count] = 0.0;

		iirHPositionR[count] = 0.0;
		iirHAngleR[count] = 0.0;
		iirMHPositionR[count] = 0.0;
		iirMHAngleR[count] = 0.0;
		iirMLPositionR[count] = 0.0;
		iirMLAngleR[count] = 0.0;
		iirLPositionR[count] = 0.0;
		iirLAngleR[count] = 0.0;
	}
	
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

AngleEQ::~AngleEQ() {}
VstInt32 AngleEQ::getVendorVersion () {return 1000;}
void AngleEQ::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void AngleEQ::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void AngleEQ::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        case kParamG: G = value; break;
        case kParamH: H = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float AngleEQ::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        case kParamG: return G; break;
        case kParamH: return H; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void AngleEQ::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Mid", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Low", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "HiFreq", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "MHiFreq", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "MLoFreq", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "LoFreq", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "Inv/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void AngleEQ::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        case kParamG: float2string (G, text, kVstMaxParamStrLen); break;
        case kParamH: float2string (H, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void AngleEQ::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 AngleEQ::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool AngleEQ::getEffectName(char* name) {
    vst_strncpy(name, "AngleEQ", kVstMaxProductStrLen); return true;
}

VstPlugCategory AngleEQ::getPlugCategory() {return kPlugCategEffect;}

bool AngleEQ::getProductString(char* text) {
  	vst_strncpy (text, "airwindows AngleEQ", kVstMaxProductStrLen); return true;
}

bool AngleEQ::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool AngleEQ::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }
    case kParamF: { auto b = string2float(text, value); return b; break; }
    case kParamG: { auto b = string2float(text, value); return b; break; }
    case kParamH: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool AngleEQ::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;
        case kParamE: return true;
        case kParamF: return true;
        case kParamG: return true;
        case kParamH: return true;

    }
    return false;
}
} // end namespace
