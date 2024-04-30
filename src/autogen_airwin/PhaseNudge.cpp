/* ========================================
 *  PhaseNudge - PhaseNudge.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PhaseNudge_H
#include "PhaseNudge.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::PhaseNudge {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new PhaseNudge(audioMaster);}

PhaseNudge::PhaseNudge(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 1.0;
	for(int count = 0; count < 1502; count++) {dL[count] = 0.0; dR[count] = 0.0;}
	one = 1; maxdelay = 9001;
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

PhaseNudge::~PhaseNudge() {}
VstInt32 PhaseNudge::getVendorVersion () {return 1000;}
void PhaseNudge::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void PhaseNudge::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void PhaseNudge::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break; //percent. Using this value, it'll be 0-100 everywhere
        default: break; // unknown parameter, shouldn't happen!
    }
}

float PhaseNudge::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void PhaseNudge::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Phase Nudge", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void PhaseNudge::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void PhaseNudge::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, " ", kVstMaxParamStrLen); break; //the percent
        default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 PhaseNudge::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool PhaseNudge::getEffectName(char* name) {
    vst_strncpy(name, "PhaseNudge", kVstMaxProductStrLen); return true;
}

VstPlugCategory PhaseNudge::getPlugCategory() {return kPlugCategEffect;}

bool PhaseNudge::getProductString(char* text) {
  	vst_strncpy (text, "airwindows PhaseNudge", kVstMaxProductStrLen); return true;
}

bool PhaseNudge::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool PhaseNudge::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool PhaseNudge::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;

    }
    return false;
}
} // end namespace
