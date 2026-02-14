/* ========================================
 *  BezEQ2 - BezEQ2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BezEQ2_H
#include "BezEQ2.h"
#endif
#include <cstdlib>
namespace airwinconsolidated::BezEQ2 {

void BezEQ2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double trebleGain = (A * 2.0);
	double derezA = B/overallscale;
	if (derezA < 0.00001) derezA = 0.00001; if (derezA > 1.0) derezA = 1.0;
	int midDelay = (int)(1.0/derezA);
	if (midDelay > 4096) midDelay = 4096;
	derezA = 1.0 / midDelay;
	double midTrim = (double)midDelay/(midDelay+1.0);
	midTrim = 1.0-(derezA*midTrim);
	midDelay = (int)(midDelay*0.5*midTrim);
	double midGain = (C * 2.0);
	
	double derezB = pow(D,2.0)/overallscale;
	if (derezB < 0.00001) derezB = 0.00001; if (derezB > 1.0) derezB = 1.0;
	int bassDelay = (int)(1.0/derezB);
	if (bassDelay > 4096) bassDelay = 4096;
	derezB = 1.0 / bassDelay;
	double bassTrim = (double)bassDelay/(bassDelay+1.0);
	bassTrim = 1.0-(derezB*bassTrim);
	bassDelay = (int)(bassDelay*0.5*bassTrim);
	double bassGain = (E * 2.0);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		bezA[bez_cycle] += derezA;
		bezA[bez_SampL] += (inputSampleL * derezA);
		bezA[bez_SampR] += (inputSampleR * derezA);
		
		if (bezA[bez_cycle] > 1.0) {
			bezA[bez_cycle] = 0.0;
			bezA[bez_CL] = bezA[bez_BL];
			bezA[bez_BL] = bezA[bez_AL];
			bezA[bez_AL] = bezA[bez_SampL];
			bezA[bez_SampL] = 0.0;
			bezA[bez_CR] = bezA[bez_BR];
			bezA[bez_BR] = bezA[bez_AR];
			bezA[bez_AR] = bezA[bez_SampR];
			bezA[bez_SampR] = 0.0;
		}
		double X = bezA[bez_cycle]*midTrim;
		double CBL = (bezA[bez_CL]*(1.0-X))+(bezA[bez_BL]*X);
		double BAL = (bezA[bez_BL]*(1.0-X))+(bezA[bez_AL]*X);
		double midL = (bezA[bez_BL]+(CBL*(1.0-X))+(BAL*X))*0.25;
		double CBR = (bezA[bez_CR]*(1.0-X))+(bezA[bez_BR]*X);
		double BAR = (bezA[bez_BR]*(1.0-X))+(bezA[bez_AR]*X);
		double midR = (bezA[bez_BR]+(CBR*(1.0-X))+(BAR*X))*0.25;
		
		//predelay
		aL[countA] = inputSampleL;
		aR[countA] = inputSampleR;
		countA++; if (countA < 0 || countA > midDelay) countA = 0;
		inputSampleL = aL[countA-((countA > midDelay)?midDelay+1:0)];
		inputSampleR = aR[countA-((countA > midDelay)?midDelay+1:0)];
		//end predelay
		
		double trebleL = inputSampleL - midL;
		double trebleR = inputSampleR - midR;
		
		bezB[bez_cycle] += derezB;
		bezB[bez_SampL] += (midL * derezB);
		bezB[bez_SampR] += (midR * derezB);
		
		if (bezB[bez_cycle] > 1.0) {
			bezB[bez_cycle] = 0.0;
			bezB[bez_CL] = bezB[bez_BL];
			bezB[bez_BL] = bezB[bez_AL];
			bezB[bez_AL] = bezB[bez_SampL];
			bezB[bez_SampL] = 0.0;
			bezB[bez_CR] = bezB[bez_BR];
			bezB[bez_BR] = bezB[bez_AR];
			bezB[bez_AR] = bezB[bez_SampR];
			bezB[bez_SampR] = 0.0;
		}
		X = bezB[bez_cycle]*bassTrim;
		CBL = (bezB[bez_CL]*(1.0-X))+(bezB[bez_BL]*X);
		BAL = (bezB[bez_BL]*(1.0-X))+(bezB[bez_AL]*X);
		double bassL = (bezB[bez_BL]+(CBL*(1.0-X))+(BAL*X))*0.25;
		CBR = (bezB[bez_CR]*(1.0-X))+(bezB[bez_BR]*X);
		BAR = (bezB[bez_BR]*(1.0-X))+(bezB[bez_AR]*X);
		double bassR = (bezB[bez_BR]+(CBR*(1.0-X))+(BAR*X))*0.25;
		
		if (midGain > 0.0001) {
			midL += (trebleL*(trebleGain/midGain));
			midR += (trebleR*(trebleGain/midGain));
		}
		
		//predelay
		bL[countB] = midL;
		bR[countB] = midR;
		countB++; if (countB < 0 || countB > bassDelay) countB = 0;
		midL = bL[countB-((countB > bassDelay)?bassDelay+1:0)];
		midR = bR[countB-((countB > bassDelay)?bassDelay+1:0)];
		//end predelay
		
		midL -= bassL;
		midR -= bassR;
		inputSampleL = (bassL*bassGain) + (midL*midGain);
		inputSampleR = (bassR*bassGain) + (midR*midGain);
		
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

void BezEQ2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double trebleGain = (A * 2.0);
	double derezA = B/overallscale;
	if (derezA < 0.00001) derezA = 0.00001; if (derezA > 1.0) derezA = 1.0;
	int midDelay = (int)(1.0/derezA);
	if (midDelay > 4096) midDelay = 4096;
	derezA = 1.0 / midDelay;
	double midTrim = (double)midDelay/(midDelay+1.0);
	midTrim = 1.0-(derezA*midTrim);
	midDelay = (int)(midDelay*0.5*midTrim);
	double midGain = (C * 2.0);
	
	double derezB = pow(D,2.0)/overallscale;
	if (derezB < 0.00001) derezB = 0.00001; if (derezB > 1.0) derezB = 1.0;
	int bassDelay = (int)(1.0/derezB);
	if (bassDelay > 4096) bassDelay = 4096;
	derezB = 1.0 / bassDelay;
	double bassTrim = (double)bassDelay/(bassDelay+1.0);
	bassTrim = 1.0-(derezB*bassTrim);
	bassDelay = (int)(bassDelay*0.5*bassTrim);
	double bassGain = (E * 2.0);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		bezA[bez_cycle] += derezA;
		bezA[bez_SampL] += (inputSampleL * derezA);
		bezA[bez_SampR] += (inputSampleR * derezA);
		
		if (bezA[bez_cycle] > 1.0) {
			bezA[bez_cycle] = 0.0;
			bezA[bez_CL] = bezA[bez_BL];
			bezA[bez_BL] = bezA[bez_AL];
			bezA[bez_AL] = bezA[bez_SampL];
			bezA[bez_SampL] = 0.0;
			bezA[bez_CR] = bezA[bez_BR];
			bezA[bez_BR] = bezA[bez_AR];
			bezA[bez_AR] = bezA[bez_SampR];
			bezA[bez_SampR] = 0.0;
		}
		double X = bezA[bez_cycle]*midTrim;
		double CBL = (bezA[bez_CL]*(1.0-X))+(bezA[bez_BL]*X);
		double BAL = (bezA[bez_BL]*(1.0-X))+(bezA[bez_AL]*X);
		double midL = (bezA[bez_BL]+(CBL*(1.0-X))+(BAL*X))*0.25;
		double CBR = (bezA[bez_CR]*(1.0-X))+(bezA[bez_BR]*X);
		double BAR = (bezA[bez_BR]*(1.0-X))+(bezA[bez_AR]*X);
		double midR = (bezA[bez_BR]+(CBR*(1.0-X))+(BAR*X))*0.25;
		
		//predelay
		aL[countA] = inputSampleL;
		aR[countA] = inputSampleR;
		countA++; if (countA < 0 || countA > midDelay) countA = 0;
		inputSampleL = aL[countA-((countA > midDelay)?midDelay+1:0)];
		inputSampleR = aR[countA-((countA > midDelay)?midDelay+1:0)];
		//end predelay
		
		double trebleL = inputSampleL - midL;
		double trebleR = inputSampleR - midR;
		
		bezB[bez_cycle] += derezB;
		bezB[bez_SampL] += (midL * derezB);
		bezB[bez_SampR] += (midR * derezB);
		
		if (bezB[bez_cycle] > 1.0) {
			bezB[bez_cycle] = 0.0;
			bezB[bez_CL] = bezB[bez_BL];
			bezB[bez_BL] = bezB[bez_AL];
			bezB[bez_AL] = bezB[bez_SampL];
			bezB[bez_SampL] = 0.0;
			bezB[bez_CR] = bezB[bez_BR];
			bezB[bez_BR] = bezB[bez_AR];
			bezB[bez_AR] = bezB[bez_SampR];
			bezB[bez_SampR] = 0.0;
		}
		X = bezB[bez_cycle]*bassTrim;
		CBL = (bezB[bez_CL]*(1.0-X))+(bezB[bez_BL]*X);
		BAL = (bezB[bez_BL]*(1.0-X))+(bezB[bez_AL]*X);
		double bassL = (bezB[bez_BL]+(CBL*(1.0-X))+(BAL*X))*0.25;
		CBR = (bezB[bez_CR]*(1.0-X))+(bezB[bez_BR]*X);
		BAR = (bezB[bez_BR]*(1.0-X))+(bezB[bez_AR]*X);
		double bassR = (bezB[bez_BR]+(CBR*(1.0-X))+(BAR*X))*0.25;
		
		if (midGain > 0.0001) {
			midL += (trebleL*(trebleGain/midGain));
			midR += (trebleR*(trebleGain/midGain));
		}
		
		//predelay
		bL[countB] = midL;
		bR[countB] = midR;
		countB++; if (countB < 0 || countB > bassDelay) countB = 0;
		midL = bL[countB-((countB > bassDelay)?bassDelay+1:0)];
		midR = bR[countB-((countB > bassDelay)?bassDelay+1:0)];
		//end predelay
		
		midL -= bassL;
		midR -= bassR;
		inputSampleL = (bassL*bassGain) + (midL*midGain);
		inputSampleR = (bassR*bassGain) + (midR*midGain);
		
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
