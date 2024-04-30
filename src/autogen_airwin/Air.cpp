/* ========================================
 *  Air - Air.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Air_H
#include "Air.h"
#endif
namespace airwinconsolidated::Air {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Air(audioMaster);}

Air::Air(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	airPrevAL = 0.0;
	airEvenAL = 0.0;
	airOddAL = 0.0;
	airFactorAL = 0.0;
	airPrevBL = 0.0;
	airEvenBL = 0.0;
	airOddBL = 0.0;
	airFactorBL = 0.0;
	airPrevCL = 0.0;
	airEvenCL = 0.0;
	airOddCL = 0.0;
	airFactorCL = 0.0;
	tripletPrevL = 0.0;
	tripletMidL = 0.0;
	tripletAL = 0.0;
	tripletBL = 0.0;
	tripletCL = 0.0;
	tripletFactorL = 0.0;

	airPrevAR = 0.0;
	airEvenAR = 0.0;
	airOddAR = 0.0;
	airFactorAR = 0.0;
	airPrevBR = 0.0;
	airEvenBR = 0.0;
	airOddBR = 0.0;
	airFactorBR = 0.0;
	airPrevCR = 0.0;
	airEvenCR = 0.0;
	airOddCR = 0.0;
	airFactorCR = 0.0;
	tripletPrevR = 0.0;
	tripletMidR = 0.0;
	tripletAR = 0.0;
	tripletBR = 0.0;
	tripletCR = 0.0;
	tripletFactorR = 0.0;
	
	flipA = false;
	flipB = false;
	flop = false;
	count = 1;

	A = 0.5;
	B = 0.5;
	C = 0.5;
	D = 0.0;
	E = 1.0;
	F = 1.0;
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

Air::~Air() {}
VstInt32 Air::getVendorVersion () {return 1000;}
void Air::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Air::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Air::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break; //percent. Using this value, it'll be 0-100 everywhere
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Air::getParameter(VstInt32 index) {
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

void Air::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "22K tap", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "15K tap", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "11K tap", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "filters Q", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Output Level", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Air::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string ((C*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;		
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;		
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;		
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Air::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Air::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Air::getEffectName(char* name) {
    vst_strncpy(name, "Air", kVstMaxProductStrLen); return true;
}

VstPlugCategory Air::getPlugCategory() {return kPlugCategEffect;}

bool Air::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Air", kVstMaxProductStrLen); return true;
}

bool Air::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Air::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }
    case kParamF: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Air::canConvertParameterTextToValue(VstInt32 index) {
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
