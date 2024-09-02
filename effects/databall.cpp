//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "databall.hpp"

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

DataBall::DataBall()
{

}

DataBall::~DataBall()
{
	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------
bool DataBall::init(unsigned long s, unsigned long e)
{	
//	int i;	

	active = false;
	startTime = s;
	endTime = e;	

	myEnable(MY_LINE_SMOOTH);
			
	return true;
}

void DataBall::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.3f;
	const float fadeoutstart = 0.7f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= ((pos-fadeinstart) / (fadeinstop-fadeinstart));
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1.0f-((pos-fadeoutstart) / (fadeoutstop-fadeoutstart));

	//alpha = alpha*0.10f;
	alpha *= 0.5f;
	myEnable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
	
	//myDisable(MY_BLEND);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	dmsSetFOV(155.0f); 
	Texture *texture = dmsGetTexture("databall.jpg");

	glLoadIdentity();
	glTranslatef(0, 0, -55);
	glRotatef(100.0f*pos, 1.0f, 1.0f, 1.0f);

	//glColor4f(1.0f, 0.5f, 0.25f, alpha);
	glColor4f(0.8f, 0.9f, 1.0f, alpha);

	glBindTexture(GL_TEXTURE_2D, texture->getID());
	dmsDrawObject("sphere.t3d");

	glLoadIdentity();
	glTranslatef(0, 0, -55);
	glRotatef(110.0f*pos, -0.9f, 0.8f, 0.7f);

	//glColor4f(1.0f, 0.3f, 0.25f, alpha);
	glColor4f(0.7f, 0.8f, 0.9f, alpha);

	glBindTexture(GL_TEXTURE_2D, texture->getID());
	dmsDrawObject("sphere.t3d");

	glLoadIdentity();
	glTranslatef(0, 0, -55);
	glRotatef(120.0f*pos, 0.4f, 0.5f, -0.6f);

	//glColor4f(0.75f, 0.5f, 0.10f, alpha);
	glColor4f(0.6f, 0.7f, 0.8f, alpha);

	glBindTexture(GL_TEXTURE_2D, texture->getID());
	dmsDrawObject("sphere.t3d");

	dmsSetFOV(45.0f);

	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
}

