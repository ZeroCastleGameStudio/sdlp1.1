#include "ZE_Core.h"
#include "ZE_Controller.h"
#include "ZE_Error.h"

using namespace std;

Controller::Controller(std::weak_ptr<ZeroEngine> core_engine, int id)
	:core_engine(core_engine)
{
	me = SDL_JoystickOpen(id);
	if (!me)
	{
		core_engine.lock()->ZE_error->PopDebugConsole_SDLError("Unable to open game controller!");
	}
	else
	{
		shake = SDL_HapticOpenFromJoystick(me);
		if (!shake)
		{
			core_engine.lock()->ZE_error->PopDebugConsole_Warning("Warning: Controller does not support haptics!");
		}
		else
		{
			if (SDL_HapticRumbleInit(shake) < 0)
			{
				core_engine.lock()->ZE_error->PopDebugConsole_Warning("Warning: Unable to initialize rumble!");
			}
		}
	}
}

void Controller::rumble(float power, int time)
{
	if (shake)
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