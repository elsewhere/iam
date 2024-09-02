//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>
#include "dotfade.hpp"

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

DotFade::DotFade()
{
	texture = NULL;
}

DotFade::~DotFade()
{
	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool DotFade::init(unsigned long s, unsigned long e)
{	
	active = false;
	startTime = s;
	endTime = e;
/*		
	texture = dmsGetTexture("dotfade");	
	if(!texture)
	{
		dmsMsg("DotFade::init error: Memory allocation error\n");
		return false;
	}
*/
	
		
	return true;	
}

void DotFade::draw()
{			
	float alpha = 1.0f;
	
	const float pos = (time - startTime) / (endTime - startTime);
				
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);	
	myDisable(MY_CULL_FACE);
	myEnable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);			
	myBlendFunc(GL_ZERO, GL_SRC_COLOR);
				
	dmsSetFOV(155.0f);

	glLoadIdentity();
	glTranslatef(0, 0, -55);
	glRotatef(1000.0f*pos, 1.0f, 1.0f, 1.0f);
	
	glColor4f(alpha, alpha, alpha, alpha);

	glBindTexture(GL_TEXTURE_2D, texture->getID());
	dmsDrawObject("sphere.t3d");	

	dmsSetFOV(45.0f);
}

void DotFade::release()
{
}
