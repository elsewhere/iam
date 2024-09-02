//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "tilpe1.hpp"

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Tilpe1::Tilpe1()
{

}

Tilpe1::~Tilpe1()
{
	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Tilpe1::init(unsigned long s, unsigned long e)
{	
	active = false;
	startTime = s;
	endTime = e;
	
	return true;
}

void Tilpe1::draw()
{
	unsigned int w, h, i;

	const float pos = (time - startTime) / (endTime - startTime);

	w = dmsGetWindowWidth();
	h = dmsGetWindowHeight();

	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_BLEND);
	myBlendFunc(GL_DST_COLOR, GL_ONE);

	dmsPerspective2D(w, h);
	
	glBegin(GL_LINES);

		for(i = 0; i < h; i += 4)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glVertex2i(0, i);
			glVertex2i(w, i);
		}

	glEnd();

	dmsPerspective3D();

	myEnable(MY_TEXTURE_2D);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
}
