#include <sstream>
#include <string>
#include "ZE_Core.h"
#include "ZE_Fraps.h"
#include "ZE_TextField.h"

using namespace std;

void Fraps::setUp()
{
	mText = make_shared<TextField>("0", 0, 12, SDL_Color{ 255, 255, 255 });
	this->addChild(mText);
}

void Fraps::setText()
{
	if ((SDL_GetTicks() - lastCheck) >= 1000)
	{
		stringstream tempSS;
		string tempSTR;
		tempSS << callCount;
		tempSS >> tempSTR;
		mText->changeText("fps:" + tempSTR);
		lastCheck = SDL_GetTicks();
		callCount = 0;
	}
	else
	{
		callCount += 1;
	}
}