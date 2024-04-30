/* ========================================
 *  SubsOnly - SubsOnly.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SubsOnly_H
#include "SubsOnly.h"
#endif
namespace airwinconsolidated::SubsOnly {

void SubsOnly::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double iirAmount = 2250/44100.0;
	double gaintarget = 1.42;
	double gain;
	iirAmount /= overallscale;
	double altAmount = 1.0 - iirAmount;
	double inputSampleL;
	double inputSampleR;
    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		gain = gaintarget;

		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		iirSampleAL = (iirSampleAL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleAL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleBL = (iirSampleBL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleBL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleCL = (iirSampleCL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleCL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleDL = (iirSampleDL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleDL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleEL = (iirSampleEL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleEL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleFL = (iirSampleFL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleFL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleGL = (iirSampleGL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleGL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleHL = (iirSampleHL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleHL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleIL = (iirSampleIL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleIL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleJL = (iirSampleJL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleJL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleKL = (iirSampleKL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleKL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleLL = (iirSampleLL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleLL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleML = (iirSampleML * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleML;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleNL = (iirSampleNL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleNL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleOL = (iirSampleOL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleOL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSamplePL = (iirSamplePL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSamplePL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleQL = (iirSampleQL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleQL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleRL = (iirSampleRL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleRL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleSL = (iirSampleSL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleSL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleTL = (iirSampleTL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleTL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleUL = (iirSampleUL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleUL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleVL = (iirSampleVL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleVL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleWL = (iirSampleWL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleWL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleXL = (iirSampleXL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleXL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleYL = (iirSampleYL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleYL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleZL = (iirSampleZL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleZL;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
				gain = gaintarget;

		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		iirSampleAR = (iirSampleAR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleAR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleBR = (iirSampleBR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleBR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleCR = (iirSampleCR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleCR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleDR = (iirSampleDR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleDR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleER = (iirSampleER * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleER;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleFR = (iirSampleFR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleFR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleGR = (iirSampleGR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleGR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleHR = (iirSampleHR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleHR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleIR = (iirSampleIR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleIR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleJR = (iirSampleJR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleJR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleKR = (iirSampleKR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleKR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleLR = (iirSampleLR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleLR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleMR = (iirSampleMR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleMR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleNR = (iirSampleNR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleNR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleOR = (iirSampleOR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleOR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSamplePR = (iirSamplePR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSamplePR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleQR = (iirSampleQR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleQR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleRR = (iirSampleRR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleRR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleSR = (iirSampleSR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleSR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleTR = (iirSampleTR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleTR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleUR = (iirSampleUR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleUR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleVR = (iirSampleVR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleVR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleWR = (iirSampleWR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleWR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleXR = (iirSampleXR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleXR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleYR = (iirSampleYR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleYR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleZR = (iirSampleZR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleZR;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;		
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void SubsOnly::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double iirAmount = 2250/44100.0;
	double gaintarget = 1.42;
	double gain;
	iirAmount /= overallscale;
	double altAmount = 1.0 - iirAmount;
	double inputSampleL;
	double inputSampleR;

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		gain = gaintarget;
		
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		iirSampleAL = (iirSampleAL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleAL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleBL = (iirSampleBL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleBL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleCL = (iirSampleCL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleCL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleDL = (iirSampleDL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleDL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleEL = (iirSampleEL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleEL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleFL = (iirSampleFL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleFL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleGL = (iirSampleGL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleGL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleHL = (iirSampleHL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleHL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleIL = (iirSampleIL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleIL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleJL = (iirSampleJL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleJL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleKL = (iirSampleKL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleKL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleLL = (iirSampleLL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleLL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleML = (iirSampleML * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleML;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleNL = (iirSampleNL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleNL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleOL = (iirSampleOL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleOL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSamplePL = (iirSamplePL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSamplePL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleQL = (iirSampleQL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleQL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleRL = (iirSampleRL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleRL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleSL = (iirSampleSL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleSL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleTL = (iirSampleTL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleTL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleUL = (iirSampleUL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleUL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleVL = (iirSampleVL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleVL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleWL = (iirSampleWL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleWL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleXL = (iirSampleXL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleXL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleYL = (iirSampleYL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleYL;
		inputSampleL *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		iirSampleZL = (iirSampleZL * altAmount) + (inputSampleL * iirAmount); inputSampleL = iirSampleZL;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
				gain = gaintarget;

		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		iirSampleAR = (iirSampleAR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleAR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleBR = (iirSampleBR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleBR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleCR = (iirSampleCR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleCR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleDR = (iirSampleDR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleDR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleER = (iirSampleER * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleER;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleFR = (iirSampleFR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleFR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleGR = (iirSampleGR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleGR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleHR = (iirSampleHR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleHR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleIR = (iirSampleIR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleIR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleJR = (iirSampleJR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleJR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleKR = (iirSampleKR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleKR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleLR = (iirSampleLR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleLR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleMR = (iirSampleMR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleMR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleNR = (iirSampleNR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleNR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleOR = (iirSampleOR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleOR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSamplePR = (iirSamplePR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSamplePR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleQR = (iirSampleQR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleQR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleRR = (iirSampleRR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleRR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleSR = (iirSampleSR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleSR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleTR = (iirSampleTR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleTR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleUR = (iirSampleUR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleUR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleVR = (iirSampleVR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleVR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleWR = (iirSampleWR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleWR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleXR = (iirSampleXR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleXR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleYR = (iirSampleYR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleYR;
		inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		iirSampleZR = (iirSampleZR * altAmount) + (inputSampleR * iirAmount); inputSampleR = iirSampleZR;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}} // end namespace
