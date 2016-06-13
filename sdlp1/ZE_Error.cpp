#include <Windows.h>
#include <SDL2/SDL.h>
#include <SDL_Extension/SDL_image.h>
#include <SDL_Extension/SDL_ttf.h>
#include <SDL_Extension/SDL_mixer.h>
#include <iostream>
#include <string>
#include "ZE_Error.h"

using namespace std;

void Error::PopToDebutConsole(string msg, bool mainConsole)
{
	OutputDebugStringA(msg.c_str());
	if (mainConsole)
		cout << msg << endl;
}

void Error::PopDebutConsole_Warning(string msg)
{
	OutputDebugStringA(("Warning: " + msg + "\n").c_str());
}

void Error::PopDebugConsole_Error(string msg, bool quit)
{
	OutputDebugStringA("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	OutputDebugStringA((msg + "\n").c_str());
	OutputDebugStringA("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	if (quit)
	{
		exit(1);
	}
}

void Error::PopDebugConsole_SDLError(string msg, bool quit)
{
	//当SDL出错时，可以随时使用SDL_GetError()方法获得错误信息
	OutputDebugStringA("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	OutputDebugStringA((msg + "[SDL Error: " + SDL_GetError() + "]\n").c_str());
	OutputDebugStringA("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	if (quit)
	{
		exit(1);
	}
}

void Error::PopDebugConsole_SDL_ImageError(string msg, bool quit)
{
	//当SDL_Image出错时，可以随时使用IMG_GetError()方法获得错误信息
	OutputDebugStringA("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	OutputDebugStringA((msg + "[SDL_Image Error: " + IMG_GetError() + "]\n").c_str());
	OutputDebugStringA("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	if (quit)
	{
		exit(1);
	}
}

void Error::PopDebugConsole_SDL_ttfError(string msg, bool quit)
{
	//当SDL_ttf出错时，可以随时使用TTF_GetError()方法获得错误信息
	OutputDebugStringA("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	OutputDebugStringA((msg + "[SDL_ttf Error: " + TTF_GetError() + "]\n").c_str());
	OutputDebugStringA("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	if (quit)
	{
		exit(1);
	}
}

void Error::PopDebugConsole_SDL_MixerError(string msg, bool quit)
{
	//当SDL_Mixer出错时，可以随时使用Mix_GetError()方法获得错误信息
	OutputDebugStringA("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	OutputDebugStringA((msg + "[SDL_Mixer Error: " + Mix_GetError() + "]\n").c_str());
	OutputDebugStringA("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	if (quit)
	{
		exit(1);
	}
}