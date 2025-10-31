/* ========================================
 *  StereoEnsemble - StereoEnsemble.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __StereoEnsemble_H
#include "StereoEnsemble.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::StereoEnsemble {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new StereoEnsemble(audioMaster);}

StereoEnsemble::StereoEnsemble(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 1.0;

	for(int count = 0; count < 7490; count++) {dA[count] = 0.0;}
	for(int count = 0; count < 7532; count++) {dB[count] = 0.0;}
	
	for(int count = 0; count < 5788; count++) {dC[count] = 0.0;}
	for(int count = 0; count < 5746; count++) {dD[count] = 0.0;}
	
	for(int count = 0; count < 4840; count++) {dE[count] = 0.0;}
	for(int count = 0; count < 4870; count++) {dF[count] = 0.0;}
	
	for(int count = 0; count < 3118; count++) {dG[count] = 0.0;}
	for(int count = 0; count < 3088; count++) {dH[count] = 0.0;}
	
	for(int count = 0; count < 2212; count++) {dI[count] = 0.0;}
	for(int count = 0; count < 2222; count++) {dJ[count] = 0.0;}
	
	for(int count = 0; count < 1336; count++) {dK[count] = 0.0;}
	for(int count = 0; count < 1330; count++) {dL[count] = 0.0;}
	
	oneA = 1;
	oneB = 1;
	oneC = 1;
	oneD = 1;
	oneE = 1;
	oneF = 1;
	oneG = 1;
	oneH = 1;
	oneI = 1;
	oneJ = 1;
	oneK = 1;
	oneL = 1;
	
	levelA = 0.75;
	levelB = 0.15;
	
	levelC = 0.15;
	levelD = 0.75;
	
	levelE = 0.63;
	levelF = 0.27;
	
	levelG = 0.27;
	levelH = 0.63;
	
	levelI = 0.48;
	levelJ = 0.32;
	
	levelK = 0.32;
	levelL = 0.48;
	
	for(int count = 0; count < 6; count++) {lastRefL[count] = 0.0;lastRefR[count] = 0.0;}
	cycle = 0;
	
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

StereoEnsemble::~StereoEnsemble() {}
VstInt32 StereoEnsemble::getVendorVersion () {return 1000;}
void StereoEnsemble::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void StereoEnsemble::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void StereoEnsemble::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float StereoEnsemble::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void StereoEnsemble::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Depth", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "FXlevel", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void StereoEnsemble::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void StereoEnsemble::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 StereoEnsemble::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool StereoEnsemble::getEffectName(char* name) {
    vst_strncpy(name, "StereoEnsemble", kVstMaxProductStrLen); return true;
}

VstPlugCategory StereoEnsemble::getPlugCategory() {return kPlugCategEffect;}

bool StereoEnsemble::getProductString(char* text) {
  	vst_strncpy (text, "airwindows StereoEnsemble", kVstMaxProductStrLen); return true;
}

bool StereoEnsemble::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool StereoEnsemble::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool StereoEnsemble::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;

    }
    return false;
}
} // end namespace
