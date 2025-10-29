/* ========================================
 *  Distance3 - Distance3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Distance3_H
#include "Distance3.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::Distance3 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Distance3(audioMaster);}

Distance3::Distance3(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 1.0;
	
	prevresultAL = lastclampAL = clampAL = changeAL = lastAL = 0.0;
	prevresultBL = lastclampBL = clampBL = changeBL = lastBL = 0.0;
	prevresultCL = lastclampCL = clampCL = changeCL = lastCL = 0.0;
	prevresultAR = lastclampAR = clampAR = changeAR = lastAR = 0.0;
	prevresultBR = lastclampBR = clampBR = changeBR = lastBR = 0.0;
	prevresultCR = lastclampCR = clampCR = changeCR = lastCR = 0.0;
	for(int count = 0; count < dscBuf+2; count++) {
		dBaL[count] = 0.0;
		dBbL[count] = 0.0;
		dBcL[count] = 0.0;
		dBaR[count] = 0.0;
		dBbR[count] = 0.0;
		dBcR[count] = 0.0;
	}
	dBaPosL = 0.0;
	dBbPosL = 0.0;
	dBcPosL = 0.0;
	dBaPosR = 0.0;
	dBbPosR = 0.0;
	dBcPosR = 0.0;
	dBaXL = 1;
	dBbXL = 1;
	dBcXL = 1;
	dBaXR = 1;
	dBbXR = 1;
	dBcXR = 1;
	
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

Distance3::~Distance3() {}
VstInt32 Distance3::getVendorVersion () {return 1000;}
void Distance3::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Distance3::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Distance3::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Distance3::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Distance3::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Distance", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Top dB", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Distance3::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A*10.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B*70.0)+70.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Distance3::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "miles", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Distance3::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Distance3::getEffectName(char* name) {
    vst_strncpy(name, "Distance3", kVstMaxProductStrLen); return true;
}

VstPlugCategory Distance3::getPlugCategory() {return kPlugCategEffect;}

bool Distance3::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Distance3", kVstMaxProductStrLen); return true;
}

bool Distance3::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Distance3::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = value / (10.0); } return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value - 70.0) / (70.0); } return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Distance3::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
