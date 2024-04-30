/* ========================================
 *  GrindAmp - GrindAmp.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_H
#include "GrindAmp.h"
#endif
namespace airwinconsolidated::GrindAmp {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new GrindAmp(audioMaster);}

GrindAmp::GrindAmp(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.8;
	D = 1.0;

	smoothAL = 0.0;
	smoothBL = 0.0;
	smoothCL = 0.0;
	smoothDL = 0.0;
	smoothEL = 0.0;
	smoothFL = 0.0;
	smoothGL = 0.0;
	smoothHL = 0.0;
	smoothIL = 0.0;
	smoothJL = 0.0;
	smoothKL = 0.0;
	secondAL = 0.0;
	secondBL = 0.0;
	secondCL = 0.0;
	secondDL = 0.0;
	secondEL = 0.0;
	secondFL = 0.0;
	secondGL = 0.0;
	secondHL = 0.0;
	secondIL = 0.0;
	secondJL = 0.0;
	secondKL = 0.0;
	thirdAL = 0.0;
	thirdBL = 0.0;
	thirdCL = 0.0;
	thirdDL = 0.0;
	thirdEL = 0.0;
	thirdFL = 0.0;
	thirdGL = 0.0;
	thirdHL = 0.0;
	thirdIL = 0.0;
	thirdJL = 0.0;
	thirdKL = 0.0;
	iirSampleAL = 0.0;
	iirSampleBL = 0.0;
	iirSampleCL = 0.0;
	iirSampleDL = 0.0;
	iirSampleEL = 0.0;
	iirSampleFL = 0.0;
	iirSampleGL = 0.0;
	iirSampleHL = 0.0;
	iirSampleIL = 0.0;
	iirLowpassL = 0.0;
	iirSubL = 0.0;
	storeSampleL = 0.0; //amp

	smoothAR = 0.0;
	smoothBR = 0.0;
	smoothCR = 0.0;
	smoothDR = 0.0;
	smoothER = 0.0;
	smoothFR = 0.0;
	smoothGR = 0.0;
	smoothHR = 0.0;
	smoothIR = 0.0;
	smoothJR = 0.0;
	smoothKR = 0.0;
	secondAR = 0.0;
	secondBR = 0.0;
	secondCR = 0.0;
	secondDR = 0.0;
	secondER = 0.0;
	secondFR = 0.0;
	secondGR = 0.0;
	secondHR = 0.0;
	secondIR = 0.0;
	secondJR = 0.0;
	secondKR = 0.0;
	thirdAR = 0.0;
	thirdBR = 0.0;
	thirdCR = 0.0;
	thirdDR = 0.0;
	thirdER = 0.0;
	thirdFR = 0.0;
	thirdGR = 0.0;
	thirdHR = 0.0;
	thirdIR = 0.0;
	thirdJR = 0.0;
	thirdKR = 0.0;
	iirSampleAR = 0.0;
	iirSampleBR = 0.0;
	iirSampleCR = 0.0;
	iirSampleDR = 0.0;
	iirSampleER = 0.0;
	iirSampleFR = 0.0;
	iirSampleGR = 0.0;
	iirSampleHR = 0.0;
	iirSampleIR = 0.0;
	iirLowpassR = 0.0;
	iirSubR = 0.0;
	storeSampleR = 0.0; //amp
	
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

GrindAmp::~GrindAmp() {}
VstInt32 GrindAmp::getVendorVersion () {return 1000;}
void GrindAmp::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void GrindAmp::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void GrindAmp::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float GrindAmp::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void GrindAmp::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Gain", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Tone", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void GrindAmp::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void GrindAmp::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 GrindAmp::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool GrindAmp::getEffectName(char* name) {
    vst_strncpy(name, "GrindAmp", kVstMaxProductStrLen); return true;
}

VstPlugCategory GrindAmp::getPlugCategory() {return kPlugCategEffect;}

bool GrindAmp::getProductString(char* text) {
  	vst_strncpy (text, "airwindows GrindAmp", kVstMaxProductStrLen); return true;
}

bool GrindAmp::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool GrindAmp::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool GrindAmp::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;

    }
    return false;
}
} // end namespace
