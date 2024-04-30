/* ========================================
 *  TransDesk - TransDesk.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TransDesk_H
#include "TransDesk.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::TransDesk {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new TransDesk(audioMaster);}

TransDesk::TransDesk(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for(int count = 0; count < 19; count++) {dL[count] = 0; dR[count] = 0;}
	gcount = 0;
	
	controlL = 0;
	lastSampleL = 0.0;
	lastOutSampleL = 0.0;
	lastSlewL = 0.0;
	
	controlR = 0;
	lastSampleR = 0.0;
	lastOutSampleR = 0.0;
	lastSlewR = 0.0;
	
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

TransDesk::~TransDesk() {}
VstInt32 TransDesk::getVendorVersion () {return 1000;}
void TransDesk::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void TransDesk::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

void TransDesk::setParameter(VstInt32 index, float value) {
}

float TransDesk::getParameter(VstInt32 index) {
	return 0.0; //we only need to update the relevant name, this is simple to manage
}

void TransDesk::getParameterName(VstInt32 index, char *text) {
}

void TransDesk::getParameterDisplay(VstInt32 index, char *text) {
}

void TransDesk::getParameterLabel(VstInt32 index, char *text) {
}

VstInt32 TransDesk::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool TransDesk::getEffectName(char* name) {
    vst_strncpy(name, "TransDesk", kVstMaxProductStrLen); return true;
}

VstPlugCategory TransDesk::getPlugCategory() {return kPlugCategEffect;}

bool TransDesk::getProductString(char* text) {
  	vst_strncpy (text, "airwindows TransDesk", kVstMaxProductStrLen); return true;
}

bool TransDesk::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool TransDesk::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool TransDesk::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
