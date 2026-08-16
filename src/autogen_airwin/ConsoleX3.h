/* ========================================
 *  ConsoleX3 - ConsoleX3.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleX3_ConsoleX3_H
#define __ConsoleX3_ConsoleX3_H

#ifndef __audioeffect__
#include "../airwin_consolidated_base.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwinconsolidated::ConsoleX3 {
enum {
	kParamHIG,
	kParamHMG,
	kParamLMG,
	kParamBSG,
	kParamHIF,
	kParamHMF,
	kParamLMF,
	kParamBSF,
	
	kParamTHR,
	kParamATK,
	kParamRLS,
	kParamRAT,
	
	kParamTRM,
	kParamMDE,
	kParamSMO,
	kParamMOR,
	
	kParamLOP,
	kParamLPQ,
	kParamHIP,
	kParamHPQ,
	
	kParamPAN,
	kParamFAD,
	kNumParameters
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'cx3a';    //Change this to what the AU identity is!

class ConsoleX3 : 
    public AudioEffectX 
{
public:
    ConsoleX3(audioMasterCallback audioMaster);
    ~ConsoleX3();
    virtual bool getEffectName(char* name);                       // The plug-in name
    virtual VstPlugCategory getPlugCategory();                    // The general category for the plug-in
    virtual bool getProductString(char* text);                    // This is a unique plug-in string provided by Steinberg
    virtual bool getVendorString(char* text);                     // Vendor info
    virtual VstInt32 getVendorVersion();                          // Version number
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);
    virtual void getProgramName(char *name);                      // read the name from the host
    virtual void setProgramName(char *name);                      // changes the name of the preset displayed in the host
    virtual float getParameter(VstInt32 index);                   // get the parameter value at the specified index
    virtual void setParameter(VstInt32 index, float value);       // set the parameter at index to value
    virtual void getParameterLabel(VstInt32 index, char *text);  // label for the parameter (eg dB)
    virtual void getParameterName(VstInt32 index, char *text);    // name of the parameter
    virtual void getParameterDisplay(VstInt32 index, char *text); // text description of the current value    
    // Added by the perl as inverses
    virtual bool parameterTextToValue(VstInt32 index, const char *text, float &value);
    virtual bool canConvertParameterTextToValue(VstInt32 index);
    virtual VstInt32 canDo(char *text);
private:
    char _programName[kVstMaxProgNameLen + 1];
    std::set< std::string > _canDo;
    
	float HIG;
	float HMG;
	float LMG;
	float BSG;
	float HIF;
	float HMF;
	float LMF;
	float BSF;
	float THR;
	float ATK;
	float RLS;
	float RAT;
	float TRM;
	float MDE;
	float SMO;
	float MOR;
	float LOP;
	float LPQ;
	float HIP;
	float HPQ;
	float PAN;
	float FAD;
	
	enum {
		bezierHigh_A,bezierHigh_B,bezierHigh_C,bezierHigh,
		bezierHmid_A,bezierHmid_B,bezierHmid_C,bezierHmid,
		bezierLmid_A,bezierLmid_B,bezierLmid_C,bezierLmid,
		bezierBass_A,bezierBass_B,bezierBass_C,bezierBass,
		bezierHighQ_A,bezierHighQ_B,bezierHighQ_C,bezierHighQ,
		bezierMidQ_A,bezierMidQ_B,bezierMidQ_C,bezierMidQ,
		bezierLowQ_A,bezierLowQ_B,bezierLowQ_C,bezierLowQ,
		bezierThresh_A,bezierThresh_B,bezierThresh_C,bezierThresh,
		bezierRatio_A,bezierRatio_B,bezierRatio_C,bezierRatio,
		bezierMore_A,bezierMore_B,bezierMore_C,bezierMore,
		bezierLFreq_A,bezierLFreq_B,bezierLFreq_C,bezierLFreq,
		bezierHFreq_A,bezierHFreq_B,bezierHFreq_C,bezierHFreq,
		bezierGainL_A,bezierGainL_B,bezierGainL_C,bezierGainL,
		bezierGainR_A,bezierGainR_B,bezierGainR_C,bezierGainR,
		bezier_cycle,
		bezier_total
	}; //control smoothing as basic use of bezier curves
	double bezier[bezier_total];
	
	enum {
		bez_HMAL, bez_HMBL, bez_HMCL, bez_HMDL,
		bez_MAL, bez_MBL, bez_MCL, bez_MDL,
		bez_LMAL, bez_LMBL, bez_LMCL, bez_LMDL,
		bez_HMAR, bez_HMBR, bez_HMCR, bez_HMDR, bez_HMcycle,
		bez_MAR, bez_MBR, bez_MCR, bez_MDR, bez_Mcycle,
		bez_LMAR, bez_LMBR, bez_LMCR, bez_LMDR, bez_LMcycle,
		bez_EQtotal
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bezEQ[bez_EQtotal][3];
	
	enum {
		bez_A,
		bez_B,
		bez_C,
		bez_Ctrl,
		bez_cycle,
		bez_min,
		bez_comp,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bezComp[bez_total];
	//Dynamics3
	
	double iirHPositionL[29];
	double iirHAngleL[29];
	double iirHPositionR[29];
	double iirHAngleR[29];
	bool hBypass;
	double iirLPositionL[29];
	double iirLAngleL[29];
	double iirLPositionR[29];
	double iirLAngleR[29];
	bool lBypass;
	//Cabs2
	
	enum {
		bip_dvAA, bip_dvAB, bip_dvAC, bip_dvAD, bip_pvAA, bip_pvAB, bip_pvAC, bip_pvAD,
		bip_dvBA, bip_dvBB, bip_dvBC, bip_dvBD, bip_pvBA, bip_pvBB, bip_pvBC, bip_pvBD,
		bip_dvLA, bip_dvLB, bip_dvLC, bip_dvLD, bip_pvLA, bip_pvLB, bip_pvLC, bip_pvLD,
		bip_dvRA, bip_dvRB, bip_dvRC, bip_dvRD, bip_pvRA, bip_pvRB, bip_pvRC, bip_pvRD,
		bip_total //each distortion section can have one of these, it stacks well
	}; //not remotely elliptic BLEP antialiasing, instead it is derivative BIP :D
	double bip[bip_total];	
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
} // end namespace
