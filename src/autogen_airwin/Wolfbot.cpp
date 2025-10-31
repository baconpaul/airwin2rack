/* ========================================
 *  Wolfbot - Wolfbot.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Wolfbot_H
#include "Wolfbot.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::Wolfbot {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Wolfbot(audioMaster);}

Wolfbot::Wolfbot(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for (int x = 0; x < kal_total; x++) {kHP[x] = 0.0; kLP[x] = 0.0;}

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

Wolfbot::~Wolfbot() {}
VstInt32 Wolfbot::getVendorVersion () {return 1000;}
void Wolfbot::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Wolfbot::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

void Wolfbot::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Wolfbot::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Wolfbot::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Wolfbot::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Wolfbot::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Wolfbot::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Wolfbot::getEffectName(char* name) {
    vst_strncpy(name, "Wolfbot", kVstMaxProductStrLen); return true;
}

VstPlugCategory Wolfbot::getPlugCategory() {return kPlugCategEffect;}

bool Wolfbot::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Wolfbot", kVstMaxProductStrLen); return true;
}

bool Wolfbot::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Wolfbot::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool Wolfbot::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
