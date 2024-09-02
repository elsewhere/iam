#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Worm.hpp"

void Worm::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.90f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
	write(320,    240, pos, 0.2f, 0.5f, alpha, "luontofontti.pcx", "I am the way");
	write(320-16, 275, pos, 0.25f, 0.55f, alpha, "luontofontti.pcx", "and the truth");
	write(320   , 310, pos, 0.3f, 0.6f, alpha, "luontofontti.pcx", "and the life");
	write(96    , 380, pos, 0.5f, 0.8f, alpha, "luontofontti.pcx", "no one comes to the future");
	write(368   , 415, pos, 0.55f, 0.85f, alpha, "luontofontti.pcx", "but by me");
}

void Worm::drawFlare(Vector &pos, float size)
{
	glBegin(GL_QUADS);
	Vector v1 = pos + Vector(-size, -size, 0);
	Vector v2 = pos + Vector(size, -size, 0);
	Vector v3 = pos + Vector(size, size, 0);
	Vector v4 = pos + Vector(-size, size, 0);

	glTexCoord2f(0, 0);
	glVertex3fv((float *)&v1);
	glTexCoord2f(1, 0);
	glVertex3fv((float *)&v2);
	glTexCoord2f(1, 1);
	glVertex3fv((float *)&v3);
	glTexCoord2f(0, 1);
	glVertex3fv((float *)&v4);
	glEnd();

}



void Worm::renderScene(float pos, float alpha)
{
	Matrix rotate, globalrotate;
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//	myBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE);
//	myBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA); 
	myBlendFunc(GL_SRC_ALPHA, GL_ONE); 
	myEnable(MY_LINE_SMOOTH);

/*
	myEnable(MY_TEXTURE_2D);
	Texture *tausta = dmsGetTexture("tosinerokas"); 
	glBindTexture(GL_TEXTURE_2D, tausta->getID());
	glLoadIdentity();
	dmsPerspective2D(640, 480); 
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK); 
	glBegin(GL_QUADS);

	const float tausta_bright = 1.0f;
	glColor3f(alpha*tausta_bright, alpha*tausta_bright, alpha*tausta_bright); 
	glTexCoord2f(0, 0);
	glVertex2f(0, 0); 
	glTexCoord2f(1, 0);
	glVertex2f(640, 0); 
	glTexCoord2f(1, 1);
	glVertex2f(640, 480); 
	glTexCoord2f(0, 1);
	glVertex2f(0, 480); 
	glEnd();
	dmsPerspective3D();

	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);

*/
	/*	glLoadIdentity();
	dmsPerspective2D(640, 480);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);
	glColor4f(1,1,1,1);
	myDisable(MY_TEXTURE_2D); 
	myDisable(MY_BLEND); 
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(640, 0);
	glVertex2f(640, 480);
	glVertex2f(0, 480);
	glEnd();
	dmsPerspective3D();
*/	
	glLoadIdentity();
	glTranslatef(0, 0, -3.0f);

	float table[512];
	memcpy(table, dmsGetSpectrum(), 512*sizeof(float));
	float globalmusic = 0.0f;
	for (int i=0;i<512;i++)
		globalmusic += table[i]*table[i]*table[i];
	globalmusic *= 0.01f; 

	const float size = 3.0f;
	float xpos = -size;
	float ypos = 0.3f;

	myDisable(MY_DEPTH_TEST);	
	myDisable(MY_DEPTH_MASK);

	Texture *texture = dmsGetTexture("flare.pcx");
	
	glBindTexture(GL_TEXTURE_2D, texture->getID());

	for (i=0;i<joints;i++)
	{
		const float phase = (float)i/(float)joints;
		const float scale = 10.0f;
		const int wrap = 1;
		const float speed = 6666.6f; 
		float musicvalue = globalmusic+table[(i*wrap+(int)(speed*pos))&511]*scale;
		rotate.makeRotation(musicvalue, (float)sin(phase*2*3.14152f + pos*50)*0.7f, (float)sin(phase*4*3.14152f + pos*120));
		Vector v1 = (Vector(xpos, 
							 (float)cos(phase*2*3.141592f+pos*16)*0.4f, 
							 (float)sin(phase*3*3.141592f+pos*7)*1.83f));
		Vector v2 = v1 + Vector(-0.015f, ypos, 0)*rotate;
		Vector v3 = v1 + Vector(0.015f, ypos, 0)*rotate;
		Vector v4 = v1 + Vector(-0.015f, -ypos, 0)*rotate;
		Vector v5 = v1 + Vector(0.015f, -ypos, 0)*rotate;
		myDisable(MY_TEXTURE_2D);
		myEnable(MY_BLEND);
		myDisable(MY_DEPTH_TEST);
		const float wormalphaclip = 0.7f;
		float wormalpha = alpha*0.6f*(musicvalue+0.1f);
		if (wormalpha > wormalphaclip) 
			wormalpha = wormalphaclip;
		glColor4f(1,1,1,wormalpha*0.6f);
		glBegin(GL_TRIANGLES);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v4.x, v4.y, v4.z);
		glVertex3f(v5.x, v5.y, v5.z);
		glEnd();
		myEnable(MY_TEXTURE_2D);
		
		glColor4f(1,1,1,wormalpha);
		float flaresize = (musicvalue+0.60f)*0.1f;
		const float flareclipsize = 0.30f;
		if (flaresize > flareclipsize)
			flaresize = flareclipsize;

		drawFlare(v2, flaresize);
		drawFlare(v3, flaresize);
		drawFlare(v4, flaresize);
		drawFlare(v5, flaresize);
//		Primitives::drawFlare(&v2, flaresize);
//		Primitives::drawFlare(&v3, flaresize);
//		Primitives::drawFlare(&v4, flaresize);
//		Primitives::drawFlare(&v5, flaresize);
		xpos += 2*size / joints;
	}

	myEnable(MY_TEXTURE_2D);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
}

Worm::Worm()
{	
	joints = 384;
}

Worm::~Worm()
{
}

bool Worm::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	
	return true;
}

