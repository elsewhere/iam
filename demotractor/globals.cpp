//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

// This extracts MFC stuff off
#define WIN32_LEAN_AND_MEAN
#define VC_LEANMEAN           

#include <stdio.h>
#include "globals.hpp"
#include "list.hpp"
#include "PAKfile.hpp"

//--------------------------------------------------------------------------------------------
//  This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

namespace TRACTION_DEMOTRACTOR
{

//--------------------------------------------------------------------------------------------
//  Enumerations for OpenGL statewrapper
//--------------------------------------------------------------------------------------------

struct enums
{
	bool flag;
	int code;
	char *name;
} enumTable[] = {	
					{ false, GL_BLEND, "Blending" },
					{ false, GL_TEXTURE_1D, "1D Textures" },
					{ false, GL_TEXTURE_2D, "2D Textures" },
					{ false, GL_DEPTH_TEST, "Depth testing" },
					{ false, GL_LIGHTING, "Lighting" }, 
					{ false, GL_LIGHT0, "Light0" },
					{ false, GL_LIGHT1, "Light1" },
					{ false, GL_LIGHT2, "Light2" },
					{ false, GL_LIGHT3, "Light3" },
					{ false, GL_LIGHT4, "Light4" },
					{ false, GL_LIGHT5, "Light5" },
					{ false, GL_LIGHT6, "Light6" },
					{ false, GL_LIGHT7, "Light7" },
					{ false, GL_NORMALIZE, "Normalization" },					
					{ false, GL_CULL_FACE, "Face culling" },
					{ false, GL_TEXTURE_CUBE_MAP_EXT, "Cube mapping" },
					{ false, GL_TEXTURE_GEN_S, "Texture coordinate (S) generation" },
					{ false, GL_TEXTURE_GEN_T, "Texture coordinate (T) generation" },
					{ false, GL_TEXTURE_GEN_R, "Texture coordinate (R) generation" },
					{ false, GL_FOG, "Fog" },
					{ false, GL_LINE_SMOOTH, "Antialised lines" },
					{ false, GL_POINT_SMOOTH, "Antialised points" },
					{ false, DUMMY, "Depth Mask" }
				};

	int nIndex = sizeof(enumTable) / 12;
	int prev_src = 0;
	int prev_dst = 0;
};

//--------------------------------------------------------------------------------------------
//  This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

namespace TRACTION_DEMOTRACTOR
{
	extern "C" __declspec(dllexport) void dmsSetLogging(bool f);
	extern "C" __declspec(dllexport) bool dmsInit();
	extern "C" __declspec(dllexport) bool dmsInitEx(int screenX, int screenY, int bpp, bool fullscreen, bool soundFlag, bool vsync, char *pakFile);
	extern "C" __declspec(dllexport) bool dmsInitDummyEx(int screenX, int screenY, int bpp, bool fullscreen, bool soundFlag, bool vsync);
	extern "C" __declspec(dllexport) void dmsSetGamma(int gamma);
	extern "C" __declspec(dllexport) void dmsShutDown();
	extern "C" __declspec(dllexport) void dmsCheckControls(float jump);
	extern "C" __declspec(dllexport) void dmsPerspective2D(int w, int h);
	extern "C" __declspec(dllexport) void dmsPerspective3D();
	extern "C" __declspec(dllexport) void dmsSetFOV(float f);

	extern "C" __declspec(dllexport) void dmsInitTimer();
	extern "C" __declspec(dllexport) void dmsUpdateTimer();
	extern "C" __declspec(dllexport) void dmsAdjustTime(float f);

	extern "C" __declspec(dllexport) void dmsInitSound();
	extern "C" __declspec(dllexport) void dmsLoadSong(const char *fileName);
	extern "C" __declspec(dllexport) void dmsLoadSongFromMemory(unsigned char *fileData, unsigned int fileSize);
	extern "C" __declspec(dllexport) void dmsPlaySong(bool f);
	extern "C" __declspec(dllexport) bool dmsCheckSyncPoint();
	
	extern "C" __declspec(dllexport) void dmsAddTexture(const char *fileName, const char *name);
	extern "C" __declspec(dllexport) void dmsAddRenderToTexture(int width, int height, const char *name);
	extern "C" __declspec(dllexport) void dmsUploadTextures();	
	extern "C" __declspec(dllexport) void dmsBindTexture(unsigned int mode, const char *name);

	extern "C" __declspec(dllexport) void dmsAddFont(int x, int y, const char *name);

	extern "C" __declspec(dllexport) void dmsInitObjects();
	extern "C" __declspec(dllexport) void dmsAddObject(const char *fileName, const char *name);
	extern "C" __declspec(dllexport) void dmsDrawObject(const char *name);

	extern "C" __declspec(dllexport) void dmsInitEffects();
	extern "C" __declspec(dllexport) void dmsAddEffect(unsigned long st, unsigned long et, const char *name, Base *effect);

	extern "C" __declspec(dllexport) void dmsRun();	

	extern "C" __declspec(dllexport) void dmsWrite2D(float x, float y, float a, float scale, char *fontName, char *msg);
	extern "C" __declspec(dllexport) void dmsWrite3D(float x, float y, float z, float a, char *fontName, char *msg);
	extern "C" __declspec(dllexport) void dmsWrite3DEx(float x, float y, float z, float rx, float ry, float rz, float angle, float a, char *fontName, char *msg);
	extern "C" __declspec(dllexport) void dmsSetWindowTitle(const char *msg);

	extern "C" __declspec(dllexport) float dmsGetTime();
	extern "C" __declspec(dllexport) float dmsGetDeltaTime();
	extern "C" __declspec(dllexport) float *dmsGetSpectrum();
	extern "C" __declspec(dllexport) T3D *dmsGetObject(const char *name);
	extern "C" __declspec(dllexport) Texture *dmsGetTexture(const char *name);
	extern "C" __declspec(dllexport) Font *dmsGetFont(const char *name);
	extern "C" __declspec(dllexport) HDC dmsGetHDC();
	extern "C" __declspec(dllexport) int dmsGetWindowWidth();
	extern "C" __declspec(dllexport) int dmsGetWindowHeight();
	
	extern "C" __declspec(dllexport) bool dmsMakePAK(const char *src, const char *dst);
	extern "C" __declspec(dllexport) void dmsLoadPic(char *name);
	extern "C" __declspec(dllexport) void dmsDrawMeter(int current, int max, float width, float height, float xpos, float ypos, float zpos);

    extern "C" bool readSetup(char *name);
	extern "C" bool readPak(char *name);
}

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  Global instances
//--------------------------------------------------------------------------------------------

GLWindow glWindow;
GLSystem glSystem;
Log *logFile = NULL;
Timer *timer = NULL;
Sound *sound = NULL;
Song music;
TextureManager *tManager = NULL;
EffectManager *eManager = NULL;
FontManager *fManager = NULL;
ObjectManager *oManager = NULL;
List <int> syncs;
unsigned int currentSync = 0;
bool enableLogging = false;

//--------------------------------------------------------------------------------------------
//  Prototype implementations
//--------------------------------------------------------------------------------------------

extern "C" __declspec(dllexport) void dmsMsg(const char *text, ...)
{	
	va_list ap;
	char buf[1024] = {0};
	
	if(!text) return;
				
	va_start(ap, text);
	    vsprintf(buf, text, ap);
	va_end(ap);
	
	if(logFile)
	{
		logFile->writeToLog(buf);
	}
}	

extern "C" __declspec(dllexport) void dmsErrorMsg(const char *text, ...)
{	
	va_list ap;
	char buf[1024] = {0};
	
	if(!text) return;
				
	va_start(ap, text);
	    vsprintf(buf, text, ap);
	va_end(ap);
	
	if(logFile)
	{
		logFile->writeToLog(buf);
	}
	else
	{
		MessageBox(NULL, buf, "DemoSystem Error", MB_OK);
	}
}	

extern "C" __declspec(dllexport) void dmsSetLogging(bool f)
{
	enableLogging = f;
}

extern "C" __declspec(dllexport) bool dmsInit()
{
	if(enableLogging)
	{
		logFile = Log::create("loki.log");
		if(!logFile)
		{
			throw "One of the class instances didn't initialize";
		}
	}
	
	timer = Timer::create();
	sound = Sound::create();
	tManager = TextureManager::create();
	eManager = EffectManager::create();
	fManager = FontManager::create();
	oManager = ObjectManager::create();
	
	// Check for existing instances
	if(	!timer ||
		!sound ||
		!tManager ||
		!eManager ||
		!fManager ||
		!oManager
	) 
	{
		throw "One of the class instances didn't initialize";			
	}

	sound->setEnabled(true);

	// Create and setup opengl window/system
	if(!glWindow.createWindow(640, 480, 32, false))
	{		
		throw "Error while creating window";			
	}

	// Init opengl system
	glSystem.init(640, 480);

	// Read setup file: ie. Textures and sounds are loaded here
	if(!readSetup("setup.ini"))
	{
		throw "dmsInit() error: \"setup.ini\" not found";
	}

	// Initialize 3D-objects
	if(!oManager->initObjects()) return false;
				
	// Init timer
	timer->init();

	if(sound)
	{
		if(!sound->checkEnabled())
		{
			sound->setVolume(NULL, 0);
		}

		music.start();
	}	

	return true;
}

extern "C" __declspec(dllexport) bool dmsInitEx(int screenX, int screenY, int bpp, bool fullscreen, bool soundFlag, bool vsync, char *pakFile)
{
	if(enableLogging)
	{
		logFile = Log::create("loki.log");
		if(!logFile)
		{
			throw "One of the class instances didn't initialize";
		}
	}
	
	timer = Timer::create();
	sound = Sound::create();
	tManager = TextureManager::create();
	eManager = EffectManager::create();
	fManager = FontManager::create();
	oManager = ObjectManager::create();
	
	// Check for existing instances
	if(	!timer ||
		!sound ||
		!tManager ||
		!eManager ||
		!fManager ||
		!oManager
	) 
	{
		throw "One of the class instances didn't initialize";			
	}

	// Toggle sound on/off
	sound->setEnabled(soundFlag);

	// Create and setup opengl window/system
	if(!glWindow.createWindow(screenX, screenY, bpp, fullscreen))
	{		
		throw "Error while creating window";			
	}

	// Init opengl system
	glSystem.init(screenX, screenY);	
	
	if(!pakFile)
	{
		// Read setup file: ie. Textures and sounds are loaded here
		if(!readSetup("setup.ini"))
		{
			throw "dmsInit() error: \"setup.ini\" not found";
		}
	}
	else
	{
		if(!readPak(pakFile))
		{
			char buf[256] = {0};

			sprintf(buf, "dmsInit(): Error while loading %s\n", pakFile);
			throw buf;
		}
	}

	dmsLoadPic("loadpic.jpg");

	// Initialize 3D-objects
	if(!oManager->initObjects()) return false;

	// Init timer
	timer->init();
	
	if(sound)
	{		
		if(!sound->checkEnabled())
		{
			sound->setVolume(NULL, 0);
		}	
	}

	return true;
}

extern "C" __declspec(dllexport) bool dmsInitDummyEx(int screenX, int screenY, int bpp, bool fullscreen, bool soundFlag, bool vsync)
{
	if(enableLogging)
	{
		logFile = Log::create("loki.log");
		if(!logFile)
		{
			throw "One of the class instances didn't initialize";
		}
	}

	timer = Timer::create();
	sound = Sound::create();
	tManager = TextureManager::create();
	eManager = EffectManager::create();
	fManager = FontManager::create();
	oManager = ObjectManager::create();
	
	// Check for existing instances
	if(	!timer ||
		!sound ||
		!tManager ||
		!eManager ||
		!fManager ||
		!oManager
	) 
	{
		throw "One of the class instances didn't initialize";			
	}

	// Toggle sound on/off
	sound->setEnabled(soundFlag);

	// Create and setup opengl window/system
	if(!glWindow.createWindow(screenX, screenY, bpp, fullscreen))
	{		
		throw "Error while creating window";			
	}

	// Init opengl system
	glSystem.init(screenX, screenY);
		
	return true;
}

extern "C" __declspec(dllexport) void dmsSetGamma(int gamma)
{
	HDC hdc = dmsGetHDC();
	
	if(hdc)
	{
		struct ramp
		{
			unsigned short red[256];
			unsigned short green[256];
			unsigned short blue[256];
		} colorRamp;
		
		// float gamma valuen pit‰‰ olla 0..2f(??)
		if(GetDeviceGammaRamp(hdc, (LPVOID)&colorRamp))
		{
			int i;
			
			for(i = 0; i < 256; i++)
			{					

				//colorRamp.red[i] = (WORD)min(65535, max(0, pow((i+1) / 256.0, float(gamma) / 256.0f) * 65535 + 0.5));
				//colorRamp.green[i] = (WORD)min(65535, max(0, pow((i+1) / 256.0, float(gamma) / 256.0f) * 65535 + 0.5));
				//colorRamp.blue[i] = (WORD)min(65535, max(0, pow((i+1) / 256.0, float(gamma) / 256.0f) * 65535 + 0.5));
				
				if(gamma < 0)
				{
					//colorRamp.red[i] = unsigned short((255 - -gamma)*(i));
					//colorRamp.green[i] = unsigned short((255 - -gamma)*(i));
					//colorRamp.blue[i] = unsigned short((255 - -gamma)*(i));										

					//colorRamp.red[i] = (WORD)(pow((i+1) / 256.0, (float(-gamma) / 256.0f)) * 65535);
					//colorRamp.green[i] = (WORD)(pow((i+1) / 256.0, (float(-gamma) / 256.0f)) * 65535);
					//colorRamp.blue[i] = (WORD)(pow((i+1) / 256.0, (float(-gamma) / 256.0f)) * 65535);

					colorRamp.red[i] = (WORD)(pow(i, 1.0f + (float(gamma) / 256.0f)) * 255);
					colorRamp.green[i] = (WORD)(pow(i, 1.0f + (float(gamma) / 256.0f)) * 255);
					colorRamp.blue[i] = (WORD)(pow(i, 1.0f + (float(gamma) / 256.0f)) * 255);
				}
				else
				{
					//colorRamp.red[i] = 65535 - unsigned short((255 - gamma)*(255 - i));
					//colorRamp.green[i] = 65535 - unsigned short((255 - gamma)*(255 - i));
					//colorRamp.blue[i] = 65535 - unsigned short((255 - gamma)*(255 - i));

					colorRamp.red[i] = (WORD)(pow((i+1) / 256.0, 1.0f + (float(gamma) / 256.0f)) * 65535);
					colorRamp.green[i] = (WORD)(pow((i+1) / 256.0, 1.0f + (float(gamma) / 256.0f)) * 65535);
					colorRamp.blue[i] = (WORD)(pow((i+1) / 256.0, 1.0f + (float(gamma) / 256.0f)) * 65535);					
				}				
			}
			
			bool b = SetDeviceGammaRamp(hdc, (LPVOID)&colorRamp);
			if(!b)
			{
				dmsErrorMsg("dmsSetGamma error: SetDeviceGammaRamp failed\n");
			}
		}
		else
		{
			dmsErrorMsg("dmsSetGamma error: GetDeviceGammaRamp failed\n");
		}		
	}
}

extern "C" __declspec(dllexport) void dmsShutDown()
{	
	syncs.freeList();
	music.stop();
	music.release();

	if(oManager)
	{
		oManager->release();
		oManager = NULL;
	}

	if(fManager)
	{
		fManager->release();
		fManager = NULL;
	}

	if(eManager)
	{
		eManager->release();
		eManager = NULL;
	}

	if(tManager)
	{
		tManager->release();
		tManager = NULL;
	}

	if(sound)
	{
		sound->close();
		sound = NULL;
	}

	if(timer)
	{
		timer->release();
		timer = NULL;
	}	

	if(logFile)
	{
		logFile->release();
		logFile = NULL;
	}
}

extern "C" __declspec(dllexport) void dmsCheckControls(float jump)
{
	// If LEFT ARROW was pressed -> ReWind small amount of time
	if(GetAsyncKeyState(VK_LEFT))
	{
		dmsAdjustTime(-jump);					
	}

	// If RIGHT ARROW was pressed -> FastForward small amount of time
	if(GetAsyncKeyState(VK_RIGHT))
	{
		dmsAdjustTime(jump);					
	}
}

extern "C" __declspec(dllexport) void dmsPerspective2D(int w, int h)
{
	glWindow.setPerspective2D(w, h);
}

extern "C" __declspec(dllexport) void dmsPerspective3D()
{
	glWindow.setPerspective3D();
}

extern "C" __declspec(dllexport) void dmsSetFOV(float f)
{
	glSystem.setFov(f);
}

extern "C" __declspec(dllexport) void dmsInitTimer()
{
	if(timer)
	{
		timer->init();
	}
	else
	{
		throw "Timer not found";
	}
}

extern "C" __declspec(dllexport) void dmsAdjustTime(float f)
{
	if(timer)
	{
		timer->addMs(f);
	}
	
	if(sound)
	{
		if((music.getPosition()+f) > 0)
		{
			music.setPosition((unsigned int)(music.getPosition()+f));
		}
		else
		{
			music.setPosition(0);
		}
	}
}

extern "C" __declspec(dllexport) void dmsInitSound()
{
	if(sound)
	{
		if(!sound->init())
		{
			throw "Sound initialization failed";
		}				
	}
	else
	{
		throw "Sound not found";
	}
}

extern "C" __declspec(dllexport) void dmsLoadSong(const char *fileName)
{
	if(sound)
	{
		if(!music.loadStream((char *)fileName))
		{			
			char buf[256] = {0};

			sprintf(buf, "dmsLoadSong() error: Couldn't load song \"%s\"", fileName);
			throw buf;
		}								
	}
	else
	{
		throw "Sound not found";
	}
}

extern "C" __declspec(dllexport) void dmsLoadSongFromMemory(unsigned char *fileData, unsigned int fileSize)
{
	if(sound)
	{
		if(!music.loadStream2((char *)fileData, fileSize))
		{			
			char buf[256] = {0};

			sprintf(buf, "dmsLoadSong() error: Couldn't load song from data file");
			throw buf;
		}									
	}
	else
	{
		throw "Sound not found";
	}
}

extern "C" __declspec(dllexport) void dmsPlaySong(bool f)
{
	if(sound)
	{
		(f) ? music.start() : music.stop();		
	}
	else
	{
		throw "Sound not found";
	}
}

extern "C" __declspec(dllexport) bool dmsCheckSyncPoint()
{
	if(sound)
	{
		Node <int> *node = syncs.get(currentSync);
		if(node)
		{
			int *value = node->a;
			if(value)
			{
				if(music.getPosition() >= *value) 
				{
					sound->setSync(true);
					currentSync ++;
					return true;
				}
				else
				{
					sound->setSync(false);
					return false;
				}
			}		
		}
	}
	
	return false;
}

extern "C" __declspec(dllexport) void dmsUploadTextures()
{
	if(tManager)
	{
		// Upload textures to the OpenGL
		tManager->uploadTextures();
	}
	else
	{
		throw "dmsUploadTextures() failed: No TextureManager found";
	}
}

extern "C" __declspec(dllexport) void dmsAddTexture(const char *fileName, const char *name)
{
	if(tManager)
	{
		int type = -1;

		(strstr(fileName, ".pcx")) ? type = PCX : type = type;
		(strstr(fileName, ".tga")) ? type = TGA : type = type;
		(strstr(fileName, ".jpg")) ? type = JPG : type = type;
		(strstr(fileName, ".png")) ? type = PNG : type = type;

		if(!tManager->addTexture((char *)fileName, (char *)name, type))
		{
			char buf[256] = {0};

			sprintf(buf, "TextureManager::addTexture() failed with \"%s\"", fileName);
			throw buf;
		}
	}
	else
	{
		throw "TextureManager::addTexture() failed! No TextureManager found";
	}
}

extern "C" __declspec(dllexport) void dmsAddRenderToTexture(int width, int height, const char *name)
{
	if(tManager)
	{		
		if(!tManager->addRenderToTexture(width, height, (char *)name))
		{
			char buf[256] = {0};

			sprintf(buf, "TextureManager::addTexture() failed with \"%s\"", name);
			throw buf;
		}
	}
	else
	{
		throw "TextureManager::addTexture() failed! No TextureManager found";
	}
}

extern "C" __declspec(dllexport) void dmsInitObjects()
{
	if(oManager)
	{
		if(!oManager->initObjects())
		{
			throw "Object initialization failed";
		}
	}
	else
	{
		throw "ObjectManager not found";
	}
}

extern "C" __declspec(dllexport) void dmsAddObject(const char *fileName, const char *name)
{
	if(oManager)
	{
		if(!oManager->addObject((char *)fileName, (char *)name))
		{
			char buf[256] = {0};

			sprintf(buf, "ObjectManager::addObject() failed with \"%s\"", fileName);
			throw buf;
		}
	}
	else
	{
		throw "ObjectManager::addObject() failed! No ”bjectManager found";
	}
}

extern "C" __declspec(dllexport) void dmsDrawObject(const char *name)
{
    T3D *t3d = dmsGetObject(name);

	T3DVertex *vert = t3d->getVertexArray();
	T3DFace *face = t3d->getFaceArray();

	if(vert && face)
	{						
		for(int i = 0; i < t3d->getFaceCount(); i++)
		{
			T3DVertex v1, v2, v3;

			v1 = vert[face[i].a];
			v2 = vert[face[i].b];
			v3 = vert[face[i].c];
													
			glBegin(GL_TRIANGLES);
				glTexCoord2f(v1.u, v1.v); glVertex3f(v1.position.x, v1.position.y, v1.position.z);
				glTexCoord2f(v2.u, v2.v); glVertex3f(v2.position.x, v2.position.y, v2.position.z);
				glTexCoord2f(v3.u, v3.v); glVertex3f(v3.position.x, v3.position.y, v3.position.z);					
			glEnd();
		}
	}
}

extern "C" __declspec(dllexport) void dmsInitEffects()
{
	if(eManager)
	{
		if(!eManager->initEffects())
		{
			throw "Effect initialization failed";
		}
	}
	else
	{
		throw "EffectManager not found";
	}
}

extern "C" __declspec(dllexport) void dmsAddEffect(unsigned long st, unsigned long et, const char *name, Base *effect)
{
	if(eManager)
	{	
		if(!eManager->addEffect(st, et, effect, (char *)name))		
		{
			char buf[256] = {0};

			sprintf(buf, "EffecteManager::addEffect() failed with \"%s\"", name);
			throw buf;
		}
	}
	else
	{
		throw "EffectManager::addEffect() failed! No EffectManager found";
	}
}

extern "C" __declspec(dllexport) void dmsRun()
{
	if(eManager)
	{
		Base *effect = NULL;
		float time;
		unsigned int i;

		time = dmsGetTime();

		for(i = 0; i < eManager->getEffectCount(); i++)
		{
			effect = eManager->getEffect(i);
			if(effect && effect->isActive(time))
			{
				effect->update();
				effect->draw();
			}
		}		

		glFlush();
	}
	else
	{
		throw "EffectManager not found";
	}
}

extern "C" __declspec(dllexport) void dmsBindTexture(unsigned int mode, const char *name)
{
	Texture *texture = NULL;

	if(tManager)
	{
		texture = tManager->getTextureName((char *)name);
		if(!texture)
		{
			char buf[256] = {0};

			sprintf(buf, "TextureManager::getTextureName(): Texture \"%s\" not found", name);
			throw buf;
		}

		glBindTexture(GL_TEXTURE_2D, texture->getID());
	}
	else
	{
		throw "TextureManager::getTextureName() failed! No TextureManager found";
	}	
}

extern "C" __declspec(dllexport) void dmsAddFont(int x, int y, const char *name)
{
	if(fManager)
	{
		char buf[256] = {0};

		if(!fManager->addFont(x, y, (char *)name))
		{
			sprintf(buf, "dmsAddFont() error: %s\n", name);
			throw buf;
		}
	}
	else
	{
		throw "FontManager not found";
	}
}

extern "C" __declspec(dllexport) void dmsUpdateTimer()
{
	if(timer)
	{
		timer->update();
	}
	else
	{
		throw "Timer not found";
	}
}

extern "C" __declspec(dllexport) void dmsWrite2D(float x, float y, float a, float scale, char *fontName, char *msg)
{
	if(fManager)
	{	
		float width = glWindow.getWidth();
		float height = glWindow.getHeight(); 

		glWindow.setPerspective2D(width, height);

		fManager->write2D(x, height - y, a, scale, fontName, msg);

		glWindow.setPerspective3D();
	}
	else
	{
		throw "FontManager not found";
	}
}

extern "C" __declspec(dllexport) void dmsWrite3D(float x, float y, float z, float a, char *fontName, char *msg)
{
	if(fManager)
	{		
		fManager->write3D(x, y, z, a, fontName, msg);
	}
	else
	{
		throw "FontManager not found";
	}
}

extern "C" __declspec(dllexport) void dmsWrite3DEx(float x, float y, float z, float rx, float ry, float rz, float angle, float a, char *fontName, char *msg)
{
	if(fManager)
	{		
		fManager->write3DEx(x, y, z, rx, ry, rz, angle, a, fontName, msg);
	}
	else
	{
		throw "FontManager not found";
	}
}

extern "C" __declspec(dllexport) void dmsSetWindowTitle(const char *msg)
{
	HWND handle = glWindow.getHandle();

	SetWindowText(handle, msg);
}

extern "C" __declspec(dllexport) float dmsGetTime()
{
	if(timer)
	{
		return timer->getTimeMs();
	}
	else
	{
		throw "Timer not found";
	}
}

extern "C" __declspec(dllexport) float dmsGetDeltaTime()
{
	if(timer)
	{
		return timer->getDeltaTime();
	}
	else
	{
		throw "Timer not found";
	}
}

extern "C" __declspec(dllexport) float *dmsGetSpectrum()
{
	if(sound)
	{
		return sound->getSpectrum();
	}

	return NULL;
}

extern "C" __declspec(dllexport) T3D *dmsGetObject(const char *name)
{
	if(oManager)
	{
		return oManager->getObjectName((char *)name);
	}
	else
	{
		throw "ObjectManager not found";
	}
}

extern "C" __declspec(dllexport) Texture *dmsGetTexture(const char *name)
{
	if(tManager)
	{
		return tManager->getTextureName((char *)name);
	}
	else
	{
		throw "TextureManager not found";
	}
}

extern "C" __declspec(dllexport) Font *dmsGetFont(const char *name)
{
	if(fManager)
	{
		return fManager->getFontName((char *)name);
	}
	else
	{
		throw "TextureManager not found";
	}
}

extern "C" __declspec(dllexport) HDC dmsGetHDC()
{
	return glWindow.getHDC();
}

extern "C" __declspec(dllexport) int dmsGetWindowWidth()
{
	return glWindow.getWidth();
}

extern "C" __declspec(dllexport) int dmsGetWindowHeight()
{
	return glWindow.getHeight();
}

// Tarkistaa GLSystemin tilan
extern "C" __declspec(dllexport) void checkState()
{
	int i, nIndex;
	char buf[256] = {0};

	dmsMsg("<b>OpenGL State Output:</b>\n");

	// Lasketaan enumtablen indexien m‰‰r‰
	nIndex = sizeof(enumTable) / 12;
			
	for(i = 0; i < nIndex; i++)
	{			
		if(enumTable[i].flag)
		{
			dmsMsg("%s: set\n", enumTable[i].name);							
		}
		else
		{
			dmsMsg("%s: unset\n", enumTable[i].name);			
		}
	}
}

extern "C" __declspec(dllexport) void myEnable(int c)
{			
	if(c >= 0 && c < nIndex)
	{
		if(!enumTable[c].flag)
		{
			enumTable[c].flag = true;
			if(c == MY_DEPTH_MASK)
			{				
				glDepthMask(GL_TRUE);				
			}
			else
			{						
				glEnable(enumTable[c].code);				
			}
		}
	}	
}

extern "C" __declspec(dllexport) void myDisable(int c)
{				
	if(c >= 0 && c < nIndex)
	{
		if(enumTable[c].flag)
		{
			enumTable[c].flag = false;
			if(c == MY_DEPTH_MASK)
			{				
				glDepthMask(GL_FALSE);
			}
			else
			{						
				glDisable(enumTable[c].code);
			}
		}
	}
}

extern "C" __declspec(dllexport) void myBlendFunc(int src, int dst)
{
	int s, d;

	s = prev_src;
	d = prev_dst;

	if(s != src || d != dst)
	{
		prev_src = src;
		prev_dst = dst;
		glBlendFunc(src, dst);
	}
}

extern "C" __declspec(dllexport) bool dmsMakePAK(const char *src, const char *dst)
{
	PAKFile pak;

	if(!pak.create((char *)src, (char *)dst))
	{
		return false;
	}

	return true;
}

extern "C" __declspec(dllexport) void dmsDrawMeter(int current, int max, float width, float height, float xpos, float ypos, float zpos)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_TEST);
	
	glLoadIdentity();

	glTranslatef(xpos, ypos, zpos);
	glColor4f(0.34f, 0.67f, 0.98f, 1.0f);

	glBegin(GL_QUADS);
		glVertex3f(  0.0f,    0.0f, 0);
		glVertex3f( width,    0.0f, 0);
		glVertex3f( width,  height, 0);
		glVertex3f(  0.0f,  height, 0);	
	glEnd();	

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	float bx = (float)width / (float)max;

	glLoadIdentity();
	glTranslatef(xpos, ypos, zpos);

	for(int i = 0; i < current; i++)
	{			
		glBegin(GL_QUADS);
			glVertex3f(		i*bx,     0.0f, 0);
			glVertex3f(  i*bx+bx,	  0.0f, 0);
			glVertex3f(  i*bx+bx,   height, 0);
			glVertex3f(		i*bx,   height, 0);	
		glEnd();
	}	

	SwapBuffers(glWindow.getHDC());

	myEnable(MY_DEPTH_TEST);
}

extern "C" __declspec(dllexport) void dmsLoadPic(char *name)
{	
	Texture *texture = tManager->getTextureName(name);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(texture)
	{
		unsigned int ID = 0;
		int w = texture->getWidth();
		int h = texture->getHeight();
		int type = texture->getType();		
		
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
				
		if((w == 512 && h == 512) || (w == 256 && h == 256) || (w == 128 && h == 128) || (w == 64 && h == 64))
		{												
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			// tyyppi RGBA, eli oletetaan ett‰ kaikki textuurit ovat 32bit
			glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getImgData());		
		}
		else
		{
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			// tyyppi RGBA, eli oletetaan ett‰ kaikki textuurit ovat 32bit
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, texture->getImgData());		
		}

		texture->setID(ID);

		myEnable(MY_TEXTURE_2D);
		myDisable(MY_DEPTH_TEST);
		myDisable(MY_DEPTH_MASK);
		
		glLoadIdentity();

		glBindTexture(GL_TEXTURE_2D, texture->getID());

		glWindow.setPerspective2D(640, 480);

		glBegin(GL_QUADS);		
			glTexCoord2f(0, 1);	glVertex2f(  0,   0);
			glTexCoord2f(1, 1);	glVertex2f(640,   0);
			glTexCoord2f(1, 0);	glVertex2f(640, 480);
			glTexCoord2f(0, 0);	glVertex2f(  0, 480);
		glEnd();

		glWindow.setPerspective3D();

		myEnable(MY_DEPTH_TEST);
		myEnable(MY_DEPTH_MASK);
	}

	SwapBuffers(glWindow.getHDC());
}

extern "C" bool readSetup(char *name)
{	
	FILE *f;
	Parser parseri;	
	char buf[256] = {0};
	char dir[256] = {0};
	
	bool readMusic = false;
	bool readFonts = false;
	bool readTextures = false;	
	bool read3DObjects = false;

	f = fopen(name, "r");
	if(!f)
	{
		char buf[256] = {0};

		sprintf(buf, "readSetup(): Error opening file \"%s\"", name);
		throw buf;		
	}

	fseek(f, SEEK_SET, 0);

	while(parseri.readRow(f, buf))
	{		
		if(strstr(buf, "[Music]"))
		{
			readMusic = true;
			readFonts = false;
			readTextures = false;
			read3DObjects = false;
			
			memset(dir, 0, sizeof(dir));
		}

		if(strstr(buf, "[Font]"))
		{
			readMusic = false;
			readFonts = true;
			readTextures = false;
			read3DObjects = false;

			memset(dir, 0, sizeof(dir));
		}

		if(strstr(buf, "[Texture]"))
		{
			readMusic = false;
			readFonts = false;
			readTextures = true;
			read3DObjects = false;

			memset(dir, 0, sizeof(dir));
		}

		if(strstr(buf, "[Objects]"))
		{
			readMusic = false;
			readFonts = false;
			readTextures = false;
			read3DObjects = true;

			memset(dir, 0, sizeof(dir));
		}

		// Music block		
		if(readMusic)
		{				
			if(sound && !strstr(buf, "[Music]"))
			{			
/*
				PAKFile pak;

				if(!pak.read("data.pak")) return false;

				for(int i = 0; i < 20; i++)
				{
					FTEntry *e = pak.getFileTableEntry(i);
					if(e)
					{
						if(strstr(e->fileName, ".mp3"))
						{
							music.loadStream2(e->fileData, e->fileSize);
						}
					}
				}
*/				
				if(!sound->init()) return false;
				if(!music.loadStream(buf))
				{					
					return false;
				}
				
				readMusic = false;
			}			
		}

		if(readFonts && !strstr(buf, "[Font]"))
		{			
			char path[256] = {0};		// polku
			char name[256] = {0};		// nimiosa
			char filename[256] = {0};	// tiedostonimi
			char token1[256] = {0}, token2[256] = {0};
			char *ptr1, *ptr2;
				
			ptr1 = strtok(buf, "=");
			ptr2 = strtok(NULL, "=");
			sprintf(token1,"%s",ptr1);
			sprintf(token2,"%s",ptr2);
						
			if(strcmp(strlwr(token1),"dir") == 0)
			{			
				sprintf(dir, "%s", token2);
			}
			else
			{			
				sprintf(filename, "%s", token2);

				ptr1 = strtok(token2, ".");
				sprintf(name, "%s", token1);
			}
/*
			if(strlen(filename) > 4 && !strstr(filename, "(null)"))
			{
				sprintf(path, "%s\\%s", dir, filename);
				if(!fManager->addFont(32, 32, name, path)) return false;
			}
*/
		}

		// Texture block
		if(readTextures && !strstr(buf, "[Texture]"))
		{				
			char ext[3] = {0};			// tiedostop‰‰te			
			char path[256] = {0};		// polku
			char name[256] = {0};		// nimiosa
			char filename[256] = {0};	// tiedostonimi
			char token1[256] = {0}, token2[256] = {0};
			char *ptr1, *ptr2;
	
			// tarkistetaan kuvatiedoston formaatti
			ext[0] = buf[strlen(buf)-3];
			ext[1] = buf[strlen(buf)-2];
			ext[2] = buf[strlen(buf)-1];

			ptr1 = strtok(buf, "=");
			ptr2 = strtok(NULL, "=");
			sprintf(token1,"%s",ptr1);
			sprintf(token2,"%s",ptr2);
						
			if(strcmp(strlwr(token1),"dir") == 0)
			{			
				sprintf(dir, "%s", token2);
			}
			else
			{			
				sprintf(filename, "%s", token2);

				ptr1 = strtok(token2, ".");
				sprintf(name, "%s", token1);
			}

			sprintf(path, "%s\\%s", dir, filename);
			if(strstr(ext,"pcx") != NULL)
			{				
				if(!tManager->addTexture(path, name, PCX)) return false;
			}

			if(strstr(ext,"tga") != NULL)
			{
				if(!tManager->addTexture(path, name, TGA)) return false;
			}
		}

		// 3DObject block
		if(read3DObjects && !strstr(buf, "[Objects]"))
		{										
			char token1[256] = {0}, token2[256] = {0};
			char *ptr1, *ptr2;
				
			ptr1 = strtok(buf, "=");
			ptr2 = strtok(NULL, "=");
			sprintf(token1,"%s",ptr1);
			sprintf(token2,"%s",ptr2);

			if(ptr1 && ptr2)
			{					
				if(strcmp(strlwr(token1),"dir") == 0)
				{			
					sprintf(dir, "%s", token2);
				}
				else
				{					
					char path[256] = {0};
					char filename[256] = {0};
					char objectName[256] = {0};

					sprintf(filename, "%s", token2);		
					sprintf(objectName, "%s", token1);
					sprintf(path, "%s\\%s", dir, filename);

					if(!oManager->addObject(path, objectName)) return false;
				}
			}
		}
		
		memset(buf, 0, 256);
	}
	
	fclose(f);

	return true;
}

extern "C" bool readPak(char *name)
{
	int i;
	PAKFile pak;
	
	if(!pak.read(name)) return false;
		
	for(i = 0; i < pak.getFileCount(); i++)
	{				
		FTEntry *e = pak.getFileTableEntry(i);
		if(e)
		{						
			// Load PCX file as texture
			if(strstr(strlwr(e->fileName), ".pcx"))
			{						
			  if(!tManager->addTextureFromMemory(e->fileName, e->fileData, e->fileSize, e->fileName, PCX)) return false;
			}

			// Load TGA file as texture
			if(strstr(strlwr(e->fileName), ".tga"))
			{						
			  if(!tManager->addTextureFromMemory(e->fileName, e->fileData, e->fileSize, e->fileName, TGA)) return false;
			}

			// Load JPG file as texture
			if(strstr(strlwr(e->fileName), ".jpg"))
			{						
			  if(!tManager->addTextureFromMemory(e->fileName, e->fileData, e->fileSize, e->fileName, JPG)) return false;
			}

			// Load music
			if(strstr(strlwr(e->fileName), ".mp3"))
			{				
				if(sound)
				{
					if(!sound->init())
					{
						return false;
					}					
				}
				
				if(!music.loadStream2((char *)e->fileData, e->fileSize))
				{
					return false;
				}
			}

			// Load sync file
			if(strstr(strlwr(e->fileName), "syncs.txt"))
			{
				char *str = strtok((char *)e->fileData, "\n");
				
				while(str != NULL)
				{
					str = strtok(NULL, "\n");
					if(str != NULL)
					{					
						int *value = new int;
						*value = atoi(str);
						syncs.addTail(value);
					}
				} 
			}

			// Load T3D as 3D-objects
			if(strstr(strlwr(e->fileName), ".t3d"))
			{
				if(!oManager->addObjectFromMemory(e->fileName, e->fileData, e->fileSize)) return false;
			}
		}
	}

	return true;
}