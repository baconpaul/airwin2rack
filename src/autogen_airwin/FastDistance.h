/* ========================================
 *  FastDistance - FastDistance.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __FastDistance_FastDistance_H
#define __FastDistance_FastDistance_H

#ifndef __audioeffect__
#include "../airwin_consolidated_base.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwinconsolidated::FastDistance {
enum {
	kParamA =0,
  kNumParameters = 1
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'fdis';    //Change this to what the AU identity is!

class FastDistance : 
    public AudioEffectX 
{
public:
    FastDistance(audioMasterCallback audioMaster);
    ~FastDistance();
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
    
    float A;

	float firstDryL,firstAvgL;
	float lsA,lsB,lsC,lsD,lsE,lsF,lsG,lsH,lsI,lsJ,lsK,lsL,lsM;
	float lsN,lsO,lsP,lsQ,lsR,lsS,lsT,lsU,lsV,lsW,lsX,lsY,lsZ;
	float lsa,lsb,lsc,lsd,lse,lsf,lsg,lsh,lsi,lsj,lsk,lsl,lsm;
	float lsn,lso,lsp,lsq,lsr,lss,lst,lsu,lsv,lsw,lsx,lsy,lsz;
	float firstDryR,firstAvgR;
	float rsA,rsB,rsC,rsD,rsE,rsF,rsG,rsH,rsI,rsJ,rsK,rsL,rsM;
	float rsN,rsO,rsP,rsQ,rsR,rsS,rsT,rsU,rsV,rsW,rsX,rsY,rsZ;
	float rsa,rsb,rsc,rsd,rse,rsf,rsg,rsh,rsi,rsj,rsk,rsl,rsm;
	float rsn,rso,rsp,rsq,rsr,rss,rst,rsu,rsv,rsw,rsx,rsy,rsz;
	
	int prevDistance;
	float pointCycle, pointAL, pointBL, pointAR, pointBR;
	//this is a simple linear interpolation sample rate routine for the plugin version
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
} // end namespace
