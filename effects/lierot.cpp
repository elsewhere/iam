//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "lierot.hpp"

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Lierot::Lierot()
{
}

Lierot::~Lierot()
{
	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------
bool Lierot::init(unsigned long s, unsigned long e)
{	
	int i;	

	active = false;
	startTime = s;
	endTime = e;	

	// PCX katos minne???
	flare = dmsGetTexture("particle.pcx");
	if(!flare) return false;

	srand(2);

	for(i = 0; i < MAXLIEROT; i ++)
	{
		liero[i].speed = ((float)(rand()%1024 - rand()%1024)) / 1024.0f;
		liero[i].nx = ((float)(rand()%1024 - rand()%1024)) / 1024.0f;
		liero[i].ny = ((float)(rand()%1024 - rand()%1024)) / 1024.0f;
		liero[i].nz = ((float)(rand()%1024 - rand()%1024)) / 1024.0f;
	}

	myEnable(MY_LINE_SMOOTH);
			
	return true;
}

void Lierot::draw()
{
	int i, j;

	glLoadIdentity();
	glTranslatef(5.5f, -4.5f, -10.0f);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glColor4f(0.35f, 0.75f, 1.0f, 0.95f);
	myEnable(MY_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, flare->getID());

	glPushMatrix();
	glRotatef(time*0.05f, 1.0f, 0.0f, 0.0f);

		glBegin(GL_TRIANGLE_STRIP);
										
			glTexCoord2f(1, 1); glVertex3f(-2.0f, -2.0f, 0.0f);
			glTexCoord2f(0, 1); glVertex3f( 2.0f, -2.0f, 0.0f);
			glTexCoord2f(1, 0); glVertex3f(-2.0f,  2.0f, 0.0f);
			glTexCoord2f(0, 0); glVertex3f( 2.0f,  2.0f, 0.0f);

		glEnd();

	glPopMatrix();

	glPushMatrix();
	glRotatef(time*0.025f, 0.0f, 1.0f, 0.0f);

		glBegin(GL_TRIANGLE_STRIP);
										
			glTexCoord2f(1, 1); glVertex3f(-2.0f, -2.0f, 0.0f);
			glTexCoord2f(0, 1); glVertex3f( 2.0f, -2.0f, 0.0f);
			glTexCoord2f(1, 0); glVertex3f(-2.0f,  2.0f, 0.0f);
			glTexCoord2f(0, 0); glVertex3f( 2.0f,  2.0f, 0.0f);

		glEnd();

	glPopMatrix();

	glPushMatrix();
	glRotatef(time*0.01f, 0.0f, 0.0f, 1.0f);

		glBegin(GL_TRIANGLE_STRIP);
										
			glTexCoord2f(1, 1); glVertex3f(-2.0f, -2.0f, 0.0f);
			glTexCoord2f(0, 1); glVertex3f( 2.0f, -2.0f, 0.0f);
			glTexCoord2f(1, 0); glVertex3f(-2.0f,  2.0f, 0.0f);
			glTexCoord2f(0, 0); glVertex3f( 2.0f,  2.0f, 0.0f);

		glEnd();

	glPopMatrix();

	for(j = 0; j < MAXLIEROT; j++)
	{
		Vector lastPos;

		glPushMatrix();

			myDisable(MY_TEXTURE_2D);
		
			glColor4f(0.25f, 0.5f, 1.0f, 0.35f);
			glBegin(GL_LINE_STRIP);
						
				for(i = 0; i < MAXJOINTS; i++)
				{			
					float mul = float(i) / float(MAXJOINTS);
					float val  = mul*float(sin(mul*liero[j].speed*3.1415926f+time*0.001f)*2.0f+cos(mul*liero[j].speed*3.1415926f+time*0.001f*0.15f)*2.0f);
					float val2 = mul*float(sin(mul*liero[j].speed*3.1415926f+time*0.001f*0.23f)*2.0f-cos(mul*liero[j].speed*3.1415926f+time*0.001f*0.05f)*2.0f);
					float val3 = mul*float(sin(mul*liero[j].speed*3.1415926f+time*0.001f*0.45f)*2.0f+cos(mul*liero[j].speed*3.1415926f+time*0.001f)*2.0f);
					glVertex3f(liero[j].nx*i+val, liero[j].ny*i+val2, liero[j].nz*i+val3);

					lastPos = Vector(liero[j].nx*i+val, liero[j].ny*i+val2, liero[j].nz*i+val3);
				}

			glEnd();

			glColor4f(0.25f, 0.5f, 1.0f, 0.35f);

			myEnable(MY_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, flare->getID());

			glBegin(GL_TRIANGLE_STRIP);
										
					glTexCoord2f(1, 1); glVertex3f(lastPos.x - 0.8f, lastPos.y - 0.8f, lastPos.z);
					glTexCoord2f(0, 1); glVertex3f(lastPos.x + 0.8f, lastPos.y - 0.8f, lastPos.z);
					glTexCoord2f(1, 0); glVertex3f(lastPos.x - 0.8f, lastPos.y + 0.8f, lastPos.z);
					glTexCoord2f(0, 0); glVertex3f(lastPos.x + 0.8f, lastPos.y + 0.8f, lastPos.z);

			glEnd();

		glPopMatrix();
	}

	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);	
}

