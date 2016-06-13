#include "ZE_Timer.h"

using namespace std;

void Timer::start()
{
	mStartTicks = SDL_GetTicks();
}

void Timer::reset()
{
	mStartTicks = 0;
}

Uint32 Timer::getTicks()
{
	return SDL_GetTicks() - mStartTicks;
}