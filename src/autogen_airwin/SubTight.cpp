/* ========================================
 *  SubTight - SubTight.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SubTight_H
#include "SubTight.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::SubTight {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new SubTight(audioMaster);}

SubTight::SubTight(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.3;
	B = 0.5;
	for (int x = 0; x < 21; x++) {
		subL[x] = 0.0;
		subR[x] = 0.0;
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

SubTight::~SubTight() {}
VstInt32 SubTight::getVendorVersion () {return 1000;}
void SubTight::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void SubTight::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void SubTight::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float SubTight::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void SubTight::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Trim", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Steep", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void SubTight::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void SubTight::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 SubTight::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool SubTight::getEffectName(char* name) {
    vst_strncpy(name, "SubTight", kVstMaxProductStrLen); return true;
}

VstPlugCategory SubTight::getPlugCategory() {return kPlugCategEffect;}

bool SubTight::getProductString(char* text) {
  	vst_strncpy (text, "airwindows SubTight", kVstMaxProductStrLen); return true;
}

bool SubTight::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool SubTight::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool SubTight::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;

    }
    return false;
}
} // end namespace
