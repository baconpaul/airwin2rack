/* ========================================
 *  Air3 - Air3.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Air3_Air3_H
#define __Air3_Air3_H

#ifndef __audioeffect__
#include "../airwin_consolidated_base.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwinconsolidated::Air3 {
enum {
	kParamA = 0,
	kParamB = 1,
  kNumParameters = 2
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'aiyt';    //Change this to what the AU identity is!

class Air3 : 
    public AudioEffectX 
{
public:
    Air3(audioMasterCallback audioMaster);
    ~Air3();
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
    
	enum {
		pvAL1,
		pvSL1,
		accSL1,
		acc2SL1,
		pvAL2,
		pvSL2,
		accSL2,
		acc2SL2,
		pvAL3,
		pvSL3,
		accSL3,
		pvAL4,
		pvSL4,
		gndavgL,
		outAL,
		gainAL,
		pvAR1,
		pvSR1,
		accSR1,
		acc2SR1,
		pvAR2,
		pvSR2,
		accSR2,
		acc2SR2,
		pvAR3,
		pvSR3,
		accSR3,
		pvAR4,
		pvSR4,
		gndavgR,
		outAR,
		gainAR,
		air_total
	};
	double air[air_total];
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
    float B;
};

#endif
} // end namespace
