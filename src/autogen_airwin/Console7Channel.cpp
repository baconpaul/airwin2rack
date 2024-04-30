/* ========================================
 *  Console7Channel - Console7Channel.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Console7Channel_H
#include "Console7Channel.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Console7Channel {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Console7Channel(audioMaster);}

Console7Channel::Console7Channel(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.772;
	gainchase = -1.0;
	chasespeed = 64.0;
	for (int x = 0; x < 15; x++) {biquadA[x] = 0.0;}
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

Console7Channel::~Console7Channel() {}
VstInt32 Console7Channel::getVendorVersion () {return 1000;}
void Console7Channel::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Console7Channel::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Console7Channel::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

float Console7Channel::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Console7Channel::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Fader", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Console7Channel::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Console7Channel::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Console7Channel::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Console7Channel::getEffectName(char* name) {
    vst_strncpy(name, "Console7Channel", kVstMaxProductStrLen); return true;
}

VstPlugCategory Console7Channel::getPlugCategory() {return kPlugCategEffect;}

bool Console7Channel::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Console7Channel", kVstMaxProductStrLen); return true;
}

bool Console7Channel::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Console7Channel::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Console7Channel::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;

    }
    return false;
}
} // end namespace
