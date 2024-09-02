#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "crystal.hpp"


void Crystal::draw()
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

	write(620, 640, pos, 0.5f, 0.9f, alpha, "teknofontti.pcx", "I am born");


}


void Crystal::renderScene(float pos, float alpha)
{
	unsigned int i;
/*

	glDepthMask(GL_FALSE);

	Texture *texture = dmsGetTexture("databall.jpg");
	glBindTexture(GL_TEXTURE_2D, texture->getID());

	const float bgalpha = 0.27f;
	glColor3f(alpha*bgalpha, alpha*bgalpha, alpha*bgalpha);
	glRotatef(pos*32, 1.0f, 0.7f, -0.5f);
//	Primitives::texturedCube(&Vector(0, 0, 0), 1, 2);
	glDepthMask(GL_TRUE);
*/
//	glLoadIdentity();
//	glTranslatef(-0.6f, -0.2f, -25.7f);

	glLoadIdentity();
	Vector kamerapos = kamera->getValue(pos);
	gluLookAt(kamerapos.x, kamerapos.y, kamerapos.z, 0, 0, 0, 0, 1, 0);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	Matrix rotate;
	rotate.makeRotation(0.1f*(float)sin(pos*25), 0.2f*(float)sin(pos*12.0f), 0);
 
	Vector valo(3, -2, 0);
	Matrix valorotate;
	valorotate.makeRotation(pos*11, pos*14, -pos*9);
	valo *= valorotate;

	Texture *tekstuuri = dmsGetTexture("kromi.pcx");
	Texture *valotekstuuri = dmsGetTexture("flare.pcx");
	T3DFace *faceptr = kappale->getFaceArray();
	T3DVertex *verteksit = kappale->getVertexArray();

	myDisable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_LINE_SMOOTH);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);

	unsigned int reunusvertexcount = reunus->getVertexCount();

	T3DVertex *vertices = reunus->getVertexArray();
	glColor4f(1, 1, 1, 0.07f*alpha);

	glPushMatrix();
	glRotatef(100*pos, 0.5f, 0.4f, -0.45f);

	glBegin(GL_LINE_STRIP);
	for (i=0;i<reunusvertexcount;i++)
	{
		const float scale = 1.8f;
		glVertex3fv((float *)&vertices->position);
		vertices++;
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();

	glRotatef(100*pos, 0.25f, 0.6f, -0.21f);
	vertices = reunus->getVertexArray();
	glColor4f(1, 0.7f, 0.2f, 0.06f*alpha); 
	glBegin(GL_LINE_STRIP);
	for (i=0;i<reunusvertexcount;i++)
	{
		const float scale = 2.1f;
		glVertex3fv((float *)&vertices->position);
		vertices++;
	}
	glEnd();
	glPopMatrix();

	myDisable(MY_BLEND);
	myEnable(MY_TEXTURE_2D);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);

	const unsigned int faces = kappale->getFaceCount();
	for (i=0;i<faces;i++)
	{
		utable[i]+=utableadd[i];
		vtable[i]+=vtableadd[i];
		
		Vector normal = faceptr->normal*rotate;
		float l1 = normal.dotProduct(valo)*alpha;

		Vector v1 = verteksit[faceptr->a].position*rotate;
		Vector v2 = verteksit[faceptr->b].position*rotate;
		Vector v3 = verteksit[faceptr->c].position*rotate;
		Vector n1 = verteksit[faceptr->a].normal*rotate;
		Vector n2 = verteksit[faceptr->b].normal*rotate;
		Vector n3 = verteksit[faceptr->c].normal*rotate;

		glBindTexture(GL_TEXTURE_2D, tekstuuri->getID());

		glBegin(GL_TRIANGLES);
		glColor3f(l1*0.6f, l1*0.6f, l1*0.6f);
		const float power = 1.3f;//0.5f;//1.3f;
		const float uadd = (float)sin(utable[i])*power;
		const float vadd = (float)cos(vtable[i])*power;

		const float umul = 0.2f;
		const float vmul = 0.2f; 
		glTexCoord2f((n1.x+v1.x+uadd)*umul, (n1.y+v1.y+vadd)*vmul);
		glVertex3fv((float *)&v1);
		glTexCoord2f((n2.x+v2.x+uadd)*umul, (n2.y+v2.y+vadd)*vmul);
		glVertex3fv((float *)&v2);
		glTexCoord2f((n3.x+v3.x+uadd)*umul, (n3.y+v3.y+vadd)*vmul);
		glVertex3fv((float *)&v3);

//		glTexCoord2f(n1.x+v1.x+uadd, n1.y+v1.y+vadd);
//		glVertex3fv((float *)&v1);
//		glTexCoord2f(n2.x+v2.x+uadd, n2.y+v2.y+vadd);
//		glVertex3fv((float *)&v2);
//		glTexCoord2f(n3.x+v3.x+uadd, n3.y+v3.y+vadd);
//		glVertex3fv((float *)&v3);
		
		glEnd();

		myEnable(MY_BLEND);
		myBlendFunc(GL_SRC_ALPHA, GL_ONE);
		myDisable(MY_DEPTH_MASK);		
		myEnable(MY_DEPTH_TEST);
		
		const float phase = (float)i/(float)faces;
		const float size = 8*(0.08f+0.03f*(float)sin(pos*60*(i*0.003f)));//+phase*2*3.141592f);
		Vector center = (v1+v2+v3)*0.33333333f;
		glBindTexture(GL_TEXTURE_2D, valotekstuuri->getID());
		glColor4f(1, 0.5f, 0.26f, l1*0.085f);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(center.x-size, center.y-size, center.z);
		glTexCoord2f(1, 0);
		glVertex3f(center.x+size, center.y-size, center.z);
		glTexCoord2f(1, 1);
		glVertex3f(center.x+size, center.y+size, center.z);
		glTexCoord2f(0, 1);
		glVertex3f(center.x-size, center.y+size, center.z);
		glEnd();
		
		myEnable(MY_DEPTH_MASK);
		myEnable(MY_DEPTH_TEST);
		myDisable(MY_BLEND);
		faceptr++;
	}
}

Crystal::Crystal()
{	
	int i;
	kappale = dmsGetObject("baby.t3d");
	reunus = dmsGetObject("flake.t3d");

	int size = kappale->getFaceCount();
	utable = new float[size];
	vtable = new float[size];
	utableadd = new float[size];
	vtableadd = new float[size];

	for (i=0;i<size;i++)
	{
		const float speed = 0.03f;
		utable[i] = (rand()%1000)/100000.0f;
		vtable[i] = (rand()%1000)/100000.0f;
		utableadd[i] = (((rand()%1000)/1000.0f)-0.5f)*speed;
		vtableadd[i] = (((rand()%1000)/1000.0f)-0.5f)*speed;
	}
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	kamera = new Path();
	kamera->addpoint(Vector(-40, 0, 40));
	kamera->addpoint(Vector(-30, 20, 20));
	kamera->addpoint(Vector(-10, -4.0f, -8));
}

Crystal::~Crystal()
{
	delete [] utable;
	delete [] vtable;
	delete [] utableadd;
	delete [] vtableadd;

//ei tuhota näitä tässä, manageri hoitelee
//	delete reunus;
//	delete kappale;

	delete kamera;
}

bool Crystal::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}
