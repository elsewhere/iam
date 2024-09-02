#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>
#include "Kasvu.hpp"
#include "../mathematics.hpp"

//#define DRAW_DEBUG

void Kasvu::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.92f;
	const float fadeoutstop = 1.0f;

//	if (pos >= fadeinstart && pos < fadeinstop)
//		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	//frameskip ja nopeus
	lastframetime = frametime;
	frametime = pos; 
	const float effectspeed = 1.3f;
	const float rotatespeed = 155.0f;
	const float flowpower = 1.4f;

	//ajan muutos edelliseen frameen nähden
	const float dt = (float)fabs((frametime - lastframetime) * effectspeed);

//	generatorpoint = Vector((float)cos(pos*16), (float)sin(pos*51), 0);
	const float move = 0.02f;
	if (GetAsyncKeyState('A'))
		generatorpoint += Vector(move, 0, 0);
	if (GetAsyncKeyState('S'))
		generatorpoint -= Vector(move, 0, 0);
	if (GetAsyncKeyState('D'))
		generatorpoint += Vector(0, move, 0);
	if (GetAsyncKeyState('F'))
		generatorpoint -= Vector(0, move, 0);
	if (GetAsyncKeyState('G'))
		generatorpoint += Vector(0, 0, move);
	if (GetAsyncKeyState('H'))
		generatorpoint -= Vector(0, 0, move);
	if (GetAsyncKeyState(VK_SPACE))
	{
		Vector randpos = Vector(((rand()%10000)/10000.0f) - 0.5f,
								  ((rand()%10000)/10000.0f) - 0.5f,
								  ((rand()%10000)/10000.0f) - 0.5f);

		for (int i=0;i<particlecount;i++)
		{
			particles[i].init(randpos);

		}

	}
	updateFlow(pos*rotatespeed, flowpower);
	updateParticles(dt);
	renderScene(pos, alpha);

	write(50, 600+50, pos, 0.15f, 0.90f, alpha, "teknofontti.pcx", "My heart are the power plants");
	write(50, 600+80, pos, 0.25f, 0.90f, alpha, "teknofontti.pcx", "My veins are the power lines");
	write(50, 600+110, pos, 0.35f, 0.90f, alpha, "teknofontti.pcx", "My mouth are the smokestacks");
}

void Kasvu::updateFlow(float pos, float flowpower)
{
	int x, y, z;

	for (z=0;z<zdim;z++)
	{
		for (y=0;y<ydim;y++)
		{
			for (x=0;x<xdim;x++)
			{
				Vector temp = grid[x+y*xdim+z*xdim*ydim].flow + (grid[x+y*xdim+z*xdim*ydim].flowadd*pos);
				grid[x+y*xdim+z*xdim*ydim].currentvector = Mathematics::sphereToCartesian(flowpower, temp.y, temp.z);
			}
		}
	}
}
void Kasvu::updateParticles(float dt)
{
	int i;
	Vector temp = Vector(0, 0, 0);
	for (i=0;i<particlecount;i++)
	{
		const float energydecrease = 30.0f;

		if (particles[i].energy <= 0)
		{
			particles[i].kill();
			particles[i].init(generatorpoint);
			continue; 
		}

		if (particles[i].pos.x > xmax || particles[i].pos.x < xmin ||
			particles[i].pos.y > ymax || particles[i].pos.y < ymin ||
			particles[i].pos.z > zmax || particles[i].pos.z < zmin)
		{
			//ollaan kaukana sivistyksestä.. vähennetään energiaa reiluummin
			particles[i].energy -= energydecrease * dt; 
		}

		//lasketaan, missä ruudussa particle on
		const int x0 = (int)particles[i].pos.x;
		const int y0 = (int)particles[i].pos.y;
		const int z0 = (int)particles[i].pos.z;

		if ((x0 >= xdim) || (x0 < 0) || (y0 >= ydim) || (y0 < 0) || (z0 >= zdim) || (z0 < 0))
		{
			//ollaan kuution rajojen ulkopuolella, vaimennetaan hiukkasta mutta se jatkaa suuntaansa
			particles[i].energy -= energydecrease * dt; 
			particles[i].pos += particles[i].speed;
			continue;
		}

		//x pienenee vasemmalle, suurenee ylöspäin
		//y pienenee alaspäin, suurenee ylöspäin
		//z pienenee ruutuun, suurenee katsojaan päin

		Vector v0 = grid[x0+ y0*xdim + z0*xdim*ydim].currentvector;
		Vector v1 = grid[(x0+1) + y0*xdim + z0*xdim*ydim].currentvector;
		Vector v2 = grid[(x0+1) + y0*xdim + (z0+1)*xdim*ydim].currentvector;
		Vector v3 = grid[x0 + y0*xdim + (z0+1)*xdim*ydim].currentvector;

		Vector v4 = grid[x0+ (y0+1)*xdim + z0*xdim*ydim].currentvector;
		Vector v5 = grid[(x0+1) + (y0+1)*xdim + z0*xdim*ydim].currentvector;
		Vector v6 = grid[(x0+1) + (y0+1)*xdim + (z0+1)*xdim*ydim].currentvector;
		Vector v7 = grid[x0 + (y0+1)*xdim + (z0+1)*xdim*ydim].currentvector;

		//trilerp
		const float fx = particles[i].pos.x - x0;
		const float fy = particles[i].pos.y - y0;
		const float fz = particles[i].pos.z - z0;
		const float rfx = 1-fx;
		const float rfy = 1-fy;
		const float rfz = 1-fz;

		Vector suunta = Vector(0, 0, 0);
		suunta += v0*rfx*rfy*rfz;
		suunta += v1*fx*rfy*rfz;
		suunta += v2*fx*rfy*fz;
		suunta += v3*rfx*rfy*fz;

		suunta += v4*rfx*fy*rfz;
		suunta += v5*fx*fy*rfz;
		suunta += v6*fx*fy*fz;
		suunta += v7*rfx*fy*fz;

		suunta *= dt;
		particles[i].speed += suunta;
		particles[i].pos += particles[i].speed;

		//lisätään piirtolistaan
		if (particles[i].vertexcount < maxvertexcount)
		{
			particles[i].vertices[particles[i].vertexcount] = particles[i].pos;
			particles[i].vertexcount++;
		}
	}
}


void Kasvu::renderScene(float pos, float alpha)
{
	int i, j;

//	glLoadIdentity();
//	glRotatef(pos*50, 0.6f, 0.8f, -0.9f);
//	myEnable(MY_TEXTURE_2D);
//	myDisable(MY_BLEND);
//	myDisable(MY_DEPTH_MASK);
//	myDisable(MY_DEPTH_TEST);

//	const float back = 0.2f;
//	glColor3f(back*alpha, back*alpha, back*alpha);
//	glBindTexture(GL_TEXTURE_2D, tManager->getTextureName("leaves")->getID());
//	Primitives::texturedCube(&Vector(0, 0, 0), 1, 1);

	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_TEXTURE_2D);

	Texture *tausta = dmsGetTexture("huuto.jpg"); 
	glBindTexture(GL_TEXTURE_2D, tausta->getID());
	glLoadIdentity();
	dmsPerspective2D(640, 480); 
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK); 
	glBegin(GL_QUADS);
	const float taustabri = 1.7f;
	glColor4f(1,1,1,1);
//	glColor3f(alpha*taustabri, alpha*taustabri, alpha*taustabri); 
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
	
	glLoadIdentity();
	glTranslatef(0, 0, -3.0f);

	glRotatef(pos*300, 0.6f, 1, 0.2f);
	myDisable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	myEnable(MY_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);


#ifdef DRAW_DEBUG
	int x, y, z;
	//peruskuutio
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINES);
	glColor4f(1,1,1,alpha*0.05f);
	for (z=0;z<zdim-1;z++)
	{
		for (y=0;y<ydim-1;y++)
		{
			for (x=0;x<xdim-1;x++)
			{
				glVertex3fv((float *)&grid[x+y*xdim+z*xdim*ydim].pos);
				glVertex3fv((float *)&grid[(x+1)+y*xdim+z*xdim*ydim].pos);
				glVertex3fv((float *)&grid[x+y*xdim+z*xdim*ydim].pos);
				glVertex3fv((float *)&grid[x+(y+1)*xdim+z*xdim*ydim].pos);
				glVertex3fv((float *)&grid[x+y*xdim+z*xdim*ydim].pos);
				glVertex3fv((float *)&grid[x+y*xdim+(z+1)*xdim*ydim].pos);
			}
		}
	}
	glEnd();

	//flow-vektorit
	glColor4f(1, 0.3f, 0.2f, alpha*1.0f);
	glBegin(GL_LINES);
	for (z=0;z<zdim;z++)
	{
		for (y=0;y<ydim;y++)
		{
			for (x=0;x<xdim;x++)
			{
				glVertex3fv((float *)&grid[x+y*xdim+z*xdim*ydim].pos);
				glVertex3fv((float *)&(grid[x+y*xdim+z*xdim*ydim].pos + grid[x+y*xdim+z*xdim*ydim].currentvector*0.01f));
			}
		}
	}
	glEnd();

#endif
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (i=0;i<particlecount;i++)
	{
		const float fade = 2.0f * particles[i].energy;

		//koita GL_LINES ja maxwidth = 10
		const float minwidth = 1.0f;
		const float maxwidth = 6.0f;
		const float widthspeed = 150;
		const float widthspacing = 0.5f;

		const float width = minwidth + (0.5f+0.5f*(float)cos(widthspeed*pos+i*widthspacing))*(maxwidth-minwidth);

/*		Vector c = Vector(0.5f+0.5f*(float)sin(i*0.4f+pos*11), 
							0.5f+0.5f*(float)cos(i*0.021f+pos*17), 
							0.5f+0.5f*(float)sin(i*0.057f+pos*43));
		glColor4f(c.x, c.y, c.z ,alpha*0.3f*fade);*/

		glLineWidth(width);
		glBegin(GL_LINE_STRIP);
		for (j=0;j<particles[i].vertexcount;j++)
		{
//			glColor4f(0.6f,0.3f, 0.2f,alpha*0.3f*fade);
			glColor4f(0.3f,0.45f, 0.7f,alpha*0.3f*fade);
			glVertex3fv((float *)&particles[i].vertices[j]);
		}
		glEnd();
	}
	glLineWidth(1.0f);

/*
	myEnable(MY_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tManager->getTextureName("particle")->getID());
	glColor4f(1, 1, 1, 0.2f*alpha);
	for (i=0;i<particlecount;i++)
	{
		Primitives::drawFlare(&particles[i].vertices[particles[i].vertexcount], 0.03f);

	}
*/
	myDisable(MY_BLEND);
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
}

void KasvuParticle::init(Vector &position)
{
	const float particleradius = 0.25f;
	const float particleminspeed = 0.01f;
	const float particlemaxspeed = 0.03f;

	const float maxenergy = 0.4f;
	const float minenergy = 0.1f;

	float phi = (rand()%10000/10000.0f)*2*3.141592f;
	float theta = (rand()%10000/10000.0f)*3.141592f;
	float rad = ((rand()%10000)/10000.0f)*particleradius;

	pos = position + Mathematics::sphereToCartesian(rad, phi, theta);

	phi = (rand()%10000/10000.0f)*2*3.141592f;
	theta = (rand()%10000/10000.0f)*3.141592f;
	rad = particleminspeed + ((rand()%10000)/10000.0f)*(particlemaxspeed - particleminspeed);

	speed = Mathematics::sphereToCartesian(rad, phi, theta);
	energy = minenergy + ((rand()%10000)/10000.0f)*(maxenergy - minenergy);

	vertexcount = 0;
}

void KasvuParticle::kill()
{
}

Kasvu::Kasvu()
{	
	int i;
	int x, y, z;
	xdim = 30;
	ydim = 30;
	zdim = 30;

	srand(167);

	particlecount = 1000;
	maxvertexcount = 80;

	frametime = 0;
	lastframetime = 0;

	grid = new KasvuNode[xdim*ydim*zdim];
	particles = new KasvuParticle[particlecount];

	for (i=0;i<particlecount;i++)
	{
		particles[i].vertices = new Vector[maxvertexcount];
		particles[i].vertexcount = 0;
	}

	const float xsize = 2;
	const float ysize = 2;
	const float zsize = 2;

	xmin = -xsize*0.5f;
	ymin = -ysize*0.5f;
	zmin = -zsize*0.5f;

	xmax = xsize*0.5f;
	ymax = ysize*0.5f;
	zmax = zsize*0.5f;

	float xpos = xmin;

	Vector cum = Vector(0, 0, 0);
	for (x=0;x<xdim;x++)
	{
		float ypos = ymin;
		for (y=0;y<ydim;y++)
		{
			float zpos = zmin;
			for (z=0;z<zdim;z++)
			{
				int offset = x + y*xdim + z*xdim*ydim;
				grid[offset].pos = Vector(xpos, ypos, zpos);

				//saattavat pyöriä epäsuhtaisesti johtuen phi:n ja thetan pyörähtämisestä ympäri eri kohdissa
				const float flowstrength = 0.01f;
				const float flowrotate = 1.0f;
				float phi = (rand()%10000/10000.0f)*2*3.141592f;
				float theta = (rand()%10000/10000.0f)*3.141592f;
				grid[offset].flow = Vector(flowstrength, phi, theta);
				grid[offset].flowadd = Vector(	0,
												((rand()%10000)/10000.0f)*2*3.141592f,
												((rand()%10000)/10000.0f)*3.141592f)*flowrotate;
				grid[offset].currentvector = Vector(0, 0, 0);

				cum += Mathematics::sphereToCartesian(grid[offset].flow.x, grid[offset].flow.y, grid[offset].flow.z);
				zpos += zsize / zdim;
			}
			ypos += ysize / ydim;
		}
		xpos += xsize / xdim;
	}

//	char h[170];
	cum *= (1.0f / xdim*ydim*zdim);
//	sprintf(h, "Average flow vector = (%f, %f, %f)", cum.x, cum.y, cum.z);
//	loki->writeToLog(h);

	generatorpoint = Vector(0, 0, 0);
	cum = Vector(0, 0, 0);

	for (i=0;i<particlecount;i++)
	{
		particles[i].init(generatorpoint);
		cum += particles[i].pos;
	}
	cum *= (1.0f / particlecount);
//	sprintf(h, "Average particle position at start = (%f, %f, %f)", cum.x, cum.y, cum.z);
//	loki->writeToLog(h);

}

Kasvu::~Kasvu()
{
	int i;
	delete [] grid;

	for (i=0;i<particlecount;i++)
	{
		delete [] particles[i].vertices;
	}
	delete [] particles;
}

bool Kasvu::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

