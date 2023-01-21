/* ========================================
 *  DeEss - DeEss.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DeEss_DeEss_H
#define __DeEss_DeEss_H

#ifndef __audioeffect__
#include "../airwin2rackbase.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwin2rack::DeEss {
enum {
	kParamA = 0,
	kParamB = 1,
	kParamC = 2,
  kNumParameters = 3
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'dess';    //Change this to what the AU identity is!

class DeEss : 
    public AudioEffectX 
{
public:
    DeEss(audioMasterCallback audioMaster);
    ~DeEss();
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
	
	double s1L;
	double s2L;
	double s3L;
	double s4L;
	double s5L;
	double s6L;
	double s7L;
	double m1L;
	double m2L;
	double m3L;
	double m4L;
	double m5L;
	double m6L;
	double c1L;
	double c2L;
	double c3L;
	double c4L;
	double c5L;
	double ratioAL;
	double ratioBL;
	double iirSampleAL;
	double iirSampleBL;
	
	double s1R;
	double s2R;
	double s3R;
	double s4R;
	double s5R;
	double s6R;
	double s7R;
	double m1R;
	double m2R;
	double m3R;
	double m4R;
	double m5R;
	double m6R;
	double c1R;
	double c2R;
	double c3R;
	double c4R;
	double c5R;
	double ratioAR;
	double ratioBR;
	double iirSampleAR;
	double iirSampleBR;
	
	
	bool flip;	
    
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
    float B;
    float C;
};

#endif
} // end namespace
