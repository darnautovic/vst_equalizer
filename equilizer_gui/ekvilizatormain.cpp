#ifndef __Ekvalizator__
#include "ekvalizator.h"
#endif

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new Ekvalizator (audioMaster);
}

