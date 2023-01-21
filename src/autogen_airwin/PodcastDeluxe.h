/* ========================================
 *  PodcastDeluxe - PodcastDeluxe.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PodcastDeluxe_PodcastDeluxe_H
#define __PodcastDeluxe_PodcastDeluxe_H

#ifndef __audioeffect__
#include "../airwin2rackbase.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwin2rack::PodcastDeluxe {
enum {
	kParamA = 0,
  kNumParameters = 1
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'podd';    //Change this to what the AU identity is!

class PodcastDeluxe : 
    public AudioEffectX 
{
public:
    PodcastDeluxe(audioMasterCallback audioMaster);
    ~PodcastDeluxe();
    virtual bool getEffectName(char* name);                       // The plug-in name
    virtual VstPlugCategory getPlugCategory();                    // The general category for the plug-in
    virtual bool getProductString(char* text);                    // This is a unique plug-in string provided by Steinberg
    virtual bool getVendorString(char* text);                     // Vendor info
    virtual VstInt32 getVendorVersion();                          // Version number
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);
    virtual void getProgramName(char *name);                      // read the name from the host
    virtual void setProgramName(char *name);                      // changes the name of the preset displayed in the host
	virtual VstInt32 getChunk (void** data, bool isPreset);
	virtual VstInt32 setChunk (void* data, VstInt32 byteSize, bool isPreset);
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
    
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
	
	double d1L[503];
	double d2L[503];
	double d3L[503];
	double d4L[503];
	double d5L[503];
	//the phase rotator
	
	double c1L;
	double c2L;
	double c3L;
	double c4L;
	double c5L;
	//the compressor
	
	double lastSampleL;
	double lastOutSampleL;
	
	double d1R[503];
	double d2R[503];
	double d3R[503];
	double d4R[503];
	double d5R[503];
	
	int tap1, tap2, tap3, tap4, tap5, maxdelay1, maxdelay2, maxdelay3, maxdelay4, maxdelay5;
	//the phase rotator
	
	double c1R;
	double c2R;
	double c3R;
	double c4R;
	double c5R;
	//the compressor
	
	double lastSampleR;
	double lastOutSampleR;
	
    float A;
};

#endif
} // end namespace
