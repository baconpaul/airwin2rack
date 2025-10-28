/* ========================================
 *  TapeDelay2 - TapeDelay2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TapeDelay2_H
#include "TapeDelay2.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::TapeDelay2 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new TapeDelay2(audioMaster);}

TapeDelay2::TapeDelay2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 0.0;
	C = 0.5;
	D = 0.0;
	E = 0.0;
	F = 1.0;
	
	for(int x = 0; x < 88210; x++) {dL[x] = 0.0; dR[x] = 0.0;}
	prevSampleL = 0.0; delayL = 0.0; sweepL = 0.0;
	prevSampleR = 0.0; delayR = 0.0; sweepR = 0.0;
	for (int x = 0; x < 9; x++) {
		regenFilterL[x] = 0.0; outFilterL[x] = 0.0; lastRefL[x] = 0.0;
		regenFilterR[x] = 0.0; outFilterR[x] = 0.0; lastRefR[x] = 0.0;
	}
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

TapeDelay2::~TapeDelay2() {}
VstInt32 TapeDelay2::getVendorVersion () {return 1000;}
void TapeDelay2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void TapeDelay2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void TapeDelay2::setParameter(VstInt32 index, float value) {
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

float TapeDelay2::getParameter(VstInt32 index) {
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

void TapeDelay2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Time", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Regen", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Freq", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Reso", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Flutter", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void TapeDelay2::getParameterDisplay(VstInt32 index, char *text) {
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

void TapeDelay2::getParameterLabel(VstInt32 index, char *text) {
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

VstInt32 TapeDelay2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool TapeDelay2::getEffectName(char* name) {
    vst_strncpy(name, "TapeDelay2", kVstMaxProductStrLen); return true;
}

VstPlugCategory TapeDelay2::getPlugCategory() {return kPlugCategEffect;}

bool TapeDelay2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows TapeDelay2", kVstMaxProductStrLen); return true;
}

bool TapeDelay2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool TapeDelay2::parameterTextToValue(VstInt32 index, const char *text, float &value) {
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
bool TapeDelay2::canConvertParameterTextToValue(VstInt32 index) {
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
