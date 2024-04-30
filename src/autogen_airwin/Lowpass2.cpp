/* ========================================
 *  Lowpass2 - Lowpass2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Lowpass2_H
#include "Lowpass2.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Lowpass2 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Lowpass2(audioMaster);}

Lowpass2::Lowpass2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.5; //-1.0 to 1.0
	C = 0.25; // 0.0 to 4.0
	D = 1.0;
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	
	iirSampleAL = 0.0;
	iirSampleBL = 0.0;
	iirSampleCL = 0.0;
	iirSampleDL = 0.0;
	iirSampleEL = 0.0;
	iirSampleFL = 0.0;
	iirSampleGL = 0.0;
	iirSampleHL = 0.0;

	iirSampleAR = 0.0;
	iirSampleBR = 0.0;
	iirSampleCR = 0.0;
	iirSampleDR = 0.0;
	iirSampleER = 0.0;
	iirSampleFR = 0.0;
	iirSampleGR = 0.0;
	iirSampleHR = 0.0;

	fpFlip = true;
	
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

Lowpass2::~Lowpass2() {}
VstInt32 Lowpass2::getVendorVersion () {return 1000;}
void Lowpass2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Lowpass2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Lowpass2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Lowpass2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Lowpass2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Lowpass", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Sft/Hrd", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Poles", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Lowpass2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C*4.0, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Lowpass2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Lowpass2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Lowpass2::getEffectName(char* name) {
    vst_strncpy(name, "Lowpass2", kVstMaxProductStrLen); return true;
}

VstPlugCategory Lowpass2::getPlugCategory() {return kPlugCategEffect;}

bool Lowpass2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Lowpass2", kVstMaxProductStrLen); return true;
}

bool Lowpass2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Lowpass2::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = value / (4.0); } return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Lowpass2::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;

    }
    return false;
}
} // end namespace
