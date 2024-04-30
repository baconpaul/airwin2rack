/* ========================================
 *  TexturizeMS - TexturizeMS.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TexturizeMS_H
#include "TexturizeMS.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::TexturizeMS {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new TexturizeMS(audioMaster);}

TexturizeMS::TexturizeMS(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	D = 0.5;
	E = 0.5;
	F = 0.5;
	
	polarityM = false;
	lastSampleM = 0.0;
	iirSampleM = 0.0;
	noiseAM = 0.0;
	noiseBM = 0.0;
	noiseCM = 0.0;
	flipM = true;
	polarityS = false;
	lastSampleS = 0.0;
	iirSampleS = 0.0;
	noiseAS = 0.0;
	noiseBS = 0.0;
	noiseCS = 0.0;
	flipS = true;	
	
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

TexturizeMS::~TexturizeMS() {}
VstInt32 TexturizeMS::getVendorVersion () {return 1000;}
void TexturizeMS::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void TexturizeMS::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void TexturizeMS::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
		case kParamF: F = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float TexturizeMS::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
		case kParamF: return F; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void TexturizeMS::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "BrightM", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "PunchyM", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "DryWetM", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "BrightS", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "PunchyS", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "DryWetS", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void TexturizeMS::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
		case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void TexturizeMS::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 TexturizeMS::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool TexturizeMS::getEffectName(char* name) {
    vst_strncpy(name, "TexturizeMS", kVstMaxProductStrLen); return true;
}

VstPlugCategory TexturizeMS::getPlugCategory() {return kPlugCategEffect;}

bool TexturizeMS::getProductString(char* text) {
  	vst_strncpy (text, "airwindows TexturizeMS", kVstMaxProductStrLen); return true;
}

bool TexturizeMS::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool TexturizeMS::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }
    case kParamF: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool TexturizeMS::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;
        case kParamE: return true;
        case kParamF: return true;

    }
    return false;
}
} // end namespace
