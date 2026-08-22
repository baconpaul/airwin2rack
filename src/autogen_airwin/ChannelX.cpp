/* ========================================
 *  ChannelX - ChannelX.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ChannelX_H
#include "ChannelX.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::ChannelX {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ChannelX(audioMaster);}

ChannelX::ChannelX(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.0;
	C = 1.0;
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	rbSampleLA = 0.0;
	rbSampleRA = 0.0;
	rbSampleLB = 0.0;
	rbSampleRB = 0.0;
	lastSampleLA = lastSampleLB = lastSampleLC = 0.0;
	lastSampleRA = lastSampleRB = lastSampleRC = 0.0;
	flip = false;
	for (int x = 0; x < bip_total; x++) bip[x] = 0.0;
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

ChannelX::~ChannelX() {}
VstInt32 ChannelX::getVendorVersion () {return 1000;}
void ChannelX::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ChannelX::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void ChannelX::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float ChannelX::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ChannelX::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Console Type", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Drive", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ChannelX::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: switch((VstInt32)( A * 4.999 )) //0 to almost edge of # of params
		{	case 0: vst_strncpy (text, "Neve", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "API", kVstMaxParamStrLen); break;
			case 2: vst_strncpy (text, "SSL", kVstMaxParamStrLen); break;
			case 3: vst_strncpy (text, "Teac", kVstMaxParamStrLen); break;
			case 4: vst_strncpy (text, "Mackie", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break; //completed consoletype 'popup' parameter, exit
		case kParamB: int2string ((VstInt32)(B*200), text, kVstMaxParamStrLen); break;
		case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ChannelX::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "%", kVstMaxParamStrLen); break; //the percent
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break; //the percent
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ChannelX::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ChannelX::getEffectName(char* name) {
    vst_strncpy(name, "ChannelX", kVstMaxProductStrLen); return true;
}

VstPlugCategory ChannelX::getPlugCategory() {return kPlugCategEffect;}

bool ChannelX::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ChannelX", kVstMaxProductStrLen); return true;
}

bool ChannelX::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool ChannelX::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value + 0.1) / 200; } return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool ChannelX::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
