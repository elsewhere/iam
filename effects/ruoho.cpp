#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Ruoho.hpp"
#include "../Mathematics.hpp"

void Voikukka::destroy()
{
	delete [] varsi;
	delete [] haituvat;
}

const float ruohoscale = 100.0f;

void Voikukka::generate()
{
	int i;

	varsicount = 10;
	haituvacount = 200;

	varsi = new Vector[varsicount];
	haituvat = new Vector[haituvacount];

	//arvotaan alkupaikka

	const float maxrad = 1.0f*ruohoscale;
	const float minsway = 0.02f*ruohoscale;
	const float maxsway = 0.03f*ruohoscale;

	float rad = ((rand()%10000)/10000.0f)*maxrad;
	const float angle = ((rand()%10000)/10000.0f)*2*3.141592f;
	const float xpos = rad * (float)sin(angle);
	const float ypos = rad * (float)cos(angle);

	const float swayrad = minsway + ((rand()%10000)/10000.0f)*(maxsway-minsway);
	const float swayangle = ((rand()%10000)/10000.0f)*2*3.141592f;

	const float minheight = 0.12f*ruohoscale;
	const float maxheight = 0.15f*ruohoscale;

	const float height = minheight + (rand()%10000/10000.0f)*(maxheight-minheight);

	Vector juuri = Vector(xpos, ypos, 0);
	Vector latva = Vector(xpos+swayrad*(float)cos(swayangle), ypos+swayrad*(float)sin(swayangle), height);

	const float beta = (rand()%10000/10000.0f)*0.02f;

	Vector tangent1 = Vector(0.02f*(float)sin(beta), 1, 0.02f*(float)cos(beta)).normalize();
	Vector tangent2 = Vector(0.02f*(float)sin(beta), -1, 0.02f*(float)cos(beta)).normalize();
	tangent1 *= 0.04f*ruohoscale;
	tangent2 *= 0.05f*ruohoscale;
	HermiteCurve c;
	c.set(juuri, tangent1, tangent2, latva);

	for (i=0;i<varsicount;i++)
	{
		Vector p = c.getValue(i * (1.0f/(varsicount-1)));
		varsi[i] = p;
	}

	for (i=0;i<haituvacount;i++)
	{
		const float maxradius = 0.017f;
		const float haituvaradius = (rand()%10000/10000.0f)*maxradius*ruohoscale;
		const float theta = (rand()%10000/10000.0f)*2*3.141592f;
		const float gamma = (rand()%10000/10000.0f)*3.141592f;
		haituvat[i] = Mathematics::sphereToCartesian(haituvaradius, theta, gamma) + latva;
	}

}

void Voikukka::draw(float alpha)
{
	int i;
	glLineWidth(1.5f);
//	glColor4f(1, 1, 1, alpha*0.3f);
	glColor4f(0.7f, 1, 0.7f, alpha*0.3f);
	glBegin(GL_LINE_STRIP);
	for (i=0;i<varsicount;i++)
	{
		glVertex3fv((float *)&varsi[i]);
	}
	glEnd();

	glLineWidth(1.0f);
//	glPointSize(5.0f);
/*
	glBegin(GL_POINTS);
	for (i=0;i<haituvacount;i++)
	{
		glVertex3fv((float *)&haituvat[i]);
	}
	glEnd();
	glPointSize(1.0f);
*/
	glBegin(GL_LINES);
	glColor4f(1,1,1,alpha*0.2f);
	for (i=0;i<haituvacount;i++)
	{
		glVertex3fv((float *)&varsi[varsicount-1]);
		glVertex3fv((float *)&haituvat[i]);
	}
	glEnd();

}

void Timotei::destroy()
{
	delete [] varsi;
	delete [] haituvat;
}

void Timotei::generate()
{
	int i;
	varsicount = 4;
	haituvacount = 200;
	varsi = new Vector[varsicount];
	haituvat = new Vector[haituvacount*2];

	//arvotaan alkupaikka

	const float maxrad = 1.0f*ruohoscale;
	const float minsway = 0.02f*ruohoscale;
	const float maxsway = 0.03f*ruohoscale;

	float rad = ((rand()%10000)/10000.0f)*maxrad;
	const float angle = ((rand()%10000)/10000.0f)*2*3.141592f;
	const float xpos = rad * (float)sin(angle);
	const float ypos = rad * (float)cos(angle);

	const float swayrad = minsway + ((rand()%10000)/10000.0f)*(maxsway-minsway);
	const float swayangle = ((rand()%10000)/10000.0f)*2*3.141592f;

	const float minheight = 0.12f*ruohoscale;
	const float maxheight = 0.15f*ruohoscale;

	const float height = minheight + (rand()%10000/10000.0f)*(maxheight-minheight);

	Vector juuri = Vector(xpos, ypos, 0);
	Vector latva = Vector(xpos+swayrad*(float)cos(swayangle), ypos+swayrad*(float)sin(swayangle), height);

	const float beta = (rand()%10000/10000.0f)*0.02f;

	Vector tangent1 = Vector(0.02f*(float)sin(beta), 1, 0.02f*(float)cos(beta)).normalize();
	Vector tangent2 = Vector(0.02f*(float)sin(beta), -1, 0.02f*(float)cos(beta)).normalize();
	tangent1 *= 0.04f*ruohoscale;
	tangent2 *= 0.05f*ruohoscale;
	HermiteCurve c;
	c.set(juuri, tangent1, tangent2, latva);

	for (i=0;i<varsicount;i++)
	{
		Vector p = c.getValue(i * (1.0f/(varsicount-1)));
		varsi[i] = p;
	}
	for (i=0;i<haituvacount;i++)
	{
		float kohta = (rand()%10000)/13000.0f;
		float kulma = ((rand()%10000)/10000.0f)*2*3.141592f;
		float radius = 0.005f*ruohoscale;
		Vector s1 = varsi[varsicount-2]*kohta + varsi[varsicount-1]*(1-kohta);
		Vector s2 = s1 + Vector((float)sin(kulma), (float)cos(kulma), 0)*radius;
		haituvat[i*2]=s1;
		haituvat[i*2+1]=s2;
	}

}
void Timotei::draw(float alpha)
{
	int i;
	glLineWidth(1.5f);
//	glColor4f(1, 1, 1, alpha*0.3f);
	glColor4f(0.7f, 1, 0.7f, alpha*0.3f);
	glBegin(GL_LINE_STRIP);
	for (i=0;i<varsicount;i++)
	{
		glVertex3fv((float *)&varsi[i]);
	}
	glEnd();

	glLineWidth(1.0f);
	glPointSize(2.0f);
/*
	glBegin(GL_POINTS);
	for (i=0;i<haituvacount;i++)
	{
		glVertex3fv((float *)&haituvat[i]);

	}
	glEnd();
	glPointSize(1.0f);*/

	glBegin(GL_LINES);
	glColor4f(1,1,1,alpha*0.3f);
	for (i=0;i<haituvacount;i++)
	{
		glVertex3fv((float *)&haituvat[i*2]);
		glVertex3fv((float *)&haituvat[i*2+1]);
	}
	glEnd();

}


void Ruoho::draw()
{
	static int frames = 0;
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.87f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
//	dmsMsg("frame = %d, pos = %f, alpha = %f\n", frames, pos, alpha);
	frames++;

	write(100, 120, pos, 0.3f, 0.6f, alpha, "luontofontti.pcx",   "I am not organized");
	write(120, 150, pos, 0.35f, 0.65f, alpha, "luontofontti.pcx", "I am not bound by rules");
	write(140, 180, pos, 0.40f, 0.70f, alpha, "luontofontti.pcx", "I am not gentle");

	write(150, 600, pos, 0.55f, 0.85f, alpha, "teknofontti.pcx", "You are fragile");
	write(150, 630, pos, 0.60f, 0.90f, alpha, "teknofontti.pcx", "You are obsolete");
	write(150, 660, pos, 0.65f, 0.95f, alpha, "teknofontti.pcx", "The winter is already here");

	/*
	Primitives::textWriter(485, 420, 0.5f, 25, 0.2f, 0.8f, alpha, "Halla");
	win.setPerspective2D();
	myDisable(MY_TEXTURE_2D);
	glLineWidth(3.0f);
	myEnable(MY_BLEND);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINES);
	glColor4f(1, 1, 1, alpha*0.7f);
	glVertex2f(0, 420);
	glVertex2f(640, 420);
	glVertex2f(0, 450);
	glVertex2f(640, 450);

	glEnd();
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);

	glLineWidth(1.0f);
	win.setPerspective3D();
*/

}



void Ruoho::renderScene(float pos, float alpha)
{
	int i;

	myDisable(MY_TEXTURE_2D);
	myDisable(MY_BLEND);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);

	const float intensity = 0.4f;
	Vector taustavari = Vector(0.1f, 0.3f, 0.04f)*intensity*alpha;

	glColor3fv((float *)&taustavari);
	dmsPerspective2D(640, 480);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(640, 0);
	glVertex2f(640, 480);
	glVertex2f(0, 480);
	glEnd();
	dmsPerspective3D();

	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);

	glLoadIdentity();
//	static int frame = 0;

	const float cam_startmove = 0.13f;
	const float cam_stopmove = 1.0f; 
	float campos = 0.0f;

	if (pos > cam_startmove)
		campos = (pos - cam_startmove) / (cam_stopmove - cam_startmove);
//	campos = (float)sin(campos*3.141592f*0.5f);
//	campos = (float)pow(campos, 1.7f); 
//	dmsMsg("frame = %d, campos = %f\n", frame, campos); 

	const float camrad = 0.4f;
	const float camspeed = 4.0f;
//	frame++;
/*
	Vector camadd = Vector(camrad*(float)cos(campos*camspeed), camrad*(float)sin(campos*camspeed), 
							 0.3f*(float)cos(campos*2*3.141592f));
	Vector cam = Vector(0, -0.8f+campos*0.2f, 0.6f)+camadd;
	cam *= ruohoscale;
	Vector camtarget = Vector(0, 0, 0.2f)*ruohoscale;
	gluLookAt(cam.x, cam.y, cam.z, camtarget.x, camtarget.y, camtarget.z, 0, 0, 1);
*/
	Vector camtarget = Vector(0, 0, 0.2f)*ruohoscale;
	Vector cam = kamera->getValue(campos); 
	gluLookAt(cam.x, cam.y, cam.z, camtarget.x, camtarget.y, camtarget.z, 0, 0, 1); 

	//piirto

	const float ruoho_start = 0.0f;
	const float ruoho_stop = 0.17f; //t‰h‰n menness‰ piirret‰‰n sitten kaikki
	const float voikukka_start = 0.11f;
	const float voikukka_stop = 0.18f; 
	const float timotei_start = 0.12f;
	const float timotei_stop = 0.20f; 

	float ruohopos, voikukkapos, timoteipos;

	if (pos < ruoho_start)
		ruohopos = 0.0f; 
	else if (pos < ruoho_stop)
		ruohopos = (pos - ruoho_start) / (ruoho_stop - ruoho_start);
	else ruohopos = 1.0f; 

	if (pos < voikukka_start)
		voikukkapos = 0.0f; 
	else if (pos < voikukka_stop)
		voikukkapos = (pos - voikukka_start) / (voikukka_stop - voikukka_start);
	else voikukkapos = 1.0f; 

	if (pos < timotei_start)
		timoteipos = 0.0f; 
	else if (pos < timotei_stop)
		timoteipos = (pos - timotei_start) / (timotei_stop - timotei_start);
	else timoteipos = 1.0f; 

	int ruohojapiirretaan = (int)(ruohopos * ruohocount);
	int voikukkiapiirretaan = (int)(voikukkapos * voikukkacount);
	int timoteitapiirretaan = (int)(timoteipos * timoteicount);

	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	myEnable(MY_LINE_SMOOTH);

	glColor4f(1, 1, 1, 0.2f*alpha);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, ruohobuffer);
	glDrawArrays(GL_LINES, 0, ruohojapiirretaan);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY);

	const float voikukka_alpha = voikukkapos*voikukkapos;
	const float timotei_alpha = timoteipos*timoteipos;
	for (i=0;i<voikukkiapiirretaan;i++)
	{
		voikukat[i].draw(alpha*voikukka_alpha);
	}
	for (i=0;i<timoteitapiirretaan;i++)
	{
		timoteit[i].draw(alpha*timotei_alpha);
	}
	myDisable(MY_BLEND);
}
Ruoho::Ruoho()
{	
	int i;
	ruohocount = 60000;
	voikukkacount = 400;//200;
	timoteicount = 250;

	nurmikko = new Ruohonkorsi[ruohocount];
	voikukat = new Voikukka[voikukkacount];
	timoteit = new Timotei[timoteicount];

	srand(16);

	for (i=0;i<ruohocount;i++)
	{
		const float zpos = 0.0f;
		//pohjan leveys
		const float minbasewidth = 0.0005f*ruohoscale;
		const float maxbasewidth = 0.002f*ruohoscale;

		//korkeus
		const float minheight = 0.02f*ruohoscale;
		const float maxheight = 0.08f*ruohoscale;

		//alueet ruohon esiintymiselle

		const float height = minheight + ((rand()%10000)/10000.0f)*(maxheight - minheight);
/*
		const float xpos = minx + ((rand()%10000)/10000.0f) * (maxx - minx);
		const float ypos = miny + ((rand()%10000)/10000.0f) * (maxy - miny);
		const float angle = ((rand()%10000)/10000.0f)*2*3.141592f;
*/
		float rad = (rand()%10000)/10000.0f;
		rad = (float)pow(rad, 0.9f)*ruohoscale;//45f);
		const float angle = ((rand()%10000)/10000.0f)*2*3.141592f;
		const float xpos = rad * (float)sin(angle);
		const float ypos = rad * (float)cos(angle);

		//yl‰reunan poikkeutus
		const float tuuliradius = 0.015f*ruohoscale;
		const float tuulirad = (rand()%10000/10000.0f)*tuuliradius;
		const float tuulikulma = (rand()%10000/10000.0f)*2*3.141592f;

		nurmikko[i].pohja = Vector(xpos, ypos, zpos);
		nurmikko[i].karki = Vector(xpos+tuulirad*(float)sin(tuulikulma), ypos+tuulirad*(float)cos(tuulikulma),
									zpos+height*(float)pow(rad, 0.1f));
	}
	ruohobuffer = new float[3*ruohocount*2];
	int offs = 0;
	for (i=0;i<ruohocount;i++)
	{
		ruohobuffer[offs++] = nurmikko[i].pohja.x;
		ruohobuffer[offs++] = nurmikko[i].pohja.y;
		ruohobuffer[offs++] = nurmikko[i].pohja.z;
		ruohobuffer[offs++] = nurmikko[i].karki.x;
		ruohobuffer[offs++] = nurmikko[i].karki.y;
		ruohobuffer[offs++] = nurmikko[i].karki.z;
	}

	for (i=0;i<voikukkacount;i++)
	{
		voikukat[i].generate();
	}
	for (i=0;i<timoteicount;i++)
	{
		timoteit[i].generate();
	}
	kamera = new Path();
/*
	const float camrad = 0.4f;
	const float camspeed = 4.0f;
	Vector camadd = Vector(camrad*(float)cos(campos*camspeed), camrad*(float)sin(campos*camspeed), 
							 0.3f*(float)cos(campos*2*3.141592f));
	Vector cam = Vector(0, -0.8f+campos*0.2f, 0.6f)+camadd;
	cam *= ruohoscale;
	Vector camtarget = Vector(0, 0, 0.2f)*ruohoscale;
	gluLookAt(cam.x, cam.y, cam.z, camtarget.x, camtarget.y, camtarget.z, 0, 0, 1);
*/
	kamera->addpoint(Vector(1, 1, 0.67f)*ruohoscale); 
	kamera->addpoint(Vector(1, 0.85f, 0.63f)*ruohoscale); 
	kamera->addpoint(Vector(1, 0.4f, 0.6f)*ruohoscale); 
	kamera->addpoint(Vector(0.65f, 0.1f, 0.35f)*ruohoscale); 
	kamera->addpoint(Vector(0.15f, -0.23f, 0.42f)*ruohoscale); 
}

Ruoho::~Ruoho()
{
	int i;
	for (i=0;i<voikukkacount;i++)
		voikukat[i].destroy();
	for (i=0;i<timoteicount;i++)
		timoteit[i].destroy();

	delete [] voikukat;
	delete [] timoteit;
	delete [] nurmikko;
	delete [] ruohobuffer;
	delete kamera;
}

bool Ruoho::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

