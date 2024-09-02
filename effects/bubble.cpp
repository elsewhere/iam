#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Bubble.hpp"

void Bubble::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.97f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
//	write3(100, 100, 1, pos, 0.2f, 0.7f, alpha, "teknofontti.pcx", "hello world");

	const float ylasaato = 32;
	const float alasaato = 32;
	const float sync = 0.05f;
	const float sync2 = 0.08f;

	if(GetAsyncKeyState(VK_SPACE))
	{
		dmsMsg("Sync %f\n", pos);
	}

	//0.3 / 8 
	//0.4 / 8 = 0.05
	// 0.042	
/*
	write3(512-5.5f*32, 128-ylasaato, 1, pos, 0.203125f, 1.0f, alpha, "teknofontti.pcx", "in my world");
	write3(512-7*32,    128+32-ylasaato, 1, pos, 0.261750f, 1.0f, alpha, "teknofontti.pcx", "there is order");
	write3(512-5.5f*32, 128+64-ylasaato, 1, pos, 0.320125f, 1.0f, alpha, "teknofontti.pcx", "in my world");
	write3(512-8.5f*32, 128+96-ylasaato, 1, pos, 0.380083f, 1.0f, alpha, "teknofontti.pcx", "the path is clear");
	write3(512-5.5f*32, 128+128-ylasaato, 1, pos, 0.442625f, 1.0f, alpha, "teknofontti.pcx", "in my world");
	write3(512-8.5f*32, 128+160-ylasaato, 1, pos, 0.505833f, 1.0f, alpha, "teknofontti.pcx", "the path is order");
	write3(512-5.5f*32, 128+192-ylasaato, 1, pos, 0.569042f, 1.0f, alpha, "teknofontti.pcx", "in my world");
	write3(512-7.5f*32, 128+224-ylasaato, 1, pos, 0.622250f, 1.0f, alpha, "teknofontti.pcx", "there is no lie");
*/

	write3(512-5.5f*32, 128-ylasaato, 1, pos, 0.1f+sync, 0.15f+sync2, alpha, "teknofontti.pcx", "in my world");
	write3(512-7*32,    128+32-ylasaato, 1, pos, 0.15f+sync, 0.2f+sync2, alpha, "teknofontti.pcx", "there is order");
	write3(512-5.5f*32, 128+64-ylasaato, 1, pos, 0.2f+sync, 0.25f+sync2, alpha, "teknofontti.pcx", "in my world");
	write3(512-8.5f*32, 128+96-ylasaato, 1, pos, 0.25f+sync, 0.3f+sync2, alpha, "teknofontti.pcx", "the path is clear");
	write3(512-5.5f*32, 128+128-ylasaato, 1, pos, 0.3f+sync, 0.35f+sync2, alpha, "teknofontti.pcx", "in my world");
	write3(512-8.5f*32, 128+160-ylasaato, 1, pos, 0.35f+sync, 0.4f+sync2, alpha, "teknofontti.pcx", "the path is order");
	write3(512-5.5f*32, 128+192-ylasaato, 1, pos, 0.4f+sync, 0.45f+sync2, alpha, "teknofontti.pcx", "in my world");
	write3(512-7.5f*32, 128+224-ylasaato, 1, pos, 0.45f+sync, 0.5f+sync2, alpha, "teknofontti.pcx", "there is no lie");

	write3(512-6.5f*32, 128+256+alasaato, 1, pos, 0.5f+sync, 0.55f+sync2, alpha, "luontofontti.pcx", "in your world");
	write3(512-7*32,    128+32+256+alasaato, 1, pos, 0.55f+sync, 0.6f+sync2, alpha, "luontofontti.pcx", "there is order");
	write3(512-6.5f*32, 128+64+256+alasaato, 1, pos, 0.6f+sync, 0.65f+sync2, alpha, "luontofontti.pcx", "in your world");
	write3(512-8.5f*32, 128+96+256+alasaato, 1, pos, 0.65f+sync, 0.7f+sync2, alpha, "luontofontti.pcx", "the path is clear");
	write3(512-6.5f*32, 128+128+256+alasaato, 1, pos, 0.7f+sync, 0.75f+sync2, alpha, "luontofontti.pcx", "in your world");
	write3(512-9.0f*32, 128+160+256+alasaato, 1, pos, 0.75f+sync, 0.8f+sync2, alpha, "luontofontti.pcx", "the path is narrow");
	write3(512-6.5f*32, 128+192+256+alasaato, 1, pos, 0.8f+sync, 0.85f+sync2, alpha, "luontofontti.pcx", "in your world");
	write3(512-7.5f*32, 128+224+256+alasaato, 1, pos, 0.85f+sync, 0.9f+sync2, alpha, "luontofontti.pcx", "there is no why");

	//in my world
	//there is order
	//in my world
	//the path is clear
	//in my world
	//the path is order
	//in my world
	//there is no lie

	//in your world
	//there is order
	//in your world
	//the path is clear
	//in your world
	//the path is narrow
	//in your world
	//there is no why
}


void Bubble::renderScene(float pos, float alpha)
{
	int i;
	const float radius_start = 0.0f;
	const float radius_stop = 0.15f;

	float radiuspos = 0.0f;
	if (pos > radius_start && pos <= radius_stop)
	{
		radiuspos = (pos - radius_start) / (radius_stop - radius_start); 
	}
	else if (pos > radius_stop)
		radiuspos = 1.0f; 

	radiuspos = (float)sin(radiuspos*3.141592f*0.5f); 
/*
	glLoadIdentity();
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);

	Texture *tausta = dmsGetTexture("eye");
	glBindTexture(GL_TEXTURE_2D, tausta->getID());
	myEnable(MY_TEXTURE_2D);
	dmsPerspective2D(640, 480);
	glBegin(GL_QUADS);
	glColor4f(1,1,1,alpha);
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

	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST); 
*/
	glLoadIdentity();
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);

	float *eq = dmsGetSpectrum();

	myEnable(MY_TEXTURE_2D);
	myDisable(MY_BLEND);
	Texture *tausta = dmsGetTexture("inverteye.jpg");


	const int xres = 15;
	const int yres = 10;

	const float coordstepx = 1.0f / xres;
	const float coordstepy = 1.0f / yres; 
	const float xstep = 640.0f / xres;
	const float ystep = 480.0f / yres;

	glBindTexture(GL_TEXTURE_2D, tausta->getID());
	dmsPerspective2D(640, 480);

	glBegin(GL_QUADS);
	for (int y=0;y<yres;y++)
	{
		for (int x=0;x<xres;x++)
		{
//			const float tausta_alpha = 0.6f + 0.6f*eq[((int)(x+y+pos*1615))%63];
			const float tausta_alpha = 1.0f;
			glColor3f(alpha*tausta_alpha, alpha*tausta_alpha, alpha*tausta_alpha);

			const float u1 = x*coordstepx;
			const float u2 = (x+1)*coordstepx;
			const float v1 = y*coordstepy;
			const float v2 = (y+1)*coordstepy
				;

			const float x1 = x*xstep;
			const float x2 = (x+1)*xstep;
			const float y1 = y*ystep;
			const float y2 = (y+1)*ystep;

			glTexCoord2f(u1, v1);
			glVertex2f(x1, y1);
			glTexCoord2f(u2, v1);
			glVertex2f(x2, y1);
			glTexCoord2f(u2, v2);
			glVertex2f(x2, y2);
			glTexCoord2f(u1, v2);
			glVertex2f(x1, y2);
		}
	}
	glEnd();
	dmsPerspective3D();


	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST); 




	glLoadIdentity();
	glTranslatef(0.0f, 0, -2.5f);

	myEnable(MY_BLEND);
//	myBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	myEnable(MY_LINE_SMOOTH);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
//	glDepthMask(GL_FALSE);

	int count = kappale->getVertexCount();
	T3DVertex *vertptr = kappale->getVertexArray();
	T3DFace *faceptr = kappale->getFaceArray();

	Matrix rotate;
	rotate.makeRotation(pos*6, pos*4, -pos*2.4f);
	Matrix scale;
	scale.makeIdentity();

	glLineWidth(2.0f); 

	myDisable(MY_TEXTURE_2D);
	for (i=0;i<count;i++)
	{
		const float phase = (float)i/(float)count;
		const float speed = 30.0f;//67.0f;
//		const float r = 0.3f;
//		const float g = 0.8f;
//		const float b = 0.5f;
		const float r = 0.1f;
		const float g = 0.1f;
		const float b = 0.1f;
		const float maxalpha = 0.25f*disttable[i]*(0.5f+(vertptr->normal*rotate).dotProduct(Vector(0,0,1)));
		Vector position = vertptr->position*rotate;

		const float radius = 0.08f + 0.04f*(float)sin(phase*2*3.141592f*6+pos*speed);
		const float kulma = i * 0.2f;
		drawCircle(position, 3 + (i%3) , kulma, radius*radiuspos, r, g, b, alpha*maxalpha); 

		glBegin(GL_LINES);
		glColor4f (r, g, b, maxalpha*alpha*0.5f);
		glVertex3f(0, 0, 0);
		glColor4f(r, g, b, maxalpha*alpha*0.2f);
		glVertex3f(position.x, position.y, position.z);
		glEnd();

		vertptr++;
	}
	glLineWidth(1.0f); 
	//glDepthMask(GL_TRUE);
	myEnable(MY_DEPTH_MASK); 
}


void Bubble::drawCircle(Vector &pos, int slices, float kulma, float radius, float r, float g, float b, float a)
{
	float alpha = kulma;//0.0f;
	int strips = slices;
	glColor4f(r, g, b, a); 
	glBegin(GL_LINE_STRIP);

	for (int i=0;i<strips+1;i++)
	{
		Vector pos1 = Vector((float)sin(alpha), (float)cos(alpha), 0)*radius;
		glVertex3fv((float *)&(pos + pos1)); 
		alpha += 2*3.141592f / strips; 
	}

	glEnd();



}


Bubble::Bubble()
{	
	unsigned int i;

	kappale = dmsGetObject("flake_bubble.t3d");
	disttable = new float[kappale->getVertexCount()];

	T3DVertex *vertptr = kappale->getVertexArray();

	float longest = 0.0f; 
	for (i=0;i<kappale->getVertexCount();i++)
	{
		Vector pos = vertptr->position;
		float pituus = pos.length();
		if (pituus > longest)
			longest = pituus;
		vertptr++;
	}

	vertptr = kappale->getVertexArray(); 
	for (i=0;i<kappale->getVertexCount(); i++)
	{
		vertptr->position *= (1.0f / longest); 
		disttable[i] = vertptr->position.length(); 
		vertptr++;
	}
}

Bubble::~Bubble()
{
//	kappale.release();
	delete [] disttable;
//	delete tausta;
}

bool Bubble::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

