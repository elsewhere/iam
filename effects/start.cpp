//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "start.hpp"

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Start::Start()
{

}

Start::~Start()
{
	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Start::init(unsigned long s, unsigned long e)
{	
	active = false;
	startTime = s;
	endTime = e;
	
	return true;
}

void Start::draw()
{
	int w, h;
	Texture *texture = dmsGetTexture("loadpic.jpg");	
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;
	
	const float fadeoutstart = 0.0f;
	const float fadeoutstop = 0.25f;
	
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= (pos-fadeoutstart) / (fadeoutstop-fadeoutstart);			
	
	w = dmsGetWindowWidth();
	h = dmsGetWindowHeight();
	
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	dmsPerspective2D(640, 480);

	glBindTexture(GL_TEXTURE_2D, texture->getID());

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f-alpha);
		
	glBegin(GL_QUADS);
	
		glTexCoord2f(0, 0); glVertex2f(  0, 480);
		glTexCoord2f(1, 0); glVertex2f(640, 480);
		glTexCoord2f(1, 1); glVertex2f(640,   0);
		glTexCoord2f(0, 1); glVertex2f(  0,   0);
		
	glEnd();

	dmsPerspective3D();

	write(512-64, 40, pos, 0.35f, 0.85f, alpha, "teknofontti.pcx", "I am");
	write(512-64, 70, pos, 0.35f, 0.85f, alpha, "luontofontti.pcx", "I am");

	write(512-336, 150, pos, 0.40f, 0.90f, alpha, "luontofontti.pcx", "A traction production");
	write(512-8*32, 180, pos, 0.45f, 0.95f, alpha, "luontofontti.pcx", "At Assembly 2004");
			
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
}
