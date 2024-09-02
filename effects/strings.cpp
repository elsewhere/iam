//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "strings.hpp"

#define MAXSTRINGS 6
#define MAXJOINTS 40

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

String::String()
{
	joint = NULL;
}

String::~String()
{
	if(joint)
	{
		delete [] joint;
		joint = NULL;
	}
}

Strings::Strings()
{
	string = NULL;
}

Strings::~Strings()
{
	if(string)
	{
		delete [] string;
		string = NULL;
	}
}

//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Strings::init(unsigned long s, unsigned long e)
{
	int i, j;

	startTime = s;
	endTime = e;
	active = false;

	string = new String[MAXSTRINGS];
	if(!string)
	{
		dmsMsg("Strings::init error: Couldn't allocate memory\n");
		return false;
	}

	for(i = 0; i < MAXSTRINGS; i++)
	{
		String *ptr = &string[i];
		Joint *jtr = NULL;
		
		ptr->joint = new Joint[MAXJOINTS];
		if(!ptr->joint)
		{
			dmsMsg("Strings::init error: Couldn't allocate memory\n");
			return false;
		}
		
		ptr->position = Vector(0.0f, 0.0f, -10.0f);
		ptr->value = (rand()%255 - rand()%255) / 128.0f;
		ptr->value2 = (rand()%255 - rand()%255) / 64.0f;
		ptr->value3 = (rand()%255 - rand()%255) / 128.0f;
		ptr->value4 = (rand()%255 - rand()%255) / 64.0f;
		for(j = 0; j < MAXJOINTS; j++)
		{
			jtr = &ptr->joint[j];

			jtr->position = Vector(10.0f-j*0.5f, 0.0f, 0.0f);			
		}
	}

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	return true;
}

void Strings::draw()
{	
	int i, j;
	bool done = false;	
	String *str = NULL;
	Joint *jtr = NULL;

	const float pos = (time - startTime) / (endTime - startTime);

	// Nauhat blendaa taustalla olevaa mustaksi at the moment.
	// ks. myBlendFunc
	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);
	myBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

	glLoadIdentity();

	glColor4f(1.0f, 1.0f, 1.0f, 0.75f);

	for(i = 0; i < MAXSTRINGS; i++)
	{
		str = &string[i];	 						
				
		for(j = 0; j < MAXJOINTS; j++)
		{						
			float tmp = (float)sin(((float)j/(float)MAXJOINTS)*str->value*3*3.141592f+time*0.001f*str->value2)*1.0f+(float)cos(((float)j/(float)MAXJOINTS)*str->value3*2*3.141592f+time*0.001f*str->value4)*1.0f;
			str->joint[j].position.y = tmp;			
		}

		glPushMatrix();
		glTranslatef(str->position.x, str->position.y, str->position.z);

		glBegin(GL_TRIANGLE_STRIP);

		for(j = 0; j < MAXJOINTS; j++)
		{
			jtr = &str->joint[j];			
						
				Vector pos;				

				pos = jtr->position;	
				glVertex3f(pos.x, pos.y, pos.z);
				glVertex3f(pos.x, pos.y-1.0f+((float)j/(float)MAXJOINTS)*1.0f, pos.z);
		}

		glEnd();			

		glPopMatrix();
	}

	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);	
}

