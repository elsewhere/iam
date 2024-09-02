#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include "Potential.hpp"

void Potential::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.83f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
//	Primitives::textWriter(10, 450, pos, 25, 0.5f, 0.9f, alpha, "Traction 2003");

//	write(160, 230, pos, 0.05f, 0.11f, alpha, "teknofontti.pcx", "i give you my word");
	write(160, 230, pos, 0.05f, 0.10f, alpha, "teknofontti.pcx", "i");// give you my word");
	write(760, 290, pos, 0.06f, 0.11f, alpha, "teknofontti.pcx", "give");// give you my word");
	write(360, 380, pos, 0.07f, 0.12f, alpha, "teknofontti.pcx", "you");// give you my word");
	write(160, 700, pos, 0.075f, 0.13f, alpha, "teknofontti.pcx", "my");// give you my word");
	write(560, 630, pos, 0.08f, 0.14f, alpha, "teknofontti.pcx", "word");// give you my word");

	write(60, 30, pos, 0.4f, 0.7f, alpha, "luontofontti.pcx", "my heart are the solar flares");
	write(80, 60, pos, 0.45f, 0.75f, alpha, "luontofontti.pcx", "my veins are the rivers");
	write(100, 90, pos, 0.50f, 0.8f, alpha, "luontofontti.pcx", "my mouth are the trees");

}


void Potential::renderScene(float pos, float alpha)
{
	glLoadIdentity();
	glTranslatef(0, 0, -3.0f);

	myDisable(MY_TEXTURE_2D);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_BLEND);
	myEnable(MY_LINE_SMOOTH);
	
	//glDepthMask(GL_FALSE);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);

	const float speed = 0.7f;

	Vector position = Vector((float)sin((pos*15+0.3f)*speed), 0.4f*(float)cos((pos*21+0.5f)*speed), 0.6f*(float)sin(cos((pos*11)*speed)));
	Vector position2 = Vector((float)cos((pos*7+4)*speed), 0.4f*(float)cos((pos*5+2)*speed), 0.7f*(float)sin(sin((pos*13)*speed)));
	Vector position3 = Vector((float)sin((pos*9+2)*speed), 0.1f*(float)sin((pos*5+2.5f)*speed)*0, 0.4f*(float)sin((pos*9+3)*speed));

	Matrix rotation, rotation2, rotation3;
	rotation.makeRotation(pos*10*speed, -pos*15*speed, -pos*7.4f*speed);
	rotation2.makeRotation(pos*7*speed, pos*6*speed, pos*3.7f*speed);
	rotation3.makeRotation(-pos*4*speed, pos*11*speed, -pos*5.4f*speed);

	renderObject(position, rotation,  pos, 0.9f, 0.6f, 0.3f, alpha*0.32f);
	renderObject(position2, rotation2, pos, 0.9f, 0.6f, 0.2f, alpha*0.42f);
	renderObject(position3, rotation3, pos, 0.5f, 0.8f, 0.4f, alpha*0.3f);

	myDisable(MY_BLEND);
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
}

void Potential::renderObject(Vector &position, Matrix &rotation, float time, float r, float g, float b, float a)
{
	int i;

	Texture *flare = dmsGetTexture("flare.pcx");

	for (i=0;i<res*res*res;i++)
	{
		const float phase = (float)i/(float)(res*res*res);
		Vector p = kappale[i];
		p *= rotation;
		p += position;
		glColor4f(r, g, b, a);

		float val = function(p.x, p.y, p.z);
		glPushMatrix();
		glTranslatef(p.x, p.y, p.z); 
		myEnable(MY_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, flare->getID());

		glBegin(GL_QUADS);
		const float size = val*val*val*0.1f;
		glTexCoord2f(0, 0);
		glVertex3f(-size, -size, 0);
		glTexCoord2f(1,0);
		glVertex3f(size, -size, 0);
		glTexCoord2f(1,1);
		glVertex3f(size, size, 0);
		glTexCoord2f(0,1);
		glVertex3f(-size, size, 0);

		glEnd();
//		Primitives::drawFlare(&Vector(0,0,0), val*val*val*0.1f);
		myDisable (MY_TEXTURE_2D);
		myEnable(MY_BLEND);
		glRotatef(time*460, 1, 1, 1);
		glColor4f(r, g, b, a*0.8f);
//		Primitives::wireCube(&Vector(0,0,0), val*0.04f);
		glPopMatrix();
	}
}

float Potential::function(float x, float y, float z)
{
//	float val = (float)(sin(x*6*3.141592f)*y + sin(y*4*3.141592f)*y -sin(z*5));
	float val = (float)(sin(x*4*3.141592f)*y + sin(y*3*3.141592f)*y -sin(z*3.5f));
	return val;

}

Potential::Potential()
{	
	int x, y, z;
	res = 6;
	kappale = new Vector[res*res*res];

	float xpos, ypos, zpos;
	const float step = 1.0f / res;

	zpos = -0.5f;
	Vector *v = kappale;
	for (z=0;z<res;z++)
	{
		ypos = -0.5f;
		for (y=0;y<res;y++)
		{
			xpos = -0.5f;
			for (x=0;x<res;x++)
			{
				v->x = xpos;
				v->y = ypos;
				v->z = zpos;
				xpos += step;
				v++;
			}
			ypos += step;
		}
		zpos += step;
	}

}

Potential::~Potential()
{
	delete [] kappale;
}

bool Potential::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

