#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include "Water.hpp"
#include "../mathematics.hpp"

//extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
//extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;

void Water::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.9f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);
	
	//frameskip
	if (pos - frametime > 0.001f)
	{
		calculateMesh(pos);
		frametime = pos;
	}

	render(pos, alpha);
/*
	write(264, 10, pos, 0.1f, 0.3f, alpha, "luontofontti.pcx", "i am a mother");
	write(298, 42, pos, 0.15f, 0.35f, alpha, "luontofontti.pcx", "my husband");
	write(330, 74, pos, 0.2f, 0.4f, alpha, "luontofontti.pcx", "is time");

	write(464, 480, pos, 0.55f, 0.75f, alpha, "teknofontti.pcx", "i am all");
	write(496, 512, pos, 0.6f, 0.8f, alpha, "teknofontti.pcx", "i need no one");
*/
	write(20, 70, pos, 0.25f, 0.40f, alpha, "teknofontti.pcx", "I am oil"); 
	write(20, 100, pos, 0.30f, 0.45f, alpha, "teknofontti.pcx", "I fuel the future"); 

	write(400, 440, pos, 0.45f, 0.60f, alpha, "luontofontti.pcx", "You are of my");
	write(420, 470, pos, 0.45f, 0.60f, alpha, "luontofontti.pcx", "essence");
	write(120, 700, pos, 0.60f, 0.80f, alpha, "teknofontti.pcx", "Yes");
}

Water::Water()
{
	int x, y;
	frametime = 0.0f;
	gridy = 75;
	gridx = 75;
	Grid = new WaterCell[(gridy+1)*(gridx+1)];
	Faces = new WaterFace[(gridx)*(gridy)*2];
	sphere = new Vector[(gridx+1)*(gridy+1)];

	waterflag = false;
	water1 = new float[(gridy+1)*(gridx+1)];
	water2 = new float[(gridy+1)*(gridx+1)];
	lastut = new Lastu[VITUSTILASTUJA];
	lastut2 = new Lastu[VITUSTILASTUJA];

	memset(water1, 0, (gridy+1)*(gridx+1)*sizeof(float));
	memset(water2, 0, (gridy+1)*(gridx+1)*sizeof(float));

	WaterFace *vesinaama = Faces;
	for (y=0;y<gridy;y++)
	{
		for (x=0;x<gridx;x++)
		{
			//vasen yläkulma
			vesinaama->v1 = &Grid[x+y*(gridx+1)]; 
			//oikea alakulma
			vesinaama->v2 = &Grid[x+1+(y+1)*(gridx+1)];
			//vasen alakulma
			vesinaama->v3 = &Grid[x+(y+1)*(gridx+1)];
			vesinaama++;
			//vasen yläkulma
			vesinaama->v1 = &Grid[x+y*(gridx+1)]; 
			//oikea yläkulma 
			vesinaama->v2 = &Grid[x+1+y*(gridx+1)]; 
			//oikea alakulma
			vesinaama->v3 = &Grid[x+1+(y+1)*(gridx+1)];
			vesinaama++;
		}
	}

	const float radius = 1.0f;
	Vector *vekt = sphere;
	for (y=0;y<gridy+1;y++)
	{
		const float phi = ((float)y/(float)(gridy))*3.141592f;
		for (x=0;x<gridx+1;x++)
		{
			const float theta = ((float)x/(float)(gridx))*2*3.141592f;
			*vekt = Mathematics::sphereToCartesian(radius, phi, theta);
			vekt++;
		}
	}

	for(int i = 0; i < VITUSTILASTUJA; i++)
	{
		lastut[i].pos.x = -4.0f;
		lastut[i].pos.y = rand()%4-rand()%4 + 5.0f;
		lastut[i].pos.z = rand()%80-rand()%80 - 80.0f;
		lastut[i].life = 1.0f - (-200.0f / lastut[i].pos.z);
		lastut[i].speed = rand()%255 / 256.0f + 0.125f;

		lastut2[i].pos.x = -4.0f;
		lastut2[i].pos.y = rand()%4-rand()%4 - 5.0f;
		lastut2[i].pos.z = rand()%80-rand()%80 - 80.0f;
		lastut2[i].life = 1.0f - (-200.0f / lastut[i].pos.z);
		lastut2[i].speed = rand()%255 / 256.0f + 0.125f;
	}
}

Water::~Water()
{
	delete [] Grid;
	delete [] Faces;
	delete [] water1;
	delete [] water2;
	delete [] sphere;
}

void Water::filterWater(float *source, float *dest)
{
	int i;
	int xsize = gridx+1;
	int ysize = gridy;
	int size = gridx*gridy;
	for (i=xsize+1;i<size-xsize-2;i++)
	{
		float val = source[i+1]+source[i-1]+source[i+xsize]+source[i-xsize] + 
			        source[i-xsize-1]+source[i-xsize+1]+source[i+xsize-1]+source[i+xsize+1];
		val /= 4.0f;
		val -= dest[i];
		dest[i] = val*0.89f;//0.89f;//92f;
	}
	
	for (int j=0;j<1;j++)
	{
		for (i=xsize+1;i<size-xsize-2;i++)
		{
			float val = dest[i+1]+dest[i-1]+dest[i+xsize]+dest[i-xsize] + 
				        dest[i-xsize-1]+dest[i-xsize+1]+dest[i+xsize-1]+dest[i+xsize+1];
			val /= 8.0f;

			const float blurval = 0.26f;
			dest[i] = dest[i]*blurval + val*(1-blurval); //0.85 0.15
		}
	}

/*
	static float t = 0.0f;
	for (int y=0;y<gridy;y++)
	{
		for (int x=0;x<gridx+1;x++)
		{
			float dist = (float)sqrt((x*0.5f-gridx)*(x*0.5f-gridx)+(y*0.5f-gridy)*(y*0.5f-gridy));
			dest[x+y*(gridx+1)]=(float)sin(dist+t*6)*0.1f;
		}
	}
	
	t += 0.01f;
*/
}

void Water::surfer(float *buffer, float xpos, float ypos, float size)
{
	int intx = (int)xpos;
	int inty = (int)ypos;

	float fracx = xpos - intx;
	float fracy = ypos - inty;

	int offset = intx + inty*gridx;
	buffer[offset] = (1-fracx)*(1-fracy)*size;
	buffer[offset+1] = fracx * (1-fracy)*size;
	buffer[offset+gridx+1] = (1-fracx)*fracy*size;
	buffer[offset+gridx+1+1] = fracx * fracy*size;
}


void Water::drop(float *buffer, int xpos, int ypos, float size)
{
}
void Water::calculateMesh(float pos)
{
	int x, y;
	WaterCell *cell;

	const float speed = 40;
	const float speed2 = 30;
	const float speed3 = 26;
	const float dropx = 20+(gridx*0.5f) + (gridx*0.26f)*(float)sin(pos*1.4f*speed);
	const float dropy = (gridy*0.5f) + (gridy*0.3f)*(float)cos(pos*0.61f*speed);
	const float dropheight = 2.0f;

	const float surfx = 20+(gridx*0.5f) + (gridx*0.26f)*(float)(sin(pos*1.4f*speed2)*sin(pos*1.5f*speed2));
	const float surfy = (gridy*0.5f) + (gridy*0.3f)*(float)(cos(pos*0.61f*speed2)*sin(pos*1.69f*speed2));
	const float surfx2 = 20+(gridx*0.5f) + (gridx*0.26f)*(float)(sin(pos*1.8f*speed3)*cos(pos*2.1*speed3));
	const float surfy2 = (gridy*0.5f) + (gridy*0.3f)*(float)(cos(pos*0.91f*speed3)*sin(pos*5.2f*speed3));

	float *drawbuffer, *source;

	if (waterflag==true)
	{
		source = water1;
		drawbuffer = water2;
		waterflag = false;
	}
	else
	{
		source = water2;
		drawbuffer = water1;
		waterflag = true;
	}
	surfer(source, dropx, dropy, dropheight);
	surfer(source, surfx, surfy, dropheight*0.8f);
	surfer(source, surfx2, surfy2, dropheight*0.7f);

	const float tx = ((rand()%(gridx-2)*100)/100.0f)+1;
	const float ty = ((rand()%(gridy-2)*100)/100.0f)+1;
	surfer(source, tx, ty, -dropheight*0.5f);
	surfer(source, tx+1, ty, -dropheight*0.5f);
	surfer(source, tx, ty+1, -dropheight*0.5f);
	surfer(source, tx+1, ty+1, -dropheight*0.5f);

	filterWater(source, drawbuffer);

	cell = Grid;
	const float xsize = 4.0f;
	const float ysize = 3.5f;
	float ypos = -ysize * 0.5f;
	for (y=0;y<gridy+1;y++)
	{
		float xpos = -xsize * 0.5f;
		for (x=0;x<gridx+1;x++)
		{
			const float waterscale = 0.6f;
			const float baseradius = 0.3f;
			cell->pos = sphere[x+y*(gridx+1)]*(baseradius+(*drawbuffer)*waterscale);
			cell++;
			drawbuffer++;
			xpos += xsize / (float)(gridx+1);
		}
		ypos += ysize / (float)(gridy+1);
	}
	cell = Grid;
	for (y=0;y<gridy+1;y++)
	{
		for (x=0;x<gridx+1;x++)
		{
			cell->normal = Vector(0,0,0);
			cell++;
		}
	}
	//ensin facenormaalit

	WaterFace *face = Faces;
	for (y=0;y<gridy;y++)
	{
		for (x=0;x<gridx;x++)
		{
			//facenormaali
			Vector nor = (face->v1->pos-face->v2->pos).crossProduct(face->v1->pos-face->v3->pos);
			//lisätään facenormaali kaikkiin vertekseihin, joihin se liittyy
			face->v1->normal += nor;
			face->v2->normal += nor;
			face->v3->normal += nor;
			face++;
			nor = (face->v1->pos-face->v2->pos).crossProduct(face->v1->pos-face->v3->pos);
			face->v1->normal += nor;
			face->v2->normal += nor;
			face->v3->normal += nor;
			face++;
		}
	}
 	cell = Grid;
	for (y=0;y<(gridy+1)*(gridx+1);y++)
	{
		(cell++)->normal.normalize();
	}
	
	cell = Grid;

	for (y=0;y<gridy;y++)
	{
		const float yphase = (float)y/(float)gridy;
		for (x=0;x<gridx+1;x++)
		{
			const float xphase = (float)x/(float)gridx;
			cell->u = xphase;//cell->normal.x;//xphase;
			cell->v = yphase;//cell->normal.y;//yphase;
			cell++;
		}
	}
}
void Water::render(float pos, float alpha)
{
	int x, y;

	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_TEXTURE_2D);
	Texture *tausta = dmsGetTexture("blast.jpg"); 
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

	drawLastut(pos);
	
	glLoadIdentity();
	Texture *texture1 = dmsGetTexture("kromi.pcx");
	Texture *texture2 = dmsGetTexture("databall.jpg");//flare.pcx.pcx"); 
	Vector cam(0.15f*(float)sin(pos*13)+0.08f*(float)cos(pos*11), 
		        0.093f*(float)cos(pos*5)+0.076f*(float)cos(pos*3), 
				-1.97f+0.4f*(float)cos(pos*11));
	glTranslatef(cam.x - 0.1f, cam.y, cam.z);
	WaterCell *cell = Grid;

	myDisable(MY_BLEND);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_TEXTURE_2D);

	Vector valo(1, 0, 0);
	valo.normalize();
	Matrix rotate, valorotate;
	rotate.makeRotation(pos*13, pos*16, -pos*15.5f);
	valorotate.makeRotation(0, pos*2, 0);
	valo *= valorotate;

	for (y=0;y<gridy;y++)
	{
		cell = Grid + y*(gridx+1);
		myDisable(MY_BLEND);
		glBindTexture(GL_TEXTURE_2D, texture1->getID());
		glBegin(GL_QUAD_STRIP);
		for (x=0;x<gridx+1;x++)
		{
			const float maxcolor = 3.0f;
			const float ambientcolor = 0.8f;
			Vector p1 = cell->pos*rotate;
			Vector p2 = (cell+gridx+1)->pos*rotate;
			Vector n1 = cell->normal*rotate;
			Vector n2 = (cell+gridx+1)->normal*rotate;
			float color1 = alpha*(ambientcolor+maxcolor*n1.dotProduct(valo));
			float color2 = alpha*(ambientcolor+maxcolor*n2.dotProduct(valo));
			glColor3f(color1, color1, color1);
			glTexCoord2f(cell->u*2, cell->v*2);
			glVertex3f(p1.x, p1.y, p1.z);
			glColor3f(color2, color2, color2);
			glTexCoord2f((cell+gridx+1)->u*2, (cell+gridx+1)->v*2);
			glVertex3f(p2.x, p2.y, p2.z);
			cell++;
		}
		glEnd();
		cell = Grid + y*(gridx+1);

		myEnable(MY_BLEND);
		myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, texture2->getID());

		glBegin(GL_QUAD_STRIP);
		for (x=0;x<gridx+1;x++)
		{
			const float maxcolor = 1.0f;
			const float ambientcolor = 0.0f;
			Vector p1 = cell->pos*rotate;
			Vector p2 = (cell+gridx+1)->pos*rotate;
			Vector n1 = cell->normal*rotate;
			Vector n2 = (cell+gridx+1)->normal*rotate;
			float color1 = alpha*(ambientcolor+maxcolor*n1.dotProduct(valo));
			float color2 = alpha*(ambientcolor+maxcolor*n2.dotProduct(valo));

			glColor4f(1, 1, 1, color2);
			glTexCoord2f(cell->normal.x, cell->normal.y);
			glVertex3f(p1.x, p1.y, p1.z);

			glColor4f(1, 1, 1, color2);
			glTexCoord2f((cell+gridx+1)->normal.x, (cell+gridx+1)->normal.y);
			glVertex3f(p2.x, p2.y, p2.z);

			cell++;
		}
		glEnd();

	}
}
bool Water::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;

	return true;
}

void Water::drawLastut(float pos)
{
	int i;
	Lastu *lastu = lastut;

	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);	

	for(i = 0; i < VITUSTILASTUJA; i++)
	{	
		lastu->pos.z += 0.01f * dmsGetDeltaTime() * lastu->speed;
		lastu->life = 1.0f - (-200.0f / lastu->pos.z);

		glPushMatrix();
		glLoadIdentity();

		glColor4f(1.0f, 1.0f, 1.0f, lastu->life);
		glTranslatef(lastu->pos.x - pos * 10.0f, lastu->pos.y, lastu->pos.z);
		glRotatef(75, 0.0f, 1.0f, 0.0f);

		glBegin(GL_QUADS);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f( 1.0f, -1.0f, 0.0f);
			glVertex3f( 1.0f,  1.0f, 0.0f);
			glVertex3f(-1.0f,  1.0f, 0.0f);
		glEnd();

		glPopMatrix();

		if(lastu->pos.z > 0.0f) 
		{
			lastu->pos.x = -4.0f;
			lastu->pos.y = rand()%4-rand()%4 + 5.0f;						
			lastu->speed = rand()%255 / 256.0f + 0.125f;
			lastu->pos.z = -200.0f;
		}

		lastu++;		
	}

	lastu = lastut2;
	for(i = 0; i < VITUSTILASTUJA; i++)
	{	
		lastu->pos.z += 0.01f * dmsGetDeltaTime() * lastu->speed;
		lastu->life = 1.0f - (-200.0f / lastu->pos.z);

		glPushMatrix();
		glLoadIdentity();

		glColor4f(1.0f, 1.0f, 1.0f, lastu->life);
		glTranslatef(lastu->pos.x - pos * 10.0f, lastu->pos.y, lastu->pos.z);
		glRotatef(75, 0.0f, 1.0f, 0.0f);

		glBegin(GL_QUADS);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f( 1.0f, -1.0f, 0.0f);
			glVertex3f( 1.0f,  1.0f, 0.0f);
			glVertex3f(-1.0f,  1.0f, 0.0f);
		glEnd();

		glPopMatrix();

		if(lastu->pos.z > 0.0f) 
		{
			lastu->pos.x = -4.0f;
			lastu->pos.y = rand()%4-rand()%4 - 5.0f;						
			lastu->speed = rand()%255 / 256.0f + 0.125f;
			lastu->pos.z = -200.0f;
		}

		lastu++;		
	}
}


