//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "ripple2.hpp"

#define GRIDWIDTH 40
#define GRIDHEIGHT 40

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Ripple2::Ripple2()
{	
	texture = NULL;
	grid = NULL;
}

Ripple2::~Ripple2()
{
	if(grid)
	{
		delete [] grid;
		grid = NULL;
	}

	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Ripple2::init(unsigned long s, unsigned long e)
{	
	active = false;
	startTime = s;
	endTime = e;

	grid = new GridPoint2[(GRIDWIDTH+1)*(GRIDHEIGHT+1)];

	texture = dmsGetTexture("target2");	
	if(!texture || !grid)
	{
		dmsMsg("Ripple2::init error: Couldn't get 3D-objects or textures\n");
		return false;
	}

	memset(grid, 0, (GRIDWIDTH)*(GRIDHEIGHT)*sizeof(GridPoint2));
		
	return true;
}

void Ripple2::draw()
{
	int x, y, width, height;
	float xStep, yStep;
	float u, v, u1, v1, u2, v2, u3, v3;
	GridPoint2 *ptr = grid;

	const float pos = (time - startTime) / (endTime - startTime);

	width = dmsGetWindowWidth();
	height = dmsGetWindowHeight();
	xStep = (float)width / float(GRIDWIDTH);
	yStep = (float)height / float(GRIDHEIGHT);
	
	for(y = -(GRIDHEIGHT/2); y < (GRIDHEIGHT/2)+1; y++)
	{
		for(x = -(GRIDWIDTH/2); x < (GRIDWIDTH/2)+1; x++)
		{			
			float len = (float)sqrt(x*x+y*y);
			float uStep = (1.0f / float(GRIDWIDTH));
			float vStep = (1.0f / float(GRIDHEIGHT));

			//ptr->u = (float)(x*uStep*cos(len*0.75f*sin(pos*3.1415926f)+(0.5f+sin(time*0.0002f)*0.5f)*5.0f)+0.5f);			
			//ptr->v = (float)(y*vStep*cos(len*0.75f*sin(pos*3.1415926f)+(0.5f+sin(time*0.0002f)*0.5f)*5.0f)+0.5f);			
			
			//ptr->u = (float)(x*uStep*cos( (len*0.75f-time*0.001f)*(1.0f-pos) )*pos+0.5f);
			//ptr->v = (float)(y*vStep*cos( (len*0.75f-time*0.001f)*(1.0f-pos) )*pos+0.5f);

			ptr->u = (float)(x*uStep*(cos( (len*0.35*(1.0f-pos)-(1.0f-pos)*time*0.001f)*(1.0f-pos)*5 ))+0.5f);
			ptr->v = (float)(y*vStep*(cos( (len*0.35*(1.0f-pos)-(1.0f-pos)*time*0.001f)*(1.0f-pos)*5 ))+0.5f);
			//ptr->u = (x+GRIDWIDTH/2)*uStep;
			//ptr->v = (y+GRIDHEIGHT/2)*vStep;
			*ptr++;
		}
	}

	dmsPerspective2D(width, height);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_CULL_FACE);
	myEnable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glBindTexture(GL_TEXTURE_2D, texture->getID());
	glColor4f(1.0f, 1.0f, 1.0f, pos);
	
	glBegin(GL_QUADS);

	ptr = grid;
	for(y = -(GRIDHEIGHT/2); y < (GRIDHEIGHT/2)+1; y++)
	{
		for(x = -(GRIDWIDTH/2); x < (GRIDWIDTH/2)+1; x++)
		{	
			float xpos = x*xStep + (float)width/2.0f;
			float ypos = y*yStep + (float)height/2.0f;

			u = ptr->u;
			v = ptr->v;
			u1 = (ptr+1)->u;
			v1 = (ptr+1)->v;
			u2 = (ptr+1+GRIDWIDTH+1)->u;
			v2 = (ptr+1+GRIDWIDTH+1)->v;
			u3 = (ptr+GRIDWIDTH+1)->u;
			v3 = (ptr+GRIDWIDTH+1)->v;
			glTexCoord2f(u , v ); glVertex2f(xpos		, ypos		);			
			glTexCoord2f(u1, v1); glVertex2f(xpos+xStep	, ypos		);		
			glTexCoord2f(u2, v2); glVertex2f(xpos+xStep	, ypos+yStep);
			glTexCoord2f(u3, v3); glVertex2f(xpos		, ypos+yStep);

			*ptr++;
		}
	}

	glEnd();

	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
	dmsPerspective3D();
}



