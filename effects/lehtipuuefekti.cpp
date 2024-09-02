#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Lehtipuuefekti.hpp"
#include "../lib/lista.hpp"

void Lehtipuuefekti::draw()
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

	write(490, 40, pos, 0.30f, 0.45f, alpha, "luontofontti.pcx", "I am root");
	write(510, 70, pos, 0.35f, 0.50f, alpha, "luontofontti.pcx", "I am stem");
	write(530, 100, pos, 0.40f, 0.55f, alpha, "luontofontti.pcx", "I am leaf");

	write(40, 640, pos, 0.70f, 0.85f, alpha, "teknofontti.pcx", "You are rot");
	write(40, 670, pos, 0.75f, 0.90f, alpha, "teknofontti.pcx", "You are old");
	write(40, 700, pos, 0.80f, 0.95f, alpha, "teknofontti.pcx", "You shall wither away");

}

Lehtipuu::Lehtipuu(Lista *vert, Lista *col, Lista *leaf)
{
	//tämä on varmaan hitusen sekava, eka kerta kun käytän verteksitaulukoita. Ei sentään vuoda
	//muistia... 

	dmsMsg("Lehtipuun konstruktori\n");

	int i;
	char b[160];
	Lehti *leafList; //lehden data

	float *ptr1;
	int offs = 0;

	sprintf(b, "Lehtipuuefektin verteksit = %d", vert->getNodeCount());

	vert->goToStart();
	col->goToStart();

	nodecount = vert->getNodeCount();	//col-listassa yhtä monta nodea.. toivottavasti :) 
	leafcount = leaf->getNodeCount();

	//varataan muisti lopullisille verteksitaulukoille
	branchArray = new float[3*nodecount];
	branchColorArray = new float[nodecount*4];

	leafArray = new float[3*leafcount*3];
	leafColorArray = new float[leafcount*4*3];

	dmsMsg("%x %x %x %x\n", branchArray, branchColorArray, leafArray, leafColorArray );

	//kopioidaan tavara oksien verteksitaulukoihin
	ptr1 = branchColorArray;
	offs = 0;
	for (i=0;i<nodecount;i++)
	{
		Vector position = *(Vector *)vert->getCurrent();
		Vector colortemp = *(Vector *)col->getCurrent();

		*ptr1++ = colortemp.x;
		*ptr1++ = colortemp.y;
		*ptr1++ = colortemp.z;
		*ptr1++ = 0; //alphakomponetti

		branchArray[offs++] = position.x;
		branchArray[offs++] = position.y;
		branchArray[offs++] = position.z;

		vert->goForward();
		col->goForward();
	}
	dmsMsg("oksien verteksitaulukot kopioitu\n");

	//lineListissä Lehtipuuefektin verteksit, skaalataan se ykkösen kokoiseksi

	double longest = 0;
	Vector painopiste = Vector(0, 0, 0);
	vert->goToStart();
	
	for (i=0;i<nodecount;i++)
	{
		Vector p = *(Vector *)vert->getCurrent();
		float value = p.x*p.x + p.y*p.y + p.z*p.z;
		if (value > longest)
			longest = value;

		painopiste += p;
		vert->goForward();
	}
	longest = 1.0f / sqrt(longest);
	painopiste *= 1.0f / nodecount;

	offs = 0;
	for (i=0;i<nodecount;i++)
	{
		branchArray[offs] -= painopiste.x;
		branchArray[offs++] *= (float)longest;
		branchArray[offs] -= painopiste.y;
		branchArray[offs++] *= (float)longest;
		branchArray[offs] -= painopiste.z;
		branchArray[offs++] *= (float)longest;
	}
	dmsMsg("oksien verteksitaulukko skaalattu ja siirretty\n");

	//sitten lehtien vuoro

	leaf->goToStart();
	leafcount = leaf->getNodeCount();
	leaf->goToStart();
	leafList = new Lehti[leafcount];
	dmsMsg("leaflist = %x\n", leafList);

	for (i=0;i<leafcount;i++)
	{
		leafList[i] = *(Lehti *)leaf->getCurrent();
		leafList[i].v1 -= painopiste;
		leafList[i].v2 -= painopiste;
		leafList[i].v3 -= painopiste;

		const float leafdist = 1.07f; //lehtien etäisyys oksista, visuaalinen juttu mutta ei luonnollinen :)

		leafList[i].v1 *= (float)longest*leafdist;
		leafList[i].v2 *= (float)longest*leafdist;
		leafList[i].v3 *= (float)longest*leafdist;
		leaf->goForward();
	}
	dmsMsg("kamat leaftlistissa\n");

	//kopioidaan leafList leafArrayhin
	offs = 0;
	int offs2 = 0;
	for (i=0;i<leafcount;i++)
	{
		leafArray[offs++] = leafList[i].v1.x;
		leafArray[offs++] = leafList[i].v1.y;
		leafArray[offs++] = leafList[i].v1.z;
		leafArray[offs++] = leafList[i].v2.x;
		leafArray[offs++] = leafList[i].v2.y;
		leafArray[offs++] = leafList[i].v2.z;
		leafArray[offs++] = leafList[i].v3.x;
		leafArray[offs++] = leafList[i].v3.y;
		leafArray[offs++] = leafList[i].v3.z;
		for (int j=0;j<3;j++)
		{
			leafColorArray[offs2++] = leafList[i].color.x;
			leafColorArray[offs2++] = leafList[i].color.y;
			leafColorArray[offs2++] = leafList[i].color.z;
			leafColorArray[offs2++] = 0; //alphakomponentti, joka asetetaan piirtovaiheessa
		}
	}
	dmsMsg("leaflist kopioitu verteksitaulukkoon\n");
	delete [] leafList;
}

void Lehtipuu::destroy()
{
	if (branchArray != 0)
		delete [] branchArray;
	if (branchColorArray != 0)
		delete [] branchColorArray;
	if (leafArray != 0)
		delete [] leafArray;
	if (leafColorArray != 0)
		delete [] leafColorArray;
}

void Lehtipuu::draw(Vector &pos, Vector &rot, float rotamount, float alpha)
{
	int i;
	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);

	myEnable(MY_LINE_SMOOTH);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glLoadIdentity();
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rotamount, rot.x, rot.y, rot.z);
	
	myEnable(MY_BLEND);
	myEnable(MY_DEPTH_MASK); 
	myDisable(MY_DEPTH_TEST);

	//maski päällä, test pois päältä
	glLineWidth(4.0f);

	//laitetaan oksien colortaulukkoon alphat oikein
	for (i=3;i<nodecount*4;i+=4)
	{
		branchColorArray[i] = 0.67f*alpha;
	}

	//piirretään oksat

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
 	glVertexPointer(3, GL_FLOAT, 0, branchArray);
	glColorPointer(4, GL_FLOAT, 0, branchColorArray);
	glDrawArrays(GL_LINES, 0, nodecount);
	glLineWidth(1.0f);

	//oksat piirretty, sitten lehdet
	myEnable(MY_DEPTH_TEST);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //blending mode näin.. kusee kyllä hitusen
	myEnable(MY_BLEND);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);

	//lehtien colortaulukkoon alphavaluet.. 
	if (alpha < 0.9f)
	{
		for (i=3;i<leafcount*12;i+=4)
		{
			leafColorArray[i] = alpha;
		}
	}

	//piirretään lehdet. 
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, leafArray);
	glColorPointer(4, GL_FLOAT, 0, leafColorArray);
	glDrawArrays(GL_TRIANGLES, 0, leafcount*3);

	myDisable(MY_BLEND);
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_TEXTURE_2D);
}


void Lehtipuuefekti::renderScene(float pos, float alpha)
{
//	Vector tausta = Vector(0.04f, 0.10f, 0.04f)*alpha;
	glLoadIdentity();
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);

	float *eq = dmsGetSpectrum();

	myEnable(MY_TEXTURE_2D);
	myDisable(MY_BLEND);
	Texture *tausta = dmsGetTexture("nousu.jpg");

	const float tausta_alpha = 1.0f; //+ 0.6f*eq[((int)(pos*1615))%63];

	glBindTexture(GL_TEXTURE_2D, tausta->getID());
	dmsPerspective2D(640, 480);
	glBegin(GL_QUADS);

	//DIRTY HÄK!

	if (pos < 0.5f)
		glColor3f(1,1,1);
	else
		glColor3f(alpha, alpha, alpha);

	glTexCoord2f(0, 1);
	glVertex2f(0, 0);
	glTexCoord2f(1, 1);
	glVertex2f(640, 0);
	glTexCoord2f(1, 0);
	glVertex2f(640, 480);
	glTexCoord2f(0, 0);
	glVertex2f(0, 480);
	glEnd();
	dmsPerspective3D();

	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST); 

	/*
	win.setPerspective2D();
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);
	glColor3fv((float *)&tausta);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(640, 0);
	glVertex2f(640, 480);
	glVertex2f(0, 480);
	glEnd();
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);

	win.setPerspective3D();
*/

	const float puu1start = 0.0f;
	const float puu1stop = 0.35f;
	const float puu2start = 0.35f;
	const float puu2stop = 0.70f;
	const float puu3start = 0.70f;
	const float puu3stop = 1.00f;

	const float puu1pos = (pos - puu1start) / (puu1stop - puu1start);
	const float puu2pos = (pos - puu2start) / (puu2stop - puu2start);
	const float puu3pos = (pos - puu3start) / (puu3stop - puu3start);

	const float maxalpha = 1.0f;

	float puu1alpha = (float)sin(puu1pos*3.141492f)*5;
	float puu2alpha = (float)sin(puu2pos*3.141492f)*3;
	float puu3alpha = (float)sin(puu3pos*3.141492f)*3;
	if (puu1alpha > maxalpha) 
		puu1alpha = maxalpha;
	if (puu2alpha > maxalpha) 
		puu2alpha = maxalpha;
	if (puu3alpha > maxalpha) 
		puu3alpha = maxalpha;
/*
	Vector paikka1 = Vector(-0.25f-0.7f*(float)pow(puu1pos*puu1pos*puu1pos,2), 0, -1.8f+puu1pos*0.76f);
	Vector rotaatio1 = Vector(1, 0.7f*(float)sin(1.5f*3.1415982f*puu1pos), 0.7f);
	Vector paikka2 = Vector(0.57f, puu2pos*0.3f, -2.6f+puu2pos*0.66f);
	Vector rotaatio2= Vector(1, -0.9f, 0.7f);
	Vector paikka3 = Vector(-0.57f+puu3pos*0.4f, (1-puu3pos)*0.2f, -1.7f+puu3pos*1.02f);
	Vector rotaatio3 = Vector(1, 1, 1.5f);
*/
	Vector paikka1 = Vector(-0.25f-0.7f*(float)pow(puu1pos*puu1pos*puu1pos,2), 0, -1.8f+puu1pos*0.76f);
	Vector rotaatio1 = Vector(1, 0.7f*(float)sin(1.5f*3.1415982f*puu1pos), 0.7f);
	Vector paikka2 = Vector(-0.57f, puu2pos*0.3f, -2.6f+puu2pos*0.66f);
	Vector rotaatio2= Vector(1, -0.9f, 0.7f);
	Vector paikka3 = Vector(-0.57f+puu3pos*0.4f, (1-puu3pos)*0.2f, -1.7f+puu3pos*1.02f);
	Vector rotaatio3 = Vector(1, 1, 1.5f);
	if (pos > puu1start && pos < puu1stop)
		puut[0]->draw(paikka1, rotaatio1, puu1pos*130, alpha * puu1alpha);
	if (pos > puu2start && pos < puu2stop)
		puut[1]->draw(paikka2, rotaatio2, puu2pos*100, alpha * puu2alpha);
	if (pos > puu3start && pos < puu3stop)
		puut[2]->draw(paikka3, rotaatio3, puu3pos*160, alpha * puu3alpha);

}

Lehtipuuefekti::Lehtipuuefekti()
{	
}

void Lehtipuuefekti::generate(Lista *vert, Lista *col, Lista *leaf, int seed)
{
	srand(seed); 

	const float size = 1.0f;
	Vector p1 = Vector(0, -0.5f, 0)*size;
	Vector p2 = Vector(0, 0.5f, 0)*size;

	recurse(&p1, &p2, vert, col, leaf, 0); 

}

void Lehtipuuefekti::recurse(Vector *p1, Vector *p2, Lista *vertices, Lista *colors, Lista *leaf, int depth)
{
	int i;

	const float magnitudemodifier = 1.0f / ((float)pow(depth, 1.8f)+1);

	const float magnitude1 = 1.0f * magnitudemodifier;
	const float magnitude2 = 1.0f * magnitudemodifier;
	const float magnitude3 = 0.6f * magnitudemodifier;

	//ole varovainen näiden kanssa, ei suuria arvoja
	const int minknots = 3;
	const int maxknots = 6;
	const int maxdepth = 8;

	const int minimumdepth = 1; //tätä pienemmillä arvoilla ei lisätä viivoja piirtolistaan

	if (depth > maxdepth)
		return;

	if (depth == maxdepth) //lisätään lehti, ei enää oksaa
	{
		Vector suunta1 = Vector( (float)sin(((rand()%1000)/1000.0f)*3.141592f*2), 
						           (float)sin(((rand()%1000)/1000.0f)*3.141592f*2),
						  		   (float)sin(((rand()%1000)/1000.0f)*3.141592f*2));

		Lehti *temp = new Lehti();

		temp->pos = *p1;
		temp->rotation = Vector( (float)sin(((rand()%1000)/1000.0f)*3.141592f*2), 
					         (float)sin(((rand()%1000)/1000.0f)*3.141592f*2),
							 (float)sin(((rand()%1000)/1000.0f)*3.141592f*2));

		Vector leafColor = Vector(0.1f, 0.3f, 0.1f);
		temp->color = leafColor * (0.7f + 0.4f * (float)((rand()%1000)/1000.0f));

		const float leafsize = 0.01f;
		Matrix rot;
		rot.makeRotation(temp->rotation.x, temp->rotation.y, temp->rotation.z);

		Vector c1 = (Vector(0, -1, 0)*rot)*leafsize;
		Vector c2 = (Vector(-1, 1, 0)*rot)*leafsize;
		Vector c3 = (Vector(1, 1, 0)*rot)*leafsize;

		temp->normal = (c1-c2).crossProduct(c1-c3);
		temp->normal.normalize();
		temp->v1= temp->pos + c1;
		temp->v2 = temp->pos + c2;
		temp->v3 = temp->pos + c3;
		leaf->add(temp);
		return;
	}

	const float depthpos = (float)depth/(float)maxdepth;
	Vector startcol = Vector(0.4f, 0.7f, 0.3f);
	Vector endcol = Vector(0.8f, 0.3f, 0.1f);

	const Vector currentColor = (startcol*(1-depthpos*depthpos))+endcol*depthpos*depthpos;
	
	Vector suunta1 = Vector( (float)sin(((rand()%1000)/1000.0f)*3.141592f*2), 
					           (float)sin(((rand()%1000)/1000.0f)*3.141592f*2),
					  		   (float)sin(((rand()%1000)/1000.0f)*3.141592f*2))*magnitude1;
	Vector suunta2 = Vector( (float)sin(((rand()%1000)/1000.0f)*3.141592f*2), 
					           (float)sin(((rand()%1000)/1000.0f)*3.141592f*2),
					  		   (float)sin(((rand()%1000)/1000.0f)*3.141592f*2))*magnitude2;
	Vector suunta3 = Vector( (float)sin(((rand()%1000)/1000.0f)*3.141592f*2), 
					           (float)sin(((rand()%1000)/1000.0f)*3.141592f*2),
					  		   (float)sin(((rand()%1000)/1000.0f)*3.141592f*2))*magnitude3;

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
	if (depth > minimumdepth)
	{
		for (i=0;i<knots-1;i++)
		{
			//voidaan lisätä nämä pareinakin... tulee raskaampi objekti

			vertices->add(new Vector(points[i].x, points[i].y, points[i].z));
			colors->add(new Vector(currentColor.x, currentColor.y, currentColor.z));
//			vertices->add(new Vector(points[i+1].x, points[i+1].y, points[i+1].z));
//			colors->add(new Vector(currentColor.x, currentColor.y, currentColor.z));
		}
	}
	for (i=0;i<knots-1;i++)
	{
		//arvotaan oksalle paikka
		Vector point = points[i] + suunta3;
		recurse (&points[i], &point, vertices, colors, leaf, depth+1);
//		recurse(&points[i], &points[i+1], lista, depth+1);
	}

	delete [] points;

}

Lehtipuuefekti::~Lehtipuuefekti()
{
	int i;
	for (i=0;i<puita;i++)
	{
		puut[i]->destroy();
		delete puut[i];
	}
	delete [] puut;
}

void Lehtipuuefekti::release()
{
/*	int i;
	for (i=0;i<puita;i++)
	{
		vertexlist[i]->destroy();
		colorlist[i]->destroy();
		leaflist[i]->destroy();
	}
	delete [] vertexlist;
	delete [] colorlist;
	delete [] leaflist;*/
}

bool Lehtipuuefekti::init(unsigned long s, unsigned long e)
{
	int i;
	startTime = s;
	endTime = e;
	active = false;

	srand(6);
	
	const int count = 3;
	const int numerot[count] = { 104, 10600, 5160 };

	puita = count;
	puut = new Lehtipuu*[puita];

	for (i=0;i<count;i++)
	{
		Lista *vert = new Lista();
		Lista *col = new Lista();
		Lista *leaf = new Lista();

		generate(vert, col, leaf, numerot[i]); //150967 on hyvä :) ja 160
		puut[i] = new Lehtipuu(vert, col, leaf);

		vert->destroy();
		col->destroy();
		leaf->destroy();

		delete vert;
		delete col;
		delete leaf;
	}
	return true;
}

