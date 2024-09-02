//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "kusetus.hpp"

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Kusetus::Kusetus()
{

}

Kusetus::~Kusetus()
{
	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Kusetus::init(unsigned long s, unsigned long e)
{	
	active = false;
	startTime = s;
	endTime = e;
	
	return true;
}

void Kusetus::draw()
{
	Texture *texture = dmsGetTexture("varjokuva.jpg");
	float fogColor[4]= { 0.0f, 0.0f, 0.0f, 1.0f };

	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 1.0f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 30.0f);
	glFogf(GL_FOG_END, 50.0f);

	myEnable(MY_FOG);

	float alpha = 1.0f;
	const float pos = (time - startTime) / (endTime - startTime);
	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.5f;
	
	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= ((pos-fadeinstart) / (fadeinstop-fadeinstart));
	
	myEnable(MY_TEXTURE_2D);
	//myDisable(MY_TEXTURE_2D);
	myDisable(MY_LIGHTING);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_BLEND);
	
	glBindTexture(GL_TEXTURE_2D, texture->getID());

	myBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
	glLoadIdentity();
	
	glTranslatef(6.0f, 0.0f, -50.0f + pos * 100.0f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	
	glBegin(GL_QUADS);

		glTexCoord2f(0, 0); glVertex3f(-9, -6, 0);
		glTexCoord2f(1, 0); glVertex3f( 9, -6, 0);
		glTexCoord2f(1, 1); glVertex3f( 9,  6, 0);
		glTexCoord2f(0, 1); glVertex3f(-9,  6, 0);

	glEnd();

	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glLoadIdentity();
	
	glTranslatef(5.5f, 0.0f, -50.0f + pos * 100.0f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	
	glColor4f(1.0f, 1.0f, 1.0f, alpha);

	glBegin(GL_QUADS);

		glTexCoord2f(0, 0); glVertex3f(-9, -6, 0);
		glTexCoord2f(1, 0); glVertex3f( 9, -6, 0);
		glTexCoord2f(1, 1); glVertex3f( 9,  6, 0);
		glTexCoord2f(0, 1); glVertex3f(-9,  6, 0);

	glEnd();
	
/*
	glLoadIdentity();	
	myBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
	
	dmsWrite3DEx(6.0f, 0.0f, -50.0f + pos * 100.0f, 0.0f, 1.0f, 0.0f, -90.0f, 1.0f, "teknofontti.pcx", "...fuckers mother all to hello");

	glLoadIdentity();
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	dmsWrite3DEx(5.5f, 0.0f, -50.0f + pos * 100.0f, 0.0f, alpha, 0.0f, -90.0f, alpha, "teknofontti.pcx", "...fuckers mother all to hello");
*/
	myDisable(MY_FOG);

}
