/* ========================================
 *  AQuickVoiceClip - AQuickVoiceClip.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __AQuickVoiceClip_H
#include "AQuickVoiceClip.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::AQuickVoiceClip {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new AQuickVoiceClip(audioMaster);}

AQuickVoiceClip::AQuickVoiceClip(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.42;
	LataLast6Sample = LataLast5Sample = LataLast4Sample = 0.0;
	LataLast3Sample = LataLast2Sample = LataLast1Sample = 0.0;
	LataHalfwaySample = LataHalfDrySample = LataHalfDiffSample = 0.0;
	LataDrySample = LataDiffSample = LataPrevDiffSample = 0.0;
	
	RataLast6Sample = RataLast5Sample = RataLast4Sample = 0.0;
	RataLast3Sample = RataLast2Sample = RataLast1Sample = 0.0;
	RataHalfwaySample = RataHalfDrySample = RataHalfDiffSample = 0.0;
	RataDrySample = RataDiffSample = RataPrevDiffSample = 0.0;
	
	LlastSample = 0.0;
	LlastOutSample = 0.0;
	LlastOut2Sample = 0.0;
	LlastOut3Sample = 0.0;
	LlpDepth = 0.0;
	Lovershoot = 0.0;
	Loverall = 0;
	LiirSampleA = 0.0;
	LiirSampleB = 0.0;
	LiirSampleC = 0.0;
	LiirSampleD = 0.0;

	RlastSample = 0.0;
	RlastOutSample = 0.0;
	RlastOut2Sample = 0.0;
	RlastOut3Sample = 0.0;
	RlpDepth = 0.0;
	Rovershoot = 0.0;
	Roverall = 0;
	RiirSampleA = 0.0;
	RiirSampleB = 0.0;
	RiirSampleC = 0.0;
	RiirSampleD = 0.0;
	flip = true;
	
	ataK1 = -0.646; //first FIR shaping of interpolated sample, brightens
	ataK2 = 0.311; //second FIR shaping of interpolated sample, thickens
	ataK6 = -0.093; //third FIR shaping of interpolated sample, brings air
	ataK7 = 0.057; //fourth FIR shaping of interpolated sample, thickens
	ataK8 = -0.023; //fifth FIR shaping of interpolated sample, brings air
	ataK3 = 0.114; //add raw to interpolated dry, toughens
	ataK4 = 0.886; //remainder of interpolated dry, adds up to 1.0
	ataK5 = 0.431; //subtract this much prev. diff sample, brightens.  0.431 becomes flat
	
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

AQuickVoiceClip::~AQuickVoiceClip() {}
VstInt32 AQuickVoiceClip::getVendorVersion () {return 1000;}
void AQuickVoiceClip::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void AQuickVoiceClip::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void AQuickVoiceClip::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float AQuickVoiceClip::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void AQuickVoiceClip::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Highpass", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void AQuickVoiceClip::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((pow(A,3)*2070)+30, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void AQuickVoiceClip::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "hz", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 AQuickVoiceClip::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool AQuickVoiceClip::getEffectName(char* name) {
    vst_strncpy(name, "AQuickVoiceClip", kVstMaxProductStrLen); return true;
}

VstPlugCategory AQuickVoiceClip::getPlugCategory() {return kPlugCategEffect;}

bool AQuickVoiceClip::getProductString(char* text) {
  	vst_strncpy (text, "airwindows AQuickVoiceClip", kVstMaxProductStrLen); return true;
}

bool AQuickVoiceClip::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool AQuickVoiceClip::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool AQuickVoiceClip::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
