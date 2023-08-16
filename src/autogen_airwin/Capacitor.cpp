/* ========================================
 *  Capacitor - Capacitor.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Capacitor_H
#include "Capacitor.h"
#endif
namespace airwin2rack::Capacitor {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Capacitor(audioMaster);}

Capacitor::Capacitor(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 0.0;
	C = 1.0;
	iirHighpassAL = 0.0;
	iirHighpassBL = 0.0;
	iirHighpassCL = 0.0;
	iirHighpassDL = 0.0;
	iirHighpassEL = 0.0;
	iirHighpassFL = 0.0;
	iirLowpassAL = 0.0;
	iirLowpassBL = 0.0;
	iirLowpassCL = 0.0;
	iirLowpassDL = 0.0;
	iirLowpassEL = 0.0;
	iirLowpassFL = 0.0;

	iirHighpassAR = 0.0;
	iirHighpassBR = 0.0;
	iirHighpassCR = 0.0;
	iirHighpassDR = 0.0;
	iirHighpassER = 0.0;
	iirHighpassFR = 0.0;
	iirLowpassAR = 0.0;
	iirLowpassBR = 0.0;
	iirLowpassCR = 0.0;
	iirLowpassDR = 0.0;
	iirLowpassER = 0.0;
	iirLowpassFR = 0.0;
	count = 0;
	lowpassChase = 0.0;
	highpassChase = 0.0;
	wetChase = 0.0;
	lowpassAmount = 1.0;
	highpassAmount = 0.0;
	wet = 1.0;
	lastLowpass = 1000.0;
	lastHighpass = 1000.0;
	lastWet = 1000.0;
	
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

Capacitor::~Capacitor() {}
VstInt32 Capacitor::getVendorVersion () {return 1000;}
void Capacitor::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Capacitor::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Capacitor::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break; //percent. Using this value, it'll be 0-100 everywhere
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
	//we can also set other defaults here, and do calculations that only have to happen
	//once when parameters actually change. Here is the 'popup' setting its (global) values.
	//variables can also be set in the processreplacing loop, and there they'll be set every buffersize
	//here they're set when a parameter's actually changed, which should be less frequent, but
	//you must use global variables in the Capacitor.h file to do it.
}

float Capacitor::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Capacitor::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Lowpass", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Highpass", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Capacitor::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break; //also display 0-1 as percent
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;		
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Capacitor::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, " ", kVstMaxParamStrLen); break; //the percent
        case kParamC: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Capacitor::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Capacitor::getEffectName(char* name) {
    vst_strncpy(name, "Capacitor", kVstMaxProductStrLen); return true;
}

VstPlugCategory Capacitor::getPlugCategory() {return kPlugCategEffect;}

bool Capacitor::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Capacitor", kVstMaxProductStrLen); return true;
}

bool Capacitor::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Capacitor::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Capacitor::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
