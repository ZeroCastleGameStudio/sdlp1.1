#pragma once
#include <string>

using namespace std;

//错误处理类
//目前基本只是在输出字符串而已，以后可能会弹出窗口（大概
class Error
{
public:
	//单纯的向DEBUG控制台输出一条信息，第二个参数表示是否同时向程序控制台输出
	void PopToDebutConsole(string msg, bool mainConsole = true);
	//输出一条警告信息，不退出
	void PopDebutConsole_Warning(string msg);
	//弹出一个一般错误，接收一个错误信息字符串
	void PopDebugConsole_Error(string msg, bool quit = true);
	//弹出一个SDL错误，接收一个错误信息字符串，并附加SDL错误信息
	void PopDebugConsole_SDLError(string msg, bool quit = true);
	//弹出一个SDL_Image错误，接收一个错误信息字符串，并附加SDL_Image错误信息
	void PopDebugConsole_SDL_ImageError(string msg, bool quit = true);
	//弹出一个SDL_ttf错误，接收一个错误信息字符串，并附加SDL_ttf错误信息
	void PopDebugConsole_SDL_ttfError(string msg, bool quit = true);
	//弹出一个SDL_Mixer错误，接收一个错误信息字符串，并附加SDL_Mixer错误信息
	void PopDebugConsole_SDL_MixerError(string msg, bool quit = true);
};
