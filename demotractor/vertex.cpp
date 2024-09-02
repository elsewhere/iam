//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "mmgr.h"
#endif

#include "vertex.hpp"

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//-------------------------------------------------------
//	Class code
//-------------------------------------------------------

Vertex::Vertex()
{
	position = Vector(0.0f, 0.0f, 0.0f);
	normal = Vector(0.0f, 0.0f, 0.0f);
	u = 0.0f;
	v = 0.0f;
}

Vertex::Vertex(Vector vec)
{
	position = vec;
	normal = Vector(0.0f, 0.0f, 0.0f);	
	u = 0.0f;
	v = 0.0f;
}

Vertex::Vertex(float x, float y, float z)
{
	position = Vector(x, y, z);
	normal = Vector(0.0f, 0.0f, 0.0f);
	u = 0.0f;
	v = 0.0f;
}

Vertex::~Vertex()
{
	position = Vector(0.0f, 0.0f, 0.0f);
	normal = Vector(0.0f, 0.0f, 0.0f);
	u = 0.0f;
	v = 0.0f;
}

//-------------------------------------------------------
//	Getters
//-------------------------------------------------------

Vector Vertex::getPosition()
{
	return position;
}

Vector Vertex::getOldPosition()
{
	return old;
}

Vector Vertex::getNormal()
{
	return normal;
}

//-------------------------------------------------------
//	Setters
//-------------------------------------------------------

void Vertex::setPosition(Vector vec)
{
	position = vec;
}

void Vertex::setNormal(Vector vec)
{
	normal = vec;
}

void Vertex::setUV(float uu, float vv)
{
	u = uu;
	v = vv;
}