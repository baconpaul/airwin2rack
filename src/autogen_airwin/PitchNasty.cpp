/* ========================================
 *  PitchNasty - PitchNasty.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PitchNasty_H
#include "PitchNasty.h"
#endif
namespace airwinconsolidated::PitchNasty {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new PitchNasty(audioMaster);}

PitchNasty::PitchNasty(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.0;
	C = 0.25;
	D = 0.0;
	E = 1.0;
	F = 1.0;
	
	for (int temp = 0; temp < 10001; temp++) {dL[temp] = 0.0; dR[temp] = 0.0;}
	inCount = 1;	
	outCount = 1.0;
	switchTransition = false;
	switchAmountL = 0.0; switchAmountR = 0.0;
	feedbackSampleL = 0.0; feedbackSampleR = 0.0;
	
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

PitchNasty::~PitchNasty() {}
VstInt32 PitchNasty::getVendorVersion () {return 1000;}
void PitchNasty::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void PitchNasty::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void PitchNasty::setParameter(VstInt32 index, float value) {
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

float PitchNasty::getParameter(VstInt32 index) {
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

void PitchNasty::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Note", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Bend", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Grind", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Feedbck", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void PitchNasty::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: int2string ((int)(A*24)-12, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B*24.0)-12.0, text, kVstMaxParamStrLen); break;
        case kParamC: int2string ((int)(C*72)-36, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void PitchNasty::getParameterLabel(VstInt32 index, char *text) {
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

VstInt32 PitchNasty::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool PitchNasty::getEffectName(char* name) {
    vst_strncpy(name, "PitchNasty", kVstMaxProductStrLen); return true;
}

VstPlugCategory PitchNasty::getPlugCategory() {return kPlugCategEffect;}

bool PitchNasty::getProductString(char* text) {
  	vst_strncpy (text, "airwindows PitchNasty", kVstMaxProductStrLen); return true;
}

bool PitchNasty::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool PitchNasty::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value + 12.0) / (24.0); } return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }
    case kParamF: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool PitchNasty::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamB: return true;
        case kParamD: return true;
        case kParamE: return true;
        case kParamF: return true;

    }
    return false;
}
} // end namespace
