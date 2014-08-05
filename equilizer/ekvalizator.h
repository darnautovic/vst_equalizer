//-------------------------------------------------------------------------------------------------------
//
// Version 1.0
// Date: Akademska godina 2011/2012
// Created by   : Damir Arnautovic
// Description  : Primjer uporabe VST SDK na jednostavnom primjeru parametarskog Ekvalizatora
//
//------

#ifndef __Ekvalizator__
#define __Ekvalizator__

#include "audioeffectx.h"  // Ukljucivanje datoteke sa glavnim klasama iz SDK
enum {
// Identifikatori parametara EQ algoritma
	nFrekvencija,
	nFaktorQ,
	nPojacanje,
	nNumParams
};

//-------------------------------------------------------------------------------------------------------

class Ekvalizator: public AudioEffectX {
public:
													// Deklariraju se javne metode dostupne drugim klasama (objektima)
	Ekvalizator(audioMasterCallback audioMaster);   // Konstruktor koji kreira objekt audioMasterCallback nuzan za komunikaciju izmedju VST host-a i plugin-a
	~Ekvalizator();


	// Deklaracija metoda nužnih za obradu
	virtual void processReplacing(float** inputs, float** outputs,
			VstInt32 sampleFrames);
	virtual void process(float** inputs, float** outputs,
			VstInt32 sampleFrames);
	// Program
	virtual void setProgramName(char* name);
	virtual void getProgramName(char* name);
	// Parameters
	virtual void setParameter(VstInt32 index, float value);
	virtual float getParameter(VstInt32 index);
	virtual void getParameterLabel(VstInt32 index, char* label);
	virtual void getParameterDisplay(VstInt32 index, char* text);
	virtual void getParameterName(VstInt32 index, char* text);
	virtual bool getEffectName(char* name);
	virtual bool getVendorString(char* text);
	virtual bool getProductString(char* text);
	virtual VstInt32 getVendorVersion();

	virtual void calcCoeffs(float f, float g, float q); // Compute biquad coefficients.
	virtual float calcFreq(float f); // Convert 0.0...1.0 --> 20 Hz... 20 kHz
	virtual float calcGain(float g); // Convert 0.0...1.0 --> -12 dB...+12 dB
	virtual float calcQ(float q); // Convert 0.0...1.0 --> 0.33... 12.0

	// Metode i varijable kojima se moze pristupiti ali nisu promjenjive

protected:
	char programName[32];

	float fFrequency; // 0.0 ... 1.0 Internal
	float fdBGain; // 0.0 ... 1.0 parameter
	float fQ; // 0.0 ... 1.0 format.

	float jFrequency; // 20 Hz ... 20 kHz GUI
	float jdBGain; // -12 dB ... +12 dB parameter
	float jQ; // 0.33 ... 12 format.

	float xnm1; // x[n-1]
	float xnm2; // x[n-2]
	float ynm1; // y[n-1]
	float ynm2; // y[n-2]

	float a0, a1, a2, b0, b1, b2; // The biquad coefficients.

	float frekvencija;
	float faktorQ;
	float pojacanje;
};

#endif
