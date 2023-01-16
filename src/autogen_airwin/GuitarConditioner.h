/* ========================================
 *  GuitarConditioner - GuitarConditioner.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __GuitarConditioner_GuitarConditioner_H
#define __GuitarConditioner_GuitarConditioner_H

#ifndef __audioeffect__
#include "../airwin2rackbase.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwin2rack::GuitarConditioner {
enum {
  kNumParameters = 0
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'gcon';    //Change this to what the AU identity is!

class GuitarConditioner : 
    public AudioEffectX 
{
public:
    GuitarConditioner(audioMasterCallback audioMaster);
    ~GuitarConditioner();
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
    virtual VstInt32 canDo(char *text);
private:
    char _programName[kVstMaxProgNameLen + 1];
    std::set< std::string > _canDo;
    
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
	bool fpFlip;
	double lastSampleTL;
	double lastSampleBL; //for Slews
	double iirSampleTAL;
	double iirSampleTBL;
	double iirSampleBAL;
	double iirSampleBBL; //for Highpasses
	
	double lastSampleTR;
	double lastSampleBR; //for Slews
	double iirSampleTAR;
	double iirSampleTBR;
	double iirSampleBAR;
	double iirSampleBBR; //for Highpasses
	
};

#endif
} // end namespace