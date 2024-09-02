//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>

#ifdef _DEBUG
	#include "mmgr.h"
#endif

#include "texture.hpp"
#include "globals.hpp"

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  Class code
//--------------------------------------------------------------------------------------------

Texture::Texture()
{
	width = 0;
	height = 0;
	bpp = 0;
	ID = 0;
}

Texture::~Texture()
{
	image.release();
}

//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Texture::loadTGA(char *name)
{
	if(!image.LoadTGA(name))
	{
		dmsErrorMsg("Texture::loadTGA() failed with \"%s\"", name);
		return false;
	}

	width = image.GetWidth();
	height = image.GetHeight();
	type = image.getType();

	return true;
}

bool Texture::loadPCX(char *name)
{
	if(!image.LoadPCX(name))
	{	
		dmsErrorMsg("Texture::loadPCX() failed with \"%s\"", name);
		return false;
	}

	width = image.GetWidth();
	height = image.GetHeight();
	type = image.getType();

	return true;
}

bool Texture::loadJPG(char *name)
{
	if(!image.LoadJPG(name))
	{		
		dmsErrorMsg("Texture::loadJPG() failed with \"%s\"", name);
		return false;
	}

	width = image.GetWidth();
	height = image.GetHeight();
	type = image.getType();

	return true;
}

bool Texture::loadPNG(char *name)
{
	if(!image.LoadPNG(name))
	{
		dmsErrorMsg("Texture::loadJPG() failed with \"%s\"", name);
		return false;
	}

	width = image.GetWidth();
	height = image.GetHeight();
	type = image.getType();

	return true;
}

bool Texture::loadPCXFromMemory(char *name, unsigned char *fileData, unsigned int fileSize)
{
	if(!image.LoadMemoryPCX(name, fileData, fileSize))
	{			
		dmsErrorMsg("Texture::loadPCXFromMemory() failed with \"%s\"", name);
		return false;
	}
		
	width = image.GetWidth();
	height = image.GetHeight();
	type = image.getType();	

	return true;
}

bool Texture::loadTGAFromMemory(char *name, unsigned char *fileData, unsigned int fileSize)
{
	if(!image.LoadMemoryTGA(name, fileData, fileSize))		
	{
		dmsErrorMsg("Texture::loadTGAFromMemory() failed with \"%s\"", name);
		return false;
	}
		
	width = image.GetWidth();
	height = image.GetHeight();
	type = image.getType();		

	return true;
}

bool Texture::loadJPGFromMemory(char *name, unsigned char *fileData, unsigned int fileSize)
{
	if(!image.LoadMemoryJPG(name, fileData, fileSize))
	{		
		dmsErrorMsg("Texture::loadJPGFromMemory() failed with \"%s\"", name);
		return false;
	}
		
	width = image.GetWidth();
	height = image.GetHeight();
	type = image.getType();

	return true;
}

void Texture::setID(int i)
{
	ID = i;
}

int Texture::getID()
{
	return ID;
}

unsigned int Texture::getHeight()
{
	return height;
}

unsigned int Texture::getWidth()
{
	return width;
}

unsigned int Texture::getType()
{
	return type;
}

unsigned int *Texture::getImgData()
{
	return image.GetPtr();
}