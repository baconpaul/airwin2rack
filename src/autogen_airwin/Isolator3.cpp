/* ========================================
 *  Isolator3 - Isolator3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Isolator3_H
#include "Isolator3.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::Isolator3 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Isolator3(audioMaster);}

Isolator3::Isolator3(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.0;
	
	for (int x = 0; x < biq_total; x++) {
		biquadA[x] = 0.0;
		biquadB[x] = 0.0;
		biquadC[x] = 0.0;
		biquadD[x] = 0.0;
		biquadE[x] = 0.0;
		biquadF[x] = 0.0;
		biquadG[x] = 0.0;
		hiquadA[x] = 0.0;
		hiquadB[x] = 0.0;
		hiquadC[x] = 0.0;
		hiquadD[x] = 0.0;
		hiquadE[x] = 0.0;
		hiquadF[x] = 0.0;
		hiquadG[x] = 0.0;
	}
	
	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	lastSampleR = 0.0;
	wasPosClipR = false;
	wasNegClipR = false;
	for (int x = 0; x < 16; x++) {intermediateL[x] = 0.0;intermediateR[x] = 0.0;}
	
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

Isolator3::~Isolator3() {}
VstInt32 Isolator3::getVendorVersion () {return 1000;}
void Isolator3::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Isolator3::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Isolator3::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Isolator3::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Isolator3::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Iso", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Q", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Isolator3::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Isolator3::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Isolator3::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Isolator3::getEffectName(char* name) {
    vst_strncpy(name, "Isolator3", kVstMaxProductStrLen); return true;
}

VstPlugCategory Isolator3::getPlugCategory() {return kPlugCategEffect;}

bool Isolator3::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Isolator3", kVstMaxProductStrLen); return true;
}

bool Isolator3::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Isolator3::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Isolator3::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;

    }
    return false;
}
} // end namespace
