/* ========================================
 *  BuildATPDF - BuildATPDF.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BuildATPDF_H
#include "BuildATPDF.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::BuildATPDF {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new BuildATPDF(audioMaster);}

BuildATPDF::BuildATPDF(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	D = 0.5;
	E = 0.5;
	F = 0.5;
	G = 0.5;
	H = 0.5;
	I = 0.5;
	J = 0.5;
	
	for(int count = 0; count < 11; count++) {bL[count] = 0.0; bR[count] = 0.0; f[count] = 0.0;}

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

BuildATPDF::~BuildATPDF() {}
VstInt32 BuildATPDF::getVendorVersion () {return 1000;}
void BuildATPDF::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void BuildATPDF::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void BuildATPDF::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break; //percent. Using this value, it'll be 0-100 everywhere
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        case kParamG: G = value; break;
        case kParamH: H = value; break;
        case kParamI: I = value; break;
        case kParamJ: J = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float BuildATPDF::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        case kParamG: return G; break;
        case kParamH: return H; break;
        case kParamI: return I; break;
        case kParamJ: return J; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void BuildATPDF::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "First", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Second", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Third", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Fourth", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Fifth", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Sixth", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "Seventh", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "Eighth", kVstMaxParamStrLen); break;
		case kParamI: vst_strncpy (text, "Ninth", kVstMaxParamStrLen); break;
		case kParamJ: vst_strncpy (text, "Tenth", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void BuildATPDF::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A*2)-1, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B*2)-1, text, kVstMaxParamStrLen); break;
        case kParamC: float2string ((C*2)-1, text, kVstMaxParamStrLen); break;
        case kParamD: float2string ((D*2)-1, text, kVstMaxParamStrLen); break;
        case kParamE: float2string ((E*2)-1, text, kVstMaxParamStrLen); break;
        case kParamF: float2string ((F*2)-1, text, kVstMaxParamStrLen); break;
        case kParamG: float2string ((G*2)-1, text, kVstMaxParamStrLen); break;
        case kParamH: float2string ((H*2)-1, text, kVstMaxParamStrLen); break;
        case kParamI: float2string ((I*2)-1, text, kVstMaxParamStrLen); break;
        case kParamJ: float2string ((J*2)-1, text, kVstMaxParamStrLen); break;
		
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void BuildATPDF::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamI: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamJ: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 BuildATPDF::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool BuildATPDF::getEffectName(char* name) {
    vst_strncpy(name, "BuildATPDF", kVstMaxProductStrLen); return true;
}

VstPlugCategory BuildATPDF::getPlugCategory() {return kPlugCategEffect;}

bool BuildATPDF::getProductString(char* text) {
  	vst_strncpy (text, "airwindows BuildATPDF", kVstMaxProductStrLen); return true;
}

bool BuildATPDF::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool BuildATPDF::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value + 1) / (2); } return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value + 1) / (2); } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = (value + 1) / (2); } return b; break; }
    case kParamD: { auto b = string2float(text, value); if (b) { value = (value + 1) / (2); } return b; break; }
    case kParamE: { auto b = string2float(text, value); if (b) { value = (value + 1) / (2); } return b; break; }
    case kParamF: { auto b = string2float(text, value); if (b) { value = (value + 1) / (2); } return b; break; }
    case kParamG: { auto b = string2float(text, value); if (b) { value = (value + 1) / (2); } return b; break; }
    case kParamH: { auto b = string2float(text, value); if (b) { value = (value + 1) / (2); } return b; break; }
    case kParamI: { auto b = string2float(text, value); if (b) { value = (value + 1) / (2); } return b; break; }
    case kParamJ: { auto b = string2float(text, value); if (b) { value = (value + 1) / (2); } return b; break; }

    }
    return false;
}
bool BuildATPDF::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;
        case kParamE: return true;
        case kParamF: return true;
        case kParamG: return true;
        case kParamH: return true;
        case kParamI: return true;
        case kParamJ: return true;

    }
    return false;
}
} // end namespace
