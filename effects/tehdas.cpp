#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include "tehdas.hpp"

Tikku::Tikku(Vector &keskipiste, float pituus, float range, int suunta, float looping)
{
	length = pituus;
	repeat = looping;
	orientation = suunta;
	switch(orientation)
	{
		case 0: endpoint1 = keskipiste + Vector(-range, 0, 0); endpoint2 = keskipiste + Vector(range, 0, 0); break;
		case 1: endpoint1 = keskipiste + Vector(0, -range, 0); endpoint2 = keskipiste + Vector(0, range, 0); break;
		case 2: endpoint1 = keskipiste + Vector(0, 0, -range); endpoint2 = keskipiste + Vector(0, 0, range); break;
	}
}

void Tikku::calcPos(float pos)
{
	float t = (float)(0.5f + 0.5f*sin(pos*2*3.141592f*repeat));
	switch(orientation)
	{
		case 0: 
		{
			v1 = endpoint1*t + endpoint2*(1-t) + Vector(-length, 0, 0);
			v2 = endpoint1*t + endpoint2*(1-t) + Vector(length, 0, 0);
		} break;

		case 1: 
		{
			v1 = endpoint1*t + endpoint2*(1-t) + Vector(0, -length, 0);
			v2 = endpoint1*t + endpoint2*(1-t) + Vector(0, length, 0);
		} break;

		case 2: 
		{
			v1 = endpoint1*t + endpoint2*(1-t) + Vector(0, 0, -length);
			v2 = endpoint1*t + endpoint2*(1-t) + Vector(0, 0, length);
		} break;
	}
}

void Tikku::draw(float alpha)
{
	glColor3fv((float *)&(color*alpha));
	glVertex3fv((float *)&v1);
	glVertex3fv((float *)&v2);

}

Laatta::Laatta(float alkuaika, float loppuaika, Vector &startpos, Vector &endpos, 
			   float leveys, float korkeus, Vector &vari, int suunta)
{
	starttime = alkuaika;
	endtime = loppuaika;
	start = startpos;
	stop = endpos;
	width = leveys;
	height = korkeus;
	color = vari;
	orientation = suunta;
}

void Laatta::calcPos(float pos)
{
	float t;

	if (pos < starttime)
		return;
	if (pos >= starttime && pos < endtime)
		t = (pos-starttime) / (endtime - starttime);
	if (pos >= endtime)
		t = 1.0f;

	Vector keskipiste = start*(1-t) + stop*t;
	switch(orientation)
	{
		case 0: 
		{
			v1 = keskipiste + Vector(-width, -height, 0); 
			v2 = keskipiste + Vector(width, -height, 0); 
			v3 = keskipiste + Vector(width, height, 0); 
			v4 = keskipiste + Vector(-width, height, 0); 
		}; break;

		case 1: 
		{
			v1 = keskipiste + Vector(-width, 0, -height); 
			v2 = keskipiste + Vector(width, 0, -height); 
			v3 = keskipiste + Vector(width, 0, height); 
			v4 = keskipiste + Vector(-width, 0, height); 
		}; break;

		case 2: 
		{
			v1 = keskipiste + Vector(0, -width, -height); 
			v2 = keskipiste + Vector(0, width, -height); 
			v3 = keskipiste + Vector(0, width, height); 
			v4 = keskipiste + Vector(0, -width, height); 
		}; break;
	}
}

void Laatta::draw(float pos, float alpha)
{
	if (pos <= starttime)
		return;
	if (pos >= starttime && pos < endtime)
	{
		float t = (pos-starttime) / (endtime - starttime);
		glColor3fv((float *)&(color*t*alpha));
	}
	if (pos >= endtime)
	{
		glColor3fv((float *)&(color*alpha));
	}
//	glTexCoord2f(0, 0);
	glVertex3fv((float *)&v1);
//	glTexCoord2f(1, 0);
	glVertex3fv((float *)&v2);
//	glTexCoord2f(1, 1);
	glVertex3fv((float *)&v3);
//	glTexCoord2f(0, 1);
	glVertex3fv((float *)&v4);
}

void Tehdas::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.96f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
//	dmsMsg("pos = %f, alpha = %f\n", pos, alpha);

	write(40, 40, pos, 0.35f, 0.55f, alpha, "teknofontti.pcx", "I am strong");
	write(40, 70, pos, 0.40f, 0.60f, alpha, "teknofontti.pcx", "I am modern");
	write(40, 100, pos, 0.45f, 0.65f, alpha, "teknofontti.pcx", "I cannot be bent");

	write(430, 660, pos, 0.65f, 0.85f, alpha, "luontofontti.pcx", "You are inflexible");
	write(460, 690, pos, 0.70f, 0.90f, alpha, "luontofontti.pcx", "You are immoral");
	write(490, 720, pos, 0.75f, 0.95f, alpha, "luontofontti.pcx", "You shall rust");
}

void Tehdas::drawAlusta(float ppos, float alpha)
{
	int i;
	const int alustoja = 7;
	const float baseypos = -1.7f;
	const float startdist = 18.0f;
	const float enddist = 0.8f;
	
	float pos = (float)sin(ppos*3.141592f*0.5f);
	pos *= pos;

	glBegin(GL_QUADS);
	for (i=0;i<alustoja;i++)
	{
		const float angle = (float)i*(2*3.141592f / (float)alustoja);
		const float fov = 0.2f;
		const float dist = enddist + (startdist-enddist)*(1-pos);

/*
		Vector loppupos = Vector(dist*(float)sin(angle), ypos, dist*(float)cos(angle));
		Vector alkupos = Vector(startdist*(float)sin(angle), ypos, startdist*(float)cos(angle));
		glColor3f(0, 0, 0);
		glVertex3fv((float *)&alkupos);
		glColor3f(alpha, alpha, alpha);
		glVertex3fv((float *)&loppupos);
*/
		const int strips = 80;
		//mennään dististä startdistiin

		for (int j=0;j<strips;j++)
		{
			const float kohta = (float)j/(float)strips;
			const float kohta2 = (float)(j+1)/(float)strips;

			const float d1 = dist + (startdist-dist)*kohta;
			const float d2 = dist + (startdist-dist)*kohta2;

//			float ypos = baseypos + (float)pow(1-kohta, 32);
//			float ypos2 = baseypos + (float)pow(1-kohta2, 32);

			float ypos = baseypos + (float)pow(1-(d1 / startdist), 35)*3.5f;
			float ypos2 = baseypos + (float)pow(1-(d2 / startdist), 35)*3.5f;

			Vector c1 = Vector(d1*(float)sin(angle-fov), ypos, d1*(float)cos(angle-fov));
			Vector c2 = Vector(d2*(float)sin(angle-fov), ypos2, d2*(float)cos(angle-fov));
			Vector c3 = Vector(d2*(float)sin(angle+fov), ypos2, d2*(float)cos(angle+fov));
			Vector c4 = Vector(d1*(float)sin(angle+fov), ypos, d1*(float)cos(angle+fov));

//			Vector color = Vector(1.0f, 0.7f, 0.3f)*alpha*(1-kohta);
//			Vector color2 = Vector(1.0f, 0.7f, 0.3f)*alpha*(1-kohta2);
			Vector color = Vector(0.45f, 0.45f, 0.45f)*alpha*(1-kohta);
			Vector color2 = Vector(0.45f, 0.45f, 0.45f)*alpha*(1-kohta2);

			glColor3fv((float *)&color);
			glVertex3fv((float *)&c4);
			glVertex3fv((float *)&c1);
			glColor3fv((float *)&color2);
			glVertex3fv((float *)&c2);
			glVertex3fv((float *)&c3);
/*
			Vector c5 = Vector(d1*(float)sin(angle-fov), -ypos, d1*(float)cos(angle-fov));
			Vector c6 = Vector(d2*(float)sin(angle-fov), -ypos2, d2*(float)cos(angle-fov));
			Vector c7 = Vector(d2*(float)sin(angle+fov), -ypos2, d2*(float)cos(angle+fov));
			Vector c8 = Vector(d1*(float)sin(angle+fov), -ypos, d1*(float)cos(angle+fov));

			glColor3fv((float *)&color);
			glVertex3fv((float *)&c8);
			glVertex3fv((float *)&c5);
			glColor3fv((float *)&color2);
			glVertex3fv((float *)&c6);
			glVertex3fv((float *)&c7);
*/			
		}
	}
	glEnd();


}

void Tehdas::renderScene(float pos, float alpha)
{
	int i;
	glLoadIdentity();
//	glTranslatef(0, 0, -10.0f);

	myDisable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myEnable(MY_LINE_SMOOTH);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//	glRotatef(pos*350, -0.5f, 0.3f, 0.1f);

	//laatat
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
	myBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

	Vector kamerapos = kamera->getValue(pos);
//	dmsSetFOV(45);
	gluLookAt(kamerapos.x, kamerapos.y, kamerapos.z, 0, 0, 0, 0, 1, 0);


//	myEnable(MY_TEXTURE_2D);
//	Texture *laattateksture = dmsGetTexture("databall.jpg"); 
//	glBindTexture(GL_TEXTURE_2D, laattateksture->getID()); 


	float *eq = dmsGetSpectrum();

	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE); 
	
	glBegin(GL_QUADS);
	for (i=0;i<laattacount;i++)
	{
		laatat[i]->calcPos(pos);
		laatat[i]->draw(pos/* *(0.8f + 0.2f*eq[(i+137)%511])*/, alpha*(0.4f + 1.95f*eq[i%511]));
	}
//	myDisable(MY_TEXTURE_2D); 
	glEnd();
	myDisable(MY_BLEND);

	glLineWidth(2.0f);
	glColor3f(alpha, alpha, alpha);
	myDisable(MY_DEPTH_MASK); //depthmaski pois
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//alusta
	const float alustastart = 0.07f;
	const float alustastop = 0.19f;
	float alustapos;

	if (pos < alustastart)
		alustapos = 0;
	if (pos >= alustastart && pos <= alustastop)
		alustapos = (pos-alustastart) / (alustastop-alustastart);
	if (pos > alustastop)
		alustapos = 1.0f;

	drawAlusta(alustapos, alpha);

	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_BLEND);

	//tikut
	glBegin(GL_LINES);
	for (i=0;i<tikkucount;i++)
	{
		tikut[i]->calcPos(pos);
		tikut[i]->draw(alpha);
	}
	glEnd();

	glLineWidth(2.5f);
	const float harmaa = 0.0;//86f;
	glColor4f(alpha*harmaa, alpha*harmaa, alpha*harmaa, 1);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const float putkistostarttime = 0.48f;
	const float putkistostoptime =  0.60f;

	int raja;
	float fraqraja;
	if (pos < putkistostarttime)
		raja = 0;
	if (pos > putkistostoptime)
		raja = putkistocount;
	if (pos >= putkistostarttime && pos <= putkistostoptime)
	{
		raja = (int)(putkistocount * (pos-putkistostarttime)/(putkistostoptime-putkistostarttime));
		fraqraja = (putkistocount * (pos-putkistostarttime)/(putkistostoptime-putkistostarttime))-raja;
	}
/*	
	glBegin(GL_LINE_STRIP);
	for (i=0;i<raja;i++)
	{
		glVertex3fv((float *)&putkisto[i]);
	}
	if (raja < putkistocount)
	{
		Vector last = putkisto[raja-1]*(1-fraqraja) + putkisto[raja]*(fraqraja);
		glVertex3fv((float *)&last);

	}
	glEnd();
*/
	myDisable(MY_BLEND);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
}
 
Tehdas::Tehdas()
{	
	int i;
	tikkucount = 1000;
	tikut = new Tikku*[tikkucount];

	const float cubesize = 1.4f;
	const float cubesize2 = 1.6f;
	const float maxtikkux = cubesize2;
	const float mintikkux = -cubesize2;
	const float maxtikkuy = cubesize2;
	const float mintikkuy = -cubesize2;
	const float maxtikkuz = cubesize2;
	const float mintikkuz = -cubesize2;

	srand(160);
	for (i=0;i<tikkucount;i++)
	{
		const float tx = (rand()%10000)/10000.0f;
		const float ty = (rand()%10000)/10000.0f;
		const float tz = (rand()%10000)/10000.0f;

		Vector tikkupos = Vector(tx*maxtikkux+(1-tx)*mintikkux, 
								   ty*maxtikkuy+(1-ty)*mintikkuy, 
								   tz*maxtikkuz+(1-tz)*mintikkuz); 
		//Tikku(Vector &keskipiste, float pituus, float range, int suunta, float looping);

		const float minloop = 4;
		const float maxloop = 12;
		const float loopt = (rand()%10000)/10000.0f;
		
		const float minlength = 0.08f; //0.04f
		const float maxlength = 0.20f; //0.15f
		const float lengtht = (rand()%10000)/10000.0f;

		const float minrange = 0.1f;
		const float maxrange = 0.3f;
		const float ranget = (rand()%10000)/10000.0f;

		tikut[i] = new Tikku(tikkupos, minlength*lengtht + maxlength*(1-lengtht), 
							 minrange*ranget + maxrange*(1-ranget), i%3, minloop*loopt + maxloop*(1-loopt));

		const float mincolor = 0.4f;
		const float maxcolor = 1.0f;
		const float colort = (rand()%10000)/10000.0f;
		tikut[i]->color = Vector(1,1,1)*(mincolor + (maxcolor-mincolor)*colort);
	}

	laattacount = 2000;
	laatat = new Laatta*[laattacount];

	for (i=0;i<laattacount;i++)
	{
		//	Laatta(float alkuaika, float loppuaika, Vector &startpos, Vector &endpos, 
		//		   float leveys, float korkeus, Vector &vari, int suunta);

		const float laattastart = 0.12f; //kaikki laatat liikkeelle vasta tämän jälkeen
		const float laattastop = 0.32f; //ja kaikki liikkeelle tähän mennessä
		const float startt = ((rand()%10000)/10000.0f) * (laattastop - laattastart);
		const float minaika = 0.04f; //minimiaika, joka laatalta kuluu alusta loppuun
		const float maxaika = 0.07f; //maksimiaika 
		const float aikat = (rand()%10000)/10000.0f;
		const float alkuaika = laattastart + startt;
		const float loppuaika = alkuaika + minaika + (maxaika-minaika)*aikat;

		//alussa laattojen täytyy olla kuution ulkopuolella
		Vector startpos;
		//etäisyydet, jotka laatat ovat alussa kuution ulkopuolella
		const float alkuminx = 5.0f;
		const float alkumaxx = 7.0f; 
		const float alkuminy = 5.0f;
		const float alkumaxy = 7.0f; 
		const float alkuminz = 5.0f;
		const float alkumaxz = 7.0f; 
		const float tx = (rand()% 30000)/30000.0f;
		const float ty = (rand()% 30000)/30000.0f;
		const float tz = (rand()% 30000)/30000.0f;
		const float tx2 = (rand()%30000)/30000.0f;
		const float ty2 = (rand()%30000)/30000.0f;
		const float tz2 = (rand()%30000)/30000.0f;
/*
		switch(rand()%2)
		{
			case 0: startpos.x = -cubesize - (alkuminx*tx + alkumaxx*(1-tx)); break;
			case 1: startpos.x = cubesize + (alkuminx*tx + alkumaxx*(1-tx)); break;
		}
		switch(rand()%2)
		{
			case 0: startpos.y = -cubesize - (alkuminy*ty + alkumaxy*(1-ty)); break;
			case 1: startpos.y = cubesize + (alkuminy*ty + alkumaxy*(1-ty)); break;
		}
		switch(rand()%2)
		{
			case 0: startpos.z = -cubesize - (alkuminz*tz + alkumaxz*(1-tz)); break;
			case 1: startpos.z = cubesize + (alkuminz*tz + alkumaxz*(1-tz)); break;
		}
		//lopussa laattojen täytyy olla kuution sisällä
		Vector endpos;
		const float tx2 = (rand()%10000)/10000.0f;
		const float ty2 = (rand()%10000)/10000.0f;
		const float tz2 = (rand()%10000)/10000.0f;

		endpos = Vector(-cubesize*tx2 + cubesize*(1-tx2), 
						 -cubesize*ty2+cubesize*(1-ty2), 
						 -cubesize*tz2 + cubesize*(1-tz2));
*/

		Vector endpos;
		int orientation;
		
		switch(rand()%4)
		{
			case 0: startpos.x = -cubesize - (alkuminx*tx + alkumaxx*(1-tx));
					startpos.y = -cubesize*ty2 + cubesize*(1-ty2);
					startpos.z = -cubesize*tz2 + cubesize*(1-tz2);
					endpos.x = -cubesize*tx2 + cubesize*(1-tx2);
					endpos.y = startpos.y;
					endpos.z = startpos.z; 
					orientation = 2; break;

			case 1: startpos.x = cubesize + (alkuminx*tx + alkumaxx*(1-tx));
					startpos.y = -cubesize*ty2 + cubesize*(1-ty2);
					startpos.z = -cubesize*tz2 + cubesize*(1-tz2);
					endpos.x = -cubesize*tx2 + cubesize*(1-tx2);
					endpos.y = startpos.y;
					endpos.z = startpos.z; 
					orientation = 2; break;
/*
			case 2: startpos.x = -cubesize*tx2 + cubesize*(1-tx2);
					startpos.y = -cubesize - (alkuminy*ty + alkumaxy*(1-ty));
					startpos.z = -cubesize*tz2 + cubesize*(1-tz2);
					endpos.x = startpos.x;
					endpos.y = -cubesize*ty2 + cubesize*(1-ty2);
					endpos.z = startpos.z; 
					orientation = 1; break;
*/
			case 2: startpos.x = -cubesize*tx2 + cubesize*(1-tx2);
					startpos.y = cubesize + (alkuminy*ty + alkumaxy*(1-ty));
					startpos.z = -cubesize*tz2 + cubesize*(1-tz2);
					endpos.x = startpos.x;
					endpos.y = -cubesize*ty2 + cubesize*(1-ty2);
					endpos.z = startpos.z; 
					orientation = 1; break;
/*
			case 3: startpos.x = -cubesize*tx2 + cubesize*(1-tx2);
					startpos.y = -cubesize*ty2 + cubesize*(1-ty2); 
					startpos.z = -cubesize - (alkuminz*tz + alkumaxz*(1-tz));
					endpos.x = startpos.x;
					endpos.y = startpos.y; 
					endpos.z = -cubesize*tz2 + cubesize*(1-tz2); 
					orientation = 0; break;
*/
			case 3: startpos.x = -cubesize*tx2 + cubesize*(1-tx2);
					startpos.y = -cubesize*ty2 + cubesize*(1-ty2); 
					startpos.z = cubesize + (alkuminz*tz + alkumaxz*(1-tz));
					endpos.x = startpos.x;
					endpos.y = startpos.y; 
					endpos.z = -cubesize*tz2 + cubesize*(1-tz2); 
					orientation = 0; break;
		}

/*
		startpos.x = -cubesize + ((rand()%10000)/10000.0f)*cubesize*2;
		startpos.y = -cubesize + ((rand()%10000)/10000.0f)*cubesize*2;
		startpos.z = -cubesize + ((rand()%10000)/10000.0f)*cubesize*2;

		endpos.x = -cubesize*tx2 + cubesize*(1-tx2);
		endpos.y = -cubesize*ty2 + cubesize*(1-ty2);
		endpos.z = -cubesize*tz2 + cubesize*(1-tz2);
		orientation = rand()%3;
*/

		float minr, maxr, ming, maxg, minb, maxb;
		const float gray = 1.0f;
		switch(rand()%2)
		{
			case 0: minr = gray; maxr = gray; ming = gray; maxg = gray; minb = gray; maxb = gray; break; //harmaa
			case 1: minr = 0.6f; maxr = 0.8f;
					ming = 0.15f; maxg = 0.25f;
					minb = 0.15f; maxb = 0.25f; break; //punainen
		}
		const float tr = (rand()%10000)/10000.0f;
		const float tg  = (rand()%10000)/10000.0f;
		const float tb = (rand()%10000)/10000.0f;

		Vector color = Vector(tr*minr + (1-tr)*maxr, tg*ming + (1-tg)*maxg, tb*minb + (1-tb)*maxb);
		const float minbright = 0.4f;
		const float maxbright = 0.86f;
		const float brightt = (rand()%10000)/10000.f;
		const float brightness = minbright + brightt*(maxbright-minbright);

		const float minwidth = 0.1f;
		const float maxwidth = 0.25f;
		const float widtht = (rand()%10000)/10000.0f;
		const float width = minwidth + (maxwidth-minwidth)*widtht;

		const float minheight = 0.1f;
		const float maxheight = 0.25f;
		const float heightt = (rand()%10000)/10000.0f;
		const float height = minheight + (maxheight-minheight)*heightt;

		laatat[i] = new Laatta(alkuaika, loppuaika, startpos, endpos, width, height, color*brightness, i%3);
	}

	putkistocount = 1000;
	srand(16510459);
	const float lowerlimit = cubesize;
	const float upperlimit = cubesize*1.5f;

	Vector pos = Vector(-cubesize*1.3f, -cubesize*1.3f, -cubesize*1.3f);
	putkisto = new Vector[putkistocount];

	for (i=0;i<putkistocount;i++)
	{
		uudestaan:
			const float minstep = 0.4f;
			const float maxstep = 0.65f;
			float step = minstep + ((rand()%10000)/10000.0f)*(maxstep-minstep);
			Vector delta;
			switch(rand()%6)
			{
				case 0: delta = Vector(-step, 0, 0); break;
				case 1: delta = Vector(step, 0, 0); break;
				case 2: delta = Vector(0, -step, 0); break;
				case 3: delta = Vector(0, step, 0); break;
				case 4: delta = Vector(0, 0, -step); break;
				case 5: delta = Vector(0, 0, step); break; 
			}
			Vector result = pos + delta;
			if (result.x > -upperlimit && result.x < upperlimit &&
				result.y > -upperlimit && result.y < upperlimit &&
				result.z > -upperlimit && result.z < upperlimit)
			{
				pos = pos + delta;
				putkisto[i] = pos;
			}
			else
				goto uudestaan;
	}

	kamera = new Path();
	kamera->addpoint(Vector(0, 0, -17.0f));
	kamera->addpoint(Vector(8, 5, 0.0f));
	kamera->addpoint(Vector(-7, 3, 6));


}

Tehdas::~Tehdas()
{
	int i;
	for (i=0;i<tikkucount;i++)
	{
		delete tikut[i];
	}
	delete[] tikut;

	for (i=0;i<laattacount;i++)
	{
		delete laatat[i];
	}
	delete[] laatat;
	delete[] putkisto;

//	kamera->destroy();
	delete kamera;
}

bool Tehdas::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}
/*
void Tehdas::update()
{	
}

void Tehdas::subInit()
{
}

*/