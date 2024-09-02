//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "mmgr.h"
#endif

#include "base.hpp"
#include "timer.hpp"

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  Externals
//--------------------------------------------------------------------------------------------

extern Timer *timer;

//--------------------------------------------------------------------------------------------
//  Class code
//--------------------------------------------------------------------------------------------

bool Base::init(unsigned long s, unsigned long e)
{
	active = false;
	startTime = s;
	endTime = e;
	time = 0;

	return true;
}

void Base::update()
{
	if(timer)
	{
		time = timer->getTimeMs();
	}
}

void Base::release()
{

}

bool Base::isActive(float time)
{
	if(time >= startTime && time <= endTime)
	{
		active = true;
	}
	else
	{
		active = false;
	}

	return active;
}