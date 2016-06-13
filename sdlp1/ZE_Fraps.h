#pragma once
#include <SDL2/SDL.h>
#include "ZE_Sprite.h"
#include "ZE_TextField.h"

using namespace std;

class Fraps : public Sprite
{
public:
	void setUp();
	void setText();

private:
	TextField* mText = NULL;
	Uint32 lastCheck;
	int callCount;
};