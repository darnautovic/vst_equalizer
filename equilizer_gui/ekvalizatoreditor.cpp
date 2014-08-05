#ifndef __ekvalizatoreditor__
#include "ekvalizatoreditor.h"
#endif

#ifndef __Ekvalizator__
#include "ekvalizator.h"
#endif

#ifndef __resource__
#include "resource.h"
#endif

#include <stdio.h>

//-----------------------------------------------------------------------------
// resource id's
enum {
	// bitmaps
	backgroundId = 101, //IDB_BITMAP1,
	faderBodyId,
	faderHandleId,
		
	// positions
	faderPosition_x = 70,
	faderPosition_y = 60,

	distanceBetweenFaders = 45,  //razmak medju slajderima

	displayPosition_x = 65 , //mislim da je ovo udaljenost displaya od ruba (udesno pomak)
	displayPosition_y = 268,

	displayWidth = 70,
	displayHeight = 14
};

void stringConvert (float value, char* string)
{
	 sprintf (string, "%d%%", value);
}

//-----------------------------------------------------------------------------
// EkvalizatorEditor class implementation
//-----------------------------------------------------------------------------
EkvalizatorEditor::EkvalizatorEditor (AudioEffect *effect)
 : AEffGUIEditor (effect) 
{
	vFrekvencijaFader    = 0;
	vFaktorQFader = 0;
	vPojacanjeFader   = 0;

	// load the background bitmap
	// we don't need to load all bitmaps, this could be done when open is called
	background = new CBitmap (backgroundId);

	// init the size of the plugin
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = (short)background->getWidth ();
	rect.bottom = (short)background->getHeight ();
}

float EkvalizatorEditor::calcFreq(float f) {

	int coef1 = 20000;
	int coef2 = 20;

	float frekvencija = f * coef1 + (20 - f * 20);

	return frekvencija;
}

float EkvalizatorEditor::calcGain(float g) {

	float pojacanje = -12 + g * 25;
	return pojacanje;
}

float EkvalizatorEditor::calcQ(float q) {

	float faktorQ = 12.0 * q + (0.33 - 0.33 * q);

	return faktorQ;
}


//-----------------------------------------------------------------------------
EkvalizatorEditor::~EkvalizatorEditor ()
{
	// free the background bitmap
	if (background)
		background->forget ();
	background = 0;
}

//-----------------------------------------------------------------------------
bool EkvalizatorEditor::open (void *ptr)
{
	// !!! always call this !!!
	AEffGUIEditor::open (ptr);
	
	//--load some bitmaps
	CBitmap* faderBody   = new CBitmap (faderBodyId);
	CBitmap* faderHandle = new CBitmap (faderHandleId);

	

	//--init background frame-----------------------------------------------
	// We use a local CFrame object so that calls to setParameter won't call into objects which may not exist yet. 
	// If all GUI objects are created we assign our class member to this one. See bottom of this method.
	CRect size (0, 0, background->getWidth (), background->getHeight ());
	CFrame* lFrame = new CFrame (size, ptr, this);
	lFrame->setBackground (background);

	

	//--init the faders------------------------------------------------
	int minPos = faderPosition_y;
	int maxPos = faderPosition_y + faderBody->getHeight () - faderHandle->getHeight () - 1;
	CPoint point (0, 0);
	CPoint offset (2, 0);

	// Vibrato Rate
	size (faderPosition_x, faderPosition_y,
          faderPosition_x + faderBody->getWidth (), faderPosition_y + faderBody->getHeight ());
	vFrekvencijaFader = new CVerticalSlider (size, this, nFrekvencija, minPos, maxPos, faderHandle, faderBody, point);
	vFrekvencijaFader->setOffsetHandle (offset);
	vFrekvencijaFader->setValue (effect->getParameter (nFrekvencija));
	lFrame->addView (vFrekvencijaFader);

	// Vibrato Depth
	size.offset (distanceBetweenFaders + faderBody->getWidth (), 0);
	vFaktorQFader = new CVerticalSlider (size, this, nFaktorQ, minPos, maxPos, faderHandle, faderBody, point);
	vFaktorQFader->setOffsetHandle (offset);
	vFaktorQFader->setValue (effect->getParameter (nFaktorQ));
	lFrame->addView (vFaktorQFader);

	// Tremolo Rate
	size.offset (distanceBetweenFaders + faderBody->getWidth (), 0);
	vPojacanjeFader = new CVerticalSlider (size, this, nPojacanje, minPos, maxPos, faderHandle, faderBody, point);
	vPojacanjeFader->setOffsetHandle (offset);
	vPojacanjeFader->setValue (effect->getParameter (nPojacanje));
	vPojacanjeFader->setDefaultValue (0.75f);
	lFrame->addView (vPojacanjeFader);



	//--init the display------------------------------------------------
	// Vibrato Rate
	size (displayPosition_x, displayPosition_y,
          displayPosition_x + displayWidth, displayPosition_y + displayHeight);
	vLowGainDisplay = new CParamDisplay (size, 0, kCenterText);
	vLowGainDisplay->setFont (kNormalFontSmall);
	vLowGainDisplay->setFontColor (kWhiteCColor);
	vLowGainDisplay->setBackColor (kBlackCColor);
	vLowGainDisplay->setFrameColor (kWhiteCColor);
	vLowGainDisplay->setValue (calcFreq(effect->getParameter (nFrekvencija)));
	lFrame->addView (vLowGainDisplay);

	// Vibrato Depth
	size.offset (distanceBetweenFaders + faderBody->getWidth (), 0);
	vMiddleGainDisplay = new CParamDisplay (size, 0, kCenterText);
	vMiddleGainDisplay->setFont (kNormalFontSmall);
	vMiddleGainDisplay->setFontColor (kWhiteCColor);
	vMiddleGainDisplay->setBackColor (kBlackCColor);
	vMiddleGainDisplay->setFrameColor (kWhiteCColor);
	vMiddleGainDisplay->setValue (calcQ(effect->getParameter (nFaktorQ)));
	lFrame->addView (vMiddleGainDisplay);


	// Tremolo Rate
	size.offset (distanceBetweenFaders + faderBody->getWidth (), 0);
	vHighGainDisplay = new CParamDisplay (size, 0, kCenterText);
	vHighGainDisplay->setFont (kNormalFontSmall);
	vHighGainDisplay->setFontColor (kWhiteCColor);
	vHighGainDisplay->setBackColor (kBlackCColor);
	vHighGainDisplay->setFrameColor (kWhiteCColor);
	vHighGainDisplay->setValue (calcGain(effect ->getParameter (nPojacanje)));
	lFrame->addView (vHighGainDisplay);




	// Note : in the constructor of a CBitmap, the number of references is set to 1.
	// Then, each time the bitmap is used (for hinstance in a vertical slider), this
	// number is incremented.
	// As a consequence, everything happens as if the constructor by itself was adding
	// a reference. That's why we need til here a call to forget ().
	// You mustn't call delete here directly, because the bitmap is used by some CControls...
	// These "rules" apply to the other VSTGUI objects too.
	faderBody->forget ();
	faderHandle->forget ();

	frame = lFrame;
	return true;
}

//-----------------------------------------------------------------------------
void EkvalizatorEditor::close ()
{
	delete frame;
	frame = 0;
}

//-----------------------------------------------------------------------------
void EkvalizatorEditor::setParameter (VstInt32 index, float value)
{
	if (frame == 0)
		return;

	// called from ekvlazitaor.cpp (Set Parameter)
	switch (index)
	{
		case nFrekvencija:
			if (vFrekvencijaFader)
				vFrekvencijaFader->setValue (effect->getParameter (index));
			if (vLowGainDisplay)
				vLowGainDisplay->setValue( calcFreq(effect->getParameter (index)));
			break;

		case nFaktorQ:
			if (vFaktorQFader)
				vFaktorQFader->setValue (effect->getParameter (index));
			if (vMiddleGainDisplay)
				vMiddleGainDisplay->setValue (calcQ(effect->getParameter (index)));
			break;

		case nPojacanje:
			if (vPojacanjeFader)
				vPojacanjeFader->setValue (effect->getParameter (index));
			if (vHighGainDisplay)
				vHighGainDisplay->setValue (calcGain(effect->getParameter (index)));
			break;

	}
}

//-----------------------------------------------------------------------------
void EkvalizatorEditor::valueChanged (CDrawContext* context, CControl* control)
{
	long tag = control->getTag ();
	switch (tag)
	{
		case nFrekvencija:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
			break;
		case nFaktorQ:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
			break;
		case nPojacanje:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
		break;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
