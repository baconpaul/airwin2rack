/* ========================================
 *  Baxandall - Baxandall.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Baxandall_H
#include "Baxandall.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::Baxandall {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Baxandall(audioMaster);}

Baxandall::Baxandall(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	for (int x = 0; x < 9; x++) {
		trebleAL[x] = 0.0;
		trebleBL[x] = 0.0;
		bassAL[x] = 0.0;
		bassBL[x] = 0.0;
		trebleAR[x] = 0.0;
		trebleBR[x] = 0.0;
		bassAR[x] = 0.0;
		bassBR[x] = 0.0;
	}
	flip = false;
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

Baxandall::~Baxandall() {}
VstInt32 Baxandall::getVendorVersion () {return 1000;}
void Baxandall::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Baxandall::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Baxandall::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Baxandall::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Baxandall::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Treble", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Bass", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Baxandall::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A*30.0)-15.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B*30.0)-15.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string ((C*30.0)-15.0, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Baxandall::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Baxandall::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Baxandall::getEffectName(char* name) {
    vst_strncpy(name, "Baxandall", kVstMaxProductStrLen); return true;
}

VstPlugCategory Baxandall::getPlugCategory() {return kPlugCategEffect;}

bool Baxandall::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Baxandall", kVstMaxProductStrLen); return true;
}

bool Baxandall::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Baxandall::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value + 15.0) / (30.0); } return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value + 15.0) / (30.0); } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = (value + 15.0) / (30.0); } return b; break; }

    }
    return false;
}
bool Baxandall::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
