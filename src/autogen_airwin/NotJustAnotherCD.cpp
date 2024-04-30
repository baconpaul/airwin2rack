/* ========================================
 *  NotJustAnotherCD - NotJustAnotherCD.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __NotJustAnotherCD_H
#include "NotJustAnotherCD.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::NotJustAnotherCD {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new NotJustAnotherCD(audioMaster);}

NotJustAnotherCD::NotJustAnotherCD(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	bynL[0] = 1000;
	bynL[1] = 301;
	bynL[2] = 176;
	bynL[3] = 125;
	bynL[4] = 97;
	bynL[5] = 79;
	bynL[6] = 67;
	bynL[7] = 58;
	bynL[8] = 51;
	bynL[9] = 46;
	bynL[10] = 1000;
	noiseShapingL = 0.0;
	
	bynR[0] = 1000;
	bynR[1] = 301;
	bynR[2] = 176;
	bynR[3] = 125;
	bynR[4] = 97;
	bynR[5] = 79;
	bynR[6] = 67;
	bynR[7] = 58;
	bynR[8] = 51;
	bynR[9] = 46;
	bynR[10] = 1000;
	noiseShapingR = 0.0;
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

NotJustAnotherCD::~NotJustAnotherCD() {}
VstInt32 NotJustAnotherCD::getVendorVersion () {return 1000;}
void NotJustAnotherCD::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void NotJustAnotherCD::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!


void NotJustAnotherCD::setParameter(VstInt32 index, float value) {
}

float NotJustAnotherCD::getParameter(VstInt32 index) {
	return 0.0; //we only need to update the relevant name, this is simple to manage
}

void NotJustAnotherCD::getParameterName(VstInt32 index, char *text) {
}

void NotJustAnotherCD::getParameterDisplay(VstInt32 index, char *text) {
}

void NotJustAnotherCD::getParameterLabel(VstInt32 index, char *text) {
}

VstInt32 NotJustAnotherCD::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool NotJustAnotherCD::getEffectName(char* name) {
    vst_strncpy(name, "NotJustAnotherCD", kVstMaxProductStrLen); return true;
}

VstPlugCategory NotJustAnotherCD::getPlugCategory() {return kPlugCategEffect;}

bool NotJustAnotherCD::getProductString(char* text) {
  	vst_strncpy (text, "airwindows NotJustAnotherCD", kVstMaxProductStrLen); return true;
}

bool NotJustAnotherCD::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool NotJustAnotherCD::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool NotJustAnotherCD::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
