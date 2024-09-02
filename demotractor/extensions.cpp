#include "extensions.hpp"

PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;

bool loadExtensions(HDC hdc)
{		
	PFNWGLGETEXTENSIONSSTRINGARBPROC wglEXT;
	char *str = (char *)glGetString(GL_EXTENSIONS);
	char *str2 = NULL;
	
	if(str)
	{
		// Haetaan wglGetExtensionsStringARB-laajennus
		// Jolla saadaan lista win32-tyyppisistä laajennuksista
		// Samaan tyyliin kuin glGetString
		wglEXT = (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress("wglGetExtensionsStringARB");

		// Jos laajennus löytyi, voidaan hakea tiedot tuetuista WGL-laajennuksista
		if(wglEXT)
		{
			str2 = (char *) wglEXT(hdc);
		}		

		// tukeeko kortti multitexture-laajennusta
		if(strstr(str, "GL_ARB_multitexture "))
		{		
			glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
			glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
			if(!glMultiTexCoord2fARB || !glActiveTextureARB)
			{				
				// Jos kortti ei tue laajennuksia
			}			
		}
		else
		{
			//Msg("Extension GL_ARB_multitexture is not supported\n");
		}

		// tarkistetaan tukeeko kortti cube mappeja
		if(!strstr(str, "GL_EXT_texture_cube_map"))
		{
			//Msg("Extension GL_EXT_texture_cube_map is not supported\n");
			//return false;
		}
	}

	return true;
}
