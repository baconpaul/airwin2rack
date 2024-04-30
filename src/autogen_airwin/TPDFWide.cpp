/* ========================================
 *  TPDFWide - TPDFWide.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TPDFWide_H
#include "TPDFWide.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::TPDFWide {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new TPDFWide(audioMaster);}

TPDFWide::TPDFWide(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 0.0;
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

TPDFWide::~TPDFWide() {}
VstInt32 TPDFWide::getVendorVersion () {return 1000;}
void TPDFWide::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void TPDFWide::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void TPDFWide::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float TPDFWide::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void TPDFWide::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Quant", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "DeRez", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void TPDFWide::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: switch((VstInt32)( A * 1.999 )) //0 to almost edge of # of params
		{	case 0: vst_strncpy (text, "CD 16", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "HD 24", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break; //completed consoletype 'popup' parameter, exit
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void TPDFWide::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 TPDFWide::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool TPDFWide::getEffectName(char* name) {
    vst_strncpy(name, "TPDFWide", kVstMaxProductStrLen); return true;
}

VstPlugCategory TPDFWide::getPlugCategory() {return kPlugCategEffect;}

bool TPDFWide::getProductString(char* text) {
  	vst_strncpy (text, "airwindows TPDFWide", kVstMaxProductStrLen); return true;
}

bool TPDFWide::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool TPDFWide::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamB: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool TPDFWide::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamB: return true;

    }
    return false;
}
} // end namespace
