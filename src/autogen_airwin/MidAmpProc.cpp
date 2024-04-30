/* ========================================
 *  MidAmp - MidAmp.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_H
#include "MidAmp.h"
#endif
namespace airwinconsolidated::MidAmp {

void MidAmp::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double inputlevel = A*3.0;
	double samplerate = getSampleRate();
	double EQ = (B/ samplerate)*22050.0;
	double basstrim = B;
	double outputlevel = C;
	double wet = D;
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= samplerate;
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check		
	
	double bleed = outputlevel/6.0;
	double BEQ = (bleed / samplerate)*44100.0;
	int side = (int)(0.0006092985*samplerate);
	if (side > 126) side = 126;
	int down = side + 1;
	inputlevel = (inputlevel + (inputlevel*basstrim))/2.0;
	
	double cutoff = (15000.0+(B*10000.0)) / getSampleRate();
	if (cutoff > 0.49) cutoff = 0.49; //don't crash if run at 44.1k
	if (cutoff < 0.001) cutoff = 0.001; //or if cutoff's too low
	
	fixF[fix_freq] = fixE[fix_freq] = fixD[fix_freq] = fixC[fix_freq] = fixB[fix_freq] = fixA[fix_freq] = cutoff;
	
    fixA[fix_reso] = 4.46570214;
	fixB[fix_reso] = 1.51387132;
	fixC[fix_reso] = 0.93979296;
	fixD[fix_reso] = 0.70710678;
	fixE[fix_reso] = 0.52972649;
	fixF[fix_reso] = 0.50316379;
	
	double K = tan(M_PI * fixA[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fixA[fix_reso] + K * K);
	fixA[fix_a0] = K * K * norm;
	fixA[fix_a1] = 2.0 * fixA[fix_a0];
	fixA[fix_a2] = fixA[fix_a0];
	fixA[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixA[fix_b2] = (1.0 - K / fixA[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixB[fix_freq]);
	norm = 1.0 / (1.0 + K / fixB[fix_reso] + K * K);
	fixB[fix_a0] = K * K * norm;
	fixB[fix_a1] = 2.0 * fixB[fix_a0];
	fixB[fix_a2] = fixB[fix_a0];
	fixB[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixB[fix_b2] = (1.0 - K / fixB[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixC[fix_freq]);
	norm = 1.0 / (1.0 + K / fixC[fix_reso] + K * K);
	fixC[fix_a0] = K * K * norm;
	fixC[fix_a1] = 2.0 * fixC[fix_a0];
	fixC[fix_a2] = fixC[fix_a0];
	fixC[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixC[fix_b2] = (1.0 - K / fixC[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixD[fix_freq]);
	norm = 1.0 / (1.0 + K / fixD[fix_reso] + K * K);
	fixD[fix_a0] = K * K * norm;
	fixD[fix_a1] = 2.0 * fixD[fix_a0];
	fixD[fix_a2] = fixD[fix_a0];
	fixD[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixD[fix_b2] = (1.0 - K / fixD[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixE[fix_freq]);
	norm = 1.0 / (1.0 + K / fixE[fix_reso] + K * K);
	fixE[fix_a0] = K * K * norm;
	fixE[fix_a1] = 2.0 * fixE[fix_a0];
	fixE[fix_a2] = fixE[fix_a0];
	fixE[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixE[fix_b2] = (1.0 - K / fixE[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixF[fix_freq]);
	norm = 1.0 / (1.0 + K / fixF[fix_reso] + K * K);
	fixF[fix_a0] = K * K * norm;
	fixF[fix_a1] = 2.0 * fixF[fix_a0];
	fixF[fix_a2] = fixF[fix_a0];
	fixF[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixF[fix_b2] = (1.0 - K / fixF[fix_reso] + K * K) * norm;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double outSample = (inputSampleL * fixA[fix_a0]) + fixA[fix_sL1];
		fixA[fix_sL1] = (inputSampleL * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sL2];
		fixA[fix_sL2] = (inputSampleL * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixA[fix_a0]) + fixA[fix_sR1];
		fixA[fix_sR1] = (inputSampleR * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sR2];
		fixA[fix_sR2] = (inputSampleR * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		double skewL = (inputSampleL - lastSampleL);
		lastSampleL = inputSampleL;
		//skew will be direction/angle
		double bridgerectifier = fabs(skewL);
		if (bridgerectifier > 3.1415926) bridgerectifier = 3.1415926;
		//for skew we want it to go to zero effect again, so we use full range of the sine
		bridgerectifier = sin(bridgerectifier);
		if (skewL > 0.0) skewL = bridgerectifier;
		else skewL = -bridgerectifier;
		//skew is now sined and clamped and then re-amplified again
		skewL *= inputSampleL;
		
		double skewR = (inputSampleR - lastSampleR);
		lastSampleR = inputSampleR;
		//skew will be direction/angle
		bridgerectifier = fabs(skewR);
		if (bridgerectifier > 3.1415926) bridgerectifier = 3.1415926;
		//for skew we want it to go to zero effect again, so we use full range of the sine
		bridgerectifier = sin(bridgerectifier);
		if (skewR > 0.0) skewR = bridgerectifier;
		else skewR = -bridgerectifier;
		//skew is now sined and clamped and then re-amplified again
		skewR *= inputSampleR;
		
		outSample = (inputSampleL * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSampleL * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSampleL * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixB[fix_a0]) + fixB[fix_sR1];
		fixB[fix_sR1] = (inputSampleR * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sR2];
		fixB[fix_sR2] = (inputSampleR * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		double offsetL = (1.0 - EQ) + (fabs(inputSampleL)*EQ);
		if (offsetL < 0.0) offsetL = 0.0;
		if (offsetL > 1.0) offsetL = 1.0;
		iirSampleAL = (iirSampleAL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleAL*0.8);
		//highpass
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewL;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//overdrive
		iirSampleCL = (iirSampleCL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleCL*0.6);
		//highpass
		skewL /= 1.57079633;
		//finished first gain stage
		
		inputSampleR *= inputlevel;
		double offsetR = (1.0 - EQ) + (fabs(inputSampleR)*EQ);
		if (offsetR < 0.0) offsetR = 0.0;
		if (offsetR > 1.0) offsetR = 1.0;
		iirSampleAR = (iirSampleAR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleAR*0.8);
		//highpass
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewR;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//overdrive
		iirSampleCR = (iirSampleCR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleCR*0.6);
		//highpass
		skewR /= 1.57079633;
		//finished first gain stage
		
		outSample = (inputSampleL * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSampleL * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSampleL * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixC[fix_a0]) + fixC[fix_sR1];
		fixC[fix_sR1] = (inputSampleR * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sR2];
		fixC[fix_sR2] = (inputSampleR * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		offsetL = (1.0 + offsetL) / 2.0;
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//overdrive
		iirSampleGL = (iirSampleGL * (1.0 - EQ)) + (inputSampleL * EQ);
		inputSampleL = inputSampleL - (iirSampleGL*0.4);
		//highpass. no further basscut, no more highpasses
		iirSampleDL = (iirSampleDL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = iirSampleDL;
		skewL /= 1.57079633;
		//lowpass. Use offset from before gain stage
		
		inputSampleR *= inputlevel;
		offsetR = (1.0 + offsetR) / 2.0;
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//overdrive
		iirSampleGR = (iirSampleGR * (1.0 - EQ)) + (inputSampleR * EQ);
		inputSampleR = inputSampleR - (iirSampleGR*0.4);
		//highpass. no further basscut, no more highpasses
		iirSampleDR = (iirSampleDR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = iirSampleDR;
		skewR /= 1.57079633;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSampleL * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSampleL * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSampleL * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixD[fix_a0]) + fixD[fix_sR1];
		fixD[fix_sR1] = (inputSampleR * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sR2];
		fixD[fix_sR2] = (inputSampleR * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//output stage has less gain, no highpass, straight lowpass
		iirSampleEL = (iirSampleEL * (1.0 - EQ)) + (inputSampleL * EQ);
		inputSampleL = iirSampleEL;
		//lowpass. Use offset from before gain stage
		
		inputSampleR *= inputlevel;
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//output stage has less gain, no highpass, straight lowpass
		iirSampleER = (iirSampleER * (1.0 - EQ)) + (inputSampleR * EQ);
		inputSampleR = iirSampleER;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSampleL * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSampleL * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSampleL * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixE[fix_a0]) + fixE[fix_sR1];
		fixE[fix_sR1] = (inputSampleR * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sR2];
		fixE[fix_sR2] = (inputSampleR * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		iirSampleFL = (iirSampleFL * (1.0 - (offsetL * BEQ))) + (inputSampleL * (offsetL * BEQ));
		//extra lowpass for 4*12" speakers
		iirSampleFR = (iirSampleFR * (1.0 - (offsetR * BEQ))) + (inputSampleR * (offsetR * BEQ));
		//extra lowpass for 4*12" speakers

		if (count < 0 || count > 128) {count = 128;}
		double resultBL = 0.0;
		double resultBR = 0.0;
		if (flip)
		{
			OddL[count+128] = OddL[count] = iirSampleFL;
			resultBL = (OddL[count+down] + OddL[count+side]);
			OddR[count+128] = OddR[count] = iirSampleFR;
			resultBR = (OddR[count+down] + OddR[count+side]);
		}
		else
		{
			EvenL[count+128] = EvenL[count] = iirSampleFL;
			resultBL = (EvenL[count+down] + EvenL[count+side]);
			EvenR[count+128] = EvenR[count] = iirSampleFR;
			resultBR = (EvenR[count+down] + EvenR[count+side]);
		}
		count--;
		
		iirSampleBL = (iirSampleBL * (1.0 - (offsetL * BEQ))) + (resultBL * (offsetL * BEQ));
		inputSampleL += (iirSampleBL*bleed);
		//extra lowpass for 4*12" speakers
		
		iirSampleBR = (iirSampleBR * (1.0 - (offsetR * BEQ))) + (resultBR * (offsetR * BEQ));
		inputSampleR += (iirSampleBR*bleed);
		//extra lowpass for 4*12" speakers
		
		iirSampleHL = (iirSampleHL * (1.0 - (offsetL * BEQ))) + (inputSampleL * (offsetL * BEQ));
		inputSampleL += iirSampleHL;
		
		iirSampleHR = (iirSampleHR * (1.0 - (offsetR * BEQ))) + (inputSampleR * (offsetR * BEQ));
		inputSampleR += iirSampleHR;
		
		inputSampleL = sin(inputSampleL*outputlevel);		
		double randy = ((double(fpdL)/UINT32_MAX)*0.047);
		inputSampleL = ((inputSampleL*(1.0-randy))+(storeSampleL*randy))*outputlevel;
		storeSampleL = inputSampleL;
		
		inputSampleR = sin(inputSampleR*outputlevel);		
		randy = ((double(fpdR)/UINT32_MAX)*0.047);
		inputSampleR = ((inputSampleR*(1.0-randy))+(storeSampleR*randy))*outputlevel;
		storeSampleR = inputSampleR;
		
		outSample = (inputSampleL * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSampleL * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSampleL * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixF[fix_a0]) + fixF[fix_sR1];
		fixF[fix_sR1] = (inputSampleR * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sR2];
		fixF[fix_sR2] = (inputSampleR * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		flip = !flip;
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		//amp
		
		cycle++;
		if (cycle == cycleEnd) {
			double temp = (inputSampleL + smoothCabAL)/3.0;
			smoothCabAL = inputSampleL;
			inputSampleL = temp;
			
			bL[87] = bL[86]; bL[86] = bL[85]; bL[85] = bL[84]; bL[84] = bL[83]; bL[83] = bL[82]; bL[82] = bL[81]; bL[81] = bL[80]; bL[80] = bL[79]; 
			bL[79] = bL[78]; bL[78] = bL[77]; bL[77] = bL[76]; bL[76] = bL[75]; bL[75] = bL[74]; bL[74] = bL[73]; bL[73] = bL[72]; bL[72] = bL[71]; 
			bL[71] = bL[70]; bL[70] = bL[69]; bL[69] = bL[68]; bL[68] = bL[67]; bL[67] = bL[66]; bL[66] = bL[65]; bL[65] = bL[64]; bL[64] = bL[63]; 
			bL[63] = bL[62]; bL[62] = bL[61]; bL[61] = bL[60]; bL[60] = bL[59]; bL[59] = bL[58]; bL[58] = bL[57]; bL[57] = bL[56]; bL[56] = bL[55]; 
			bL[55] = bL[54]; bL[54] = bL[53]; bL[53] = bL[52]; bL[52] = bL[51]; bL[51] = bL[50]; bL[50] = bL[49]; bL[49] = bL[48]; bL[48] = bL[47]; 
			bL[47] = bL[46]; bL[46] = bL[45]; bL[45] = bL[44]; bL[44] = bL[43]; bL[43] = bL[42]; bL[42] = bL[41]; bL[41] = bL[40]; bL[40] = bL[39]; 
			bL[39] = bL[38]; bL[38] = bL[37]; bL[37] = bL[36]; bL[36] = bL[35]; bL[35] = bL[34]; bL[34] = bL[33]; bL[33] = bL[32]; bL[32] = bL[31]; 
			bL[31] = bL[30]; bL[30] = bL[29]; bL[29] = bL[28]; bL[28] = bL[27]; bL[27] = bL[26]; bL[26] = bL[25]; bL[25] = bL[24]; bL[24] = bL[23]; 
			bL[23] = bL[22]; bL[22] = bL[21]; bL[21] = bL[20]; bL[20] = bL[19]; bL[19] = bL[18]; bL[18] = bL[17]; bL[17] = bL[16]; bL[16] = bL[15]; 
			bL[15] = bL[14]; bL[14] = bL[13]; bL[13] = bL[12]; bL[12] = bL[11]; bL[11] = bL[10]; bL[10] = bL[9]; bL[9] = bL[8]; bL[8] = bL[7]; 
			bL[7] = bL[6]; bL[6] = bL[5]; bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1]; bL[1] = bL[0]; bL[0] = inputSampleL;
			inputSampleL += (bL[1] * (1.31819680801404560  + (0.00362521700518292*fabs(bL[1]))));
			inputSampleL += (bL[2] * (1.37738284126127919  + (0.14134596126256205*fabs(bL[2]))));
			inputSampleL += (bL[3] * (1.09957637225311622  + (0.33199581815501555*fabs(bL[3]))));
			inputSampleL += (bL[4] * (0.62025358899656258  + (0.37476042042088142*fabs(bL[4]))));
			inputSampleL += (bL[5] * (0.12926194402137478  + (0.24702655568406759*fabs(bL[5]))));
			inputSampleL -= (bL[6] * (0.28927985011367602  - (0.13289168298307708*fabs(bL[6]))));
			inputSampleL -= (bL[7] * (0.56518146339033448  - (0.11026641793526121*fabs(bL[7]))));
			inputSampleL -= (bL[8] * (0.59843200696815069  - (0.10139909232154271*fabs(bL[8]))));
			inputSampleL -= (bL[9] * (0.45219971861789204  - (0.13313355255903159*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.32520490032331351  - (0.29009061730364216*fabs(bL[10]))));
			inputSampleL -= (bL[11] * (0.29773131872442909  - (0.45307495356996669*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.31738895975218867  - (0.43198591958928922*fabs(bL[12]))));
			inputSampleL -= (bL[13] * (0.33336150604703757  - (0.24240412850274029*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.32461638442042151  - (0.02779297492397464*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.27812829473787770  + (0.15565718905032455*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.19413454458668097  + (0.32087693535188599*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.12378036344480114  + (0.37736575956794161*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.12550494837257106  + (0.25593811142722300*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.17725736033713696  + (0.07708896413593636*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.22023699647700670  - (0.01600371273599124*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.21987645486953747  + (0.00973336938352798*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.15014276479707978  + (0.11602269600138954*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.05176520203073560  + (0.20383164255692698*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.04276687165294867  + (0.17785002166834518*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.15951546388137597  + (0.06748854885822464*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.30211952144352616  - (0.03440494237025149*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.36462803375134506  - (0.05874284362202409*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.32283960219377539  + (0.01189623197958362*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.19245178663350720  + (0.11088858383712991*fabs(bL[29]))));
			inputSampleL += (bL[30] * (0.00681589563349590  - (0.16314250963457660*fabs(bL[30]))));
			inputSampleL += (bL[31] * (0.20927798345622584  - (0.16952981620487462*fabs(bL[31]))));
			inputSampleL += (bL[32] * (0.25638846543430976  - (0.11462562122281153*fabs(bL[32]))));
			inputSampleL += (bL[33] * (0.04584495673888605  + (0.04669671229804190*fabs(bL[33]))));
			inputSampleL -= (bL[34] * (0.25221561978187662  - (0.19250758741703761*fabs(bL[34]))));
			inputSampleL -= (bL[35] * (0.35662801992424953  - (0.12244680002787561*fabs(bL[35]))));
			inputSampleL -= (bL[36] * (0.21498114329314663  + (0.12152120956991189*fabs(bL[36]))));
			inputSampleL += (bL[37] * (0.00968605571673376  - (0.30597812512858558*fabs(bL[37]))));
			inputSampleL += (bL[38] * (0.18029119870614621  - (0.31569386468576782*fabs(bL[38]))));
			inputSampleL += (bL[39] * (0.22744437185251629  - (0.18028438460422197*fabs(bL[39]))));
			inputSampleL += (bL[40] * (0.09725687638959078  + (0.05479918522830433*fabs(bL[40]))));
			inputSampleL -= (bL[41] * (0.17970389267353537  - (0.29222750363124067*fabs(bL[41]))));
			inputSampleL -= (bL[42] * (0.42371969704763018  - (0.34924957781842314*fabs(bL[42]))));
			inputSampleL -= (bL[43] * (0.43313266755788055  - (0.11503731970288061*fabs(bL[43]))));
			inputSampleL -= (bL[44] * (0.22178165627851801  + (0.25002925550036226*fabs(bL[44]))));
			inputSampleL -= (bL[45] * (0.00410198176852576  + (0.43283281457037676*fabs(bL[45]))));
			inputSampleL += (bL[46] * (0.09072426344812032  - (0.35318250460706446*fabs(bL[46]))));
			inputSampleL += (bL[47] * (0.08405839183965140  - (0.16936391987143717*fabs(bL[47]))));
			inputSampleL -= (bL[48] * (0.01110419756114383  - (0.01247164991313877*fabs(bL[48]))));
			inputSampleL -= (bL[49] * (0.18593334646855278  - (0.14513260199423966*fabs(bL[49]))));
			inputSampleL -= (bL[50] * (0.33665010871497486  - (0.14456206192169668*fabs(bL[50]))));
			inputSampleL -= (bL[51] * (0.32644968491439380  + (0.01594380759082303*fabs(bL[51]))));
			inputSampleL -= (bL[52] * (0.14855437679485431  + (0.23555511219002742*fabs(bL[52]))));
			inputSampleL += (bL[53] * (0.05113019250820622  - (0.35556617126595202*fabs(bL[53]))));
			inputSampleL += (bL[54] * (0.12915754942362243  - (0.28571671825750300*fabs(bL[54]))));
			inputSampleL += (bL[55] * (0.07406865846069306  - (0.10543886479975995*fabs(bL[55]))));
			inputSampleL -= (bL[56] * (0.03669573814193980  - (0.03194267657582078*fabs(bL[56]))));
			inputSampleL -= (bL[57] * (0.13429103278009327  - (0.06145796486786051*fabs(bL[57]))));
			inputSampleL -= (bL[58] * (0.17884021749974641  - (0.00156626902982124*fabs(bL[58]))));
			inputSampleL -= (bL[59] * (0.16138212225178239  + (0.09402070836837134*fabs(bL[59]))));
			inputSampleL -= (bL[60] * (0.10867028245257521  + (0.15407963447815898*fabs(bL[60]))));
			inputSampleL -= (bL[61] * (0.06312416389213464  + (0.11241095544179526*fabs(bL[61]))));
			inputSampleL -= (bL[62] * (0.05826376574081994  - (0.03635253545701986*fabs(bL[62]))));
			inputSampleL -= (bL[63] * (0.07991631148258237  - (0.18041947557579863*fabs(bL[63]))));
			inputSampleL -= (bL[64] * (0.07777397532506500  - (0.20817156738202205*fabs(bL[64]))));
			inputSampleL -= (bL[65] * (0.03812528734394271  - (0.13679963125162486*fabs(bL[65]))));
			inputSampleL += (bL[66] * (0.00204900323943951  + (0.04009000730101046*fabs(bL[66]))));
			inputSampleL += (bL[67] * (0.01779599498119764  - (0.04218637577942354*fabs(bL[67]))));
			inputSampleL += (bL[68] * (0.00950301949319113  - (0.07908911305044238*fabs(bL[68]))));
			inputSampleL -= (bL[69] * (0.04283600714814891  + (0.02716262334097985*fabs(bL[69]))));
			inputSampleL -= (bL[70] * (0.14478320837041933  - (0.08823515277628832*fabs(bL[70]))));
			inputSampleL -= (bL[71] * (0.23250267035795688  - (0.15334197814956568*fabs(bL[71]))));
			inputSampleL -= (bL[72] * (0.22369031446225857  - (0.08550989980799503*fabs(bL[72]))));
			inputSampleL -= (bL[73] * (0.11142757883989868  + (0.08321482928259660*fabs(bL[73]))));
			inputSampleL += (bL[74] * (0.02752318631713307  - (0.25252906099212968*fabs(bL[74]))));
			inputSampleL += (bL[75] * (0.11940028414727490  - (0.34358127205009553*fabs(bL[75]))));
			inputSampleL += (bL[76] * (0.12702057126698307  - (0.31808560130583663*fabs(bL[76]))));
			inputSampleL += (bL[77] * (0.03639067777025356  - (0.17970282734717846*fabs(bL[77]))));
			inputSampleL -= (bL[78] * (0.11389848143835518  + (0.00470616711331971*fabs(bL[78]))));
			inputSampleL -= (bL[79] * (0.23024072979374310  - (0.09772245468884058*fabs(bL[79]))));
			inputSampleL -= (bL[80] * (0.24389015061112601  - (0.09600959885615798*fabs(bL[80]))));
			inputSampleL -= (bL[81] * (0.16680269075295703  - (0.05194978963662898*fabs(bL[81]))));
			inputSampleL -= (bL[82] * (0.05108041495077725  - (0.01796071525570735*fabs(bL[82]))));
			inputSampleL += (bL[83] * (0.06489835353859555  - (0.00808013770331126*fabs(bL[83]))));
			inputSampleL += (bL[84] * (0.15481511440233464  - (0.02674063848284838*fabs(bL[84]))));
			inputSampleL += (bL[85] * (0.18620867857907253  - (0.01786423699465214*fabs(bL[85]))));
			inputSampleL += (bL[86] * (0.13879832139055756  + (0.01584446839973597*fabs(bL[86]))));
			inputSampleL += (bL[87] * (0.04878235109120615  + (0.02962866516075816*fabs(bL[87]))));
			
			temp = (inputSampleL + smoothCabBL)/3.0;
			smoothCabBL = inputSampleL;
			inputSampleL = temp/4.0;
			
			randy = ((double(fpdL)/UINT32_MAX)*0.05);
			drySampleL = ((((inputSampleL*(1-randy))+(lastCabSampleL*randy))*wet)+(drySampleL*(1.0-wet)))*outputlevel;
			lastCabSampleL = inputSampleL;
			inputSampleL = drySampleL; //cab L
			
			
			temp = (inputSampleR + smoothCabAR)/3.0;
			smoothCabAR = inputSampleR;
			inputSampleR = temp;
			
			bR[87] = bR[86]; bR[86] = bR[85]; bR[85] = bR[84]; bR[84] = bR[83]; bR[83] = bR[82]; bR[82] = bR[81]; bR[81] = bR[80]; bR[80] = bR[79]; 
			bR[79] = bR[78]; bR[78] = bR[77]; bR[77] = bR[76]; bR[76] = bR[75]; bR[75] = bR[74]; bR[74] = bR[73]; bR[73] = bR[72]; bR[72] = bR[71]; 
			bR[71] = bR[70]; bR[70] = bR[69]; bR[69] = bR[68]; bR[68] = bR[67]; bR[67] = bR[66]; bR[66] = bR[65]; bR[65] = bR[64]; bR[64] = bR[63]; 
			bR[63] = bR[62]; bR[62] = bR[61]; bR[61] = bR[60]; bR[60] = bR[59]; bR[59] = bR[58]; bR[58] = bR[57]; bR[57] = bR[56]; bR[56] = bR[55]; 
			bR[55] = bR[54]; bR[54] = bR[53]; bR[53] = bR[52]; bR[52] = bR[51]; bR[51] = bR[50]; bR[50] = bR[49]; bR[49] = bR[48]; bR[48] = bR[47]; 
			bR[47] = bR[46]; bR[46] = bR[45]; bR[45] = bR[44]; bR[44] = bR[43]; bR[43] = bR[42]; bR[42] = bR[41]; bR[41] = bR[40]; bR[40] = bR[39]; 
			bR[39] = bR[38]; bR[38] = bR[37]; bR[37] = bR[36]; bR[36] = bR[35]; bR[35] = bR[34]; bR[34] = bR[33]; bR[33] = bR[32]; bR[32] = bR[31]; 
			bR[31] = bR[30]; bR[30] = bR[29]; bR[29] = bR[28]; bR[28] = bR[27]; bR[27] = bR[26]; bR[26] = bR[25]; bR[25] = bR[24]; bR[24] = bR[23]; 
			bR[23] = bR[22]; bR[22] = bR[21]; bR[21] = bR[20]; bR[20] = bR[19]; bR[19] = bR[18]; bR[18] = bR[17]; bR[17] = bR[16]; bR[16] = bR[15]; 
			bR[15] = bR[14]; bR[14] = bR[13]; bR[13] = bR[12]; bR[12] = bR[11]; bR[11] = bR[10]; bR[10] = bR[9]; bR[9] = bR[8]; bR[8] = bR[7]; 
			bR[7] = bR[6]; bR[6] = bR[5]; bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1]; bR[1] = bR[0]; bR[0] = inputSampleR;
			inputSampleR += (bR[1] * (1.31819680801404560  + (0.00362521700518292*fabs(bR[1]))));
			inputSampleR += (bR[2] * (1.37738284126127919  + (0.14134596126256205*fabs(bR[2]))));
			inputSampleR += (bR[3] * (1.09957637225311622  + (0.33199581815501555*fabs(bR[3]))));
			inputSampleR += (bR[4] * (0.62025358899656258  + (0.37476042042088142*fabs(bR[4]))));
			inputSampleR += (bR[5] * (0.12926194402137478  + (0.24702655568406759*fabs(bR[5]))));
			inputSampleR -= (bR[6] * (0.28927985011367602  - (0.13289168298307708*fabs(bR[6]))));
			inputSampleR -= (bR[7] * (0.56518146339033448  - (0.11026641793526121*fabs(bR[7]))));
			inputSampleR -= (bR[8] * (0.59843200696815069  - (0.10139909232154271*fabs(bR[8]))));
			inputSampleR -= (bR[9] * (0.45219971861789204  - (0.13313355255903159*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.32520490032331351  - (0.29009061730364216*fabs(bR[10]))));
			inputSampleR -= (bR[11] * (0.29773131872442909  - (0.45307495356996669*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (0.31738895975218867  - (0.43198591958928922*fabs(bR[12]))));
			inputSampleR -= (bR[13] * (0.33336150604703757  - (0.24240412850274029*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.32461638442042151  - (0.02779297492397464*fabs(bR[14]))));
			inputSampleR -= (bR[15] * (0.27812829473787770  + (0.15565718905032455*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.19413454458668097  + (0.32087693535188599*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.12378036344480114  + (0.37736575956794161*fabs(bR[17]))));
			inputSampleR -= (bR[18] * (0.12550494837257106  + (0.25593811142722300*fabs(bR[18]))));
			inputSampleR -= (bR[19] * (0.17725736033713696  + (0.07708896413593636*fabs(bR[19]))));
			inputSampleR -= (bR[20] * (0.22023699647700670  - (0.01600371273599124*fabs(bR[20]))));
			inputSampleR -= (bR[21] * (0.21987645486953747  + (0.00973336938352798*fabs(bR[21]))));
			inputSampleR -= (bR[22] * (0.15014276479707978  + (0.11602269600138954*fabs(bR[22]))));
			inputSampleR -= (bR[23] * (0.05176520203073560  + (0.20383164255692698*fabs(bR[23]))));
			inputSampleR -= (bR[24] * (0.04276687165294867  + (0.17785002166834518*fabs(bR[24]))));
			inputSampleR -= (bR[25] * (0.15951546388137597  + (0.06748854885822464*fabs(bR[25]))));
			inputSampleR -= (bR[26] * (0.30211952144352616  - (0.03440494237025149*fabs(bR[26]))));
			inputSampleR -= (bR[27] * (0.36462803375134506  - (0.05874284362202409*fabs(bR[27]))));
			inputSampleR -= (bR[28] * (0.32283960219377539  + (0.01189623197958362*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.19245178663350720  + (0.11088858383712991*fabs(bR[29]))));
			inputSampleR += (bR[30] * (0.00681589563349590  - (0.16314250963457660*fabs(bR[30]))));
			inputSampleR += (bR[31] * (0.20927798345622584  - (0.16952981620487462*fabs(bR[31]))));
			inputSampleR += (bR[32] * (0.25638846543430976  - (0.11462562122281153*fabs(bR[32]))));
			inputSampleR += (bR[33] * (0.04584495673888605  + (0.04669671229804190*fabs(bR[33]))));
			inputSampleR -= (bR[34] * (0.25221561978187662  - (0.19250758741703761*fabs(bR[34]))));
			inputSampleR -= (bR[35] * (0.35662801992424953  - (0.12244680002787561*fabs(bR[35]))));
			inputSampleR -= (bR[36] * (0.21498114329314663  + (0.12152120956991189*fabs(bR[36]))));
			inputSampleR += (bR[37] * (0.00968605571673376  - (0.30597812512858558*fabs(bR[37]))));
			inputSampleR += (bR[38] * (0.18029119870614621  - (0.31569386468576782*fabs(bR[38]))));
			inputSampleR += (bR[39] * (0.22744437185251629  - (0.18028438460422197*fabs(bR[39]))));
			inputSampleR += (bR[40] * (0.09725687638959078  + (0.05479918522830433*fabs(bR[40]))));
			inputSampleR -= (bR[41] * (0.17970389267353537  - (0.29222750363124067*fabs(bR[41]))));
			inputSampleR -= (bR[42] * (0.42371969704763018  - (0.34924957781842314*fabs(bR[42]))));
			inputSampleR -= (bR[43] * (0.43313266755788055  - (0.11503731970288061*fabs(bR[43]))));
			inputSampleR -= (bR[44] * (0.22178165627851801  + (0.25002925550036226*fabs(bR[44]))));
			inputSampleR -= (bR[45] * (0.00410198176852576  + (0.43283281457037676*fabs(bR[45]))));
			inputSampleR += (bR[46] * (0.09072426344812032  - (0.35318250460706446*fabs(bR[46]))));
			inputSampleR += (bR[47] * (0.08405839183965140  - (0.16936391987143717*fabs(bR[47]))));
			inputSampleR -= (bR[48] * (0.01110419756114383  - (0.01247164991313877*fabs(bR[48]))));
			inputSampleR -= (bR[49] * (0.18593334646855278  - (0.14513260199423966*fabs(bR[49]))));
			inputSampleR -= (bR[50] * (0.33665010871497486  - (0.14456206192169668*fabs(bR[50]))));
			inputSampleR -= (bR[51] * (0.32644968491439380  + (0.01594380759082303*fabs(bR[51]))));
			inputSampleR -= (bR[52] * (0.14855437679485431  + (0.23555511219002742*fabs(bR[52]))));
			inputSampleR += (bR[53] * (0.05113019250820622  - (0.35556617126595202*fabs(bR[53]))));
			inputSampleR += (bR[54] * (0.12915754942362243  - (0.28571671825750300*fabs(bR[54]))));
			inputSampleR += (bR[55] * (0.07406865846069306  - (0.10543886479975995*fabs(bR[55]))));
			inputSampleR -= (bR[56] * (0.03669573814193980  - (0.03194267657582078*fabs(bR[56]))));
			inputSampleR -= (bR[57] * (0.13429103278009327  - (0.06145796486786051*fabs(bR[57]))));
			inputSampleR -= (bR[58] * (0.17884021749974641  - (0.00156626902982124*fabs(bR[58]))));
			inputSampleR -= (bR[59] * (0.16138212225178239  + (0.09402070836837134*fabs(bR[59]))));
			inputSampleR -= (bR[60] * (0.10867028245257521  + (0.15407963447815898*fabs(bR[60]))));
			inputSampleR -= (bR[61] * (0.06312416389213464  + (0.11241095544179526*fabs(bR[61]))));
			inputSampleR -= (bR[62] * (0.05826376574081994  - (0.03635253545701986*fabs(bR[62]))));
			inputSampleR -= (bR[63] * (0.07991631148258237  - (0.18041947557579863*fabs(bR[63]))));
			inputSampleR -= (bR[64] * (0.07777397532506500  - (0.20817156738202205*fabs(bR[64]))));
			inputSampleR -= (bR[65] * (0.03812528734394271  - (0.13679963125162486*fabs(bR[65]))));
			inputSampleR += (bR[66] * (0.00204900323943951  + (0.04009000730101046*fabs(bR[66]))));
			inputSampleR += (bR[67] * (0.01779599498119764  - (0.04218637577942354*fabs(bR[67]))));
			inputSampleR += (bR[68] * (0.00950301949319113  - (0.07908911305044238*fabs(bR[68]))));
			inputSampleR -= (bR[69] * (0.04283600714814891  + (0.02716262334097985*fabs(bR[69]))));
			inputSampleR -= (bR[70] * (0.14478320837041933  - (0.08823515277628832*fabs(bR[70]))));
			inputSampleR -= (bR[71] * (0.23250267035795688  - (0.15334197814956568*fabs(bR[71]))));
			inputSampleR -= (bR[72] * (0.22369031446225857  - (0.08550989980799503*fabs(bR[72]))));
			inputSampleR -= (bR[73] * (0.11142757883989868  + (0.08321482928259660*fabs(bR[73]))));
			inputSampleR += (bR[74] * (0.02752318631713307  - (0.25252906099212968*fabs(bR[74]))));
			inputSampleR += (bR[75] * (0.11940028414727490  - (0.34358127205009553*fabs(bR[75]))));
			inputSampleR += (bR[76] * (0.12702057126698307  - (0.31808560130583663*fabs(bR[76]))));
			inputSampleR += (bR[77] * (0.03639067777025356  - (0.17970282734717846*fabs(bR[77]))));
			inputSampleR -= (bR[78] * (0.11389848143835518  + (0.00470616711331971*fabs(bR[78]))));
			inputSampleR -= (bR[79] * (0.23024072979374310  - (0.09772245468884058*fabs(bR[79]))));
			inputSampleR -= (bR[80] * (0.24389015061112601  - (0.09600959885615798*fabs(bR[80]))));
			inputSampleR -= (bR[81] * (0.16680269075295703  - (0.05194978963662898*fabs(bR[81]))));
			inputSampleR -= (bR[82] * (0.05108041495077725  - (0.01796071525570735*fabs(bR[82]))));
			inputSampleR += (bR[83] * (0.06489835353859555  - (0.00808013770331126*fabs(bR[83]))));
			inputSampleR += (bR[84] * (0.15481511440233464  - (0.02674063848284838*fabs(bR[84]))));
			inputSampleR += (bR[85] * (0.18620867857907253  - (0.01786423699465214*fabs(bR[85]))));
			inputSampleR += (bR[86] * (0.13879832139055756  + (0.01584446839973597*fabs(bR[86]))));
			inputSampleR += (bR[87] * (0.04878235109120615  + (0.02962866516075816*fabs(bR[87]))));
			
			temp = (inputSampleR + smoothCabBR)/3.0;
			smoothCabBR = inputSampleR;
			inputSampleR = temp/4.0;
			
			
			randy = ((double(fpdR)/UINT32_MAX)*0.05);
			drySampleR = ((((inputSampleR*(1-randy))+(lastCabSampleR*randy))*wet)+(drySampleR*(1.0-wet)))*outputlevel;
			lastCabSampleR = inputSampleR;
			inputSampleR = drySampleR; //cab
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
		}
		switch (cycleEnd) //multi-pole average using lastRef[] variables
		{
			case 4:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[7])*0.5;
				lastRefL[7] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[7])*0.5;
				lastRefR[7] = lastRefR[8]; //continue, do not break
			case 3:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[6])*0.5;
				lastRefL[6] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[6])*0.5;
				lastRefR[6] = lastRefR[8]; //continue, do not break
			case 2:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[5])*0.5;
				lastRefL[5] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[5])*0.5;
				lastRefR[5] = lastRefR[8]; //continue, do not break
			case 1:
				break; //no further averaging
		}
				
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

void MidAmp::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double inputlevel = A*3.0;
	double samplerate = getSampleRate();
	double EQ = (B/ samplerate)*22050.0;
	double basstrim = B;
	double outputlevel = C;
	double wet = D;
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= samplerate;
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check		
	
	double bleed = outputlevel/6.0;
	double BEQ = (bleed / samplerate)*44100.0;
	int side = (int)(0.0006092985*samplerate);
	if (side > 126) side = 126;
	int down = side + 1;
	inputlevel = (inputlevel + (inputlevel*basstrim))/2.0;
	
	double cutoff = (15000.0+(B*10000.0)) / getSampleRate();
	if (cutoff > 0.49) cutoff = 0.49; //don't crash if run at 44.1k
	if (cutoff < 0.001) cutoff = 0.001; //or if cutoff's too low
	
	fixF[fix_freq] = fixE[fix_freq] = fixD[fix_freq] = fixC[fix_freq] = fixB[fix_freq] = fixA[fix_freq] = cutoff;
	
    fixA[fix_reso] = 4.46570214;
	fixB[fix_reso] = 1.51387132;
	fixC[fix_reso] = 0.93979296;
	fixD[fix_reso] = 0.70710678;
	fixE[fix_reso] = 0.52972649;
	fixF[fix_reso] = 0.50316379;
	
	double K = tan(M_PI * fixA[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fixA[fix_reso] + K * K);
	fixA[fix_a0] = K * K * norm;
	fixA[fix_a1] = 2.0 * fixA[fix_a0];
	fixA[fix_a2] = fixA[fix_a0];
	fixA[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixA[fix_b2] = (1.0 - K / fixA[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixB[fix_freq]);
	norm = 1.0 / (1.0 + K / fixB[fix_reso] + K * K);
	fixB[fix_a0] = K * K * norm;
	fixB[fix_a1] = 2.0 * fixB[fix_a0];
	fixB[fix_a2] = fixB[fix_a0];
	fixB[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixB[fix_b2] = (1.0 - K / fixB[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixC[fix_freq]);
	norm = 1.0 / (1.0 + K / fixC[fix_reso] + K * K);
	fixC[fix_a0] = K * K * norm;
	fixC[fix_a1] = 2.0 * fixC[fix_a0];
	fixC[fix_a2] = fixC[fix_a0];
	fixC[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixC[fix_b2] = (1.0 - K / fixC[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixD[fix_freq]);
	norm = 1.0 / (1.0 + K / fixD[fix_reso] + K * K);
	fixD[fix_a0] = K * K * norm;
	fixD[fix_a1] = 2.0 * fixD[fix_a0];
	fixD[fix_a2] = fixD[fix_a0];
	fixD[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixD[fix_b2] = (1.0 - K / fixD[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixE[fix_freq]);
	norm = 1.0 / (1.0 + K / fixE[fix_reso] + K * K);
	fixE[fix_a0] = K * K * norm;
	fixE[fix_a1] = 2.0 * fixE[fix_a0];
	fixE[fix_a2] = fixE[fix_a0];
	fixE[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixE[fix_b2] = (1.0 - K / fixE[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixF[fix_freq]);
	norm = 1.0 / (1.0 + K / fixF[fix_reso] + K * K);
	fixF[fix_a0] = K * K * norm;
	fixF[fix_a1] = 2.0 * fixF[fix_a0];
	fixF[fix_a2] = fixF[fix_a0];
	fixF[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixF[fix_b2] = (1.0 - K / fixF[fix_reso] + K * K) * norm;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double outSample = (inputSampleL * fixA[fix_a0]) + fixA[fix_sL1];
		fixA[fix_sL1] = (inputSampleL * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sL2];
		fixA[fix_sL2] = (inputSampleL * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixA[fix_a0]) + fixA[fix_sR1];
		fixA[fix_sR1] = (inputSampleR * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sR2];
		fixA[fix_sR2] = (inputSampleR * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		double skewL = (inputSampleL - lastSampleL);
		lastSampleL = inputSampleL;
		//skew will be direction/angle
		double bridgerectifier = fabs(skewL);
		if (bridgerectifier > 3.1415926) bridgerectifier = 3.1415926;
		//for skew we want it to go to zero effect again, so we use full range of the sine
		bridgerectifier = sin(bridgerectifier);
		if (skewL > 0.0) skewL = bridgerectifier;
		else skewL = -bridgerectifier;
		//skew is now sined and clamped and then re-amplified again
		skewL *= inputSampleL;
		
		double skewR = (inputSampleR - lastSampleR);
		lastSampleR = inputSampleR;
		//skew will be direction/angle
		bridgerectifier = fabs(skewR);
		if (bridgerectifier > 3.1415926) bridgerectifier = 3.1415926;
		//for skew we want it to go to zero effect again, so we use full range of the sine
		bridgerectifier = sin(bridgerectifier);
		if (skewR > 0.0) skewR = bridgerectifier;
		else skewR = -bridgerectifier;
		//skew is now sined and clamped and then re-amplified again
		skewR *= inputSampleR;
		
		outSample = (inputSampleL * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSampleL * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSampleL * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixB[fix_a0]) + fixB[fix_sR1];
		fixB[fix_sR1] = (inputSampleR * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sR2];
		fixB[fix_sR2] = (inputSampleR * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		double offsetL = (1.0 - EQ) + (fabs(inputSampleL)*EQ);
		if (offsetL < 0.0) offsetL = 0.0;
		if (offsetL > 1.0) offsetL = 1.0;
		iirSampleAL = (iirSampleAL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleAL*0.8);
		//highpass
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewL;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//overdrive
		iirSampleCL = (iirSampleCL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleCL*0.6);
		//highpass
		skewL /= 1.57079633;
		//finished first gain stage
		
		inputSampleR *= inputlevel;
		double offsetR = (1.0 - EQ) + (fabs(inputSampleR)*EQ);
		if (offsetR < 0.0) offsetR = 0.0;
		if (offsetR > 1.0) offsetR = 1.0;
		iirSampleAR = (iirSampleAR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleAR*0.8);
		//highpass
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewR;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//overdrive
		iirSampleCR = (iirSampleCR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleCR*0.6);
		//highpass
		skewR /= 1.57079633;
		//finished first gain stage
		
		outSample = (inputSampleL * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSampleL * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSampleL * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixC[fix_a0]) + fixC[fix_sR1];
		fixC[fix_sR1] = (inputSampleR * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sR2];
		fixC[fix_sR2] = (inputSampleR * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		offsetL = (1.0 + offsetL) / 2.0;
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//overdrive
		iirSampleGL = (iirSampleGL * (1.0 - EQ)) + (inputSampleL * EQ);
		inputSampleL = inputSampleL - (iirSampleGL*0.4);
		//highpass. no further basscut, no more highpasses
		iirSampleDL = (iirSampleDL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = iirSampleDL;
		skewL /= 1.57079633;
		//lowpass. Use offset from before gain stage
		
		inputSampleR *= inputlevel;
		offsetR = (1.0 + offsetR) / 2.0;
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//overdrive
		iirSampleGR = (iirSampleGR * (1.0 - EQ)) + (inputSampleR * EQ);
		inputSampleR = inputSampleR - (iirSampleGR*0.4);
		//highpass. no further basscut, no more highpasses
		iirSampleDR = (iirSampleDR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = iirSampleDR;
		skewR /= 1.57079633;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSampleL * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSampleL * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSampleL * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixD[fix_a0]) + fixD[fix_sR1];
		fixD[fix_sR1] = (inputSampleR * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sR2];
		fixD[fix_sR2] = (inputSampleR * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//output stage has less gain, no highpass, straight lowpass
		iirSampleEL = (iirSampleEL * (1.0 - EQ)) + (inputSampleL * EQ);
		inputSampleL = iirSampleEL;
		//lowpass. Use offset from before gain stage
		
		inputSampleR *= inputlevel;
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//output stage has less gain, no highpass, straight lowpass
		iirSampleER = (iirSampleER * (1.0 - EQ)) + (inputSampleR * EQ);
		inputSampleR = iirSampleER;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSampleL * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSampleL * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSampleL * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixE[fix_a0]) + fixE[fix_sR1];
		fixE[fix_sR1] = (inputSampleR * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sR2];
		fixE[fix_sR2] = (inputSampleR * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		iirSampleFL = (iirSampleFL * (1.0 - (offsetL * BEQ))) + (inputSampleL * (offsetL * BEQ));
		//extra lowpass for 4*12" speakers
		iirSampleFR = (iirSampleFR * (1.0 - (offsetR * BEQ))) + (inputSampleR * (offsetR * BEQ));
		//extra lowpass for 4*12" speakers
		
		if (count < 0 || count > 128) {count = 128;}
		double resultBL = 0.0;
		double resultBR = 0.0;
		if (flip)
		{
			OddL[count+128] = OddL[count] = iirSampleFL;
			resultBL = (OddL[count+down] + OddL[count+side]);
			OddR[count+128] = OddR[count] = iirSampleFR;
			resultBR = (OddR[count+down] + OddR[count+side]);
		}
		else
		{
			EvenL[count+128] = EvenL[count] = iirSampleFL;
			resultBL = (EvenL[count+down] + EvenL[count+side]);
			EvenR[count+128] = EvenR[count] = iirSampleFR;
			resultBR = (EvenR[count+down] + EvenR[count+side]);
		}
		count--;
		
		iirSampleBL = (iirSampleBL * (1.0 - (offsetL * BEQ))) + (resultBL * (offsetL * BEQ));
		inputSampleL += (iirSampleBL*bleed);
		//extra lowpass for 4*12" speakers
		
		iirSampleBR = (iirSampleBR * (1.0 - (offsetR * BEQ))) + (resultBR * (offsetR * BEQ));
		inputSampleR += (iirSampleBR*bleed);
		//extra lowpass for 4*12" speakers
		
		iirSampleHL = (iirSampleHL * (1.0 - (offsetL * BEQ))) + (inputSampleL * (offsetL * BEQ));
		inputSampleL += iirSampleHL;
		
		iirSampleHR = (iirSampleHR * (1.0 - (offsetR * BEQ))) + (inputSampleR * (offsetR * BEQ));
		inputSampleR += iirSampleHR;
		
		inputSampleL = sin(inputSampleL*outputlevel);		
		double randy = ((double(fpdL)/UINT32_MAX)*0.047);
		inputSampleL = ((inputSampleL*(1.0-randy))+(storeSampleL*randy))*outputlevel;
		storeSampleL = inputSampleL;
		
		inputSampleR = sin(inputSampleR*outputlevel);		
		randy = ((double(fpdR)/UINT32_MAX)*0.047);
		inputSampleR = ((inputSampleR*(1.0-randy))+(storeSampleR*randy))*outputlevel;
		storeSampleR = inputSampleR;
		
		outSample = (inputSampleL * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSampleL * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSampleL * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixF[fix_a0]) + fixF[fix_sR1];
		fixF[fix_sR1] = (inputSampleR * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sR2];
		fixF[fix_sR2] = (inputSampleR * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		flip = !flip;
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		//amp
		
		cycle++;
		if (cycle == cycleEnd) {
			double temp = (inputSampleL + smoothCabAL)/3.0;
			smoothCabAL = inputSampleL;
			inputSampleL = temp;
			
			bL[87] = bL[86]; bL[86] = bL[85]; bL[85] = bL[84]; bL[84] = bL[83]; bL[83] = bL[82]; bL[82] = bL[81]; bL[81] = bL[80]; bL[80] = bL[79]; 
			bL[79] = bL[78]; bL[78] = bL[77]; bL[77] = bL[76]; bL[76] = bL[75]; bL[75] = bL[74]; bL[74] = bL[73]; bL[73] = bL[72]; bL[72] = bL[71]; 
			bL[71] = bL[70]; bL[70] = bL[69]; bL[69] = bL[68]; bL[68] = bL[67]; bL[67] = bL[66]; bL[66] = bL[65]; bL[65] = bL[64]; bL[64] = bL[63]; 
			bL[63] = bL[62]; bL[62] = bL[61]; bL[61] = bL[60]; bL[60] = bL[59]; bL[59] = bL[58]; bL[58] = bL[57]; bL[57] = bL[56]; bL[56] = bL[55]; 
			bL[55] = bL[54]; bL[54] = bL[53]; bL[53] = bL[52]; bL[52] = bL[51]; bL[51] = bL[50]; bL[50] = bL[49]; bL[49] = bL[48]; bL[48] = bL[47]; 
			bL[47] = bL[46]; bL[46] = bL[45]; bL[45] = bL[44]; bL[44] = bL[43]; bL[43] = bL[42]; bL[42] = bL[41]; bL[41] = bL[40]; bL[40] = bL[39]; 
			bL[39] = bL[38]; bL[38] = bL[37]; bL[37] = bL[36]; bL[36] = bL[35]; bL[35] = bL[34]; bL[34] = bL[33]; bL[33] = bL[32]; bL[32] = bL[31]; 
			bL[31] = bL[30]; bL[30] = bL[29]; bL[29] = bL[28]; bL[28] = bL[27]; bL[27] = bL[26]; bL[26] = bL[25]; bL[25] = bL[24]; bL[24] = bL[23]; 
			bL[23] = bL[22]; bL[22] = bL[21]; bL[21] = bL[20]; bL[20] = bL[19]; bL[19] = bL[18]; bL[18] = bL[17]; bL[17] = bL[16]; bL[16] = bL[15]; 
			bL[15] = bL[14]; bL[14] = bL[13]; bL[13] = bL[12]; bL[12] = bL[11]; bL[11] = bL[10]; bL[10] = bL[9]; bL[9] = bL[8]; bL[8] = bL[7]; 
			bL[7] = bL[6]; bL[6] = bL[5]; bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1]; bL[1] = bL[0]; bL[0] = inputSampleL;
			inputSampleL += (bL[1] * (1.31819680801404560  + (0.00362521700518292*fabs(bL[1]))));
			inputSampleL += (bL[2] * (1.37738284126127919  + (0.14134596126256205*fabs(bL[2]))));
			inputSampleL += (bL[3] * (1.09957637225311622  + (0.33199581815501555*fabs(bL[3]))));
			inputSampleL += (bL[4] * (0.62025358899656258  + (0.37476042042088142*fabs(bL[4]))));
			inputSampleL += (bL[5] * (0.12926194402137478  + (0.24702655568406759*fabs(bL[5]))));
			inputSampleL -= (bL[6] * (0.28927985011367602  - (0.13289168298307708*fabs(bL[6]))));
			inputSampleL -= (bL[7] * (0.56518146339033448  - (0.11026641793526121*fabs(bL[7]))));
			inputSampleL -= (bL[8] * (0.59843200696815069  - (0.10139909232154271*fabs(bL[8]))));
			inputSampleL -= (bL[9] * (0.45219971861789204  - (0.13313355255903159*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.32520490032331351  - (0.29009061730364216*fabs(bL[10]))));
			inputSampleL -= (bL[11] * (0.29773131872442909  - (0.45307495356996669*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.31738895975218867  - (0.43198591958928922*fabs(bL[12]))));
			inputSampleL -= (bL[13] * (0.33336150604703757  - (0.24240412850274029*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.32461638442042151  - (0.02779297492397464*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.27812829473787770  + (0.15565718905032455*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.19413454458668097  + (0.32087693535188599*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.12378036344480114  + (0.37736575956794161*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.12550494837257106  + (0.25593811142722300*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.17725736033713696  + (0.07708896413593636*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.22023699647700670  - (0.01600371273599124*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.21987645486953747  + (0.00973336938352798*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.15014276479707978  + (0.11602269600138954*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.05176520203073560  + (0.20383164255692698*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.04276687165294867  + (0.17785002166834518*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.15951546388137597  + (0.06748854885822464*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.30211952144352616  - (0.03440494237025149*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.36462803375134506  - (0.05874284362202409*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.32283960219377539  + (0.01189623197958362*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.19245178663350720  + (0.11088858383712991*fabs(bL[29]))));
			inputSampleL += (bL[30] * (0.00681589563349590  - (0.16314250963457660*fabs(bL[30]))));
			inputSampleL += (bL[31] * (0.20927798345622584  - (0.16952981620487462*fabs(bL[31]))));
			inputSampleL += (bL[32] * (0.25638846543430976  - (0.11462562122281153*fabs(bL[32]))));
			inputSampleL += (bL[33] * (0.04584495673888605  + (0.04669671229804190*fabs(bL[33]))));
			inputSampleL -= (bL[34] * (0.25221561978187662  - (0.19250758741703761*fabs(bL[34]))));
			inputSampleL -= (bL[35] * (0.35662801992424953  - (0.12244680002787561*fabs(bL[35]))));
			inputSampleL -= (bL[36] * (0.21498114329314663  + (0.12152120956991189*fabs(bL[36]))));
			inputSampleL += (bL[37] * (0.00968605571673376  - (0.30597812512858558*fabs(bL[37]))));
			inputSampleL += (bL[38] * (0.18029119870614621  - (0.31569386468576782*fabs(bL[38]))));
			inputSampleL += (bL[39] * (0.22744437185251629  - (0.18028438460422197*fabs(bL[39]))));
			inputSampleL += (bL[40] * (0.09725687638959078  + (0.05479918522830433*fabs(bL[40]))));
			inputSampleL -= (bL[41] * (0.17970389267353537  - (0.29222750363124067*fabs(bL[41]))));
			inputSampleL -= (bL[42] * (0.42371969704763018  - (0.34924957781842314*fabs(bL[42]))));
			inputSampleL -= (bL[43] * (0.43313266755788055  - (0.11503731970288061*fabs(bL[43]))));
			inputSampleL -= (bL[44] * (0.22178165627851801  + (0.25002925550036226*fabs(bL[44]))));
			inputSampleL -= (bL[45] * (0.00410198176852576  + (0.43283281457037676*fabs(bL[45]))));
			inputSampleL += (bL[46] * (0.09072426344812032  - (0.35318250460706446*fabs(bL[46]))));
			inputSampleL += (bL[47] * (0.08405839183965140  - (0.16936391987143717*fabs(bL[47]))));
			inputSampleL -= (bL[48] * (0.01110419756114383  - (0.01247164991313877*fabs(bL[48]))));
			inputSampleL -= (bL[49] * (0.18593334646855278  - (0.14513260199423966*fabs(bL[49]))));
			inputSampleL -= (bL[50] * (0.33665010871497486  - (0.14456206192169668*fabs(bL[50]))));
			inputSampleL -= (bL[51] * (0.32644968491439380  + (0.01594380759082303*fabs(bL[51]))));
			inputSampleL -= (bL[52] * (0.14855437679485431  + (0.23555511219002742*fabs(bL[52]))));
			inputSampleL += (bL[53] * (0.05113019250820622  - (0.35556617126595202*fabs(bL[53]))));
			inputSampleL += (bL[54] * (0.12915754942362243  - (0.28571671825750300*fabs(bL[54]))));
			inputSampleL += (bL[55] * (0.07406865846069306  - (0.10543886479975995*fabs(bL[55]))));
			inputSampleL -= (bL[56] * (0.03669573814193980  - (0.03194267657582078*fabs(bL[56]))));
			inputSampleL -= (bL[57] * (0.13429103278009327  - (0.06145796486786051*fabs(bL[57]))));
			inputSampleL -= (bL[58] * (0.17884021749974641  - (0.00156626902982124*fabs(bL[58]))));
			inputSampleL -= (bL[59] * (0.16138212225178239  + (0.09402070836837134*fabs(bL[59]))));
			inputSampleL -= (bL[60] * (0.10867028245257521  + (0.15407963447815898*fabs(bL[60]))));
			inputSampleL -= (bL[61] * (0.06312416389213464  + (0.11241095544179526*fabs(bL[61]))));
			inputSampleL -= (bL[62] * (0.05826376574081994  - (0.03635253545701986*fabs(bL[62]))));
			inputSampleL -= (bL[63] * (0.07991631148258237  - (0.18041947557579863*fabs(bL[63]))));
			inputSampleL -= (bL[64] * (0.07777397532506500  - (0.20817156738202205*fabs(bL[64]))));
			inputSampleL -= (bL[65] * (0.03812528734394271  - (0.13679963125162486*fabs(bL[65]))));
			inputSampleL += (bL[66] * (0.00204900323943951  + (0.04009000730101046*fabs(bL[66]))));
			inputSampleL += (bL[67] * (0.01779599498119764  - (0.04218637577942354*fabs(bL[67]))));
			inputSampleL += (bL[68] * (0.00950301949319113  - (0.07908911305044238*fabs(bL[68]))));
			inputSampleL -= (bL[69] * (0.04283600714814891  + (0.02716262334097985*fabs(bL[69]))));
			inputSampleL -= (bL[70] * (0.14478320837041933  - (0.08823515277628832*fabs(bL[70]))));
			inputSampleL -= (bL[71] * (0.23250267035795688  - (0.15334197814956568*fabs(bL[71]))));
			inputSampleL -= (bL[72] * (0.22369031446225857  - (0.08550989980799503*fabs(bL[72]))));
			inputSampleL -= (bL[73] * (0.11142757883989868  + (0.08321482928259660*fabs(bL[73]))));
			inputSampleL += (bL[74] * (0.02752318631713307  - (0.25252906099212968*fabs(bL[74]))));
			inputSampleL += (bL[75] * (0.11940028414727490  - (0.34358127205009553*fabs(bL[75]))));
			inputSampleL += (bL[76] * (0.12702057126698307  - (0.31808560130583663*fabs(bL[76]))));
			inputSampleL += (bL[77] * (0.03639067777025356  - (0.17970282734717846*fabs(bL[77]))));
			inputSampleL -= (bL[78] * (0.11389848143835518  + (0.00470616711331971*fabs(bL[78]))));
			inputSampleL -= (bL[79] * (0.23024072979374310  - (0.09772245468884058*fabs(bL[79]))));
			inputSampleL -= (bL[80] * (0.24389015061112601  - (0.09600959885615798*fabs(bL[80]))));
			inputSampleL -= (bL[81] * (0.16680269075295703  - (0.05194978963662898*fabs(bL[81]))));
			inputSampleL -= (bL[82] * (0.05108041495077725  - (0.01796071525570735*fabs(bL[82]))));
			inputSampleL += (bL[83] * (0.06489835353859555  - (0.00808013770331126*fabs(bL[83]))));
			inputSampleL += (bL[84] * (0.15481511440233464  - (0.02674063848284838*fabs(bL[84]))));
			inputSampleL += (bL[85] * (0.18620867857907253  - (0.01786423699465214*fabs(bL[85]))));
			inputSampleL += (bL[86] * (0.13879832139055756  + (0.01584446839973597*fabs(bL[86]))));
			inputSampleL += (bL[87] * (0.04878235109120615  + (0.02962866516075816*fabs(bL[87]))));
			
			temp = (inputSampleL + smoothCabBL)/3.0;
			smoothCabBL = inputSampleL;
			inputSampleL = temp/4.0;
			
			randy = ((double(fpdL)/UINT32_MAX)*0.05);
			drySampleL = ((((inputSampleL*(1-randy))+(lastCabSampleL*randy))*wet)+(drySampleL*(1.0-wet)))*outputlevel;
			lastCabSampleL = inputSampleL;
			inputSampleL = drySampleL; //cab L
			
			
			temp = (inputSampleR + smoothCabAR)/3.0;
			smoothCabAR = inputSampleR;
			inputSampleR = temp;
			
			bR[87] = bR[86]; bR[86] = bR[85]; bR[85] = bR[84]; bR[84] = bR[83]; bR[83] = bR[82]; bR[82] = bR[81]; bR[81] = bR[80]; bR[80] = bR[79]; 
			bR[79] = bR[78]; bR[78] = bR[77]; bR[77] = bR[76]; bR[76] = bR[75]; bR[75] = bR[74]; bR[74] = bR[73]; bR[73] = bR[72]; bR[72] = bR[71]; 
			bR[71] = bR[70]; bR[70] = bR[69]; bR[69] = bR[68]; bR[68] = bR[67]; bR[67] = bR[66]; bR[66] = bR[65]; bR[65] = bR[64]; bR[64] = bR[63]; 
			bR[63] = bR[62]; bR[62] = bR[61]; bR[61] = bR[60]; bR[60] = bR[59]; bR[59] = bR[58]; bR[58] = bR[57]; bR[57] = bR[56]; bR[56] = bR[55]; 
			bR[55] = bR[54]; bR[54] = bR[53]; bR[53] = bR[52]; bR[52] = bR[51]; bR[51] = bR[50]; bR[50] = bR[49]; bR[49] = bR[48]; bR[48] = bR[47]; 
			bR[47] = bR[46]; bR[46] = bR[45]; bR[45] = bR[44]; bR[44] = bR[43]; bR[43] = bR[42]; bR[42] = bR[41]; bR[41] = bR[40]; bR[40] = bR[39]; 
			bR[39] = bR[38]; bR[38] = bR[37]; bR[37] = bR[36]; bR[36] = bR[35]; bR[35] = bR[34]; bR[34] = bR[33]; bR[33] = bR[32]; bR[32] = bR[31]; 
			bR[31] = bR[30]; bR[30] = bR[29]; bR[29] = bR[28]; bR[28] = bR[27]; bR[27] = bR[26]; bR[26] = bR[25]; bR[25] = bR[24]; bR[24] = bR[23]; 
			bR[23] = bR[22]; bR[22] = bR[21]; bR[21] = bR[20]; bR[20] = bR[19]; bR[19] = bR[18]; bR[18] = bR[17]; bR[17] = bR[16]; bR[16] = bR[15]; 
			bR[15] = bR[14]; bR[14] = bR[13]; bR[13] = bR[12]; bR[12] = bR[11]; bR[11] = bR[10]; bR[10] = bR[9]; bR[9] = bR[8]; bR[8] = bR[7]; 
			bR[7] = bR[6]; bR[6] = bR[5]; bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1]; bR[1] = bR[0]; bR[0] = inputSampleR;
			inputSampleR += (bR[1] * (1.31819680801404560  + (0.00362521700518292*fabs(bR[1]))));
			inputSampleR += (bR[2] * (1.37738284126127919  + (0.14134596126256205*fabs(bR[2]))));
			inputSampleR += (bR[3] * (1.09957637225311622  + (0.33199581815501555*fabs(bR[3]))));
			inputSampleR += (bR[4] * (0.62025358899656258  + (0.37476042042088142*fabs(bR[4]))));
			inputSampleR += (bR[5] * (0.12926194402137478  + (0.24702655568406759*fabs(bR[5]))));
			inputSampleR -= (bR[6] * (0.28927985011367602  - (0.13289168298307708*fabs(bR[6]))));
			inputSampleR -= (bR[7] * (0.56518146339033448  - (0.11026641793526121*fabs(bR[7]))));
			inputSampleR -= (bR[8] * (0.59843200696815069  - (0.10139909232154271*fabs(bR[8]))));
			inputSampleR -= (bR[9] * (0.45219971861789204  - (0.13313355255903159*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.32520490032331351  - (0.29009061730364216*fabs(bR[10]))));
			inputSampleR -= (bR[11] * (0.29773131872442909  - (0.45307495356996669*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (0.31738895975218867  - (0.43198591958928922*fabs(bR[12]))));
			inputSampleR -= (bR[13] * (0.33336150604703757  - (0.24240412850274029*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.32461638442042151  - (0.02779297492397464*fabs(bR[14]))));
			inputSampleR -= (bR[15] * (0.27812829473787770  + (0.15565718905032455*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.19413454458668097  + (0.32087693535188599*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.12378036344480114  + (0.37736575956794161*fabs(bR[17]))));
			inputSampleR -= (bR[18] * (0.12550494837257106  + (0.25593811142722300*fabs(bR[18]))));
			inputSampleR -= (bR[19] * (0.17725736033713696  + (0.07708896413593636*fabs(bR[19]))));
			inputSampleR -= (bR[20] * (0.22023699647700670  - (0.01600371273599124*fabs(bR[20]))));
			inputSampleR -= (bR[21] * (0.21987645486953747  + (0.00973336938352798*fabs(bR[21]))));
			inputSampleR -= (bR[22] * (0.15014276479707978  + (0.11602269600138954*fabs(bR[22]))));
			inputSampleR -= (bR[23] * (0.05176520203073560  + (0.20383164255692698*fabs(bR[23]))));
			inputSampleR -= (bR[24] * (0.04276687165294867  + (0.17785002166834518*fabs(bR[24]))));
			inputSampleR -= (bR[25] * (0.15951546388137597  + (0.06748854885822464*fabs(bR[25]))));
			inputSampleR -= (bR[26] * (0.30211952144352616  - (0.03440494237025149*fabs(bR[26]))));
			inputSampleR -= (bR[27] * (0.36462803375134506  - (0.05874284362202409*fabs(bR[27]))));
			inputSampleR -= (bR[28] * (0.32283960219377539  + (0.01189623197958362*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.19245178663350720  + (0.11088858383712991*fabs(bR[29]))));
			inputSampleR += (bR[30] * (0.00681589563349590  - (0.16314250963457660*fabs(bR[30]))));
			inputSampleR += (bR[31] * (0.20927798345622584  - (0.16952981620487462*fabs(bR[31]))));
			inputSampleR += (bR[32] * (0.25638846543430976  - (0.11462562122281153*fabs(bR[32]))));
			inputSampleR += (bR[33] * (0.04584495673888605  + (0.04669671229804190*fabs(bR[33]))));
			inputSampleR -= (bR[34] * (0.25221561978187662  - (0.19250758741703761*fabs(bR[34]))));
			inputSampleR -= (bR[35] * (0.35662801992424953  - (0.12244680002787561*fabs(bR[35]))));
			inputSampleR -= (bR[36] * (0.21498114329314663  + (0.12152120956991189*fabs(bR[36]))));
			inputSampleR += (bR[37] * (0.00968605571673376  - (0.30597812512858558*fabs(bR[37]))));
			inputSampleR += (bR[38] * (0.18029119870614621  - (0.31569386468576782*fabs(bR[38]))));
			inputSampleR += (bR[39] * (0.22744437185251629  - (0.18028438460422197*fabs(bR[39]))));
			inputSampleR += (bR[40] * (0.09725687638959078  + (0.05479918522830433*fabs(bR[40]))));
			inputSampleR -= (bR[41] * (0.17970389267353537  - (0.29222750363124067*fabs(bR[41]))));
			inputSampleR -= (bR[42] * (0.42371969704763018  - (0.34924957781842314*fabs(bR[42]))));
			inputSampleR -= (bR[43] * (0.43313266755788055  - (0.11503731970288061*fabs(bR[43]))));
			inputSampleR -= (bR[44] * (0.22178165627851801  + (0.25002925550036226*fabs(bR[44]))));
			inputSampleR -= (bR[45] * (0.00410198176852576  + (0.43283281457037676*fabs(bR[45]))));
			inputSampleR += (bR[46] * (0.09072426344812032  - (0.35318250460706446*fabs(bR[46]))));
			inputSampleR += (bR[47] * (0.08405839183965140  - (0.16936391987143717*fabs(bR[47]))));
			inputSampleR -= (bR[48] * (0.01110419756114383  - (0.01247164991313877*fabs(bR[48]))));
			inputSampleR -= (bR[49] * (0.18593334646855278  - (0.14513260199423966*fabs(bR[49]))));
			inputSampleR -= (bR[50] * (0.33665010871497486  - (0.14456206192169668*fabs(bR[50]))));
			inputSampleR -= (bR[51] * (0.32644968491439380  + (0.01594380759082303*fabs(bR[51]))));
			inputSampleR -= (bR[52] * (0.14855437679485431  + (0.23555511219002742*fabs(bR[52]))));
			inputSampleR += (bR[53] * (0.05113019250820622  - (0.35556617126595202*fabs(bR[53]))));
			inputSampleR += (bR[54] * (0.12915754942362243  - (0.28571671825750300*fabs(bR[54]))));
			inputSampleR += (bR[55] * (0.07406865846069306  - (0.10543886479975995*fabs(bR[55]))));
			inputSampleR -= (bR[56] * (0.03669573814193980  - (0.03194267657582078*fabs(bR[56]))));
			inputSampleR -= (bR[57] * (0.13429103278009327  - (0.06145796486786051*fabs(bR[57]))));
			inputSampleR -= (bR[58] * (0.17884021749974641  - (0.00156626902982124*fabs(bR[58]))));
			inputSampleR -= (bR[59] * (0.16138212225178239  + (0.09402070836837134*fabs(bR[59]))));
			inputSampleR -= (bR[60] * (0.10867028245257521  + (0.15407963447815898*fabs(bR[60]))));
			inputSampleR -= (bR[61] * (0.06312416389213464  + (0.11241095544179526*fabs(bR[61]))));
			inputSampleR -= (bR[62] * (0.05826376574081994  - (0.03635253545701986*fabs(bR[62]))));
			inputSampleR -= (bR[63] * (0.07991631148258237  - (0.18041947557579863*fabs(bR[63]))));
			inputSampleR -= (bR[64] * (0.07777397532506500  - (0.20817156738202205*fabs(bR[64]))));
			inputSampleR -= (bR[65] * (0.03812528734394271  - (0.13679963125162486*fabs(bR[65]))));
			inputSampleR += (bR[66] * (0.00204900323943951  + (0.04009000730101046*fabs(bR[66]))));
			inputSampleR += (bR[67] * (0.01779599498119764  - (0.04218637577942354*fabs(bR[67]))));
			inputSampleR += (bR[68] * (0.00950301949319113  - (0.07908911305044238*fabs(bR[68]))));
			inputSampleR -= (bR[69] * (0.04283600714814891  + (0.02716262334097985*fabs(bR[69]))));
			inputSampleR -= (bR[70] * (0.14478320837041933  - (0.08823515277628832*fabs(bR[70]))));
			inputSampleR -= (bR[71] * (0.23250267035795688  - (0.15334197814956568*fabs(bR[71]))));
			inputSampleR -= (bR[72] * (0.22369031446225857  - (0.08550989980799503*fabs(bR[72]))));
			inputSampleR -= (bR[73] * (0.11142757883989868  + (0.08321482928259660*fabs(bR[73]))));
			inputSampleR += (bR[74] * (0.02752318631713307  - (0.25252906099212968*fabs(bR[74]))));
			inputSampleR += (bR[75] * (0.11940028414727490  - (0.34358127205009553*fabs(bR[75]))));
			inputSampleR += (bR[76] * (0.12702057126698307  - (0.31808560130583663*fabs(bR[76]))));
			inputSampleR += (bR[77] * (0.03639067777025356  - (0.17970282734717846*fabs(bR[77]))));
			inputSampleR -= (bR[78] * (0.11389848143835518  + (0.00470616711331971*fabs(bR[78]))));
			inputSampleR -= (bR[79] * (0.23024072979374310  - (0.09772245468884058*fabs(bR[79]))));
			inputSampleR -= (bR[80] * (0.24389015061112601  - (0.09600959885615798*fabs(bR[80]))));
			inputSampleR -= (bR[81] * (0.16680269075295703  - (0.05194978963662898*fabs(bR[81]))));
			inputSampleR -= (bR[82] * (0.05108041495077725  - (0.01796071525570735*fabs(bR[82]))));
			inputSampleR += (bR[83] * (0.06489835353859555  - (0.00808013770331126*fabs(bR[83]))));
			inputSampleR += (bR[84] * (0.15481511440233464  - (0.02674063848284838*fabs(bR[84]))));
			inputSampleR += (bR[85] * (0.18620867857907253  - (0.01786423699465214*fabs(bR[85]))));
			inputSampleR += (bR[86] * (0.13879832139055756  + (0.01584446839973597*fabs(bR[86]))));
			inputSampleR += (bR[87] * (0.04878235109120615  + (0.02962866516075816*fabs(bR[87]))));
			
			temp = (inputSampleR + smoothCabBR)/3.0;
			smoothCabBR = inputSampleR;
			inputSampleR = temp/4.0;
			
			
			randy = ((double(fpdR)/UINT32_MAX)*0.05);
			drySampleR = ((((inputSampleR*(1-randy))+(lastCabSampleR*randy))*wet)+(drySampleR*(1.0-wet)))*outputlevel;
			lastCabSampleR = inputSampleR;
			inputSampleR = drySampleR; //cab
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
		}
		switch (cycleEnd) //multi-pole average using lastRef[] variables
		{
			case 4:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[7])*0.5;
				lastRefL[7] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[7])*0.5;
				lastRefR[7] = lastRefR[8]; //continue, do not break
			case 3:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[6])*0.5;
				lastRefL[6] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[6])*0.5;
				lastRefR[6] = lastRefR[8]; //continue, do not break
			case 2:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[5])*0.5;
				lastRefL[5] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[5])*0.5;
				lastRefR[5] = lastRefR[8]; //continue, do not break
			case 1:
				break; //no further averaging
		}
		
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
