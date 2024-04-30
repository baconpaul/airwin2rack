/* ========================================
 *  ToVinyl4 - ToVinyl4.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ToVinyl4_H
#include "ToVinyl4.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::ToVinyl4 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ToVinyl4(audioMaster);}

ToVinyl4::ToVinyl4(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for(int count = 0; count < 11; count++) {
		aMid[count] = 0.0;
		bMid[count] = 0.0;
		fMid[count] = 0.0;
		aSide[count] = 0.0;
		bSide[count] = 0.0;
		fSide[count] = 0.0;
	}
	aMidPrev = 0.0; aSidePrev = 0.0;
	bMidPrev = 0.0; bSidePrev = 0.0;
	ataLastOutL = ataLastOutR = 0.0;	
	midSampleA = 0.0;
	midSampleB = 0.0;
	midSampleC = 0.0;
	midSampleD = 0.0;
	midSampleE = 0.0;
	midSampleF = 0.0;
	midSampleG = 0.0;
	midSampleH = 0.0;
	midSampleI = 0.0;
	midSampleJ = 0.0;
	midSampleK = 0.0;
	midSampleL = 0.0;
	midSampleM = 0.0;
	midSampleN = 0.0;
	midSampleO = 0.0;
	midSampleP = 0.0;
	midSampleQ = 0.0;
	midSampleR = 0.0;
	midSampleS = 0.0;
	midSampleT = 0.0;
	midSampleU = 0.0;
	midSampleV = 0.0;
	midSampleW = 0.0;
	midSampleX = 0.0;
	midSampleY = 0.0;
	midSampleZ = 0.0;
	
	sideSampleA = 0.0;
	sideSampleB = 0.0;
	sideSampleC = 0.0;
	sideSampleD = 0.0;
	sideSampleE = 0.0;
	sideSampleF = 0.0;
	sideSampleG = 0.0;
	sideSampleH = 0.0;
	sideSampleI = 0.0;
	sideSampleJ = 0.0;
	sideSampleK = 0.0;
	sideSampleL = 0.0;
	sideSampleM = 0.0;
	sideSampleN = 0.0;
	sideSampleO = 0.0;
	sideSampleP = 0.0;
	sideSampleQ = 0.0;
	sideSampleR = 0.0;
	sideSampleS = 0.0;
	sideSampleT = 0.0;
	sideSampleU = 0.0;
	sideSampleV = 0.0;
	sideSampleW = 0.0;
	sideSampleX = 0.0;
	sideSampleY = 0.0;
	sideSampleZ = 0.0;
	s1L = s2L = s3L = 0.0;
	o1L = o2L = o3L = 0.0;
	m1L = m2L = desL = 0.0;
	s1R = s2R = s3R = 0.0;
	o1R = o2R = o3R = 0.0;
	m1R = m2R = desR = 0.0;

	A = 0.203419;  //22.0 hz = ((A*A)*290)+10  (A*A)*290 = 12   (A*A) = 0.0413793  sqrt() = 0.203419
	B = 0.3424051; //44.0 hz = ((B*B)*290)+10  (B*B)*290 = 34   (B*B) = 0.1172413  sqrt() = 0.3424051
	C = 0.32;
	D = 0.064;
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

ToVinyl4::~ToVinyl4() {}
VstInt32 ToVinyl4::getVendorVersion () {return 1000;}
void ToVinyl4::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ToVinyl4::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void ToVinyl4::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float ToVinyl4::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ToVinyl4::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Mid HiP", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "SideHiP", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "H Limit", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Gv Wear", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ToVinyl4::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (((A*A)*290.0)+10.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (((B*B)*290.0)+10.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ToVinyl4::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "hz", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "hz", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ToVinyl4::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ToVinyl4::getEffectName(char* name) {
    vst_strncpy(name, "ToVinyl4", kVstMaxProductStrLen); return true;
}

VstPlugCategory ToVinyl4::getPlugCategory() {return kPlugCategEffect;}

bool ToVinyl4::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ToVinyl4", kVstMaxProductStrLen); return true;
}

bool ToVinyl4::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool ToVinyl4::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = sqrt(std::max((value - 10.0) / (290.0), 0.)); } return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = sqrt(std::max((value - 10.0) / (290.0), 0.)); } return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool ToVinyl4::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;

    }
    return false;
}
} // end namespace
