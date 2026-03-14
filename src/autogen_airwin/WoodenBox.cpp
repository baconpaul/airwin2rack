/* ========================================
 *  WoodenBox - WoodenBox.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __WoodenBox_H
#include "WoodenBox.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::WoodenBox {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new WoodenBox(audioMaster);}

WoodenBox::WoodenBox(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	
	c4AL = c4BL = c4CL = c4DL = c4EL = c4FL = c4GL = c4HL = 1;
	c4IL = c4JL = c4KL = c4LL = c4ML = c4NL = c4OL = c4PL = 1;
	c4AR = c4BR = c4CR = c4DR = c4ER = c4FR = c4GR = c4HR = 1;
	c4IR = c4JR = c4KR = c4LR = c4MR = c4NR = c4OR = c4PR = 1;	
	for(int x = 0; x < d4A+2; x++) {b4AL[x] = 0.0; b4AR[x] = 0.0;}
	for(int x = 0; x < d4B+2; x++) {b4BL[x] = 0.0; b4BR[x] = 0.0;}
	for(int x = 0; x < d4C+2; x++) {b4CL[x] = 0.0; b4CR[x] = 0.0;}
	for(int x = 0; x < d4D+2; x++) {b4DL[x] = 0.0; b4DR[x] = 0.0;}
	for(int x = 0; x < d4E+2; x++) {b4EL[x] = 0.0; b4ER[x] = 0.0;}
	for(int x = 0; x < d4F+2; x++) {b4FL[x] = 0.0; b4FR[x] = 0.0;}
	for(int x = 0; x < d4G+2; x++) {b4GL[x] = 0.0; b4GR[x] = 0.0;}
	for(int x = 0; x < d4H+2; x++) {b4HL[x] = 0.0; b4HR[x] = 0.0;}
	for(int x = 0; x < d4I+2; x++) {b4IL[x] = 0.0; b4IR[x] = 0.0;}
	for(int x = 0; x < d4J+2; x++) {b4JL[x] = 0.0; b4JR[x] = 0.0;}
	for(int x = 0; x < d4K+2; x++) {b4KL[x] = 0.0; b4KR[x] = 0.0;}
	for(int x = 0; x < d4L+2; x++) {b4LL[x] = 0.0; b4LR[x] = 0.0;}
	for(int x = 0; x < d4M+2; x++) {b4ML[x] = 0.0; b4MR[x] = 0.0;}
	for(int x = 0; x < d4N+2; x++) {b4NL[x] = 0.0; b4NR[x] = 0.0;}
	for(int x = 0; x < d4O+2; x++) {b4OL[x] = 0.0; b4OR[x] = 0.0;}
	for(int x = 0; x < d4P+2; x++) {b4PL[x] = 0.0; b4PR[x] = 0.0;}
	g4AL = g4BL = g4CL = g4DL = 0.0;
	g4DR = g4HR = g4LR = g4PR = 0.0;
	
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
	
	shortA = 173;
	shortB = 82;
	shortC = 240;
	shortD = 191;
	shortE = 196;
	shortF = 257;
	shortG = 203;
	shortH = 252;
	shortI = 207;
	shortJ = 203;
	shortK = 250;
	shortL = 220;
	shortM = 261;
	shortN = 235;
	shortO = 161;
	shortP = 161;
	prevclearcoat = -1;
		
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

WoodenBox::~WoodenBox() {}
VstInt32 WoodenBox::getVendorVersion () {return 1000;}
void WoodenBox::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void WoodenBox::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void WoodenBox::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float WoodenBox::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void WoodenBox::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Select", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Reso", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Depth", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void WoodenBox::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: int2string ((int)(A*16.999), text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void WoodenBox::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 WoodenBox::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool WoodenBox::getEffectName(char* name) {
    vst_strncpy(name, "WoodenBox", kVstMaxProductStrLen); return true;
}

VstPlugCategory WoodenBox::getPlugCategory() {return kPlugCategEffect;}

bool WoodenBox::getProductString(char* text) {
  	vst_strncpy (text, "airwindows WoodenBox", kVstMaxProductStrLen); return true;
}

bool WoodenBox::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool WoodenBox::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); if (b) { value = (value + 0.1) / 16.999; } return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool WoodenBox::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
