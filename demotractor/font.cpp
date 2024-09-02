//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "font.hpp"
#include "globals.hpp"

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  Class code
//--------------------------------------------------------------------------------------------

Font::Font()
{
	fontMap = NULL;
	fontX = 0;
	fontY = 0;
	dx = 0.0f;
	dy = 0.0f;
}

Font::~Font()
{

}

bool Font::init(int x, int y, char *name)
{
	extern TextureManager *tManager;
	char buf[256] = {0};
	int i;
	float sx, sy;

	if(!tManager) return false;

	fontMap = tManager->getTextureName(name);
	if(!fontMap)
	{	
		dmsErrorMsg("Couldn't get texture \"%s\"", name);		
		return false;		
	}

	fontX = x;
	fontY = y;

	dx = 1.0f / (fontMap->getWidth() / fontX);
	dy = 1.0f / (fontMap->getHeight() / fontY);
	sx = 0;
	sy = 0;
	
	for(i = 0; i < 57; i++)
	{		
		texels[i].u1 = sx;
		texels[i].v1 = sy;
		texels[i].u2 = sx+dx;
		texels[i].v2 = sy+dy;

		sx += dx;
		
		if(sx >= 1.0f)
		{
			sy += dy;
			sx = 0;
		}
	}

	return true;
}

bool Font::write2D(float x, float y, float a, float scale, const char *msg, ...)
{
	char text[256];
	int lenght, i, index;
	float sx, sy, space;
	float tmp = 0;
	
	space = fontX / 2.0f;

	va_list ap;

	if(msg == NULL)
	{
		return false;
	}
	
	va_start(ap, msg);
	    vsprintf(text, msg, ap);
	va_end(ap);

	// muutetaan uppercaseksi ja otetaan pituus
	strupr(text);
	lenght = strlen(text);

	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_BLEND);		
	myEnable(MY_TEXTURE_2D);	
	glBindTexture(GL_TEXTURE_2D, fontMap->getID());

	sx = 0;
	sy = 0;	
	tmp = x;
	
	glColor4f(1.0f, 1.0f, 1.0f, a);
	glBegin(GL_QUADS);

	for(i = 0; i < lenght; i++)
	{	
		index = text[i] - 34; // 48 on ascii koodi '0':lle		
		FontUV t = texels[index];		

		// jos textin merkki != välilyönti
		if(text[i] != 32)
		{
			glTexCoord2f( t.u1, t.v2); glVertex2f( (tmp      ), (y-fontY*scale));
			glTexCoord2f( t.u2, t.v2); glVertex2f( (tmp+fontX*scale), (y-fontY*scale));
			glTexCoord2f( t.u2, t.v1); glVertex2f( (tmp+fontX*scale), y);
			glTexCoord2f( t.u1, t.v1); glVertex2f( (tmp      ), y);
		}

		tmp += fontX*scale;
		sx += dx;
		
		if(sx >= 1.0f)
		{
			sy += dy;
			sx = 0;
		}
	}

	glEnd();

	myDisable(MY_BLEND);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);

	return true;
}

bool Font::write3D(float x, float y, float z, float a, const char *msg, ...)
{	
	char text[256];
	int lenght, i, index;
	float sx, sy, space;
	float tmp = 0;

	space = fontX / 2.0f;

	va_list ap;

	if(msg == NULL)
	{
		return false;
	}
	
	va_start(ap, msg);
	    vsprintf(text, msg, ap);
	va_end(ap);

	// muutetaan uppercaseksi ja otetaan pituus
	strupr(text);
	lenght = strlen(text);
	
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_BLEND);	
	myEnable(MY_TEXTURE_2D);	
	glBindTexture(GL_TEXTURE_2D, fontMap->getID());
		
	sx = 0;
	sy = 0;
	tmp = -(dx*space)*(lenght*0.5f);
	
	glTranslatef(x, y, z);
	
	glColor4f(1.0f, 1.0f, 1.0f, a);
	
	glBegin(GL_QUADS);

	for(i = 0; i < lenght; i++)
	{	
		index = text[i] - 34; // 48 on ascii koodi '0':lle		
		FontUV t = texels[index];

		tmp += dx*space;

		// jos textin merkki != välilyönti
		if(text[i] != 32)
		{
			glTexCoord2f( t.u1, t.v2); glVertex3f(-1.0f+tmp, -1.0f,  0.0f);
			glTexCoord2f( t.u2, t.v2); glVertex3f( 1.0f+tmp, -1.0f,  0.0f);
			glTexCoord2f( t.u2, t.v1); glVertex3f( 1.0f+tmp,  1.0f,  0.0f);
			glTexCoord2f( t.u1, t.v1); glVertex3f(-1.0f+tmp,  1.0f,  0.0f);
		}

		sx += dx;
		
		if(sx >= 1.0f)
		{
			sy += dy;
			sx = 0;
		}
	}

	glEnd();

	myDisable(MY_BLEND);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);

	return true;
}

bool Font::write3DEx(float x, float y, float z, float rx, float ry, float rz, float angle, float a, const char *msg, ...)
{	
	char text[256];
	int lenght, i, index;
	float sx, sy, space;
	float tmp = 0;

	space = fontX / 2.0f;

	va_list ap;

	if(msg == NULL)
	{
		return false;
	}
	
	va_start(ap, msg);
	    vsprintf(text, msg, ap);
	va_end(ap);

	// muutetaan uppercaseksi ja otetaan pituus
	strupr(text);
	lenght = strlen(text);
	
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_BLEND);	
	myEnable(MY_TEXTURE_2D);	
	glBindTexture(GL_TEXTURE_2D, fontMap->getID());
		
	sx = 0;
	sy = 0;
	tmp = -(dx*space)*(lenght*0.5f);
	
	glTranslatef(x, y, z);
	glRotatef(angle, rx, ry, rz);
	
	glColor4f(1.0f, 1.0f, 1.0f, a);
	
	glBegin(GL_QUADS);

	for(i = 0; i < lenght; i++)
	{	
		index = text[i] - 34; // 48 on ascii koodi '0':lle		
		FontUV t = texels[index];

		tmp += dx*space;

		// jos textin merkki != välilyönti
		if(text[i] != 32)
		{
			glTexCoord2f( t.u1, t.v2); glVertex3f(-1.0f+tmp, -1.0f,  0.0f);
			glTexCoord2f( t.u2, t.v2); glVertex3f( 1.0f+tmp, -1.0f,  0.0f);
			glTexCoord2f( t.u2, t.v1); glVertex3f( 1.0f+tmp,  1.0f,  0.0f);
			glTexCoord2f( t.u1, t.v1); glVertex3f(-1.0f+tmp,  1.0f,  0.0f);
		}

		sx += dx;
		
		if(sx >= 1.0f)
		{
			sy += dy;
			sx = 0;
		}
	}

	glEnd();

	myDisable(MY_BLEND);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);

	return true;
}

int Font::getFontWidth()
{
	return fontX;
}

int Font::getFontHeight()
{
	return fontY;
}