/* ========================================
 *  ClipOnly3 - ClipOnly3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ClipOnly3_H
#include "ClipOnly3.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::ClipOnly3 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ClipOnly3(audioMaster);}

ClipOnly3::ClipOnly3(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	lastSampleR = 0.0;
	wasPosClipR = false;
	wasNegClipR = false;
	for (int x = 0; x < 17; x++) {intermediateL[x] = 0.0; intermediateR[x] = 0.0;}
	for (int x = 0; x < 33; x++) {slewL[x] = 0.0; slewR[x] = 0.0;}
	
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

ClipOnly3::~ClipOnly3() {}
VstInt32 ClipOnly3::getVendorVersion () {return 1000;}
void ClipOnly3::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ClipOnly3::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

void ClipOnly3::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    }
}

float ClipOnly3::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ClipOnly3::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ClipOnly3::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ClipOnly3::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ClipOnly3::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ClipOnly3::getEffectName(char* name) {
    vst_strncpy(name, "ClipOnly3", kVstMaxProductStrLen); return true;
}

VstPlugCategory ClipOnly3::getPlugCategory() {return kPlugCategEffect;}

bool ClipOnly3::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ClipOnly3", kVstMaxProductStrLen); return true;
}

bool ClipOnly3::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool ClipOnly3::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool ClipOnly3::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
