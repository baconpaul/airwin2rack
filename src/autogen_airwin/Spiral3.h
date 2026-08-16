/* ========================================
 *  Spiral3 - Spiral3.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Spiral3_Spiral3_H
#define __Spiral3_Spiral3_H

#ifndef __audioeffect__
#include "../airwin_consolidated_base.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwinconsolidated::Spiral3 {
enum {
	kParamA =0,
	kParamB =1,
	kParamC =2,
  kNumParameters = 3
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'spi3';    //Change this to what the AU identity is!

class Spiral3 : 
    public AudioEffectX 
{
public:
    Spiral3(audioMasterCallback audioMaster);
    ~Spiral3();
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
    float B;
    float C;
	enum {
		bezierInput_A,bezierInput_B,bezierInput_C,bezierInput_Out,
		bezierOutput_A,bezierOutput_B,bezierOutput_C,bezierOutput_Out,
		bezier_cycle,
		bezier_total
	}; //control smoothing as basic use of bezier curves
	double bezier[bezier_total];
	
	enum {
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
