/* ========================================
 *  StudioTan - StudioTan.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __StudioTan_H
#include "StudioTan.h"
#endif
namespace airwinconsolidated::StudioTan {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new StudioTan(audioMaster);}

StudioTan::StudioTan(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;

	bynL[0] = 1000.0;
	bynL[1] = 301.0;
	bynL[2] = 176.0;
	bynL[3] = 125.0;
	bynL[4] = 97.0;
	bynL[5] = 79.0;
	bynL[6] = 67.0;
	bynL[7] = 58.0;
	bynL[8] = 51.0;
	bynL[9] = 46.0;
	bynL[10] = 1000.0;
	noiseShapingL = 0.0;
	lastSampleL = 0.0;
	lastSample2L = 0.0;

	bynR[0] = 1000.0;
	bynR[1] = 301.0;
	bynR[2] = 176.0;
	bynR[3] = 125.0;
	bynR[4] = 97.0;
	bynR[5] = 79.0;
	bynR[6] = 67.0;
	bynR[7] = 58.0;
	bynR[8] = 51.0;
	bynR[9] = 46.0;
	bynR[10] = 1000.0;
	noiseShapingR = 0.0;
	lastSampleR = 0.0;
	lastSample2R = 0.0;
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

StudioTan::~StudioTan() {}
VstInt32 StudioTan::getVendorVersion () {return 1000;}
void StudioTan::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void StudioTan::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void StudioTan::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float StudioTan::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void StudioTan::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Quantzr", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void StudioTan::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: switch((VstInt32)( A * 5.999 )) //0 to almost edge of # of params
		{case 0: vst_strncpy (text, "ST 24", kVstMaxParamStrLen); break;
		 case 1: vst_strncpy (text, "DMT 24", kVstMaxParamStrLen); break;
		 case 2: vst_strncpy (text, "NJAD 24", kVstMaxParamStrLen); break;
		 case 3: vst_strncpy (text, "ST 16", kVstMaxParamStrLen); break;
		 case 4: vst_strncpy (text, "DMT 16", kVstMaxParamStrLen); break;
		 case 5: vst_strncpy (text, "NJAD 16", kVstMaxParamStrLen); break;
		 default: break; // unknown parameter, shouldn't happen!
		} break; //E as example 'popup' parameter with four values  */
			
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void StudioTan::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 StudioTan::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool StudioTan::getEffectName(char* name) {
    vst_strncpy(name, "StudioTan", kVstMaxProductStrLen); return true;
}

VstPlugCategory StudioTan::getPlugCategory() {return kPlugCategEffect;}

bool StudioTan::getProductString(char* text) {
  	vst_strncpy (text, "airwindows StudioTan", kVstMaxProductStrLen); return true;
}

bool StudioTan::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool StudioTan::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool StudioTan::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
