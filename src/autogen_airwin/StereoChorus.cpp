/* ========================================
 *  StereoChorus - StereoChorus.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __StereoChorus_H
#include "StereoChorus.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::StereoChorus {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new StereoChorus(audioMaster);}

StereoChorus::StereoChorus(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.0;
	
	for(int count = 0; count < 65535; count++) {pL[count] = 0;pR[count] = 0;}
	sweepL = 3.141592653589793238 / 2.7;
	sweepR = 3.141592653589793238;
	gcount = 0;
	airPrevL = 0.0;
	airEvenL = 0.0;
	airOddL = 0.0;
	airFactorL = 0.0;
	airPrevR = 0.0;
	airEvenR = 0.0;
	airOddR = 0.0;
	airFactorR = 0.0;
	flip = false;
	
	for(int count = 0; count < 6; count++) {lastRefL[count] = 0.0;lastRefR[count] = 0.0;}
	cycle = 0;
	
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

StereoChorus::~StereoChorus() {}
VstInt32 StereoChorus::getVendorVersion () {return 1000;}
void StereoChorus::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void StereoChorus::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void StereoChorus::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float StereoChorus::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void StereoChorus::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Speed", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Depth", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void StereoChorus::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void StereoChorus::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 StereoChorus::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool StereoChorus::getEffectName(char* name) {
    vst_strncpy(name, "StereoChorus", kVstMaxProductStrLen); return true;
}

VstPlugCategory StereoChorus::getPlugCategory() {return kPlugCategEffect;}

bool StereoChorus::getProductString(char* text) {
  	vst_strncpy (text, "airwindows StereoChorus", kVstMaxProductStrLen); return true;
}

bool StereoChorus::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool StereoChorus::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool StereoChorus::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;

    }
    return false;
}
} // end namespace
