//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "noise.hpp"

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Noise::Noise()
{
	data = NULL;
	texture = NULL;
}

Noise::~Noise()
{
	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Noise::init(unsigned long s, unsigned long e)
{	
	active = false;
	startTime = s;
	endTime = e;
		
	texture = dmsGetTexture("global");
	data = new unsigned int[256*256];
	if(!data || !texture)
	{
		dmsMsg("Noise::init error: Memory allocation error\n");
		return false;
	}

	memset(data, 0, 256*256*sizeof(unsigned int));
	
	return true;
}

void Noise::draw()
{	
	float w, h;
	unsigned char *ptr = (unsigned char *)data;

	w = (float)dmsGetWindowWidth();
	h = (float)dmsGetWindowHeight();

	for(int y = 0; y < 256; y++)
	{
		for(int x = 0; x < 256; x++)
		{
			unsigned char c = rand()%255;
			*(ptr ++) = c;
			*(ptr ++) = c;
			*(ptr ++) = c;
			*(ptr ++) = c;
		}
	}

	glBindTexture(GL_TEXTURE_2D, texture->getID());
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
			
	dmsPerspective2D(dmsGetWindowWidth(), dmsGetWindowHeight());	

	glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(1, 0); glVertex2f(w, 0);
		glTexCoord2f(1, 1);	glVertex2f(w, h);
		glTexCoord2f(0, 1);	glVertex2f(0, h);
	glEnd();	

	dmsPerspective3D();

	myDisable(MY_BLEND);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
}

void Noise::release()
{
	if(data)
	{
		delete [] data;
		data = NULL;
	}
}

