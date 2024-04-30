/* ========================================
 *  Infinity - Infinity.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Infinity_H
#include "Infinity.h"
#endif
namespace airwinconsolidated::Infinity {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Infinity(audioMaster);}

Infinity::Infinity(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for (int x = 0; x < 11; x++) {biquadA[x] = 0.0; biquadB[x] = 0.0; biquadC[x] = 0.0;}
	
	feedbackAL = feedbackAR = 0.0;
	feedbackBL = feedbackBR = 0.0;
	feedbackCL = feedbackCR = 0.0;
	feedbackDL = feedbackDR = 0.0;
	feedbackEL = feedbackER = 0.0;
	feedbackFL = feedbackFR = 0.0;
	feedbackGL = feedbackGR = 0.0;
	feedbackHL = feedbackHR = 0.0;
	
	int count;
	for(count = 0; count < 8110; count++) {aAL[count] = aAR[count] = 0.0;}
	for(count = 0; count < 7510; count++) {aBL[count] = aBR[count] = 0.0;}
	for(count = 0; count < 7310; count++) {aCL[count] = aCR[count] = 0.0;}
	for(count = 0; count < 6910; count++) {aDL[count] = aDR[count] = 0.0;}
	for(count = 0; count < 6310; count++) {aEL[count] = aER[count] = 0.0;}
	for(count = 0; count < 6110; count++) {aFL[count] = aFR[count] = 0.0;}
	for(count = 0; count < 5510; count++) {aGL[count] = aGR[count] = 0.0;}
	for(count = 0; count < 4910; count++) {aHL[count] = aHR[count] = 0.0;}
	//maximum value needed will be delay * 100, plus 206 (absolute max vibrato depth)
	for(count = 0; count < 4510; count++) {aIL[count] = aIR[count] = 0.0;}
	for(count = 0; count < 4310; count++) {aJL[count] = aJR[count] = 0.0;}
	for(count = 0; count < 3910; count++) {aKL[count] = aKR[count] = 0.0;}
	for(count = 0; count < 3310; count++) {aLL[count] = aLR[count] = 0.0;}
	//maximum value will be delay * 100
	countA = 1; delayA = 79;
	countB = 1; delayB = 73;
	countC = 1; delayC = 71;
	countD = 1; delayD = 67;	
	countE = 1; delayE = 61;
	countF = 1; delayF = 59;
	countG = 1; delayG = 53;
	countH = 1; delayH = 47;
	//the householder matrices
	countI = 1; delayI = 43;
	countJ = 1; delayJ = 41;
	countK = 1; delayK = 37;
	countL = 1; delayL = 31;
	//the allpasses

	A = 1.0;
	B = 0.0;
	C = 0.5;
	D = 1.0;
	
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

Infinity::~Infinity() {}
VstInt32 Infinity::getVendorVersion () {return 1000;}
void Infinity::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Infinity::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Infinity::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Infinity::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Infinity::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Filter", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Damping", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Size", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Infinity::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Infinity::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Infinity::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Infinity::getEffectName(char* name) {
    vst_strncpy(name, "Infinity", kVstMaxProductStrLen); return true;
}

VstPlugCategory Infinity::getPlugCategory() {return kPlugCategEffect;}

bool Infinity::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Infinity", kVstMaxProductStrLen); return true;
}

bool Infinity::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Infinity::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Infinity::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;

    }
    return false;
}
} // end namespace
