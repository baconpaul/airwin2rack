/* ========================================
 *  ToTape5 - ToTape5.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ToTape5_H
#include "ToTape5.h"
#endif
namespace airwinconsolidated::ToTape5 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ToTape5(audioMaster);}

ToTape5::ToTape5(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.25;
	B = 0.0;
	C = 0.0;
	D = 0.0;
	E = 1.0;
	F = 1.0;
	
	iirMidRollerAL = 0.0;
	iirMidRollerBL = 0.0;
	iirMidRollerCL = 0.0;
	iirHeadBumpAL = 0.0;
	iirHeadBumpBL = 0.0;
	iirHeadBumpCL = 0.0;
	iirMinHeadBumpL = 0.0;
	
	iirMidRollerAR = 0.0;
	iirMidRollerBR = 0.0;
	iirMidRollerCR = 0.0;
	iirHeadBumpAR = 0.0;
	iirHeadBumpBR = 0.0;
	iirHeadBumpCR = 0.0;
	iirMinHeadBumpR = 0.0;
	
	iirSampleAL = 0.0;
	iirSampleBL = 0.0;
	iirSampleCL = 0.0;
	iirSampleDL = 0.0;
	iirSampleEL = 0.0;
	iirSampleFL = 0.0;
	iirSampleGL = 0.0;
	iirSampleHL = 0.0;
	iirSampleIL = 0.0;
	iirSampleJL = 0.0;
	iirSampleKL = 0.0;
	iirSampleLL = 0.0;
	iirSampleML = 0.0;
	iirSampleNL = 0.0;
	iirSampleOL = 0.0;
	iirSamplePL = 0.0;
	iirSampleQL = 0.0;
	iirSampleRL = 0.0;
	iirSampleSL = 0.0;
	iirSampleTL = 0.0;
	iirSampleUL = 0.0;
	iirSampleVL = 0.0;
	iirSampleWL = 0.0;
	iirSampleXL = 0.0;
	iirSampleYL = 0.0;
	iirSampleZL = 0.0;
	
	iirSampleAR = 0.0;
	iirSampleBR = 0.0;
	iirSampleCR = 0.0;
	iirSampleDR = 0.0;
	iirSampleER = 0.0;
	iirSampleFR = 0.0;
	iirSampleGR = 0.0;
	iirSampleHR = 0.0;
	iirSampleIR = 0.0;
	iirSampleJR = 0.0;
	iirSampleKR = 0.0;
	iirSampleLR = 0.0;
	iirSampleMR = 0.0;
	iirSampleNR = 0.0;
	iirSampleOR = 0.0;
	iirSamplePR = 0.0;
	iirSampleQR = 0.0;
	iirSampleRR = 0.0;
	iirSampleSR = 0.0;
	iirSampleTR = 0.0;
	iirSampleUR = 0.0;
	iirSampleVR = 0.0;
	iirSampleWR = 0.0;
	iirSampleXR = 0.0;
	iirSampleYR = 0.0;
	iirSampleZR = 0.0;
	
	for (int temp = 0; temp < 999; temp++) {dL[temp] = 0.0; eL[temp] = 0.0; dR[temp] = 0.0; eR[temp] = 0.0;}
	
	gcount = 0;	
	rateof = 0.5;
	sweep = 0.0;
	nextmax = 0.5;
	hcount = 0;	
	flip = 0;
	
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

ToTape5::~ToTape5() {}
VstInt32 ToTape5::getVendorVersion () {return 1000;}
void ToTape5::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ToTape5::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void ToTape5::setParameter(VstInt32 index, float value) {
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

float ToTape5::getParameter(VstInt32 index) {
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

void ToTape5::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Louder", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Softer", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Fatter", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Flutter", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ToTape5::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	}
}

void ToTape5::getParameterLabel(VstInt32 index, char *text) {
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

VstInt32 ToTape5::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ToTape5::getEffectName(char* name) {
    vst_strncpy(name, "ToTape5", kVstMaxProductStrLen); return true;
}

VstPlugCategory ToTape5::getPlugCategory() {return kPlugCategEffect;}

bool ToTape5::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ToTape5", kVstMaxProductStrLen); return true;
}

bool ToTape5::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool ToTape5::parameterTextToValue(VstInt32 index, const char *text, float &value) {
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
bool ToTape5::canConvertParameterTextToValue(VstInt32 index) {
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
