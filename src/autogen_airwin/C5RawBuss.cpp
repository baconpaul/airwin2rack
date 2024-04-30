/* ========================================
 *  C5RawBuss - C5RawBuss.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __C5RawBuss_H
#include "C5RawBuss.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::C5RawBuss {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new C5RawBuss(audioMaster);}

C5RawBuss::C5RawBuss(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	lastFXBussL = 0.0;
	lastSampleBussL = 0.0;
	lastFXBussR = 0.0;
	lastSampleBussR = 0.0;
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

C5RawBuss::~C5RawBuss() {}
VstInt32 C5RawBuss::getVendorVersion () {return 1000;}
void C5RawBuss::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void C5RawBuss::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void C5RawBuss::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float C5RawBuss::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void C5RawBuss::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Center", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void C5RawBuss::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void C5RawBuss::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 C5RawBuss::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool C5RawBuss::getEffectName(char* name) {
    vst_strncpy(name, "C5RawBuss", kVstMaxProductStrLen); return true;
}

VstPlugCategory C5RawBuss::getPlugCategory() {return kPlugCategEffect;}

bool C5RawBuss::getProductString(char* text) {
  	vst_strncpy (text, "airwindows C5RawBuss", kVstMaxProductStrLen); return true;
}

bool C5RawBuss::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool C5RawBuss::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool C5RawBuss::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;

    }
    return false;
}
} // end namespace
