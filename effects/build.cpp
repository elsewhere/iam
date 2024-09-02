#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Build.hpp"

void Build::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.82f;
	const float fadeoutstop = 0.9f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
//	if (pos > fadeoutstart && pos <= fadeoutstop)
//		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	//ei fadeouttia

	renderScene(pos, alpha);
}


void Build::renderScene(float pos, float alpha)
{
	int i;
	const float movestart = 0.0f;
	const float movestop = 0.95f;

	const float zoomstart = 0.8f;
	const float zoomstop = 0.9f;

	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, dmsGetTexture("huuto.jpg")->getID());
	glTranslatef(0, 0, -1.0f);
	glColor3f(alpha, alpha, alpha);
	myEnable(MY_TEXTURE_2D);
//	myDisable(MY_BLEND);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	myDisable(MY_DEPTH_TEST);
	glColor4f(1, 1, 1, alpha);

	float zoompos = 1-(pos-zoomstart)/(zoomstop-zoomstart);
	if (zoompos < 0.0f) zoompos = 0.0f;
	if (zoompos > 1.0f) zoompos = 1.0f;

//	glScalef(zoompos, zoompos, zoompos);
//	glRotatef((1-zoompos)*180, 1, 0, 1);

	const float scale = 1.05f;
	glScalef(scale, scale, scale);
	for (i=0;i<xres*yres;i++)
	{
		PicturePoly *p = &polygons[i];

		float movepos = ((pos-p->time)-movestart)/(movestop-movestart);
		if (movepos < 0.0f) movepos = 0.0f;
		if (movepos > 1.0f) movepos = 1.0f;

		movepos = (float)sin(movepos*3.141592f*0.5f); //tasoitetaan liike

		//rotaatio
		Vector r = p->startrotate*(1-movepos) + p->endrotate*movepos;
		Matrix rotmat;
		rotmat.makeRotation(r.x, r.y, r.z);

		//verteksit
		Vector position = p->startpos*(1-movepos) + p->endpos*movepos;
		Vector v1 = position + (Vector(-p->xsize*0.5f, -p->ysize*0.5f, 0))*rotmat;
		Vector v2 = position + (Vector(p->xsize*0.5f, -p->ysize*0.5f, 0))*rotmat;
		Vector v3 = position + (Vector(p->xsize*0.5f, p->ysize*0.5f, 0))*rotmat;
		Vector v4 = position + (Vector(-p->xsize*0.5f, p->ysize*0.5f, 0))*rotmat;

		Vector valo = Vector(1, 0, 1);
		Vector nor = Vector(0, 0, 1)*rotmat;
		float light = alpha * valo.dotProduct(nor);

		glColor4f(1, 1, 1, light);//light, light, light);

		glBegin(GL_QUADS);
		glTexCoord2f(p->uv1.x, p->uv1.y);
		glVertex3f(v1.x, v1.y, v1.z);
		glTexCoord2f(p->uv2.x, p->uv2.y);
		glVertex3f(v2.x, v2.y, v2.z);
		glTexCoord2f(p->uv3.x, p->uv3.y);
		glVertex3f(v3.x, v3.y, v3.z);
		glTexCoord2f(p->uv4.x, p->uv4.y);
		glVertex3f(v4.x, v4.y, v4.z);
		glEnd();
	}
	myEnable(MY_DEPTH_TEST);
	myDisable(MY_BLEND);
}
Build::Build()
{	
	int x, y;
	xres = 20;
	yres = 20;
	polygons = new PicturePoly[xres*yres];

	PicturePoly *poly = polygons;


	const float aspect = 480.0f / 640.0f;
	const float xsize3d = 1.0f;
	const float ysize3d = 1.0f * aspect;
	const float startposx = -xsize3d*0.5f;
	const float startposy = -ysize3d*0.5f;

	const float ustep = 1.0f / (float)xres;
	const float vstep = 1.0f / (float)yres;
	const float xstep = xsize3d / (float)(xres-1);
	const float ystep = ysize3d / (float)(yres-1);

	float vval = 0.0f;
	float endy = startposy;
	for (y=0;y<yres;y++)
	{
		float uval = 0.0f;
		float endx = startposx;
		for (x=0;x<xres;x++)
		{
			const float mintime = 0.1f;
			const float rotates = 6;
			const float startdist = 10;
			const float rotx = 1.3f;
			const float roty = 0.6f;
			const float rotz = 1.0f;
			poly->uv1 = Vector(uval, 1-vval, 0);
			poly->uv2 = Vector(uval+ustep, 1-vval, 0);
			poly->uv3 = Vector(uval+ustep, 1-(vval+vstep), 0);
			poly->uv4 = Vector(uval, 1-(vval+vstep), 0);
			poly->startpos = Vector((((rand()%1000)/1000.0f)-0.5f), 
									 (((rand()%1000)/1000.0f)-0.5f),
									 -0.6f)*startdist;
			poly->endpos = Vector(endx, endy, 0);
			poly->startrotate = Vector((rand()%1000)/1000.0f, 
										(rand()%1000)/1000.0f,
										(rand()%1000)/1000.0f)*rotates*3.141592f;
			poly->endrotate = Vector(0, 0, 0);

			Matrix scale;
			scale.makeIdentity();
			scale.data[0][0] = rotx;
			scale.data[1][1] = roty;
			scale.data[2][2] = rotz;
//			scale.makeScale(rotx, roty, rotz);
			poly->startrotate *= scale;

			poly->ysize = ystep;
			poly->xsize = xstep;

			poly->time = ((float)(-x*y)/(float)(xres*yres))*mintime;

			poly++;

			endx += xstep;
			uval += ustep;

		}
		endy += ystep;
		vval += vstep;
	}
}

Build::~Build()
{
	delete [] polygons;
}

bool Build::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

