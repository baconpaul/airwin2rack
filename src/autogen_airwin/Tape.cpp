/* ========================================
 *  Tape - Tape.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Tape_H
#include "Tape.h"
#endif
namespace airwinconsolidated::Tape {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Tape(audioMaster);}

Tape::Tape(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	iirMidRollerAL = 0.0;
	iirMidRollerBL = 0.0;
	iirHeadBumpAL = 0.0;
	iirHeadBumpBL = 0.0;
	iirMidRollerAR = 0.0;
	iirMidRollerBR = 0.0;
	iirHeadBumpAR = 0.0;
	iirHeadBumpBR = 0.0;
	for (int x = 0; x < 9; x++) {
		biquadAL[x] = 0.0;biquadBL[x] = 0.0;biquadCL[x] = 0.0;biquadDL[x] = 0.0;
		biquadAR[x] = 0.0;biquadBR[x] = 0.0;biquadCR[x] = 0.0;biquadDR[x] = 0.0;
	}
	flip = false;
	lastSampleL = 0.0;
	lastSampleR = 0.0;	
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

Tape::~Tape() {}
VstInt32 Tape::getVendorVersion () {return 1000;}
void Tape::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Tape::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Tape::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Tape::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Tape::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Slam", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "Bump", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Tape::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A-0.5)*24.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Tape::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Tape::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Tape::getEffectName(char* name) {
    vst_strncpy(name, "Tape", kVstMaxProductStrLen); return true;
}

VstPlugCategory Tape::getPlugCategory() {return kPlugCategEffect;}

bool Tape::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Tape", kVstMaxProductStrLen); return true;
}

bool Tape::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Tape::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value / 24.0) + 0.5; } return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Tape::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;

    }
    return false;
}
} // end namespace
