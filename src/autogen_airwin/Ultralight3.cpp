/* ========================================
 *  Ultralight3 - Ultralight3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Ultralight3_H
#include "Ultralight3.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::Ultralight3 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Ultralight3(audioMaster);}

Ultralight3::Ultralight3(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.3;
	B = 0.0;
	C = 0.0;
	D = 0.0;
	E = 0.5;
	F = 0.0;
	G = 0.0;
	H = 0.0;

	for(int x = 0; x < d4A+2; x++) aA[x] = 0.0f;
	for(int x = 0; x < d4B+2; x++) aB[x] = 0.0f;
	for(int x = 0; x < d4C+2; x++) aC[x] = 0.0f;
	for(int x = 0; x < d4D+2; x++) aD[x] = 0.0f;
	for(int x = 0; x < d4E+2; x++) aE[x] = 0.0f;
	for(int x = 0; x < d4F+2; x++) aF[x] = 0.0f;
	for(int x = 0; x < d4G+2; x++) aG[x] = 0.0f;
	for(int x = 0; x < d4H+2; x++) aH[x] = 0.0f;
	for(int x = 0; x < d4I+2; x++) aI[x] = 0.0f;
	for(int x = 0; x < d4J+2; x++) aJ[x] = 0.0f;
	for(int x = 0; x < d4K+2; x++) aK[x] = 0.0f;
	for(int x = 0; x < d4L+2; x++) aL[x] = 0.0f;
	for(int x = 0; x < d4M+2; x++) aM[x] = 0.0f;
	for(int x = 0; x < d4N+2; x++) aN[x] = 0.0f;
	for(int x = 0; x < d4O+2; x++) aO[x] = 0.0f;
	for(int x = 0; x < d4P+2; x++) aP[x] = 0.0f;
	for(int x = 0; x < d4Q+2; x++) aQ[x] = 0.0f;
	for(int x = 0; x < d4R+2; x++) aR[x] = 0.0f;
	for(int x = 0; x < d4S+2; x++) aS[x] = 0.0f;
	for(int x = 0; x < d4T+2; x++) aT[x] = 0.0f;
	cA = cB = cC = cD = cE = cF = cG = cH = 1;
	cI = cJ = cK = cL = cM = cN = cO = cP = 1;
	cQ = cR = cS = cT = 1;
	oQ = oR = oS = oT = 0.0f;
	fA = fB = fC = fD = 0.0f;
	fhA = fhB = fhC = fhD = 0.0f;
	flA = flB = flC = flD = 0.0f;
	hA = hB = hC = hD = hE = hF = hG = hH = hI = 0.0f;
	
	fdLA = fdLB = fdLC = fdLD = fdLE = fdLF = fdLG = fdLH = 0.0;
	fdRA = fdRB = fdRC = fdRD = fdRE = fdRF = fdRG = fdRH = 0.0;
	
	pointCycle = pointAL = pointBL = pointAR = pointBR = 0.0f;	
		
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

Ultralight3::~Ultralight3() {}
VstInt32 Ultralight3::getVendorVersion () {return 1000;}
void Ultralight3::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Ultralight3::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Ultralight3::setParameter(VstInt32 index, float value) {
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

float Ultralight3::getParameter(VstInt32 index) {
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

void Ultralight3::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Damping", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "UnSolid", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "UnReflc", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Predlay", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Frnt/Bk", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "Bot/Top", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "Distnce", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Ultralight3::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: int2string ((int)(A*9.99), text, kVstMaxParamStrLen); break;
        case kParamB: int2string ((int)(B*4.99), text, kVstMaxParamStrLen); break;
        case kParamC: int2string ((int)(C*4.99), text, kVstMaxParamStrLen); break;
        case kParamD: int2string ((int)(D*998.0), text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        case kParamG: float2string (G, text, kVstMaxParamStrLen); break;
        case kParamH: float2string (H, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Ultralight3::getParameterLabel(VstInt32 index, char *text) {
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

VstInt32 Ultralight3::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Ultralight3::getEffectName(char* name) {
    vst_strncpy(name, "Ultralight3", kVstMaxProductStrLen); return true;
}

VstPlugCategory Ultralight3::getPlugCategory() {return kPlugCategEffect;}

bool Ultralight3::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Ultralight3", kVstMaxProductStrLen); return true;
}

bool Ultralight3::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Ultralight3::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value + 0.1) / 9.99; } return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value + 0.1) / 4.99; } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = (value + 0.1) / 4.99; } return b; break; }
    case kParamD: { auto b = string2float(text, value); if (b) { value = (value + 0.1) / 998.0; } return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }
    case kParamF: { auto b = string2float(text, value); return b; break; }
    case kParamG: { auto b = string2float(text, value); return b; break; }
    case kParamH: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Ultralight3::canConvertParameterTextToValue(VstInt32 index) {
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
