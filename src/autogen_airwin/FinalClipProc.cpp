/* ========================================
 *  FinalClip - FinalClip.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __FinalClip_H
#include "FinalClip.h"
#endif
namespace airwinconsolidated::FinalClip {

void FinalClip::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	//double hardness = 0.7390851332151606; // x == cos(x)
	//double softness = 0.260914866784839; // 1.0 - hardness
	//double refclip = 0.9549925859; // -0.2dB we're making all this pure raw code
	//refclip*hardness = 0.705820822569392  to use ClipOnly as a prefab code-chunk.
	//refclip*softness = 0.249171763330607	Seven decimal places is plenty as it's
	//not related to the original sound much: it's an arbitrary position in softening.
	//This is where the numbers come from, referencing the code of the original ClipOnly
	
	//double hardness = 0.618033988749894; // golden ratio
	//double softness = 0.381966011250105; // 1.0 - hardness
	//double refclip = 1.618033988749894; // -0.2dB we're making all this pure raw code
	//refclip*hardness = 1.0  to use ClipOnly as a prefab code-chunk.
	//refclip*softness = 0.618033988749894	Seven decimal places is plenty as it's
	//not related to the original sound much: it's an arbitrary position in softening.
	
    while (--sampleFrames >= 0)
    {
		float inputSampleL = *in1;
		float inputSampleR = *in2;
		//		if (fabs(inputSampleL)<1.18e-37) inputSampleL = fpdL * 1.18e-37;
		//		if (fabs(inputSampleR)<1.18e-37) inputSampleR = fpdR * 1.18e-37;
		//do NOT use on ClipOnly, it is almost always a pure bypass
		
		if (inputSampleL > 4.0) inputSampleL = 4.0; if (inputSampleL < -4.0) inputSampleL = -4.0;
		if (inputSampleL - lastSampleL > 0.618033988749894) inputSampleL = lastSampleL + 0.618033988749894;
		if (inputSampleL - lastSampleL < -0.618033988749894) inputSampleL = lastSampleL - 0.618033988749894;
		//same as slew clippage
		
		//begin ClipOnly2 stereo as a little, compressed chunk that can be dropped into code
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=1.0+(inputSampleL*0.381966011250105);
			else lastSampleL = 0.618033988749894+(lastSampleL*0.618033988749894);
		} wasPosClipL = false;
		if (inputSampleL>1.618033988749894) {wasPosClipL=true;inputSampleL=1.0+(lastSampleL*0.381966011250105);}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=-1.0+(inputSampleL*0.381966011250105);
			else lastSampleL=-0.618033988749894+(lastSampleL*0.618033988749894);
		} wasNegClipL = false;
		if (inputSampleL<-1.618033988749894) {wasNegClipL=true;inputSampleL=-1.0+(lastSampleL*0.381966011250105);}
		intermediateL[spacing] = inputSampleL;
        inputSampleL = lastSampleL; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateL[x-1] = intermediateL[x];
		lastSampleL = intermediateL[0]; //run a little buffer to handle this
		
		if (inputSampleR > 4.0) inputSampleR = 4.0; if (inputSampleR < -4.0) inputSampleR = -4.0;
		if (inputSampleR - lastSampleR > 0.618033988749894) inputSampleR = lastSampleR + 0.618033988749894;
		if (inputSampleR - lastSampleR < -0.618033988749894) inputSampleR = lastSampleR - 0.618033988749894;
		//same as slew clippage

		if (wasPosClipR == true) { //current will be over
			if (inputSampleR<lastSampleR) lastSampleR=1.0+(inputSampleR*0.381966011250105);
			else lastSampleR = 0.618033988749894+(lastSampleR*0.618033988749894);
		} wasPosClipR = false;
		if (inputSampleR>1.618033988749894) {wasPosClipR=true;inputSampleR=1.0+(lastSampleR*0.381966011250105);}
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) lastSampleR=-1.0+(inputSampleR*0.381966011250105);
			else lastSampleR=-0.618033988749894+(lastSampleR*0.618033988749894);
		} wasNegClipR = false;
		if (inputSampleR<-1.618033988749894) {wasNegClipR=true;inputSampleR=-1.0+(lastSampleR*0.381966011250105);}
		intermediateR[spacing] = inputSampleR;
        inputSampleR = lastSampleR; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateR[x-1] = intermediateR[x];
		lastSampleR = intermediateR[0]; //run a little buffer to handle this
		//end ClipOnly2 stereo as a little, compressed chunk that can be dropped into code
		
		//begin 32 bit stereo floating point dither
		//		int expon; frexpf((float)inputSampleL, &expon);
		//		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//		frexpf((float)inputSampleR, &expon);
		//		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		//do NOT use on ClipOnly, it is almost always a pure bypass
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;
		
		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void FinalClip::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	//double hardness = 0.7390851332151606; // x == cos(x)
	//double softness = 0.260914866784839; // 1.0 - hardness
	//double refclip = 0.9549925859; // -0.2dB we're making all this pure raw code
	//refclip*hardness = 0.705820822569392  to use ClipOnly as a prefab code-chunk.
	//refclip*softness = 0.249171763330607	Seven decimal places is plenty as it's
	//not related to the original sound much: it's an arbitrary position in softening.
	//This is where the numbers come from, referencing the code of the original ClipOnly
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		//		if (fabs(inputSampleL)<1.18e-43) inputSampleL = fpdL * 1.18e-43;
		//		if (fabs(inputSampleR)<1.18e-43) inputSampleR = fpdR * 1.18e-43;
		//do NOT use on ClipOnly, it is almost always a pure bypass
		
		if (inputSampleL > 4.0) inputSampleL = 4.0; if (inputSampleL < -4.0) inputSampleL = -4.0;
		if (inputSampleL - lastSampleL > 0.618033988749894) inputSampleL = lastSampleL + 0.618033988749894;
		if (inputSampleL - lastSampleL < -0.618033988749894) inputSampleL = lastSampleL - 0.618033988749894;
		//same as slew clippage
		
		//begin ClipOnly2 stereo as a little, compressed chunk that can be dropped into code
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=1.0+(inputSampleL*0.381966011250105);
			else lastSampleL = 0.618033988749894+(lastSampleL*0.618033988749894);
		} wasPosClipL = false;
		if (inputSampleL>1.618033988749894) {wasPosClipL=true;inputSampleL=1.0+(lastSampleL*0.381966011250105);}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=-1.0+(inputSampleL*0.381966011250105);
			else lastSampleL=-0.618033988749894+(lastSampleL*0.618033988749894);
		} wasNegClipL = false;
		if (inputSampleL<-1.618033988749894) {wasNegClipL=true;inputSampleL=-1.0+(lastSampleL*0.381966011250105);}
		intermediateL[spacing] = inputSampleL;
        inputSampleL = lastSampleL; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateL[x-1] = intermediateL[x];
		lastSampleL = intermediateL[0]; //run a little buffer to handle this
		
		if (inputSampleR > 4.0) inputSampleR = 4.0; if (inputSampleR < -4.0) inputSampleR = -4.0;
		if (inputSampleR - lastSampleR > 0.618033988749894) inputSampleR = lastSampleR + 0.618033988749894;
		if (inputSampleR - lastSampleR < -0.618033988749894) inputSampleR = lastSampleR - 0.618033988749894;
		//same as slew clippage
		
		if (wasPosClipR == true) { //current will be over
			if (inputSampleR<lastSampleR) lastSampleR=1.0+(inputSampleR*0.381966011250105);
			else lastSampleR = 0.618033988749894+(lastSampleR*0.618033988749894);
		} wasPosClipR = false;
		if (inputSampleR>1.618033988749894) {wasPosClipR=true;inputSampleR=1.0+(lastSampleR*0.381966011250105);}
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) lastSampleR=-1.0+(inputSampleR*0.381966011250105);
			else lastSampleR=-0.618033988749894+(lastSampleR*0.618033988749894);
		} wasNegClipR = false;
		if (inputSampleR<-1.618033988749894) {wasNegClipR=true;inputSampleR=-1.0+(lastSampleR*0.381966011250105);}
		intermediateR[spacing] = inputSampleR;
        inputSampleR = lastSampleR; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateR[x-1] = intermediateR[x];
		lastSampleR = intermediateR[0]; //run a little buffer to handle this
		//end ClipOnly2 stereo as a little, compressed chunk that can be dropped into code
		
		//begin 64 bit stereo floating point dither
		//		int expon; frexp((double)inputSampleL, &expon);
		//		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//		frexp((double)inputSampleR, &expon);
		//		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		//do NOT use on ClipOnly, it is almost always a pure bypass
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
} // end namespace
