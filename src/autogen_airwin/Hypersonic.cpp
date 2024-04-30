/* ========================================
 *  Hypersonic - Hypersonic.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Hypersonic_H
#include "Hypersonic.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Hypersonic {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Hypersonic(audioMaster);}

Hypersonic::Hypersonic(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for (int x = 0; x < fix_total; x++) {
		fixA[x] = 0.0;
		fixB[x] = 0.0;
		fixC[x] = 0.0;
		fixD[x] = 0.0;
		fixE[x] = 0.0;
		fixF[x] = 0.0;
		fixG[x] = 0.0;
	}
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

Hypersonic::~Hypersonic() {}
VstInt32 Hypersonic::getVendorVersion () {return 1000;}
void Hypersonic::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Hypersonic::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Hypersonic::setParameter(VstInt32 index, float value) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

float Hypersonic::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Hypersonic::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Hypersonic::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Hypersonic::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Hypersonic::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Hypersonic::getEffectName(char* name) {
    vst_strncpy(name, "Hypersonic", kVstMaxProductStrLen); return true;
}

VstPlugCategory Hypersonic::getPlugCategory() {return kPlugCategEffect;}

bool Hypersonic::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Hypersonic", kVstMaxProductStrLen); return true;
}

bool Hypersonic::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Hypersonic::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool Hypersonic::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
