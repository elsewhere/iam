#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "MetaPilar.hpp"
#include "metatables.hpp"

MetaPilar::MetaPilar()
{	
	texture = NULL;
	vertices = NULL;
	cubes = NULL;	
	verticeStatus = NULL;
	cubeStatus = NULL;
	polyList = NULL;
	polyCount = 0;
}

MetaPilar::~MetaPilar()
{	
	release();
}

bool MetaPilar::init(unsigned long s, unsigned long e)
{
	int x, y, z;
	int index;

	startTime = s;
	endTime = e;
	angle = 0.0f;
		
	texture = dmsGetTexture("testi.jpg");	
	if(!texture)
	{
		dmsMsg("MetaPilar::Init, Couldn't allocate buffers");	
		return false;
	}

	nVertices = (MAXGRIDSIZE+1)*(MAXGRIDSIZE+1)*(MAXGRIDSIZE+1);
	nCubes = MAXGRIDSIZE*MAXGRIDSIZE*MAXGRIDSIZE;

	polyList = new MetaGridPoint[nCubes];
	vertices = new MetaGridPoint[nVertices];
	cubes = new MetaGridCube[nCubes];
	verticeStatus = new char[nVertices];
	cubeStatus = new char[nCubes];
	if(!vertices || !cubes || !verticeStatus || !cubeStatus || !polyList)
	{		
		dmsMsg("MetaPilar::Init, Couldn't allocate buffers");
			
		return false;
	}

	index = 0;

	// Alustetaan gridin vertexien arvot	
	for(x = 0; x < MAXGRIDSIZE+1; x++)
	{
		for(y = 0; y < MAXGRIDSIZE+1; y++)
		{
			for(z = 0; z < MAXGRIDSIZE+1; z++)
			{
				vertices[index].x = (x - MAXGRIDSIZE*0.5f);
				vertices[index].y = (y - MAXGRIDSIZE*0.5f);
				vertices[index].z = (z - MAXGRIDSIZE*0.5f);		
				index ++;
			}
		}
	}

	index = 0;

	// Kuutiotaulukko osoittaa gridin vertexeihin
	for(x = 0; x < MAXGRIDSIZE; x++)
	{
		for(y = 0; y < MAXGRIDSIZE; y++)
		{
			for(z = 0; z < MAXGRIDSIZE; z++)
			{
				cubes[index].vertices[0] = &vertices[(x    *(MAXGRIDSIZE+1)+y)    *(MAXGRIDSIZE+1)+z  ];
				cubes[index].vertices[1] = &vertices[(x    *(MAXGRIDSIZE+1)+y)    *(MAXGRIDSIZE+1)+z+1];
				cubes[index].vertices[2] = &vertices[(x    *(MAXGRIDSIZE+1)+(y+1))*(MAXGRIDSIZE+1)+z+1];
				cubes[index].vertices[3] = &vertices[(x    *(MAXGRIDSIZE+1)+(y+1))*(MAXGRIDSIZE+1)+z  ];
				cubes[index].vertices[4] = &vertices[((x+1)*(MAXGRIDSIZE+1)+y)    *(MAXGRIDSIZE+1)+z  ];
				cubes[index].vertices[5] = &vertices[((x+1)*(MAXGRIDSIZE+1)+y)    *(MAXGRIDSIZE+1)+z+1];
				cubes[index].vertices[6] = &vertices[((x+1)*(MAXGRIDSIZE+1)+(y+1))*(MAXGRIDSIZE+1)+z+1];
				cubes[index].vertices[7] = &vertices[((x+1)*(MAXGRIDSIZE+1)+(y+1))*(MAXGRIDSIZE+1)+z  ];
				
				index++;
			}
		}
	}

	srand(10);
	
	for(index = 0; index < MAXBALLS; index ++)
	{		
		float r = float((rand()%255) / 255.0f)*1.25f+1.5f;
		
		pallot[index].x = 0.0f;
		pallot[index].y = 0.0f;
		pallot[index].z = 0.0f;
		pallot[index].radius = r*r;
		pallot[index].speed = ((rand()%255) / 255.0f)*2.25f+0.75f;
	}

	glGenTextures(1, &cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, cubeMap);
				
	// tyyppi RGBA, eli oletetaan että kaikki textuurit ovat 32bit
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getImgData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getImgData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getImgData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getImgData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getImgData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getImgData());
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT); 
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT); 
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);    

	glShadeModel(GL_SMOOTH);

	//set up lighting
	float ambient[]={0.0f, 0.0f, 0.0f, 1.0f};
	float diffuse[]={1.2f, 1.2f, 0.2f, 1.0f};
	float position[]={ 50.0f, 5.0f, -5.0f, 0.0f};
	float specular[]={1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
		
	myEnable(MY_LIGHT1);
	
	return true;
}

void MetaPilar::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.2f;
	const float fadeoutstart = 0.9f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	float fogColor[4]= { 0.0f, 0.0f, 0.0f, 1.0f };

	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.85f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 20.0f);
	glFogf(GL_FOG_END, 40.0f);

	myEnable(MY_FOG);

	polyCount = 0;
	memset(polyList, 0, sizeof(MetaGridPoint)*nCubes);

	for(int i = 0; i < MAXBALLS; i++)
	{
		angle = dmsGetTime() * 0.0001f * (float((MAXBALLS - i))/float(MAXBALLS)) * pallot[i].speed * 3.1415926f + time * 0.0001f;
		float rad = 1.5f*((1.0f - float((MAXBALLS - i)) / float(MAXBALLS))*sin(angle)+0.82f);

		pallot[i].x = float(rad*(cos(angle)+rad*sin(angle)));
		pallot[i].y = -7.0f + (i*3.0f);
		pallot[i].z = float(rad*(sin(angle)-rad*cos(angle)));
	}
	
	// Hakee pinnat
	drawPilar();
			
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(-6.0f, 0.0f, -25.0f);
	glRotatef((float)sin(pos * 2 * 3.1415926) * 2.5f, 0.0f, 1.0f, 0.0f);
		
	myDisable(MY_BLEND);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_TEXTURE_CUBE_MAP_EXT);
	myEnable(MY_TEXTURE_GEN_S); 
	myEnable(MY_TEXTURE_GEN_T); 
	myEnable(MY_TEXTURE_GEN_R); 	
	myEnable(MY_CULL_FACE);
	myEnable(MY_TEXTURE_2D);	
	myEnable(MY_LIGHTING);
	myEnable(MY_LIGHT1);
				
	float ambient[]={0.0f, 0.0f, 0.0f, 1.0f};	
	float diffuse[]={alpha, alpha, alpha, alpha};
	float position[]={ 50.0f, 5.0f, -5.0f, 0.0f};
	float specular[]={1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);	

	glColor4f(1.0f, 1.0f, 1.0f, alpha);

	drawPolyList();	

/*
	myDisable(MY_TEXTURE_CUBE_MAP_EXT);
	myDisable(MY_TEXTURE_GEN_S); 
	myDisable(MY_TEXTURE_GEN_T); 
	myDisable(MY_TEXTURE_GEN_R); 		
	myDisable(MY_TEXTURE_2D);
	myDisable(MY_LIGHTING);
	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glColor4f(1.0f, 0.75f, 0.35f, 0.45f);	
	drawPolyListWireFrame();
*/	

	myDisable(MY_TEXTURE_CUBE_MAP_EXT);
	myDisable(MY_TEXTURE_GEN_S); 
	myDisable(MY_TEXTURE_GEN_T); 
	myDisable(MY_TEXTURE_GEN_R); 		
	myDisable(MY_TEXTURE_2D);
	myEnable(MY_CULL_FACE);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_LIGHTING);
	myEnable(MY_LIGHT1);
	myEnable(MY_BLEND);
	//myBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);	
	//myBlendFunc(GL_DST_COLOR,GL_ONE);	
	myBlendFunc(GL_SRC_COLOR,GL_ONE);	
				
	glColor4f(0.79f, 0.8f, 0.85f, alpha);	
		
	float ambient2[]={0.0f, 0.0f, 0.0f, 1.0f};	
	float diffuse2[]={alpha*0.5f, alpha*0.5f, alpha*0.5f, alpha*0.5f};
	float position2[]={ -50.0f, 5.0f, -5.0f, 0.0f};
	float specular2[]={0.0f, 0.0f, 0.0f, 0.0f};
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);
	glLightfv(GL_LIGHT1, GL_POSITION, position2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular2);

	drawPolyListFlat();

	glPopMatrix();

	//drawGrid();	

	myDisable(MY_FOG);
	myDisable(MY_TEXTURE_CUBE_MAP_EXT);
	myDisable(MY_TEXTURE_GEN_S); 
	myDisable(MY_TEXTURE_GEN_T); 
	myDisable(MY_TEXTURE_GEN_R); 		
	myDisable(MY_TEXTURE_2D);
	myDisable(MY_LIGHTING);	
}

void MetaPilar::release()
{
	if(polyList)
	{	
		delete [] polyList;
		polyList = NULL;
	}

	if(verticeStatus)
	{
		delete [] verticeStatus;
		verticeStatus = NULL;
	}

	if(cubeStatus)
	{
		delete [] cubeStatus;
		cubeStatus = NULL;
	}

	if(vertices)
	{
		delete [] vertices;
		vertices = NULL;
	}

	if(cubes)
	{
		delete [] cubes;
		cubes = NULL;
	}

	glDeleteTextures(1, &cubeMap);
}

void MetaPilar::drawGrid()
{
	Vector p1, p2, p3, p4, p5, p6, p7, p8;
	MetaGridPoint *vert = NULL;

	myDisable(MY_TEXTURE_CUBE_MAP_EXT);
	myDisable(MY_TEXTURE_GEN_S); 
	myDisable(MY_TEXTURE_GEN_T); 
	myDisable(MY_TEXTURE_GEN_R);
	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_LIGHTING);
	myEnable(MY_LINE_SMOOTH);	
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Konvertoidaan gridin piste World Spaceen
	p1.x = (0 - MAXGRIDSIZE*0.5f);
	p1.y = (0 - MAXGRIDSIZE*0.5f);
	p1.z = (0 - MAXGRIDSIZE*0.5f);

	p2.x = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);
	p2.y = (0 - MAXGRIDSIZE*0.5f);
	p2.z = (0 - MAXGRIDSIZE*0.5f);
	
	p3.x = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);
	p3.y = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);
	p3.z = (0 - MAXGRIDSIZE*0.5f);

	p4.x = (0 - MAXGRIDSIZE*0.5f);
	p4.y = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);
	p4.z = (0 - MAXGRIDSIZE*0.5f);

	p5.x = (0 - MAXGRIDSIZE*0.5f);
	p5.y = (0 - MAXGRIDSIZE*0.5f);
	p5.z = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);

	p6.x = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);
	p6.y = (0 - MAXGRIDSIZE*0.5f);
	p6.z = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);
	
	p7.x = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);
	p7.y = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);
	p7.z = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);

	p8.x = (0 - MAXGRIDSIZE*0.5f);
	p8.y = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);
	p8.z = (MAXGRIDSIZE - MAXGRIDSIZE*0.5f);

	glBegin(GL_LINES);

		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p4.x, p4.y, p4.z);
		glVertex3f(p4.x, p4.y, p4.z);
		glVertex3f(p1.x, p1.y, p1.z);
		
		glVertex3f(p5.x, p5.y, p5.z);
		glVertex3f(p6.x, p6.y, p6.z);
		glVertex3f(p6.x, p6.y, p6.z);
		glVertex3f(p7.x, p7.y, p7.z);
		glVertex3f(p7.x, p7.y, p7.z);
		glVertex3f(p8.x, p8.y, p8.z);
		glVertex3f(p8.x, p8.y, p8.z);
		glVertex3f(p5.x, p5.y, p5.z);

		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p5.x, p5.y, p5.z);
		glVertex3f(p4.x, p4.y, p4.z);
		glVertex3f(p8.x, p8.y, p8.z);

		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p6.x, p6.y, p6.z);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p7.x, p7.y, p7.z);
		
	glEnd();

	myDisable(MY_BLEND);
	myEnable(MY_TEXTURE_2D);
	myEnable(MY_DEPTH_MASK);
}

void MetaPilar::drawPilar()
{
	unsigned int i;
	MetaGridPoint *vert = NULL;

	// Alusta gridit	
	memset(verticeStatus, 0, sizeof(char)*nVertices);
	memset(cubeStatus, 0, sizeof(char)*nCubes);	

	for(i = 0; i < MAXBALLS; i++)
	{			
		// Pallon koordinaatit Grid Spaceen
		int x = (int)(pallot[i].x + MAXGRIDSIZE*0.5f);
		int y = (int)(pallot[i].y + MAXGRIDSIZE*0.5f);
		int z = (int)(pallot[i].z + MAXGRIDSIZE*0.5f);

		// Aloitetaan rekursiivinen tsekkaus gridin kuutiosta joka on lähinnä pallon sen hetkistä
		// sijaintia		
		
		recurse(x, y, z);
	}			
}

void MetaPilar::recurse(int x, int y, int z)
{
	if(x <= 1 || x >= MAXGRIDSIZE-1) return;
	if(y <= 1 || y >= MAXGRIDSIZE-1) return;
	if(z <= 1 || z >= MAXGRIDSIZE-1) return;

	if(processCube(x, y, z) == 255)	return;
					
	recurse(x+1, y  , z);
	recurse(x-1, y  , z);
	recurse(x  , y+1, z);
	recurse(x  , y-1, z);
	recurse(x  , y  , z+1);
	recurse(x  , y  , z-1);
}

// HUOM! Parametrit ovat objektin paikka avaruudessa
// funktio etsii oikean kuution joka ympäröi pisteen ja
// laskee kuution pisteille energiat
int MetaPilar::processCube(int x, int y, int z)
{		
	unsigned int intersectedEdges = 0, currentEdge, offset = 0;
	float energy[8];
	unsigned char cubeIndex = 0;
	char *stat = NULL;
	MetaGridPoint resultingVertices[12];	// 12 reunan vertexit
	MetaGridPoint *vert = NULL;
	MetaGridCube *cube = NULL;	

	//dmsMsg("%d %d %d\n", x, y, z);
	
	if(x <= 1 || x >= MAXGRIDSIZE-1) return 255;
	if(y <= 1 || y >= MAXGRIDSIZE-1) return 255;
	if(z <= 1 || z >= MAXGRIDSIZE-1) return 255;
	
	offset = x + y*MAXGRIDSIZE + z*MAXGRIDSIZE*MAXGRIDSIZE;
	cube = &cubes[offset];
	stat = &cubeStatus[offset];
		
	if(*stat == 1)
	{
		//dmsMsg("Cube is allready calculated\n");
		return 255;
	}
	
	//drawCube(x, y, z);

	*stat = 1;
	energy[0] = getGridPointEnergy(cube->vertices[0]);
	energy[1] = getGridPointEnergy(cube->vertices[1]);
	energy[2] = getGridPointEnergy(cube->vertices[2]);
	energy[3] = getGridPointEnergy(cube->vertices[3]);
	energy[4] = getGridPointEnergy(cube->vertices[4]);
	energy[5] = getGridPointEnergy(cube->vertices[5]);
	energy[6] = getGridPointEnergy(cube->vertices[6]);
	energy[7] = getGridPointEnergy(cube->vertices[7]);		

	// Haetaan offsetti tableen
	(energy[0] < 1.0f) ? cubeIndex |= 1 : 0;
	(energy[1] < 1.0f) ? cubeIndex |= 2 : 0;
	(energy[2] < 1.0f) ? cubeIndex |= 4 : 0;
	(energy[3] < 1.0f) ? cubeIndex |= 8 : 0;
	(energy[4] < 1.0f) ? cubeIndex |= 16 : 0;
	(energy[5] < 1.0f) ? cubeIndex |= 32 : 0;
	(energy[6] < 1.0f) ? cubeIndex |= 64 : 0;
	(energy[7] < 1.0f) ? cubeIndex |= 128 : 0;
	
	intersectedEdges = edgeTable[cubeIndex];
	//dmsMsg("%d\n", cubeIndex);
	
	// Jos kaikki pisteet ulkona tai sisällä ..
	if(intersectedEdges == 0 || intersectedEdges == 255) return cubeIndex;
	
	for(currentEdge = 0; currentEdge < 12; currentEdge++)
	{
		// Tarkistetaan bitit
		// 0 = 1
		// 1 = 2
		// 3 = 4
		// 4 = 8
		// 5 = 16
		// 6 = 32
		// 7 = 64
		// 8 = 128
		// 9 = 256
		// 10 = 512
		// 11 = 1024
		if(intersectedEdges & 1<<currentEdge)
		{
			float m = 0.0f;
			MetaGridPoint *v1 = cube->vertices[edgeList[currentEdge*2  ]];
			MetaGridPoint *v2 = cube->vertices[edgeList[currentEdge*2+1]];

			if(v1->value == 1.0f || v1->value == v2->value)
			{
				resultingVertices[currentEdge].x = v1->x;
				resultingVertices[currentEdge].y = v1->y;
				resultingVertices[currentEdge].z = v1->z;
				resultingVertices[currentEdge].alpha = v1->alpha;
				
				resultingVertices[currentEdge].nx = v1->nx;
				resultingVertices[currentEdge].ny = v1->ny;
				resultingVertices[currentEdge].nz = v1->nz;

				break;
			}

			if(v2->value == 1.0f)
			{
				resultingVertices[currentEdge].x = v2->x;
				resultingVertices[currentEdge].y = v2->y;
				resultingVertices[currentEdge].z = v2->z;
				resultingVertices[currentEdge].alpha = v2->alpha;
			
				resultingVertices[currentEdge].nx = v2->nx;
				resultingVertices[currentEdge].ny = v2->ny;
				resultingVertices[currentEdge].nz = v2->nz;
				break;
			}

			m = float(1.0f - v1->value) / float(v2->value - v1->value);			
			resultingVertices[currentEdge].x = v1->x + m * (v2->x - v1->x);
			resultingVertices[currentEdge].y = v1->y + m * (v2->y - v1->y);
			resultingVertices[currentEdge].z = v1->z + m * (v2->z - v1->z);
			resultingVertices[currentEdge].alpha = v1->alpha + m * (v2->alpha - v1->alpha);
			
			resultingVertices[currentEdge].nx = v1->nx + m * (v2->nx - v1->nx);
			resultingVertices[currentEdge].ny = v1->ny + m * (v2->ny - v1->ny);
			resultingVertices[currentEdge].nz = v1->nz + m * (v2->nz - v1->nz);
		}
	}	
	
	MetaGridPoint *ptr = NULL;

	for(int j = 0; triTable[cubeIndex][j] != -1; j+= 3)
	{		
		MetaGridPoint *p1 = &resultingVertices[triTable[cubeIndex][j+0]];
		MetaGridPoint *p2 = &resultingVertices[triTable[cubeIndex][j+2]];
		MetaGridPoint *p3 = &resultingVertices[triTable[cubeIndex][j+1]];
		
		ptr = &polyList[polyCount];
		ptr->x = p1->x;
		ptr->y = p1->y;
		ptr->z = p1->z;
		ptr->nx = p1->nx;
		ptr->ny = p1->ny;
		ptr->nz = p1->nz;
		ptr->alpha = p1->alpha;
		ptr++;
		polyCount++;

		ptr = &polyList[polyCount];
		ptr->x = p2->x;
		ptr->y = p2->y;
		ptr->z = p2->z;
		ptr->nx = p2->nx;
		ptr->ny = p2->ny;
		ptr->nz = p2->nz;
		ptr->alpha = p2->alpha;
		ptr++;
		polyCount++;

		ptr = &polyList[polyCount];
		ptr->x = p3->x;
		ptr->y = p3->y;
		ptr->z = p3->z;
		ptr->nx = p3->nx;
		ptr->ny = p3->ny;
		ptr->nz = p3->nz;
		ptr->alpha = p3->alpha;
		ptr++;
		polyCount++;

		//dmsMsg("%d %f %f %f\n", j, p1->alpha, p2->alpha, p3->alpha);

	}

	return cubeIndex;
}

void MetaPilar::drawPolyList()
{			
	glBegin(GL_TRIANGLES);

	for(unsigned int i = 0; i < polyCount; i++)
	{	
		MetaGridPoint *p = &polyList[i];
						
		glNormal3fv((float *)&p->nx);
		glVertex3fv((float *)&p->x);
	}		

	glEnd();
}

void MetaPilar::drawPolyListFlat()
{				
	glBegin(GL_TRIANGLES);

	for(unsigned int i = 0; i < polyCount; i+=3)
	{	
		MetaGridPoint *a = &polyList[i+0];
		MetaGridPoint *b = &polyList[i+1];
		MetaGridPoint *c = &polyList[i+2];
		float dx, dy, dz, dx2, dy2, dz2;
		float len;
		Vector tmp, normal;	

		dx = b->x - a->x; dy = b->y - a->y; dz = b->z - a->z;
		dx2= c->x - a->x; dy2= c->y - a->y; dz2= c->z - a->z;

		tmp = Vector((dy * dz2 - dz * dy2), (dz * dx2 - dx * dz2), (dx * dy2 - dy * dx2));
		len = tmp.length();
		if(len <= 0.0f) len = 0.001f;
	
		len = 1.0f / len;
		normal = tmp * len;		
		
		//dmsMsg("%f %f %f\n", p->x, p->y, p->z);
		
		//glNormal3f(p->nx, p->ny, p->nz);
		//glVertex3f(p->x, p->y, p->z);
		
		glNormal3fv((float *)&normal.x);
		glVertex3fv((float *)&a->x);
		glVertex3fv((float *)&b->x);
		glVertex3fv((float *)&c->x);
	}		

	glEnd();
}

void MetaPilar::drawPolyListWireFrame()
{			
	glBegin(GL_LINES);

	for(unsigned int i = 0; i < polyCount; i+=1)
	{	
		MetaGridPoint *a = &polyList[i+0];
		//MetaGridPoint *b = &polyList[i+1];
		//MetaGridPoint *c = &polyList[i+2];
				
		glVertex3fv((float *)&a->x);
		//glVertex3fv((float *)&b->x);
		//glVertex3fv((float *)&c->x);
	}		

	glEnd();
}

float MetaPilar::getGridPointEnergy(MetaGridPoint *point)
{	
	int x = (int)(point->x + MAXGRIDSIZE*0.5f);
	int y = (int)(point->y + MAXGRIDSIZE*0.5f);
	int z = (int)(point->z + MAXGRIDSIZE*0.5f);
	unsigned int offset = x + y*(MAXGRIDSIZE+1) + z*(MAXGRIDSIZE+1)*(MAXGRIDSIZE+1);
	char *stat = &verticeStatus[offset];

	if(*stat == 1)
	{
		return point->value;
	}
				
	point->value = getFloorEnergyForGridPoint(point);
	point->value += getBallsEnergyForGridPoint(point);	
	getBallsNormalForGridPoint(point);
	*stat = 1;

	return point->value;
}

float MetaPilar::getBallsEnergyForGridPoint(MetaGridPoint *point)
{
	unsigned int i;
	MetaBall *ball = NULL;
	float energy = 0.0f, cx, cy, cz;

	for(i = 0; i < MAXBALLS; i++)
	{	
		float tmp = 0.0f;
		ball = &pallot[i];

		cx = point->x-ball->x;
		cy = point->y-ball->y;
		cz = point->z-ball->z;
		tmp = (cx*cx+cy*cy+cz*cz);

		if(tmp <= 0.0f) tmp = 0.1f;
						
		energy += ball->radius / tmp;		
	}		

	return energy;	
}

void MetaPilar::getBallsNormalForGridPoint(MetaGridPoint *point)
{
	unsigned int i;
	MetaBall *ball = NULL;
	float cx, cy, cz;	

	for(i = 0; i < MAXBALLS; i++)
	{	
		float tmp = 0.0f;
		ball = &pallot[i];

		cx = point->x-ball->x;
		cy = point->y-ball->y;
		cz = point->z-ball->z;
		tmp = (cx*cx+cy*cy+cz*cz);

		if(tmp < 0.0f) tmp = 0.001f;
						
		point->nx += 2 * cx / (tmp*tmp);
		point->ny += 2 * cy / (tmp*tmp);
		point->nz += 2 * cz / (tmp*tmp);
	}

	Vector n(point->nx, point->ny, point->nz);
	n.normalize();

	point->nx = n.x;
	point->ny = n.y;
	point->nz = n.z;
}

float MetaPilar::getFloorEnergyForGridPoint(MetaGridPoint *point)
{
	MetaBall *ball = NULL;
	float energy = 0.0f;
	
	// Koko lattia täytyy at the moment
	if((point->y == -7.0f))
	{
		point->alpha = 0.0f;
		energy += 1.8f;		
	}

	if(point->x == 14.0f)
	{
		energy += 1.8f;
	}

	// Koko lattia täytyy at the moment
	if((point->y == 7.0f))
	{
		point->alpha = 0.0f;
		energy += 1.8f;
	}
	
	return energy;	
}

void MetaPilar::drawCube(int x, int y, int z)
{
	Vector p1, p2, p3, p4, p5, p6, p7, p8;
	MetaGridPoint *vert = NULL;
	MetaGridCube *cube = &cubes[x + y*MAXGRIDSIZE + z*MAXGRIDSIZE*MAXGRIDSIZE];

	myDisable(MY_TEXTURE_CUBE_MAP_EXT);
	myDisable(MY_TEXTURE_GEN_S); 
	myDisable(MY_TEXTURE_GEN_T); 
	myDisable(MY_TEXTURE_GEN_R);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_LIGHTING);
	myDisable(MY_TEXTURE_2D);
	myEnable(MY_LINE_SMOOTH);	
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glColor4f(0.65f, 0.45f, 1.0f, 0.45f);

	// Konvertoidaan gridin piste World Spaceen
	p1.x = cube->vertices[0]->x;
	p1.y = cube->vertices[0]->y;
	p1.z = cube->vertices[0]->z;

	p2.x = cube->vertices[1]->x;
	p2.y = cube->vertices[1]->y;
	p2.z = cube->vertices[1]->z;

	p3.x = cube->vertices[2]->x;
	p3.y = cube->vertices[2]->y;
	p3.z = cube->vertices[2]->z;

	p4.x = cube->vertices[3]->x;
	p4.y = cube->vertices[3]->y;
	p4.z = cube->vertices[3]->z;

	p5.x = cube->vertices[4]->x;
	p5.y = cube->vertices[4]->y;
	p5.z = cube->vertices[4]->z;

	p6.x = cube->vertices[5]->x;
	p6.y = cube->vertices[5]->y;
	p6.z = cube->vertices[5]->z;

	p7.x = cube->vertices[6]->x;
	p7.y = cube->vertices[6]->y;
	p7.z = cube->vertices[6]->z;

	p8.x = cube->vertices[7]->x;
	p8.y = cube->vertices[7]->y;
	p8.z = cube->vertices[7]->z;
	
	glBegin(GL_LINES);

		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p4.x, p4.y, p4.z);
		glVertex3f(p4.x, p4.y, p4.z);
		glVertex3f(p1.x, p1.y, p1.z);
		
		glVertex3f(p5.x, p5.y, p5.z);
		glVertex3f(p6.x, p6.y, p6.z);
		glVertex3f(p6.x, p6.y, p6.z);
		glVertex3f(p7.x, p7.y, p7.z);
		glVertex3f(p7.x, p7.y, p7.z);
		glVertex3f(p8.x, p8.y, p8.z);
		glVertex3f(p8.x, p8.y, p8.z);
		glVertex3f(p5.x, p5.y, p5.z);

		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p5.x, p5.y, p5.z);
		glVertex3f(p4.x, p4.y, p4.z);
		glVertex3f(p8.x, p8.y, p8.z);

		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p6.x, p6.y, p6.z);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p7.x, p7.y, p7.z);
		
	glEnd();

	myDisable(MY_BLEND);
	myEnable(MY_TEXTURE_2D);
	myEnable(MY_TEXTURE_CUBE_MAP_EXT);
	myEnable(MY_TEXTURE_GEN_S); 
	myEnable(MY_TEXTURE_GEN_T); 
	myEnable(MY_TEXTURE_GEN_R); 
}
