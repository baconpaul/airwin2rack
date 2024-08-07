/* ========================================
 *  Distance3 - Distance3.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Distance3_Distance3_H
#define __Distance3_Distance3_H

#ifndef __audioeffect__
#include "../airwin_consolidated_base.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwinconsolidated::Distance3 {
enum {
	kParamA =0,
	kParamB =1,
	kParamC =2,
  kNumParameters = 3
}; //
const int dscBuf = 90;

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'disv';    //Change this to what the AU identity is!

class Distance3 : 
    public AudioEffectX 
{
public:
    Distance3(audioMasterCallback audioMaster);
    ~Distance3();
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
	
	double lastclampAL;
	double clampAL;
	double changeAL;
	double prevresultAL;
	double lastAL;
	
	double lastclampBL;
	double clampBL;
	double changeBL;
	double prevresultBL;
	double lastBL;
	
	double lastclampCL;
	double clampCL;
	double changeCL;
	double prevresultCL;
	double lastCL;
	
	double dBaL[dscBuf+5];
	double dBaPosL;
	int dBaXL;
	
	double dBbL[dscBuf+5];
	double dBbPosL;
	int dBbXL;
	
	double dBcL[dscBuf+5];
	double dBcPosL;
	int dBcXL;

	double lastclampAR;
	double clampAR;
	double changeAR;
	double prevresultAR;
	double lastAR;
	
	double lastclampBR;
	double clampBR;
	double changeBR;
	double prevresultBR;
	double lastBR;
	
	double lastclampCR;
	double clampCR;
	double changeCR;
	double prevresultCR;
	double lastCR;
	
	double dBaR[dscBuf+5];
	double dBaPosR;
	int dBaXR;
	
	double dBbR[dscBuf+5];
	double dBbPosR;
	int dBbXR;
	
	double dBcR[dscBuf+5];
	double dBcPosR;
	int dBcXR;
	

	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
} // end namespace
