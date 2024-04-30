/* ========================================
 *  kChamberAR - kChamberAR.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __kChamberAR_kChamberAR_H
#define __kChamberAR_kChamberAR_H

#ifndef __audioeffect__
#include "../airwin_consolidated_base.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwinconsolidated::kChamberAR {
enum {
	kParamA = 0,
	kParamB = 1,
	kParamC = 2,
	kParamD = 3,
	kParamE = 4,
  kNumParameters = 5
}; //

const int earlyA = 17; const int earlyB = 503; const int earlyC = 43; const int earlyD = 71; const int earlyE = 653; const int earlyF = 353; const int earlyG = 641; const int earlyH = 5; const int earlyI = 439; const int predelay = 48010; //1 to 40 ms, 52 seat room. Scarcity, 1 in 53537
const int delayA = 263; const int delayB = 103; const int delayC = 43; const int delayD = 349; const int delayE = 109; const int delayF = 71; const int delayG = 173; const int delayH = 11; const int delayI = 311; const int delayJ = 331; const int delayK = 5; const int delayL = 353; const int delayM = 359; const int delayN = 7; const int delayO = 23; const int delayP = 367; const int delayQ = 29; const int delayR = 313; const int delayS = 3; const int delayT = 251; const int delayU = 383; const int delayV = 191; const int delayW = 17; const int delayX = 233; const int delayY = 101; //1 to 40 ms, 52 seat room. Scarcity, 1 in 26639


const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'kcha';    //Change this to what the AU identity is!

class kChamberAR : 
    public AudioEffectX 
{
public:
    kChamberAR(audioMasterCallback audioMaster);
    ~kChamberAR();
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
    
	double iirAL;
	double iirBL;
	
	double steedRegenL;
	double steedRegenR;
	double prevSampleL;
	double prevSampleR;
	
	double gainIn;
	double gainOutL;
	double gainOutR;
	
	double delay;
	double sweep;
	
	
	double eAL[earlyA+5];
	double eBL[earlyB+5];
	double eCL[earlyC+5];
	double eDL[earlyD+5];
	double eEL[earlyE+5];
	double eFL[earlyF+5];
	double eGL[earlyG+5];
	double eHL[earlyH+5];
	double eIL[earlyI+5];
	
	double eAR[earlyA+5];
	double eBR[earlyB+5];
	double eCR[earlyC+5];
	double eDR[earlyD+5];
	double eER[earlyE+5];
	double eFR[earlyF+5];
	double eGR[earlyG+5];
	double eHR[earlyH+5];
	double eIR[earlyI+5];
	
	int earlyAL, earlyAR;
	int earlyBL, earlyBR;
	int earlyCL, earlyCR;
	int earlyDL, earlyDR;
	int earlyEL, earlyER;
	int earlyFL, earlyFR;
	int earlyGL, earlyGR;
	int earlyHL, earlyHR;
	int earlyIL, earlyIR;
	
	double aAL[delayA+5];
	double aBL[delayB+5];
	double aCL[delayC+5];
	double aDL[delayD+5];
	double aEL[delayE+5];
	double aFL[delayF+5];
	double aGL[delayG+5];
	double aHL[delayH+5];
	double aIL[delayI+5];
	double aJL[delayJ+5];
	double aKL[delayK+5];
	double aLL[delayL+5];
	double aML[delayM+5];
	double aNL[delayN+5];
	double aOL[delayO+5];
	double aPL[delayP+5];
	double aQL[delayQ+5];
	double aRL[delayR+5];
	double aSL[delayS+5];
	double aTL[delayT+5];
	double aUL[delayU+5];
	double aVL[delayV+5];
	double aWL[delayW+5];
	double aXL[delayX+5];
	double aYL[delayY+5];
	
	double aAR[delayA+5];
	double aBR[delayB+5];
	double aCR[delayC+5];
	double aDR[delayD+5];
	double aER[delayE+5];
	double aFR[delayF+5];
	double aGR[delayG+5];
	double aHR[delayH+5];
	double aIR[delayI+5];
	double aJR[delayJ+5];
	double aKR[delayK+5];
	double aLR[delayL+5];
	double aMR[delayM+5];
	double aNR[delayN+5];
	double aOR[delayO+5];
	double aPR[delayP+5];
	double aQR[delayQ+5];
	double aRR[delayR+5];
	double aSR[delayS+5];
	double aTR[delayT+5];
	double aUR[delayU+5];
	double aVR[delayV+5];
	double aWR[delayW+5];
	double aXR[delayX+5];
	double aYR[delayY+5];
	
	double aZL[predelay+5];
	double aZR[predelay+5];
	
	
	double feedbackAL;
	double feedbackBL;
	double feedbackCL;
	double feedbackDL;
	double feedbackEL;
	
	double feedbackER;
	double feedbackJR;
	double feedbackOR;
	double feedbackTR;
	double feedbackYR;
	
	double previousAL;
	double previousBL;
	double previousCL;
	double previousDL;
	double previousEL;
	
	double lastRefL[7];
	
	double iirAR;
	double iirBR;
	
	double previousAR;
	double previousBR;
	double previousCR;
	double previousDR;
	double previousER;
	
	double lastRefR[7];
	
	int countAL;
	int countBL;
	int countCL;
	int countDL;
	int countEL;
	int countFL;
	int countGL;
	int countHL;
	int countIL;
	int countJL;
	int countKL;
	int countLL;		
	int countML;		
	int countNL;		
	int countOL;		
	int countPL;		
	int countQL;		
	int countRL;		
	int countSL;		
	int countTL;		
	int countUL;		
	int countVL;		
	int countWL;		
	int countXL;		
	int countYL;		
	
	int countAR;
	int countBR;
	int countCR;
	int countDR;
	int countER;
	int countFR;
	int countGR;
	int countHR;
	int countIR;
	int countJR;
	int countKR;
	int countLR;		
	int countMR;		
	int countNR;		
	int countOR;		
	int countPR;		
	int countQR;		
	int countRR;		
	int countSR;		
	int countTR;		
	int countUR;		
	int countVR;		
	int countWR;		
	int countXR;		
	int countYR;
	
	int countZ;		
	
	int cycle;
	
	enum {
		fix_freq,
		fix_reso,
		fix_a0,
		fix_a1,
		fix_a2,
		fix_b1,
		fix_b2,
		fix_sL1,
		fix_sL2,
		fix_sR1,
		fix_sR2,
		fix_total
	}; //fixed frequency biquad filter for ultrasonics, stereo
	double fixA[fix_total];
	double fixC[fix_total];
	double fixD[fix_total];
	
	double prevMulchDL;
	double prevMulchDR;
	double prevMulchEL;
	double prevMulchER;
	
	double prevOutDL;
	double prevOutDR;
	double prevOutEL;
	double prevOutER;
	
	double prevInDL;
	double prevInDR;
	double prevInEL;
	double prevInER;
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
    float B;
    float C;
    float D;
    float E; //parameters. Always 0-1, and we scale/alter them elsewhere.

};

#endif
} // end namespace
