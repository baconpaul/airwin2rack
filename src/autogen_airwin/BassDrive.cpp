/* ========================================
 *  BassDrive - BassDrive.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BassDrive_H
#include "BassDrive.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::BassDrive {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new BassDrive(audioMaster);}

BassDrive::BassDrive(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	D = 0.5;
	E = 0.5;
	for (int fcount = 0; fcount < 7; fcount++)
	{
		presenceInAL[fcount] = 0.0;
		presenceOutAL[fcount] = 0.0;
		highInAL[fcount] = 0.0;
		highOutAL[fcount] = 0.0;
		midInAL[fcount] = 0.0;
		midOutAL[fcount] = 0.0;
		lowInAL[fcount] = 0.0;
		lowOutAL[fcount] = 0.0;
		presenceInBL[fcount] = 0.0;
		presenceOutBL[fcount] = 0.0;
		highInBL[fcount] = 0.0;
		highOutBL[fcount] = 0.0;
		midInBL[fcount] = 0.0;
		midOutBL[fcount] = 0.0;
		lowInBL[fcount] = 0.0;
		lowOutBL[fcount] = 0.0;
		
		presenceInAR[fcount] = 0.0;
		presenceOutAR[fcount] = 0.0;
		highInAR[fcount] = 0.0;
		highOutAR[fcount] = 0.0;
		midInAR[fcount] = 0.0;
		midOutAR[fcount] = 0.0;
		lowInAR[fcount] = 0.0;
		lowOutAR[fcount] = 0.0;
		presenceInBR[fcount] = 0.0;
		presenceOutBR[fcount] = 0.0;
		highInBR[fcount] = 0.0;
		highOutBR[fcount] = 0.0;
		midInBR[fcount] = 0.0;
		midOutBR[fcount] = 0.0;
		lowInBR[fcount] = 0.0;
		lowOutBR[fcount] = 0.0;
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

BassDrive::~BassDrive() {}
VstInt32 BassDrive::getVendorVersion () {return 1000;}
void BassDrive::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void BassDrive::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void BassDrive::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float BassDrive::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void BassDrive::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Presnce", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Mid", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Low", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Drive", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void BassDrive::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void BassDrive::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 BassDrive::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool BassDrive::getEffectName(char* name) {
    vst_strncpy(name, "BassDrive", kVstMaxProductStrLen); return true;
}

VstPlugCategory BassDrive::getPlugCategory() {return kPlugCategEffect;}

bool BassDrive::getProductString(char* text) {
  	vst_strncpy (text, "airwindows BassDrive", kVstMaxProductStrLen); return true;
}

bool BassDrive::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool BassDrive::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool BassDrive::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;
        case kParamE: return true;

    }
    return false;
}
} // end namespace
