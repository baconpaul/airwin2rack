/* ========================================
 *  SampleDelay - SampleDelay.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SampleDelay_H
#include "SampleDelay.h"
#endif
namespace airwinconsolidated::SampleDelay {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new SampleDelay(audioMaster);}

SampleDelay::SampleDelay(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.0;
	C = 0.0;
	D = 1.0;
	
	for(int count = 0; count < 16385; count++) {pL[count] = 0.0;pR[count] = 0.0;}
	gcount = 0;

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

SampleDelay::~SampleDelay() {}
VstInt32 SampleDelay::getVendorVersion () {return 1000;}
void SampleDelay::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void SampleDelay::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void SampleDelay::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float SampleDelay::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void SampleDelay::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "ms", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Samples", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Subsamp", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Inv/Wet", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void SampleDelay::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: int2string (floor(A*100.0), text, kVstMaxParamStrLen); break;
        case kParamB: int2string (floor(B*10.0), text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string ((D*2.0)-1.0, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void SampleDelay::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 SampleDelay::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool SampleDelay::getEffectName(char* name) {
    vst_strncpy(name, "SampleDelay", kVstMaxProductStrLen); return true;
}

VstPlugCategory SampleDelay::getPlugCategory() {return kPlugCategEffect;}

bool SampleDelay::getProductString(char* text) {
  	vst_strncpy (text, "airwindows SampleDelay", kVstMaxProductStrLen); return true;
}

bool SampleDelay::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool SampleDelay::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamC: { auto b = string2float(text, value); return b; break; }
    case kParamD: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }

    }
    return false;
}
bool SampleDelay::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamC: return true;
        case kParamD: return true;

    }
    return false;
}
} // end namespace
