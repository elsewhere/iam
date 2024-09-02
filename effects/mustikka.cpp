
#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Mustikka.hpp"

void Varpu::render()
{
}

void Varpu::destroy()
{
	if (marjat)
		delete [] marjat;
	if (varsi)
		delete [] varsi;
}

void Mustikka::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.13f;
	const float fadeoutstart = 0.92f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
}

void Mustikka::calculateFades(float pos)
{
	int i;
	for (i=0;i<mustikkacount;i++)
	{
		float alpha = 1.0;

		//vain fadeout
		if (pos < fadetimes_start[i])
		{
			fadetable[i] = 1.0f;
		}
		else if (pos > fadetimes_stop[i])
		{
			fadetable[i] = 0.0f;
		}
		else if ((pos >= fadetimes_start[i]) && (pos <= fadetimes_stop[i]))
		{
			fadetable[i] = 1-(pos-fadetimes_start[i]) / (fadetimes_stop[i]-fadetimes_start[i]);
		}
	}

}


void Mustikka::renderScene(float pos, float alpha)
{
	int i, j;

	Matrix rotation;
	glLoadIdentity();

	glTranslatef(0.2f+pos*2, -1.4f, -6.0f);
	rotation.makeRotation(-0.1f, -0.5f-pos*0.7f, -0.4f);

	T3DVertex *vertices = kappale->getVertexArray();
	T3DFace *faces = kappale->getFaceArray();
	const int facecount = kappale->getFaceCount();
	const int vertexcount = kappale->getVertexCount();
	calculateFades(pos); 

	//PÄÄN PIIRROSSA ON JOTAIN, JOKA RIKKOO FADEN JA AIHEUTTAA NYKIMISTÄ. KORJAA! 
	// ???

	myDisable(MY_BLEND);
	myDisable(MY_CULL_FACE);
	myEnable(MY_TEXTURE_2D);		
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
		
	Texture *kuutiotexture = dmsGetTexture("kromi.pcx");
	glBindTexture(GL_TEXTURE_2D, kuutiotexture->getID());
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-2.2f, 1.0f, -8.0f);
//	glRotatef(36, 1, 0, 0);
//	glRotatef(110, 0, 1, 0);
//	glRotatef(30, 0, 0, 1);
//	glRotatef(pos*50, 0.09f, 0.2f, 0);

	Matrix paarotate;
//	paarotate.makeRotation(-pos*0.15f*0, -pos*0.40f, -pos*0.19f); 
	paarotate.makeRotation(-pos*0.15f*0, -pos*0.40f - (110.0f/180.0f)*3.141592f, -pos*0.19f - (30.0f/180.0f)*3.141592f); 
	glScalef(0.4f, 0.4f, 0.4f); 

	const Vector envadd = Vector(pos, 0, 0);  
	glBegin(GL_TRIANGLES);

	for (i=0;i<facecount;i++)
	{
		const float step = (mustikkacount) / (float)facecount;
		const int index_to_mustikkatable = (int)(i * step);
		const float fadevalue = 1 - fadetable[index_to_mustikkatable % mustikkacount];

		const float maxalpha = 1.0f*fadevalue;
		//glColor3f(alpha*maxalpha, alpha*maxalpha, alpha*maxalpha);
		glColor4f(alpha*maxalpha, alpha*maxalpha, alpha*maxalpha, alpha*maxalpha);
		Vector v1 = vertices[faces->a].position * paarotate;
		Vector v2 = vertices[faces->b].position * paarotate;
		Vector v3 = vertices[faces->c].position * paarotate;

		Vector n1 = (vertices[faces->a].normal + envadd) * paarotate;
		Vector n2 = (vertices[faces->b].normal + envadd) * paarotate;
		Vector n3 = (vertices[faces->c].normal + envadd) * paarotate;

		glTexCoord2fv((float *)&n1);
		glVertex3fv((float *)&v1); 
		glTexCoord2fv((float *)&n2);
		glVertex3fv((float *)&v2); 
		glTexCoord2fv((float *)&n3);
		glVertex3fv((float *)&v3); 

		faces++;
	}
	glEnd();

	//flaret
	Texture *flaretexture = dmsGetTexture("flare.pcx"); 
	glBindTexture(GL_TEXTURE_2D, flaretexture->getID());
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST); 
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	const int travel = (int)(pos*1400);
	float *eq = dmsGetSpectrum();

	float equ[512];
	for (i=0;i<512;i++)
	{
		equ[i] = (float)pow(eq[i], 1.01f); 
	}

	const float step = (float)mustikkacount / (float)vertexcount;

	Texture *particle1 = dmsGetTexture("particle1.pcx");
	Texture *particle2 = dmsGetTexture("particle2.pcx");
	Texture *particle3 = dmsGetTexture("particle3.pcx");
	for (i=0;i<vertexcount;i++)
	{
		Vector n = (vertices[i].normal)*paarotate;
		const int index_to_mustikkatable = (int)(i * step);
		const float fadevalue = 1 - fadetable[(index_to_mustikkatable) % mustikkacount];
		
		if (n.z > 0.0f)
		{
			Vector v = (vertices[i].position*1.02f) * paarotate;
			const float eqvalue = equ[(i + travel)%511];
			const float size = 7.00f*fadevalue*eqvalue;

			switch(rand()%3)
			{
				case 0: glBindTexture(GL_TEXTURE_2D, particle1->getID()); break;
				case 1: glBindTexture(GL_TEXTURE_2D, particle2->getID()); break;
				case 2: glBindTexture(GL_TEXTURE_2D, particle3->getID()); break;
			}
			glColor4f(1,1,1,alpha);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3fv((float *)&(v + Vector(-size, -size, 0))); 
			glTexCoord2f(1, 0);
			glVertex3fv((float *)&(v + Vector(size, -size, 0))); 
			glTexCoord2f(1, 1);
			glVertex3fv((float *)&(v + Vector(size, size, 0))); 
			glTexCoord2f(0, 1);
			glVertex3fv((float *)&(v + Vector(-size, size, 0))); 
			glEnd();
		}
	}

	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);

	glPopMatrix();

	myDisable(MY_TEXTURE_2D);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	myEnable(MY_BLEND);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	myEnable(MY_LINE_SMOOTH);

	glColor4f(0.4f, 0.8f, 0.5f, alpha*0.66f);
	glLineWidth(2.5f);

	calculateFades(pos); 

	//glLoadIdentity();
	//glTranslatef(0.4f-pos*0.13f, -1.4f+0.2f*(float)sin(pos*3.141592f), -6.0f+pos*0.6f);

	for (i=0;i<mustikkacount;i++)
	{
		glBegin(GL_LINE_STRIP);
		for (j=0;j<mustikat[i].varsicount;j++)
		{
			const float alphavalue = 0.7f+0.3f*fadetable[i];
			const float c = (float)j/ 6.0f;
			glColor4f(mustikat[i].r, mustikat[i].g, mustikat[i].b, (0.3f+c*0.7f)*alpha*alphavalue);
			Vector v = mustikat[i].varsi[j]*rotation;
			glVertex3f(v.x, v.y, v.z);
		}
		glEnd();
	}

	myEnable(MY_TEXTURE_2D);
	Texture *glowtexture = dmsGetTexture("flare.pcx");
	glBindTexture(GL_TEXTURE_2D, glowtexture->getID());
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);

	glBegin(GL_QUADS); 
	for (i=0;i<mustikkacount;i++)
	{
		glColor4f(1,1,1,alpha*0.34f);
		for (j=0;j<mustikat[i].marjacount;j++)
		{
			Vector v = mustikat[i].marjat[j]*rotation;
			const float size = 0.02f*fadetable[i]; 
			glTexCoord2f(0, 0);
			glVertex3fv((float *)&(v + Vector(-size, -size, 0))); 
			glTexCoord2f(1, 0);
			glVertex3fv((float *)&(v + Vector(size, -size, 0))); 
			glTexCoord2f(1, 1);
			glVertex3fv((float *)&(v + Vector(size, size, 0))); 
			glTexCoord2f(0, 1);
			glVertex3fv((float *)&(v + Vector(-size, size, 0))); 
		}
		const float phase = (float)i/(float)mustikkacount;
		const float glow = 0.07f*(float)fabs((float)sin(pos*17*2*3.141592f*phase));
		glColor4f(1,1,1,alpha*(0.1f+glow)*fadetable[i]);
//		glColor4f(0.2f, 0.4f, 0.9f,alpha*(0.3f+glow)*fadetable[i]);
		Vector u = (mustikat[i].varsi[mustikat[i].varsicount-1])*rotation;

		const float glowsize = 0.15f; 
		glTexCoord2f(0, 0);
		glVertex3fv((float *)&(u + Vector(-glowsize, -glowsize, 0))); 
		glTexCoord2f(1, 0);
		glVertex3fv((float *)&(u + Vector(glowsize, -glowsize, 0))); 
		glTexCoord2f(1, 1);
		glVertex3fv((float *)&(u + Vector(glowsize, glowsize, 0))); 
		glTexCoord2f(0, 1);
		glVertex3fv((float *)&(u + Vector(-glowsize, glowsize, 0))); 
	}
	glEnd();
	glLineWidth(1.0f);

	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
}

Mustikka::Mustikka()
{	
	int i, j;
	Matrix mat;
	mustikkacount = 1200;

	mustikat = new Varpu[mustikkacount];
	fadetable = new float[mustikkacount];
	fadetimes_start = new float[mustikkacount];
	fadetimes_stop = new float[mustikkacount];

	kappale = dmsGetObject("baby.t3d");

	srand(50);
	for (i=0;i<mustikkacount;i++)
	{
		const float y = 0.0f;

		const float xrange = 4.0f;
		const float zrange = 6.0f;
		const float minheight = 0.15f;
		const float heightadd = 0.05f;
		const float circle = 0.1f;


		const float fadestart = 0.2f + 0.5f*(float)((rand()%10000)/10000.0f); 
		const float fadetime = 0.1f + 0.05f*(float)((rand()%10000)/10000.0f); 
		const float fadestop = fadestart+fadetime;

		fadetimes_start[i] = fadestart;
		fadetimes_stop[i] = fadestop;

//		mat.makeScale(xrange, 1, zrange);
		mat.data[0][0] = xrange;
		mat.data[1][1] = 1;
		mat.data[2][2] = zrange;

		//juuren paikkaVector
		const float zeta = ((rand()%10000)/10000.0f)*2*3.1415492f;
		float rad = ((rand()%10000)/10000.0f)*1.0f;
		Vector temp1 = Vector(((rand()%1000)/1000.0f)-0.5f,
							  y,
							  ((rand()%1000)/1000.0f)-0.5f)*mat;
		Vector temp2 = Vector(rad*(float)cos(zeta), y, rad*(float)sin(zeta))*mat;
		Vector pos = temp1*0.5f + temp2 * 0.5f;

		const float alpha = ((rand()%10000)/10000.0f)*2*3.141592f;
		const float height = minheight + heightadd*((rand()%1000)/1000.0f);
		//loppupään ero juuren paikkaVectorsta
		Vector delta = Vector(circle * (float)sin(alpha), 
			                    height, 
								circle * (float)cos(alpha));

		//bezier-käyrien tangentit
		const float beta = ((rand()%10000)/10000.0f)*2*3.141592f;
		const float gamma = ((rand()%10000)/10000.0f)*2*3.141592f;
		const float bezierscale = 0.5f;
		const float circle2 = 0.04f;

		Vector tangent1 = Vector(circle2*(float)sin(beta), 1, circle2*(float)cos(beta)).normalize();
		Vector tangent2 = Vector(circle2*(float)sin(beta), -1, circle2*(float)cos(beta)).normalize();

		tangent1 *= bezierscale;
		tangent2 *= (bezierscale*0.01f);

		Vector startpos = pos;
		Vector endpos = pos+delta;

		//luodaan varsi bezier-käyrällä
		const int varrenpalasia = 8;
		mustikat[i].varsi = new Vector[varrenpalasia];
		mustikat[i].varsicount = varrenpalasia;

		HermiteCurve c;
		c.set(startpos, tangent1, tangent2, endpos);
		for (j=0;j<varrenpalasia;j++)
		{
			Vector v = c.getValue(j * (float)(1.0f / (float)varrenpalasia));
			mustikat[i].varsi[j] = v;
		}

		//värit
//			glColor4f(0.4f, 0.8f, 0.5f, (0.3f+c*0.7f)*alpha);
		const float brightness = 0.2f + ((rand()%1000)/1000.0f)*0.4f;
		mustikat[i].r = 0.4f*brightness;
		mustikat[i].g = 0.8f*brightness;
		mustikat[i].b = 0.5f*brightness;

		//luodaan marjat
		const int marjoja = 1+(rand()%4);
		mustikat[i].marjat = new Vector[marjoja];
		mustikat[i].marjacount = marjoja;
		for (j=0;j<marjoja;j++)
		{
			const float marjaradius = 0.01f;
			const float theta = (float)((rand()%1000)/1000.0f)*2*3.141592f;
			const float rho = (float)((rand()%1000)/1000.0f)*3.141592f;

			Vector marjapos = Mathematics::sphereToCartesian(marjaradius, theta, rho);
			mustikat[i].marjat[j] = endpos + marjapos;
		}
	}
}

Mustikka::~Mustikka()
{
	int i;
	for (i=0;i<mustikkacount;i++)
		mustikat[i].destroy();
	delete [] mustikat;

	delete [] fadetable;
	delete [] fadetimes_start;
	delete [] fadetimes_stop;

}

bool Mustikka::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

