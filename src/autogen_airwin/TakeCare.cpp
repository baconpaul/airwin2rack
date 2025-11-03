/* ========================================
 *  TakeCare - TakeCare.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TakeCare_H
#include "TakeCare.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::TakeCare {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new TakeCare(audioMaster);}

TakeCare::TakeCare(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.15;
	B = 0.5;
	C = 0.5;
	D = 0.5;
	E = 0.5;
	F = 0.5;
	G = 1.0;
	H = 1.0;
	
	for(int x = 0; x < 32767+2; x++) {a3AL[x] = 0.0; a3AR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3BL[x] = 0.0; a3BR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3CL[x] = 0.0; a3CR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3DL[x] = 0.0; a3DR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3EL[x] = 0.0; a3ER[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3FL[x] = 0.0; a3FR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3GL[x] = 0.0; a3GR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3HL[x] = 0.0; a3HR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3IL[x] = 0.0; a3IR[x] = 0.0;}
	c3AL = c3BL = c3CL = c3DL = c3EL = c3FL = c3GL = c3HL = c3IL = 1;
	c3AR = c3BR = c3CR = c3DR = c3ER = c3FR = c3GR = c3HR = c3IR = 1;
	f3AL = f3BL = f3CL = 0.0;
	f3CR = f3FR = f3IR = 0.0;
	avg3L = avg3R = 0.0;
	
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
	
	rotate = 0.0;
	oldfpd = 0.4294967295;
	
	buf = 8192;
	vibDepth = 0.0;
	outA = 1.0;
	outB = 1.0;
	wetA = 1.0;
	wetB = 1.0;
	derezA = 0.5;
	derezB = 0.5;
	
	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	lastSampleR = 0.0;
	wasPosClipR = false;
	wasNegClipR = false;

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

TakeCare::~TakeCare() {}
VstInt32 TakeCare::getVendorVersion () {return 1000;}
void TakeCare::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void TakeCare::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void TakeCare::setParameter(VstInt32 index, float value) {
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

float TakeCare::getParameter(VstInt32 index) {
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

void TakeCare::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Speed", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Rando", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Depth", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Regen", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Derez", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Buffer", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void TakeCare::getParameterDisplay(VstInt32 index, char *text) {
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

void TakeCare::getParameterLabel(VstInt32 index, char *text) {
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

VstInt32 TakeCare::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool TakeCare::getEffectName(char* name) {
    vst_strncpy(name, "TakeCare", kVstMaxProductStrLen); return true;
}

VstPlugCategory TakeCare::getPlugCategory() {return kPlugCategEffect;}

bool TakeCare::getProductString(char* text) {
  	vst_strncpy (text, "airwindows TakeCare", kVstMaxProductStrLen); return true;
}

bool TakeCare::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool TakeCare::parameterTextToValue(VstInt32 index, const char *text, float &value) {
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
bool TakeCare::canConvertParameterTextToValue(VstInt32 index) {
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
