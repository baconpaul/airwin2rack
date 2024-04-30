/* ========================================
 *  Logical4 - Logical4.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Logical4_H
#include "Logical4.h"
#endif
namespace airwinconsolidated::Logical4 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Logical4(audioMaster);}

Logical4::Logical4(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.2;
	C = 0.19202020202020202;
	D = 0.5;
	E = 1.0;
	
	//begin ButterComps
	controlAposL = 1.0;
	controlAnegL = 1.0;
	controlBposL = 1.0;
	controlBnegL = 1.0;
	targetposL = 1.0;
	targetnegL = 1.0;
	
	controlAposBL = 1.0;
	controlAnegBL = 1.0;
	controlBposBL = 1.0;
	controlBnegBL = 1.0;
	targetposBL = 1.0;
	targetnegBL = 1.0;
	
	controlAposCL = 1.0;
	controlAnegCL = 1.0;
	controlBposCL = 1.0;
	controlBnegCL = 1.0;
	targetposCL = 1.0;
	targetnegCL = 1.0;
	
	avgAL = avgBL = avgCL = avgDL = avgEL = avgFL = 0.0;
	nvgAL = nvgBL = nvgCL = nvgDL = nvgEL = nvgFL = 0.0;
	//end ButterComps
	
	//begin ButterComps
	controlAposR = 1.0;
	controlAnegR = 1.0;
	controlBposR = 1.0;
	controlBnegR = 1.0;
	targetposR = 1.0;
	targetnegR = 1.0;
	
	controlAposBR = 1.0;
	controlAnegBR = 1.0;
	controlBposBR = 1.0;
	controlBnegBR = 1.0;
	targetposBR = 1.0;
	targetnegBR = 1.0;
	
	controlAposCR = 1.0;
	controlAnegCR = 1.0;
	controlBposCR = 1.0;
	controlBnegCR = 1.0;
	targetposCR = 1.0;
	targetnegCR = 1.0;
	
	avgAR = avgBR = avgCR = avgDR = avgER = avgFR = 0.0;
	nvgAR = nvgBR = nvgCR = nvgDR = nvgER = nvgFR = 0.0;
	//end ButterComps
	
	//begin Power Sags
	for(int count = 0; count < 999; count++) {dL[count] = 0; bL[count] = 0; cL[count] = 0; dR[count] = 0; bR[count] = 0; cR[count] = 0;}
	controlL = 0; controlBL = 0; controlCL = 0;
	controlR = 0; controlBR = 0; controlCR = 0;
	
	gcount = 0;
	//end Power Sags
	fpFlip = true;
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

Logical4::~Logical4() {}
VstInt32 Logical4::getVendorVersion () {return 1000;}
void Logical4::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Logical4::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Logical4::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Logical4::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Logical4::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Threshold", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Ratio", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Speed", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "MakeupGn", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Logical4::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ( (A*40.0)-20.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ( ((B*B)*15.0)+1.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string ( ((C*C)*99.0)+1.0, text, kVstMaxParamStrLen); break;
        case kParamD: float2string ( (D*40.0)-20.0, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Logical4::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "/1", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "ms", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Logical4::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Logical4::getEffectName(char* name) {
    vst_strncpy(name, "Logical4", kVstMaxProductStrLen); return true;
}

VstPlugCategory Logical4::getPlugCategory() {return kPlugCategEffect;}

bool Logical4::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Logical4", kVstMaxProductStrLen); return true;
}

bool Logical4::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Logical4::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value + 20.0) / (40.0); } return b; break; }
    case kParamD: { auto b = string2float(text, value); if (b) { value = (value + 20.0) / (40.0); } return b; break; }
    case kParamE: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Logical4::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamD: return true;
        case kParamE: return true;

    }
    return false;
}
} // end namespace
