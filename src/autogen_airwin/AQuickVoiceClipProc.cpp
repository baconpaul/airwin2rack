/* ========================================
 *  AQuickVoiceClip - AQuickVoiceClip.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __AQuickVoiceClip_H
#include "AQuickVoiceClip.h"
#endif
namespace airwinconsolidated::AQuickVoiceClip {

void AQuickVoiceClip::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double softness = 0.484416;
	double hardness = 1.0 - softness;
	double iirAmount = ((pow(A,3)*2070)+30)/8000.0;
	iirAmount /= overallscale;
	double altAmount = (1.0 - iirAmount);
	double cancelnew = 0.0682276;
	double cancelold = 1.0 - cancelnew;
	double lpSpeed = 0.0009;
	double cliplevel = 0.98;
	double refclip = 0.5; //preset to cut out gain quite a lot. 91%? no touchy unless clip
	
	double LmaxRecent;
	bool LclipOnset;
	double LpassThrough;
	double LoutputSample;
	double LdrySample;
	
	double RmaxRecent;
	bool RclipOnset;
	double RpassThrough;
	double RoutputSample;
	double RdrySample;
	

	double inputSampleL;
	double inputSampleR;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		LpassThrough = LataDrySample = inputSampleL;
		RpassThrough = RataDrySample = inputSampleR;
		
		LataHalfDrySample = LataHalfwaySample = (inputSampleL + LataLast1Sample + (LataLast2Sample*ataK1) + (LataLast3Sample*ataK2) + (LataLast4Sample*ataK6) + (LataLast5Sample*ataK7) + (LataLast6Sample*ataK8)) / 2.0;
		LataLast6Sample = LataLast5Sample; LataLast5Sample = LataLast4Sample; LataLast4Sample = LataLast3Sample; LataLast3Sample = LataLast2Sample; LataLast2Sample = LataLast1Sample; LataLast1Sample = inputSampleL;
		//setting up oversampled special antialiasing
		RataHalfDrySample = RataHalfwaySample = (inputSampleR + RataLast1Sample + (RataLast2Sample*ataK1) + (RataLast3Sample*ataK2) + (RataLast4Sample*ataK6) + (RataLast5Sample*ataK7) + (RataLast6Sample*ataK8)) / 2.0;
		RataLast6Sample = RataLast5Sample; RataLast5Sample = RataLast4Sample; RataLast4Sample = RataLast3Sample; RataLast3Sample = RataLast2Sample; RataLast2Sample = RataLast1Sample; RataLast1Sample = inputSampleR;
		//setting up oversampled special antialiasing
		LclipOnset = false;
		RclipOnset = false;
		
		
		LmaxRecent = fabs( LataLast6Sample );
		if (fabs( LataLast5Sample ) > LmaxRecent ) LmaxRecent = fabs( LataLast5Sample );
		if (fabs( LataLast4Sample ) > LmaxRecent ) LmaxRecent = fabs( LataLast4Sample );
		if (fabs( LataLast3Sample ) > LmaxRecent ) LmaxRecent = fabs( LataLast3Sample );
		if (fabs( LataLast2Sample ) > LmaxRecent ) LmaxRecent = fabs( LataLast2Sample );
		if (fabs( LataLast1Sample ) > LmaxRecent ) LmaxRecent = fabs( LataLast1Sample );
		if (fabs( inputSampleL ) > LmaxRecent ) LmaxRecent = fabs( inputSampleL );
		//this gives us something that won't cut out in zero crossings, to interpolate with
		
		RmaxRecent = fabs( RataLast6Sample );
		if (fabs( RataLast5Sample ) > RmaxRecent ) RmaxRecent = fabs( RataLast5Sample );
		if (fabs( RataLast4Sample ) > RmaxRecent ) RmaxRecent = fabs( RataLast4Sample );
		if (fabs( RataLast3Sample ) > RmaxRecent ) RmaxRecent = fabs( RataLast3Sample );
		if (fabs( RataLast2Sample ) > RmaxRecent ) RmaxRecent = fabs( RataLast2Sample );
		if (fabs( RataLast1Sample ) > RmaxRecent ) RmaxRecent = fabs( RataLast1Sample );
		if (fabs( inputSampleR ) > RmaxRecent ) RmaxRecent = fabs( inputSampleR );
		//this gives us something that won't cut out in zero crossings, to interpolate with
		
		LmaxRecent *= 2.0;
		RmaxRecent *= 2.0;
		//by refclip this is 1.0 and fully into the antialiasing
		if (LmaxRecent > 1.0) LmaxRecent = 1.0;
		if (RmaxRecent > 1.0) RmaxRecent = 1.0;
		//and it tops out at 1. Higher means more antialiasing, lower blends into passThrough without antialiasing
		
		LataHalfwaySample -= Loverall;
		RataHalfwaySample -= Roverall;
		//subtract dist-cancel from input after getting raw input, before doing anything
		
		LdrySample = LataHalfwaySample;
		RdrySample = RataHalfwaySample;
		
		
		//begin L channel for the clipper
		if (LlastSample >= refclip)
		{
			LlpDepth += 0.1;
			if (LataHalfwaySample < refclip)
			{
				LlastSample = ((refclip*hardness) + (LataHalfwaySample * softness));
			}
			else LlastSample = refclip;
		}
		
		if (LlastSample <= -refclip)
		{
			LlpDepth += 0.1;
			if (LataHalfwaySample > -refclip)
			{
				LlastSample = ((-refclip*hardness) + (LataHalfwaySample * softness));
			}
			else LlastSample = -refclip;
		}
		
		if (LataHalfwaySample > refclip)
		{
			LlpDepth += 0.1;
			if (LlastSample < refclip)
			{
				LataHalfwaySample = ((refclip*hardness) + (LlastSample * softness));
			}
			else LataHalfwaySample = refclip;
		}
		
		if (LataHalfwaySample < -refclip)
		{
			LlpDepth += 0.1;
			if (LlastSample > -refclip)
			{
				LataHalfwaySample = ((-refclip*hardness) + (LlastSample * softness));
			}
			else LataHalfwaySample = -refclip;
		}
		///end L channel for the clipper
		
		//begin R channel for the clipper
		if (RlastSample >= refclip)
		{
			RlpDepth += 0.1;
			if (RataHalfwaySample < refclip)
			{
				RlastSample = ((refclip*hardness) + (RataHalfwaySample * softness));
			}
			else RlastSample = refclip;
		}
		
		if (RlastSample <= -refclip)
		{
			RlpDepth += 0.1;
			if (RataHalfwaySample > -refclip)
			{
				RlastSample = ((-refclip*hardness) + (RataHalfwaySample * softness));
			}
			else RlastSample = -refclip;
		}
		
		if (RataHalfwaySample > refclip)
		{
			RlpDepth += 0.1;
			if (RlastSample < refclip)
			{
				RataHalfwaySample = ((refclip*hardness) + (RlastSample * softness));
			}
			else RataHalfwaySample = refclip;
		}
		
		if (RataHalfwaySample < -refclip)
		{
			RlpDepth += 0.1;
			if (RlastSample > -refclip)
			{
				RataHalfwaySample = ((-refclip*hardness) + (RlastSample * softness));
			}
			else RataHalfwaySample = -refclip;
		}
		///end R channel for the clipper
		
        LoutputSample = LlastSample;
        RoutputSample = RlastSample;
		
		LlastSample = LataHalfwaySample;
		RlastSample = RataHalfwaySample;

		LataHalfwaySample = LoutputSample;
		RataHalfwaySample = RoutputSample;
		//swap around in a circle for one final ADClip,
		//this time not tracking overshoot anymore
		//end interpolated sample		
		//begin raw sample- inputSample and ataDrySample handled separately here
		
		inputSampleL -= Loverall;
		inputSampleR -= Roverall;
		//subtract dist-cancel from input after getting raw input, before doing anything
		
		LdrySample = inputSampleL;
		RdrySample = inputSampleR;
		
		//begin second L clip
		if (LlastSample >= refclip)
		{
			LlpDepth += 0.1;
			if (inputSampleL < refclip)
			{
				LlastSample = ((refclip*hardness) + (inputSampleL * softness));
			}
			else LlastSample = refclip;
		}
		
		if (LlastSample <= -refclip)
		{
			LlpDepth += 0.1;
			if (inputSampleL > -refclip)
			{
				LlastSample = ((-refclip*hardness) + (inputSampleL * softness));
			}
			else LlastSample = -refclip;
		}
		
		if (inputSampleL > refclip)
		{
			LlpDepth += 0.1;
			if (LlastSample < refclip)
			{
				inputSampleL = ((refclip*hardness) + (LlastSample * softness));
			}
			else inputSampleL = refclip;
		}
		
		if (inputSampleL < -refclip)
		{
			LlpDepth += 0.1;
			if (LlastSample > -refclip)
			{
				inputSampleL = ((-refclip*hardness) + (LlastSample * softness));
			}
			else inputSampleL = -refclip;
		}		
		//end second L clip
		
		//begin second R clip
		if (RlastSample >= refclip)
		{
			RlpDepth += 0.1;
			if (inputSampleR < refclip)
			{
				RlastSample = ((refclip*hardness) + (inputSampleR * softness));
			}
			else RlastSample = refclip;
		}
		
		if (RlastSample <= -refclip)
		{
			RlpDepth += 0.1;
			if (inputSampleR > -refclip)
			{
				RlastSample = ((-refclip*hardness) + (inputSampleR * softness));
			}
			else RlastSample = -refclip;
		}
		
		if (inputSampleR > refclip)
		{
			RlpDepth += 0.1;
			if (RlastSample < refclip)
			{
				inputSampleR = ((refclip*hardness) + (RlastSample * softness));
			}
			else inputSampleR = refclip;
		}
		
		if (inputSampleR < -refclip)
		{
			RlpDepth += 0.1;
			if (RlastSample > -refclip)
			{
				inputSampleR = ((-refclip*hardness) + (RlastSample * softness));
			}
			else inputSampleR = -refclip;
		}		
		//end second R clip
		
		LoutputSample = LlastSample;
		RoutputSample = RlastSample;
		LlastSample = inputSampleL;
		RlastSample = inputSampleR;
		inputSampleL = LoutputSample;
		inputSampleR = RoutputSample;
		
		LataHalfDrySample = (LataDrySample*ataK3)+(LataHalfDrySample*ataK4);
		LataHalfDiffSample = (LataHalfwaySample - LataHalfDrySample)/2.0;
		LataLastDiffSample = LataDiffSample*ataK5;
		LataDiffSample = (inputSampleL - LataDrySample)/2.0;
		LataDiffSample += LataHalfDiffSample;
		LataDiffSample -= LataLastDiffSample;
		inputSampleL = LataDrySample;
		inputSampleL += LataDiffSample;
		
		RataHalfDrySample = (RataDrySample*ataK3)+(RataHalfDrySample*ataK4);
		RataHalfDiffSample = (RataHalfwaySample - RataHalfDrySample)/2.0;
		RataLastDiffSample = RataDiffSample*ataK5;
		RataDiffSample = (inputSampleR - RataDrySample)/2.0;
		RataDiffSample += RataHalfDiffSample;
		RataDiffSample -= RataLastDiffSample;
		inputSampleR = RataDrySample;
		inputSampleR += RataDiffSample;
		
		Loverall = (Loverall * cancelold) + (LataDiffSample * cancelnew);
		Roverall = (Roverall * cancelold) + (RataDiffSample * cancelnew);
		//apply all the diffs to a lowpassed IIR

		
		if (flip)
		{
			LiirSampleA = (LiirSampleA * altAmount) + (inputSampleL * iirAmount);
			inputSampleL -= LiirSampleA;
			LiirSampleC = (LiirSampleC * altAmount) + (LpassThrough * iirAmount);
			LpassThrough -= LiirSampleC;
			
			RiirSampleA = (RiirSampleA * altAmount) + (inputSampleR * iirAmount);
			inputSampleR -= RiirSampleA;
			RiirSampleC = (RiirSampleC * altAmount) + (RpassThrough * iirAmount);
			RpassThrough -= RiirSampleC;
		}
		else
		{
			LiirSampleB = (LiirSampleB * altAmount) + (inputSampleL * iirAmount);
			inputSampleL -= LiirSampleB;
			LiirSampleD = (LiirSampleD * altAmount) + (LpassThrough * iirAmount);
			LpassThrough -= LiirSampleD;
			
			RiirSampleB = (RiirSampleB * altAmount) + (inputSampleR * iirAmount);
			inputSampleR -= RiirSampleB;
			RiirSampleD = (RiirSampleD * altAmount) + (RpassThrough * iirAmount);
			RpassThrough -= RiirSampleD;
		}
		flip = !flip;
		//highpass section 
		
		LlastOut3Sample = LlastOut2Sample;
		LlastOut2Sample = LlastOutSample;
		LlastOutSample = inputSampleL;
		
		RlastOut3Sample = RlastOut2Sample;
		RlastOut2Sample = RlastOutSample;
		RlastOutSample = inputSampleR;
		
		
		LlpDepth -= lpSpeed;
		RlpDepth -= lpSpeed;

		if (LlpDepth > 0.0)
		{
			if (LlpDepth > 1.0) LlpDepth = 1.0;
			inputSampleL *= (1.0-LlpDepth);
			inputSampleL += (((LlastOutSample + LlastOut2Sample + LlastOut3Sample) / 3.6)*LlpDepth);
		}
		
		if (RlpDepth > 0.0)
		{
			if (RlpDepth > 1.0) RlpDepth = 1.0;
			inputSampleR *= (1.0-RlpDepth);
			inputSampleR += (((RlastOutSample + RlastOut2Sample + RlastOut3Sample) / 3.6)*RlpDepth);
		}
		
		if (LlpDepth < 0.0) LlpDepth = 0.0;
		if (RlpDepth < 0.0) RlpDepth = 0.0;
				
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		inputSampleL *= (1.0-LmaxRecent);
		inputSampleR *= (1.0-RmaxRecent);
		inputSampleL += (LpassThrough * LmaxRecent);
		inputSampleR += (RpassThrough * RmaxRecent);
		//there's our raw signal, without antialiasing. Brings up low level stuff and softens more when hot
		
		if (inputSampleL > cliplevel) inputSampleL = cliplevel;
		if (inputSampleL < -cliplevel) inputSampleL = -cliplevel;
		if (inputSampleR > cliplevel) inputSampleR = cliplevel;
		if (inputSampleR < -cliplevel) inputSampleR = -cliplevel;
		//final iron bar
		

		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void AQuickVoiceClip::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double softness = 0.484416;
	double hardness = 1.0 - softness;
	double iirAmount = ((pow(A,3)*2070)+30)/8000.0;
	iirAmount /= overallscale;
	double altAmount = (1.0 - iirAmount);
	double cancelnew = 0.0682276;
	double cancelold = 1.0 - cancelnew;
	double lpSpeed = 0.0009;
	double cliplevel = 0.98;
	double refclip = 0.5; //preset to cut out gain quite a lot. 91%? no touchy unless clip
	
	double LmaxRecent;
	bool LclipOnset;
	double LpassThrough;
	double LoutputSample;
	double LdrySample;
	
	double RmaxRecent;
	bool RclipOnset;
	double RpassThrough;
	double RoutputSample;
	double RdrySample;
	

	double inputSampleL;
	double inputSampleR;

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		LpassThrough = LataDrySample = inputSampleL;
		RpassThrough = RataDrySample = inputSampleR;
		
		LataHalfDrySample = LataHalfwaySample = (inputSampleL + LataLast1Sample + (LataLast2Sample*ataK1) + (LataLast3Sample*ataK2) + (LataLast4Sample*ataK6) + (LataLast5Sample*ataK7) + (LataLast6Sample*ataK8)) / 2.0;
		LataLast6Sample = LataLast5Sample; LataLast5Sample = LataLast4Sample; LataLast4Sample = LataLast3Sample; LataLast3Sample = LataLast2Sample; LataLast2Sample = LataLast1Sample; LataLast1Sample = inputSampleL;
		//setting up oversampled special antialiasing
		RataHalfDrySample = RataHalfwaySample = (inputSampleR + RataLast1Sample + (RataLast2Sample*ataK1) + (RataLast3Sample*ataK2) + (RataLast4Sample*ataK6) + (RataLast5Sample*ataK7) + (RataLast6Sample*ataK8)) / 2.0;
		RataLast6Sample = RataLast5Sample; RataLast5Sample = RataLast4Sample; RataLast4Sample = RataLast3Sample; RataLast3Sample = RataLast2Sample; RataLast2Sample = RataLast1Sample; RataLast1Sample = inputSampleR;
		//setting up oversampled special antialiasing
		LclipOnset = false;
		RclipOnset = false;
		
		
		LmaxRecent = fabs( LataLast6Sample );
		if (fabs( LataLast5Sample ) > LmaxRecent ) LmaxRecent = fabs( LataLast5Sample );
		if (fabs( LataLast4Sample ) > LmaxRecent ) LmaxRecent = fabs( LataLast4Sample );
		if (fabs( LataLast3Sample ) > LmaxRecent ) LmaxRecent = fabs( LataLast3Sample );
		if (fabs( LataLast2Sample ) > LmaxRecent ) LmaxRecent = fabs( LataLast2Sample );
		if (fabs( LataLast1Sample ) > LmaxRecent ) LmaxRecent = fabs( LataLast1Sample );
		if (fabs( inputSampleL ) > LmaxRecent ) LmaxRecent = fabs( inputSampleL );
		//this gives us something that won't cut out in zero crossings, to interpolate with
		
		RmaxRecent = fabs( RataLast6Sample );
		if (fabs( RataLast5Sample ) > RmaxRecent ) RmaxRecent = fabs( RataLast5Sample );
		if (fabs( RataLast4Sample ) > RmaxRecent ) RmaxRecent = fabs( RataLast4Sample );
		if (fabs( RataLast3Sample ) > RmaxRecent ) RmaxRecent = fabs( RataLast3Sample );
		if (fabs( RataLast2Sample ) > RmaxRecent ) RmaxRecent = fabs( RataLast2Sample );
		if (fabs( RataLast1Sample ) > RmaxRecent ) RmaxRecent = fabs( RataLast1Sample );
		if (fabs( inputSampleR ) > RmaxRecent ) RmaxRecent = fabs( inputSampleR );
		//this gives us something that won't cut out in zero crossings, to interpolate with
		
		LmaxRecent *= 2.0;
		RmaxRecent *= 2.0;
		//by refclip this is 1.0 and fully into the antialiasing
		if (LmaxRecent > 1.0) LmaxRecent = 1.0;
		if (RmaxRecent > 1.0) RmaxRecent = 1.0;
		//and it tops out at 1. Higher means more antialiasing, lower blends into passThrough without antialiasing
		
		LataHalfwaySample -= Loverall;
		RataHalfwaySample -= Roverall;
		//subtract dist-cancel from input after getting raw input, before doing anything
		
		LdrySample = LataHalfwaySample;
		RdrySample = RataHalfwaySample;
		
		
		//begin L channel for the clipper
		if (LlastSample >= refclip)
		{
			LlpDepth += 0.1;
			if (LataHalfwaySample < refclip)
			{
				LlastSample = ((refclip*hardness) + (LataHalfwaySample * softness));
			}
			else LlastSample = refclip;
		}
		
		if (LlastSample <= -refclip)
		{
			LlpDepth += 0.1;
			if (LataHalfwaySample > -refclip)
			{
				LlastSample = ((-refclip*hardness) + (LataHalfwaySample * softness));
			}
			else LlastSample = -refclip;
		}
		
		if (LataHalfwaySample > refclip)
		{
			LlpDepth += 0.1;
			if (LlastSample < refclip)
			{
				LataHalfwaySample = ((refclip*hardness) + (LlastSample * softness));
			}
			else LataHalfwaySample = refclip;
		}
		
		if (LataHalfwaySample < -refclip)
		{
			LlpDepth += 0.1;
			if (LlastSample > -refclip)
			{
				LataHalfwaySample = ((-refclip*hardness) + (LlastSample * softness));
			}
			else LataHalfwaySample = -refclip;
		}
		///end L channel for the clipper
		
		//begin R channel for the clipper
		if (RlastSample >= refclip)
		{
			RlpDepth += 0.1;
			if (RataHalfwaySample < refclip)
			{
				RlastSample = ((refclip*hardness) + (RataHalfwaySample * softness));
			}
			else RlastSample = refclip;
		}
		
		if (RlastSample <= -refclip)
		{
			RlpDepth += 0.1;
			if (RataHalfwaySample > -refclip)
			{
				RlastSample = ((-refclip*hardness) + (RataHalfwaySample * softness));
			}
			else RlastSample = -refclip;
		}
		
		if (RataHalfwaySample > refclip)
		{
			RlpDepth += 0.1;
			if (RlastSample < refclip)
			{
				RataHalfwaySample = ((refclip*hardness) + (RlastSample * softness));
			}
			else RataHalfwaySample = refclip;
		}
		
		if (RataHalfwaySample < -refclip)
		{
			RlpDepth += 0.1;
			if (RlastSample > -refclip)
			{
				RataHalfwaySample = ((-refclip*hardness) + (RlastSample * softness));
			}
			else RataHalfwaySample = -refclip;
		}
		///end R channel for the clipper
		
        LoutputSample = LlastSample;
        RoutputSample = RlastSample;
		
		LlastSample = LataHalfwaySample;
		RlastSample = RataHalfwaySample;
		
		LataHalfwaySample = LoutputSample;
		RataHalfwaySample = RoutputSample;
		//swap around in a circle for one final ADClip,
		//this time not tracking overshoot anymore
		//end interpolated sample		
		//begin raw sample- inputSample and ataDrySample handled separately here
		
		inputSampleL -= Loverall;
		inputSampleR -= Roverall;
		//subtract dist-cancel from input after getting raw input, before doing anything
		
		LdrySample = inputSampleL;
		RdrySample = inputSampleR;
		
		//begin second L clip
		if (LlastSample >= refclip)
		{
			LlpDepth += 0.1;
			if (inputSampleL < refclip)
			{
				LlastSample = ((refclip*hardness) + (inputSampleL * softness));
			}
			else LlastSample = refclip;
		}
		
		if (LlastSample <= -refclip)
		{
			LlpDepth += 0.1;
			if (inputSampleL > -refclip)
			{
				LlastSample = ((-refclip*hardness) + (inputSampleL * softness));
			}
			else LlastSample = -refclip;
		}
		
		if (inputSampleL > refclip)
		{
			LlpDepth += 0.1;
			if (LlastSample < refclip)
			{
				inputSampleL = ((refclip*hardness) + (LlastSample * softness));
			}
			else inputSampleL = refclip;
		}
		
		if (inputSampleL < -refclip)
		{
			LlpDepth += 0.1;
			if (LlastSample > -refclip)
			{
				inputSampleL = ((-refclip*hardness) + (LlastSample * softness));
			}
			else inputSampleL = -refclip;
		}		
		//end second L clip
		
		//begin second R clip
		if (RlastSample >= refclip)
		{
			RlpDepth += 0.1;
			if (inputSampleR < refclip)
			{
				RlastSample = ((refclip*hardness) + (inputSampleR * softness));
			}
			else RlastSample = refclip;
		}
		
		if (RlastSample <= -refclip)
		{
			RlpDepth += 0.1;
			if (inputSampleR > -refclip)
			{
				RlastSample = ((-refclip*hardness) + (inputSampleR * softness));
			}
			else RlastSample = -refclip;
		}
		
		if (inputSampleR > refclip)
		{
			RlpDepth += 0.1;
			if (RlastSample < refclip)
			{
				inputSampleR = ((refclip*hardness) + (RlastSample * softness));
			}
			else inputSampleR = refclip;
		}
		
		if (inputSampleR < -refclip)
		{
			RlpDepth += 0.1;
			if (RlastSample > -refclip)
			{
				inputSampleR = ((-refclip*hardness) + (RlastSample * softness));
			}
			else inputSampleR = -refclip;
		}		
		//end second R clip
		
		LoutputSample = LlastSample;
		RoutputSample = RlastSample;
		LlastSample = inputSampleL;
		RlastSample = inputSampleR;
		inputSampleL = LoutputSample;
		inputSampleR = RoutputSample;
		
		LataHalfDrySample = (LataDrySample*ataK3)+(LataHalfDrySample*ataK4);
		LataHalfDiffSample = (LataHalfwaySample - LataHalfDrySample)/2.0;
		LataLastDiffSample = LataDiffSample*ataK5;
		LataDiffSample = (inputSampleL - LataDrySample)/2.0;
		LataDiffSample += LataHalfDiffSample;
		LataDiffSample -= LataLastDiffSample;
		inputSampleL = LataDrySample;
		inputSampleL += LataDiffSample;
		
		RataHalfDrySample = (RataDrySample*ataK3)+(RataHalfDrySample*ataK4);
		RataHalfDiffSample = (RataHalfwaySample - RataHalfDrySample)/2.0;
		RataLastDiffSample = RataDiffSample*ataK5;
		RataDiffSample = (inputSampleR - RataDrySample)/2.0;
		RataDiffSample += RataHalfDiffSample;
		RataDiffSample -= RataLastDiffSample;
		inputSampleR = RataDrySample;
		inputSampleR += RataDiffSample;
		
		Loverall = (Loverall * cancelold) + (LataDiffSample * cancelnew);
		Roverall = (Roverall * cancelold) + (RataDiffSample * cancelnew);
		//apply all the diffs to a lowpassed IIR
		
		
		if (flip)
		{
			LiirSampleA = (LiirSampleA * altAmount) + (inputSampleL * iirAmount);
			inputSampleL -= LiirSampleA;
			LiirSampleC = (LiirSampleC * altAmount) + (LpassThrough * iirAmount);
			LpassThrough -= LiirSampleC;
			
			RiirSampleA = (RiirSampleA * altAmount) + (inputSampleR * iirAmount);
			inputSampleR -= RiirSampleA;
			RiirSampleC = (RiirSampleC * altAmount) + (RpassThrough * iirAmount);
			RpassThrough -= RiirSampleC;
		}
		else
		{
			LiirSampleB = (LiirSampleB * altAmount) + (inputSampleL * iirAmount);
			inputSampleL -= LiirSampleB;
			LiirSampleD = (LiirSampleD * altAmount) + (LpassThrough * iirAmount);
			LpassThrough -= LiirSampleD;
			
			RiirSampleB = (RiirSampleB * altAmount) + (inputSampleR * iirAmount);
			inputSampleR -= RiirSampleB;
			RiirSampleD = (RiirSampleD * altAmount) + (RpassThrough * iirAmount);
			RpassThrough -= RiirSampleD;
		}
		flip = !flip;
		//highpass section 
		
		LlastOut3Sample = LlastOut2Sample;
		LlastOut2Sample = LlastOutSample;
		LlastOutSample = inputSampleL;
		
		RlastOut3Sample = RlastOut2Sample;
		RlastOut2Sample = RlastOutSample;
		RlastOutSample = inputSampleR;
		
		
		LlpDepth -= lpSpeed;
		RlpDepth -= lpSpeed;
		
		if (LlpDepth > 0.0)
		{
			if (LlpDepth > 1.0) LlpDepth = 1.0;
			inputSampleL *= (1.0-LlpDepth);
			inputSampleL += (((LlastOutSample + LlastOut2Sample + LlastOut3Sample) / 3.6)*LlpDepth);
		}
		
		if (RlpDepth > 0.0)
		{
			if (RlpDepth > 1.0) RlpDepth = 1.0;
			inputSampleR *= (1.0-RlpDepth);
			inputSampleR += (((RlastOutSample + RlastOut2Sample + RlastOut3Sample) / 3.6)*RlpDepth);
		}
		
		if (LlpDepth < 0.0) LlpDepth = 0.0;
		if (RlpDepth < 0.0) RlpDepth = 0.0;
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		inputSampleL *= (1.0-LmaxRecent);
		inputSampleR *= (1.0-RmaxRecent);
		inputSampleL += (LpassThrough * LmaxRecent);
		inputSampleR += (RpassThrough * RmaxRecent);
		//there's our raw signal, without antialiasing. Brings up low level stuff and softens more when hot
		
		if (inputSampleL > cliplevel) inputSampleL = cliplevel;
		if (inputSampleL < -cliplevel) inputSampleL = -cliplevel;
		if (inputSampleR > cliplevel) inputSampleR = cliplevel;
		if (inputSampleR < -cliplevel) inputSampleR = -cliplevel;
		//final iron bar
				
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}} // end namespace
