/* ========================================
 *  ConsoleX2Pre - ConsoleX2Pre.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleX2Pre_H
#include "ConsoleX2Pre.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::ConsoleX2Pre {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ConsoleX2Pre(audioMaster);}

ConsoleX2Pre::ConsoleX2Pre(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	D = 0.5;
	E = 0.5;
	F = 0.5;
	G = 0.5;
	H = 0.5;
	I = 1.0;
	J = 0.5;
	K = 0.5;
	L = 0.0;
	M = 1.0;
	N = 0.0;
	O = 0.0;
	P = 0.5;
	Q = 0.5;
	
	for (int x = 0; x < biq_total; x++) {
		highA[x] = 0.0;
		highB[x] = 0.0;
		highC[x] = 0.0;
		midA[x] = 0.0;
		midB[x] = 0.0;
		midC[x] = 0.0;
		lowA[x] = 0.0;
		lowB[x] = 0.0;
		lowC[x] = 0.0;
	}
	highLIIR = 0.0;
	highRIIR = 0.0;
	midLIIR = 0.0;
	midRIIR = 0.0;
	lowLIIR = 0.0;
	lowRIIR = 0.0;
	//SmoothEQ2
	
	for (int x = 0; x < bez_total; x++) {bezCompF[x] = 0.0;bezCompS[x] = 0.0;}
	bezCompF[bez_cycle] = 1.0; bezMaxF = 0.0;
	bezCompS[bez_cycle] = 1.0; bezGate = 2.0;
	//Dynamics2
	
	for(int count = 0; count < 22; count++) {
		iirHPositionL[count] = 0.0;
		iirHAngleL[count] = 0.0;
		iirHPositionR[count] = 0.0;
		iirHAngleR[count] = 0.0;
	}
	hBypass = false;
	
	for(int count = 0; count < 14; count++) {
		iirLPositionL[count] = 0.0;
		iirLAngleL[count] = 0.0;
		iirLPositionR[count] = 0.0;
		iirLAngleR[count] = 0.0;
	}
	lBypass = false;	
	//Cabs2
	
	for(int count = 0; count < dscBuf+2; count++) {
		dBaL[count] = 0.0;
		dBaR[count] = 0.0;
	}
	dBaPosL = 0.0;
	dBaPosR = 0.0;
	dBaXL = 1;
	dBaXR = 1;
	//Discontapeity
	
	lFreqA = 1.0; lFreqB = 1.0;
	hFreqA = 0.0; hFreqB = 0.0;
	panA = 0.5; panB = 0.5;
	inTrimA = 0.5; inTrimB = 0.5;
	
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

ConsoleX2Pre::~ConsoleX2Pre() {}
VstInt32 ConsoleX2Pre::getVendorVersion () {return 1000;}
void ConsoleX2Pre::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ConsoleX2Pre::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void ConsoleX2Pre::setParameter(VstInt32 index, float value) {
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
        case kParamK: K = value; break;
        case kParamL: L = value; break;
        case kParamM: M = value; break;
        case kParamN: N = value; break;
        case kParamO: O = value; break;
        case kParamP: P = value; break;
        case kParamQ: Q = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float ConsoleX2Pre::getParameter(VstInt32 index) {
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
        case kParamK: return K; break;
        case kParamL: return L; break;
        case kParamM: return M; break;
        case kParamN: return N; break;
        case kParamO: return O; break;
        case kParamP: return P; break;
        case kParamQ: return Q; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ConsoleX2Pre::getParameterName(VstInt32 index, char *text) {
    switch (index) {
		case kParamA: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "HMid", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "LMid", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Bass", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "HighF", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "HMidF", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "LMidF", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "BassF", kVstMaxParamStrLen); break;
		case kParamI: vst_strncpy (text, "Thresh", kVstMaxParamStrLen); break;
		case kParamJ: vst_strncpy (text, "Attack", kVstMaxParamStrLen); break;
		case kParamK: vst_strncpy (text, "Release", kVstMaxParamStrLen); break;
		case kParamL: vst_strncpy (text, "Gate", kVstMaxParamStrLen); break;
		case kParamM: vst_strncpy (text, "Lowpass", kVstMaxParamStrLen); break;
		case kParamN: vst_strncpy (text, "Hipass", kVstMaxParamStrLen); break;
		case kParamO: vst_strncpy (text, "More", kVstMaxParamStrLen); break;
		case kParamP: vst_strncpy (text, "Pan", kVstMaxParamStrLen); break;
		case kParamQ: vst_strncpy (text, "Fader", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ConsoleX2Pre::getParameterDisplay(VstInt32 index, char *text) {
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
        case kParamK: float2string (K, text, kVstMaxParamStrLen); break;
        case kParamL: float2string (L, text, kVstMaxParamStrLen); break;
        case kParamM: float2string (M, text, kVstMaxParamStrLen); break;
        case kParamN: float2string (N, text, kVstMaxParamStrLen); break;
        case kParamO: float2string (O, text, kVstMaxParamStrLen); break;
        case kParamP: float2string (P, text, kVstMaxParamStrLen); break;
        case kParamQ: float2string (Q, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ConsoleX2Pre::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "eq", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "freq", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamI: vst_strncpy (text, "dyn", kVstMaxParamStrLen); break;
        case kParamJ: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamK: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamL: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamM: vst_strncpy (text, "fltr", kVstMaxParamStrLen); break;
        case kParamN: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamO: vst_strncpy (text, "+", kVstMaxParamStrLen); break;
        case kParamP: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamQ: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ConsoleX2Pre::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ConsoleX2Pre::getEffectName(char* name) {
    vst_strncpy(name, "ConsoleX2Pre", kVstMaxProductStrLen); return true;
}

VstPlugCategory ConsoleX2Pre::getPlugCategory() {return kPlugCategEffect;}

bool ConsoleX2Pre::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ConsoleX2Pre", kVstMaxProductStrLen); return true;
}

bool ConsoleX2Pre::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool ConsoleX2Pre::parameterTextToValue(VstInt32 index, const char *text, float &value) {
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
    case kParamK: { auto b = string2float(text, value); return b; break; }
    case kParamL: { auto b = string2float(text, value); return b; break; }
    case kParamM: { auto b = string2float(text, value); return b; break; }
    case kParamN: { auto b = string2float(text, value); return b; break; }
    case kParamO: { auto b = string2float(text, value); return b; break; }
    case kParamP: { auto b = string2float(text, value); return b; break; }
    case kParamQ: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool ConsoleX2Pre::canConvertParameterTextToValue(VstInt32 index) {
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
        case kParamK: return true;
        case kParamL: return true;
        case kParamM: return true;
        case kParamN: return true;
        case kParamO: return true;
        case kParamP: return true;
        case kParamQ: return true;

    }
    return false;
}
} // end namespace
