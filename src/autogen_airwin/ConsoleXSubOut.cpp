/* ========================================
 *  ConsoleXSubOut - ConsoleXSubOut.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleXSubOut_H
#include "ConsoleXSubOut.h"
#endif
namespace airwin2rack::ConsoleXSubOut {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ConsoleXSubOut(audioMaster);}

ConsoleXSubOut::ConsoleXSubOut(audioMasterCallback audioMaster) :
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
	
	for (int x = 0; x < biq_total; x++) {biquad[x] = 0.0;}
	for (int x = 0; x < air_total; x++) air[x] = 0.0;
	for (int x = 0; x < kal_total; x++) kal[x] = 0.0;
	
	for(int count = 0; count < 2004; count++) {mpkL[count] = 0.0; mpkR[count] = 0.0;}
	for(int count = 0; count < 65; count++) {f[count] = 0.0;}
	prevfreqMPeak = -1;
	prevamountMPeak = -1;
	mpc = 1;	
	
	for(int count = 0; count < dscBuf+2; count++) {
		dBaL[count] = 0.0;
		dBaR[count] = 0.0;
	}
	dBaPosL = 0.0;
	dBaPosR = 0.0;
	dBaXL = 1;
	dBaXR = 1;
	
	trebleGainA = 1.0; trebleGainB = 1.0;
	midGainA = 1.0; midGainB = 1.0;
	mPeakA = 1.0; mPeakB = 1.0;
	bassGainA = 1.0; bassGainB = 1.0;
	panA = 0.5; panB = 0.5;
	inTrimA = 1.0; inTrimB = 1.0;	
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

ConsoleXSubOut::~ConsoleXSubOut() {}
VstInt32 ConsoleXSubOut::getVendorVersion () {return 1000;}
void ConsoleXSubOut::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ConsoleXSubOut::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void ConsoleXSubOut::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        case kParamG: G = value; break;
        case kParamH: H = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float ConsoleXSubOut::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        case kParamG: return G; break;
        case kParamH: return H; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ConsoleXSubOut::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Air", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Fire", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Stone", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Reso", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Range", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Top dB", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "Pan", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "Fader", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ConsoleXSubOut::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string ((F*70.0)+70.0, text, kVstMaxParamStrLen); break;
        case kParamG: float2string (G, text, kVstMaxParamStrLen); break;
        case kParamH: float2string (H, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ConsoleXSubOut::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ConsoleXSubOut::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ConsoleXSubOut::getEffectName(char* name) {
    vst_strncpy(name, "ConsoleXSubOut", kVstMaxProductStrLen); return true;
}

VstPlugCategory ConsoleXSubOut::getPlugCategory() {return kPlugCategEffect;}

bool ConsoleXSubOut::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ConsoleXSubOut", kVstMaxProductStrLen); return true;
}

bool ConsoleXSubOut::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool ConsoleXSubOut::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }
    case kParamF: { auto b = string2float(text, value); if (b) { value = (value - 70.0) / (70.0); } return b; break; }
    case kParamG: { auto b = string2float(text, value); return b; break; }
    case kParamH: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool ConsoleXSubOut::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;
        case kParamD: return true;
        case kParamE: return true;
        case kParamF: return true;
        case kParamG: return true;
        case kParamH: return true;

    }
    return false;
}
} // end namespace
