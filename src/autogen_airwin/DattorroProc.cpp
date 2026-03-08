/* ========================================
 *  Dattorro - Dattorro.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Dattorro_H
#include "Dattorro.h"
#endif
#include <cstdlib>
namespace airwinconsolidated::Dattorro {

void Dattorro::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
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
	freqB = pow(A,overallscale+1.0)*1.225;
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
		
		lowL += freq*bandL; bandL += freq*((reso*inputSampleL)-lowL-(reso*bandL));
		inputSampleL = (lowL-sin(bandL*0.5))*out;
		
		lowR += freq*bandR; bandR += freq*((reso*inputSampleR)-lowR-(reso*bandR));
		inputSampleR = (lowR-sin(bandR*0.5))*out; //airwindattorro
		
		//since this is called Dattorro, I'm including a variation on
		//the textbook code for this, so you can have the normal SVF
		//on tap if you want its lowpass, bandpass or highpass.
		//You can steepen it by cascading additional layers of SVF.
		//the Dattorro source does not produce correct frequencies.
		//it uses cutoff = GetParameter( kParam_A )*20000.0;
		//and then f = 2.0*sin(M_PI * (cutoff / GetSampleRate()));
		//this causes a crash when f is higher than 0.25 Nyquist
		//and also doesn't return the right frequency
		//-------- here is the controls code, for outside the buffer
		//double f = pow(GetParameter( kParam_A ),overallscale+1.0)*1.225;
		//double q = pow(1.0-GetParameter( kParam_B ),2.0); //reso
		//if (q < 0.001) q = 0.001; // q of 0.0 is just a tone
		//double outL = GetParameter( kParam_C ); //lowpass output
		//double outB = 0.0; //bandpass output
		//double outH = 0.0; //highpass output
		//notch output is simply highpass+lowpass
		//-------- here is the audio code, inside the buffer
		//low += f*band;
		//band += f*((q*inputSample)-low-(q*band));
		//const double high = (q*inputSample) - low - (q*band);
		//band += f*high;
		//inputSample = 0.0; //now let's build from the outputs
		//inputSample += low*outL;
		//inputSample += band*outB;
		//inputSample += high*outH;
		//-------- and we're done, that's a Dattorro SVF
		
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

void Dattorro::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
	freqB = pow(A,overallscale+1.0)*1.225;
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
		
		lowL += freq*bandL; bandL += freq*((reso*inputSampleL)-lowL-(reso*bandL));
		inputSampleL = (lowL-sin(bandL*0.5))*out;
		
		lowR += freq*bandR; bandR += freq*((reso*inputSampleR)-lowR-(reso*bandR));
		inputSampleR = (lowR-sin(bandR*0.5))*out; //airwindattorro
		
		//since this is called Dattorro, I'm including a variation on
		//the textbook code for this, so you can have the normal SVF
		//on tap if you want its lowpass, bandpass or highpass.
		//You can steepen it by cascading additional layers of SVF.
		//the Dattorro source does not produce correct frequencies.
		//it uses cutoff = GetParameter( kParam_A )*20000.0;
		//and then f = 2.0*sin(M_PI * (cutoff / GetSampleRate()));
		//this causes a crash when f is higher than 0.25 Nyquist
		//and also doesn't return the right frequency
		//-------- here is the controls code, for outside the buffer
		//double f = pow(GetParameter( kParam_A ),overallscale+1.0)*1.225;
		//double q = pow(1.0-GetParameter( kParam_B ),2.0); //reso
		//if (q < 0.001) q = 0.001; // q of 0.0 is just a tone
		//double outL = GetParameter( kParam_C ); //lowpass output
		//double outB = 0.0; //bandpass output
		//double outH = 0.0; //highpass output
		//notch output is simply highpass+lowpass
		//-------- here is the audio code, inside the buffer
		//low += f*band;
		//band += f*((q*inputSample)-low-(q*band));
		//const double high = (q*inputSample) - low - (q*band);
		//band += f*high;
		//inputSample = 0.0; //now let's build from the outputs
		//inputSample += low*outL;
		//inputSample += band*outB;
		//inputSample += high*outH;
		//-------- and we're done, that's a Dattorro SVF
		
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
