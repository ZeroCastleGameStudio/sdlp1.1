#pragma once
#include <SDL2/SDL.h>

using namespace std;

class Controller
{
public:
	SDL_Joystick* me = NULL;
	SDL_Haptic* shake = NULL;
	Controller(int);
	~Controller();
	void rumble(float = 1, int = 150);
	void stopRumble();
};