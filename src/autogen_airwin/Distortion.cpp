/* ========================================
 *  Distortion - Distortion.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Distortion_H
#include "Distortion.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Distortion {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Distortion(audioMaster);}

Distortion::Distortion(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	D = 1.0;
	for (int x = 0; x < 9; x++) {
		previousInL[x] = 0.0; previousOutL[x] = 0.0;
		previousInR[x] = 0.0; previousOutR[x] = 0.0;
	}
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

Distortion::~Distortion() {}
VstInt32 Distortion::getVendorVersion () {return 1000;}
void Distortion::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Distortion::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Distortion::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Distortion::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Distortion::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Input", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Mode", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Distortion::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A-0.5)*24.0, text, kVstMaxParamStrLen); break;
        case kParamB:  switch((VstInt32)( B * 4.999 )) //0 to almost edge of # of params
		{
			case 0: vst_strncpy (text, "Density", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "Drive", kVstMaxParamStrLen); break;
			case 2: vst_strncpy (text, "Spiral", kVstMaxParamStrLen); break;
			case 3: vst_strncpy (text, "Mojo", kVstMaxParamStrLen); break;
			case 4: vst_strncpy (text, "Dyno", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break;
        case kParamC: float2string ((C-0.5)*24.0, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Distortion::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Distortion::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Distortion::getEffectName(char* name) {
    vst_strncpy(name, "Distortion", kVstMaxProductStrLen); return true;
}

VstPlugCategory Distortion::getPlugCategory() {return kPlugCategEffect;}

bool Distortion::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Distortion", kVstMaxProductStrLen); return true;
}

bool Distortion::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Distortion::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value / 24.0) + 0.5; } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = (value / 24.0) + 0.5; } return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Distortion::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamC: return true;
        case kParamD: return true;

    }
    return false;
}
} // end namespace
