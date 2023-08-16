/* ========================================
 *  AverMatrix - AverMatrix.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __AverMatrix_H
#include "AverMatrix.h"
#endif
namespace airwin2rack::AverMatrix {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new AverMatrix(audioMaster);}

AverMatrix::AverMatrix(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.0;
	C = 1.0;
	for(int x = 0; x < 11; x++) {
		f[x] = 0.0;
		for (int y = 0; y < 11; y++) {
			bL[x][y] = 0.0; bR[x][y] = 0.0;
		}
	}

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

AverMatrix::~AverMatrix() {}
VstInt32 AverMatrix::getVendorVersion () {return 1000;}
void AverMatrix::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void AverMatrix::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void AverMatrix::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float AverMatrix::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void AverMatrix::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Average", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Depth", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Inv/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void AverMatrix::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A * 9.0)+1.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B * 9.0)+1.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string ((C * 2.0)-1.0, text, kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void AverMatrix::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "taps", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "poles", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 AverMatrix::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool AverMatrix::getEffectName(char* name) {
    vst_strncpy(name, "AverMatrix", kVstMaxProductStrLen); return true;
}

VstPlugCategory AverMatrix::getPlugCategory() {return kPlugCategEffect;}

bool AverMatrix::getProductString(char* text) {
  	vst_strncpy (text, "airwindows AverMatrix", kVstMaxProductStrLen); return true;
}

bool AverMatrix::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool AverMatrix::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value - 1.0) / (9.0); } return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value - 1.0) / (9.0); } return b; break; }
    case kParamC: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }

    }
    return false;
}
bool AverMatrix::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
