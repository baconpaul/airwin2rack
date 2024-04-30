/* ========================================
 *  Console4Channel - Console4Channel.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Console4Channel_Console4Channel_H
#define __Console4Channel_Console4Channel_H

#ifndef __audioeffect__
#include "../airwin_consolidated_base.h"
#endif

#include <set>
#include <string>
#include <math.h>


// TODO: Change kFirstParameter to your first parameter and add any additional parameters.
namespace airwinconsolidated::Console4Channel {
enum {
  kConsole4ChannelParam = 0,
  kNumParameters = 1
};

// TODO: Add other macros or preprocessor defines here

// TODO: Change to reflect your plugin
const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'conh';    //Change this to what the AU identity is!

class Console4Channel : 
    public AudioEffectX 
{
public:
    Console4Channel(audioMasterCallback audioMaster);
    ~Console4Channel();
    
    // Configuration
    virtual bool getEffectName(char* name);                       // The plug-in name
    virtual VstPlugCategory getPlugCategory();                    // The general category for the plug-in
    
    virtual bool getProductString(char* text);                    // This is a unique plug-in string provided by Steinberg
                                                                  // when the plug-in is registered with them.
    
    virtual bool getVendorString(char* text);                     // Vendor info
    virtual VstInt32 getVendorVersion();                          // Version number
    
    // Processing
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);

    // Programs and parameters
    
    virtual void getProgramName(char *name);                      // read the name from the host
    virtual void setProgramName(char *name);                      // changes the name of the preset displayed in the host
    
    // parameter values are all 0.0f to 1.0f
    virtual float getParameter(VstInt32 index);                   // get the parameter value at the specified index
    virtual void setParameter(VstInt32 index, float value);       // set the parameter at index to value

    virtual void getParameterLabel(VstInt32 index, char *text);  // label for the parameter (eg dB)
    virtual void getParameterName(VstInt32 index, char *text);    // name of the parameter
    virtual void getParameterDisplay(VstInt32 index, char *text); // text description of the current value    
    // Added by the perl as inverses
    virtual bool parameterTextToValue(VstInt32 index, const char *text, float &value);
    virtual bool canConvertParameterTextToValue(VstInt32 index);
    
    // Capabilities
    virtual VstInt32 canDo(char *text);
private:
    char _programName[kVstMaxProgNameLen + 1];
    std::set< std::string > _canDo;
    
	double gainchase;
	double settingchase;
	double chasespeed;
	uint32_t fpdL;
	uint32_t fpdR;

	double fpNShapeL;
	double fpNShapeR;
	//default stuff
	float gain;
};

#endif
} // end namespace
