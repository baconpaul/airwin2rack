/* ========================================
 *  Channel8 - Channel8.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Channel8_H
#include "Channel8.h"
#endif
namespace airwinconsolidated::Channel8 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Channel8(audioMaster);}

Channel8::Channel8(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	consoletype = 0.0;
	drive = 0.0;
	output = 1.0;
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	iirSampleLA = 0.0;
	iirSampleRA = 0.0;
	iirSampleLB = 0.0;
	iirSampleRB = 0.0;
	lastSampleAL = lastSampleBL = lastSampleCL = 0.0;
	lastSampleAR = lastSampleBR = lastSampleCR = 0.0;
	flip = false;
	iirAmount = 0.005832;
	threshold = 0.33362176; //instantiating with Neve values
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

Channel8::~Channel8() {}
VstInt32 Channel8::getVendorVersion () {return 1000;}
void Channel8::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Channel8::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Channel8::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: consoletype = value; break;
        case kParamB: drive = value; break;
        case kParamC: output = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
	//we can also set other defaults here, and do calculations that only have to happen
	//once when parameters actually change. Here is the 'popup' setting its (global) values.
	//variables can also be set in the processreplacing loop, and there they'll be set every buffersize
	//here they're set when a parameter's actually changed, which should be less frequent, but
	//you must use global variables in the Channel4.h file to do it.
	switch((VstInt32)( consoletype * 2.999 ))
	{  
		case 0: iirAmount = 0.005832; threshold = 0.33362176; break; //Neve
		case 1: iirAmount = 0.004096; threshold = 0.59969536; break; //API
		case 2: iirAmount = 0.004913; threshold = 0.84934656; break; //SSL			
		default: break; //should not happen
	}		
}

float Channel8::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return consoletype; break;
        case kParamB: return drive; break;
        case kParamC: return output; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Channel8::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Console Type", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Drive", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Channel8::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: switch((VstInt32)( consoletype * 2.999 )) //0 to almost edge of # of params
		{	case 0: vst_strncpy (text, "Neve", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "API", kVstMaxParamStrLen); break;
			case 2: vst_strncpy (text, "SSL", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break; //completed consoletype 'popup' parameter, exit
		case kParamB: int2string ((VstInt32)(drive*200), text, kVstMaxParamStrLen); break;
		case kParamC: float2string (output, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Channel8::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "%", kVstMaxParamStrLen); break; //the percent
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break; //the percent
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Channel8::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Channel8::getEffectName(char* name) {
    vst_strncpy(name, "Channel8", kVstMaxProductStrLen); return true;
}

VstPlugCategory Channel8::getPlugCategory() {return kPlugCategEffect;}

bool Channel8::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Channel8", kVstMaxProductStrLen); return true;
}

bool Channel8::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Channel8::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamC: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Channel8::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamC: return true;

    }
    return false;
}
} // end namespace
