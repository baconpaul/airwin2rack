/* ========================================
 *  Focus - Focus.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Focus_H
#include "Focus.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Focus {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Focus(audioMaster);}

Focus::Focus(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.5;
	C = 0.5;
	D = 1.0;
	E = 1.0;
	for (int x = 0; x < 9; x++) {figureL[x] = 0.0;figureR[x] = 0.0;}
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

Focus::~Focus() {}
VstInt32 Focus::getVendorVersion () {return 1000;}
void Focus::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Focus::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Focus::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Focus::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Focus::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Boost", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Focus", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Mode", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Focus::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A*12.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: switch((VstInt32)( C * 4.999 )) //0 to almost edge of # of params
		{
			case 0: vst_strncpy (text, "Density", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "Drive", kVstMaxParamStrLen); break;
			case 2: vst_strncpy (text, "Spiral", kVstMaxParamStrLen); break;
			case 3: vst_strncpy (text, "Mojo", kVstMaxParamStrLen); break;
			case 4: vst_strncpy (text, "Dyno", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Focus::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Focus::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Focus::getEffectName(char* name) {
    vst_strncpy(name, "Focus", kVstMaxProductStrLen); return true;
}

VstPlugCategory Focus::getPlugCategory() {return kPlugCategEffect;}

bool Focus::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Focus", kVstMaxProductStrLen); return true;
}

bool Focus::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Focus::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = value / (12.0); } return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Focus::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamD: return true;
        case kParamE: return true;

    }
    return false;
}
} // end namespace
