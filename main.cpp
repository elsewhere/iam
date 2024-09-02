//-------------------------------------------------------
//	Defines
//-------------------------------------------------------

// This extracts MFC stuff off
#define WIN32_LEAN_AND_MEAN
#define VC_LEANMEAN           

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include <windows.h>
#include <stdio.h>

#ifdef _DEBUG
	#include <stdlib.h>
	#include "mmgr.h"
#endif

#include "externs.hpp"
#include "config.hpp"

// Effects
#include "effects/lierot.hpp"
#include "effects/refface.hpp"
#include "effects/ripple.hpp"
#include "effects/ripple2.hpp"
#include "effects/3dripple.hpp"
#include "effects/databall.hpp"
#include "effects/noise.hpp"
#include "effects/metapilar.hpp"
#include "effects/efekti1.hpp"
#include "effects/efekti2.hpp"
#include "effects/efekti3.hpp"
#include "effects/fadein.hpp"
#include "effects/strings.hpp"
#include "effects/slide.hpp"
#include "effects/dotfade.hpp"
#include "effects/tilpe1.hpp"
#include "effects/tilpe2.hpp"
#include "effects/lehtipuuefekti.hpp"

#include "effects/bubble.hpp"
#include "effects/crystal.hpp"
#include "effects/digital.hpp"
#include "effects/flubber.hpp"
#include "effects/kasviefekti.hpp"
#include "effects/kasvu.hpp"
#include "effects/keiju.hpp"
#include "effects/layers.hpp"
#include "effects/mustikka.hpp"
#include "effects/potential.hpp"
#include "effects/ruoho.hpp"
#include "effects/tehdas.hpp"
#include "effects/water.hpp"
#include "effects/worm.hpp"
#include "effects/build.hpp"
#include "effects/viimeinen.hpp"
#include "effects/kusetus.hpp"
#include "effects/start.hpp"

using namespace TRACTION_DEMOTRACTOR;

void write(float xpos, float ypos, float pos, float starttime, float endtime, float alpha, char *fontti, char *teksti)
{
	const float width = (float)dmsGetWindowWidth();
	const float height = (float)dmsGetWindowHeight();

	const float newxpos = xpos * (width / 1024.0f);
	const float newypos = ypos * (height / 768.0f);
	const float newscale = (width / 1024.0f)*1.0f; 

	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE); 
	if (pos > starttime && pos < endtime)
	{
		const float textpos = (pos-starttime) / (endtime-starttime);
		float textalpha = (float)sin(textpos*3.141592f)*4.0f*alpha;
		if (textalpha > 1.0f) textalpha = 1.0f;
		dmsWrite2D(newxpos, newypos, textalpha, newscale, fontti, teksti); 
	}
}

void write2(float xpos, float ypos, float scale, float pos, float starttime, float endtime, float alpha, char *fontti, char *teksti)
{
	const float width = (float)dmsGetWindowWidth();
	const float height = (float)dmsGetWindowHeight();

	const float newxpos = xpos * (width / 1024.0f);
	const float newypos = ypos * (height / 768.0f);
	const float newscale = (width / 1024.0f)*1.0f; 

	myBlendFunc(GL_SRC_ALPHA, GL_ONE); 
	if (pos > starttime && pos < endtime)
	{
		const float textpos = (pos-starttime) / (endtime-starttime);
		float textalpha = (float)sin(textpos*3.141592f)*4.0f*alpha;
		if (textalpha > 1.0f) textalpha = 1.0f;
		dmsWrite2D(newxpos, newypos, textalpha, newscale*scale, fontti, teksti); 
	}
}

void write3(float xpos, float ypos, float scale, float pos, float starttime, float endtime, float alpha, char *fontti, char *teksti)
{
	const float width = (float)dmsGetWindowWidth();
	const float height = (float)dmsGetWindowHeight();

	const float newxpos = xpos * (width / 1024.0f);
	const float newypos = ypos * (height / 768.0f);
	const float newscale = (width / 1024.0f)*1.0f; 

	myBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR); 
	if (pos > starttime && pos < endtime)
	{
		const float textpos = (pos-starttime) / (endtime-starttime);
		float textalpha = (float)sin(textpos*3.141592f)*4.0f*alpha;
		if (textalpha > 1.0f) textalpha = 1.0f;
		dmsWrite2D(newxpos, newypos, textalpha, newscale*scale, fontti, teksti); 
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{	
	// Stores messages that windows sends to the application
	MSG msg = {0};			
	HDC hdc = NULL;
	bool done = false;	

	Lierot *lierot = NULL;
	RefFace *refface = NULL;
	Ripple *ripple = NULL;
	Ripple2 *ripple2 = NULL;
	Ripple3D *ripple3D = NULL;
	DataBall *dataBall = NULL;
	Noise *noise = NULL;
	MetaPilar *metaPilar = NULL;
	Efekti1 *efekti1 = NULL;
	Efekti2 *efekti2 = NULL;
	Efekti3 *efekti3 = NULL;
	FadeIn *fadeIn = NULL;
	Strings *strings = NULL;
    Slide *slide = NULL;
	DotFade *dotFade = NULL;
	Tilpe1 *tilpe1 = NULL;
	Tilpe2 *tilpe2 = NULL;
	
	// Tee PAK-tiedosto käsin
	//dmsMakePAK("sourcedata", "data.pak");

	try
	{
		Config cfg;

		cfg.run(hInstance);
		
		if(cfg.getRunFlag())
		{
			dmsSetLogging(cfg.getLogFile());
			dmsInitEx(cfg.getScreenX(), cfg.getScreenY(), cfg.getBpp(), cfg.getFullscreen(), cfg.getSound(), cfg.getVsync(), "data.pak");

			// Lisätään pari globaalia renderToTextures
//			dmsAddRenderToTexture(256, 256, "target1");			
//			dmsAddRenderToTexture(256, 256, "target2");		
//			dmsAddRenderToTexture(256, 256, "dotfade");
									
//			dmsAddTexture("testi.jpg", "testi");			
//			dmsAddTexture("varjokuva.jpg", "varjokuva");			
//			dmsAddTexture("teknofontti.pcx", "teknofontti.pcx");			
//			dmsAddTexture("luontofontti.pcx", "luontofontti.pcx");			
//			dmsAddTexture("font32.pcx", "font32.pcx");
			

			dmsAddFont(32, 32, "teknofontti.pcx");
			dmsAddFont(32, 32, "luontofontti.pcx");
			dmsAddFont(32, 32, "font32.pcx");

			//porttauksen aikana lisätyt
//			dmsAddObject("flake.t3d", "flake");
//			dmsAddObject("baby.t3d", "baby");		
//			dmsAddObject("flake_bubble.t3d", "flake_bubble.t3d")			;
//			dmsAddObject("hedra.t3d", "hedra");			
//			dmsAddTexture("inverteye.jpg", "inverteye.jpg");			
//			dmsAddTexture("kromi.pcx", "kromi.pcx"); 			
//			dmsAddTexture("huuto.jpg", "huuto.jpg");			
//			dmsAddTexture("nousu.jpg", "nousu.jpg");			
//			dmsAddTexture("gootti.pcx", "gootti.pcx");			
//			dmsAddTexture("blast.jpg", "blast.jpg");			
//			dmsAddTexture("kasvis.jpg", "kasvis.jpg");			
//			dmsAddTexture("particle1.pcx", "particle1.pcx");			
//			dmsAddTexture("particle2.pcx", "particle2.pcx");			
//			dmsAddTexture("particle3.pcx", "particle3.pcx");
//			dmsAddTexture("saastetaivas.jpg", "saastetaivas.jpg");
			
//			dmsAddEffect(0, 25000, "keiju", new Keiju());
//			dmsAddEffect(24000, 50000, "viimeinen", new Viimeinen());

//			dmsAddEffect(0, 25000, "hehe", new Bubble());
//			dmsAddEffect(0, 25000, "databall", new DataBall());
//			dmsAddEffect(0, 25000, "crystal", new Crystal());
//			dmsAddEffect(0, 25000, "digital", new Digital());
//			dmsAddEffect(0, 25000, "flubber", new Flubber());
//			dmsAddEffect(0, 25000, "kasviefekti", new Kasviefekti ());
//			dmsAddEffect(0, 25000, "kasvu", new Kasvu());
//			dmsAddEffect(0, 25000, "keiju", new Keiju());
//			dmsAddEffect(0, 25000, "layers", new Layers());
//			dmsAddEffect(0, 25000, "lehtipuuefekti", new Lehtipuuefekti());
//			dmsAddEffect(0, 25000, "mustikka", new Mustikka());
//			dmsAddEffect(0, 25000, "potential", new Potential());
//			dmsAddEffect(19000, 25000, "****SIIRTYMÄ1****", new FadeIn());
//			dmsAddEffect(25000, 50000, "kasvu", new Kasvu());
//			dmsAddEffect(0, 25000, "refface", new RefFace());
//			dmsAddEffect(0, 25000, "ruoho", new Ruoho());
//			dmsAddEffect(0, 25000, "tehdas", new Tehdas());
//			dmsAddEffect(0, 25000, "water", new Water());
//			dmsAddEffect(0, 25000, "worm", new Worm());
//			dmsAddEffect(0, 25000, "kasvu", new Kasvu());
			//luonnot ja tekniikat tässä sekaisin
//			dmsAddEffect(0, 25000, "meta", new MetaPilar());
//			dmsAddEffect(0, 25000, "kusetus", new Kusetus());
//			dmsAddEffect(23000, 45000, "worm", new Worm());

//			dmsAddEffect(000, 6000, "potential", new Potential); //luonto
//			dmsAddEffect(000, 6000, "****SIIRTYMÄ1****", new Build());
//			dmsAddEffect(6000, 31000, "kasvu", new Kasvu()); //tekniikka
//			dmsAddEffect(0, 24000, "digital", new Digital()); //tekniikka
//			dmsAddEffect(8000, 20000, "fadein", new FadeIn());
//			dmsAddEffect(20000, 40000, "lehtipuu", new Lehtipuuefekti()); //luonto
//			dmsAddEffect(000, 25000, "digital", new Digital()); //tekniikka
//			dmsAddEffect(19000, 25000, "****SIIRTYMÄ1****", new FadeIn());
//			dmsAddEffect(25000, 50000, "lehtipuu", new Lehtipuuefekti()); //luonto
//			dmsAddEffect(000, 5000, "****SIIRTYMÄ1****", new FadeIn());

//			dmsAddEffect(0, 25000, "worm", new Worm());  //luonto
//			dmsAddEffect(00, 25000, "keiju", new Keiju()); //luonto 
//			dmsAddEffect(23000, 50000, "loppu", new Viimeinen()); //luonto

//			dmsAddEffect(0, 25000, "datapallo", new DataBall()); //tekniikka
//			dmsAddEffect(0, 25000, "kristalli", new Crystal());  //tekniikka

//			dmsAddEffect(0, 24000, "bubbles", new Bubble()); //luonto

			dmsAddEffect(0, 10000, "startti", new Start()); //tekniikka + luonto
			dmsAddEffect(10000, 39000, "kasvot", new RefFace()); //tekniikka + luonto
			dmsAddEffect(39000, 64000, "mustikka", new Mustikka()); //tekniikka + luonto
			dmsAddEffect(64000, 89000, "datapallo", new DataBall()); //tekniikka
			dmsAddEffect(64000, 89000, "kristalli", new Crystal());  //tekniikka
			dmsAddEffect(89000, 116000, "ruoho", new Ruoho()); //luonto
			dmsAddEffect(114000, 143000, "tehdas", new Tehdas()); //tekniikka
			dmsAddEffect(142000, 167000, "kasviefekti", new Kasviefekti()); //luonto
			dmsAddEffect(167000, 191000, "digital", new Digital()); //tekniikka
			
			dmsAddEffect(190000, 193000, "****SIIRTYMÄ1****", new FadeIn());
			dmsAddEffect(193000, 219000, "lehtipuu", new Lehtipuuefekti()); //luonto
			dmsAddEffect(219000, 244000, "flubber", new Flubber()); //tekniikka
			dmsAddEffect(242000, 270000, "potential", new Potential); //luonto
			dmsAddEffect(264000, 270000, "****SIIRTYMÄ2****", new Build());
			dmsAddEffect(270000, 296000, "kasvu", new Kasvu()); //tekniikka
			dmsAddEffect(296000, 320000, "bubbles", new Bubble()); //luonto
			dmsAddEffect(320000, 346000, "water", new Water());  //tekniikka
			dmsAddEffect(346000, 376000, "meta", new MetaPilar());  //luonto
			dmsAddEffect(346000, 376000, "kusiainen", new Kusetus());  //luonto
			dmsAddEffect(372000, 405000, "worm", new Worm());  //luonto
			dmsAddEffect(404000, 430000, "keiju", new Keiju()); //luonto 
			dmsAddEffect(428500, 456000, "loppu", new Viimeinen()); //luonto 

//			dmsAddEffect(0, 25000, "tehdas", new Tehdas());
//			dmsAddEffect(0, 25000, "lehtipuu", new Lehtipuuefekti()); //luonto
			
			//dmsAddFont(32, 32, "font32.pcx");

			// Efektit demoon - jotkut käytettäväksi tilpenä (kuten noise)
			//lierot = new Lierot;
			//refface = new RefFace;

			// HUOM! RenderToTexturet pitää olla listan alussa - ennen
			// normaaleja efektejä

			// RenderToTexture Efekti 1
			//efekti1 = new Efekti1;

			// RenderToTexture Efekti 2
			//efekti2 = new Efekti2;

			// RenderToTexture Efekti 3
			//efekti3 = new Efekti3;

			// Siirtymä - vaatii efekti1 ja efekti2
			//ripple = new Ripple;
			//ripple2 = new Ripple2;

			// Siirtymä - tausta kuvalla tms. Tai ehkä hyvän efektin
			// kanssa texturessa
			//fadeIn = new FadeIn;

			// Siirtymä - vaatii efekti1 ja efekti2
			//slide = new Slide;

			// Siirtymä - tai ehkä enemmän tilpe; vaatii Efekti3
			//dotFade = new DotFade;

			// Tilpe?
			//strings = new Strings;

//			tilpe1 = new Tilpe1;
			//tilpe2 = new Tilpe2;

			//ripple3D = new Ripple3D;
//			dataBall = new DataBall;
			//noise = new Noise;
			//metaPilar = new MetaPilar;
			
			//dmsAddEffect(0, 20000, "lierot", lierot);			
			//dmsAddEffect(0, 20000, "refface", refface);			

			// Render-to-Texture efektit
			//dmsAddEffect(0, 20000, "efekti1", efekti1);
			//dmsAddEffect(0, 20000, "efekti2", efekti2);
			//dmsAddEffect(0, 20000, "efekti3", efekti3);

			// Siirtymä			
			//dmsAddEffect(0, 5000, "ripple", ripple);
			//dmsAddEffect(2500, 7500, "ripple2", ripple2);
			
			// Siirtymä
			//dmsAddEffect(0, 20000, "fadein", fadeIn);

			// Siirtymä
			//dmsAddEffect(0, 1500, "Zoom3", slide);			
			
			// Tilpe efekti
			//dmsAddEffect(0, 20000, "nauhat", strings);

			//dmsAddEffect(0, 40000, "ripple3D", ripple3D);
//			dmsAddEffect(0, 20000, "datapallo", new dataBall);
			//dmsAddEffect(0, 20000, "noise", noise);			
			//dmsAddEffect(0, 200000, "metapilari", metaPilar);			

			// Siirtymä / Tilpe
			//dmsAddEffect(0, 20000, "Zilog :)", dotFade);			
//			dmsAddEffect(0, 20000, "viiva tilpe", tilpe1);
			//dmsAddEffect(0, 20000, "pallo tilpe", tilpe2);

//			dmsAddEffect(0, 25000, "lehtipuuefekti", new Lehtipuuefekti());
		
			dmsInitEffects();
			
			// Uploadi tässä, jos efuissa on addeja mukana
			dmsUploadTextures();
			
			dmsInitTimer();
			
			dmsPlaySong(true);
			
		}
		else
		{
			done = true;
		}		
		
		// Demo loop
		while(!done)
		{		
			// Check for Windows messages in queue
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				//If app was closed send a note to the demo to stop the Demo loop
				if(msg.message == WM_QUIT)
				{
					done = true;
				}
				else
				{
					// Other messages are handled by Windows own handlers
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			// No messages left, we can do some own code now which means
			// actually running the demo :)
			else
			{	
				dmsUpdateTimer();
				dmsCheckControls(1000);				
											 
				if(GetAsyncKeyState(VK_ESCAPE)) done = true;

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
								
				dmsRun();

				// Näin voi piirtää jotain ...
				//glBindTexture(blaablaa);				
				//dmsDrawObject("baby.t3d");

				glLoadIdentity();
				myBlendFunc(GL_SRC_COLOR, GL_ONE);
//				dmsWrite2D(16, 16, 1, "font32.pcx", "hello world");
//				dmsWrite2D(16, 16, 1, "teknofontti.pcx", "abcdefghijklmnopqrstuvwxyz");
//				dmsWrite2D(16, 50, 1, "teknofontti.pcx", "123456789!\"#¤%&/()");
//				dmsWrite2D(16, 86, 1, "teknofontti.pcx", "I smell the winter coming");
//	dmsWrite2D(40, 40, 1, "teknofontti.pcx", "hello world");

				// Update the demo window's screen			
				hdc = dmsGetHDC();
				SwapBuffers(hdc);					

				char buf[256] = {0};

				sprintf(buf, "Time: %fs", dmsGetTime() * 0.001f);
				dmsSetWindowTitle(buf);
				if (dmsGetTime() >= 456000)
					done = true;
			}
		}	
	}
	catch(const char *msg)
	{
		dmsMsg("Exception: %s\n", msg);
	}
		
	dmsSetGamma(0);
	dmsShutDown();

	// This application is done and we let the Windows know that
	return msg.wParam;
}

