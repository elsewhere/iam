//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "slide.hpp"

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Slide::Slide()
{
	prev = NULL;
	next = NULL;
}

Slide::~Slide()
{
	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Slide::init(unsigned long s, unsigned long e)
{	
	active = false;
	startTime = s;
	endTime = e;

	prev = dmsGetTexture("target1");
	next = dmsGetTexture("target2");
	if(!prev || !next)
	{
		dmsMsg("Slide::init error: Couldn't get textures\n");
		return false;
	}

	return true;
}

void Slide::draw()
{	
	float w, h, currentPos;
	const float pos = (time-startTime)/(endTime-startTime);
	
	w = (float)dmsGetWindowWidth();
	h = (float)dmsGetWindowHeight();
	
	currentPos = w * (1.0f - pos);
	
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);

	dmsPerspective2D(w, h);

	glBindTexture(GL_TEXTURE_2D, prev->getID());

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(         0, 0);
		glTexCoord2f(1, 0); glVertex2f(currentPos, 0);
		glTexCoord2f(1, 1);	glVertex2f(currentPos, h);
		glTexCoord2f(0, 1);	glVertex2f(         0, h);
	glEnd();	

	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, next->getID());

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(currentPos, 0);
		glTexCoord2f(1, 0); glVertex2f(         w, 0);
		glTexCoord2f(1, 1);	glVertex2f(         w, h);
		glTexCoord2f(0, 1);	glVertex2f(currentPos, h);
	glEnd();	

	dmsPerspective3D();

	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
}

