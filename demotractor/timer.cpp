//-------------------------------------------------------
// Includes
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "mmgr.h"
#endif

#include "timer.hpp"
#include "globals.hpp"

//-------------------------------------------------------
// Staattiset julkiset j‰senet
//-------------------------------------------------------

bool Timer::createdFlag = false;
Timer *Timer::instance = NULL;

//-------------------------------------------------------
// Destructori, Constructorit ovat privoja jotta niit‰ ei 
// voisi kutsua
//-------------------------------------------------------


Timer::~Timer()
{
}

//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

Timer *Timer::create()
{	
	if(!createdFlag)
	{
		instance = new Timer;
		if(!instance)
		{
			throw "Timer::create(): Memory allocation error";
			return NULL;
		}

		createdFlag = true;

		dmsMsg("Timer::create(): Timer instance created\n");
	}
	else
	{		
		throw "One instance of class Log allready running";
		return NULL;
	}	

	return instance;
}

void Timer::init()
{
	//	High Resolution Timerin taajuutta ei voi muuttaa niin kauan
	//	kuin systeemi on p‰‰ll‰. Taajuus haetaan vain kerran.
		
	HRT = true;
	startTime = 0;
	if(!QueryPerformanceFrequency((LARGE_INTEGER *)ticksPerSec))
	{				
		ticksPerSec = (__int64)1000.0f;
		HRT = false;
		startTime = (float)timeGetTime();		
	}
	else
	{
		// Jos ep‰onnistui k‰ytet‰‰n MultimediaTimeria
		startTime = getTime();
	}

	addTime = 0.0f;
	frames = 0;
	lastTime = 0;
	currentTime = 0;
	fps = 0;
	time = 0;	

	update();
}

void Timer::update()
{						
	if(HRT)
	{
		UINT64 ticks;

		// onnistuu
		QueryPerformanceCounter((LARGE_INTEGER *)&ticks);
		
		// jaetaan taajuudella jotta saadaan sekunnit
		time = ((float)(__int64)ticks)/((float)(__int64)ticksPerSec);
		(startTime == 0.0f) ? startTime = time : 0;

		// v‰hennet‰‰n alkuaika jotta saadaan kulunut aika jonka ohjelma 
		// on ollut k‰ynniss‰
		time = time + addTime - startTime;
	}
	else
	{
		// Jos ei ole HRT-ajastinta, k‰ytet‰‰n Multimedia timeria
		// vaatii WINMM.LIB:n				
		time = timeGetTime();
		(startTime == 0.0f) ? startTime = time : 0;

		time = time + addTime - startTime;
	}	

	if(time < 0.0f)
	{
		addTime = 0.0f;
		frames = 0;
		startTime = 0.0f;
		lastTime = 0;
		currentTime = 0;		
		fps = 0;		
		time = 0.0f;
	}
	else
	{
		deltaTime = time - lastTime;
		lastTime = time;

		frames++;
		fps = frames / (time*0.001f);
	}
}

//-------------------------------------------------------
//	Getters
//-------------------------------------------------------

// Palauttaa ajan sekunteina
float Timer::getTime()
{	
	return time*0.001f;
}

// Palauttaa ajan millisekunteina
float Timer::getTimeMs()
{	
	return time;
}

float Timer::getFPS()
{
	return fps;
}

float Timer::getDeltaTime()
{
	return deltaTime;
}

//-------------------------------------------------------
//	Misc.
//-------------------------------------------------------

void Timer::addMs(float a)
{	
	addTime += a;
}

void Timer::release()
{
	createdFlag = false;

	if(instance)
	{
		delete instance;
		instance = NULL;
	}	

	dmsMsg("Timer::release(): Instance deleted\n");
}
