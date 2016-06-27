#pragma once
#include <SDL.h>

using namespace std;

class Timer
{
public:
	void start();
	void reset();

	Uint32 getTicks() const;

private:
	Uint32 mStartTicks;
};