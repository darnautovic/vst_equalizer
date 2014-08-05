
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
	// Parametri
	virtual void setParameter(VstInt32 index, float value);
	virtual float getParameter(VstInt32 index);
	virtual void getParameterLabel(VstInt32 index, char* label);
	virtual void getParameterDisplay(VstInt32 index, char* text);
	virtual void getParameterName(VstInt32 index, char* text);
	virtual bool getEffectName(char* name);
	virtual bool getVendorString(char* text);
	virtual bool getProductString(char* text);
	virtual VstInt32 getVendorVersion();

	virtual void calcCoeffs(float f, float g, float q); 
	virtual float calcFreq(float f); 
	virtual float calcGain(float g); 
	virtual float calcQ(float q); 

	// Metode i varijable kojima se moze pristupiti ali nisu promjenjive

protected:
	char programName[32];

	float fFrequency; 
	float fdBGain; 
	float fQ; 

	float jFrequency; 
	float jdBGain; 
	float jQ; 

	float xnm1; 
	float xnm2; 
	float ynm1; 
	float ynm2; 

	float a0, a1, a2, b0, b1, b2; 

	float frekvencija;
	float faktorQ;
	float pojacanje;
};

#endif
