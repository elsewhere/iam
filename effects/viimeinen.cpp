#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Viimeinen.hpp"
#include "../mathematics.hpp"

void Viimeinen::draw()
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

	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	write(384,    300, pos, 0.65f, 0.75f, alpha, "teknofontti.pcx", "Preacher");
	write(384+16, 340, pos, 0.70f, 0.80f, alpha, "teknofontti.pcx", "Vulture");
	write(384   , 380, pos, 0.75f, 0.85f, alpha, "teknofontti.pcx", "Kofeiini");
	write(448   , 420, pos, 0.80f, 0.90f, alpha, "teknofontti.pcx", "Grip");

	write(512-64, 100, pos, 0.30f, 0.55f, alpha, "luontofontti.pcx", "I am");
	write(512-64, 600, pos, 0.35f, 0.60f, alpha, "teknofontti.pcx", "I am");



}

void Viimeinen::drawFlare(Vector &pos, float size)
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



void Viimeinen::renderScene(float pos, float alpha)
{
	int i, j;
	glLoadIdentity();
	glTranslatef(0, 0, -1.5f-pos*2.0f);
	glRotatef(pos*pos*150, 1, 0.5f, 0.2f);

	myEnable(MY_LINE_SMOOTH);
	myEnable(MY_BLEND);
	myDisable(MY_TEXTURE_2D); 
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);//_MINUS_SRC_ALPHA);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK); 
	glLineWidth(4.0f); 

	for (i=0;i<oksacount;i++)
	{
		const int strips = 50;

		glLineWidth(2.0f); 
		float oksapos = pos + 0.001f*i;
/*		oksapos = (float)pow(oksapos, 1.8f); 
		if (oksapos > 1)
		{
			glLineWidth(2.0f + (oksapos-1));
			oksapos = 1.0f; 
/*			oksapos = 1.0f;
			Vector polypos = oksat[i]->getValue(1);
			glBegin(GL_TRIANGLES);
			const float size = 0.04f;
			glColor4f(0.6f, 0.6f, 0.6f, alpha*0.95f); 
			glVertex3fv((float *)&(polypos + Vector(0, -size, 0)));
			glVertex3fv((float *)&(polypos + Vector(size*0.5f, size*0.5f, 0)));
			glVertex3fv((float *)&(polypos + Vector(-size*0.5f, size*0.5f, 0)));
			glEnd();*/
//		}


//		glColor4f(0.4f, 0.8f, 0.37f, alpha*0.45f); 
//		glColor4f(0.4f, 0.8f, 0.37f, alpha*0.15f); 
		glBegin(GL_LINE_STRIP);
		for (j=0;j<strips;j++)
		{
			const float step = oksapos / (float)strips;

			float p = step*j;
			Vector v = oksat[i]->getValue(p);
//			Vector tangentti = oksat[i]->getDerivative(p); 
			const float swirly = j * 0.1f;
			const float radius = 0.01f + 0.005f*(float)sin(swirly*4.11f + pos*63); 
//			rot.makeRotation(swirly, swirly, swirly);
//			const float radius = 0.01f;
			Vector v2 = v + Vector((float)sin(swirly), (float)cos(swirly), 0)*radius;  
//			Vector v2 = v + ((tangentti-v) * rot); //Vector(radius*(float)sin(swirly), radius*(float)cos(swirly), 0);  

			p *= p; 
			Vector c1 = Vector(0.4f, 0.4f, 0.4f); 
			Vector c2 = Vector(0.4f, 0.8f, 0.37f); 
			Vector c = c2*p + c1*(1-p);
			glColor4f(c.x, c.y, c.z, alpha*0.25f); 
			glVertex3fv((float *)&v); 
			glVertex3fv((float *)&v2); 
		}
		glEnd();
	}
}

Viimeinen::Viimeinen()
{	
	int i, j;
	/*
	int oksacount;
	int pointcount;
	Vektori *points;
	Path **oksat; */

	pointcount = 1000;
	oksacount = 250;

	points = new Vector[pointcount];
	oksat = new Path*[oksacount];

	srand(666+007);
	for (i=0;i<pointcount;i++)
	{
		const float minradius = 0.15f;
		const float maxradius = 1.0f; 
		const float radius = minradius + ((rand()%10000)/10000.0f) * maxradius;

		const float phi = 2*3.141592f * ((rand()%10000)/10000.0f);
		const float theta = 3.141592f * ((rand()%10000)/10000.0f);

		Vector v = Mathematics::sphereToCartesian(radius, theta, phi);//, theta); 
		points[i] = v;
	}

	for (i=0;i<oksacount;i++)
	{
		oksat[i] = new Path();

		const int minpoints = 4;
		const int maxpoints = 7;
		const int pointcount = minpoints + (rand()%(maxpoints-minpoints));

		oksat[i]->addpoint(Vector(0,0,0));

		float phi = 2*3.141592f * ((rand()%10000)/10000.0f);
		float theta = 3.141592f * ((rand()%10000)/10000.0f);
		float radius = 0.15f;

		for (j=0;j<pointcount;j++)
		{
/*			const float minphiadd = 0.2f;
			const float maxphiadd = 1.0f; 
			const float minthetaadd = 0.1f;
			const float maxthetaadd = 0.5f; 
			const float minradiusadd = 0.10f;
			const float maxradiusadd = 0.25f; */
			const float minphiadd = 0.15f;
			const float maxphiadd = 0.75f; 
			const float minthetaadd = 0.1f;
			const float maxthetaadd = 0.5f; 
			const float minradiusadd = 0.15f;
			const float maxradiusadd = 0.30f; 
			const float phiadd = minphiadd + (rand()%10000/10000.0f)*(maxphiadd-minphiadd);
			const float thetaadd = minthetaadd + (rand()%10000/10000.0f)*(maxthetaadd-minthetaadd);
			const float radiusadd = minradiusadd + (rand()%10000/10000.0f)*(maxradiusadd-minradiusadd);
			oksat[i]->addpoint(Mathematics::sphereToCartesian(radius, phi, theta));

			phi += phiadd;
			theta += thetaadd;
			radius += radiusadd;

//			const int pointindex = rand()%pointcount;
//			Vector v = points[pointindex];
//			oksat[i]->addpoint(Vector(v.x, v.y, v.z));

		}
	}
}

Viimeinen::~Viimeinen()
{
	int i;
	delete [] points;

	for (i=0;i<oksacount;i++)
	{
//		oksat[i]->destroy();
		delete oksat[i];
	}
	delete [] oksat;

}

bool Viimeinen::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

