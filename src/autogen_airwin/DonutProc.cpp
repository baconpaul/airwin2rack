/* ========================================
 *  Donut - Donut.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Donut_H
#include "Donut.h"
#endif
#include <cstdlib>
namespace airwinconsolidated::Donut {

void Donut::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double bezRez = pow(1.0-A, 6.0) / overallscale; 
	double sloRez = pow(1.0-B, 6.0) / overallscale;
	bezRez = fmin(fmax(bezRez,0.00001),1.0);
	sloRez = fmin(fmax(sloRez,0.00001),1.0);
	freqA = freqB; resoA = resoB; outA = outB;
	freqB = pow(C,overallscale+1.0)*1.225;
	double movFreq = (D*2.0)-1.0;
	resoB = pow(1.0-E,2.0);
	if (resoB < 0.001) resoB = 0.001; // q of 0.0 is just a tone
	double movReso = (F*-2.0)+1.0;
	outB = G/sqrt(resoB);
	
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
		
		double ctrl = fabs(inputSampleL);
		bezMinL = fmax(bezMinL-sloRez,ctrl);
		bezCompL[bez_cycle] += bezRez;
		bezCompL[bez_Ctrl] += (bezMinL * bezRez);
		if (bezCompL[bez_cycle] > 1.0) {
			bezCompL[bez_cycle] -= 1.0;
			bezCompL[bez_C] = bezCompL[bez_B];
			bezCompL[bez_B] = bezCompL[bez_A];
			bezCompL[bez_A] = bezCompL[bez_Ctrl];
			bezCompL[bez_Ctrl] = 0.0;
		}
		double CB = (bezCompL[bez_C]*(1.0-bezCompL[bez_cycle]))+(bezCompL[bez_B]*bezCompL[bez_cycle]);
		double BA = (bezCompL[bez_B]*(1.0-bezCompL[bez_cycle]))+(bezCompL[bez_A]*bezCompL[bez_cycle]);
		double CBA = (bezCompL[bez_B]+(CB*(1.0-bezCompL[bez_cycle]))+(BA*bezCompL[bez_cycle]))*0.5;
		double mFreq = fmin(fmax(freq+(CBA*movFreq),0.004/overallscale),1.225);
		double mReso = fmin(fmax(reso+(CBA*movReso),0.001),1.0);
		lowL += mFreq*bandL; bandL += mFreq*((mReso*inputSampleL)-lowL-(mReso*bandL));
		inputSampleL = (lowL-sin(bandL*0.5))*out; //airwin-donut	
		
		ctrl = fabs(inputSampleR);
		bezMinR = fmax(bezMinR-sloRez,ctrl);
		bezCompR[bez_cycle] += bezRez;
		bezCompR[bez_Ctrl] += (bezMinR * bezRez);
		if (bezCompR[bez_cycle] > 1.0) {
			bezCompR[bez_cycle] -= 1.0;
			bezCompR[bez_C] = bezCompR[bez_B];
			bezCompR[bez_B] = bezCompR[bez_A];
			bezCompR[bez_A] = bezCompR[bez_Ctrl];
			bezCompR[bez_Ctrl] = 0.0;
		}
		CB = (bezCompR[bez_C]*(1.0-bezCompR[bez_cycle]))+(bezCompR[bez_B]*bezCompR[bez_cycle]);
		BA = (bezCompR[bez_B]*(1.0-bezCompR[bez_cycle]))+(bezCompR[bez_A]*bezCompR[bez_cycle]);
		CBA = (bezCompR[bez_B]+(CB*(1.0-bezCompR[bez_cycle]))+(BA*bezCompR[bez_cycle]))*0.5;
		mFreq = fmin(fmax(freq+(CBA*movFreq),0.004/overallscale),1.225);
		mReso = fmin(fmax(reso+(CBA*movReso),0.001),1.0);
		lowR += mFreq*bandR; bandR += mFreq*((mReso*inputSampleR)-lowR-(mReso*bandR));
		inputSampleR = (lowR-sin(bandR*0.5))*out; //airwin-donut	
		
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

void Donut::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double bezRez = pow(1.0-A, 6.0) / overallscale; 
	double sloRez = pow(1.0-B, 6.0) / overallscale;
	bezRez = fmin(fmax(bezRez,0.00001),1.0);
	sloRez = fmin(fmax(sloRez,0.00001),1.0);
	freqA = freqB; resoA = resoB; outA = outB;
	freqB = pow(C,overallscale+1.0)*1.225;
	double movFreq = (D*2.0)-1.0;
	resoB = pow(1.0-E,2.0);
	if (resoB < 0.001) resoB = 0.001; // q of 0.0 is just a tone
	double movReso = (F*-2.0)+1.0;
	outB = G/sqrt(resoB);
	
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
		
		double ctrl = fabs(inputSampleL);
		bezMinL = fmax(bezMinL-sloRez,ctrl);
		bezCompL[bez_cycle] += bezRez;
		bezCompL[bez_Ctrl] += (bezMinL * bezRez);
		if (bezCompL[bez_cycle] > 1.0) {
			bezCompL[bez_cycle] -= 1.0;
			bezCompL[bez_C] = bezCompL[bez_B];
			bezCompL[bez_B] = bezCompL[bez_A];
			bezCompL[bez_A] = bezCompL[bez_Ctrl];
			bezCompL[bez_Ctrl] = 0.0;
		}
		double CB = (bezCompL[bez_C]*(1.0-bezCompL[bez_cycle]))+(bezCompL[bez_B]*bezCompL[bez_cycle]);
		double BA = (bezCompL[bez_B]*(1.0-bezCompL[bez_cycle]))+(bezCompL[bez_A]*bezCompL[bez_cycle]);
		double CBA = (bezCompL[bez_B]+(CB*(1.0-bezCompL[bez_cycle]))+(BA*bezCompL[bez_cycle]))*0.5;
		double mFreq = fmin(fmax(freq+(CBA*movFreq),0.004/overallscale),1.225);
		double mReso = fmin(fmax(reso+(CBA*movReso),0.001),1.0);
		lowL += mFreq*bandL; bandL += mFreq*((mReso*inputSampleL)-lowL-(mReso*bandL));
		inputSampleL = (lowL-sin(bandL*0.5))*out; //airwin-donut	
		
		ctrl = fabs(inputSampleR);
		bezMinR = fmax(bezMinR-sloRez,ctrl);
		bezCompR[bez_cycle] += bezRez;
		bezCompR[bez_Ctrl] += (bezMinR * bezRez);
		if (bezCompR[bez_cycle] > 1.0) {
			bezCompR[bez_cycle] -= 1.0;
			bezCompR[bez_C] = bezCompR[bez_B];
			bezCompR[bez_B] = bezCompR[bez_A];
			bezCompR[bez_A] = bezCompR[bez_Ctrl];
			bezCompR[bez_Ctrl] = 0.0;
		}
		CB = (bezCompR[bez_C]*(1.0-bezCompR[bez_cycle]))+(bezCompR[bez_B]*bezCompR[bez_cycle]);
		BA = (bezCompR[bez_B]*(1.0-bezCompR[bez_cycle]))+(bezCompR[bez_A]*bezCompR[bez_cycle]);
		CBA = (bezCompR[bez_B]+(CB*(1.0-bezCompR[bez_cycle]))+(BA*bezCompR[bez_cycle]))*0.5;
		mFreq = fmin(fmax(freq+(CBA*movFreq),0.004/overallscale),1.225);
		mReso = fmin(fmax(reso+(CBA*movReso),0.001),1.0);
		lowR += mFreq*bandR; bandR += mFreq*((mReso*inputSampleR)-lowR-(mReso*bandR));
		inputSampleR = (lowR-sin(bandR*0.5))*out; //airwin-donut	
		
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
