/* ========================================
 *  CloudCoat - CloudCoat.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __CloudCoat_H
#include "CloudCoat.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::CloudCoat {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new CloudCoat(audioMaster);}

CloudCoat::CloudCoat(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 1.0;
	
	for(int count = 0; count < kshortA+2; count++) {aAL[count] = 0.0; aAR[count] = 0.0;}
	for(int count = 0; count < kshortB+2; count++) {aBL[count] = 0.0; aBR[count] = 0.0;}
	for(int count = 0; count < kshortC+2; count++) {aCL[count] = 0.0; aCR[count] = 0.0;}
	for(int count = 0; count < kshortD+2; count++) {aDL[count] = 0.0; aDR[count] = 0.0;}
	for(int count = 0; count < kshortE+2; count++) {aEL[count] = 0.0; aER[count] = 0.0;}
	for(int count = 0; count < kshortF+2; count++) {aFL[count] = 0.0; aFR[count] = 0.0;}
	for(int count = 0; count < kshortG+2; count++) {aGL[count] = 0.0; aGR[count] = 0.0;}
	for(int count = 0; count < kshortH+2; count++) {aHL[count] = 0.0; aHR[count] = 0.0;}
	for(int count = 0; count < kshortI+2; count++) {aIL[count] = 0.0; aIR[count] = 0.0;}
	for(int count = 0; count < kshortJ+2; count++) {aJL[count] = 0.0; aJR[count] = 0.0;}
	for(int count = 0; count < kshortK+2; count++) {aKL[count] = 0.0; aKR[count] = 0.0;}
	for(int count = 0; count < kshortL+2; count++) {aLL[count] = 0.0; aLR[count] = 0.0;}
	for(int count = 0; count < kshortM+2; count++) {aML[count] = 0.0; aMR[count] = 0.0;}
	for(int count = 0; count < kshortN+2; count++) {aNL[count] = 0.0; aNR[count] = 0.0;}
	for(int count = 0; count < kshortO+2; count++) {aOL[count] = 0.0; aOR[count] = 0.0;}
	for(int count = 0; count < kshortP+2; count++) {aPL[count] = 0.0; aPR[count] = 0.0;}
	
	feedbackAL = 0.0;
	feedbackBL = 0.0;
	feedbackCL = 0.0;
	feedbackDL = 0.0;
	
	previousAL = 0.0;
	previousBL = 0.0;
	previousCL = 0.0;
	previousDL = 0.0;
	previousEL = 0.0;
	
	feedbackDR = 0.0;
	feedbackHR = 0.0;
	feedbackLR = 0.0;
	feedbackPR = 0.0;
	
	avgAL = 0.0;
	avgBL = 0.0;
	avgCL = 0.0;
	avgDL = 0.0;
	
	avgDR = 0.0;
	avgHR = 0.0;
	avgLR = 0.0;
	avgPR = 0.0;
	
	previousAR = 0.0;
	previousBR = 0.0;
	previousCR = 0.0;
	previousDR = 0.0;
	previousER = 0.0;
	
	prevMulchAL = 0.0;
	prevMulchAR = 0.0;
	
	tailL = 0.0;
	tailR = 0.0;
	
	for(int count = 0; count < 6; count++) {lastRefL[count] = 0.0; lastRefR[count] = 0.0;}
	
	countAL = 1;
	countBL = 1;
	countCL = 1;
	countDL = 1;	
	countEL = 1;
	countFL = 1;
	countGL = 1;
	countHL = 1;
	countIL = 1;
	countJL = 1;
	countKL = 1;
	countLL = 1;
	countML = 1;
	countNL = 1;
	countOL = 1;
	countPL = 1;
	
	countAR = 1;
	countBR = 1;
	countCR = 1;
	countDR = 1;	
	countER = 1;
	countFR = 1;
	countGR = 1;
	countHR = 1;
	countIR = 1;
	countJR = 1;
	countKR = 1;
	countLR = 1;
	countMR = 1;
	countNR = 1;
	countOR = 1;
	countPR = 1;
	
	cycle = 0;
	
	shortA = 336;
	shortB = 1660;
	shortC = 386;
	shortD = 623;
	shortE = 693;
	shortF = 1079;
	shortG = 891;
	shortH = 1574;
	shortI = 24;
	shortJ = 2641;
	shortK = 1239;
	shortL = 775;
	shortM = 11;
	shortN = 3104;
	shortO = 55;
	shortP = 2366;
	prevcloudcoat = -1;
		
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

CloudCoat::~CloudCoat() {}
VstInt32 CloudCoat::getVendorVersion () {return 1000;}
void CloudCoat::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void CloudCoat::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void CloudCoat::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float CloudCoat::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void CloudCoat::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Select", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Sustain", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void CloudCoat::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: int2string ((VstInt32)( A * 16.999 ), text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void CloudCoat::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
 		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 CloudCoat::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool CloudCoat::getEffectName(char* name) {
    vst_strncpy(name, "CloudCoat", kVstMaxProductStrLen); return true;
}

VstPlugCategory CloudCoat::getPlugCategory() {return kPlugCategEffect;}

bool CloudCoat::getProductString(char* text) {
  	vst_strncpy (text, "airwindows CloudCoat", kVstMaxProductStrLen); return true;
}

bool CloudCoat::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool CloudCoat::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool CloudCoat::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
