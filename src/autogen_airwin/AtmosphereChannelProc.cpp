/* ========================================
 *  AtmosphereChannel - AtmosphereChannel.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __AtmosphereChannel_H
#include "AtmosphereChannel.h"
#endif
namespace airwinconsolidated::AtmosphereChannel {

void AtmosphereChannel::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	
	double clamp;
	double inputgain = A;
	
	if (settingchase != inputgain) {
		chasespeed *= 2.0;
		settingchase = inputgain;
	}
	if (chasespeed > 2500.0) chasespeed = 2500.0;
	if (gainchase < 0.0) gainchase = inputgain;	
	
	thresholdA = 0.618033988749894 / overallscale;
	thresholdB = 0.679837387624884 / overallscale;
	thresholdC = 0.747821126387373 / overallscale;
	thresholdD = 0.82260323902611 / overallscale;
	thresholdE = 0.904863562928721 / overallscale;
	thresholdF = 0.995349919221593 / overallscale;
	thresholdG = 1.094884911143752 / overallscale;
	thresholdH = 1.204373402258128 / overallscale;
	thresholdI = 1.32481074248394 / overallscale;
	thresholdJ = 1.457291816732335 / overallscale;
	thresholdK = 1.603020998405568 / overallscale;
	thresholdL = 1.763323098246125 / overallscale;
	thresholdM = 1.939655408070737 / overallscale;	
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		chasespeed *= 0.9999;
		chasespeed -= 0.01;
		if (chasespeed < 350.0) chasespeed = 350.0;
		//we have our chase speed compensated for recent fader activity
		
		gainchase = (((gainchase*chasespeed)+inputgain)/(chasespeed+1.0));
		//gainchase is chasing the target, as a simple multiply gain factor
		
		if (1.0 != gainchase) {
			inputSampleL *= gainchase;
			inputSampleR *= gainchase;
		}
		//done with trim control
		
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		inputSampleL = sin(inputSampleL);
		inputSampleR = sin(inputSampleR);
		//amplitude aspect

		clamp = inputSampleL - lastSampleAL;
		if (clamp > thresholdA) inputSampleL = lastSampleAL + thresholdA;
		if (-clamp > thresholdA) inputSampleL = lastSampleAL - thresholdA;
		
		clamp = inputSampleL - lastSampleBL;
		if (clamp > thresholdB) inputSampleL = lastSampleBL + thresholdB;
		if (-clamp > thresholdB) inputSampleL = lastSampleBL - thresholdB;
		
		clamp = inputSampleL - lastSampleCL;
		if (clamp > thresholdC) inputSampleL = lastSampleCL + thresholdC;
		if (-clamp > thresholdC) inputSampleL = lastSampleCL - thresholdC;
		
		clamp = inputSampleL - lastSampleDL;
		if (clamp > thresholdD) inputSampleL = lastSampleDL + thresholdD;
		if (-clamp > thresholdD) inputSampleL = lastSampleDL - thresholdD;
		
		clamp = inputSampleL - lastSampleEL;
		if (clamp > thresholdE) inputSampleL = lastSampleEL + thresholdE;
		if (-clamp > thresholdE) inputSampleL = lastSampleEL - thresholdE;
		
		clamp = inputSampleL - lastSampleFL;
		if (clamp > thresholdF) inputSampleL = lastSampleFL + thresholdF;
		if (-clamp > thresholdF) inputSampleL = lastSampleFL - thresholdF;
		
		clamp = inputSampleL - lastSampleGL;
		if (clamp > thresholdG) inputSampleL = lastSampleGL + thresholdG;
		if (-clamp > thresholdG) inputSampleL = lastSampleGL - thresholdG;
		
		clamp = inputSampleL - lastSampleHL;
		if (clamp > thresholdH) inputSampleL = lastSampleHL + thresholdH;
		if (-clamp > thresholdH) inputSampleL = lastSampleHL - thresholdH;
		
		clamp = inputSampleL - lastSampleIL;
		if (clamp > thresholdI) inputSampleL = lastSampleIL + thresholdI;
		if (-clamp > thresholdI) inputSampleL = lastSampleIL - thresholdI;
		
		clamp = inputSampleL - lastSampleJL;
		if (clamp > thresholdJ) inputSampleL = lastSampleJL + thresholdJ;
		if (-clamp > thresholdJ) inputSampleL = lastSampleJL - thresholdJ;
		
		clamp = inputSampleL - lastSampleKL;
		if (clamp > thresholdK) inputSampleL = lastSampleKL + thresholdK;
		if (-clamp > thresholdK) inputSampleL = lastSampleKL - thresholdK;
		
		clamp = inputSampleL - lastSampleLL;
		if (clamp > thresholdL) inputSampleL = lastSampleLL + thresholdL;
		if (-clamp > thresholdL) inputSampleL = lastSampleLL - thresholdL;
		
		clamp = inputSampleL - lastSampleML;
		if (clamp > thresholdM) inputSampleL = lastSampleML + thresholdM;
		if (-clamp > thresholdM) inputSampleL = lastSampleML - thresholdM;
		
		
		lastSampleML = lastSampleLL;
		lastSampleLL = lastSampleKL;
		lastSampleKL = lastSampleJL;
		lastSampleJL = lastSampleIL;
		lastSampleIL = lastSampleHL;
		lastSampleHL = lastSampleGL;
		lastSampleGL = lastSampleFL;
		lastSampleFL = lastSampleEL;
		lastSampleEL = lastSampleDL;
		lastSampleDL = lastSampleCL;
		lastSampleCL = lastSampleBL;
		lastSampleBL = lastSampleAL;
		lastSampleAL = drySampleL;
		//store the raw L input sample again for use next time

		clamp = inputSampleR - lastSampleAR;
		if (clamp > thresholdA) inputSampleR = lastSampleAR + thresholdA;
		if (-clamp > thresholdA) inputSampleR = lastSampleAR - thresholdA;
		
		clamp = inputSampleR - lastSampleBR;
		if (clamp > thresholdB) inputSampleR = lastSampleBR + thresholdB;
		if (-clamp > thresholdB) inputSampleR = lastSampleBR - thresholdB;
		
		clamp = inputSampleR - lastSampleCR;
		if (clamp > thresholdC) inputSampleR = lastSampleCR + thresholdC;
		if (-clamp > thresholdC) inputSampleR = lastSampleCR - thresholdC;
		
		clamp = inputSampleR - lastSampleDR;
		if (clamp > thresholdD) inputSampleR = lastSampleDR + thresholdD;
		if (-clamp > thresholdD) inputSampleR = lastSampleDR - thresholdD;
		
		clamp = inputSampleR - lastSampleER;
		if (clamp > thresholdE) inputSampleR = lastSampleER + thresholdE;
		if (-clamp > thresholdE) inputSampleR = lastSampleER - thresholdE;
		
		clamp = inputSampleR - lastSampleFR;
		if (clamp > thresholdF) inputSampleR = lastSampleFR + thresholdF;
		if (-clamp > thresholdF) inputSampleR = lastSampleFR - thresholdF;
		
		clamp = inputSampleR - lastSampleGR;
		if (clamp > thresholdG) inputSampleR = lastSampleGR + thresholdG;
		if (-clamp > thresholdG) inputSampleR = lastSampleGR - thresholdG;
		
		clamp = inputSampleR - lastSampleHR;
		if (clamp > thresholdH) inputSampleR = lastSampleHR + thresholdH;
		if (-clamp > thresholdH) inputSampleR = lastSampleHR - thresholdH;
		
		clamp = inputSampleR - lastSampleIR;
		if (clamp > thresholdI) inputSampleR = lastSampleIR + thresholdI;
		if (-clamp > thresholdI) inputSampleR = lastSampleIR - thresholdI;
		
		clamp = inputSampleR - lastSampleJR;
		if (clamp > thresholdJ) inputSampleR = lastSampleJR + thresholdJ;
		if (-clamp > thresholdJ) inputSampleR = lastSampleJR - thresholdJ;
		
		clamp = inputSampleR - lastSampleKR;
		if (clamp > thresholdK) inputSampleR = lastSampleKR + thresholdK;
		if (-clamp > thresholdK) inputSampleR = lastSampleKR - thresholdK;
		
		clamp = inputSampleR - lastSampleLR;
		if (clamp > thresholdL) inputSampleR = lastSampleLR + thresholdL;
		if (-clamp > thresholdL) inputSampleR = lastSampleLR - thresholdL;
		
		clamp = inputSampleR - lastSampleMR;
		if (clamp > thresholdM) inputSampleR = lastSampleMR + thresholdM;
		if (-clamp > thresholdM) inputSampleR = lastSampleMR - thresholdM;
		
		
		lastSampleMR = lastSampleLR;
		lastSampleLR = lastSampleKR;
		lastSampleKR = lastSampleJR;
		lastSampleJR = lastSampleIR;
		lastSampleIR = lastSampleHR;
		lastSampleHR = lastSampleGR;
		lastSampleGR = lastSampleFR;
		lastSampleFR = lastSampleER;
		lastSampleER = lastSampleDR;
		lastSampleDR = lastSampleCR;
		lastSampleCR = lastSampleBR;
		lastSampleBR = lastSampleAR;
		lastSampleAR = drySampleR;
		//store the raw R input sample again for use next time
		
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void AtmosphereChannel::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	
	double clamp;
	double inputgain = A;
	
	if (settingchase != inputgain) {
		chasespeed *= 2.0;
		settingchase = inputgain;
	}
	if (chasespeed > 2500.0) chasespeed = 2500.0;
	if (gainchase < 0.0) gainchase = inputgain;	
	
	thresholdA = 0.618033988749894 / overallscale;
	thresholdB = 0.679837387624884 / overallscale;
	thresholdC = 0.747821126387373 / overallscale;
	thresholdD = 0.82260323902611 / overallscale;
	thresholdE = 0.904863562928721 / overallscale;
	thresholdF = 0.995349919221593 / overallscale;
	thresholdG = 1.094884911143752 / overallscale;
	thresholdH = 1.204373402258128 / overallscale;
	thresholdI = 1.32481074248394 / overallscale;
	thresholdJ = 1.457291816732335 / overallscale;
	thresholdK = 1.603020998405568 / overallscale;
	thresholdL = 1.763323098246125 / overallscale;
	thresholdM = 1.939655408070737 / overallscale;	
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		chasespeed *= 0.9999;
		chasespeed -= 0.01;
		if (chasespeed < 350.0) chasespeed = 350.0;
		//we have our chase speed compensated for recent fader activity
		
		gainchase = (((gainchase*chasespeed)+inputgain)/(chasespeed+1.0));
		//gainchase is chasing the target, as a simple multiply gain factor
		
		if (1.0 != gainchase) {
			inputSampleL *= gainchase;
			inputSampleR *= gainchase;
		}
		//done with trim control
		
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		inputSampleL = sin(inputSampleL);
		inputSampleR = sin(inputSampleR);
		//amplitude aspect
		
		clamp = inputSampleL - lastSampleAL;
		if (clamp > thresholdA) inputSampleL = lastSampleAL + thresholdA;
		if (-clamp > thresholdA) inputSampleL = lastSampleAL - thresholdA;
		
		clamp = inputSampleL - lastSampleBL;
		if (clamp > thresholdB) inputSampleL = lastSampleBL + thresholdB;
		if (-clamp > thresholdB) inputSampleL = lastSampleBL - thresholdB;
		
		clamp = inputSampleL - lastSampleCL;
		if (clamp > thresholdC) inputSampleL = lastSampleCL + thresholdC;
		if (-clamp > thresholdC) inputSampleL = lastSampleCL - thresholdC;
		
		clamp = inputSampleL - lastSampleDL;
		if (clamp > thresholdD) inputSampleL = lastSampleDL + thresholdD;
		if (-clamp > thresholdD) inputSampleL = lastSampleDL - thresholdD;
		
		clamp = inputSampleL - lastSampleEL;
		if (clamp > thresholdE) inputSampleL = lastSampleEL + thresholdE;
		if (-clamp > thresholdE) inputSampleL = lastSampleEL - thresholdE;
		
		clamp = inputSampleL - lastSampleFL;
		if (clamp > thresholdF) inputSampleL = lastSampleFL + thresholdF;
		if (-clamp > thresholdF) inputSampleL = lastSampleFL - thresholdF;
		
		clamp = inputSampleL - lastSampleGL;
		if (clamp > thresholdG) inputSampleL = lastSampleGL + thresholdG;
		if (-clamp > thresholdG) inputSampleL = lastSampleGL - thresholdG;
		
		clamp = inputSampleL - lastSampleHL;
		if (clamp > thresholdH) inputSampleL = lastSampleHL + thresholdH;
		if (-clamp > thresholdH) inputSampleL = lastSampleHL - thresholdH;
		
		clamp = inputSampleL - lastSampleIL;
		if (clamp > thresholdI) inputSampleL = lastSampleIL + thresholdI;
		if (-clamp > thresholdI) inputSampleL = lastSampleIL - thresholdI;
		
		clamp = inputSampleL - lastSampleJL;
		if (clamp > thresholdJ) inputSampleL = lastSampleJL + thresholdJ;
		if (-clamp > thresholdJ) inputSampleL = lastSampleJL - thresholdJ;
		
		clamp = inputSampleL - lastSampleKL;
		if (clamp > thresholdK) inputSampleL = lastSampleKL + thresholdK;
		if (-clamp > thresholdK) inputSampleL = lastSampleKL - thresholdK;
		
		clamp = inputSampleL - lastSampleLL;
		if (clamp > thresholdL) inputSampleL = lastSampleLL + thresholdL;
		if (-clamp > thresholdL) inputSampleL = lastSampleLL - thresholdL;
		
		clamp = inputSampleL - lastSampleML;
		if (clamp > thresholdM) inputSampleL = lastSampleML + thresholdM;
		if (-clamp > thresholdM) inputSampleL = lastSampleML - thresholdM;
		
		
		lastSampleML = lastSampleLL;
		lastSampleLL = lastSampleKL;
		lastSampleKL = lastSampleJL;
		lastSampleJL = lastSampleIL;
		lastSampleIL = lastSampleHL;
		lastSampleHL = lastSampleGL;
		lastSampleGL = lastSampleFL;
		lastSampleFL = lastSampleEL;
		lastSampleEL = lastSampleDL;
		lastSampleDL = lastSampleCL;
		lastSampleCL = lastSampleBL;
		lastSampleBL = lastSampleAL;
		lastSampleAL = drySampleL;
		//store the raw L input sample again for use next time
		
		clamp = inputSampleR - lastSampleAR;
		if (clamp > thresholdA) inputSampleR = lastSampleAR + thresholdA;
		if (-clamp > thresholdA) inputSampleR = lastSampleAR - thresholdA;
		
		clamp = inputSampleR - lastSampleBR;
		if (clamp > thresholdB) inputSampleR = lastSampleBR + thresholdB;
		if (-clamp > thresholdB) inputSampleR = lastSampleBR - thresholdB;
		
		clamp = inputSampleR - lastSampleCR;
		if (clamp > thresholdC) inputSampleR = lastSampleCR + thresholdC;
		if (-clamp > thresholdC) inputSampleR = lastSampleCR - thresholdC;
		
		clamp = inputSampleR - lastSampleDR;
		if (clamp > thresholdD) inputSampleR = lastSampleDR + thresholdD;
		if (-clamp > thresholdD) inputSampleR = lastSampleDR - thresholdD;
		
		clamp = inputSampleR - lastSampleER;
		if (clamp > thresholdE) inputSampleR = lastSampleER + thresholdE;
		if (-clamp > thresholdE) inputSampleR = lastSampleER - thresholdE;
		
		clamp = inputSampleR - lastSampleFR;
		if (clamp > thresholdF) inputSampleR = lastSampleFR + thresholdF;
		if (-clamp > thresholdF) inputSampleR = lastSampleFR - thresholdF;
		
		clamp = inputSampleR - lastSampleGR;
		if (clamp > thresholdG) inputSampleR = lastSampleGR + thresholdG;
		if (-clamp > thresholdG) inputSampleR = lastSampleGR - thresholdG;
		
		clamp = inputSampleR - lastSampleHR;
		if (clamp > thresholdH) inputSampleR = lastSampleHR + thresholdH;
		if (-clamp > thresholdH) inputSampleR = lastSampleHR - thresholdH;
		
		clamp = inputSampleR - lastSampleIR;
		if (clamp > thresholdI) inputSampleR = lastSampleIR + thresholdI;
		if (-clamp > thresholdI) inputSampleR = lastSampleIR - thresholdI;
		
		clamp = inputSampleR - lastSampleJR;
		if (clamp > thresholdJ) inputSampleR = lastSampleJR + thresholdJ;
		if (-clamp > thresholdJ) inputSampleR = lastSampleJR - thresholdJ;
		
		clamp = inputSampleR - lastSampleKR;
		if (clamp > thresholdK) inputSampleR = lastSampleKR + thresholdK;
		if (-clamp > thresholdK) inputSampleR = lastSampleKR - thresholdK;
		
		clamp = inputSampleR - lastSampleLR;
		if (clamp > thresholdL) inputSampleR = lastSampleLR + thresholdL;
		if (-clamp > thresholdL) inputSampleR = lastSampleLR - thresholdL;
		
		clamp = inputSampleR - lastSampleMR;
		if (clamp > thresholdM) inputSampleR = lastSampleMR + thresholdM;
		if (-clamp > thresholdM) inputSampleR = lastSampleMR - thresholdM;
		
		
		lastSampleMR = lastSampleLR;
		lastSampleLR = lastSampleKR;
		lastSampleKR = lastSampleJR;
		lastSampleJR = lastSampleIR;
		lastSampleIR = lastSampleHR;
		lastSampleHR = lastSampleGR;
		lastSampleGR = lastSampleFR;
		lastSampleFR = lastSampleER;
		lastSampleER = lastSampleDR;
		lastSampleDR = lastSampleCR;
		lastSampleCR = lastSampleBR;
		lastSampleBR = lastSampleAR;
		lastSampleAR = drySampleR;
		//store the raw R input sample again for use next time
		
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}} // end namespace
