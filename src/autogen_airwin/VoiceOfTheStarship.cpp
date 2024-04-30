/* ========================================
 *  VoiceOfTheStarship - VoiceOfTheStarship.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __VoiceOfTheStarship_H
#include "VoiceOfTheStarship.h"
#endif
namespace airwinconsolidated::VoiceOfTheStarship {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new VoiceOfTheStarship(audioMaster);}

VoiceOfTheStarship::VoiceOfTheStarship(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.0;
	position = 99999999;
	quadratic = 0;
	noiseAL = 0.0;
	noiseBL = 0.0;
	noiseCL = 0.0;
	flipL = false;
	noiseAR = 0.0;
	noiseBR = 0.0;
	noiseCR = 0.0;
	flipR = false;
	filterflip = false;
	for(int count = 0; count < 11; count++) {bL[count] = 0.0; bR[count] = 0.0; f[count] = 0.0;}
	lastAlgorithm = 0;
	
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

VoiceOfTheStarship::~VoiceOfTheStarship() {}
VstInt32 VoiceOfTheStarship::getVendorVersion () {return 1000;}
void VoiceOfTheStarship::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void VoiceOfTheStarship::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void VoiceOfTheStarship::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float VoiceOfTheStarship::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void VoiceOfTheStarship::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Filter", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Algrthm", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void VoiceOfTheStarship::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (floor(B*16.9), text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void VoiceOfTheStarship::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 VoiceOfTheStarship::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool VoiceOfTheStarship::getEffectName(char* name) {
    vst_strncpy(name, "VoiceOfTheStarship", kVstMaxProductStrLen); return true;
}

VstPlugCategory VoiceOfTheStarship::getPlugCategory() {return kPlugCategEffect;}

bool VoiceOfTheStarship::getProductString(char* text) {
  	vst_strncpy (text, "airwindows VoiceOfTheStarship", kVstMaxProductStrLen); return true;
}

bool VoiceOfTheStarship::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool VoiceOfTheStarship::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool VoiceOfTheStarship::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;

    }
    return false;
}
} // end namespace
