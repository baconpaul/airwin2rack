/* ========================================
 *  LilAmp - LilAmp.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_H
#include "LilAmp.h"
#endif
namespace airwinconsolidated::LilAmp {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new LilAmp(audioMaster);}

LilAmp::LilAmp(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.8;
	D = 1.0;

	lastSampleL = 0.0;
	storeSampleL = 0.0;
	lastSlewL = 0.0;
	iirSampleAL = 0.0;
	iirSampleBL = 0.0;
	iirSampleCL = 0.0;
	iirSampleDL = 0.0;
	iirSampleEL = 0.0;
	OddAL = 0.0;
	OddBL = 0.0;
	OddCL = 0.0;
	OddDL = 0.0;
	OddEL = 0.0;
	EvenAL = 0.0;
	EvenBL = 0.0;
	EvenCL = 0.0;
	EvenDL = 0.0;
	EvenEL = 0.0; //amp

	lastSampleR = 0.0;
	storeSampleR = 0.0;
	lastSlewR = 0.0;
	iirSampleAR = 0.0;
	iirSampleBR = 0.0;
	iirSampleCR = 0.0;
	iirSampleDR = 0.0;
	iirSampleER = 0.0;
	OddAR = 0.0;
	OddBR = 0.0;
	OddCR = 0.0;
	OddDR = 0.0;
	OddER = 0.0;
	EvenAR = 0.0;
	EvenBR = 0.0;
	EvenCR = 0.0;
	EvenDR = 0.0;
	EvenER = 0.0;
	
	flip = false; //amp	
	
	for(int fcount = 0; fcount < 90; fcount++) {
		bL[fcount] = 0;
		bR[fcount] = 0;
	}
	smoothCabAL = 0.0; smoothCabBL = 0.0; lastCabSampleL = 0.0; //cab
	smoothCabAR = 0.0; smoothCabBR = 0.0; lastCabSampleR = 0.0; //cab
	
	for (int fcount = 0; fcount < 9; fcount++) {
		lastRefL[fcount] = 0.0;
		lastRefR[fcount] = 0.0;
	}
	cycle = 0; //undersampling
	
	for (int x = 0; x < fix_total; x++) {
		fixA[x] = 0.0;
		fixB[x] = 0.0;
		fixC[x] = 0.0;
		fixD[x] = 0.0;
		fixE[x] = 0.0;
		fixF[x] = 0.0;
	}	//filtering
		
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

LilAmp::~LilAmp() {}
VstInt32 LilAmp::getVendorVersion () {return 1000;}
void LilAmp::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void LilAmp::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void LilAmp::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float LilAmp::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void LilAmp::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Gain", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Tone", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void LilAmp::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void LilAmp::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 LilAmp::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool LilAmp::getEffectName(char* name) {
    vst_strncpy(name, "LilAmp", kVstMaxProductStrLen); return true;
}

VstPlugCategory LilAmp::getPlugCategory() {return kPlugCategEffect;}

bool LilAmp::getProductString(char* text) {
  	vst_strncpy (text, "airwindows LilAmp", kVstMaxProductStrLen); return true;
}

bool LilAmp::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool LilAmp::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool LilAmp::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;

    }
    return false;
}
} // end namespace
