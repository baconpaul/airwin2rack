/* ========================================
 *  Energy - Energy.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Energy_H
#include "Energy.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Energy {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Energy(audioMaster);}

Energy::Energy(audioMasterCallback audioMaster) :
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
	I = 1.0;
	
	duoEvenL = 0.0;
	duoOddL = 0.0;
	duoFactorL = 0.0;
	duoEvenR = 0.0;
	duoOddR = 0.0;
	duoFactorR = 0.0;
	flip = false;
	
	tripletAL = 0.0;
	tripletBL = 0.0;
	tripletCL = 0.0;
	tripletFactorL = 0.0;
	tripletAR = 0.0;
	tripletBR = 0.0;
	tripletCR = 0.0;
	tripletFactorR = 0.0;
	countA = 1;
	
	quadAL = 0.0;
	quadBL = 0.0;
	quadCL = 0.0;
	quadDL = 0.0;
	quadFactorL = 0.0;
	quadAR = 0.0;
	quadBR = 0.0;
	quadCR = 0.0;
	quadDR = 0.0;
	quadFactorR = 0.0;
	countB = 1;
	
	quintAL = 0.0;
	quintBL = 0.0;
	quintCL = 0.0;
	quintDL = 0.0;
	quintEL = 0.0;
	quintFactorL = 0.0;
	quintAR = 0.0;
	quintBR = 0.0;
	quintCR = 0.0;
	quintDR = 0.0;
	quintER = 0.0;
	quintFactorR = 0.0;
	countC = 1;
	
	sextAL = 0.0;
	sextBL = 0.0;
	sextCL = 0.0;
	sextDL = 0.0;
	sextEL = 0.0;
	sextFL = 0.0;
	sextFactorL = 0.0;
	sextAR = 0.0;
	sextBR = 0.0;
	sextCR = 0.0;
	sextDR = 0.0;
	sextER = 0.0;
	sextFR = 0.0;
	sextFactorR = 0.0;
	countD = 1;
	
	septAL = 0.0;
	septBL = 0.0;
	septCL = 0.0;
	septDL = 0.0;
	septEL = 0.0;
	septFL = 0.0;
	septGL = 0.0;
	septFactorL = 0.0;
	septAR = 0.0;
	septBR = 0.0;
	septCR = 0.0;
	septDR = 0.0;
	septER = 0.0;
	septFR = 0.0;
	septGR = 0.0;
	septFactorR = 0.0;
	countE = 1;
	
	octAL = 0.0;
	octBL = 0.0;
	octCL = 0.0;
	octDL = 0.0;
	octEL = 0.0;
	octFL = 0.0;
	octGL = 0.0;
	octHL = 0.0;
	octFactorL = 0.0;
	octAR = 0.0;
	octBR = 0.0;
	octCR = 0.0;
	octDR = 0.0;
	octER = 0.0;
	octFR = 0.0;
	octGR = 0.0;
	octHR = 0.0;
	octFactorR = 0.0;
	countF = 1;
	
	nintAL = 0.0;
	nintBL = 0.0;
	nintCL = 0.0;
	nintDL = 0.0;
	nintEL = 0.0;
	nintFL = 0.0;
	nintGL = 0.0;
	nintHL = 0.0;
	nintIL = 0.0;
	nintFactorL = 0.0;
	nintAR = 0.0;
	nintBR = 0.0;
	nintCR = 0.0;
	nintDR = 0.0;
	nintER = 0.0;
	nintFR = 0.0;
	nintGR = 0.0;
	nintHR = 0.0;
	nintIR = 0.0;
	nintFactorR = 0.0;
	countG = 1;
	
	PrevHL = 0.0;
	PrevGL = 0.0;
	PrevFL = 0.0;
	PrevEL = 0.0;
	PrevDL = 0.0;
	PrevCL = 0.0;
	PrevBL = 0.0;
	PrevAL = 0.0;
	PrevHR = 0.0;
	PrevGR = 0.0;
	PrevFR = 0.0;
	PrevER = 0.0;
	PrevDR = 0.0;
	PrevCR = 0.0;
	PrevBR = 0.0;
	PrevAR = 0.0;
	
	
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

Energy::~Energy() {}
VstInt32 Energy::getVendorVersion () {return 1000;}
void Energy::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Energy::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Energy::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        case kParamG: G = value; break;
        case kParamH: H = value; break;
        case kParamI: I = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Energy::getParameter(VstInt32 index) {
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
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Energy::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Hiss", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Glitter", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Rat", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Fizz", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Scrape", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Chug", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "Yowr", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "Snarl", kVstMaxParamStrLen); break;
		case kParamI: vst_strncpy (text, "I/Dr/Wt", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Energy::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string ((C*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamD: float2string ((D*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamE: float2string ((E*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamF: float2string ((F*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamG: float2string ((G*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamH: float2string ((H*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamI: float2string ((I*2.0)-1.0, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Energy::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamI: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Energy::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Energy::getEffectName(char* name) {
    vst_strncpy(name, "Energy", kVstMaxProductStrLen); return true;
}

VstPlugCategory Energy::getPlugCategory() {return kPlugCategEffect;}

bool Energy::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Energy", kVstMaxProductStrLen); return true;
}

bool Energy::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Energy::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamD: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamE: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamF: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamG: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamH: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamI: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }

    }
    return false;
}
bool Energy::canConvertParameterTextToValue(VstInt32 index) {
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

    }
    return false;
}
} // end namespace
