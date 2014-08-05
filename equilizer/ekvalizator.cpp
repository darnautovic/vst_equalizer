//-------------------------------------------------------------------------------------------------------
// 
// Version 1.0	
// Date: Akademska godina 2010/2011
// Created by   : Damir Arnautovic
// Description  : Primjer uporabe VST SDK na jednostavnom primjeru parametarskog Ekvalizatora
//
//------

#ifndef __Ekvalizator_H
#include "Ekvalizator.h" // Provjera i ukljucivanje header datoteke
#include <cmath>
#endif 

#define M_PI 3.1415926535897932384626433832795

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
	return new Ekvalizator(audioMaster);
}

//-------------------------------------------------------------------------------------------------------
//
Ekvalizator::Ekvalizator(audioMasterCallback audioMaster) //SVE ISTO
:
		AudioEffectX(audioMaster, 1, nNumParams) // PRESET, BROJ PARAMETARA (f3Band)
{
	setNumInputs(1); //  in
	setNumOutputs(1); //  out
	setUniqueID('Eq3'); // identifikacija
	canProcessReplacing(); // podrška 'replacing' izlaza
	vst_strncpy(programName, "Default", kVstMaxProgNameLen); // default program name

	frekvencija = 1.0;
	pojacanje = 1.0;
	faktorQ = 1.0;

	calcCoeffs(frekvencija, pojacanje, faktorQ);
}
// Destruktor
Ekvalizator::~Ekvalizator() {
}

void Ekvalizator::calcCoeffs(float f, float g, float q) {
	float A, omega, cs, sn, alpha; 
	A = pow(10, g / 40.0f);
	omega = (2 * M_PI * f) / sampleRate; 
	sn = sin(omega);
	cs = cos(omega);
	alpha = sn / (2.0 * q);
	b0 = 1 + (alpha * A); 
	b1 = -2 * cs;
	b2 = 1 - (alpha * A);
	a0 = 1 + (alpha / (float) A);
	a1 = -2 * cs;
	a2 = 1 - (alpha / (float) A);
}

float Ekvalizator::calcFreq(float f) {

	int coef1 = 20000;
	int coef2 = 20;

	float frekvencija = f * coef1 + (20 - f * 20);

	return frekvencija;
}

float Ekvalizator::calcGain(float g) {

	float pojacanje = -12 + g * 25;
	return pojacanje;
}

float Ekvalizator::calcQ(float q) {

	float faktorQ = 12.0 * q + (0.33 - 0.33 * q);

	return faktorQ;
}

//-------------------------------------------------------------------------------------------------------

// Koristi se za promjenu naziva plugin-a
void Ekvalizator::setProgramName(char* name) {
	vst_strncpy(programName, name, kVstMaxProgNameLen);
}

// Koristi se za dobijanje naziva plugin-a
void Ekvalizator::getProgramName(char* name) {
	vst_strncpy(name, programName, kVstMaxProgNameLen);
}

// Postavljanje inicijalno postavljanje parametara plugin-a.
// index je identifikator parametara dok value predstavlja vrijednost parametra
void Ekvalizator::setParameter(VstInt32 index, float value) {
	switch (index) {
	case nFrekvencija:
		calcCoeffs(calcFreq(value), pojacanje, faktorQ);
		frekvencija = calcFreq(value);
		break;
	case nFaktorQ:
		calcCoeffs(frekvencija, pojacanje, calcQ(value));
		faktorQ = calcQ(value);
		break;
	case nPojacanje:
		calcCoeffs(frekvencija, calcGain(value), faktorQ);
		pojacanje = calcGain(value);
		break;
	}

}

// Dobavljanje parametara
float Ekvalizator::getParameter(VstInt32 index) {
	float v = 0;
	switch (index) {
	case nFrekvencija:
		v = frekvencija;
		break;
	case nFaktorQ:
		v = faktorQ;
		break;
	case nPojacanje:
		v = pojacanje;
		break;
	}
	return v;
}

// Dobavljanje naziva parametara
void Ekvalizator::getParameterName(VstInt32 index, char *label) {
	switch (index) {
	case nFrekvencija:
		strcpy(label, "Frekvencija");
		break;
	case nFaktorQ:
		strcpy(label, "Q");
		break;
	case nPojacanje:
		strcpy(label, "Pojacanje");
		break;
	}
}

// Postavljanje izgleda parametara. Po defaultu vrijednost parametra koji korisnik mijenja nalazi se
// u rasponu od 0. - 1. Ovime je moguce podesiti raspone na zeljene (kako ce se prikazati raspon vrijednosti)
void Ekvalizator::getParameterDisplay(VstInt32 index, char *text) {
	switch (index) {
	case nFrekvencija:
		float2string(frekvencija, text, kVstMaxParamStrLen);
		break;
	case nFaktorQ:
		float2string(faktorQ, text, kVstMaxParamStrLen);
		break;
	case nPojacanje:
		float2string(pojacanje, text, kVstMaxParamStrLen);
		break;
	}
}

// Defaultna postavka natpisa (label)
void Ekvalizator::getParameterLabel(VstInt32 index, char *label) {
	switch (index) {
	case nFrekvencija:
		strcpy(label, "Hz");
		break;
	case nFaktorQ:
		strcpy(label, "q");
		break;
	case nPojacanje:
		strcpy(label, "dB");
		break;
	}
}

//------------------------------------------------------------------------

// Naziv plugin-a
bool Ekvalizator::getEffectName(char* name) {
	vst_strncpy(name, "Ekvalizator bez GUI", kVstMaxEffectNameLen);
	return true;
}

//
bool Ekvalizator::getProductString(char* text) {
	vst_strncpy(text, "Ekvalizator", kVstMaxProductStrLen);
	return true;
}

// Postavke proizvodjaca (autora) plugina
bool Ekvalizator::getVendorString(char* text) {
	vst_strncpy(text, "Damir Arnautovic, FESB, Split", kVstMaxVendorStrLen);
	return true;
}

// Verzija plugina
VstInt32 Ekvalizator::getVendorVersion() {
	return 1000;
}

//-----------------------------------------------------------------------------------------

// Metoda processReplacing uzima ulazni stream (podataka -zvuk), primjenjuje na njima algoritam i predaje na izlaz s tim da u potpunosti prepisuje izlazni buffer
// Implementacija ove metode je opcionalna
void Ekvalizator::processReplacing(float** inputs, float** outputs,
		VstInt32 sampleFrames) {
	float *in = inputs[0]; 
	float *out = outputs[0];
	float xn, yn; 
	float sampleRate = updateSampleRate();

	while (--sampleFrames >= 0) 
	{
		xn = *in++; 

		yn = (b0 * xn + b1 * xnm1 + b2 * xnm2 - a1 * ynm1 - a2 * ynm2) / a0; 
		xnm2 = xnm1; 
		xnm1 = xn; 
		ynm2 = ynm1; 
		ynm1 = yn; 

		(*out++) = (yn); 
	}
}


void Ekvalizator::process(float** inputs, float** outputs,
		VstInt32 sampleFrames) {
	float *in = inputs[0]; 
	float *out = outputs[0]; 
	float xn, yn; 
	float sampleRate = updateSampleRate();

	while (--sampleFrames >= 0) 
	{
		xn = *in++; 

		yn = (b0 * xn + b1 * xnm1 + b2 * xnm2 - a1 * ynm1 - a2 * ynm2) / a0; 
		xnm2 = xnm1; 
		xnm1 = xn;
		ynm2 = ynm1; 
		ynm1 = yn; 

		(*out++) += (yn); 
	}
}

