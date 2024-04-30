/* ========================================
 *  Monitoring - Monitoring.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Monitoring_Monitoring_H
#define __Monitoring_Monitoring_H

#ifndef __audioeffect__
#include "../airwin_consolidated_base.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwinconsolidated::Monitoring {
enum {
	kParamA = 0,
  kNumParameters = 1
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'moni';    //Change this to what the AU identity is!

class Monitoring : 
    public AudioEffectX 
{
public:
    Monitoring(audioMasterCallback audioMaster);
    ~Monitoring();
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
    
	double bynL[13], bynR[13];
	double noiseShapingL, noiseShapingR;
	//NJAD
	double aL[1503], bL[1503], cL[1503], dL[1503];
	double aR[1503], bR[1503], cR[1503], dR[1503];
	int ax, bx, cx, dx;
	//PeaksOnly
	double lastSampleL, lastSampleR;
	//SlewOnly
	double iirSampleAL, iirSampleBL, iirSampleCL, iirSampleDL, iirSampleEL, iirSampleFL, iirSampleGL;
	double iirSampleHL, iirSampleIL, iirSampleJL, iirSampleKL, iirSampleLL, iirSampleML, iirSampleNL, iirSampleOL, iirSamplePL;
	double iirSampleQL, iirSampleRL, iirSampleSL;
	double iirSampleTL, iirSampleUL, iirSampleVL;
	double iirSampleWL, iirSampleXL, iirSampleYL, iirSampleZL;
	
	double iirSampleAR, iirSampleBR, iirSampleCR, iirSampleDR, iirSampleER, iirSampleFR, iirSampleGR;
	double iirSampleHR, iirSampleIR, iirSampleJR, iirSampleKR, iirSampleLR, iirSampleMR, iirSampleNR, iirSampleOR, iirSamplePR;
	double iirSampleQR, iirSampleRR, iirSampleSR;
	double iirSampleTR, iirSampleUR, iirSampleVR;
	double iirSampleWR, iirSampleXR, iirSampleYR, iirSampleZR; // o/`
	//SubsOnly
	double biquadL[11];
	double biquadR[11];
	//Bandpasses

	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
};

#endif
} // end namespace
