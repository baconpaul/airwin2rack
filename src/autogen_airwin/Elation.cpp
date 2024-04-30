/* ========================================
 *  Elation - Elation.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Elation_H
#include "Elation.h"
#endif
namespace airwinconsolidated::Elation {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Elation(audioMaster);}

Elation::Elation(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 0.3333333;
	C = 0.3333333;
	D = 1.0;
	for(int count = 0; count < 34; count++) {bR[count] = 0;bL[count] = 0;}
	compAL = 1.0; compBL = 1.0; previousL = 0.0;
	compCL = 1.0; compDL = 1.0; previousBL = 0.0;
	compAR = 1.0; compBR = 1.0; previousR = 0.0;
	compCR = 1.0; compDR = 1.0; previousBR = 0.0;
	flip = false;
	lastSampleR = 0.0;lastSampleL = 0.0;
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

Elation::~Elation() {}
VstInt32 Elation::getVendorVersion () {return 1000;}
void Elation::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Elation::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Elation::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Elation::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Elation::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Persnlty", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Drive", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Elation::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B*3.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C*3.0, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Elation::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Elation::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Elation::getEffectName(char* name) {
    vst_strncpy(name, "Elation", kVstMaxProductStrLen); return true;
}

VstPlugCategory Elation::getPlugCategory() {return kPlugCategEffect;}

bool Elation::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Elation", kVstMaxProductStrLen); return true;
}

bool Elation::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Elation::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = value / (3.0); } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = value / (3.0); } return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Elation::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;

    }
    return false;
}
} // end namespace
