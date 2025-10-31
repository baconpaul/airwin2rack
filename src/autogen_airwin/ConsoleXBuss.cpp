/* ========================================
 *  ConsoleXBuss - ConsoleXBuss.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleXBuss_H
#include "ConsoleXBuss.h"
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>
namespace airwinconsolidated::ConsoleXBuss {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ConsoleXBuss(audioMaster);}

ConsoleXBuss::ConsoleXBuss(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	HIP = 0.0;
	LOP = 0.0;
	AIR = 0.5;
	FIR = 0.5;
	STO = 0.5;
	RNG = 0.5;
	FCT = 1.0;
	SCT = 1.0;
	FCR = 1.0;
	SCR = 1.0;
	FCA = 0.5;
	SCA = 0.5;
	FCL = 0.5;
	SCL = 0.5;
	FGT = 0.0;
	SGT = 0.0;
	FGR = 1.0;
	SGR = 1.0;
	FGS = 0.5;
	SGS = 0.5;
	FGL = 0.5;
	SGL = 0.5;	
	TRF = 0.5;
	TRG = 0.5;
	TRR = 0.5;
	HMF = 0.5;
	HMG = 0.5;
	HMR = 0.5;
	LMF = 0.5;
	LMG = 0.5;
	LMR = 0.5;
	BSF = 0.5;
	BSG = 0.5;
	BSR = 0.5;
	DSC = 0.5;
	PAN = 0.5;
	FAD = 0.5;

	for (int x = 0; x < hilp_total; x++) {
		highpass[x] = 0.0;
		lowpass[x] = 0.0;
	}
	
	for (int x = 0; x < air_total; x++) air[x] = 0.0;
	for (int x = 0; x < kal_total; x++) kal[x] = 0.0;
	fireCompL = 1.0;
	fireCompR = 1.0;
	fireGate = 1.0;
	stoneCompL = 1.0;
	stoneCompR = 1.0;
	stoneGate = 1.0;	
	
	for (int x = 0; x < biqs_total; x++) {
		high[x] = 0.0;
		hmid[x] = 0.0;
		lmid[x] = 0.0;
		bass[x] = 0.0;
	}
	
	for(int count = 0; count < dscBuf+2; count++) {
		dBaL[count] = 0.0;
		dBaR[count] = 0.0;
	}
	dBaPosL = 0.0;
	dBaPosR = 0.0;
	dBaXL = 1;
	dBaXR = 1;
		
	airGainA = 0.5; airGainB = 0.5;
	fireGainA = 0.5; fireGainB = 0.5;
	stoneGainA = 0.5; stoneGainB = 0.5;
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

ConsoleXBuss::~ConsoleXBuss() {}
VstInt32 ConsoleXBuss::getVendorVersion () {return 1000;}
void ConsoleXBuss::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ConsoleXBuss::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void ConsoleXBuss::setParameter(VstInt32 index, float value) {
    switch (index) {
		case kParamHIP: HIP = value; break;
		case kParamLOP: LOP = value; break;
		case kParamAIR: AIR = value; break;
		case kParamFIR: FIR = value; break;
		case kParamSTO: STO = value; break;
		case kParamRNG: RNG = value; break;
		case kParamFCT: FCT = value; break;
		case kParamSCT: SCT = value; break;
		case kParamFCR: FCR = value; break;
		case kParamSCR: SCR = value; break;
		case kParamFCA: FCA = value; break;
		case kParamSCA: SCA = value; break;
		case kParamFCL: FCL = value; break;
		case kParamSCL: SCL = value; break;
		case kParamFGT: FGT = value; break;
		case kParamSGT: SGT = value; break;
		case kParamFGR: FGR = value; break;
		case kParamSGR: SGR = value; break;
		case kParamFGS: FGS = value; break;
		case kParamSGS: SGS = value; break;
		case kParamFGL: FGL = value; break;
		case kParamSGL: SGL = value; break;	
		case kParamTRF: TRF = value; break;
		case kParamTRG: TRG = value; break;
		case kParamTRR: TRR = value; break;
		case kParamHMF: HMF = value; break;
		case kParamHMG: HMG = value; break;
		case kParamHMR: HMR = value; break;
		case kParamLMF: LMF = value; break;
		case kParamLMG: LMG = value; break;
		case kParamLMR: LMR = value; break;
		case kParamBSF: BSF = value; break;
		case kParamBSG: BSG = value; break;
		case kParamBSR: BSR = value; break;
		case kParamDSC: DSC = value; break;
		case kParamPAN: PAN = value; break;
		case kParamFAD: FAD = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float ConsoleXBuss::getParameter(VstInt32 index) {
    switch (index) {
		case kParamHIP: return HIP; break;
		case kParamLOP: return LOP; break;
		case kParamAIR: return AIR; break;
		case kParamFIR: return FIR; break;
		case kParamSTO: return STO; break;
		case kParamRNG: return RNG; break;
		case kParamFCT: return FCT; break;
		case kParamSCT: return SCT; break;
		case kParamFCR: return FCR; break;
		case kParamSCR: return SCR; break;
		case kParamFCA: return FCA; break;
		case kParamSCA: return SCA; break;
		case kParamFCL: return FCL; break;
		case kParamSCL: return SCL; break;
		case kParamFGT: return FGT; break;
		case kParamSGT: return SGT; break;
		case kParamFGR: return FGR; break;
		case kParamSGR: return SGR; break;
		case kParamFGS: return FGS; break;
		case kParamSGS: return SGS; break;
		case kParamFGL: return FGL; break;
		case kParamSGL: return SGL; break;	
		case kParamTRF: return TRF; break;
		case kParamTRG: return TRG; break;
		case kParamTRR: return TRR; break;
		case kParamHMF: return HMF; break;
		case kParamHMG: return HMG; break;
		case kParamHMR: return HMR; break;
		case kParamLMF: return LMF; break;
		case kParamLMG: return LMG; break;
		case kParamLMR: return LMR; break;
		case kParamBSF: return BSF; break;
		case kParamBSG: return BSG; break;
		case kParamBSR: return BSR; break;
		case kParamDSC: return DSC; break;
		case kParamPAN: return PAN; break;
		case kParamFAD: return FAD; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ConsoleXBuss::getParameterName(VstInt32 index, char *text) {
    switch (index) {
		case kParamHIP: vst_strncpy (text, "Highpas", kVstMaxParamStrLen); break;
		case kParamLOP: vst_strncpy (text, "Lowpass", kVstMaxParamStrLen); break;
		case kParamAIR: vst_strncpy (text, "Air", kVstMaxParamStrLen); break;
		case kParamFIR: vst_strncpy (text, "Fire", kVstMaxParamStrLen); break;
		case kParamSTO: vst_strncpy (text, "Stone", kVstMaxParamStrLen); break;
		case kParamRNG: vst_strncpy (text, "Range", kVstMaxParamStrLen); break;
		case kParamFCT: vst_strncpy (text, "FC Thrs", kVstMaxParamStrLen); break;
		case kParamSCT: vst_strncpy (text, "SC Thrs", kVstMaxParamStrLen); break;
		case kParamFCR: vst_strncpy (text, "FC Rati", kVstMaxParamStrLen); break;
		case kParamSCR: vst_strncpy (text, "SC Rati", kVstMaxParamStrLen); break;
		case kParamFCA: vst_strncpy (text, "FC Atk", kVstMaxParamStrLen); break;
		case kParamSCA: vst_strncpy (text, "SC Atk", kVstMaxParamStrLen); break;
		case kParamFCL: vst_strncpy (text, "FC Rls", kVstMaxParamStrLen); break;
		case kParamSCL: vst_strncpy (text, "SC Rls", kVstMaxParamStrLen); break;
		case kParamFGT: vst_strncpy (text, "FG Thrs", kVstMaxParamStrLen); break;
		case kParamSGT: vst_strncpy (text, "SG Thrs", kVstMaxParamStrLen); break;
		case kParamFGR: vst_strncpy (text, "FG Rati", kVstMaxParamStrLen); break;
		case kParamSGR: vst_strncpy (text, "SG Rati", kVstMaxParamStrLen); break;
		case kParamFGS: vst_strncpy (text, "FG Sust", kVstMaxParamStrLen); break;
		case kParamSGS: vst_strncpy (text, "SG Sust", kVstMaxParamStrLen); break;
		case kParamFGL: vst_strncpy (text, "FG Rls", kVstMaxParamStrLen); break;
		case kParamSGL: vst_strncpy (text, "SG Rls", kVstMaxParamStrLen); break;	
		case kParamTRF: vst_strncpy (text, "Tr Freq", kVstMaxParamStrLen); break;
		case kParamTRG: vst_strncpy (text, "Treble", kVstMaxParamStrLen); break;
		case kParamTRR: vst_strncpy (text, "Tr Reso", kVstMaxParamStrLen); break;
		case kParamHMF: vst_strncpy (text, "HM Freq", kVstMaxParamStrLen); break;
		case kParamHMG: vst_strncpy (text, "HighMid", kVstMaxParamStrLen); break;
		case kParamHMR: vst_strncpy (text, "HM Reso", kVstMaxParamStrLen); break;
		case kParamLMF: vst_strncpy (text, "LM Freq", kVstMaxParamStrLen); break;
		case kParamLMG: vst_strncpy (text, "LowMid", kVstMaxParamStrLen); break;
		case kParamLMR: vst_strncpy (text, "LM Reso", kVstMaxParamStrLen); break;
		case kParamBSF: vst_strncpy (text, "Bs Freq", kVstMaxParamStrLen); break;
		case kParamBSG: vst_strncpy (text, "Bass", kVstMaxParamStrLen); break;
		case kParamBSR: vst_strncpy (text, "Bs Reso", kVstMaxParamStrLen); break;
		case kParamDSC: vst_strncpy (text, "Top dB", kVstMaxParamStrLen); break;
		case kParamPAN: vst_strncpy (text, "Pan", kVstMaxParamStrLen); break;
		case kParamFAD: vst_strncpy (text, "Fader", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ConsoleXBuss::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
		case kParamHIP: float2string (HIP, text, kVstMaxParamStrLen); break;
		case kParamLOP: float2string (LOP, text, kVstMaxParamStrLen); break;
		case kParamAIR: float2string (AIR, text, kVstMaxParamStrLen); break;
		case kParamFIR: float2string (FIR, text, kVstMaxParamStrLen); break;
		case kParamSTO: float2string (STO, text, kVstMaxParamStrLen); break;
		case kParamRNG: float2string (RNG, text, kVstMaxParamStrLen); break;
		case kParamFCT: float2string (FCT, text, kVstMaxParamStrLen); break;
		case kParamSCT: float2string (SCT, text, kVstMaxParamStrLen); break;
		case kParamFCR: float2string (FCR, text, kVstMaxParamStrLen); break;
		case kParamSCR: float2string (SCR, text, kVstMaxParamStrLen); break;
		case kParamFCA: float2string (FCA, text, kVstMaxParamStrLen); break;
		case kParamSCA: float2string (SCA, text, kVstMaxParamStrLen); break;
		case kParamFCL: float2string (FCL, text, kVstMaxParamStrLen); break;
		case kParamSCL: float2string (SCL, text, kVstMaxParamStrLen); break;
		case kParamFGT: float2string (FGT, text, kVstMaxParamStrLen); break;
		case kParamSGT: float2string (SGT, text, kVstMaxParamStrLen); break;
		case kParamFGR: float2string (FGR, text, kVstMaxParamStrLen); break;
		case kParamSGR: float2string (SGR, text, kVstMaxParamStrLen); break;
		case kParamFGS: float2string (FGS, text, kVstMaxParamStrLen); break;
		case kParamSGS: float2string (SGS, text, kVstMaxParamStrLen); break;
		case kParamFGL: float2string (FGL, text, kVstMaxParamStrLen); break;
		case kParamSGL: float2string (SGL, text, kVstMaxParamStrLen); break;	
		case kParamTRF: float2string (TRF, text, kVstMaxParamStrLen); break;
		case kParamTRG: float2string (TRG, text, kVstMaxParamStrLen); break;
		case kParamTRR: float2string (TRR, text, kVstMaxParamStrLen); break;
		case kParamHMF: float2string (HMF, text, kVstMaxParamStrLen); break;
		case kParamHMG: float2string (HMG, text, kVstMaxParamStrLen); break;
		case kParamHMR: float2string (HMR, text, kVstMaxParamStrLen); break;
		case kParamLMF: float2string (LMF, text, kVstMaxParamStrLen); break;
		case kParamLMG: float2string (LMG, text, kVstMaxParamStrLen); break;
		case kParamLMR: float2string (LMR, text, kVstMaxParamStrLen); break;
		case kParamBSF: float2string (BSF, text, kVstMaxParamStrLen); break;
		case kParamBSG: float2string (BSG, text, kVstMaxParamStrLen); break;
		case kParamBSR: float2string (BSR, text, kVstMaxParamStrLen); break;
		case kParamDSC: float2string ((DSC*70.0)+70.0, text, kVstMaxParamStrLen); break;
		case kParamPAN: float2string (PAN, text, kVstMaxParamStrLen); break;
		case kParamFAD: float2string (FAD, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ConsoleXBuss::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		case kParamHIP: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamLOP: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamAIR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamFIR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamSTO: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamRNG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamFCT: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamSCT: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamFCR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamSCR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamFCA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamSCA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamFCL: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamSCL: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamFGT: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamSGT: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamFGR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamSGR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamFGS: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamSGS: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamFGL: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamSGL: vst_strncpy (text, "", kVstMaxParamStrLen); break;	
		case kParamTRF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamTRG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamTRR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamHMF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamHMG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamHMR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamLMF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamLMG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamLMR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamBSF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamBSG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamBSR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamDSC: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
		case kParamPAN: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		case kParamFAD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ConsoleXBuss::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ConsoleXBuss::getEffectName(char* name) {
    vst_strncpy(name, "ConsoleXBuss", kVstMaxProductStrLen); return true;
}

VstPlugCategory ConsoleXBuss::getPlugCategory() {return kPlugCategEffect;}

bool ConsoleXBuss::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ConsoleXBuss", kVstMaxProductStrLen); return true;
}

bool ConsoleXBuss::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool ConsoleXBuss::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamHIP: { auto b = string2float(text, value); return b; break; }
    case kParamLOP: { auto b = string2float(text, value); return b; break; }
    case kParamAIR: { auto b = string2float(text, value); return b; break; }
    case kParamFIR: { auto b = string2float(text, value); return b; break; }
    case kParamSTO: { auto b = string2float(text, value); return b; break; }
    case kParamRNG: { auto b = string2float(text, value); return b; break; }
    case kParamFCT: { auto b = string2float(text, value); return b; break; }
    case kParamSCT: { auto b = string2float(text, value); return b; break; }
    case kParamFCR: { auto b = string2float(text, value); return b; break; }
    case kParamSCR: { auto b = string2float(text, value); return b; break; }
    case kParamFCA: { auto b = string2float(text, value); return b; break; }
    case kParamSCA: { auto b = string2float(text, value); return b; break; }
    case kParamFCL: { auto b = string2float(text, value); return b; break; }
    case kParamSCL: { auto b = string2float(text, value); return b; break; }
    case kParamFGT: { auto b = string2float(text, value); return b; break; }
    case kParamSGT: { auto b = string2float(text, value); return b; break; }
    case kParamFGR: { auto b = string2float(text, value); return b; break; }
    case kParamSGR: { auto b = string2float(text, value); return b; break; }
    case kParamFGS: { auto b = string2float(text, value); return b; break; }
    case kParamSGS: { auto b = string2float(text, value); return b; break; }
    case kParamFGL: { auto b = string2float(text, value); return b; break; }
    case kParamSGL: { auto b = string2float(text, value); return b; break; }
    case kParamTRF: { auto b = string2float(text, value); return b; break; }
    case kParamTRG: { auto b = string2float(text, value); return b; break; }
    case kParamTRR: { auto b = string2float(text, value); return b; break; }
    case kParamHMF: { auto b = string2float(text, value); return b; break; }
    case kParamHMG: { auto b = string2float(text, value); return b; break; }
    case kParamHMR: { auto b = string2float(text, value); return b; break; }
    case kParamLMF: { auto b = string2float(text, value); return b; break; }
    case kParamLMG: { auto b = string2float(text, value); return b; break; }
    case kParamLMR: { auto b = string2float(text, value); return b; break; }
    case kParamBSF: { auto b = string2float(text, value); return b; break; }
    case kParamBSG: { auto b = string2float(text, value); return b; break; }
    case kParamBSR: { auto b = string2float(text, value); return b; break; }
    case kParamDSC: { auto b = string2float(text, value); if (b) { value = (value - 70.0) / (70.0); } return b; break; }
    case kParamPAN: { auto b = string2float(text, value); return b; break; }
    case kParamFAD: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool ConsoleXBuss::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamHIP: return true;
        case kParamLOP: return true;
        case kParamAIR: return true;
        case kParamFIR: return true;
        case kParamSTO: return true;
        case kParamRNG: return true;
        case kParamFCT: return true;
        case kParamSCT: return true;
        case kParamFCR: return true;
        case kParamSCR: return true;
        case kParamFCA: return true;
        case kParamSCA: return true;
        case kParamFCL: return true;
        case kParamSCL: return true;
        case kParamFGT: return true;
        case kParamSGT: return true;
        case kParamFGR: return true;
        case kParamSGR: return true;
        case kParamFGS: return true;
        case kParamSGS: return true;
        case kParamFGL: return true;
        case kParamSGL: return true;
        case kParamTRF: return true;
        case kParamTRG: return true;
        case kParamTRR: return true;
        case kParamHMF: return true;
        case kParamHMG: return true;
        case kParamHMR: return true;
        case kParamLMF: return true;
        case kParamLMG: return true;
        case kParamLMR: return true;
        case kParamBSF: return true;
        case kParamBSG: return true;
        case kParamBSR: return true;
        case kParamDSC: return true;
        case kParamPAN: return true;
        case kParamFAD: return true;

    }
    return false;
}
} // end namespace
