#include "ZE_Core.h"
#include "ZE_Controller.h"

using namespace std;

Controller::Controller(int id)
{
	me = SDL_JoystickOpen(id);
	if (me == NULL)
	{
		ZE_error.PopDebugConsole_SDLError("Unable to open game controller!");
	}
	else
	{
		shake = SDL_HapticOpenFromJoystick(me);
		if (shake == NULL)
		{
			ZE_error.PopDebutConsole_Warning("Warning: Controller does not support haptics!");
		}
		else
		{
			if( SDL_HapticRumbleInit( shake ) < 0 )
			{
				ZE_error.PopDebutConsole_Warning("Warning: Unable to initialize rumble!");
			}
		}
	}
}

void Controller::rumble(float power, int time)
{
	if (shake != NULL)
	{
		SDL_HapticRumblePlay(shake, power, time);
	}
}

void Controller::stopRumble()
{
	SDL_HapticRumbleStop(shake);
}

Controller::~Controller()
{
	SDL_HapticClose(shake);
	SDL_JoystickClose(me);
}