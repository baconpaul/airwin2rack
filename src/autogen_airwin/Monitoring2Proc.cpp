/* ========================================
 *  Monitoring2 - Monitoring2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Monitoring2_H
#include "Monitoring2.h"
#endif
namespace airwinconsolidated::Monitoring2 {

void Monitoring2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int depth = (int)(17.0*overallscale);
	if (depth < 3) depth = 3;
	if (depth > 98) depth = 98; //for Dark	
	
	int processing = (VstInt32)( A * 16.999 );
	int am = (int)149.0 * overallscale;
	int bm = (int)179.0 * overallscale;
	int cm = (int)191.0 * overallscale;
	int dm = (int)223.0 * overallscale; //these are 'good' primes, spacing out the allpasses
	int allpasstemp;
	//for PeaksOnly
	biquad[fix_freq] = 0.0375/overallscale; biquad[fix_reso] = 0.1575; //define as AURAT, MONORAT, MONOLAT unless overridden
	if (processing == kVINYL) {biquad[fix_freq] = 0.0385/overallscale; biquad[fix_reso] = 0.0825;}
	if (processing == kPHONE) {biquad[fix_freq] = 0.1245/overallscale; biquad[fix_reso] = 0.46;}
	double K = tan(M_PI * biquad[fix_freq]);
	double norm = 1.0 / (1.0 + K / biquad[fix_reso] + K * K);
	biquad[fix_a0] = K / biquad[fix_reso] * norm;
	biquad[fix_a2] = -biquad[fix_a0]; //for bandpass, ignore [fix_a1] = 0.0
	biquad[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	biquad[fix_b2] = (1.0 - K / biquad[fix_reso] + K * K) * norm;
	//for Bandpasses
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//we need to make our dither run up here, there's no spot on the end to do it
		
		switch (processing)
		{
			case kDKAD:
			case kDKCD:
				break;
			case kPEAK:
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//amplitude aspect
				allpasstemp = ax - 1; if (allpasstemp < 0 || allpasstemp > am) allpasstemp = am;
				inputSampleL -= aL[allpasstemp]*0.5; aL[ax] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= aR[allpasstemp]*0.5; aR[ax] = inputSampleR; inputSampleR *= 0.5;
				ax--; if (ax < 0 || ax > am) {ax = am;}
				inputSampleL += (aL[ax]);
				inputSampleR += (aR[ax]);
				//a single Midiverb-style allpass
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//amplitude aspect
				
				allpasstemp = bx - 1; if (allpasstemp < 0 || allpasstemp > bm) allpasstemp = bm;
				inputSampleL -= bL[allpasstemp]*0.5; bL[bx] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= bR[allpasstemp]*0.5; bR[bx] = inputSampleR; inputSampleR *= 0.5;
				bx--; if (bx < 0 || bx > bm) {bx = bm;}
				inputSampleL += (bL[bx]);
				inputSampleR += (bR[bx]);
				//a single Midiverb-style allpass
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//amplitude aspect
				
				allpasstemp = cx - 1; if (allpasstemp < 0 || allpasstemp > cm) allpasstemp = cm;
				inputSampleL -= cL[allpasstemp]*0.5; cL[cx] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= cR[allpasstemp]*0.5; cR[cx] = inputSampleR; inputSampleR *= 0.5;
				cx--; if (cx < 0 || cx > cm) {cx = cm;}
				inputSampleL += (cL[cx]);
				inputSampleR += (cR[cx]);
				//a single Midiverb-style allpass
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//amplitude aspect
				
				allpasstemp = dx - 1; if (allpasstemp < 0 || allpasstemp > dm) allpasstemp = dm;
				inputSampleL -= dL[allpasstemp]*0.5; dL[dx] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= dR[allpasstemp]*0.5; dR[dx] = inputSampleR; inputSampleR *= 0.5;
				dx--; if (dx < 0 || dx > dm) {dx = dm;}
				inputSampleL += (dL[dx]);
				inputSampleR += (dR[dx]);
				//a single Midiverb-style allpass
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//amplitude aspect
				
				inputSampleL *= 0.63679; inputSampleR *= 0.63679; //scale it to 0dB output at full blast
				//PeaksOnly
				break;
			case kSLEW:
				double trim;
				trim = 2.302585092994045684017991; //natural logarithm of 10
				double slewSample; slewSample = (inputSampleL - lastSampleL)*trim;
				lastSampleL = inputSampleL;
				if (slewSample > 1.0) slewSample = 1.0; if (slewSample < -1.0) slewSample = -1.0;
				inputSampleL = slewSample;
				slewSample = (inputSampleR - lastSampleR)*trim;
				lastSampleR = inputSampleR;
				if (slewSample > 1.0) slewSample = 1.0; if (slewSample < -1.0) slewSample = -1.0;
				inputSampleR = slewSample;
				//SlewOnly
				break;
			case kSUBS:
				double iirAmount; iirAmount = (2250/44100.0) / overallscale;
				double gain; gain = 1.42;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				
				iirSampleAL = (iirSampleAL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleAL;
				iirSampleAR = (iirSampleAR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleAR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleBL = (iirSampleBL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleBL;
				iirSampleBR = (iirSampleBR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleBR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleCL = (iirSampleCL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleCL;
				iirSampleCR = (iirSampleCR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleCR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleDL = (iirSampleDL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleDL;
				iirSampleDR = (iirSampleDR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleDR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleEL = (iirSampleEL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleEL;
				iirSampleER = (iirSampleER * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleER;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleFL = (iirSampleFL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleFL;
				iirSampleFR = (iirSampleFR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleFR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleGL = (iirSampleGL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleGL;
				iirSampleGR = (iirSampleGR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleGR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleHL = (iirSampleHL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleHL;
				iirSampleHR = (iirSampleHR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleHR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleIL = (iirSampleIL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleIL;
				iirSampleIR = (iirSampleIR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleIR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleJL = (iirSampleJL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleJL;
				iirSampleJR = (iirSampleJR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleJR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleKL = (iirSampleKL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleKL;
				iirSampleKR = (iirSampleKR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleKR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleLL = (iirSampleLL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleLL;
				iirSampleLR = (iirSampleLR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleLR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleML = (iirSampleML * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleML;
				iirSampleMR = (iirSampleMR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleMR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleNL = (iirSampleNL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleNL;
				iirSampleNR = (iirSampleNR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleNR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleOL = (iirSampleOL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleOL;
				iirSampleOR = (iirSampleOR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleOR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSamplePL = (iirSamplePL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSamplePL;
				iirSamplePR = (iirSamplePR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSamplePR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleQL = (iirSampleQL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleQL;
				iirSampleQR = (iirSampleQR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleQR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleRL = (iirSampleRL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleRL;
				iirSampleRR = (iirSampleRR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleRR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleSL = (iirSampleSL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleSL;
				iirSampleSR = (iirSampleSR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleSR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleTL = (iirSampleTL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleTL;
				iirSampleTR = (iirSampleTR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleTR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleUL = (iirSampleUL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleUL;
				iirSampleUR = (iirSampleUR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleUR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleVL = (iirSampleVL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleVL;
				iirSampleVR = (iirSampleVR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleVR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleWL = (iirSampleWL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleWL;
				iirSampleWR = (iirSampleWR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleWR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleXL = (iirSampleXL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleXL;
				iirSampleXR = (iirSampleXR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleXR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleYL = (iirSampleYL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleYL;
				iirSampleYR = (iirSampleYR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleYR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleZL = (iirSampleZL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleZL;
				iirSampleZR = (iirSampleZR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleZR;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;				
				//SubsOnly
				break;
			case kMONO:
			case kSIDE:
				double mid; mid = inputSampleL + inputSampleR;
				double side; side = inputSampleL - inputSampleR;
				if (processing < kSIDE) side = 0.0;
				else mid = 0.0; //mono monitoring, or side-only monitoring
				inputSampleL = (mid+side)/2.0;
				inputSampleR = (mid-side)/2.0; 
				break;
			case kVINYL:
			case kAURAT:
			case kMONORAT:
			case kMONOLAT:
			case kPHONE:
				//Bandpass: changes in EQ are up in the variable defining, not here
				//7 Vinyl, 8 9 10 Aurat, 11 Phone
				if (processing == kMONORAT) {inputSampleR = (inputSampleL + inputSampleR)*0.5;inputSampleL = 0.0;}
				if (processing == kMONOLAT) {inputSampleL = (inputSampleL + inputSampleR)*0.5;inputSampleR = 0.0;}
				if (processing == kPHONE) {double M; M = (inputSampleL + inputSampleR)*0.5; inputSampleL = M;inputSampleR = M;}
				
				inputSampleL = sin(inputSampleL); inputSampleR = sin(inputSampleR);
				//encode Console5: good cleanness
				
				double tempSampleL; tempSampleL = (inputSampleL * biquad[fix_a0]) + biquad[fix_sL1];
				biquad[fix_sL1] = (-tempSampleL * biquad[fix_b1]) + biquad[fix_sL2];
				biquad[fix_sL2] = (inputSampleL * biquad[fix_a2]) - (tempSampleL * biquad[fix_b2]);
				inputSampleL = tempSampleL; //like mono AU, 7 and 8 store L channel
				
				double tempSampleR; tempSampleR = (inputSampleR * biquad[fix_a0]) + biquad[fix_sR1];
				biquad[fix_sR1] = (-tempSampleR * biquad[fix_b1]) + biquad[fix_sR2];
				biquad[fix_sR2] = (inputSampleR * biquad[fix_a2]) - (tempSampleR * biquad[fix_b2]);
				inputSampleR = tempSampleR; //note: 9 and 10 store the R channel
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				//without this, you can get a NaN condition where it spits out DC offset at full blast!
				inputSampleL = asin(inputSampleL); inputSampleR = asin(inputSampleR);
				//amplitude aspect
				break;
			case kCANSA:
			case kCANSB:
			case kCANSC:
			case kCANSD:
				if (processing == kCANSA) {inputSampleL *= 0.855; inputSampleR *= 0.855;}
				if (processing == kCANSB) {inputSampleL *= 0.748; inputSampleR *= 0.748;}
				if (processing == kCANSC) {inputSampleL *= 0.713; inputSampleR *= 0.713;}
				if (processing == kCANSD) {inputSampleL *= 0.680; inputSampleR *= 0.680;}
				//we do a volume compensation immediately to gain stage stuff cleanly
				inputSampleL = sin(inputSampleL);
				inputSampleR = sin(inputSampleR);
				double drySampleL; drySampleL = inputSampleL;
				double drySampleR; drySampleR = inputSampleR; //everything runs 'inside' Console
				double bass; bass = (processing * processing * 0.00001) / overallscale;
				//we are using the iir filters from out of SubsOnly
				
				mid = inputSampleL + inputSampleR; side = inputSampleL - inputSampleR;
				iirSampleAL = (iirSampleAL * (1.0 - (bass*0.618))) + (side * bass * 0.618); side = side - iirSampleAL;
				inputSampleL = (mid+side)/2.0; inputSampleR = (mid-side)/2.0;
				//bass narrowing filter				
				
				allpasstemp = ax - 1; if (allpasstemp < 0 || allpasstemp > am) allpasstemp = am;
				inputSampleL -= aL[allpasstemp]*0.5; aL[ax] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= aR[allpasstemp]*0.5; aR[ax] = inputSampleR; inputSampleR *= 0.5;
				
				ax--; if (ax < 0 || ax > am) {ax = am;}
				inputSampleL += (aL[ax])*0.5; inputSampleR += (aR[ax])*0.5;
				if (ax == am) {inputSampleL += (aL[0])*0.5; inputSampleR += (aR[0])*0.5;}
				else {inputSampleL += (aL[ax+1])*0.5; inputSampleR += (aR[ax+1])*0.5;}
				//a darkened Midiverb-style allpass
				
				if (processing == kCANSA) {inputSampleL *= 0.125; inputSampleR *= 0.125;}
				if (processing == kCANSB) {inputSampleL *= 0.25; inputSampleR *= 0.25;}
				if (processing == kCANSC) {inputSampleL *= 0.30; inputSampleR *= 0.30;}
				if (processing == kCANSD) {inputSampleL *= 0.35; inputSampleR *= 0.35;}
				//Cans A suppresses the crossfeed more, Cans B makes it louder
				
				drySampleL += inputSampleR;
				drySampleR += inputSampleL; //the crossfeed
				
				allpasstemp = dx - 1; if (allpasstemp < 0 || allpasstemp > dm) allpasstemp = dm;
				inputSampleL -= dL[allpasstemp]*0.5; dL[dx] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= dR[allpasstemp]*0.5; dR[dx] = inputSampleR; inputSampleR *= 0.5;
				
				dx--; if (dx < 0 || dx > dm) {dx = dm;}
				inputSampleL += (dL[dx])*0.5; inputSampleR += (dR[dx])*0.5;
				if (dx == dm) {inputSampleL += (dL[0])*0.5; inputSampleR += (dR[0])*0.5;}
				else {inputSampleL += (dL[dx+1])*0.5; inputSampleR += (dR[dx+1])*0.5;}
				//a darkened Midiverb-style allpass, which is stretching the previous one even more
				
				inputSampleL *= 0.25; inputSampleR *= 0.25;
				//for all versions of Cans the second level of bloom is this far down
				//and, remains on the opposite speaker rather than crossing again to the original side
				
				drySampleL += inputSampleR;
				drySampleR += inputSampleL; //add the crossfeed and very faint extra verbyness
				
				inputSampleL = drySampleL;
				inputSampleR = drySampleR; //and output our can-opened headphone feed
				
				mid = inputSampleL + inputSampleR; side = inputSampleL - inputSampleR;
				iirSampleAR = (iirSampleAR * (1.0 - bass)) + (side * bass); side = side - iirSampleAR;
				inputSampleL = (mid+side)/2.0; inputSampleR = (mid-side)/2.0;
				//bass narrowing filter
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//ConsoleBuss processing
				break;
			case kTRICK:
				double inputSample = (inputSampleL + inputSampleR) * 0.5;
				inputSampleL = -inputSample;
				inputSampleR = inputSample;
				break;
		}
		
		//begin Dark		
		if (processing == kDKCD) {
			inputSampleL *= 32768.0; //or 16 bit option
			inputSampleR *= 32768.0; //or 16 bit option
		} else {
			inputSampleL *= 8388608.0; //for literally everything else
			inputSampleR *= 8388608.0; //we will apply the 24 bit Dark
		} //on the not unreasonable assumption that we are very likely playing back on 24 bit DAC
		
		//We are doing it first Left, then Right, because the loops may run faster if
		//they aren't too jammed full of variables. This means re-running code.
		
		//begin left
		int quantA = floor(inputSampleL);
		int quantB = floor(inputSampleL+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		float expectedSlew = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlew += (darkSampleL[x+1] - darkSampleL[x]);
		}
		expectedSlew /= depth; //we have an average of all recent slews
		//we are doing that to voice the thing down into the upper mids a bit
		//it mustn't just soften the brightest treble, it must smooth high mids too
		
		float testA = fabs((darkSampleL[0] - quantA) - expectedSlew);
		float testB = fabs((darkSampleL[0] - quantB) - expectedSlew);
		
		if (testA < testB) inputSampleL = quantA;
		else inputSampleL = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			darkSampleL[x+1] = darkSampleL[x];
		}
		darkSampleL[0] = inputSampleL;
		//end Dark left
		
		//begin right
		quantA = floor(inputSampleR);
		quantB = floor(inputSampleR+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		expectedSlew = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlew += (darkSampleR[x+1] - darkSampleR[x]);
		}
		expectedSlew /= depth; //we have an average of all recent slews
		//we are doing that to voice the thing down into the upper mids a bit
		//it mustn't just soften the brightest treble, it must smooth high mids too
		
		testA = fabs((darkSampleR[0] - quantA) - expectedSlew);
		testB = fabs((darkSampleR[0] - quantB) - expectedSlew);
		
		if (testA < testB) inputSampleR = quantA;
		else inputSampleR = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			darkSampleR[x+1] = darkSampleR[x];
		}
		darkSampleR[0] = inputSampleR;
		//end Dark right
		
		if (processing == kDKCD) {
			inputSampleL /= 32768.0;
			inputSampleR /= 32768.0;
		} else {
			inputSampleL /= 8388608.0;
			inputSampleR /= 8388608.0;
		}
		//does not use 32 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void Monitoring2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int depth = (int)(17.0*overallscale);
	if (depth < 3) depth = 3;
	if (depth > 98) depth = 98; //for Dark	
	
	int processing = (VstInt32)( A * 16.999 );
	int am = (int)149.0 * overallscale;
	int bm = (int)179.0 * overallscale;
	int cm = (int)191.0 * overallscale;
	int dm = (int)223.0 * overallscale; //these are 'good' primes, spacing out the allpasses
	int allpasstemp;
	//for PeaksOnly
	biquad[fix_freq] = 0.0375/overallscale; biquad[fix_reso] = 0.1575; //define as AURAT, MONORAT, MONOLAT unless overridden
	if (processing == kVINYL) {biquad[fix_freq] = 0.0385/overallscale; biquad[fix_reso] = 0.0825;}
	if (processing == kPHONE) {biquad[fix_freq] = 0.1245/overallscale; biquad[fix_reso] = 0.46;}
	double K = tan(M_PI * biquad[fix_freq]);
	double norm = 1.0 / (1.0 + K / biquad[fix_reso] + K * K);
	biquad[fix_a0] = K / biquad[fix_reso] * norm;
	biquad[fix_a2] = -biquad[fix_a0]; //for bandpass, ignore [fix_a1] = 0.0
	biquad[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	biquad[fix_b2] = (1.0 - K / biquad[fix_reso] + K * K) * norm;
	//for Bandpasses
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//we need to make our dither run up here, there's no spot on the end to do it
		
		switch (processing)
		{
			case kDKAD:
			case kDKCD:
				break;
			case kPEAK:
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//amplitude aspect
				allpasstemp = ax - 1; if (allpasstemp < 0 || allpasstemp > am) allpasstemp = am;
				inputSampleL -= aL[allpasstemp]*0.5; aL[ax] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= aR[allpasstemp]*0.5; aR[ax] = inputSampleR; inputSampleR *= 0.5;
				ax--; if (ax < 0 || ax > am) {ax = am;}
				inputSampleL += (aL[ax]);
				inputSampleR += (aR[ax]);
				//a single Midiverb-style allpass
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//amplitude aspect
				
				allpasstemp = bx - 1; if (allpasstemp < 0 || allpasstemp > bm) allpasstemp = bm;
				inputSampleL -= bL[allpasstemp]*0.5; bL[bx] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= bR[allpasstemp]*0.5; bR[bx] = inputSampleR; inputSampleR *= 0.5;
				bx--; if (bx < 0 || bx > bm) {bx = bm;}
				inputSampleL += (bL[bx]);
				inputSampleR += (bR[bx]);
				//a single Midiverb-style allpass
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//amplitude aspect
				
				allpasstemp = cx - 1; if (allpasstemp < 0 || allpasstemp > cm) allpasstemp = cm;
				inputSampleL -= cL[allpasstemp]*0.5; cL[cx] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= cR[allpasstemp]*0.5; cR[cx] = inputSampleR; inputSampleR *= 0.5;
				cx--; if (cx < 0 || cx > cm) {cx = cm;}
				inputSampleL += (cL[cx]);
				inputSampleR += (cR[cx]);
				//a single Midiverb-style allpass
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//amplitude aspect
				
				allpasstemp = dx - 1; if (allpasstemp < 0 || allpasstemp > dm) allpasstemp = dm;
				inputSampleL -= dL[allpasstemp]*0.5; dL[dx] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= dR[allpasstemp]*0.5; dR[dx] = inputSampleR; inputSampleR *= 0.5;
				dx--; if (dx < 0 || dx > dm) {dx = dm;}
				inputSampleL += (dL[dx]);
				inputSampleR += (dR[dx]);
				//a single Midiverb-style allpass
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//amplitude aspect
				
				inputSampleL *= 0.63679; inputSampleR *= 0.63679; //scale it to 0dB output at full blast
				//PeaksOnly
				break;
			case kSLEW:
				double trim;
				trim = 2.302585092994045684017991; //natural logarithm of 10
				double slewSample; slewSample = (inputSampleL - lastSampleL)*trim;
				lastSampleL = inputSampleL;
				if (slewSample > 1.0) slewSample = 1.0; if (slewSample < -1.0) slewSample = -1.0;
				inputSampleL = slewSample;
				slewSample = (inputSampleR - lastSampleR)*trim;
				lastSampleR = inputSampleR;
				if (slewSample > 1.0) slewSample = 1.0; if (slewSample < -1.0) slewSample = -1.0;
				inputSampleR = slewSample;
				//SlewOnly
				break;
			case kSUBS:
				double iirAmount; iirAmount = (2250/44100.0) / overallscale;
				double gain; gain = 1.42;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				
				iirSampleAL = (iirSampleAL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleAL;
				iirSampleAR = (iirSampleAR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleAR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleBL = (iirSampleBL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleBL;
				iirSampleBR = (iirSampleBR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleBR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleCL = (iirSampleCL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleCL;
				iirSampleCR = (iirSampleCR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleCR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleDL = (iirSampleDL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleDL;
				iirSampleDR = (iirSampleDR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleDR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleEL = (iirSampleEL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleEL;
				iirSampleER = (iirSampleER * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleER;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleFL = (iirSampleFL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleFL;
				iirSampleFR = (iirSampleFR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleFR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleGL = (iirSampleGL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleGL;
				iirSampleGR = (iirSampleGR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleGR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleHL = (iirSampleHL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleHL;
				iirSampleHR = (iirSampleHR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleHR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleIL = (iirSampleIL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleIL;
				iirSampleIR = (iirSampleIR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleIR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleJL = (iirSampleJL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleJL;
				iirSampleJR = (iirSampleJR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleJR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleKL = (iirSampleKL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleKL;
				iirSampleKR = (iirSampleKR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleKR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleLL = (iirSampleLL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleLL;
				iirSampleLR = (iirSampleLR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleLR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleML = (iirSampleML * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleML;
				iirSampleMR = (iirSampleMR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleMR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleNL = (iirSampleNL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleNL;
				iirSampleNR = (iirSampleNR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleNR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleOL = (iirSampleOL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleOL;
				iirSampleOR = (iirSampleOR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleOR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSamplePL = (iirSamplePL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSamplePL;
				iirSamplePR = (iirSamplePR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSamplePR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleQL = (iirSampleQL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleQL;
				iirSampleQR = (iirSampleQR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleQR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleRL = (iirSampleRL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleRL;
				iirSampleRR = (iirSampleRR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleRR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleSL = (iirSampleSL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleSL;
				iirSampleSR = (iirSampleSR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleSR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleTL = (iirSampleTL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleTL;
				iirSampleTR = (iirSampleTR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleTR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleUL = (iirSampleUL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleUL;
				iirSampleUR = (iirSampleUR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleUR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleVL = (iirSampleVL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleVL;
				iirSampleVR = (iirSampleVR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleVR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleWL = (iirSampleWL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleWL;
				iirSampleWR = (iirSampleWR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleWR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleXL = (iirSampleXL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleXL;
				iirSampleXR = (iirSampleXR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleXR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleYL = (iirSampleYL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleYL;
				iirSampleYR = (iirSampleYR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleYR;
				inputSampleL *= gain; inputSampleR *= gain; gain = ((gain-1)*0.75)+1;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				
				iirSampleZL = (iirSampleZL * (1.0-iirAmount)) + (inputSampleL * iirAmount); inputSampleL = iirSampleZL;
				iirSampleZR = (iirSampleZR * (1.0-iirAmount)) + (inputSampleR * iirAmount); inputSampleR = iirSampleZR;
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;				
				//SubsOnly
				break;
			case kMONO:
			case kSIDE:
				double mid; mid = inputSampleL + inputSampleR;
				double side; side = inputSampleL - inputSampleR;
				if (processing < kSIDE) side = 0.0;
				else mid = 0.0; //mono monitoring, or side-only monitoring
				inputSampleL = (mid+side)/2.0;
				inputSampleR = (mid-side)/2.0; 
				break;
			case kVINYL:
			case kAURAT:
			case kMONORAT:
			case kMONOLAT:
			case kPHONE:
				//Bandpass: changes in EQ are up in the variable defining, not here
				//7 Vinyl, 8 9 10 Aurat, 11 Phone
				if (processing == kMONORAT) {inputSampleR = (inputSampleL + inputSampleR)*0.5;inputSampleL = 0.0;}
				if (processing == kMONOLAT) {inputSampleL = (inputSampleL + inputSampleR)*0.5;inputSampleR = 0.0;}
				if (processing == kPHONE) {double M; M = (inputSampleL + inputSampleR)*0.5; inputSampleL = M;inputSampleR = M;}
				
				inputSampleL = sin(inputSampleL); inputSampleR = sin(inputSampleR);
				//encode Console5: good cleanness
				
				double tempSampleL; tempSampleL = (inputSampleL * biquad[fix_a0]) + biquad[fix_sL1];
				biquad[fix_sL1] = (-tempSampleL * biquad[fix_b1]) + biquad[fix_sL2];
				biquad[fix_sL2] = (inputSampleL * biquad[fix_a2]) - (tempSampleL * biquad[fix_b2]);
				inputSampleL = tempSampleL; //like mono AU, 7 and 8 store L channel
				
				double tempSampleR; tempSampleR = (inputSampleR * biquad[fix_a0]) + biquad[fix_sR1];
				biquad[fix_sR1] = (-tempSampleR * biquad[fix_b1]) + biquad[fix_sR2];
				biquad[fix_sR2] = (inputSampleR * biquad[fix_a2]) - (tempSampleR * biquad[fix_b2]);
				inputSampleR = tempSampleR; //note: 9 and 10 store the R channel
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
				//without this, you can get a NaN condition where it spits out DC offset at full blast!
				inputSampleL = asin(inputSampleL); inputSampleR = asin(inputSampleR);
				//amplitude aspect
				break;
			case kCANSA:
			case kCANSB:
			case kCANSC:
			case kCANSD:
				if (processing == kCANSA) {inputSampleL *= 0.855; inputSampleR *= 0.855;}
				if (processing == kCANSB) {inputSampleL *= 0.748; inputSampleR *= 0.748;}
				if (processing == kCANSC) {inputSampleL *= 0.713; inputSampleR *= 0.713;}
				if (processing == kCANSD) {inputSampleL *= 0.680; inputSampleR *= 0.680;}
				//we do a volume compensation immediately to gain stage stuff cleanly
				inputSampleL = sin(inputSampleL);
				inputSampleR = sin(inputSampleR);
				double drySampleL; drySampleL = inputSampleL;
				double drySampleR; drySampleR = inputSampleR; //everything runs 'inside' Console
				double bass; bass = (processing * processing * 0.00001) / overallscale;
				//we are using the iir filters from out of SubsOnly
				
				mid = inputSampleL + inputSampleR; side = inputSampleL - inputSampleR;
				iirSampleAL = (iirSampleAL * (1.0 - (bass*0.618))) + (side * bass * 0.618); side = side - iirSampleAL;
				inputSampleL = (mid+side)/2.0; inputSampleR = (mid-side)/2.0;
				//bass narrowing filter				
				
				allpasstemp = ax - 1; if (allpasstemp < 0 || allpasstemp > am) allpasstemp = am;
				inputSampleL -= aL[allpasstemp]*0.5; aL[ax] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= aR[allpasstemp]*0.5; aR[ax] = inputSampleR; inputSampleR *= 0.5;
				
				ax--; if (ax < 0 || ax > am) {ax = am;}
				inputSampleL += (aL[ax])*0.5; inputSampleR += (aR[ax])*0.5;
				if (ax == am) {inputSampleL += (aL[0])*0.5; inputSampleR += (aR[0])*0.5;}
				else {inputSampleL += (aL[ax+1])*0.5; inputSampleR += (aR[ax+1])*0.5;}
				//a darkened Midiverb-style allpass
				
				if (processing == kCANSA) {inputSampleL *= 0.125; inputSampleR *= 0.125;}
				if (processing == kCANSB) {inputSampleL *= 0.25; inputSampleR *= 0.25;}
				if (processing == kCANSC) {inputSampleL *= 0.30; inputSampleR *= 0.30;}
				if (processing == kCANSD) {inputSampleL *= 0.35; inputSampleR *= 0.35;}
				//Cans A suppresses the crossfeed more, Cans B makes it louder
				
				drySampleL += inputSampleR;
				drySampleR += inputSampleL; //the crossfeed
				
				allpasstemp = dx - 1; if (allpasstemp < 0 || allpasstemp > dm) allpasstemp = dm;
				inputSampleL -= dL[allpasstemp]*0.5; dL[dx] = inputSampleL; inputSampleL *= 0.5;
				inputSampleR -= dR[allpasstemp]*0.5; dR[dx] = inputSampleR; inputSampleR *= 0.5;
				
				dx--; if (dx < 0 || dx > dm) {dx = dm;}
				inputSampleL += (dL[dx])*0.5; inputSampleR += (dR[dx])*0.5;
				if (dx == dm) {inputSampleL += (dL[0])*0.5; inputSampleR += (dR[0])*0.5;}
				else {inputSampleL += (dL[dx+1])*0.5; inputSampleR += (dR[dx+1])*0.5;}
				//a darkened Midiverb-style allpass, which is stretching the previous one even more
				
				inputSampleL *= 0.25; inputSampleR *= 0.25;
				//for all versions of Cans the second level of bloom is this far down
				//and, remains on the opposite speaker rather than crossing again to the original side
				
				drySampleL += inputSampleR;
				drySampleR += inputSampleL; //add the crossfeed and very faint extra verbyness
				
				inputSampleL = drySampleL;
				inputSampleR = drySampleR; //and output our can-opened headphone feed
				
				mid = inputSampleL + inputSampleR; side = inputSampleL - inputSampleR;
				iirSampleAR = (iirSampleAR * (1.0 - bass)) + (side * bass); side = side - iirSampleAR;
				inputSampleL = (mid+side)/2.0; inputSampleR = (mid-side)/2.0;
				//bass narrowing filter
				
				if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0; inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0; inputSampleR = asin(inputSampleR);
				//ConsoleBuss processing
				break;
			case kTRICK:
				double inputSample = (inputSampleL + inputSampleR) * 0.5;
				inputSampleL = -inputSample;
				inputSampleR = inputSample;
				break;
		}
		
		//begin Dark		
		if (processing == kDKCD) {
			inputSampleL *= 32768.0; //or 16 bit option
			inputSampleR *= 32768.0; //or 16 bit option
		} else {
			inputSampleL *= 8388608.0; //for literally everything else
			inputSampleR *= 8388608.0; //we will apply the 24 bit Dark
		} //on the not unreasonable assumption that we are very likely playing back on 24 bit DAC
		
		//We are doing it first Left, then Right, because the loops may run faster if
		//they aren't too jammed full of variables. This means re-running code.
		
		//begin left
		int quantA = floor(inputSampleL);
		int quantB = floor(inputSampleL+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		float expectedSlew = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlew += (darkSampleL[x+1] - darkSampleL[x]);
		}
		expectedSlew /= depth; //we have an average of all recent slews
		//we are doing that to voice the thing down into the upper mids a bit
		//it mustn't just soften the brightest treble, it must smooth high mids too
		
		float testA = fabs((darkSampleL[0] - quantA) - expectedSlew);
		float testB = fabs((darkSampleL[0] - quantB) - expectedSlew);
		
		if (testA < testB) inputSampleL = quantA;
		else inputSampleL = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			darkSampleL[x+1] = darkSampleL[x];
		}
		darkSampleL[0] = inputSampleL;
		//end Dark left
		
		//begin right
		quantA = floor(inputSampleR);
		quantB = floor(inputSampleR+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		expectedSlew = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlew += (darkSampleR[x+1] - darkSampleR[x]);
		}
		expectedSlew /= depth; //we have an average of all recent slews
		//we are doing that to voice the thing down into the upper mids a bit
		//it mustn't just soften the brightest treble, it must smooth high mids too
		
		testA = fabs((darkSampleR[0] - quantA) - expectedSlew);
		testB = fabs((darkSampleR[0] - quantB) - expectedSlew);
		
		if (testA < testB) inputSampleR = quantA;
		else inputSampleR = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			darkSampleR[x+1] = darkSampleR[x];
		}
		darkSampleR[0] = inputSampleR;
		//end Dark right
		
		if (processing == kDKCD) {
			inputSampleL /= 32768.0;
			inputSampleR /= 32768.0;
		} else {
			inputSampleL /= 8388608.0;
			inputSampleR /= 8388608.0;
		}
		//does not use 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
} // end namespace
