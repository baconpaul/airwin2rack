/* ========================================
 *  Galactic3 - Galactic3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Galactic3_H
#include "Galactic3.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Galactic3 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Galactic3(audioMaster);}

Galactic3::Galactic3(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	D = 1.0;
	E = 1.0;
	F = 1.0;
	
	iirAL = 0.0; iirAR = 0.0;
	iirBL = 0.0; iirBR = 0.0;
	
	for(int count = 0; count < 6479; count++) {aIL[count] = 0.0;aIR[count] = 0.0;}
	for(int count = 0; count < 3659; count++) {aJL[count] = 0.0;aJR[count] = 0.0;}
	for(int count = 0; count < 1719; count++) {aKL[count] = 0.0;aKR[count] = 0.0;}
	for(int count = 0; count < 679; count++) {aLL[count] = 0.0;aLR[count] = 0.0;}
	
	for(int count = 0; count < 9699; count++) {aAL[count] = 0.0;aAR[count] = 0.0;}
	for(int count = 0; count < 5999; count++) {aBL[count] = 0.0;aBR[count] = 0.0;}
	for(int count = 0; count < 2319; count++) {aCL[count] = 0.0;aCR[count] = 0.0;}
	for(int count = 0; count < 939; count++) {aDL[count] = 0.0;aDR[count] = 0.0;}
	
	for(int count = 0; count < 15219; count++) {aEL[count] = 0.0;aER[count] = 0.0;}
	for(int count = 0; count < 8459; count++) {aFL[count] = 0.0;aFR[count] = 0.0;}
	for(int count = 0; count < 4539; count++) {aGL[count] = 0.0;aGR[count] = 0.0;}
	for(int count = 0; count < 3199; count++) {aHL[count] = 0.0;aHR[count] = 0.0;}
	
	for(int count = 0; count < 3110; count++) {aML[count] = aMR[count] = 0.0;}	
	
	feedbackAL = 0.0; feedbackAR = 0.0;
	feedbackBL = 0.0; feedbackBR = 0.0;
	feedbackCL = 0.0; feedbackCR = 0.0;
	feedbackDL = 0.0; feedbackDR = 0.0;
		
	countI = 1;
	countJ = 1;
	countK = 1;
	countL = 1;
	
	countA = 1;
	countB = 1;
	countC = 1;
	countD = 1;	
	
	countE = 1;
	countF = 1;
	countG = 1;
	countH = 1;
	countM = 1;
	//the predelay
	
	vibM = 3.0;
	
	oldfpd = 429496.7295;
	
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
		
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

Galactic3::~Galactic3() {}
VstInt32 Galactic3::getVendorVersion () {return 1000;}
void Galactic3::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Galactic3::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Galactic3::setParameter(VstInt32 index, float value) {
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

float Galactic3::getParameter(VstInt32 index) {
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

void Galactic3::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Replace", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Brightns", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Detune", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Derez", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Bigness", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Galactic3::getParameterDisplay(VstInt32 index, char *text) {
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

void Galactic3::getParameterLabel(VstInt32 index, char *text) {
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

VstInt32 Galactic3::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Galactic3::getEffectName(char* name) {
    vst_strncpy(name, "Galactic3", kVstMaxProductStrLen); return true;
}

VstPlugCategory Galactic3::getPlugCategory() {return kPlugCategEffect;}

bool Galactic3::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Galactic3", kVstMaxProductStrLen); return true;
}

bool Galactic3::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Galactic3::parameterTextToValue(VstInt32 index, const char *text, float &value) {
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
bool Galactic3::canConvertParameterTextToValue(VstInt32 index) {
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
