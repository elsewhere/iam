#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Kasviefekti.hpp"

Kasvi::Kasvi(Lista *vertices, Lista *colors)
{
	int i;
	vertices->goToStart();
	colors->goToStart();

	Vector *lineList;
	Vector *colorList;
	nodecount = vertices->getNodeCount();	//col-listassa yhtä monta nodea.. toivottavasti :) 
	lineList = new Vector[nodecount];
	colorList = new Vector[nodecount];
	for (i=0;i<nodecount;i++)
	{
		lineList[i] = *(Vector *)vertices->getCurrent();
		colorList[i] = *(Vector *)colors->getCurrent();
		vertices->goForward();
		colors->goForward();
	}

	//lineListissä kasvin verteksit, skaalataan se ykkösen kokoiseksi

	double longest = 0;
	Vector painopiste = Vector(0, 0, 0);
	for (i=0;i<nodecount;i++)
	{
		float value = lineList[i].x*lineList[i].x + lineList[i].y*lineList[i].y + lineList[i].z*lineList[i].z;
		if (value > longest)
			longest = value;

		painopiste += lineList[i];
	}
	longest = 1.0f / sqrt(longest);
	painopiste *= 1.0f / nodecount;
	for (i=0;i<nodecount;i++)
	{
		lineList[i] -= painopiste;
		lineList[i] *= (float)longest;
	}


	//kopioidaan kama vertex-ja colortaulukoihin
	colorArray = new float [nodecount*4];
	vertexArray = new float [nodecount*3];

	float *cptr = colorArray;
	float *vptr = vertexArray;
	for (i=0;i<nodecount;i++)
	{
		*cptr++ = colorList[i].x;
		*cptr++ = colorList[i].y;
		*cptr++ = colorList[i].z;
		*cptr++ = 0; //alpha

		*vptr++ = lineList[i].x;
		*vptr++ = lineList[i].y;
		*vptr++ = lineList[i].z;
	}

	delete [] lineList;
	delete [] colorList;

}

void Kasvi::draw(Vector &pos, Vector &rot, float rotamount, float alpha)
{
	int i;

	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);

	myEnable(MY_LINE_SMOOTH);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);	

	glLoadIdentity();
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rotamount, rot.x, rot.y, rot.z);

	glLineWidth(2.0f);
	const float maxalpha = 1.0f;
	if (alpha < maxalpha)
	{
		for (i=3;i<nodecount*4;i+=4)
		{
			colorArray[i]=0.20f*alpha;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertexArray);
	glColorPointer(4, GL_FLOAT, 0, colorArray);
	glDrawArrays(GL_LINES, 0, nodecount);

	glLineWidth(1.0f);
	glEnd();

	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_TEXTURE_2D);
}

void Kasvi::destroy()
{
/*	if (lineList != 0)
 		delete [] lineList;
	if (colorList != 0)
		delete [] colorList;*/

	if (vertexArray != 0)
		delete [] vertexArray;
	if (colorArray != 0)
		delete [] colorArray;
}

void Kasviefekti::draw()
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
	write(50, 80, pos, 0.35f, 0.55f, alpha, "luontofontti.pcx", "I am a fractal"); 
	write(520, 710, pos, 0.65f, 0.85f, alpha, "teknofontti.pcx", "You are nothing"); 
//	write(100, 110, pos, 0.45f, 0.65f, alpha, "luontofontti.pcx", 
//	Primitives::textWriter(425, 450, pos, 24, 0.1f, 0.9f, alpha, "Organiks");
}


void Kasviefekti::renderScene(float pos, float alpha)
{
	int i;
//	Vector tausta = Vector(0.04f, 0.10f, 0.04f)*alpha;


	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_TEXTURE_2D);
	Texture *tausta = dmsGetTexture("kasvis.jpg"); 
	glBindTexture(GL_TEXTURE_2D, tausta->getID());
	glLoadIdentity();
	dmsPerspective2D(640, 480); 
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK); 
	glBegin(GL_QUADS);

	const float tausta_bright = 1.0f;
	glColor4f(alpha*tausta_bright, alpha*tausta_bright, alpha*tausta_bright, alpha); 	
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

	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);





	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);

	myEnable(MY_LINE_SMOOTH);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
/*
	win.setPerspective2D();
	glColor3fv((float *)&tausta);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(640, 0);
	glVertex2f(640, 480);
	glVertex2f(0, 480);
	glEnd();
*/
	//neliöt

	myEnable(MY_BLEND);
	for (i=0;i<squarecount;i++)
	{
		const float maxsize = 35.0f * (260-positions[i].x)/260;
		const float speed = 38.0f;
		const float spacing = 0.34f;
		const float fade = 0.07f;//0.7f+0.3f*(float)sin(i*0.01f + pos*5);
		float s = maxsize * (float)(0.6f+0.4f*sin(pos*speed + i*spacing));

		Vector col = Vector(0.20f, 0.4f, 0.15f)*alpha*fade;
		nelio(s, &positions[i], &col, alpha);
	}

//	win.setPerspective3D();
	myEnable(MY_BLEND);

	const float kasvi1start = 0.0f;
	const float kasvi1stop = 0.40f;
	const float kasvi2start = 0.35f;
	const float kasvi2stop = 0.70f;
	const float kasvi3start = 0.62f;
	const float kasvi3stop = 1.00f;

	const float kasvi1pos = (pos - kasvi1start) / (kasvi1stop - kasvi1start);
	const float kasvi2pos = (pos - kasvi2start) / (kasvi2stop - kasvi2start);
	const float kasvi3pos = (pos - kasvi3start) / (kasvi3stop - kasvi3start);

	//jos iteraatioita on paljon, säädä tätä pienemmäksi
	const float maxalpha = 0.7f;

	float kasvi1alpha = (float)sin(kasvi1pos*3.141492f)*5;
	float kasvi2alpha = (float)sin(kasvi2pos*3.141492f)*3;
	float kasvi3alpha = (float)sin(kasvi3pos*3.141492f)*3;
	if (kasvi1alpha > maxalpha) 
		kasvi1alpha = maxalpha;
	if (kasvi2alpha > maxalpha) 
		kasvi2alpha = maxalpha;
	if (kasvi3alpha > maxalpha) 
		kasvi3alpha = maxalpha;

	Vector paikka1 = Vector(-0.25f, 0, -1.8f+kasvi1pos*0.76f);
	Vector rotaatio1 = Vector(1, 0.7f*(float)sin(1.5f*3.1415982f*kasvi1pos), 0.7f);
	Vector paikka2 = Vector(0.21f, 0, -1.6f+kasvi2pos*0.66f);
	Vector rotaatio2= Vector(1, -0.7f, 0.7f);
	Vector paikka3 = Vector(-0.17f, 0, -1.1f+kasvi3pos*0.36f);
	Vector rotaatio3 = Vector(1, 0.2f, 0.3f);

	if (pos >= kasvi1start && pos <= kasvi1stop)
		kasvit[0]->draw(paikka1, rotaatio1, kasvi1pos*130, alpha * kasvi1alpha);
	if (pos >= kasvi2start && pos <= kasvi2stop)
		kasvit[1]->draw(paikka2, rotaatio2, kasvi2pos*100, alpha * kasvi2alpha);
	if (pos >= kasvi3start && pos <= kasvi3stop)
		kasvit[2]->draw(paikka3, rotaatio3, kasvi3pos*160, alpha * kasvi3alpha);

}

void Kasviefekti::nelio(float size, Vector *position, Vector *color, float alpha)
{
/*	glBegin(GL_LINE_LOOP);
	glColor3fv((float *)color);
	glVertex2f(position->x-size, position->y-size);
	glVertex2f(position->x+size, position->y-size);
	glVertex2f(position->x+size, position->y+size);
	glVertex2f(position->x-size, position->y+size);
	glEnd();*/

	glBegin(GL_QUADS);
	glColor3fv((float *)color);
	glVertex2f(position->x-size, position->y-size);
	glVertex2f(position->x+size, position->y-size);
	glVertex2f(position->x+size, position->y+size);
	glVertex2f(position->x-size, position->y+size);
	glEnd();

}

Kasviefekti::Kasviefekti()
{	
	int i;
	squarecount = 400;

	srand(6);
	positions = new Vector[squarecount];
	for (i=0;i<squarecount;i++)
	{
		/*
		float temp = (float)((rand()%1000)/1000.0f);
		float temp2 = (float)((rand()%1000)/1000.0f);

		const float centerx = 160;
		const float centery = 240;
		const float radius = 30+temp * temp * 420;

		positions[i].x = centerx + radius*(float)sin(temp2*2*3.1541592f);
		positions[i].y = centery + radius*(float)cos(temp2*2*3.1541592f);*/

		positions[i].y = (float)(rand()%480);
		switch(rand()%5)
		{
			case 0: positions[i].x = 40; break;
			case 1: positions[i].x = 100; break;
			case 2: positions[i].x = 150; break;
			case 3: positions[i].x = 190; break;
			case 4: positions[i].x = 260; break;
		}
	}

	//vittu C++ ja sen idioottimaisuudet..
	const int count = 3;
	const int numerot[count] = { 1337, 405, 160 };

	kasveja = count;
	kasvit = new Kasvi*[kasveja];

	for (i=0;i<count;i++)
	{
		Lista *vert = new Lista();
		Lista *col = new Lista();

		generate(vert, col, numerot[i]);
		kasvit[i] = new Kasvi(vert, col);
		vert->destroy();
		col->destroy();
		delete vert;

		delete col;
	}
}

void Kasviefekti::generate(Lista *vert, Lista *col, int seed)
{
	srand(seed); 

	const float size = 1.0f;
	Vector p1 = Vector(0, -0.5f, 0)*size;
	Vector p2 = Vector(0, 0.5f, 0)*size;

	recurse(&p1, &p2, vert, col, 0); 

}

void Kasviefekti::recurse(Vector *p1, Vector *p2, Lista *vertices, Lista *colors, int depth)
{
	int i;

	const float magnitudemodifier = 1.0f / ((float)pow(depth, 1.8f)+1);

	const float magnitude1 = 1.0f * magnitudemodifier;
	const float magnitude2 = 1.0f * magnitudemodifier;
	const float magnitude3 = 0.6f * magnitudemodifier;

	//ole varovainen näiden kanssa, ei suuria arvoja
	//3, 7, 8 menee vielä
	//3, 6, 8 on vakio
	//2, 7, 9 näyttää kivalta mutta kestää hetken... (precalcci tiedostoon?) 

	const int minknots = 2; //minimimäärä oksia oksassa
	const int maxknots = 7; //maksimi oksassa
	const int maxdepth = 9; //rekursiosyvyys, yhdeksän melkolailla ehdoton maksimi

	const int listdepth = 0;  //tätä pienemmällä syvyydellä ei laiteta piirtolistaan

	if (depth > maxdepth)
		return;

	const float depthpos = (float)depth/(float)maxdepth;
	Vector startcol = Vector(0.4f, 0.7f, 0.3f);
	Vector endcol = Vector(0.8f, 0.3f, 0.1f);

	const Vector currentColor = (startcol*(1-depthpos*depthpos))+endcol*depthpos*depthpos;
//	const Vector currentColor = Vector((rand()%1000)/1000.0f,(rand()%1000)/2000.0f, (rand()%1000)/4000.0f);
	
	Vector suunta1 = Vector( (float)sin(((rand()%1000)/1000.0f)*3.141592f*2), 
					           (float)sin(((rand()%1000)/1000.0f)*3.141592f*2),
					  		   (float)sin(((rand()%1000)/1000.0f)*3.141592f*2))*magnitude1;
	Vector suunta2 = Vector( (float)sin(((rand()%1000)/1000.0f)*3.141592f*2), 
					           (float)sin(((rand()%1000)/1000.0f)*3.141592f*2),
					  		   (float)sin(((rand()%1000)/1000.0f)*3.141592f*2))*magnitude2;
	Vector suunta3 = Vector( (float)sin(((rand()%1000)/1000.0f)*3.141592f*2), 
					           (float)sin(((rand()%1000)/1000.0f)*3.141592f*2),
					  		   (float)sin(((rand()%1000)/1000.0f)*3.141592f*2))*magnitude3;

	//Tähän ei yli neljää!!!
	const int knots = minknots + (rand()%(maxknots-minknots));
	Vector *points = new Vector[knots];

//	HermiteCurve curve;
//	curve.set(*p1, suunta1, suunta2, *p2);

	CubicBezierCurve curve;
	curve.set(*p1, *p1+suunta1, *p2 + suunta2, *p2);

	i = 0;
	for (float t=0.0f;t<1.0f;t+= 1.0f / knots)
	{
		points[i++] = curve.getValue(t);
	}
	if (depth > listdepth)
	{
		for (i=0;i<knots-1;i++)
		{
			//voidaan lisätä nämä pareinakin... tulee raskaampi objekti

			vertices->add(new Vector(points[i].x, points[i].y, points[i].z));
			colors->add(new Vector(currentColor.x, currentColor.y, currentColor.z));
	//		vertices->add(new Vector(points[i+1].x, points[i+1].y, points[i+1].z));
	//		colors->add(new Vector(currentColor.x, currentColor.y, currentColor.z));
		}
	}
	for (i=0;i<knots-1;i++)
	{
		//arvotaan oksalle paikka
		Vector point = points[i] + suunta3;
		recurse (&points[i], &point, vertices, colors, depth+1);

//		recurse(&points[i], &points[i+1], lista, depth+1);
	}

	delete [] points;

}

Kasviefekti::~Kasviefekti()
{
	int i;
	delete [] positions;

	for (i=0;i<kasveja;i++)
	{
		kasvit[i]->destroy();
		delete kasvit[i];
	}
	delete [] kasvit;


}

bool Kasviefekti::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

