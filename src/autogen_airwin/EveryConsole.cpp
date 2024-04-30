/* ========================================
 *  EveryConsole - EveryConsole.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __EveryConsole_H
#include "EveryConsole.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::EveryConsole {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new EveryConsole(audioMaster);}

EveryConsole::EveryConsole(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.5;
	C = 0.5;
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

EveryConsole::~EveryConsole() {}
VstInt32 EveryConsole::getVendorVersion () {return 1000;}
void EveryConsole::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void EveryConsole::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void EveryConsole::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float EveryConsole::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void EveryConsole::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "ConType", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "InTrim", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "OutTrim", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void EveryConsole::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: switch((VstInt32)( A * 11.999 )) //0 to almost edge of # of params
		{case 0: vst_strncpy (text, "Retro Ch", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "Retro Bs", kVstMaxParamStrLen); break;
			case 2: vst_strncpy (text, "Sin() Ch", kVstMaxParamStrLen); break;
			case 3: vst_strncpy (text, "aSin() Bs", kVstMaxParamStrLen); break;
			case 4: vst_strncpy (text, "C6 Ch", kVstMaxParamStrLen); break;
			case 5: vst_strncpy (text, "C6 Bs", kVstMaxParamStrLen); break;
			case 6: vst_strncpy (text, "C7 Ch", kVstMaxParamStrLen); break;
			case 7: vst_strncpy (text, "C7 Bs", kVstMaxParamStrLen); break;
			case 8: vst_strncpy (text, "BShift Ch", kVstMaxParamStrLen); break;
			case 9: vst_strncpy (text, "BShift Bs", kVstMaxParamStrLen); break;
			case 10: vst_strncpy (text, "CZero Ch", kVstMaxParamStrLen); break;
			case 11: vst_strncpy (text, "CZero Bs", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break;
        case kParamB: float2string (B*2.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C*2.0, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void EveryConsole::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 EveryConsole::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool EveryConsole::getEffectName(char* name) {
    vst_strncpy(name, "EveryConsole", kVstMaxProductStrLen); return true;
}

VstPlugCategory EveryConsole::getPlugCategory() {return kPlugCategEffect;}

bool EveryConsole::getProductString(char* text) {
  	vst_strncpy (text, "airwindows EveryConsole", kVstMaxProductStrLen); return true;
}

bool EveryConsole::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool EveryConsole::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamB: { auto b = string2float(text, value); if (b) { value = value / (2.0); } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = value / (2.0); } return b; break; }

    }
    return false;
}
bool EveryConsole::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
