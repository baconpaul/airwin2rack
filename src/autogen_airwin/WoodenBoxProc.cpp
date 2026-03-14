/* ========================================
 *  WoodenBox - WoodenBox.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __WoodenBox_H
#include "WoodenBox.h"
#endif
#include <cstdlib>
namespace airwinconsolidated::WoodenBox {

void WoodenBox::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int clearcoat = (int)(A*16.999);
	if (clearcoat != prevclearcoat) {
		for(int count = 0; count < d4A+2; count++) {b4AL[count] = 0.0; b4AR[count] = 0.0;}
		for(int count = 0; count < d4B+2; count++) {b4BL[count] = 0.0; b4BR[count] = 0.0;}
		for(int count = 0; count < d4C+2; count++) {b4CL[count] = 0.0; b4CR[count] = 0.0;}
		for(int count = 0; count < d4D+2; count++) {b4DL[count] = 0.0; b4DR[count] = 0.0;}
		for(int count = 0; count < d4E+2; count++) {b4EL[count] = 0.0; b4ER[count] = 0.0;}
		for(int count = 0; count < d4F+2; count++) {b4FL[count] = 0.0; b4FR[count] = 0.0;}
		for(int count = 0; count < d4G+2; count++) {b4GL[count] = 0.0; b4GR[count] = 0.0;}
		for(int count = 0; count < d4H+2; count++) {b4HL[count] = 0.0; b4HR[count] = 0.0;}
		for(int count = 0; count < d4I+2; count++) {b4IL[count] = 0.0; b4IR[count] = 0.0;}
		for(int count = 0; count < d4J+2; count++) {b4JL[count] = 0.0; b4JR[count] = 0.0;}
		for(int count = 0; count < d4K+2; count++) {b4KL[count] = 0.0; b4KR[count] = 0.0;}
		for(int count = 0; count < d4L+2; count++) {b4LL[count] = 0.0; b4LR[count] = 0.0;}
		for(int count = 0; count < d4M+2; count++) {b4ML[count] = 0.0; b4MR[count] = 0.0;}
		for(int count = 0; count < d4N+2; count++) {b4NL[count] = 0.0; b4NR[count] = 0.0;}
		for(int count = 0; count < d4O+2; count++) {b4OL[count] = 0.0; b4OR[count] = 0.0;}
		for(int count = 0; count < d4P+2; count++) {b4PL[count] = 0.0; b4PR[count] = 0.0;}		
		c4AL = 1;
		c4BL = 1;
		c4CL = 1;
		c4DL = 1;	
		c4EL = 1;
		c4FL = 1;
		c4GL = 1;
		c4HL = 1;
		c4IL = 1;
		c4JL = 1;
		c4KL = 1;
		c4LL = 1;
		c4ML = 1;
		c4NL = 1;
		c4OL = 1;
		c4PL = 1;
		
		c4AR = 1;
		c4BR = 1;
		c4CR = 1;
		c4DR = 1;	
		c4ER = 1;
		c4FR = 1;
		c4GR = 1;
		c4HR = 1;
		c4IR = 1;
		c4JR = 1;
		c4KR = 1;
		c4LR = 1;
		c4MR = 1;
		c4NR = 1;
		c4OR = 1;
		c4PR = 1;
		switch (clearcoat)
		{
			case 0:
				shortA = 17; shortB = 10; shortC = 23; shortD = 3; shortE = 8; shortF = 7; shortG = 41; shortH = 6; shortI = 3; shortJ = 6; shortK = 59; shortL = 61; shortM = 4; shortN = 71; shortO = 5; shortP = 4; break; //0 to 4 ms, 0 seat room  
			case 1:
				shortA = 12; shortB = 19; shortC = 89; shortD = 25; shortE = 92; shortF = 8; shortG = 41; shortH = 11; shortI = 80; shortJ = 27; shortK = 6; shortL = 4; shortM = 3; shortN = 21; shortO = 7; shortP = 63; break; //0 to 7 ms, 1 seat room  
			case 2:
				shortA = 35; shortB = 19; shortC = 5; shortD = 7; shortE = 15; shortF = 7; shortG = 41; shortH = 191; shortI = 177; shortJ = 3; shortK = 6; shortL = 22; shortM = 23; shortN = 118; shortO = 4; shortP = 79; break; //0 to 11 ms, 4 seat room  
			case 3:
				shortA = 17; shortB = 19; shortC = 105; shortD = 135; shortE = 31; shortF = 86; shortG = 41; shortH = 16; shortI = 3; shortJ = 16; shortK = 6; shortL = 151; shortM = 147; shortN = 26; shortO = 3; shortP = 10; break; //0 to 11 ms, 4 seat room  
			case 4:
				shortA = 134; shortB = 13; shortC = 26; shortD = 10; shortE = 34; shortF = 24; shortG = 4; shortH = 60; shortI = 88; shortJ = 9; shortK = 155; shortL = 11; shortM = 3; shortN = 18; shortO = 9; shortP = 161; break; //0 to 11 ms, 4 seat room  
			case 5:
				shortA = 17; shortB = 82; shortC = 23; shortD = 29; shortE = 133; shortF = 3; shortG = 41; shortH = 27; shortI = 10; shortJ = 177; shortK = 6; shortL = 37; shortM = 14; shortN = 145; shortO = 4; shortP = 9; break; //0 to 12 ms, 4 seat room  
			case 6:
				shortA = 31; shortB = 19; shortC = 3; shortD = 29; shortE = 196; shortF = 11; shortG = 10; shortH = 65; shortI = 21; shortJ = 3; shortK = 148; shortL = 4; shortM = 26; shortN = 7; shortO = 161; shortP = 155; break; //0 to 12 ms, 4 seat room  
			case 7:
				shortA = 17; shortB = 8; shortC = 3; shortD = 37; shortE = 3; shortF = 19; shortG = 41; shortH = 15; shortI = 7; shortJ = 197; shortK = 178; shortL = 22; shortM = 26; shortN = 97; shortO = 16; shortP = 156; break; //0 to 12 ms, 5 seat room  
			case 8:
				shortA = 17; shortB = 3; shortC = 8; shortD = 29; shortE = 39; shortF = 156; shortG = 7; shortH = 43; shortI = 101; shortJ = 8; shortK = 15; shortL = 169; shortM = 67; shortN = 39; shortO = 154; shortP = 4; break; //0 to 13 ms, 5 seat room  
			case 9:
				shortA = 18; shortB = 19; shortC = 23; shortD = 5; shortE = 176; shortF = 3; shortG = 41; shortH = 147; shortI = 7; shortJ = 148; shortK = 5; shortL = 15; shortM = 10; shortN = 30; shortO = 119; shortP = 19; break; //0 to 13 ms, 5 seat room  
			case 10:
				shortA = 173; shortB = 19; shortC = 23; shortD = 27; shortE = 8; shortF = 37; shortG = 7; shortH = 202; shortI = 8; shortJ = 13; shortK = 3; shortL = 174; shortM = 67; shortN = 21; shortO = 73; shortP = 14; break; //0 to 14 ms, 6 seat room  
			case 11:
				shortA = 17; shortB = 19; shortC = 23; shortD = 25; shortE = 19; shortF = 145; shortG = 9; shortH = 43; shortI = 47; shortJ = 203; shortK = 18; shortL = 180; shortM = 226; shortN = 3; shortO = 73; shortP = 12; break; //0 to 15 ms, 7 seat room  
			case 12:
				shortA = 17; shortB = 19; shortC = 23; shortD = 3; shortE = 3; shortF = 20; shortG = 203; shortH = 99; shortI = 207; shortJ = 15; shortK = 10; shortL = 61; shortM = 20; shortN = 174; shortO = 33; shortP = 77; break; //0 to 15 ms, 7 seat room  
			case 13:
				shortA = 17; shortB = 19; shortC = 23; shortD = 29; shortE = 3; shortF = 210; shortG = 183; shortH = 43; shortI = 13; shortJ = 12; shortK = 26; shortL = 220; shortM = 67; shortN = 235; shortO = 11; shortP = 23; break; //0 to 15 ms, 8 seat room  
			case 14:
				shortA = 17; shortB = 3; shortC = 21; shortD = 191; shortE = 31; shortF = 10; shortG = 41; shortH = 218; shortI = 15; shortJ = 6; shortK = 111; shortL = 29; shortM = 129; shortN = 206; shortO = 4; shortP = 7; break; //0 to 16 ms, 8 seat room  
			case 15:
				shortA = 17; shortB = 25; shortC = 240; shortD = 29; shortE = 4; shortF = 18; shortG = 41; shortH = 43; shortI = 29; shortJ = 28; shortK = 250; shortL = 12; shortM = 261; shortN = 9; shortO = 5; shortP = 79; break; //0 to 18 ms, 10 seat room  
			case 16:
			default:
				shortA = 5; shortB = 3; shortC = 23; shortD = 29; shortE = 3; shortF = 257; shortG = 199; shortH = 252; shortI = 132; shortJ = 18; shortK = 11; shortL = 6; shortM = 30; shortN = 27; shortO = 7; shortP = 8; break; //0 to 19 ms, 11 seat room  
		}
		prevclearcoat = clearcoat;
	}
	double reg4n = (1.0-pow(1.0-B,2.0))*0.0336;
	double derez = 1.0;
	derez = fmin(fmax(derez/overallscale,0.0001),1.0);
	int bezFraction = (int)(1.0/derez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	derez = 1.0 / bezFraction;
	bezTrim = 1.0-(derez*bezTrim);
	//the revision more accurately connects the bezier curves
	double wet = 1.0-pow(1.0-C,2.0);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += (inputSampleR*derez);
		bez[bez_SampR] += (inputSampleL*derez); //stereo got reversed somewhere?
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			//left verbs
			double dualmonoSampleL =  bez[bez_SampL];			
			b4AL[c4AL] = dualmonoSampleL + (g4AL * reg4n);
			b4BL[c4BL] = dualmonoSampleL + (g4BL * reg4n);
			b4CL[c4CL] = dualmonoSampleL + (g4CL * reg4n);
			b4DL[c4DL] = dualmonoSampleL + (g4DL * reg4n);
			
			c4AL++; if (c4AL < 0 || c4AL > shortA) c4AL = 0;
			c4BL++; if (c4BL < 0 || c4BL > shortB) c4BL = 0;
			c4CL++; if (c4CL < 0 || c4CL > shortC) c4CL = 0;
			c4DL++; if (c4DL < 0 || c4DL > shortD) c4DL = 0;
			
			double hA = b4AL[c4AL-((c4AL > shortA)?shortA+1:0)];
			double hB = b4BL[c4BL-((c4BL > shortB)?shortB+1:0)];
			double hC = b4CL[c4CL-((c4CL > shortC)?shortC+1:0)];
			double hD = b4DL[c4DL-((c4DL > shortD)?shortD+1:0)];
			b4EL[c4EL] = hA - (hB + hC + hD);
			b4FL[c4FL] = hB - (hA + hC + hD);
			b4GL[c4GL] = hC - (hA + hB + hD);
			b4HL[c4HL] = hD - (hA + hB + hC);
			
			c4EL++; if (c4EL < 0 || c4EL > shortE) c4EL = 0;
			c4FL++; if (c4FL < 0 || c4FL > shortF) c4FL = 0;
			c4GL++; if (c4GL < 0 || c4GL > shortG) c4GL = 0;
			c4HL++; if (c4HL < 0 || c4HL > shortH) c4HL = 0;
			
			hA = b4EL[c4EL-((c4EL > shortE)?shortE+1:0)];
			hB = b4FL[c4FL-((c4FL > shortF)?shortF+1:0)];
			hC = b4GL[c4GL-((c4GL > shortG)?shortG+1:0)];
			hD = b4HL[c4HL-((c4HL > shortH)?shortH+1:0)];
			b4IL[c4IL] = hA - (hB + hC + hD);
			b4JL[c4JL] = hB - (hA + hC + hD);
			b4KL[c4KL] = hC - (hA + hB + hD);
			b4LL[c4LL] = hD - (hA + hB + hC);
			
			c4IL++; if (c4IL < 0 || c4IL > shortI) c4IL = 0;
			c4JL++; if (c4JL < 0 || c4JL > shortJ) c4JL = 0;
			c4KL++; if (c4KL < 0 || c4KL > shortK) c4KL = 0;
			c4LL++; if (c4LL < 0 || c4LL > shortL) c4LL = 0;
			
			hA = b4IL[c4IL-((c4IL > shortI)?shortI+1:0)];
			hB = b4JL[c4JL-((c4JL > shortJ)?shortJ+1:0)];
			hC = b4KL[c4KL-((c4KL > shortK)?shortK+1:0)];
			hD = b4LL[c4LL-((c4LL > shortL)?shortL+1:0)];
			b4ML[c4ML] = hA - (hB + hC + hD);
			b4NL[c4NL] = hB - (hA + hC + hD);
			b4OL[c4OL] = hC - (hA + hB + hD);
			b4PL[c4PL] = hD - (hA + hB + hC);
			
			c4ML++; if (c4ML < 0 || c4ML > shortM) c4ML = 0;
			c4NL++; if (c4NL < 0 || c4NL > shortN) c4NL = 0;
			c4OL++; if (c4OL < 0 || c4OL > shortO) c4OL = 0;
			c4PL++; if (c4PL < 0 || c4PL > shortP) c4PL = 0;
			
			hA = b4ML[c4ML-((c4ML > shortM)?shortM+1:0)];
			hB = b4NL[c4NL-((c4NL > shortN)?shortN+1:0)];
			hC = b4OL[c4OL-((c4OL > shortO)?shortO+1:0)];
			hD = b4PL[c4PL-((c4PL > shortP)?shortP+1:0)];
			g4AL = hA - (hB + hC + hD);
			g4BL = hB - (hA + hC + hD);
			g4CL = hC - (hA + hB + hD);
			g4DL = hD - (hA + hB + hC);			
			dualmonoSampleL = (hA + hB + hC + hD)*0.125;
			
			//right verbs
			double dualmonoSampleR =  bez[bez_SampR];
			b4DR[c4DR] = dualmonoSampleR + (g4DR * reg4n);
			b4HR[c4HR] = dualmonoSampleR + (g4HR * reg4n);
			b4LR[c4LR] = dualmonoSampleR + (g4LR * reg4n);
			b4PR[c4PR] = dualmonoSampleR + (g4PR * reg4n);
			
			c4DR++; if (c4DR < 0 || c4DR > shortD) c4DR = 0;
			c4HR++; if (c4HR < 0 || c4HR > shortH) c4HR = 0;
			c4LR++; if (c4LR < 0 || c4LR > shortL) c4LR = 0;
			c4PR++; if (c4PR < 0 || c4PR > shortP) c4PR = 0;
			
			hA = b4DR[c4DR-((c4DR > shortD)?shortD+1:0)];
			hB = b4HR[c4HR-((c4HR > shortH)?shortH+1:0)];
			hC = b4LR[c4LR-((c4LR > shortL)?shortL+1:0)];
			hD = b4PR[c4PR-((c4PR > shortP)?shortP+1:0)];
			b4CR[c4CR] = hA - (hB + hC + hD);
			b4GR[c4GR] = hB - (hA + hC + hD);
			b4KR[c4KR] = hC - (hA + hB + hD);
			b4OR[c4OR] = hD - (hA + hB + hC);
			
			c4CR++; if (c4CR < 0 || c4CR > shortC) c4CR = 0;
			c4GR++; if (c4GR < 0 || c4GR > shortG) c4GR = 0;
			c4KR++; if (c4KR < 0 || c4KR > shortK) c4KR = 0;
			c4OR++; if (c4OR < 0 || c4OR > shortO) c4OR = 0;
			
			hA = b4CR[c4CR-((c4CR > shortC)?shortC+1:0)];
			hB = b4GR[c4GR-((c4GR > shortG)?shortG+1:0)];
			hC = b4KR[c4KR-((c4KR > shortK)?shortK+1:0)];
			hD = b4OR[c4OR-((c4OR > shortO)?shortO+1:0)];
			b4BR[c4BR] = hA - (hB + hC + hD);
			b4FR[c4FR] = hB - (hA + hC + hD);
			b4JR[c4JR] = hC - (hA + hB + hD);
			b4NR[c4NR] = hD - (hA + hB + hC);
			
			c4BR++; if (c4BR < 0 || c4BR > shortB) c4BR = 0;
			c4FR++; if (c4FR < 0 || c4FR > shortF) c4FR = 0;
			c4JR++; if (c4JR < 0 || c4JR > shortJ) c4JR = 0;
			c4NR++; if (c4NR < 0 || c4NR > shortN) c4NR = 0;
			
			hA = b4BR[c4BR-((c4BR > shortB)?shortB+1:0)];
			hB = b4FR[c4FR-((c4FR > shortF)?shortF+1:0)];
			hC = b4JR[c4JR-((c4JR > shortJ)?shortJ+1:0)];
			hD = b4NR[c4NR-((c4NR > shortN)?shortN+1:0)];
			b4AR[c4AR] = hA - (hB + hC + hD);
			b4ER[c4ER] = hB - (hA + hC + hD);
			b4IR[c4IR] = hC - (hA + hB + hD);
			b4MR[c4MR] = hD - (hA + hB + hC);
			
			c4AR++; if (c4AR < 0 || c4AR > shortA) c4AR = 0;
			c4ER++; if (c4ER < 0 || c4ER > shortE) c4ER = 0;
			c4IR++; if (c4IR < 0 || c4IR > shortI) c4IR = 0;
			c4MR++; if (c4MR < 0 || c4MR > shortM) c4MR = 0;
			
			hA = b4AR[c4AR-((c4AR > shortA)?shortA+1:0)];
			hB = b4ER[c4ER-((c4ER > shortE)?shortE+1:0)];
			hC = b4IR[c4IR-((c4IR > shortI)?shortI+1:0)];
			hD = b4MR[c4MR-((c4MR > shortM)?shortM+1:0)];
			g4DR = hA - (hB + hC + hD);				
			g4HR = hB - (hA + hC + hD);
			g4LR = hC - (hA + hB + hD);
			g4PR = hD - (hA + hB + hC);
			dualmonoSampleR = (hA + hB + hC + hD)*0.125;
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = dualmonoSampleR;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = dualmonoSampleL;
			bez[bez_SampR] = 0.0;
		}
		double X = bez[bez_cycle]*bezTrim;
		double CBL = (bez[bez_CL]*(1.0-X))+(bez[bez_BL]*X);
		double CBR = (bez[bez_CR]*(1.0-X))+(bez[bez_BR]*X);
		double BAL = (bez[bez_BL]*(1.0-X))+(bez[bez_AL]*X);
		double BAR = (bez[bez_BR]*(1.0-X))+(bez[bez_AR]*X);
		inputSampleL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*0.125;
		inputSampleR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*0.125;
		
		inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
		
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

void WoodenBox::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int clearcoat = (int)(A*16.999);
	if (clearcoat != prevclearcoat) {
		for(int count = 0; count < d4A+2; count++) {b4AL[count] = 0.0; b4AR[count] = 0.0;}
		for(int count = 0; count < d4B+2; count++) {b4BL[count] = 0.0; b4BR[count] = 0.0;}
		for(int count = 0; count < d4C+2; count++) {b4CL[count] = 0.0; b4CR[count] = 0.0;}
		for(int count = 0; count < d4D+2; count++) {b4DL[count] = 0.0; b4DR[count] = 0.0;}
		for(int count = 0; count < d4E+2; count++) {b4EL[count] = 0.0; b4ER[count] = 0.0;}
		for(int count = 0; count < d4F+2; count++) {b4FL[count] = 0.0; b4FR[count] = 0.0;}
		for(int count = 0; count < d4G+2; count++) {b4GL[count] = 0.0; b4GR[count] = 0.0;}
		for(int count = 0; count < d4H+2; count++) {b4HL[count] = 0.0; b4HR[count] = 0.0;}
		for(int count = 0; count < d4I+2; count++) {b4IL[count] = 0.0; b4IR[count] = 0.0;}
		for(int count = 0; count < d4J+2; count++) {b4JL[count] = 0.0; b4JR[count] = 0.0;}
		for(int count = 0; count < d4K+2; count++) {b4KL[count] = 0.0; b4KR[count] = 0.0;}
		for(int count = 0; count < d4L+2; count++) {b4LL[count] = 0.0; b4LR[count] = 0.0;}
		for(int count = 0; count < d4M+2; count++) {b4ML[count] = 0.0; b4MR[count] = 0.0;}
		for(int count = 0; count < d4N+2; count++) {b4NL[count] = 0.0; b4NR[count] = 0.0;}
		for(int count = 0; count < d4O+2; count++) {b4OL[count] = 0.0; b4OR[count] = 0.0;}
		for(int count = 0; count < d4P+2; count++) {b4PL[count] = 0.0; b4PR[count] = 0.0;}		
		c4AL = 1;
		c4BL = 1;
		c4CL = 1;
		c4DL = 1;	
		c4EL = 1;
		c4FL = 1;
		c4GL = 1;
		c4HL = 1;
		c4IL = 1;
		c4JL = 1;
		c4KL = 1;
		c4LL = 1;
		c4ML = 1;
		c4NL = 1;
		c4OL = 1;
		c4PL = 1;
		
		c4AR = 1;
		c4BR = 1;
		c4CR = 1;
		c4DR = 1;	
		c4ER = 1;
		c4FR = 1;
		c4GR = 1;
		c4HR = 1;
		c4IR = 1;
		c4JR = 1;
		c4KR = 1;
		c4LR = 1;
		c4MR = 1;
		c4NR = 1;
		c4OR = 1;
		c4PR = 1;
		switch (clearcoat)
		{
			case 0:
				shortA = 17; shortB = 10; shortC = 23; shortD = 3; shortE = 8; shortF = 7; shortG = 41; shortH = 6; shortI = 3; shortJ = 6; shortK = 59; shortL = 61; shortM = 4; shortN = 71; shortO = 5; shortP = 4; break; //0 to 4 ms, 0 seat room  
			case 1:
				shortA = 12; shortB = 19; shortC = 89; shortD = 25; shortE = 92; shortF = 8; shortG = 41; shortH = 11; shortI = 80; shortJ = 27; shortK = 6; shortL = 4; shortM = 3; shortN = 21; shortO = 7; shortP = 63; break; //0 to 7 ms, 1 seat room  
			case 2:
				shortA = 35; shortB = 19; shortC = 5; shortD = 7; shortE = 15; shortF = 7; shortG = 41; shortH = 191; shortI = 177; shortJ = 3; shortK = 6; shortL = 22; shortM = 23; shortN = 118; shortO = 4; shortP = 79; break; //0 to 11 ms, 4 seat room  
			case 3:
				shortA = 17; shortB = 19; shortC = 105; shortD = 135; shortE = 31; shortF = 86; shortG = 41; shortH = 16; shortI = 3; shortJ = 16; shortK = 6; shortL = 151; shortM = 147; shortN = 26; shortO = 3; shortP = 10; break; //0 to 11 ms, 4 seat room  
			case 4:
				shortA = 134; shortB = 13; shortC = 26; shortD = 10; shortE = 34; shortF = 24; shortG = 4; shortH = 60; shortI = 88; shortJ = 9; shortK = 155; shortL = 11; shortM = 3; shortN = 18; shortO = 9; shortP = 161; break; //0 to 11 ms, 4 seat room  
			case 5:
				shortA = 17; shortB = 82; shortC = 23; shortD = 29; shortE = 133; shortF = 3; shortG = 41; shortH = 27; shortI = 10; shortJ = 177; shortK = 6; shortL = 37; shortM = 14; shortN = 145; shortO = 4; shortP = 9; break; //0 to 12 ms, 4 seat room  
			case 6:
				shortA = 31; shortB = 19; shortC = 3; shortD = 29; shortE = 196; shortF = 11; shortG = 10; shortH = 65; shortI = 21; shortJ = 3; shortK = 148; shortL = 4; shortM = 26; shortN = 7; shortO = 161; shortP = 155; break; //0 to 12 ms, 4 seat room  
			case 7:
				shortA = 17; shortB = 8; shortC = 3; shortD = 37; shortE = 3; shortF = 19; shortG = 41; shortH = 15; shortI = 7; shortJ = 197; shortK = 178; shortL = 22; shortM = 26; shortN = 97; shortO = 16; shortP = 156; break; //0 to 12 ms, 5 seat room  
			case 8:
				shortA = 17; shortB = 3; shortC = 8; shortD = 29; shortE = 39; shortF = 156; shortG = 7; shortH = 43; shortI = 101; shortJ = 8; shortK = 15; shortL = 169; shortM = 67; shortN = 39; shortO = 154; shortP = 4; break; //0 to 13 ms, 5 seat room  
			case 9:
				shortA = 18; shortB = 19; shortC = 23; shortD = 5; shortE = 176; shortF = 3; shortG = 41; shortH = 147; shortI = 7; shortJ = 148; shortK = 5; shortL = 15; shortM = 10; shortN = 30; shortO = 119; shortP = 19; break; //0 to 13 ms, 5 seat room  
			case 10:
				shortA = 173; shortB = 19; shortC = 23; shortD = 27; shortE = 8; shortF = 37; shortG = 7; shortH = 202; shortI = 8; shortJ = 13; shortK = 3; shortL = 174; shortM = 67; shortN = 21; shortO = 73; shortP = 14; break; //0 to 14 ms, 6 seat room  
			case 11:
				shortA = 17; shortB = 19; shortC = 23; shortD = 25; shortE = 19; shortF = 145; shortG = 9; shortH = 43; shortI = 47; shortJ = 203; shortK = 18; shortL = 180; shortM = 226; shortN = 3; shortO = 73; shortP = 12; break; //0 to 15 ms, 7 seat room  
			case 12:
				shortA = 17; shortB = 19; shortC = 23; shortD = 3; shortE = 3; shortF = 20; shortG = 203; shortH = 99; shortI = 207; shortJ = 15; shortK = 10; shortL = 61; shortM = 20; shortN = 174; shortO = 33; shortP = 77; break; //0 to 15 ms, 7 seat room  
			case 13:
				shortA = 17; shortB = 19; shortC = 23; shortD = 29; shortE = 3; shortF = 210; shortG = 183; shortH = 43; shortI = 13; shortJ = 12; shortK = 26; shortL = 220; shortM = 67; shortN = 235; shortO = 11; shortP = 23; break; //0 to 15 ms, 8 seat room  
			case 14:
				shortA = 17; shortB = 3; shortC = 21; shortD = 191; shortE = 31; shortF = 10; shortG = 41; shortH = 218; shortI = 15; shortJ = 6; shortK = 111; shortL = 29; shortM = 129; shortN = 206; shortO = 4; shortP = 7; break; //0 to 16 ms, 8 seat room  
			case 15:
				shortA = 17; shortB = 25; shortC = 240; shortD = 29; shortE = 4; shortF = 18; shortG = 41; shortH = 43; shortI = 29; shortJ = 28; shortK = 250; shortL = 12; shortM = 261; shortN = 9; shortO = 5; shortP = 79; break; //0 to 18 ms, 10 seat room  
			case 16:
			default:
				shortA = 5; shortB = 3; shortC = 23; shortD = 29; shortE = 3; shortF = 257; shortG = 199; shortH = 252; shortI = 132; shortJ = 18; shortK = 11; shortL = 6; shortM = 30; shortN = 27; shortO = 7; shortP = 8; break; //0 to 19 ms, 11 seat room  
		}
		prevclearcoat = clearcoat;
	}
	double reg4n = (1.0-pow(1.0-B,2.0))*0.0336;
	double derez = 1.0;
	derez = fmin(fmax(derez/overallscale,0.0001),1.0);
	int bezFraction = (int)(1.0/derez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	derez = 1.0 / bezFraction;
	bezTrim = 1.0-(derez*bezTrim);
	//the revision more accurately connects the bezier curves
	double wet = 1.0-pow(1.0-C,2.0);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += (inputSampleR*derez);
		bez[bez_SampR] += (inputSampleL*derez); //stereo got reversed somewhere?
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			//left verbs
			double dualmonoSampleL =  bez[bez_SampL];			
			b4AL[c4AL] = dualmonoSampleL + (g4AL * reg4n);
			b4BL[c4BL] = dualmonoSampleL + (g4BL * reg4n);
			b4CL[c4CL] = dualmonoSampleL + (g4CL * reg4n);
			b4DL[c4DL] = dualmonoSampleL + (g4DL * reg4n);
			
			c4AL++; if (c4AL < 0 || c4AL > shortA) c4AL = 0;
			c4BL++; if (c4BL < 0 || c4BL > shortB) c4BL = 0;
			c4CL++; if (c4CL < 0 || c4CL > shortC) c4CL = 0;
			c4DL++; if (c4DL < 0 || c4DL > shortD) c4DL = 0;
			
			double hA = b4AL[c4AL-((c4AL > shortA)?shortA+1:0)];
			double hB = b4BL[c4BL-((c4BL > shortB)?shortB+1:0)];
			double hC = b4CL[c4CL-((c4CL > shortC)?shortC+1:0)];
			double hD = b4DL[c4DL-((c4DL > shortD)?shortD+1:0)];
			b4EL[c4EL] = hA - (hB + hC + hD);
			b4FL[c4FL] = hB - (hA + hC + hD);
			b4GL[c4GL] = hC - (hA + hB + hD);
			b4HL[c4HL] = hD - (hA + hB + hC);
			
			c4EL++; if (c4EL < 0 || c4EL > shortE) c4EL = 0;
			c4FL++; if (c4FL < 0 || c4FL > shortF) c4FL = 0;
			c4GL++; if (c4GL < 0 || c4GL > shortG) c4GL = 0;
			c4HL++; if (c4HL < 0 || c4HL > shortH) c4HL = 0;
			
			hA = b4EL[c4EL-((c4EL > shortE)?shortE+1:0)];
			hB = b4FL[c4FL-((c4FL > shortF)?shortF+1:0)];
			hC = b4GL[c4GL-((c4GL > shortG)?shortG+1:0)];
			hD = b4HL[c4HL-((c4HL > shortH)?shortH+1:0)];
			b4IL[c4IL] = hA - (hB + hC + hD);
			b4JL[c4JL] = hB - (hA + hC + hD);
			b4KL[c4KL] = hC - (hA + hB + hD);
			b4LL[c4LL] = hD - (hA + hB + hC);
			
			c4IL++; if (c4IL < 0 || c4IL > shortI) c4IL = 0;
			c4JL++; if (c4JL < 0 || c4JL > shortJ) c4JL = 0;
			c4KL++; if (c4KL < 0 || c4KL > shortK) c4KL = 0;
			c4LL++; if (c4LL < 0 || c4LL > shortL) c4LL = 0;
			
			hA = b4IL[c4IL-((c4IL > shortI)?shortI+1:0)];
			hB = b4JL[c4JL-((c4JL > shortJ)?shortJ+1:0)];
			hC = b4KL[c4KL-((c4KL > shortK)?shortK+1:0)];
			hD = b4LL[c4LL-((c4LL > shortL)?shortL+1:0)];
			b4ML[c4ML] = hA - (hB + hC + hD);
			b4NL[c4NL] = hB - (hA + hC + hD);
			b4OL[c4OL] = hC - (hA + hB + hD);
			b4PL[c4PL] = hD - (hA + hB + hC);
			
			c4ML++; if (c4ML < 0 || c4ML > shortM) c4ML = 0;
			c4NL++; if (c4NL < 0 || c4NL > shortN) c4NL = 0;
			c4OL++; if (c4OL < 0 || c4OL > shortO) c4OL = 0;
			c4PL++; if (c4PL < 0 || c4PL > shortP) c4PL = 0;
			
			hA = b4ML[c4ML-((c4ML > shortM)?shortM+1:0)];
			hB = b4NL[c4NL-((c4NL > shortN)?shortN+1:0)];
			hC = b4OL[c4OL-((c4OL > shortO)?shortO+1:0)];
			hD = b4PL[c4PL-((c4PL > shortP)?shortP+1:0)];
			g4AL = hA - (hB + hC + hD);
			g4BL = hB - (hA + hC + hD);
			g4CL = hC - (hA + hB + hD);
			g4DL = hD - (hA + hB + hC);			
			dualmonoSampleL = (hA + hB + hC + hD)*0.125;
			
			//right verbs
			double dualmonoSampleR =  bez[bez_SampR];
			b4DR[c4DR] = dualmonoSampleR + (g4DR * reg4n);
			b4HR[c4HR] = dualmonoSampleR + (g4HR * reg4n);
			b4LR[c4LR] = dualmonoSampleR + (g4LR * reg4n);
			b4PR[c4PR] = dualmonoSampleR + (g4PR * reg4n);
			
			c4DR++; if (c4DR < 0 || c4DR > shortD) c4DR = 0;
			c4HR++; if (c4HR < 0 || c4HR > shortH) c4HR = 0;
			c4LR++; if (c4LR < 0 || c4LR > shortL) c4LR = 0;
			c4PR++; if (c4PR < 0 || c4PR > shortP) c4PR = 0;
			
			hA = b4DR[c4DR-((c4DR > shortD)?shortD+1:0)];
			hB = b4HR[c4HR-((c4HR > shortH)?shortH+1:0)];
			hC = b4LR[c4LR-((c4LR > shortL)?shortL+1:0)];
			hD = b4PR[c4PR-((c4PR > shortP)?shortP+1:0)];
			b4CR[c4CR] = hA - (hB + hC + hD);
			b4GR[c4GR] = hB - (hA + hC + hD);
			b4KR[c4KR] = hC - (hA + hB + hD);
			b4OR[c4OR] = hD - (hA + hB + hC);
			
			c4CR++; if (c4CR < 0 || c4CR > shortC) c4CR = 0;
			c4GR++; if (c4GR < 0 || c4GR > shortG) c4GR = 0;
			c4KR++; if (c4KR < 0 || c4KR > shortK) c4KR = 0;
			c4OR++; if (c4OR < 0 || c4OR > shortO) c4OR = 0;
			
			hA = b4CR[c4CR-((c4CR > shortC)?shortC+1:0)];
			hB = b4GR[c4GR-((c4GR > shortG)?shortG+1:0)];
			hC = b4KR[c4KR-((c4KR > shortK)?shortK+1:0)];
			hD = b4OR[c4OR-((c4OR > shortO)?shortO+1:0)];
			b4BR[c4BR] = hA - (hB + hC + hD);
			b4FR[c4FR] = hB - (hA + hC + hD);
			b4JR[c4JR] = hC - (hA + hB + hD);
			b4NR[c4NR] = hD - (hA + hB + hC);
			
			c4BR++; if (c4BR < 0 || c4BR > shortB) c4BR = 0;
			c4FR++; if (c4FR < 0 || c4FR > shortF) c4FR = 0;
			c4JR++; if (c4JR < 0 || c4JR > shortJ) c4JR = 0;
			c4NR++; if (c4NR < 0 || c4NR > shortN) c4NR = 0;
			
			hA = b4BR[c4BR-((c4BR > shortB)?shortB+1:0)];
			hB = b4FR[c4FR-((c4FR > shortF)?shortF+1:0)];
			hC = b4JR[c4JR-((c4JR > shortJ)?shortJ+1:0)];
			hD = b4NR[c4NR-((c4NR > shortN)?shortN+1:0)];
			b4AR[c4AR] = hA - (hB + hC + hD);
			b4ER[c4ER] = hB - (hA + hC + hD);
			b4IR[c4IR] = hC - (hA + hB + hD);
			b4MR[c4MR] = hD - (hA + hB + hC);
			
			c4AR++; if (c4AR < 0 || c4AR > shortA) c4AR = 0;
			c4ER++; if (c4ER < 0 || c4ER > shortE) c4ER = 0;
			c4IR++; if (c4IR < 0 || c4IR > shortI) c4IR = 0;
			c4MR++; if (c4MR < 0 || c4MR > shortM) c4MR = 0;
			
			hA = b4AR[c4AR-((c4AR > shortA)?shortA+1:0)];
			hB = b4ER[c4ER-((c4ER > shortE)?shortE+1:0)];
			hC = b4IR[c4IR-((c4IR > shortI)?shortI+1:0)];
			hD = b4MR[c4MR-((c4MR > shortM)?shortM+1:0)];
			g4DR = hA - (hB + hC + hD);				
			g4HR = hB - (hA + hC + hD);
			g4LR = hC - (hA + hB + hD);
			g4PR = hD - (hA + hB + hC);
			dualmonoSampleR = (hA + hB + hC + hD)*0.125;
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = dualmonoSampleR;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = dualmonoSampleL;
			bez[bez_SampR] = 0.0;
		}
		double X = bez[bez_cycle]*bezTrim;
		double CBL = (bez[bez_CL]*(1.0-X))+(bez[bez_BL]*X);
		double CBR = (bez[bez_CR]*(1.0-X))+(bez[bez_BR]*X);
		double BAL = (bez[bez_BL]*(1.0-X))+(bez[bez_AL]*X);
		double BAR = (bez[bez_BR]*(1.0-X))+(bez[bez_AR]*X);
		inputSampleL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*0.125;
		inputSampleR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*0.125;
		
		inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
		
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
