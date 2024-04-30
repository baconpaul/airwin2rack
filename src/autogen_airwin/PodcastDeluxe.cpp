/* ========================================
 *  PodcastDeluxe - PodcastDeluxe.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PodcastDeluxe_H
#include "PodcastDeluxe.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::PodcastDeluxe {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new PodcastDeluxe(audioMaster);}

PodcastDeluxe::PodcastDeluxe(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	
	for(int count = 0; count < 502; count++) {
		d1L[count] = 0.0;
		d2L[count] = 0.0;
		d3L[count] = 0.0;
		d4L[count] = 0.0;
		d5L[count] = 0.0;
		d1R[count] = 0.0;
		d2R[count] = 0.0;
		d3R[count] = 0.0;
		d4R[count] = 0.0;
		d5R[count] = 0.0;
	}
	c1L = 2.0; c2L = 2.0; c3L = 2.0; c4L = 2.0; c5L = 2.0; //startup comp gains	
	lastSampleL = lastOutSampleL = lastSampleR = lastOutSampleR = 0.0;
	tap1 = 1; tap2 = 1; tap3 = 1; tap4 = 1; tap5 = 1;
	maxdelay1 = 9001; maxdelay2 = 9001; maxdelay3 = 9001; maxdelay4 = 9001; maxdelay5 = 9001;
	c1R = 2.0; c2R = 2.0; c3R = 2.0; c4R = 2.0; c5R = 2.0; //startup comp gains	
	
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

PodcastDeluxe::~PodcastDeluxe() {}
VstInt32 PodcastDeluxe::getVendorVersion () {return 1000;}
void PodcastDeluxe::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void PodcastDeluxe::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void PodcastDeluxe::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float PodcastDeluxe::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void PodcastDeluxe::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Boost", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void PodcastDeluxe::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void PodcastDeluxe::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 PodcastDeluxe::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool PodcastDeluxe::getEffectName(char* name) {
    vst_strncpy(name, "PodcastDeluxe", kVstMaxProductStrLen); return true;
}

VstPlugCategory PodcastDeluxe::getPlugCategory() {return kPlugCategEffect;}

bool PodcastDeluxe::getProductString(char* text) {
  	vst_strncpy (text, "airwindows PodcastDeluxe", kVstMaxProductStrLen); return true;
}

bool PodcastDeluxe::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool PodcastDeluxe::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool PodcastDeluxe::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;

    }
    return false;
}
} // end namespace
