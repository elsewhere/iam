//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "tilpe2.hpp"

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Tilpe2::Tilpe2()
{

}

Tilpe2::~Tilpe2()
{
	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Tilpe2::init(unsigned long s, unsigned long e)
{	
	active = false;
	startTime = s;
	endTime = e;
	
	return true;
}

void Tilpe2::draw()
{
	Texture *texture = dmsGetTexture("flare.pcx");
	unsigned int w, h, x, y, offset = 0;	
	const float pos = (time - startTime) / (endTime - startTime);

	if(!texture) return;

	w = dmsGetWindowWidth();
	h = dmsGetWindowHeight();
	
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myBlendFunc(GL_DST_COLOR, GL_ONE);

	dmsPerspective2D(w, h);

	glBindTexture(GL_TEXTURE_2D, texture->getID());

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	glBegin(GL_QUADS);
	
		for(y = 0; y < h; y+= 16)
		{
			for(x = 0; x < w; x += 16)
			{				
				glTexCoord2f(0, 0); glVertex2i(   x, y);
				glTexCoord2f(1, 0); glVertex2i(x+16, y);
				glTexCoord2f(1, 1); glVertex2i(x+16, y+16);
				glTexCoord2f(0, 1); glVertex2i(   x, y+16);
				offset++;
			}
		}

	glEnd();

	dmsPerspective3D();
	
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
}
