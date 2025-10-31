/* ========================================
 *  LRConvolve - LRConvolve.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __LRConvolve_H
#include "LRConvolve.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::LRConvolve {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new LRConvolve(audioMaster);}

LRConvolve::LRConvolve(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
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

LRConvolve::~LRConvolve() {}
VstInt32 LRConvolve::getVendorVersion () {return 1000;}
void LRConvolve::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void LRConvolve::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void LRConvolve::setParameter(VstInt32 index, float value) {
}

float LRConvolve::getParameter(VstInt32 index) {
	return 0.0; //we only need to update the relevant name, this is simple to manage
}

void LRConvolve::getParameterName(VstInt32 index, char *text) {
}

void LRConvolve::getParameterDisplay(VstInt32 index, char *text) {
}

void LRConvolve::getParameterLabel(VstInt32 index, char *text) {
}

VstInt32 LRConvolve::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool LRConvolve::getEffectName(char* name) {
    vst_strncpy(name, "LRConvolve", kVstMaxProductStrLen); return true;
}

VstPlugCategory LRConvolve::getPlugCategory() {return kPlugCategEffect;}

bool LRConvolve::getProductString(char* text) {
  	vst_strncpy (text, "airwindows LRConvolve", kVstMaxProductStrLen); return true;
}

bool LRConvolve::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool LRConvolve::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool LRConvolve::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
