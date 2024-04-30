/* ========================================
 *  Recurve - Recurve.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Recurve_H
#include "Recurve.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Recurve {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Recurve(audioMaster);}

Recurve::Recurve(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	gain = 2.0;
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

Recurve::~Recurve() {}
VstInt32 Recurve::getVendorVersion () {return 1000;}
void Recurve::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Recurve::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!


void Recurve::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Recurve::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Recurve::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Recurve::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Recurve::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Recurve::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Recurve::getEffectName(char* name) {
    vst_strncpy(name, "Recurve", kVstMaxProductStrLen); return true;
}

VstPlugCategory Recurve::getPlugCategory() {return kPlugCategEffect;}

bool Recurve::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Recurve", kVstMaxProductStrLen); return true;
}

bool Recurve::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Recurve::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool Recurve::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
