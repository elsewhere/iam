#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

//-------------------------------------------------------
//	Headerit
//-------------------------------------------------------

#include <windows.h>

//-------------------------------------------------------
// Config-luokka
//
// Hoitaa demon conffaamisen.
//-------------------------------------------------------

class Config
{		
	public:
		
		Config();
		~Config();

		bool run(HINSTANCE hinstance);
		
		// getterit
		int getScreenX();
		int getScreenY();
		int getBpp();		
		int getGamma();
		bool getFullscreen();
		bool getSound();
		bool getVsync();
		bool getRunFlag();
		bool getLogFile();

		static CALLBACK ConfigProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		static int screenX, screenY, bpp, gamma;
		static bool fullscreen, sound, vsync, runFlag, logFile;

	private: 

		HWND handle;
		static HINSTANCE instance;
};

#endif