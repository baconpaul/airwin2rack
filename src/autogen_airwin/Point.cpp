/* ========================================
 *  Point - Point.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Point_H
#include "Point.h"
#endif
#include <cmath>
#include <algorithm>
namespace airwinconsolidated::Point {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Point(audioMaster);}

Point::Point(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	nibAL = 0.0;
	nobAL = 0.0;
	nibBL = 0.0;
	nobBL = 0.0;
	nibAR = 0.0;
	nobAR = 0.0;
	nibBR = 0.0;
	nobBR = 0.0;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	fpFlip = true;
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

Point::~Point() {}
VstInt32 Point::getVendorVersion () {return 1000;}
void Point::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Point::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void Point::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break; //percent. Using this value, it'll be 0-100 everywhere
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float Point::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Point::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Input Trim", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Point", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Reaction Speed", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Point::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string ((A*24.0)-12.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string ((B*2.0)-1.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Point::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, " ", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Point::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Point::getEffectName(char* name) {
    vst_strncpy(name, "Point", kVstMaxProductStrLen); return true;
}

VstPlugCategory Point::getPlugCategory() {return kPlugCategEffect;}

bool Point::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Point", kVstMaxProductStrLen); return true;
}

bool Point::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool Point::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value + 12.0) / (24.0); } return b; break; }
    case kParamB: { auto b = string2float(text, value); if (b) { value = (value + 1.0) / (2.0); } return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool Point::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
