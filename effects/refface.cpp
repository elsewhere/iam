/*

  I watch

  My heart is beating






*/


//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "refface.hpp"


Heart::Heart()
{
	vaaka = 50;
	pysty = 50;

	points = new Vector [vaaka*pysty];
}

Heart::~Heart()
{
	delete [] points;
}

void Heart::calculate(float pos)
{
    int i, j;
    float alpha, beta;
    Vector *vertices = points;
    
    alpha = 0.0;
	const float aika = pos*10;
	const float radiusmultiply = 3;
    for (i=0;i<pysty;i++)
    {
        beta = 0.0;
        for (j=0;j<vaaka;j++)
        {
                float jkohta = 2*j / (float)vaaka;
                float ikohta = 2*i / (float)pysty;
                
                float radius = 1.0f + 0.2f*((float)sin(3*aika + 6*ikohta)+
											(float)cos(2*aika+11*jkohta+4*(float)sin(ikohta+aika*3)));

				radius *= radiusmultiply;

                Vector paikka = Vector(radius*(float)sin(alpha)*(float)sin(beta),
                                       radius*(float)sin(alpha)*(float)cos(beta), 
                                       radius*(float)cos(alpha));
                *vertices++ = paikka;
                beta += 2*3.141592f / vaaka;
        }
        alpha += 3.141592f / pysty;
    }
}

void Heart::draw(Vector &center, float pos, float alpha)
{
	int i;
	Vector *vertices;
	const float aika = pos*10;
	glPushMatrix();
	glTranslatef(center.x, center.y, center.z);
    glRotatef(aika*60, 1, 1, 0.3f);

	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_TEST);
	myEnable(MY_LINE_SMOOTH);
	myEnable(MY_BLEND);

	glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    vertices = points;
    for (i=0;i<vaaka*pysty;i++)
    {
        glColor4f(0.3f, 0.45f, 0.25f, 0.6f*alpha); //0.8f
        glVertex3fv((float *)&(vertices[i]));
//		glVertex3fv((float *)&vertices[i]);
//		glVertex3fv((float *)&vertices[(i+1)%(vaaka*pysty)]);
//		glVertex3fv((float *)&vertices[(i+vaaka)%(vaaka*pysty)]);
//        vertices++;
    }
    glEnd();  
	glLineWidth(1.5f);
    
    glScalef(1.15f, 1.15f, 1.15f);
    
//	glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    vertices = points;
    for (i=0;i<vaaka*pysty;i++)
    {
        glColor4f(0.8f, 0.6f, 0.2f, 0.1f*alpha);  //0.4f
        glVertex3fv((float *)&(vertices[i]));
//        vertices++;
    }
	glEnd();
//	glLineWidth(1.0f);

	glPopMatrix();

}
//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

RefFace::RefFace()
{
}

RefFace::~RefFace()
{
	release();
	delete heart;
	delete kamera;
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------
bool RefFace::init(unsigned long s, unsigned long e)
{			
	active = false;
	startTime = s;
	endTime = e;	
	
	myEnable(MY_LINE_SMOOTH);
	heart = new Heart();
	kamera = new Path();

	kamera->addpoint(Vector (-50, -10, -5));
	kamera->addpoint(Vector (-10, -30, -46));
	kamera->addpoint(Vector (40, 50, -90));
	kamera->addpoint(Vector (76, 30, -44));
				
	return true;
}

void RefFace::draw()
{	
	Texture *texture = dmsGetTexture("gootti.pcx");
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.9f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= ((pos-fadeinstart) / (fadeinstop-fadeinstart));
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1.0f-((pos-fadeoutstart) / (fadeoutstop-fadeoutstart));

	if(texture)
	{				
		unsigned int *data = texture->getImgData();		
		int x, y, w, h;

		w = texture->getWidth() / 2;
		h = texture->getHeight() / 2;

		glLoadIdentity();
//		gluLookAt(sin(time*0.00005f)*64, cos(time*0.00005f)*64, -70, 0, 0, 0, 0, 1, 0);

		Vector v = kamera->getValue(pos);
		gluLookAt(v.x, v.y, v.z, 0, 0, -pos*16, 0, 1, 0);
	
		myDisable(MY_CULL_FACE);
		myDisable(MY_NORMALIZE);
		myDisable(MY_LIGHTING);
		myDisable(MY_TEXTURE_2D);
		myEnable(MY_BLEND);
		myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();
		glScalef(2, 2, 1.2f);
		glBegin(GL_QUADS);

		float *zetat = new float[2*h*2*w];
		int index = 0;
		for(y = -h; y < h; y++)
		{
			for(x = -w; x < w; x++)
			{
				float len = (float)sqrt(x*x+y*y);
				float zpos = float(len*((0.5f-sin(pos))*0.25f)*cos(len*0.14f-(time*0.001f)));
				zetat[index++] = zpos;
			}
		}

		index = 0;
		for(y = -h; y < h; y++)
		{
			for(x = -w; x < w; x++)
			{
				unsigned int color = *data;
				float zpos = zetat[index];
				
				glColor4ub(color, color, color, unsigned char(alpha*255));
				glVertex3f(x-0.0f, y-0.0f,  0.0f-zpos);	
				glVertex3f(x+1.0f, y-0.0f,  0.0f-zpos);	
				glVertex3f(x+1.0f, y+1.0f,  0.0f-zpos);	
				glVertex3f(x-0.0f, y+1.0f,  0.0f-zpos);
				index++;

				*data++;				
			}
		}
		delete [] zetat;
/*
		for(y = -h; y < h; y++)
		{
			for(x = -w; x < w; x++)
			{
				unsigned int color = *data;
				float len = (float)sqrt(x*x+y*y);
				float zpos = float(len*((0.5f-sin(pos))*0.25f)*cos(len*0.14f-(time*0.001f)));
				
				glColor4ub(color, color, color, unsigned char(alpha*255));
				glVertex3f(x-0.0f, y-0.0f,  0.0f-zpos);	
				glVertex3f(x+1.0f, y-0.0f,  0.0f-zpos);	
				glVertex3f(x+1.0f, y+1.0f,  0.0f-zpos);	
				glVertex3f(x-0.0f, y+1.0f,  0.0f-zpos);	

				*data++;				
			}
		}
*/
		glEnd();
		
		myDisable(MY_DEPTH_TEST);
		myEnable(MY_BLEND);
		myBlendFunc(GL_SRC_ALPHA, GL_ONE);

		data = texture->getImgData();
		
		glBegin(GL_LINES);

		for(y = -h; y < h; y++)
		{
			for(x = -w; x < w; x++)
			{
				unsigned int color = *data;
				float len = (float)sqrt(x*x+y*y);
				float zpos = float(len*((0.5f-sin(pos))*0.25f)*cos(len*0.14f-(time*0.001f)));
	
				glColor4f(1.0f, 1.0f, 1.0f, alpha*0.05f);
				glVertex3f(x-0.0f, y-0.0f, 0.0f-zpos);
				glVertex3f(x+1.0f, y-0.0f, 0.0f-zpos);

				glVertex3f(x+1.0f, y-0.0f, 0.0f-zpos);
				glVertex3f(x+1.0f, y+1.0f, 0.0f-zpos);
				
				glVertex3f(x+1.0f, y+1.0f, 0.0f-zpos);
				glVertex3f(x-0.0f, y+1.0f, 0.0f-zpos);

				glVertex3f(x-0.0f, y+1.0f, 0.0f-zpos);
				glVertex3f(x-0.0f, y-0.0f, 0.0f-zpos);
				
				*data++;				
			}
		}

		glEnd();
	}
	glPopMatrix();

	heart->calculate(pos*4);
	heart->draw(Vector(0, 10.0f, -80.f+(float)pow(pos, 1.7f)*40), pos, alpha);

	write(40, 40, pos, 0.3f, 0.65f, alpha, "teknofontti.pcx", "I observe");
	write(640, 670, pos, 0.60f, 0.95f, alpha, "luontofontti.pcx", "My heart");
	write(650, 700, pos, 0.63f, 0.98f, alpha, "luontofontti.pcx", "is beating");

}

