#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Layers.hpp"

void Layers::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.97f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
}


void Layers::renderScene(float pos, float alpha)
{
	int i;
	glLoadIdentity();
	glTranslatef(0.5f, 0, -152.7f);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	Matrix rotate;
	rotate.makeRotation(pos*1.3f, pos*5.3f, -pos*1.4f);

	Vector valo(1,1,0.5f);
	Matrix valorotate;
	valorotate.makeRotation(pos*15, -pos*13, pos*7);
	valo *= valorotate;

	Texture *texture1 = dmsGetTexture("visual2.pcx");
	Texture *texture2 = dmsGetTexture("visual11.pcx");

	T3DFace *faceptr = kappale->getFaceArray();
	T3DVertex *vertices = kappale->getVertexArray();
	int facecount = kappale->getFaceCount();
		
	for (i=0;i<facecount;i++)
	{	
		myEnable(MY_DEPTH_TEST);
		myEnable(MY_DEPTH_MASK);

		const float maxalpha = 0.7f*alpha;
		glBindTexture(GL_TEXTURE_2D, texture1->getID());
		Vector v1 = vertices[faceptr->a].position*rotate;
		Vector v2 = vertices[faceptr->b].position*rotate;
		Vector v3 = vertices[faceptr->c].position*rotate;
		Vector n1 = vertices[faceptr->a].normal*rotate;
		Vector n2 = vertices[faceptr->b].normal*rotate;
		Vector n3 = vertices[faceptr->c].normal*rotate;
		float l1 = (faceptr->normal*rotate).dotProduct(valo);
		l1 *= l1; l1 *= l1; 
		glBegin(GL_TRIANGLES);
//		glColor3f(maxalpha*l1, maxalpha*l1, maxalpha*l1);
		glColor3f(1,1,1);
		glTexCoord2f(vertices[faceptr->a].u, vertices[faceptr->a].v);
		glVertex3fv((float *)&v1);
		glTexCoord2f(vertices[faceptr->b].u, vertices[faceptr->b].v);
		glVertex3fv((float *)&v2);
		glTexCoord2f(vertices[faceptr->c].u, vertices[faceptr->c].v);
		glVertex3fv((float *)&v3);
		glEnd();

		//multitexture envymap
		myEnable(MY_BLEND);
		myEnable(MY_DEPTH_TEST);
		myDisable(MY_DEPTH_MASK);

		glColor4f(1, 1, 1, alpha*0.2f);
		glBindTexture(GL_TEXTURE_2D, texture2->getID());
		glBegin(GL_TRIANGLES);
		glTexCoord2fv((float *)&n1);
		glVertex3fv((float *)&v1);
		glTexCoord2fv((float *)&n2);
		glVertex3fv((float *)&v2);
		glTexCoord2fv((float *)&n3);
		glVertex3fv((float *)&v3);

		glEnd();

		myEnable(MY_DEPTH_MASK);
/*
		glBindTexture(GL_TEXTURE_2D, texture1->getID());
		const float phase = (float)i/(float)facecount;
		const float size = 1.2f + 0.1f*(float)sin(pos*16);//+phase*6*3.141592f);
		v1 *= size; v2 *= size; v3 *= size;

		glBegin(GL_TRIANGLES);
		glColor4f(1,1,1,0.13f*alpha);
		glTexCoord2f(vertices[faceptr->a].u, vertices[faceptr->a].v);
		glVertex3fv((float *)&v1);
		glTexCoord2f(vertices[faceptr->b].u, vertices[faceptr->b].v);
		glVertex3fv((float *)&v2);
		glTexCoord2f(vertices[faceptr->c].u, vertices[faceptr->c].v);
		glVertex3fv((float *)&v3);
		glEnd();
		glDepthMask(GL_TRUE);
		myDisable(MY_BLEND);
*/	
		faceptr++;
	}


	myDisable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	//myBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_LINE_SMOOTH);
	
	Matrix reunusrotate;
	reunusrotate.makeRotation(pos*1.2f, -pos*0.9f, pos*0.07f);

	T3DVertex *vertexptr = reunus->getVertexArray();
	int reunusvertices = reunus->getVertexCount();
	glBegin(GL_LINE_STRIP);
	glColor4f(1, 1, 1, 0.047f*alpha);
	for (i=0;i<reunusvertices;i++)
	{
		const float scale = 2.32f;
		Vector v = (vertexptr->position*reunusrotate)*scale;
		glVertex3fv((float *)&v);
		vertexptr++;
	}
	glEnd();

/*
	glDisable(MY_DEPTH_MASK);
	Vertex *vertexptr = reunus.getVertexPointer();
	glColor4f(1, 1, 1, 0.077f*alpha);
	glBegin(GL_LINE_STRIP);
	for (i=0;i<reunus.getVertices();i++)
	{
		const float scale = 2.32f;
		Vector v = (vertexptr->getPosition()*reunusrotate)*scale;
		glColor4f(1, 1, 1, 0.047f*alpha);
		glVertex3f(v.x, v.y, v.z);
		vertexptr++;
	}
	glEnd();
	glEnable(MY_DEPTH_MASK);
	vertexptr = reunus.getVertexPointer();
	reunusrotate.makeRotation(-pos*1.5f, pos*3.1f, -pos*1.4f);
	glColor4f(1, 0.8f, 0.6f, 0.10f*alpha);
	glBegin(GL_LINE_STRIP);
	for (i=0;i<reunus.getVertices();i++)
	{
		const float scale = 0.7f;
		Vector v = (vertexptr->getPosition()*reunusrotate)*scale;
		glVertex3f(v.x, v.y, v.z);
		vertexptr++;
	}
	glEnd();
*/
	myDisable(MY_BLEND);
	myEnable(MY_TEXTURE_2D);
}
Layers::Layers()
{
	kappale = dmsGetObject("flake.t3d");
	reunus = dmsGetObject("hedra.t3d");

	/*
	Loader3DS loader;
	if (loader.import3DS("3ds\\flake.3ds")==false)
	{
		loki->writeToLog("erroria!");		
	}
	if (!(loader >> &kappale))
	{
		loki->writeToLog("lis‰‰ erroria!");
	}
	loader.shut();
	if (loader.import3DS("3ds\\hedra1.3ds")==false)
	{
		loki->writeToLog("erroria!");		
	}
	if (!(loader >> &reunus))
	{
		loki->writeToLog("lis‰‰ erroria!");
	}
	loader.shut();
	kappale.reduceToUnit();
	reunus.reduceToUnit();
*/
}

Layers::~Layers()
{
}

bool Layers::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}