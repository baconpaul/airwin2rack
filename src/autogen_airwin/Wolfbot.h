/* ========================================
 *  Wolfbot - Wolfbot.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Wolfbot_Wolfbot_H
#define __Wolfbot_Wolfbot_H

#ifndef __audioeffect__
#include "../airwin_consolidated_base.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwinconsolidated::Wolfbot {
enum {
  kNumParameters = 0
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'wolf';    //Change this to what the AU identity is!

class Wolfbot : 
    public AudioEffectX 
{
public:
    Wolfbot(audioMasterCallback audioMaster);
    ~Wolfbot();
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
		prevSampL1,
		prevSlewL1,
		accSlewL1,
		prevSampL2,
		prevSlewL2,
		accSlewL2,
		prevSampL3,
		prevSlewL3,
		accSlewL3,
		kalGainL,
		kalOutL,
		prevSampR1,
		prevSlewR1,
		accSlewR1,
		prevSampR2,
		prevSlewR2,
		accSlewR2,
		prevSampR3,
		prevSlewR3,
		accSlewR3,
		kalGainR,
		kalOutR,
		kal_total
	};
	double kHP[kal_total];
	double kLP[kal_total];
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
} // end namespace
