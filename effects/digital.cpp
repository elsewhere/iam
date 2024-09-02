/* TODO: muuta rumat kaupungit hienoiksi ja abstrakteiksi glenz-kuutioiksi */

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "digital.hpp"

void Digital::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.92f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);

	write(40, 40, pos, 0.35f, 0.55f, alpha, "teknofontti.pcx", "I am pure");
	write(40, 70, pos, 0.40f, 0.60f, alpha, "teknofontti.pcx", "I am defined");
	write(40, 100, pos, 0.45f, 0.65f, alpha, "teknofontti.pcx", "I am expanding");

	write(330, 670, pos, 0.55f, 0.75f, alpha, "luontofontti.pcx", "You are insignificant");
	write(350, 700, pos, 0.60f, 0.80f, alpha, "luontofontti.pcx", "You are superficial");
	write(370, 730, pos, 0.65f, 0.85f, alpha, "luontofontti.pcx", "You are formal");
	
	/*	Primitives::textWriter(5, 45, pos, 14, 0.2f, 0.5f, alpha, "Vast networked cities");

  Primitives::textWriter(15, 60, pos, 14, 0.25f, 0.55f, alpha, "Feeding on our");
	Primitives::textWriter(25, 75, pos, 14, 0.30f, 0.60f, alpha, "subconsciousness");

	Primitives::textWriter(415, 50, pos, 14, 0.55f, 0.65f, alpha, "organized");
	Primitives::textWriter(425, 65, pos, 14, 0.58f, 0.68f, alpha, "mechanical");
	Primitives::textWriter(435, 80, pos, 14, 0.61f, 0.71f, alpha, "precise");

	Primitives::textWriter(5, 45, pos, 14, 0.65f, 0.80f, alpha, "permanently");
	Primitives::textWriter(15, 60, pos, 14, 0.68f, 0.83f, alpha, "connecting");
	Primitives::textWriter(25, 75, pos, 14, 0.71f, 0.86f, alpha, "man to machine");
*/
}


void Digital::renderScene(float pos, float alpha)
{
	int i;
	glLoadIdentity();
	Vector kamera = Vector(pos*0.5f, 0.8f+pos*0.3f + 0.2f*(float)sin(pos*3.141592f), -7+pos*5.0f);

	gluLookAt(kamera.x, kamera.y, kamera.z, -pos*2, 0, 0, -pos*0.15f, 1, 0);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	myEnable(MY_LINE_SMOOTH);
	myEnable(MY_BLEND);
	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);

	glBegin(GL_LINES);
	glColor4f(1, 1, 1, alpha*0.2f);

	for (int z=0;z<zdim-1;z++)
	{
		for (int x=0;x<xdim-1;x++)
		{
			//vaakasuora
			glColor4f(1, 1, 1, alpha*grid[x+z*xdim].brightness);
			glVertex3fv((float *)&grid[x+z*xdim].position);
			glColor4f(1, 1, 1, alpha*grid[x+1+z*xdim].brightness);
			glVertex3fv((float *)&grid[x+1+z*xdim].position);

			//pystysuora
			glColor4f(1, 1, 1, alpha*grid[x+z*xdim].brightness);
			glVertex3fv((float *)&grid[x+z*xdim].position);
			glColor4f(1, 1, 1, alpha*grid[x+(z+1)*xdim].brightness);
			glVertex3fv((float *)&grid[x+(z+1)*xdim].position);
/*
			//vino
			glColor4f(1, 1, 1, alpha*grid[x+z*xdim].brightness);
			glVertex3fv((float *)&grid[x+z*xdim].position);
			glColor4f(1, 1, 1, alpha*grid[x+1+(z+1)*xdim].brightness);
			glVertex3fv((float *)&grid[x+1+(z+1)*xdim].position);
*/
		}
	}
	glEnd();

	float *eq = dmsGetSpectrum();
	int index = 0;

	glBegin(GL_QUADS);
	for (z=0;z<zdim-1;z++)
	{
		for (int x=0;x<xdim-1;x++)
		{
			const float maa_r = 0.5f;
			const float maa_g = 0.7f;
			const float maa_b = 1.0f; 
			const float maa_r2 = 0.4f;
			const float maa_g2 = 0.4f;
			const float maa_b2 = 0.4f; 
			const int kohta = (int)(pos*1000);
			const int multiply = x+z;
			const float arvo = 0.5f + 3*(eq[(index*multiply+kohta)%511] + 
								  eq[(index*multiply+kohta+127)%511] + 
								  eq[(index*multiply+kohta+384)%511]);
			index++;

			glColor4f(maa_r*arvo + maa_r2*(1-arvo), maa_g*arvo + maa_g2*(1-arvo), maa_b*arvo + maa_b2*(1-arvo), alpha*grid[x+z*xdim].brightness*arvo);
			glVertex3fv((float *)&grid[x+z*xdim].position);
			glColor4f(maa_r*arvo + maa_r2*(1-arvo), maa_g*arvo + maa_g2*(1-arvo), maa_b*arvo + maa_b2*(1-arvo), alpha*grid[x+1+z*xdim].brightness*arvo);
			glVertex3fv((float *)&grid[x+1+z*xdim].position);
			glColor4f(maa_r*arvo + maa_r2*(1-arvo), maa_g*arvo + maa_g2*(1-arvo), maa_b*arvo + maa_b2*(1-arvo), alpha*grid[x+(z+1)*xdim].brightness*arvo);
			glVertex3fv((float *)&grid[x+1+(z+1)*xdim].position);
			glColor4f(maa_r*arvo + maa_r2*(1-arvo), maa_g*arvo + maa_g2*(1-arvo), maa_b*arvo + maa_b2*(1-arvo), alpha*grid[x+1+(z+1)*xdim].brightness*arvo);
			glVertex3fv((float *)&grid[x+(z+1)*xdim].position);
/*
			glColor4f(maa_r, maa_g, maa_b, alpha*grid[x+z*xdim].brightness*arvo);
			glVertex3fv((float *)&grid[x+z*xdim].position);
			glColor4f(maa_r, maa_g, maa_b, alpha*grid[x+1+z*xdim].brightness*arvo);
			glVertex3fv((float *)&grid[x+1+z*xdim].position);
			glColor4f(maa_r, maa_g, maa_b, alpha*grid[x+1+(z+1)*xdim].brightness*arvo);
			glVertex3fv((float *)&grid[x+1+(z+1)*xdim].position);
			glColor4f(maa_r, maa_g, maa_b, alpha*grid[x+(z+1)*xdim].brightness*arvo);
			glVertex3fv((float *)&grid[x+(z+1)*xdim].position);
*/
		}
	}
	glEnd();

	glBegin(GL_LINES);
	for (i=0;i<regularcount;i++)
	{
		const float width = 0.005f;
		glColor4f(1, 1, 1, alpha*0.67f*regular[i].brightness);
		glVertex3fv((float *)&regular[i].position);
		glVertex3fv((float *)&(regular[i].position + Vector(0, regular[i].height, 0)));
		glVertex3fv((float *)&regular[i].position);
		glVertex3fv((float *)&(regular[i].position + Vector(width, 0, 0)));
		glVertex3fv((float *)&regular[i].position);
		glVertex3fv((float *)&(regular[i].position + Vector(0, 0, width)));

	}
	glEnd();

	//kaupungit
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	for (i=0;i<nodecount;i++)
	{		
		const float width = 0.003f;
	 	glColor4f(1, 1, 1, alpha*0.07f*nodes[i].brightness);
//		building(nodes[i].position, width, nodes[i].height, width);		
		cube(nodes[i].position + Vector(0, 0, width*0.5f*5), width*5); 
	}
/*	//viivat kaupunkien välillä
	glLineWidth(1.5f);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBegin(GL_LINES);

	for (i=0;i<citycount*citylinecount;i++)
	{ 
		const float phase = ((float)i/(float)(citycount*citylinecount))*pos*66.6f;
		const float mul = 0.7f + 0.5f*(float)sin(phase);
		glColor4f(1.0f, 1.0f, 0.8f, citylines[i].brightness*alpha*mul*1.45f);
		glVertex3fv((float *)&citylines[i].position);
	}

	glEnd();
	glLineWidth(1.0f);
*/
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE); 

	for (int j=0;j<4;j++)
	for (i=0;i<citycount;i++)
	{
//		const float cubesize = 0.15f;
		const float cubesize = 0.025f*j;
		glColor4f(1,1,1,alpha*(0.14f)*(4-j)*0.35f); 
		cube(cities[i] + Vector(0, 0.5f*cubesize, 0), cubesize); 
	}


	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_TEXTURE_2D);
	myDisable(MY_BLEND);
}

void Digital::building(Vector &position, float xsize, float ysize, float zsize)
{
	Vector front_down_left = position;
	Vector front_up_left = position + Vector(0, ysize, 0);
	Vector back_up_left = position + Vector(0, ysize, zsize);
	Vector back_down_left = position + Vector(0, 0, zsize);

	Vector front_down_right = position + Vector(xsize, 0, 0); 
	Vector front_up_right = position + Vector(xsize, ysize, 0);
	Vector back_up_right = position + Vector(xsize, ysize, zsize);
	Vector back_down_right = position + Vector(xsize, 0, zsize);

	glBegin(GL_LINES);
	glVertex3fv((float *)&front_down_left);
	glVertex3fv((float *)&front_down_right);
	glVertex3fv((float *)&front_down_left);
	glVertex3fv((float *)&front_up_left);
	glVertex3fv((float *)&front_down_left);
	glVertex3fv((float *)&back_down_left);

	glVertex3fv((float *)&front_up_left);
	glVertex3fv((float *)&back_up_left);
	glVertex3fv((float *)&front_up_left);
	glVertex3fv((float *)&front_up_right);

	glVertex3fv((float *)&front_down_right);
	glVertex3fv((float *)&front_up_right);
	glVertex3fv((float *)&front_down_right);
	glVertex3fv((float *)&back_down_right);

	glVertex3fv((float *)&front_up_right);
	glVertex3fv((float *)&back_up_right);

	glVertex3fv((float *)&back_up_right);
	glVertex3fv((float *)&back_down_right);
	glVertex3fv((float *)&back_up_right);
	glVertex3fv((float *)&back_up_left);

	glVertex3fv((float *)&back_up_left);
	glVertex3fv((float *)&back_down_left);
	glVertex3fv((float *)&back_down_left);
	glVertex3fv((float *)&back_down_right);
	glEnd();
}

void Digital::cube(Vector &position, float size)
{
	Vector up_front_right = position + Vector(size, -size, -size); 
	Vector up_front_left = position  + Vector(-size, -size, -size); 
	Vector up_back_right = position  + Vector(size, -size, size); 
	Vector up_back_left = position   + Vector(-size, -size, size); 

	Vector down_front_right = position + Vector(size, size, -size); 
	Vector down_front_left = position  + Vector(-size, size, -size); 
	Vector down_back_right = position  + Vector(size, size, size); 
	Vector down_back_left = position   + Vector(-size, size, size);

	glBegin(GL_QUADS);

	//ylä
	glVertex3fv((float *)&up_front_right); 
	glVertex3fv((float *)&up_front_left); 
	glVertex3fv((float *)&up_back_left); 
	glVertex3fv((float *)&up_back_right); 

	//ala
	glVertex3fv((float *)&down_front_right); 
	glVertex3fv((float *)&down_front_left); 
	glVertex3fv((float *)&down_back_left); 
	glVertex3fv((float *)&down_back_right); 

	//etu
	glVertex3fv((float *)&up_front_right); 
	glVertex3fv((float *)&up_front_left); 
	glVertex3fv((float *)&down_front_left); 
	glVertex3fv((float *)&down_front_right); 

	//taka
	glVertex3fv((float *)&up_back_right); 
	glVertex3fv((float *)&up_back_left); 
	glVertex3fv((float *)&down_back_left); 
	glVertex3fv((float *)&down_back_right); 

	//vasen
	glVertex3fv((float *)&up_front_left); 
	glVertex3fv((float *)&up_back_left); 
	glVertex3fv((float *)&down_back_left); 
	glVertex3fv((float *)&down_front_left); 

	//oikea
	glVertex3fv((float *)&up_front_right); 
	glVertex3fv((float *)&up_back_right); 
	glVertex3fv((float *)&down_back_right); 
	glVertex3fv((float *)&down_front_right); 


	glEnd();
	

}

Digital::Digital()
{
	int i, j;
	int x, z;
	int index = 0;

	//perusruudukko
	xdim = 60;
	zdim = 60;

	//kaupungit
	citycount = 30;
	citylinecount = 5;
	cities = new Vector[citycount];
	const int linespercity = 300;
	const float cityradius = 0.35f;

	nodecount = citycount*linespercity;
	regularcount = xdim*zdim;
	regular = new DigitalNode[regularcount];
	nodes = new DigitalNode[nodecount];

	citylines = new LineGridNode[citylinecount*citycount*2];

	const float zsize = 15;
	const float xsize = 15;
	const float zstep = (float)zsize / zdim;
	const float xstep = (float)xsize / xdim;
	float zpos;
	float xpos;

	const float heightmodifier = 0.7f;
	const float cityheightmodifier = 0.1f;

	//gridi
	gridcount = xdim*zdim;
	grid = new LineGridNode[gridcount];

	index = 0;

	zpos = -zsize * 0.5f;
	float maxdist = 1.0f;

	const float pisin = (float)sqrt(xsize*0.5f*xsize*0.5f); //olettaen, että vehje on neliö
	for (z=0;z<zdim;z++)
	{
		xpos = -xsize * 0.5f;
		for (x=0;x<xdim;x++)
		{

			const float plasmaval = 0.5f + 0.25f*(float)sin(x*0.5f + z*0.3f) + 0.25f*(float)cos(x*z*0.01f);
			const float minkorkeus = 0.0f;
			const float maxkorkeus = plasmaval*0.1f + 0.1f;

			const float korkeus = minkorkeus + ((rand()%10000)/10000.0f)*(maxkorkeus-minkorkeus); 
			Vector p = Vector(xpos, korkeus, zpos);
			const float height = 0.04f;
			regular[index].position = p;
			float brightness = 1-1.2f*(nodes[index].position.z + zsize*0.5f) / zsize;
			if (brightness < 0)
				brightness = 0;
			regular[index].height = height*heightmodifier;

			grid[index].position = p;

			float dist = p.length();
			if (dist > xsize*0.5f)
				dist = xsize*0.5f; //valokeilan reunalla eli nolla
			dist = dist / pisin;

			const float totalbrightness = 0.25f;
			const float regularbrightness = 0.65f;
			float ambient;

			if (zpos < 0)
				ambient = 0.2f * (float)(-zpos/(zsize*0.5f));
			else
				ambient = 0.0f;

			grid[index].brightness = ambient + (1-dist)*totalbrightness;//p.length();
			regular[index].brightness = ambient + (1-dist)*regularbrightness;

			index++;
			xpos += xstep;
		}
		zpos += zstep;
	}
	srand(14516);

	index = 0;
	for (i=0;i<citycount;i++)
	{
		const float cityx = ((rand()%10000)/10000.0f) * xsize - xsize*0.5f;
		const float cityz = ((rand()%10000)/10000.0f) * zsize - zsize*0.5f;
		Vector citypos = Vector(cityx, 0, cityz);
		cities[i] = citypos;
		for (j=0;j<linespercity;j++)
		{
			const float radius = (float)pow(((rand()%10000)/10000.0f)*cityradius, 1.2f);
			const float randomheightadd = (rand()%10000/10000.0f)*0.8f;
			const float height = (float)pow(1-radius, 8) + randomheightadd;
			const float angle = ((rand()%10000)/10000.0f)*2*3.141592f;

			nodes[index].position = citypos + Vector((float)sin(angle), 0, (float)cos(angle))*radius;
			nodes[index].brightness = (1 -(nodes[index].position.z + zsize*0.5f) / zsize)*(0.6f + randomheightadd*0.5f);
			nodes[index].height = height*cityheightmodifier;
			index++;

		}
	}
	index = 0;

	for (i=0;i<citycount;i++)
	{
		for (j=0;j<citylinecount;j++)
		{
/*		float tmp = grid[x].brightness / maxdist;
		tmp = (float)pow(1-tmp, 1.7f)*0.35f;
		if (tmp > 0.25f) tmp = 0.25f;
		grid[x].brightness = tmp;
*/
			//mistä
			citylines[index].position = cities[i];
			float tmp = cities[i].length() / maxdist;
			tmp = (float)pow(1-tmp, 1.0f)*0.35f;
			if (tmp > 0.2f) tmp = 0.2f;
			if (tmp < 0.0f) tmp = 0.0f;
			citylines[index].brightness = tmp;//0.2f;//tmp;
			index++;

			//mihin

			const float angle = ((rand()%10000)/10000.0f) * 2*3.141592f;
			const float radius = (float)pow((rand()%10000)/10000.0f, 1.5f)*0.2f;
			const Vector randomsuunta = Vector((float)sin(angle), 0, (float)cos(angle))*radius;
			const int offset = rand()%citycount;
			citylines[index].position = cities[(i+offset)%citycount] + randomsuunta;
			tmp = cities[(i+offset)%citycount].length() / maxdist;
			tmp = (float)pow(1-tmp, 1.0f)*0.35f;
			if (tmp > 0.2f) tmp = 0.2f;
			if (tmp < 0.1f) tmp = 0.1f;
			citylines[index].brightness = 0.2f;//tmp;
			index++;
		}
	}
}

Digital::~Digital()
{
	delete[] nodes;
	delete[] grid;
	delete[] citylines;
	delete[] cities;
	delete[] regular;
}

bool Digital::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

