//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>
#include "efekti3.hpp"

#define DOTS 40

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Efekti3::Efekti3()
{
}

Efekti3::~Efekti3()
{
	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Efekti3::init(unsigned long s, unsigned long e)
{	
	active = false;
	startTime = s;
	endTime = e;

	texture = dmsGetTexture("dotfade");
	dot = dmsGetTexture("flare.pcx");
	dots = new Vector[DOTS];
	if(!texture || !dot)
	{
		dmsMsg("Efekti3::init() error: Couldn't get textures");
		return false;
	}

	for(int i = 0; i < DOTS; i++)
	{
		dots[i].x = float(rand()%12 - rand()%12);
		dots[i].y = float(rand()%12 - rand()%12);
		dots[i].z = 0.0f;
	}

	return true;
}

void Efekti3::draw()
{			
	glBindTexture(GL_TEXTURE_2D, dot->getID());
	drawDots();
}

void Efekti3::drawHole(float x, float y)
{		
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);	
	myDisable(MY_CULL_FACE);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	glLoadIdentity();
	glTranslatef( x, y, -20.0f);
		
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBegin(GL_TRIANGLE_STRIP);									
		glTexCoord2f(1, 1); glVertex3f(-2.0f, -2.0f, 0.0f);
		glTexCoord2f(0, 1); glVertex3f( 2.0f, -2.0f, 0.0f);
		glTexCoord2f(1, 0); glVertex3f(-2.0f,  2.0f, 0.0f);
		glTexCoord2f(0, 0); glVertex3f( 2.0f,  2.0f, 0.0f);
	glEnd();

	myEnable(MY_TEXTURE_2D);
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
}

void Efekti3::drawDots()
{
	float w, h;
	float alpha = 1.0f;
	const float pos = (time - startTime) / (endTime - startTime);
	const float fadeinstart = 0.9f;
	const float fadeinstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= ((pos-fadeinstart) / (fadeinstop-fadeinstart));

	w = (float)dmsGetWindowWidth();
	h = (float)dmsGetWindowHeight();	

	glViewport(0, 0, texture->getWidth(), texture->getHeight());

	for(int i = 0; i < DOTS*pos*4; i++)
	{		
		drawHole(dots[i].x, dots[i%DOTS].y);		
	}

	dmsPerspective2D(w, h);
	glBindTexture(GL_TEXTURE_2D, 0);

	glColor4f(pos, pos, pos, pos);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(1, 0); glVertex2f(w, 0);
		glTexCoord2f(1, 1); glVertex2f(w, h);
		glTexCoord2f(0, 1); glVertex2f(0, h);
	glEnd();

	dmsPerspective3D();
	
	glBindTexture(GL_TEXTURE_2D, texture->getID());
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, texture->getWidth(), texture->getHeight(), 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, dmsGetWindowWidth(), dmsGetWindowHeight());
}

