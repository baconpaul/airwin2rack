/* ========================================
 *  ContentHideD - ContentHideD.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ContentHideD_H
#include "ContentHideD.h"
#endif
namespace airwin2rack::ContentHideD {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ContentHideD(audioMaster);}

ContentHideD::ContentHideD(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	demotimer = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
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

ContentHideD::~ContentHideD() {}
VstInt32 ContentHideD::getVendorVersion () {return 1000;}
void ContentHideD::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ContentHideD::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

void ContentHideD::setParameter(VstInt32 index, float value) {
}

float ContentHideD::getParameter(VstInt32 index) {
	return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ContentHideD::getParameterName(VstInt32 index, char *text) {
}

void ContentHideD::getParameterDisplay(VstInt32 index, char *text) {
}

void ContentHideD::getParameterLabel(VstInt32 index, char *text) {
}

VstInt32 ContentHideD::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ContentHideD::getEffectName(char* name) {
    vst_strncpy(name, "ContentHideD", kVstMaxProductStrLen); return true;
}

VstPlugCategory ContentHideD::getPlugCategory() {return kPlugCategEffect;}

bool ContentHideD::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ContentHideD", kVstMaxProductStrLen); return true;
}

bool ContentHideD::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool ContentHideD::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool ContentHideD::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
