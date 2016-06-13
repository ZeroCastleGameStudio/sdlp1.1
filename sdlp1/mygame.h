#pragma once
#include "ZE_Core.h"
#include "ZE_AssetManager.h"

using namespace std;

class mygame : public Game
{
public:
	AssetManager resourses;

	mygame();
	void Init();
	virtual void MainLoop();
	virtual void Close();

	void eventtest(SDL_Event);
	void eventtest2(SDL_Event);
	void eventtest3(SDL_Event);
	void eventtest4(SDL_Event);
	void eventtest5(SDL_Event);
	void eventtest6(SDL_Event);
};