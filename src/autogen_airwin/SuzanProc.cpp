/* ========================================
 *  Suzan - Suzan.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Suzan_H
#include "Suzan.h"
#endif
#include <cstdlib>
namespace airwinconsolidated::Suzan {

void Suzan::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	freqA = freqB; resoA = resoB; outA = outB;
	freqB = pow(A,overallscale+1.0)*1.22;
	resoB = pow(1.0-B,2.0);
	if (resoB < 0.001) resoB = 0.001; // q of 0.0 is just a tone
	outB = C/sqrt(resoB);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		const double temp = (double)sampleFrames/inFramesToProcess;
		const double freq = (freqA*temp)+(freqB*(1.0-temp));
		const double reso = (resoA*temp)+(resoB*(1.0-temp));
		const double out = (outA*temp)+(outB*(1.0-temp)); //dezippering
		
		lowAL += freq*bandAL; bandAL += freq*((reso*inputSampleL)-lowAL-(reso*bandAL));
		inputSampleL = (lowAL-sin(bandCL*0.5)); //alternate airwindowsizationA
		
		lowBL += freq*bandBL; bandBL += freq*((reso*inputSampleL)-lowBL-(reso*bandBL));
		inputSampleL = (lowBL-sin(bandAL*0.5)); //alternate airwindowsizationB
		
		lowCL += freq*bandBL; bandCL += freq*((reso*inputSampleL)-lowCL-(reso*bandCL));
		inputSampleL = (lowCL+sin(bandBL))*out; //alternate airwindowsizationC
		
		lowAR += freq*bandAR; bandAR += freq*((reso*inputSampleR)-lowAR-(reso*bandAR));
		inputSampleR = (lowAR-sin(bandCR*0.5)); //alternate airwindowsizationA
		
		lowBR += freq*bandBR; bandBR += freq*((reso*inputSampleR)-lowBR-(reso*bandBR));
		inputSampleR = (lowBR-sin(bandAR*0.5)); //alternate airwindowsizationB
		
		lowCR += freq*bandBR; bandCR += freq*((reso*inputSampleR)-lowCR-(reso*bandCR));
		inputSampleR = (lowCR+sin(bandBR))*out; //alternate airwindowsizationC
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void Suzan::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	freqA = freqB; resoA = resoB; outA = outB;
	freqB = pow(A,overallscale+1.0)*1.22;
	resoB = pow(1.0-B,2.0);
	if (resoB < 0.001) resoB = 0.001; // q of 0.0 is just a tone
	outB = C/sqrt(resoB);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		const double temp = (double)sampleFrames/inFramesToProcess;
		const double freq = (freqA*temp)+(freqB*(1.0-temp));
		const double reso = (resoA*temp)+(resoB*(1.0-temp));
		const double out = (outA*temp)+(outB*(1.0-temp)); //dezippering
		
		lowAL += freq*bandAL; bandAL += freq*((reso*inputSampleL)-lowAL-(reso*bandAL));
		inputSampleL = (lowAL-sin(bandCL*0.5)); //alternate airwindowsizationA
		
		lowBL += freq*bandBL; bandBL += freq*((reso*inputSampleL)-lowBL-(reso*bandBL));
		inputSampleL = (lowBL-sin(bandAL*0.5)); //alternate airwindowsizationB
		
		lowCL += freq*bandBL; bandCL += freq*((reso*inputSampleL)-lowCL-(reso*bandCL));
		inputSampleL = (lowCL+sin(bandBL))*out; //alternate airwindowsizationC
		
		lowAR += freq*bandAR; bandAR += freq*((reso*inputSampleR)-lowAR-(reso*bandAR));
		inputSampleR = (lowAR-sin(bandCR*0.5)); //alternate airwindowsizationA
		
		lowBR += freq*bandBR; bandBR += freq*((reso*inputSampleR)-lowBR-(reso*bandBR));
		inputSampleR = (lowBR-sin(bandAR*0.5)); //alternate airwindowsizationB
		
		lowCR += freq*bandBR; bandCR += freq*((reso*inputSampleR)-lowCR-(reso*bandCR));
		inputSampleR = (lowCR+sin(bandBR))*out; //alternate airwindowsizationC
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
} // end namespace
