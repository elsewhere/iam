//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "fadein.hpp"

#define GRIDWIDTH 20
#define GRIDHEIGHT 20

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

FadeIn::FadeIn()
{	
	texture = NULL;
	grid = NULL;
}

FadeIn::~FadeIn()
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

bool FadeIn::init(unsigned long s, unsigned long e)
{	
	active = false;
	startTime = s;
	endTime = e;

	grid = new GridPoint2D[(GRIDWIDTH+1)*(GRIDHEIGHT+1)];

	texture = dmsGetTexture("nousu.jpg");	
	if(!texture || !grid)
	{
		dmsMsg("FadeIn::init error: Couldn't get 3D-objects or textures\n");
		return false;
	}

	memset(grid, 0, (GRIDWIDTH)*(GRIDHEIGHT)*sizeof(GridPoint2D));
		
	return true;
}

void FadeIn::draw()
{
	int x, y, width, height;
	float xStep, yStep;
	float u, v, u1, v1, u2, v2, u3, v3;
	GridPoint2D *ptr = grid;

	const float pos = (time - startTime) / (endTime - startTime);

	//dmsMsg("pos = %f\n", pos); 

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
								
			//ptr->u = (float)(x*uStep*(cos( (len*0.75f-0)*(1.0f-pos) )-sin( (float(x) / float(GRIDWIDTH)) * 2 * 3.1415926 / 90 ))*pos+0.5f);
			ptr->u = (float)(x*uStep*(cos( (len*0.75f-0)*(1.0f-pos) )-sin( (float(x) / float(GRIDWIDTH)) / 90 )) + 0.5f);
			ptr->v = 1-(float)(y*uStep*(cos( (float(y) / float(GRIDHEIGHT)) * len * 0.25f * 3.1415926 / 90 ))*pos+0.5f);
			
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
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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



