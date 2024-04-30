/* ========================================
 *  Gringer - Gringer.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gringer_H
#include "Gringer.h"
#endif
namespace airwinconsolidated::Gringer {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Gringer(audioMaster);}

Gringer::Gringer(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for (int x = 0; x < 9; x++) {inbandL[x] = 0.0; outbandL[x] = 0.0; inbandR[x] = 0.0; outbandR[x] = 0.0;}
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

Gringer::~Gringer() {}
VstInt32 Gringer::getVendorVersion () {return 1000;}
void Gringer::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Gringer::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

void Gringer::setParameter(VstInt32 index, float value) {
}

float Gringer::getParameter(VstInt32 index) {
	return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Gringer::getParameterName(VstInt32 index, char *text) {
}

void Gringer::getParameterDisplay(VstInt32 index, char *text) {
}

void Gringer::getParameterLabel(VstInt32 index, char *text) {
}

VstInt32 Gringer::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Gringer::getEffectName(char* name) {
    vst_strncpy(name, "Gringer", kVstMaxProductStrLen); return true;
}

VstPlugCategory Gringer::getPlugCategory() {return kPlugCategEffect;}

bool Gringer::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Gringer", kVstMaxProductStrLen); return true;
}

bool Gringer::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Gringer::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool Gringer::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
