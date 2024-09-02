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

	myDisable(MY_FOG);
	myDisable(MY_TEXTURE_CUBE_MAP_EXT);
	myDisable(MY_TEXTURE_GEN_S); 
	myDisable(MY_TEXTURE_GEN_T); 
	myDisable(MY_TEXTURE_GEN_R); 		
	myDisable(MY_LIGHTING);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_CULL_FACE);
	myEnable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);		
	myEnable(MY_FOG);

	float alpha = 1.0f;
	const float pos = (time - startTime) / (endTime - startTime);
	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.5f;
	
	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= ((pos-fadeinstart) / (fadeinstop-fadeinstart));
		
	glBindTexture(GL_TEXTURE_2D, texture->getID());
		
	myBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
	glLoadIdentity();
	
	glTranslatef(6.0f, 0.0f, -50.0f + pos * 50.0f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef((float)sin(pos * 2 * 3.1415926) * 2.5f, 0.0f, 1.0f, 0.0f);
	
	glBegin(GL_QUADS);

		glTexCoord2f(0, 0); glVertex3f(-9,  6, 0);
		glTexCoord2f(1, 0); glVertex3f( 9,  6, 0);
		glTexCoord2f(1, 1); glVertex3f( 9, -6, 0);
		glTexCoord2f(0, 1); glVertex3f(-9, -6, 0);

	glEnd();

	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glLoadIdentity();
	
	glTranslatef(5.0f, 0.0f, -50.0f + pos * 50.0f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef((float)sin(pos * 2 * 3.1415926) * 2.5f, 0.0f, 1.0f, 0.0f);
	
	glColor4f(1.0f, 1.0f, 1.0f, alpha);

	glBegin(GL_QUADS);

		glTexCoord2f(0, 0); glVertex3f(-9,  6, 0);
		glTexCoord2f(1, 0); glVertex3f( 9,  6, 0);
		glTexCoord2f(1, 1); glVertex3f( 9, -6, 0);
		glTexCoord2f(0, 1); glVertex3f(-9, -6, 0);

	glEnd();

  	myDisable(MY_FOG);

	write(30, 30, pos, 0.25f, 0.45f, alpha, "teknofontti.pcx", "i am watching you");
	write(30, 62, pos, 0.35f, 0.55f, alpha, "teknofontti.pcx", "i know what you need");

	write(62, 530, pos, 0.65f, 0.85f, alpha, "luontofontti.pcx", "you know nothing");
	write(94, 562, pos, 0.75f, 0.95f, alpha, "luontofontti.pcx", "you are hollow");

	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);

}
