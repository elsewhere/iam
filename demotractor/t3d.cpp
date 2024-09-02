//--------------------------------------------------------------------------------------------
//  
//	Format description:
//
//	4 bytes : Vertex count
//  4 bytes : Face count
//  
//  Vertex count * 32 bytes : Vertex data (float pos[3], float norm[3], float u, float v)
//  Face count * 24 bytes : Face data (float norm[3], int a, int b, int c)
//
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "mmgr.h"
#include "t3d.hpp"

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  Code
//--------------------------------------------------------------------------------------------

T3D::T3D()
{
	vertex = NULL;
	face = NULL;

}

T3D::~T3D()
{
	if(face)
	{
		delete [] face;
		face = NULL;
	}

	if(vertex)
	{
		delete [] vertex;
		vertex = NULL;
	}
}

bool T3D::load(char *name)
{
	unsigned int i;

	FILE *f = fopen(name, "rb");
	if(!f)
	{
		return false;
	}

	// Read vertex and face count
	fread(&this->vertices, 1, sizeof(this->vertices), f);
	fread(&this->faces, 1, sizeof(this->faces), f);

	if(!initVertices(this->vertices)) return false;
	if(!initFaces(this->faces)) return false;

	// Read in all vertex data
	for(i = 0; i < this->vertices; i++)
	{
		float pos[3], norm[3], u, v;

		fread(&pos, 3, sizeof(float), f);
		fread(&norm, 3, sizeof(float), f);
		fread(&u, 1, sizeof(float), f);
		fread(&v, 1, sizeof(float), f);

		vertex[i].position.x = pos[0];
		vertex[i].position.y = pos[1];
		vertex[i].position.z = pos[2];
		vertex[i].normal.x = norm[0];
		vertex[i].normal.y = norm[1];
		vertex[i].normal.z = norm[2];
		vertex[i].u = u;
		vertex[i].v = v;
	}

	// Read in all face data
	for(i = 0; i < this->faces; i++)
	{
		float norm[3];
		int a, b, c;
		
		fread(&norm, 3, sizeof(float), f);
		fread(&a, 1, sizeof(int), f);
		fread(&b, 1, sizeof(int), f);
		fread(&c, 1, sizeof(int), f);

		face[i].normal.x = norm[0];
		face[i].normal.y = norm[1];
		face[i].normal.z = norm[2];
		face[i].a = a;
		face[i].b = b;
		face[i].c = c;
	}

	fclose(f);

	return true;
}

bool T3D::loadFromMemory(unsigned char *fileData, unsigned int fileSize)
{
	unsigned int i, offset = 0;
	
	// Read vertex and face count	
	memcpy(&this->vertices, fileData + offset, sizeof(this->vertices));
	offset += sizeof(this->vertices);

	memcpy(&this->faces, fileData + offset, sizeof(this->faces));
	offset += sizeof(this->faces);

	if(!initVertices(this->vertices)) return false;
	if(!initFaces(this->faces)) return false;

	// Read in all vertex data
	for(i = 0; i < this->vertices; i++)
	{
		float pos[3], norm[3], u, v;
		
		memcpy(&pos, fileData + offset, sizeof(float)*3);
		offset += sizeof(float)*3;

		memcpy(&norm, fileData + offset, sizeof(float)*3);
		offset += sizeof(float)*3;

		memcpy(&u, fileData + offset, sizeof(float));
		offset += sizeof(float);

		memcpy(&v, fileData + offset, sizeof(float));
		offset += sizeof(float);

		vertex[i].position.x = pos[0];
		vertex[i].position.y = pos[1];
		vertex[i].position.z = pos[2];
		vertex[i].normal.x = norm[0];
		vertex[i].normal.y = norm[1];
		vertex[i].normal.z = norm[2];
		vertex[i].u = u;
		vertex[i].v = v;
	}

	// Read in all face data
	for(i = 0; i < this->faces; i++)
	{
		float norm[3];
		int a, b, c;
				
		memcpy(&norm, fileData + offset, sizeof(float)*3);
		offset += sizeof(float)*3;

		memcpy(&a, fileData + offset, sizeof(int));
		offset += sizeof(int);

		memcpy(&b, fileData + offset, sizeof(int));
		offset += sizeof(int);

		memcpy(&c, fileData + offset, sizeof(int));
		offset += sizeof(int);

		face[i].normal.x = norm[0];
		face[i].normal.y = norm[1];
		face[i].normal.z = norm[2];
		face[i].a = a;
		face[i].b = b;
		face[i].c = c;
	}

	return true;
}

bool T3D::initVertices(unsigned int count)
{
	vertex = new T3DVertex[count];
	if(!vertex)
	{
		return false;
	}

	return true;
}

bool T3D::initFaces(unsigned int count)
{
	face = new T3DFace[count];
	if(!face)
	{
		return false;
	}

	return true;
}

void T3D::debug()
{
	unsigned int i;

	printf("\n----------\n");
	printf("T3D DEBUG:\n");
	printf("----------\n");
	printf("Vertex count: %d\n", vertices);
	printf("Face count: %d\n\n", faces);

	for(i = 0; i < this->vertices; i++)
	{
		Vector p = vertex[i].position;
		Vector n = vertex[i].normal;
		float u = vertex[i].u;
		float v = vertex[i].v;

		printf("Vertex: %d, POS: (%f, %f %f) NOR: (%f, %f %f) U: %f V: %f\n", i, p.x, p.y, p.z, n.x, n.y, n.z, u, v);
	}

	for(i = 0; i < this->faces; i++)
	{		
		Vector n = face[i].normal;
		int a = face[i].a;
		int b = face[i].b;
		int c = face[i].c;
		
		printf("Face: %d, NOR: (%f, %f %f) A: %d B: %d C: %d\n", i, n.x, n.y, n.z, a, b, c);
	}
}

T3DVertex *T3D::getVertexArray()
{
	return vertex;
}

T3DFace *T3D::getFaceArray()
{
	return face;
}

unsigned int T3D::getVertexCount()
{
	return this->vertices;
}

unsigned int T3D::getFaceCount()
{
	return this->faces;
}