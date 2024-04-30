/* ========================================
 *  CrunchyGrooveWear - CrunchyGrooveWear.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __CrunchyGrooveWear_H
#include "CrunchyGrooveWear.h"
#endif
namespace airwinconsolidated::CrunchyGrooveWear {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new CrunchyGrooveWear(audioMaster);}

CrunchyGrooveWear::CrunchyGrooveWear(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.064;
	B = 1.0;
	
	for(int count = 0; count < 21; count++) {
		aMidL[count] = 0.0;
		bMidL[count] = 0.0;
		cMidL[count] = 0.0;
		dMidL[count] = 0.0;
		aMidR[count] = 0.0;
		bMidR[count] = 0.0;
		cMidR[count] = 0.0;
		dMidR[count] = 0.0;
		fMid[count] = 0.0;
	}
	aMidPrevL = 0.0;
	bMidPrevL = 0.0;
	cMidPrevL = 0.0;
	dMidPrevL = 0.0;
	
	aMidPrevR = 0.0;
	bMidPrevR = 0.0;
	cMidPrevR = 0.0;
	dMidPrevR = 0.0;
	
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

CrunchyGrooveWear::~CrunchyGrooveWear() {}
VstInt32 CrunchyGrooveWear::getVendorVersion () {return 1000;}
void CrunchyGrooveWear::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void CrunchyGrooveWear::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void CrunchyGrooveWear::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float CrunchyGrooveWear::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void CrunchyGrooveWear::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Freq", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Apply", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void CrunchyGrooveWear::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void CrunchyGrooveWear::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 CrunchyGrooveWear::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool CrunchyGrooveWear::getEffectName(char* name) {
    vst_strncpy(name, "CrunchyGrooveWear", kVstMaxProductStrLen); return true;
}

VstPlugCategory CrunchyGrooveWear::getPlugCategory() {return kPlugCategEffect;}

bool CrunchyGrooveWear::getProductString(char* text) {
  	vst_strncpy (text, "airwindows CrunchyGrooveWear", kVstMaxProductStrLen); return true;
}

bool CrunchyGrooveWear::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool CrunchyGrooveWear::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool CrunchyGrooveWear::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;

    }
    return false;
}
} // end namespace
