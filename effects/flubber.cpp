#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Flubber.hpp"

void Flubber::draw()
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

//#define _BLURHASSAKKA_

	renderScene(pos, alpha);

	write(320,    240, pos, 0.2f, 0.5f, alpha, "teknofontti.pcx", "I am the way");
	write(320-16, 275, pos, 0.25f, 0.55f, alpha, "teknofontti.pcx", "and the truth");
	write(320   , 310, pos, 0.3f, 0.6f, alpha, "teknofontti.pcx", "and the life");
	write(96    , 380, pos, 0.5f, 0.8f, alpha, "teknofontti.pcx", "no one comes to the future");
	write(368   , 415, pos, 0.55f, 0.85f, alpha, "teknofontti.pcx", "but by me");
//	write(

}


void Flubber::renderScene(float pos, float alpha)
{
	int x, y, i;


	glLoadIdentity();
	glTranslatef(0, 0, -20.0f); 
	dmsSetFOV(100);
	drawTausta(pos, alpha); 

	myEnable(MY_TEXTURE_2D);

	dmsSetFOV(45.0f); 
	glLoadIdentity();
//	glTranslatef(-0.67f, 0, -2.3f);
	glTranslatef(0, 0, -2.1f);

	updateSpikes(alpha*alpha*alpha*0.7f, pos*2);
	morph(pos*2.5f);
	calculateNormals();

	Matrix valorotate, rotate;
	valorotate.makeRotation(pos*7, 0, 0);
	rotate.makeRotation(0, pos*10, 0);
	Vector valo = Vector(1, 0, 0.8f)*valorotate + Vector(-0.8f*0, 0, 0.3f);
	for (i=0;i<yres*slices;i++)
	{
		const float ambient = 0.1f;
		const float maxlight = 5;
		vertices[i].rnormal = vertices[i].normal*rotate;
		float l = (ambient+valo.dotProduct(vertices[i].rnormal)*maxlight)*alpha;

		vertices[i].light = l;
		vertices[i].position *= rotate;
	}
	
//	Texture *colormap = dmsGetTexture("flubber2"); //;//v7
	Texture *envmap = dmsGetTexture("kromi.pcx");//juupo

	for (y=0;y<yres-1;y++)
	{
/*
		myDisable(MY_BLEND);
		//tekstuuri
		glBindTexture(GL_TEXTURE_2D, colormap->getID());
		glBegin(GL_TRIANGLE_STRIP);
		glColor3f(vertices[y*slices].light, vertices[y*slices].light, vertices[y*slices].light);
		glTexCoord2f(vertices[y*slices].u, vertices[y*slices].v);
		glVertex3f(vertices[y*slices].position.x,
				   vertices[y*slices].position.y, 
				   vertices[y*slices].position.z);
		glColor3f(vertices[(y+1)*slices].light, vertices[(y+1)*slices].light, vertices[(y+1)*slices].light);
		glTexCoord2f(vertices[(y+1)*slices].u, vertices[(y+1)*slices].v);
		glVertex3f(vertices[(y+1)*slices].position.x,
				   vertices[(y+1)*slices].position.y, 
				   vertices[(y+1)*slices].position.z);
		for (x=1;x<slices;x++)
		{
			glColor3f(vertices[y*slices+x].light, vertices[y*slices+x].light, vertices[y*slices+x].light);
			glTexCoord2f(vertices[y*slices+x].u, vertices[y*slices+x].v);
			glVertex3f(vertices[y*slices+x].position.x,
					   vertices[y*slices+x].position.y, 
					   vertices[y*slices+x].position.z);
			glColor3f(vertices[(y+1)*slices+x].light, vertices[(y+1)*slices+x].light, vertices[(y+1)*slices+x].light);
			glTexCoord2f(vertices[(y+1)*slices+x].u, vertices[(y+1)*slices+x].v);
			glVertex3f(vertices[(y+1)*slices+x].position.x,
					   vertices[(y+1)*slices+x].position.y, 
					   vertices[(y+1)*slices+x].position.z);

		}

		//texture-u määritelty suoraan, koska openGL ei tajua pyöräyttää
		//tekstuurikoordinaattia ympäri -> paskaraita
		glColor3f(vertices[y*slices].light, vertices[y*slices].light, vertices[y*slices].light);
		glTexCoord2f(2, vertices[y*slices].v);
		glVertex3f(vertices[y*slices].position.x,
				   vertices[y*slices].position.y, 
				   vertices[y*slices].position.z);
		glColor3f(vertices[(y+1)*slices].light, vertices[(y+1)*slices].light, vertices[(y+1)*slices].light);
		glTexCoord2f(2, vertices[(y+1)*slices].v);
		glVertex3f(vertices[(y+1)*slices].position.x,
				   vertices[(y+1)*slices].position.y, 
				   vertices[(y+1)*slices].position.z);
		glEnd();
*/
		//envmap
		const float envcolor = 1.5f*alpha;
		myEnable(MY_BLEND);
//		myDisable(MY_BLEND);

// FLUBBER FIX: 
// Sul ei ollu Z-bufferit päällä :) Ilmeisesti toi piirtää ton just sopivasti sillä blendauksella
// Il
//
//		myDisable(MY_DEPTH_TEST);
//		myDisable(MY_DEPTH_MASK);
		
		myEnable(MY_DEPTH_TEST);
		myEnable(MY_DEPTH_MASK);

		myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		
		glBindTexture(GL_TEXTURE_2D, envmap->getID());
		glBegin(GL_TRIANGLE_STRIP);
		glColor4f(alpha, alpha, alpha, vertices[y*slices].light*envcolor);
		glTexCoord2f(vertices[y*slices].rnormal.x, vertices[y*slices].rnormal.y);
		glVertex3fv((float *)&vertices[y*slices].position);
		glColor4f(alpha, alpha, alpha, vertices[(y+1)*slices].light*envcolor);
		glTexCoord2f(vertices[(y+1)*slices].rnormal.x, vertices[(y+1)*slices].rnormal.y);
		glVertex3fv((float *)&vertices[(y+1)*slices].position);

		for (x=1;x<slices;x++)
		{
			glColor4f(alpha, alpha, alpha, vertices[y*slices+x].light*envcolor);
			glTexCoord2f(vertices[y*slices+x].rnormal.x, vertices[y*slices+x].rnormal.y);
			glVertex3f(vertices[y*slices+x].position.x,
					   vertices[y*slices+x].position.y, 
					   vertices[y*slices+x].position.z);
			glColor4f(alpha, alpha, alpha, vertices[(y+1)*slices+x].light*envcolor);
			glTexCoord2f(vertices[(y+1)*slices+x].rnormal.x, vertices[(y+1)*slices+x].rnormal.y);
			glVertex3f(vertices[(y+1)*slices+x].position.x,
					   vertices[(y+1)*slices+x].position.y, 
					   vertices[(y+1)*slices+x].position.z);

		}
		glColor4f(alpha, alpha, alpha, vertices[y*slices].light*envcolor);
		glTexCoord2f(vertices[y*slices].rnormal.x, vertices[y*slices].rnormal.y);
		glVertex3f(vertices[y*slices].position.x,
				   vertices[y*slices].position.y, 
				   vertices[y*slices].position.z);
		glColor4f(alpha, alpha, alpha, vertices[(y+1)*slices].light*envcolor);
		glTexCoord2f(vertices[(y+1)*slices].rnormal.x, vertices[(y+1)*slices].rnormal.y);
		glVertex3f(vertices[(y+1)*slices].position.x,
				   vertices[(y+1)*slices].position.y, 
				   vertices[(y+1)*slices].position.z);
		glEnd();
		myEnable(MY_DEPTH_TEST);
		myEnable(MY_DEPTH_MASK);
	}
//	glDepthMask(GL_TRUE);
}


void Flubber::updateSpikes(float power, float pos)
{
	int i;
	int x, y;
	memset(spikebuffer, 0, yres*slices*sizeof(float));
	for (i=0;i<spikecount;i++)
	{
		spikes[i].calculatePos((float)slices, 1.5f*(float)yres, pos);
	}
	for (y=0;y<yres;y++)
	{
		float ysin = (float)sin(y*0.143 + pos*57);
		for (x=0;x<slices;x++)
		{
			float xsin = (float)sin(x*0.056 + pos*74.5f);
			spikebuffer[x+y*slices] = (xsin + ysin)*0.02f;
		}
	}
	for (i=0;i<spikecount;i++)
	{
		drawSpike(spikes[i].x, spikes[i].y, power*0.7f);
	}
}

void Flubber::drawSpike(float xpos, float ypos, float power)
{
	int ystart, yend;
	int xstart, xend;
	int tx, ty;
	int tystart;
	int x, y;
	ystart = (int)ypos;
	yend = ystart + spikeyres;
	tystart = 0;

	xstart = (int)xpos;
	xend = xstart + spikexres;

	//pysyvät vakiona koko kuvan piirron ajan
	float fracx = xpos - (int)xpos;
	float fracy = ypos - (int)ypos;

	//kokonaan näkymättömissä?
	if (ypos + spikeyres < 0 || ypos > yres)
		return;

	//yläpää. Jos bugittaa vielä, tässä on todennäköinen syyllinen
	if (ypos < 1)
	{
		ystart = 1;
		tystart = -(int)(ypos)+1;
	}
	//klipataan alapää (hihihi)
	//-2, koska bilerppi syö yhden ja x-klippauksen puute toisen
	if (ypos > yres-spikeyres-2)
	{
		yend = yres-2;
	}

	ty = tystart;

	for (y=ystart;y<yend;y++)
	{
		tx = 0;
		for (x=xstart;x<xend;x++)
		{
			float sval = spikedata[tx+ty*spikeyres]*power;
			spikebuffer[x+y*slices] += (1-fracy)*(1-fracx)*sval;
			spikebuffer[x+1+y*slices] += (1-fracy)*(fracx)*sval;
			spikebuffer[x+(y+1)*slices] += (fracy)*(1-fracx)*sval;
			spikebuffer[x+1+(y+1)*slices] += (fracy)*(fracx)*sval;
			tx++;
		}
		ty++;
	}
}


void Flubber::morph(float pos)
{
	int x, y;
	FlubberVertex *v = vertices;

	float *spikeptr = spikebuffer;

	for (y=0;y<yres;y++)
	{
		float alpha = 0.0f;
		Matrix looprotate;
		looprotate.makeRotation(0, 2*3.141592f*(float)(y*0.01f + pos*1.4f), 0);
		for (x=0;x<slices;x++)
		{
			Vector vec = v->old;
			float rad = 0.3f*(float)sin(pos*6+0.03*y) + 0.1f*(float)cos(y*0.5+pos*3);

			//luonnollinen morffaus
			vec.x = vec.x*(1-rad) + vec.x * rad*(float)sin(alpha+pos*15);
			vec.z = vec.z*(1-rad) + vec.z * rad*(float)cos(alpha+pos*13);

			//piikit
			const float sval = *spikeptr++;
			vec.x += sval*(float)sin(alpha);
			vec.z += sval*(float)cos(alpha);

			//keskikohta
			const float cangle = 0.14f*y + pos*17;
			const float cangle2 = (0.11f*y + pos*20.0f)*0.7f;
			const float twistpower = 0.10f;
			vec.x += twistpower * (float)(sin(cangle)+cos(cangle2));
			vec.z += twistpower * (float)(cos(cangle)+sin(cangle2));

			v->position = vec*looprotate;

			alpha += 2*3.141592f / (float)slices;
			v++;
		}
	}
}
void Flubber::calculateNormals()
{
	int i;
	const int facecount = (yres-1)*(slices-1)*2; 
	const int vertexcount = yres*slices;
	FlubberFace *faceptr = faces;

	for (i=0;i<vertexcount;i++)
	{
		vertices[i].normal = Vector(0, 0, 0);
	}

	for (i=0;i<facecount;i++)
	{
		Vector v1 = faceptr->b->position - faceptr->a->position;
		Vector v2 = faceptr->c->position - faceptr->a->position;
		v1.normalize();
		v2.normalize();
		Vector normal = v1.crossProduct(v2);
		faceptr->normal = normal;
		faceptr->a->normal += normal*(1.0f/6.0f);
		faceptr->b->normal += normal*(1.0f/6.0f);
		faceptr->c->normal += normal*(1.0f/6.0f);
		faceptr++;
	}

}

void FlubberSpike::calculatePos(float xmul, float ymul, float pos)
{
	x = xmul * (float)sin(xangle * pos * speed);
	y = ymul * (float)sin(yangle * pos * speed);
	power = 0.3f+0.7f*(float)sin(pangle * pos * pspeed);

}

void Flubber::drawTausta(float pos, float alpha)
{
	int i;
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE); 
	myDisable(MY_TEXTURE_2D); 
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST); 
	myEnable(MY_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	glLineWidth(2.0f); 
	
	glColor4f(1,1,1,alpha*0.3f); 

	float ypos = -9.0f; 
	glBegin(GL_LINES);
	for (i=0;i<taustacount;i++)
	{
		const float fov = taustalenghts[i];
		const float kulma = taustapos[i] + pos*taustaspeed[i];
		const float radius = taustaradiuses[i]*20;

		Vector v1 = Vector(radius * (float)sin(kulma-fov), ypos, radius * (float)cos(kulma-fov));
		Vector v2 = Vector(radius * (float)sin(kulma+fov), ypos, radius * (float)cos(kulma+fov));

		glVertex3fv((float *)&v1); 
		glVertex3fv((float *)&v2); 
		ypos += 18.0f / taustacount;
	}
	glEnd();
	glLineWidth(1.0f); 

	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
	myDisable(MY_BLEND);
}

Flubber::Flubber()
{	
	int i;
	int x, y;
	yres = 70;//100;
	slices = 42;//64;
	spikecount = 20;
	spikexres = 10;//12;//25;
	spikeyres = 10;//12;//25;

	taustacount = 10000;
	taustaradiuses = new float[taustacount];
	taustalenghts = new float[taustacount];
	taustaspeed = new float[taustacount];
	taustapos = new float[taustacount];

	srand(405919); 

	for (i=0;i<taustacount;i++)
	{
		const float maxradius = 4.0f;
		const float minradius = 1.0f;
		const float minlength = 0.01f;
		const float maxlength = 0.03f;

		const float minspeed = 3.0f;
		const float maxspeed = 8.0f; 

		taustaradiuses[i] = minradius + ((rand()%10000)/10000.0f)*(maxradius-minradius); 
		taustalenghts[i] = minlength + ((rand()%10000)/10000.0f)*(maxlength - minlength); 
		taustaspeed[i] = minspeed + ((rand()%10000)/10000.0f)*(maxspeed - minspeed);
		taustapos[i] = 2*3.141592f * ((rand()%10000)/10000.0f); 
	}

	vertices = new FlubberVertex[yres*slices];
	faces = new FlubberFace[(yres)*(slices)*2];

	const float ysize = 2.0f;
	float ypos = -ysize*0.5f;

	srand(16);

	for (y=0;y<yres;y++)
	{
		float alpha = 0.0f;
		float u = 0.0f;
		for (x=0;x<slices;x++)
		{
			const float radius = 0.26f;
			const float radius2 = 0.26f;
			vertices[x+y*slices].old = Vector(radius*(float)sin(alpha), 
									  		   ypos,
											   radius2*(float)cos(alpha));
			vertices[x+y*slices].u = u*2;
			vertices[x+y*slices].v = ypos;

			alpha += 2*3.141592f / slices;
			u += (1.0f / (slices));
		}
		ypos += ysize / yres;
	}

	spikedata = new float[spikexres*spikeyres];
	spikes = new FlubberSpike[spikecount];

	for (x=0;x<spikecount;x++)
	{
		const float minspeed = 0.5f;
		const float maxspeed = 0.7f;
		const float minpspeed = 0.5f;
		const float maxpspeed = 1.2f;
		spikes[x].speed = minspeed + ((rand()%10000)/10000.0f)*(maxspeed-minspeed);
		spikes[x].pspeed = -(minpspeed + ((rand()%10000)/10000.0f)*(maxpspeed-minpspeed));
		spikes[x].xangle = ((rand()%10000)/10000.0f)*2*3.141592f;
		spikes[x].yangle = ((rand()%10000)/10000.0f)*2*3.141592f;
		spikes[x].pangle = ((rand()%10000)/10000.0f)*2*3.141592f;
	}

	for (y=0;y<spikeyres;y++)
	{
		for (x=0;x<spikexres;x++)
		{
			const float maxdist = (float)sqrt((spikexres*0.5f)*(spikexres*0.5f)+(spikeyres*0.5f)*(spikeyres*0.5f)); 
			const float dist = (float)sqrt((x-spikexres*0.5f)*(x-spikexres*0.5f) + 
										   (y-spikeyres*0.5f)*(y-spikeyres*0.5f));

			if (dist > maxdist)
			{
				spikedata[x+y*spikexres] = 0.0f;
			}
			else
			{
				const float height = 0.95f;
				const float val = height - (height * (dist / maxdist));
				spikedata[x+y*spikexres] = val*val*val*val;
//				char b[170];
//				sprintf(b, "spikedata[%d, %d] = %f", x, y, spikedata[x+y*spikexres]);
//				loki->writeToLog(b);
			}


		}
	}
	spikebuffer = new float [yres*slices];

	FlubberFace *kasvot = faces;
	for (y=0;y<yres;y++)
	{
		for (x=0;x<slices;x++)
		{
			//vasen yläkulma
			kasvot->a = &vertices[x+y*(slices+1)]; 
			//oikea alakulma
			kasvot->b = &vertices[x+1+(y+1)*(slices+1)];
			//vasen alakulma
			kasvot->c = &vertices[x+(y+1)*(slices+1)];
			kasvot++;
			//vasen yläkulma
			kasvot->a = &vertices[x+y*(slices+1)]; 
			//oikea yläkulma 
			kasvot->b = &vertices[x+1+y*(slices+1)]; 
			//oikea alakulma
			kasvot->c = &vertices[x+1+(y+1)*(slices+1)];
			kasvot++;
		}
	}

}

Flubber::~Flubber()
{
	delete [] vertices;
	delete [] faces;
	delete [] spikedata;
	delete [] spikes;
	delete [] spikebuffer;
	delete [] taustaradiuses;
	delete [] taustalenghts; 
	delete [] taustaspeed;
	delete [] taustapos;
}

bool Flubber::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

