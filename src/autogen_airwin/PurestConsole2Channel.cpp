/* ========================================
 *  PurestConsole2Channel - PurestConsole2Channel.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestConsole2Channel_H
#include "PurestConsole2Channel.h"
#endif
namespace airwin2rack::PurestConsole2Channel {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new PurestConsole2Channel(audioMaster);}

PurestConsole2Channel::PurestConsole2Channel(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for (int x = 0; x < 15; x++) {biquadA[x] = 0.0;}
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

PurestConsole2Channel::~PurestConsole2Channel() {}
VstInt32 PurestConsole2Channel::getVendorVersion () {return 1000;}
void PurestConsole2Channel::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void PurestConsole2Channel::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

void PurestConsole2Channel::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    }
}

float PurestConsole2Channel::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void PurestConsole2Channel::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void PurestConsole2Channel::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void PurestConsole2Channel::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 PurestConsole2Channel::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool PurestConsole2Channel::getEffectName(char* name) {
    vst_strncpy(name, "PurestConsole2Channel", kVstMaxProductStrLen); return true;
}

VstPlugCategory PurestConsole2Channel::getPlugCategory() {return kPlugCategEffect;}

bool PurestConsole2Channel::getProductString(char* text) {
  	vst_strncpy (text, "airwindows PurestConsole2Channel", kVstMaxProductStrLen); return true;
}

bool PurestConsole2Channel::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool PurestConsole2Channel::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool PurestConsole2Channel::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
