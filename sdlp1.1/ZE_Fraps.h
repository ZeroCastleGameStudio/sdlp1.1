#pragma once
#include <SDL.h>
#include "ZE_Sprite.h"

using namespace std;

class TextField;
class Fraps : public Sprite
{
public:

	using Sprite::Sprite;
	void setUp();
	void setText();

private:
	shared_ptr<TextField> mText;
	Uint32 lastCheck{ 0 };
	int callCount{ 0 };
};