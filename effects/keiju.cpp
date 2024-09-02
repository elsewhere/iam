#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Keiju.hpp"

void Keiju::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.94f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
	write(224, 200, pos, 0.35f, 0.50f, alpha, "luontofontti.pcx", "I am made of myths");
	write(224-32, 230, pos, 0.40f, 0.55f, alpha, "luontofontti.pcx", "I am made of stories");
	write(224-32, 260, pos, 0.45f, 0.60f, alpha, "luontofontti.pcx", "I am made of legends");

	write(304, 450, pos, 0.55f, 0.70f, alpha, "teknofontti.pcx", "I am progress");
	write(304-16, 480, pos, 0.60f, 0.75f, alpha, "teknofontti.pcx", "I am evolution");
	write(512-4*32+16, 510, pos, 0.65f, 0.80f, alpha, "teknofontti.pcx", "give way");
	
/*	write(512-32, 600, pos, 0.70f, 0.95f, alpha, "luontofontti.pcx", "no");
	write(512-32-5, 600, pos, 0.70f, 0.95f, alpha, "luontofontti.pcx", "no");
	write(512-32-5, 600-5, pos, 0.70f, 0.95f, alpha, "luontofontti.pcx", "no");
	write(512-32, 600-5, pos, 0.70f, 0.95f, alpha, "luontofontti.pcx", "no");
*/
	write2(512-64, 600, 2, pos, 0.70f, 0.95f, alpha, "luontofontti.pcx", "no");
//	write2(512-64-4, 600-4, 2, pos, 0.70f, 0.95f, alpha, "luontofontti.pcx", "no");
}


/*
  I am made of myths
  I am made of stories
  I am made of legends

  I am progress
  I am evolution
  Give way

  no


*/

void Keiju::renderScene(float pos, float alpha)
{
	int i;
	float t;

	glLoadIdentity();
	glTranslatef(0, 0, -2.5f);
	Matrix globalrotate;
	globalrotate.makeRotation(-pos*2, -pos*3, -pos*1.5f);

	const float zoominstart = 0.0f;
	const float zoominstop = 0.1f;
	const float zoomoutstart = 0.9f;
	const float zoomoutstop = 1.0f;

	float koko = 1.0f;
	if (pos >= zoominstart && pos <= zoominstop)
		koko = (float)(0.6f+0.4f*sin(((pos-zoominstart) / (zoominstop-zoominstart))*3.141592f*0.5f));
	if (pos >= zoomoutstart && pos <= zoomoutstop)
	{
		float zoompos = (pos - zoomoutstart) / (zoomoutstop-zoomoutstart);
		koko = (float)pow(1-zoompos, 1.6f);
	}
	
//	float koko = (float)sin(pos*3.141592f*0.5f);
//	koko *= koko; //sin^2
	koko *= 0.7f;

	glScalef(koko, koko, koko);

	Texture *flare = dmsGetTexture("flare.pcx");
	Texture *flare2 = dmsGetTexture("particle.jpg");

	for (i=0;i<itemcount;i++)
	{
		const float rotspeed = 5.0f;
		Matrix rotate;


		rotate.makeRotation(items[i].rotate.x+items[i].rotatespeed.x*rotspeed*pos, 
							items[i].rotate.y+items[i].rotatespeed.y*rotspeed*pos,
							items[i].rotate.z+items[i].rotatespeed.z*rotspeed*pos);

		Vector r = items[i].pos * globalrotate;

		Vector v1 = r + (items[i].v1 * rotate);
		Vector v2 = r + (items[i].v2 * rotate);
		Vector v3 = r + (items[i].v3 * rotate);
		Vector n = (items[i].n * rotate)*globalrotate;
		Vector valo(1, 1, 0);

		const float c = 0.2f*alpha * n.dotProduct(valo);
		glColor3f(c, c, c);
		myDisable(MY_TEXTURE_2D);
/*		glBegin(GL_TRIANGLES);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glEnd();*/
		myEnable(MY_TEXTURE_2D);
		myEnable(MY_BLEND);
		myBlendFunc(GL_SRC_ALPHA, GL_ONE);
		myDisable(MY_DEPTH_TEST);
		myDisable(MY_DEPTH_MASK);
		if (c > 0.0f)
		{
			glColor4f(1,1,1,c*3);
//			glBindTexture(GL_TEXTURE_2D, tManager->getTextureName("particle")->getID());
			glBindTexture(GL_TEXTURE_2D, flare->getID());
			drawFlare(v1, 0.03f);
			drawFlare(v2, 0.03f);
			drawFlare(v3, 0.03f);

			//pitäisi olla eri värinen kuin yläpuolella oleva.. tässä ei vaan ole flaretekstuureja kiinni nyt
//			glBindTexture(GL_TEXTURE_2D, tManager->getTextureName("particle3")->getID());
//			glColor4f(0.9f,0.6f,0.5f,3.25f*alpha*c);
			glBindTexture(GL_TEXTURE_2D, flare2->getID());
			glColor4f(0.5f,0.9f,0.6f,4.6f*alpha*c);
			drawFlare(r, 0.25f);
		}
	}
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	myEnable(MY_BLEND);
	myEnable(MY_LINE_SMOOTH);
	myDisable(MY_DEPTH_MASK);
	//glDepthMask(GL_FALSE);
	myDisable(MY_TEXTURE_2D);

	Vector *ptr = path;
	for (i=0;i<itemcount-1;i++)
	{
		Matrix brotate, brotate2;
		brotate.makeRotation(items[i].bezierrotate.x*pos, 
							 items[i].bezierrotate.y*pos, 
							 items[i].bezierrotate.z*pos);
		brotate2.makeRotation(items[i+1].bezierrotate.x*pos, 
							  items[i+1].bezierrotate.y*pos, 
							  items[i+1].bezierrotate.z*pos);

		CubicBezierCurve curve; 
		curve.set(items[i].pos, items[i].beziertangent*brotate, (items[i+1].beziertangent*brotate2)*-1, items[i+1].pos);
		for (t=0.0f;t<1.0f;t+=1.0f / (float)bezierstrips)
		{
			Vector cpos = curve.getValue(t) * globalrotate;
			*ptr = cpos;
			ptr++;
		}
	}

	glLineWidth(2.5f);

	glBegin(GL_LINE_STRIP);
	for (i=0;i<(itemcount-1)*bezierstrips;i++)
	{
		Vector cpos = path[i];
		const float c = cpos.z;
		glColor4f(0.5f, 0.9f, 0.3f, c*alpha*0.6f);
		glVertex3fv((float *)&cpos);

//		glVertex3f(cpos.x, cpos.y, cpos.z);
	}
	glEnd();

	//glDepthMask(GL_TRUE);
	myEnable(MY_DEPTH_MASK);
	myDisable(MY_BLEND);
	myDisable(MY_LINE_SMOOTH);

	glLineWidth(1.0f);
}

void Keiju::drawFlare(Vector &pos, float size)
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
Keiju::Keiju()
{
	int i;
	itemcount = 150;
	bezierstrips = 40;
	items = new KeijuItem[itemcount];
	path = new Vector[itemcount * (bezierstrips+1)];

	srand(150101);
	for (i=0;i<itemcount;i++)
	{
		const float xsize = 2.0f;
		const float ysize = 2.0f;
		const float zsize = 2.0f;
		const float rotspeed = 0.10f;
		const float polysize = 0.03f;
		const float tangentpower = 0.12f;
		const float tangentrotate = 2.0f;
		//yksikkökuution sisään
		items[i].pos = Vector((((rand()%1000)/1000.0f) - 0.5f)*xsize,
							   (((rand()%1000)/1000.0f) - 0.5f)*ysize,
							   (((rand()%1000)/1000.0f) - 0.5f)*zsize);

		items[i].rotate = Vector(((rand()%1000)/1000.0f)*2*3.1415292f,
								  ((rand()%1000)/1000.0f)*2*3.1415292f,
								  ((rand()%1000)/1000.0f)*2*3.1415292f);

		items[i].rotatespeed = Vector(((rand()%1000)/1000.0f)*2*3.1415292f,
								  ((rand()%1000)/1000.0f)*2*3.1415292f,
								  ((rand()%1000)/1000.0f)*2*3.1415292f);

		items[i].beziertangent = Vector(((rand()%1000)/1000.0f)*2*3.1415292f,
								  ((rand()%1000)/1000.0f)*2*3.1415292f,
								  ((rand()%1000)/1000.0f)*2*3.1415292f)*tangentpower;
		items[i].bezierrotate = Vector(((rand()%1000)/1000.0f)*2*3.1415292f,
								  ((rand()%1000)/1000.0f)*2*3.1415292f,
								  ((rand()%1000)/1000.0f)*2*3.1415292f)*tangentrotate;

		items[i].v1 = Vector(0, 2, 0)*polysize;
		items[i].v2 = Vector(-2, -1, 0)*polysize;
		items[i].v3 = Vector(3, 0, 0)*polysize;

		items[i].n = (items[i].v1-items[i].v2).crossProduct(items[i].v1-items[i].v3);
		items[i].n.normalize();
	}
}

Keiju::~Keiju()
{
	delete [] items;
	delete [] path;
}

bool Keiju::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}
