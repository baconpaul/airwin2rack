/* ========================================
 *  DubSub - DubSub.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DubSub_H
#include "DubSub.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::DubSub {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new DubSub(audioMaster);}

DubSub::DubSub(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.9;
	B = 0.5;
	C = 0.74;
	D = 1.0;
	E = 0.95;
	F = 0.5;
	G = 0.2;
	H = 0.2;
	I = 0.5;
	J = 1.0;
	
	WasNegativeL = false;
	SubOctaveL = false;
	WasNegativeR = false;
	SubOctaveR = false;
	flip = false;
	bflip = 0;
	
	iirDriveSampleAL = 0.0;
	iirDriveSampleBL = 0.0;
	iirDriveSampleCL = 0.0;
	iirDriveSampleDL = 0.0;
	iirDriveSampleEL = 0.0;
	iirDriveSampleFL = 0.0;
	iirDriveSampleAR = 0.0;
	iirDriveSampleBR = 0.0;
	iirDriveSampleCR = 0.0;
	iirDriveSampleDR = 0.0;
	iirDriveSampleER = 0.0;
	iirDriveSampleFR = 0.0;
	
	iirHeadBumpAL = 0.0;
	iirHeadBumpBL = 0.0;
	iirHeadBumpCL = 0.0;
	iirHeadBumpAR = 0.0;
	iirHeadBumpBR = 0.0;
	iirHeadBumpCR = 0.0;
	
	iirSubBumpAL = 0.0;
	iirSubBumpBL = 0.0;
	iirSubBumpCL = 0.0;
	iirSubBumpAR = 0.0;
	iirSubBumpBR = 0.0;
	iirSubBumpCR = 0.0;
	
	lastHeadBumpL = 0.0;
	lastSubBumpL = 0.0;
	lastHeadBumpR = 0.0;
	lastSubBumpR = 0.0;
	
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
	
	oscGateL = 1.0;
	oscGateR = 1.0;	
	
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

DubSub::~DubSub() {}
VstInt32 DubSub::getVendorVersion () {return 1000;}
void DubSub::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void DubSub::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void DubSub::setParameter(VstInt32 index, float value) {
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
        case kParamJ: J = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float DubSub::getParameter(VstInt32 index) {
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

void DubSub::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "TGrind", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Grd/Out", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "XOver", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "BsDrive", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "BsVoice", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "BassOut", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "SbDrive", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "SbVoice", kVstMaxParamStrLen); break;
		case kParamI: vst_strncpy (text, "SubOut", kVstMaxParamStrLen); break;
		case kParamJ: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void DubSub::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string ((F*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamG: float2string (G, text, kVstMaxParamStrLen); break;
        case kParamH: float2string (H, text, kVstMaxParamStrLen); break;
        case kParamI: float2string ((I*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamJ: float2string (J, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void DubSub::getParameterLabel(VstInt32 index, char *text) {
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
        case kParamJ: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 DubSub::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool DubSub::getEffectName(char* name) {
    vst_strncpy(name, "DubSub", kVstMaxProductStrLen); return true;
}

VstPlugCategory DubSub::getPlugCategory() {return kPlugCategEffect;}

bool DubSub::getProductString(char* text) {
  	vst_strncpy (text, "airwindows DubSub", kVstMaxProductStrLen); return true;
}

bool DubSub::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool DubSub::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }
    case kParamF: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamG: { auto b = string2float(text, value); return b; break; }
    case kParamH: { auto b = string2float(text, value); return b; break; }
    case kParamI: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamJ: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool DubSub::canConvertParameterTextToValue(VstInt32 index) {
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
