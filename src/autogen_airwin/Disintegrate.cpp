/* ========================================
 *  Disintegrate - Disintegrate.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Disintegrate_H
#include "Disintegrate.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::Disintegrate {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Disintegrate(audioMaster);}

Disintegrate::Disintegrate(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.2;
	D = 0.5;
	E = 1.0;
	
	for (int stage = 0; stage < layersMax; stage++) {
		for (int count = 0; count < dscBufMax+2; count++) {
			dBaL[count][stage] = 0.0;
			dBaR[count][stage] = 0.0;
		}
		dBaPosL[stage] = 0.0;
		dBaPosBL[stage] = 0.0;
		dBaXL[stage] = 1;
		dBaPosR[stage] = 0.0;
		dBaPosBR[stage] = 0.0;
		dBaXR[stage] = 1;
	}
	outFilterL = 0.0;
	outFilterR = 0.0;

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

Disintegrate::~Disintegrate() {}
VstInt32 Disintegrate::getVendorVersion () {return 1000;}
void Disintegrate::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Disintegrate::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Disintegrate::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Disintegrate::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Disintegrate::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Top dB", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "BufSize", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Layers", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Filter", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Disintegrate::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (60.0+(A*80.0), text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Disintegrate::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Disintegrate::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Disintegrate::getEffectName(char* name) {
    vst_strncpy(name, "Disintegrate", kVstMaxProductStrLen); return true;
}

VstPlugCategory Disintegrate::getPlugCategory() {return kPlugCategEffect;}

bool Disintegrate::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Disintegrate", kVstMaxProductStrLen); return true;
}

bool Disintegrate::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Disintegrate::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Disintegrate::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;
        case kParamE: return true;

    }
    return false;
}
} // end namespace
