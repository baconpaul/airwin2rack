/* ========================================
 *  AtmosphereBuss - AtmosphereBuss.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __AtmosphereBuss_H
#include "AtmosphereBuss.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::AtmosphereBuss {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new AtmosphereBuss(audioMaster);}

AtmosphereBuss::AtmosphereBuss(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;

	gainchase = -90.0;
	settingchase = -90.0;
	chasespeed = 350.0;
	
	fpNShapeL = 0.0;
	lastSampleAL = 0.0;
	lastSampleBL = 0.0;
	lastSampleCL = 0.0;
	lastSampleDL = 0.0;
	lastSampleEL = 0.0;
	lastSampleFL = 0.0;
	lastSampleGL = 0.0;
	lastSampleHL = 0.0;
	lastSampleIL = 0.0;
	lastSampleJL = 0.0;
	lastSampleKL = 0.0;
	lastSampleLL = 0.0;
	lastSampleML = 0.0;

	fpNShapeR = 0.0;
	lastSampleAR = 0.0;
	lastSampleBR = 0.0;
	lastSampleCR = 0.0;
	lastSampleDR = 0.0;
	lastSampleER = 0.0;
	lastSampleFR = 0.0;
	lastSampleGR = 0.0;
	lastSampleHR = 0.0;
	lastSampleIR = 0.0;
	lastSampleJR = 0.0;
	lastSampleKR = 0.0;
	lastSampleLR = 0.0;
	lastSampleMR = 0.0;

	thresholdA = 0.618033988749894;
	thresholdB = 0.679837387624884;
	thresholdC = 0.747821126387373;
	thresholdD = 0.82260323902611;
	thresholdE = 0.904863562928721;
	thresholdF = 0.995349919221593;
	thresholdG = 1.094884911143752;
	thresholdH = 1.204373402258128;
	thresholdI = 1.32481074248394;
	thresholdJ = 1.457291816732335;
	thresholdK = 1.603020998405568;
	thresholdL = 1.763323098246125;
	thresholdM = 1.939655408070737;
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

AtmosphereBuss::~AtmosphereBuss() {}
VstInt32 AtmosphereBuss::getVendorVersion () {return 1000;}
void AtmosphereBuss::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void AtmosphereBuss::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void AtmosphereBuss::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float AtmosphereBuss::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void AtmosphereBuss::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Input", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void AtmosphereBuss::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void AtmosphereBuss::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 AtmosphereBuss::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool AtmosphereBuss::getEffectName(char* name) {
    vst_strncpy(name, "AtmosphereBuss", kVstMaxProductStrLen); return true;
}

VstPlugCategory AtmosphereBuss::getPlugCategory() {return kPlugCategEffect;}

bool AtmosphereBuss::getProductString(char* text) {
  	vst_strncpy (text, "airwindows AtmosphereBuss", kVstMaxProductStrLen); return true;
}

bool AtmosphereBuss::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool AtmosphereBuss::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool AtmosphereBuss::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;

    }
    return false;
}
} // end namespace
