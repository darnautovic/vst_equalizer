
#ifndef __ekvalizatoreditor__
#define __ekvalizatoreditor__

// include VSTGUI
#ifndef __vstgui__
#include "vstgui.h"
#endif


//-----------------------------------------------------------------------------
class EkvalizatorEditor : public AEffGUIEditor, public CControlListener
{
public:
	EkvalizatorEditor (AudioEffect* effect);
	virtual ~EkvalizatorEditor ();

public:
	virtual bool open (void* ptr);
	virtual void close ();

	virtual void setParameter (VstInt32 index, float value);
	virtual void valueChanged (CDrawContext* context, CControl* control);

	virtual float calcFreq(float f); // Convert 0.0...1.0 --> 20 Hz... 20 kHz
	virtual float calcGain(float g); // Convert 0.0...1.0 --> -12 dB...+12 dB
	virtual float calcQ(float q); // Convert 0.0...1.0 --> 0.33... 12.0


private:
	// Controls
	CVerticalSlider* vFrekvencijaFader;
	CVerticalSlider* vFaktorQFader;
	CVerticalSlider* vPojacanjeFader;

	CParamDisplay* vLowGainDisplay;
	CParamDisplay* vMiddleGainDisplay;
	CParamDisplay* vHighGainDisplay;
	


	// Bitmap
	CBitmap* background;
};

#endif
