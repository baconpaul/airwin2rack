/* ========================================
 *  PurestConsoleChannel - PurestConsoleChannel.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestConsoleChannel_H
#include "PurestConsoleChannel.h"
#endif
namespace airwinconsolidated::PurestConsoleChannel {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new PurestConsoleChannel(audioMaster);}

PurestConsoleChannel::PurestConsoleChannel(audioMasterCallback audioMaster) :
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

PurestConsoleChannel::~PurestConsoleChannel() {}
VstInt32 PurestConsoleChannel::getVendorVersion () {return 1000;}
void PurestConsoleChannel::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void PurestConsoleChannel::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

void PurestConsoleChannel::setParameter(VstInt32 index, float value) {
    switch (index) {
       default: break; // unknown parameter, shouldn't happen!
    }
}

float PurestConsoleChannel::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void PurestConsoleChannel::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void PurestConsoleChannel::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void PurestConsoleChannel::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 PurestConsoleChannel::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool PurestConsoleChannel::getEffectName(char* name) {
    vst_strncpy(name, "PurestConsoleChannel", kVstMaxProductStrLen); return true;
}

VstPlugCategory PurestConsoleChannel::getPlugCategory() {return kPlugCategEffect;}

bool PurestConsoleChannel::getProductString(char* text) {
  	vst_strncpy (text, "airwindows PurestConsoleChannel", kVstMaxProductStrLen); return true;
}

bool PurestConsoleChannel::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool PurestConsoleChannel::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool PurestConsoleChannel::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
