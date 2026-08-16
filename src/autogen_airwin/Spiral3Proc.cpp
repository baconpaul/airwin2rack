/* ========================================
 *  Spiral3 - Spiral3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Spiral3_H
#include "Spiral3.h"
#endif
#include <cstdlib>
namespace airwinconsolidated::Spiral3 {

void Spiral3::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double input = A;
	double bezierRez = fmax(pow((1.0-B)*0.25,3.0)/overallscale,0.00001); 
	int stepped = 999999; if (bezierRez > 0.000001) stepped = (int)(1.0/bezierRez); bezierRez = 0.99999999/stepped;
	double bezierTrim = 1.0-(bezierRez*((double)stepped/(stepped+1.0)));
	//manages the overall Bezier control smoothing system plugin-wide and feed all controls
	//into bezier[] as just 0-1 values, unprocessed. do it IN the control smoothing engine
	double output = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//begin Bezier control smoothing engine
		bezier[bezier_cycle] += bezierRez;
		if (bezier[bezier_cycle] > bezierTrim) {bezier[bezier_cycle] = 0.0;
			bezier[bezierInput_C] = bezier[bezierInput_B]; bezier[bezierInput_B] = bezier[bezierInput_A];
			bezier[bezierOutput_C] = bezier[bezierOutput_B]; bezier[bezierOutput_B] = bezier[bezierOutput_A];
			//one of these bucket brigade lines for every smoothed control
			//begin expensive control calculations
			bezier[bezierInput_A] = input*10.0;
			bezier[bezierOutput_A] = output;
			//end expensive control calculations
		} double lerp = bezier[bezier_cycle];
		bezier[bezierInput_Out] = (bezier[bezierInput_B]+(bezier[bezierInput_C]*(1.0-lerp)*(1.0-lerp))+(bezier[bezierInput_B]*2.0*(1.0-lerp)*lerp)+(bezier[bezierInput_A]*lerp*lerp))*0.5;
		bezier[bezierOutput_Out] = (bezier[bezierOutput_B]+(bezier[bezierOutput_C]*(1.0-lerp)*(1.0-lerp))+(bezier[bezierOutput_B]*2.0*(1.0-lerp)*lerp)+(bezier[bezierOutput_A]*lerp*lerp))*0.5;
		//end Bezier control smoothing engine
		
		inputSampleL *= bezier[bezierInput_Out];
		double bip_delta = inputSampleL; //delta can be just local and re-used
		if (inputSampleL != 0.0) {
			inputSampleL = fmax(fmin(inputSampleL,1.0),-1.0);
			inputSampleL *= 1.0795;
			long double spiral = inputSampleL * fabs(inputSampleL);
			long double X = spiral; X *= X;
			long double temp = spiral * X;
			spiral -= temp*0.16666666666666666666666666666666666; temp *= X;//-8 dB
			spiral += temp*0.00833333333333333333333333333333333; temp *= X;//-20 dB
			spiral -= temp*0.00019841269841269841269841269841269; temp *= X;//-37 dB
			spiral += temp*0.00000275573192239858906525573192239; temp *= X;//-55 dB
			spiral -= temp*0.00000002505210838544171877521083854; temp *= X;//-76 dB
			spiral += temp*0.00000000016059043836821614599392377; temp *= X;//-108 dB
			spiral -= temp*0.00000000000076471637318198164759011; temp *= X;//-121 dB
			spiral += temp*0.00000000000000281145725434552076319; temp *= X;//-146 dB
			spiral -= temp*0.00000000000000000822063524662432971; temp *= X;//-171 dB
			spiral += temp*0.00000000000000000001957294106339126;           //-197 dB
			inputSampleL = spiral / fabs(inputSampleL); //purest form is full Spiral
		}
		bip[bip_dvLA] = bip_delta - inputSampleL; // these are derivatives: raw clip is position
		bip[bip_dvLB] = bip[bip_pvLA]-bip[bip_dvLA]; bip[bip_pvLA] = bip[bip_dvLA];//velocity
		bip[bip_dvLC] = bip[bip_pvLB]-bip[bip_dvLB]; bip[bip_pvLB] = bip[bip_dvLB];//acceleration
		bip[bip_dvLD] = bip[bip_pvLC]-bip[bip_dvLC]; bip[bip_pvLC] = bip[bip_dvLC];//jerk
		double bip_dvE = bip[bip_pvLD]-bip[bip_dvLD]; bip[bip_pvLD] = bip[bip_dvLD];//snap
		inputSampleL *= (1.0+(fabs(bip[bip_dvLC])*0.0618)+(fabs(bip[bip_dvLD])*-0.05982)+(fabs(bip_dvE)*0.0206));
		inputSampleL *= bezier[bezierOutput_Out];
		
		inputSampleR *= bezier[bezierInput_Out];
		bip_delta = inputSampleR; //delta can be just local and re-used
		if (inputSampleR != 0.0) {
			inputSampleR = fmax(fmin(inputSampleR,1.0),-1.0);
			inputSampleR *= 1.0795;
			long double spiral = inputSampleR * fabs(inputSampleR);
			long double X = spiral; X *= X;
			long double temp = spiral * X;
			spiral -= temp*0.16666666666666666666666666666666666; temp *= X;//-8 dB
			spiral += temp*0.00833333333333333333333333333333333; temp *= X;//-20 dB
			spiral -= temp*0.00019841269841269841269841269841269; temp *= X;//-37 dB
			spiral += temp*0.00000275573192239858906525573192239; temp *= X;//-55 dB
			spiral -= temp*0.00000002505210838544171877521083854; temp *= X;//-76 dB
			spiral += temp*0.00000000016059043836821614599392377; temp *= X;//-108 dB
			spiral -= temp*0.00000000000076471637318198164759011; temp *= X;//-121 dB
			spiral += temp*0.00000000000000281145725434552076319; temp *= X;//-146 dB
			spiral -= temp*0.00000000000000000822063524662432971; temp *= X;//-171 dB
			spiral += temp*0.00000000000000000001957294106339126;           //-197 dB
			inputSampleR = spiral / fabs(inputSampleR); //purest form is full Spiral
		}
		bip[bip_dvRA] = bip_delta - inputSampleR; // these are derivatives: raw clip is position
		bip[bip_dvRB] = bip[bip_pvRA]-bip[bip_dvRA]; bip[bip_pvRA] = bip[bip_dvRA];//velocity
		bip[bip_dvRC] = bip[bip_pvRB]-bip[bip_dvRB]; bip[bip_pvRB] = bip[bip_dvRB];//acceleration
		bip[bip_dvRD] = bip[bip_pvRC]-bip[bip_dvRC]; bip[bip_pvRC] = bip[bip_dvRC];//jerk
		bip_dvE = bip[bip_pvRD]-bip[bip_dvRD]; bip[bip_pvRD] = bip[bip_dvRD];//snap
		inputSampleR *= (1.0+(fabs(bip[bip_dvRC])*0.0618)+(fabs(bip[bip_dvRD])*-0.05982)+(fabs(bip_dvE)*0.0206));
		inputSampleR *= bezier[bezierOutput_Out];
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void Spiral3::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double input = A;
	double bezierRez = fmax(pow((1.0-B)*0.25,3.0)/overallscale,0.00001); 
	int stepped = 999999; if (bezierRez > 0.000001) stepped = (int)(1.0/bezierRez); bezierRez = 0.99999999/stepped;
	double bezierTrim = 1.0-(bezierRez*((double)stepped/(stepped+1.0)));
	//manages the overall Bezier control smoothing system plugin-wide and feed all controls
	//into bezier[] as just 0-1 values, unprocessed. do it IN the control smoothing engine
	double output = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//begin Bezier control smoothing engine
		bezier[bezier_cycle] += bezierRez;
		if (bezier[bezier_cycle] > bezierTrim) {bezier[bezier_cycle] = 0.0;
			bezier[bezierInput_C] = bezier[bezierInput_B]; bezier[bezierInput_B] = bezier[bezierInput_A];
			bezier[bezierOutput_C] = bezier[bezierOutput_B]; bezier[bezierOutput_B] = bezier[bezierOutput_A];
			//one of these bucket brigade lines for every smoothed control
			//begin expensive control calculations
			bezier[bezierInput_A] = input*10.0;
			bezier[bezierOutput_A] = output;
			//end expensive control calculations
		} double lerp = bezier[bezier_cycle];
		bezier[bezierInput_Out] = (bezier[bezierInput_B]+(bezier[bezierInput_C]*(1.0-lerp)*(1.0-lerp))+(bezier[bezierInput_B]*2.0*(1.0-lerp)*lerp)+(bezier[bezierInput_A]*lerp*lerp))*0.5;
		bezier[bezierOutput_Out] = (bezier[bezierOutput_B]+(bezier[bezierOutput_C]*(1.0-lerp)*(1.0-lerp))+(bezier[bezierOutput_B]*2.0*(1.0-lerp)*lerp)+(bezier[bezierOutput_A]*lerp*lerp))*0.5;
		//end Bezier control smoothing engine
		
		inputSampleL *= bezier[bezierInput_Out];
		double bip_delta = inputSampleL; //delta can be just local and re-used
		if (inputSampleL != 0.0) {
			inputSampleL = fmax(fmin(inputSampleL,1.0),-1.0);
			inputSampleL *= 1.0795;
			long double spiral = inputSampleL * fabs(inputSampleL);
			long double X = spiral; X *= X;
			long double temp = spiral * X;
			spiral -= temp*0.16666666666666666666666666666666666; temp *= X;//-8 dB
			spiral += temp*0.00833333333333333333333333333333333; temp *= X;//-20 dB
			spiral -= temp*0.00019841269841269841269841269841269; temp *= X;//-37 dB
			spiral += temp*0.00000275573192239858906525573192239; temp *= X;//-55 dB
			spiral -= temp*0.00000002505210838544171877521083854; temp *= X;//-76 dB
			spiral += temp*0.00000000016059043836821614599392377; temp *= X;//-108 dB
			spiral -= temp*0.00000000000076471637318198164759011; temp *= X;//-121 dB
			spiral += temp*0.00000000000000281145725434552076319; temp *= X;//-146 dB
			spiral -= temp*0.00000000000000000822063524662432971; temp *= X;//-171 dB
			spiral += temp*0.00000000000000000001957294106339126;           //-197 dB
			inputSampleL = spiral / fabs(inputSampleL); //purest form is full Spiral
		}
		bip[bip_dvLA] = bip_delta - inputSampleL; // these are derivatives: raw clip is position
		bip[bip_dvLB] = bip[bip_pvLA]-bip[bip_dvLA]; bip[bip_pvLA] = bip[bip_dvLA];//velocity
		bip[bip_dvLC] = bip[bip_pvLB]-bip[bip_dvLB]; bip[bip_pvLB] = bip[bip_dvLB];//acceleration
		bip[bip_dvLD] = bip[bip_pvLC]-bip[bip_dvLC]; bip[bip_pvLC] = bip[bip_dvLC];//jerk
		double bip_dvE = bip[bip_pvLD]-bip[bip_dvLD]; bip[bip_pvLD] = bip[bip_dvLD];//snap
		inputSampleL *= (1.0+(fabs(bip[bip_dvLC])*0.0618)+(fabs(bip[bip_dvLD])*-0.05982)+(fabs(bip_dvE)*0.0206));
		inputSampleL *= bezier[bezierOutput_Out];
		
		inputSampleR *= bezier[bezierInput_Out];
		bip_delta = inputSampleR; //delta can be just local and re-used
		if (inputSampleR != 0.0) {
			inputSampleR = fmax(fmin(inputSampleR,1.0),-1.0);
			inputSampleR *= 1.0795;
			long double spiral = inputSampleR * fabs(inputSampleR);
			long double X = spiral; X *= X;
			long double temp = spiral * X;
			spiral -= temp*0.16666666666666666666666666666666666; temp *= X;//-8 dB
			spiral += temp*0.00833333333333333333333333333333333; temp *= X;//-20 dB
			spiral -= temp*0.00019841269841269841269841269841269; temp *= X;//-37 dB
			spiral += temp*0.00000275573192239858906525573192239; temp *= X;//-55 dB
			spiral -= temp*0.00000002505210838544171877521083854; temp *= X;//-76 dB
			spiral += temp*0.00000000016059043836821614599392377; temp *= X;//-108 dB
			spiral -= temp*0.00000000000076471637318198164759011; temp *= X;//-121 dB
			spiral += temp*0.00000000000000281145725434552076319; temp *= X;//-146 dB
			spiral -= temp*0.00000000000000000822063524662432971; temp *= X;//-171 dB
			spiral += temp*0.00000000000000000001957294106339126;           //-197 dB
			inputSampleR = spiral / fabs(inputSampleR); //purest form is full Spiral
		}
		bip[bip_dvRA] = bip_delta - inputSampleR; // these are derivatives: raw clip is position
		bip[bip_dvRB] = bip[bip_pvRA]-bip[bip_dvRA]; bip[bip_pvRA] = bip[bip_dvRA];//velocity
		bip[bip_dvRC] = bip[bip_pvRB]-bip[bip_dvRB]; bip[bip_pvRB] = bip[bip_dvRB];//acceleration
		bip[bip_dvRD] = bip[bip_pvRC]-bip[bip_dvRC]; bip[bip_pvRC] = bip[bip_dvRC];//jerk
		bip_dvE = bip[bip_pvRD]-bip[bip_dvRD]; bip[bip_pvRD] = bip[bip_dvRD];//snap
		inputSampleR *= (1.0+(fabs(bip[bip_dvRC])*0.0618)+(fabs(bip[bip_dvRD])*-0.05982)+(fabs(bip_dvE)*0.0206));
		inputSampleR *= bezier[bezierOutput_Out];
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l  * pow(2,expon+62));
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
