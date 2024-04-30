/* ========================================
 *  Console5DarkCh - Console5DarkCh.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Console5DarkCh_H
#include "Console5DarkCh.h"
#endif
namespace airwinconsolidated::Console5DarkCh {

void Console5DarkCh::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double inputgain = A;
	double differenceL;
	double differenceR;
	double nearZeroL;
	double nearZeroR;
	double servoTrim = 0.0000001 / overallscale;
	double bassTrim = 0.005 / overallscale;
	double inputSampleL;
	double inputSampleR;
	
	if (settingchase != inputgain) {
		chasespeed *= 2.0;
		settingchase = inputgain;
	}
	if (chasespeed > 2500.0) chasespeed = 2500.0;
	if (gainchase < 0.0) gainchase = inputgain;
	
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
		
		differenceL = lastSampleChannelL - inputSampleL;
		lastSampleChannelL = inputSampleL;
		differenceR = lastSampleChannelR - inputSampleR;
		lastSampleChannelR = inputSampleR;
		//derive slew part off direct sample measurement + from last time
		
		if (differenceL > 1.0) differenceL = 1.0;
		if (differenceL < -1.0) differenceL = -1.0;
		if (differenceR > 1.0) differenceR = 1.0;
		if (differenceR < -1.0) differenceR = -1.0;
		//clamp the slew correction to prevent invalid math results
		
		differenceL = lastFXChannelL + sin(differenceL);
		differenceR = lastFXChannelR + sin(differenceR);
		//we're about to use this twice and then not use difference again, so we'll reuse it
		//enhance slew is arcsin(): cutting it back is sin()
		
		iirCorrectL += inputSampleL - differenceL;
		inputSampleL = differenceL;
		iirCorrectR += inputSampleR - differenceR;
		inputSampleR = differenceR;
		//apply the slew to stored value: can develop DC offsets.
		//store the change we made so we can dial it back
		
		lastFXChannelL = inputSampleL;
		lastFXChannelR = inputSampleR;
		if (lastFXChannelL > 1.0) lastFXChannelL = 1.0;
		if (lastFXChannelL < -1.0) lastFXChannelL = -1.0;
		if (lastFXChannelR > 1.0) lastFXChannelR = 1.0;
		if (lastFXChannelR < -1.0) lastFXChannelR = -1.0;
		//store current sample as new base for next offset
		
		nearZeroL = pow(fabs(fabs(lastFXChannelL)-1.0), 2);
		nearZeroR = pow(fabs(fabs(lastFXChannelR)-1.0), 2);
		//if the sample is very near zero this number is higher.
		if (iirCorrectL > 0) iirCorrectL -= servoTrim;
		if (iirCorrectL < 0) iirCorrectL += servoTrim;
		if (iirCorrectR > 0) iirCorrectR -= servoTrim;
		if (iirCorrectR < 0) iirCorrectR += servoTrim;
		//cut back the servo by which we're pulling back the DC
		lastFXChannelL += (iirCorrectL * 0.0000005);
		lastFXChannelR += (iirCorrectR * 0.0000005);
		//apply the servo to the stored value, pulling back the DC
		lastFXChannelL *= (1.0 - (nearZeroL * bassTrim));
		lastFXChannelR *= (1.0 - (nearZeroR * bassTrim));
		//this cuts back the DC offset directly, relative to how near zero we are		
		
		if (inputSampleL > 1.57079633) inputSampleL= 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//amplitude aspect
		
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//amplitude aspect
		
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

void Console5DarkCh::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double inputgain = A;
	double differenceL;
	double differenceR;
	double nearZeroL;
	double nearZeroR;
	double servoTrim = 0.0000001 / overallscale;
	double bassTrim = 0.005 / overallscale;
	double inputSampleL;
	double inputSampleR;
	
	if (settingchase != inputgain) {
		chasespeed *= 2.0;
		settingchase = inputgain;
	}
	if (chasespeed > 2500.0) chasespeed = 2500.0;
	if (gainchase < 0.0) gainchase = inputgain;
	
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
		
		differenceL = lastSampleChannelL - inputSampleL;
		lastSampleChannelL = inputSampleL;
		differenceR = lastSampleChannelR - inputSampleR;
		lastSampleChannelR = inputSampleR;
		//derive slew part off direct sample measurement + from last time
		
		if (differenceL > 1.0) differenceL = 1.0;
		if (differenceL < -1.0) differenceL = -1.0;
		if (differenceR > 1.0) differenceR = 1.0;
		if (differenceR < -1.0) differenceR = -1.0;
		//clamp the slew correction to prevent invalid math results
		
		differenceL = lastFXChannelL + sin(differenceL);
		differenceR = lastFXChannelR + sin(differenceR);
		//we're about to use this twice and then not use difference again, so we'll reuse it
		//enhance slew is arcsin(): cutting it back is sin()
		
		iirCorrectL += inputSampleL - differenceL;
		inputSampleL = differenceL;
		iirCorrectR += inputSampleR - differenceR;
		inputSampleR = differenceR;
		//apply the slew to stored value: can develop DC offsets.
		//store the change we made so we can dial it back
		
		lastFXChannelL = inputSampleL;
		lastFXChannelR = inputSampleR;
		if (lastFXChannelL > 1.0) lastFXChannelL = 1.0;
		if (lastFXChannelL < -1.0) lastFXChannelL = -1.0;
		if (lastFXChannelR > 1.0) lastFXChannelR = 1.0;
		if (lastFXChannelR < -1.0) lastFXChannelR = -1.0;
		//store current sample as new base for next offset
		
		nearZeroL = pow(fabs(fabs(lastFXChannelL)-1.0), 2);
		nearZeroR = pow(fabs(fabs(lastFXChannelR)-1.0), 2);
		//if the sample is very near zero this number is higher.
		if (iirCorrectL > 0) iirCorrectL -= servoTrim;
		if (iirCorrectL < 0) iirCorrectL += servoTrim;
		if (iirCorrectR > 0) iirCorrectR -= servoTrim;
		if (iirCorrectR < 0) iirCorrectR += servoTrim;
		//cut back the servo by which we're pulling back the DC
		lastFXChannelL += (iirCorrectL * 0.0000005);
		lastFXChannelR += (iirCorrectR * 0.0000005);
		//apply the servo to the stored value, pulling back the DC
		lastFXChannelL *= (1.0 - (nearZeroL * bassTrim));
		lastFXChannelR *= (1.0 - (nearZeroR * bassTrim));
		//this cuts back the DC offset directly, relative to how near zero we are		
		
		if (inputSampleL > 1.57079633) inputSampleL= 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//amplitude aspect
		
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//amplitude aspect
		
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
