/* ========================================
 *  PointyGuitar - PointyGuitar.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PointyGuitar_H
#include "PointyGuitar.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::PointyGuitar {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new PointyGuitar(audioMaster);}

PointyGuitar::PointyGuitar(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.6;
	B = 0.5;
	C = 0.8;
	D = 0.6;
	E = 0.4;
	F = 0.5;
	G = 0.7;
	H = 0.4;
	I = 0.0;
	J = 0.7;

	for(int x=0; x<17; x++) {
		for(int y=0; y<11; y++) {
			angSL[x][y] = 0.0;angAL[x][y] = 0.0;
			angSR[x][y] = 0.0;angAR[x][y] = 0.0;
		}
	}
	for(int y=0; y<11; y++) angG[y] = 0.0;
	for(int count = 0; count < 36; count++) {
		iirHPositionL[count] = 0.0;
		iirHAngleL[count] = 0.0;
		iirBPositionL[count] = 0.0;
		iirBAngleL[count] = 0.0;
		iirHPositionR[count] = 0.0;
		iirHAngleR[count] = 0.0;
		iirBPositionR[count] = 0.0;
		iirBAngleR[count] = 0.0;
	}
	WasNegativeL = false;
	ZeroCrossL = 0;
	gaterollerL = 0.0;	
	WasNegativeR = false;
	ZeroCrossR = 0;
	gaterollerR = 0.0;	
	
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

PointyGuitar::~PointyGuitar() {}
VstInt32 PointyGuitar::getVendorVersion () {return 1000;}
void PointyGuitar::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void PointyGuitar::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void PointyGuitar::setParameter(VstInt32 index, float value) {
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

float PointyGuitar::getParameter(VstInt32 index) {
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

void PointyGuitar::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Drive", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Presnce", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Mid", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Low", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Sub", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "HSpeakr", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "LSpeakr", kVstMaxParamStrLen); break;
		case kParamI: vst_strncpy (text, "Gate", kVstMaxParamStrLen); break;
		case kParamJ: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void PointyGuitar::getParameterDisplay(VstInt32 index, char *text) {
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

void PointyGuitar::getParameterLabel(VstInt32 index, char *text) {
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

VstInt32 PointyGuitar::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool PointyGuitar::getEffectName(char* name) {
    vst_strncpy(name, "PointyGuitar", kVstMaxProductStrLen); return true;
}

VstPlugCategory PointyGuitar::getPlugCategory() {return kPlugCategEffect;}

bool PointyGuitar::getProductString(char* text) {
  	vst_strncpy (text, "airwindows PointyGuitar", kVstMaxProductStrLen); return true;
}

bool PointyGuitar::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool PointyGuitar::parameterTextToValue(VstInt32 index, const char *text, float &value) {
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
bool PointyGuitar::canConvertParameterTextToValue(VstInt32 index) {
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
