/* ========================================
 *  AtmosphereChannel - AtmosphereChannel.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __AtmosphereChannel_AtmosphereChannel_H
#define __AtmosphereChannel_AtmosphereChannel_H

#ifndef __audioeffect__
#include "../airwin2rackbase.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwin2rack::AtmosphereChannel {
enum {
	kParamA = 0,
  kNumParameters = 1
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'atch';    //Change this to what the AU identity is!

class AtmosphereChannel : 
    public AudioEffectX 
{
public:
    AtmosphereChannel(audioMasterCallback audioMaster);
    ~AtmosphereChannel();
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
    
	double gainchase;
	double settingchase;
	double chasespeed;		
	
	uint32_t fpdL;
	uint32_t fpdR;
	double fpNShapeL;
	double lastSampleAL;
	double lastSampleBL;
	double lastSampleCL;
	double lastSampleDL;
	double lastSampleEL;
	double lastSampleFL;
	double lastSampleGL;
	double lastSampleHL;
	double lastSampleIL;
	double lastSampleJL;
	double lastSampleKL;
	double lastSampleLL;
	double lastSampleML;
	
	double fpNShapeR;
	double lastSampleAR;
	double lastSampleBR;
	double lastSampleCR;
	double lastSampleDR;
	double lastSampleER;
	double lastSampleFR;
	double lastSampleGR;
	double lastSampleHR;
	double lastSampleIR;
	double lastSampleJR;
	double lastSampleKR;
	double lastSampleLR;
	double lastSampleMR;
	
	double thresholdA;
	double thresholdB;
	double thresholdC;
	double thresholdD;
	double thresholdE;
	double thresholdF;
	double thresholdG;
	double thresholdH;
	double thresholdI;
	double thresholdJ;
	double thresholdK;
	double thresholdL;
	double thresholdM;
	
    float A;
};

#endif
} // end namespace
