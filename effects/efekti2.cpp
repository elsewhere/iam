//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "efekti2.hpp"

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Efekti2::Efekti2()
{
}

Efekti2::~Efekti2()
{
	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Efekti2::init(unsigned long s, unsigned long e)
{	
	active = false;
	startTime = s;
	endTime = e;

	return true;
}

void Efekti2::draw()
{	
	Texture *texture = dmsGetTexture("target2");
	Texture *t = dmsGetTexture("visual11.pcx");
	if(!texture || !t) return;

	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_CULL_FACE);
	myDisable(MY_BLEND);

	glViewport(0, 0, texture->getWidth(), texture->getHeight());

		glLoadIdentity();
		glTranslatef(0, 0, -160.0f);
		glRotatef(time * 0.01f, 1.0f, 0.5f, 0.25f);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t->getID());
		dmsDrawObject("sphere.t3d");

	glBindTexture(GL_TEXTURE_2D, texture->getID());
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, texture->getWidth(), texture->getHeight(), 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, dmsGetWindowWidth(), dmsGetWindowHeight());

}

