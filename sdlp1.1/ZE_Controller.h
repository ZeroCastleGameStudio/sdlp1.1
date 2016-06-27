#pragma once
#include <memory>
#include <SDL.h>

using namespace std;
class ZeroEngine;
class Controller
{
public:
	SDL_Joystick* me = NULL;
	SDL_Haptic* shake = NULL;
	Controller(std::weak_ptr<ZeroEngine> core_engine, int id);
	~Controller();
	void rumble(float = 1, int = 150);
	void stopRumble();
	std::weak_ptr<ZeroEngine> core_engine;
};