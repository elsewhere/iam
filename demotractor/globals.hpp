#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include "parser.hpp"
#include "opengl.hpp"
#include "log.hpp"
#include "timer.hpp"
#include "sound.hpp"
#include "texturemanager.hpp"
#include "effectmanager.hpp"
#include "fontmanager.hpp"
#include "objectmanager.hpp"

//--------------------------------------------------------------------------------------------
//  Function prototypes
//--------------------------------------------------------------------------------------------

extern "C" __declspec(dllexport) void dmsMsg(const char *text, ...);
extern "C" __declspec(dllexport) void dmsErrorMsg(const char *text, ...);
extern "C" __declspec(dllexport) void checkState();
extern "C" __declspec(dllexport) void myEnable(int c);
extern "C" __declspec(dllexport) void myDisable(int c);
extern "C" __declspec(dllexport) void myBlendFunc(int src, int dst);

#endif