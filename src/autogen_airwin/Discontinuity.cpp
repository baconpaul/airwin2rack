/* ========================================
 *  Discontinuity - Discontinuity.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Discontinuity_H
#include "Discontinuity.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Discontinuity {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Discontinuity(audioMaster);}

Discontinuity::Discontinuity(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	for(int count = 0; count < dscBuf+2; count++) {
		dBaL[count] = 0.0;
		dBbL[count] = 0.0;
		dBcL[count] = 0.0;
		dBaR[count] = 0.0;
		dBbR[count] = 0.0;
		dBcR[count] = 0.0;
	}
	dBaPosL = 0.0;
	dBbPosL = 0.0;
	dBcPosL = 0.0;
	dBaPosR = 0.0;
	dBbPosR = 0.0;
	dBcPosR = 0.0;
	dBaXL = 1;
	dBbXL = 1;
	dBcXL = 1;
	dBaXR = 1;
	dBbXR = 1;
	dBcXR = 1;
	
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

Discontinuity::~Discontinuity() {}
VstInt32 Discontinuity::getVendorVersion () {return 1000;}
void Discontinuity::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Discontinuity::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Discontinuity::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Discontinuity::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Discontinuity::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Top dB", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Discontinuity::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A*70.0)+70.0, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Discontinuity::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Discontinuity::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Discontinuity::getEffectName(char* name) {
    vst_strncpy(name, "Discontinuity", kVstMaxProductStrLen); return true;
}

VstPlugCategory Discontinuity::getPlugCategory() {return kPlugCategEffect;}

bool Discontinuity::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Discontinuity", kVstMaxProductStrLen); return true;
}

bool Discontinuity::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Discontinuity::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value - 70.0) / (70.0); } return b; break; }

    }
    return false;
}
bool Discontinuity::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;

    }
    return false;
}
} // end namespace
