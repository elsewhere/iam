//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>
#include "opengl.hpp"
#include "extensions.hpp"
#include "globals.hpp"

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  Class code
//--------------------------------------------------------------------------------------------

GLWindow::GLWindow()
{
	hwnd = NULL;
	hinstance = NULL;
	hdc = NULL;
	hrc = NULL;

	// Default setup
	width = 640;
	height = 480;
	bpp = 32;
	zbpp = 16;
	sbpp = 8;
	fullscreen = false;
	title = "DemoTractor v1.0";
	className = "DemoTractorGL";

	wglSwapIntervalEXT = NULL;
	wglGetSwapIntervalEXT = NULL;

	verticalSyncFlag = false;
	verticalSync = 0;
}

GLWindow::~GLWindow()
{
	kill();
}

bool GLWindow::createWindow()
{
	WNDCLASSEX wc;
	DWORD eStyle;
	DWORD style;
	RECT rect;	
	static PIXELFORMATDESCRIPTOR p;	
	char buf[256];
	int format;

	rect.left = (long)0;
	rect.right = (long)width;
	rect.top = (long)0;
	rect.bottom = (long)height;
	hinstance = GetModuleHandle(NULL);	

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC) WndProc;	
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wc.lpszClassName = className;

	if(!RegisterClassEx(&wc))
	{
		sprintf(buf,"GLWindow::createWindow(): %s luokan rekisteröinti epäonnistui!\n",className);
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}	
	
	ZeroMemory(&p, sizeof(p));
	p.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	p.nVersion = 1;
	p.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	p.iPixelType = PFD_TYPE_RGBA;
	p.cColorBits = bpp;
	p.cDepthBits = zbpp;
	p.iLayerType = PFD_MAIN_PLANE;
	p.cStencilBits = sbpp;

	if(fullscreen)
	{
		DEVMODE dmScreenSettings; // device mode

		ZeroMemory(&dmScreenSettings, sizeof(DEVMODE)); // nollataan 
		dmScreenSettings.dmSize = sizeof(DEVMODE);
		dmScreenSettings.dmPelsWidth = width;		// ruudun koko
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = bpp;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// kokeillaan josko wintoosa suostuisi vaihtamaan moodia:
		// CDS_FULLSCREEN: ei start bar:a 
		if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			fullscreen = false;			
		}		

		eStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;
		style = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;		
		ShowCursor(FALSE);

	}
	else
	{
		eStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}	

	AdjustWindowRectEx(&rect, style, FALSE, eStyle);
	hwnd = CreateWindowEx(eStyle, 
						  className,
						  title,
						  style,
						  0,
						  0,
						  rect.right - rect.left,
						  rect.bottom - rect.top,
						  HWND_DESKTOP,
						  NULL,
						  hinstance,
						  NULL);

	// Korjataan ruudun kokoa jos ollaan ikkunoidussa tilassa
	if(!fullscreen)
	{
		//height = rect.bottom - rect.top + 5;
	}

	if(!hwnd)
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): ei voinut luoda ikkunaa!\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	hdc = GetDC(hwnd);
	if(!hdc)
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): ei voinut luoda device contexia!\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	format = ChoosePixelFormat(hdc, &p);
	if(!format)
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): haettua resoluutiota ei tueta!\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	
	if(!SetPixelFormat(hdc, format, &p))
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): ei voinut asettaa haettua resoa!\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	
	if(!(hrc = wglCreateContext(hdc)))
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): ei voinut luoda GL-device contexia!\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(!wglMakeCurrent(hdc, hrc))
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): ei voinut aktivoida rendreing contexia\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;	
	}

	// Vertical Syncin poisto alkaa. Tapahtuu WGL-laajennuksen avulla
	
	// haetaan funkkarit
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
	wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress("wglGetSwapIntervalEXT");

	// verticalSyncFlag = false jos laajennusten haku ei onnistunut.
	// Ohjelma ei kuitenkaan kaadu tähän
	if (!wglSwapIntervalEXT || !wglGetSwapIntervalEXT) 
	{
		verticalSyncFlag = false;
	}
	else
	{
		// ...muulloin haetaan V-syncin arvo ennen kuin ajetaan ohjelma.
		verticalSync = wglGetSwapIntervalEXT();

		// Nyt voi ottaa V-Syncin turvallisesti pois päältä
		wglSwapIntervalEXT(0);
	}

	if(!loadExtensions(hdc)) return false;
		
	ShowWindow(hwnd, SW_SHOW);	
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	
	return true;
}

bool GLWindow::createWindow(int w, int h, int b, bool screen)
{
	WNDCLASSEX wc;
	DWORD eStyle;
	DWORD style;
	RECT rect;	
	static PIXELFORMATDESCRIPTOR p;	
	char buf[256];
	int format;

	width = w;
	height = h;
	bpp = b;
	fullscreen = screen;

	rect.left = (long)0;
	rect.right = (long)width;
	rect.top = (long)0;
	rect.bottom = (long)height;
	hinstance = GetModuleHandle(NULL);	

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC) WndProc;	
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wc.lpszClassName = className;

	if(!RegisterClassEx(&wc))
	{
		sprintf(buf,"GLWindow::createWindow(): %s luokan rekisteröinti epäonnistui!\n",className);
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}	
	
	ZeroMemory(&p, sizeof(p));
	p.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	p.nVersion = 1;
	p.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	p.iPixelType = PFD_TYPE_RGBA;
	p.cColorBits = bpp;
	p.cDepthBits = zbpp;
	p.iLayerType = PFD_MAIN_PLANE;
	p.cStencilBits = sbpp;

	if(fullscreen)
	{
		DEVMODE dmScreenSettings; // device mode

		ZeroMemory(&dmScreenSettings, sizeof(DEVMODE)); // nollataan 
		dmScreenSettings.dmSize = sizeof(DEVMODE);
		dmScreenSettings.dmPelsWidth = width;		// ruudun koko
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = bpp;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// kokeillaan josko wintoosa suostuisi vaihtamaan moodia:
		// CDS_FULLSCREEN: ei start bar:a 
		if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			fullscreen = false;			
		}		

		eStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;
		style = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;		
		ShowCursor(FALSE);

	}
	else
	{
		eStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}	

	AdjustWindowRectEx(&rect, style, FALSE, eStyle);
	hwnd = CreateWindowEx(eStyle, 
						  className,
						  title,
						  style,
						  0,
						  0,
						  rect.right - rect.left,
						  rect.bottom - rect.top,
						  HWND_DESKTOP,
						  NULL,
						  hinstance,
						  NULL);

	// Korjataan ruudun kokoa jos ollaan ikkunoidussa tilassa
	if(!fullscreen)
	{
		//height = rect.bottom - rect.top + 5;
	}

	if(!hwnd)
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): ei voinut luoda ikkunaa!\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	hdc = GetDC(hwnd);
	if(!hdc)
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): ei voinut luoda device contexia!\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	format = ChoosePixelFormat(hdc, &p);
	if(!format)
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): haettua resoluutiota ei tueta!\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(!SetPixelFormat(hdc, format, &p))
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): ei voinut asettaa haettua resoa!\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	
	if(!(hrc = wglCreateContext(hdc)))
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): ei voinut luoda GL-device contexia!\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(!wglMakeCurrent(hdc, hrc))
	{
		kill();

		sprintf(buf,"GLWindow::createWindow(): ei voinut aktivoida rendreing contexia\n");
		MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
		return false;	
	}

	// Vertical Syncin poisto alkaa. Tapahtuu WGL-extensionin avulla
	// Muuttujat jotka osoittavat laajennus funkkareihin
	PFNWGLSWAPINTERVALEXTPROC	wglSwapIntervalEXT = 0;
	PFNWGLGETSWAPINTERVALEXTPROC	wglGetSwapIntervalEXT = 0;

	// haetaan funkkarit
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
	wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress("wglGetSwapIntervalEXT");

	// verticalSyncFlag = false jos laajennusten haku ei onnistunut.
	// Ohjelma ei kuitenkaan kaadu tähän
	if (!wglSwapIntervalEXT || !wglGetSwapIntervalEXT) 
	{
		verticalSyncFlag = false;;
	}
	else
	{
		// ...muulloin haetaan V-syncin arvo ennen kuin ajetaan ohjelma.
		verticalSync = wglGetSwapIntervalEXT();

		// Nyt voi ottaa V-Syncin turvallisesti pois päältä
		wglSwapIntervalEXT(0);
	}

	if(!loadExtensions(hdc)) return false;

	ShowWindow(hwnd, SW_SHOW);	
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	
	return true;
}

bool GLWindow::kill()
{
	char buf[256];

	if(hwnd)
	{
		// jos verticalSync:ä on näpelöity ...
		if(verticalSyncFlag)
		{
			// ... palautetaan se alkuperäiseksi
			wglSwapIntervalEXT(verticalSync);
		}

		if(hdc)
		{
				wglMakeCurrent(hdc, 0);
				if(hrc)
				{
					wglDeleteContext(hrc);
					hrc = NULL;
				}

				ReleaseDC(hwnd, hdc);
				hdc = NULL;
		}
		DestroyWindow(hwnd);
		hwnd = NULL;

		if(!UnregisterClass(className, hinstance))
		{	
			sprintf(buf,"GLWindow::kill(): ei voinut vapauttaa luokkaa %s\n", className);
			MessageBox(NULL, buf,"Error", MB_OK | MB_ICONEXCLAMATION);
			return false;	
		}
	}	

	ChangeDisplaySettings(NULL, 0);
	ShowCursor(true);

	return true;
}

bool GLWindow::getFullscreen()
{
	return fullscreen;
}

//	Getterit
unsigned int GLWindow::getWidth()
{
	return width;
}

unsigned int GLWindow::getHeight()
{
	return height;
}

HWND GLWindow::getHandle()
{
	return hwnd;
}

HINSTANCE GLWindow::getInstance()
{
	return hinstance;
}

HDC GLWindow::getHDC()
{
	return hdc;
}

// Setterit
void GLWindow::setWidth(unsigned int w)
{
	width = w;
}

void GLWindow::setHeight(unsigned int h)
{
	height = h;
}

void GLWindow::setBpp(unsigned int b)
{
	bpp = b;
}

void GLWindow::setZbpp(unsigned int z)
{
	zbpp = z;
}

void GLWindow::setSbpp(unsigned int s)
{
	sbpp = s;
}

void GLWindow::setFullscreen(bool f)
{
	fullscreen = f;
}

void GLWindow::setTitle(char *t)
{
	title = t;
}

void GLWindow::setClassName(char *c)
{
	className = c;
}

void GLWindow::setPerspective2D(void)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//glOrtho(640,0,480,0,-1,1);
	glOrtho(0,640, 0,480,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); 
	glLoadIdentity();
}

void GLWindow::setPerspective2D(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//glOrtho(w,0,h,0,-1,1);
	glOrtho(0,w,0,h,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); 
	glLoadIdentity();
}

void GLWindow::setPerspective3D(void)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

GLSystem::GLSystem()
{	
	width = 640;
	height = 480;	
}

GLSystem::~GLSystem()
{

}

bool GLSystem::init(int w, int h)
{
	resize(w, h);

	width = w;
	height = h;

	if(height == 0) height = 1;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width/(float)height, 0.5f, 14000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);	
		
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
	
	glDepthFunc(GL_LEQUAL);	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);		
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
				
	myDisable(MY_NORMALIZE);
	myDisable(GL_TEXTURE_GEN_S);
	myDisable(GL_TEXTURE_GEN_T);				
	myEnable(MY_TEXTURE_2D);	

	return true;
}

void GLSystem::resize(int w, int h)
{
	width = w;
	height = h;

	if(height == 0) height = 1;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width/(float)height, 0.5f, 14000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLSystem::setClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void GLSystem::setFov(float fov)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (float)width/(float)height, 0.5f, 14000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLSystem::loadOMeter(int current, int max)
{
	float step = float(current - max) / float(max);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myDisable(MY_TEXTURE_2D);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	

	for(int i = 0; i < current; i++)
	{	
		glLoadIdentity();
		glTranslatef(0, 0, -50);

		glBegin(GL_QUADS);
			glVertex3f(-1.0f+i, -1.0f, 0);
			glVertex3f( 1.0f+i, -1.0f, 0);
			glVertex3f( 1.0f+i,  1.0f, 0);
			glVertex3f(-1.0f+i,  1.0f, 0);	
		glEnd();
	}	
}

//--------------------------------------------------------------------------------------------
//  Includes global functions to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

namespace TRACTION_DEMOTRACTOR
{
	LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg,	WPARAM wParam, LPARAM lParam)
	{			
		switch(uMsg)
		{	
			case WM_SYSCOMMAND:
			{
				switch (wParam)
				{
					case SC_SCREENSAVE:
					case SC_MONITORPOWER:
					return 0;
				}		
				break;									
			} break;

				case WM_CLOSE:
				{
					PostQuitMessage(0);
					return 0;
				} break;

				case WM_DESTROY:
				{
					PostQuitMessage(0);
					return 0;
				} break;

				case WM_SIZE:
				{
					return 0;
				} break;
			}	

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}