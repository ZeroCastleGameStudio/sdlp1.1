#include <sstream>
#include <string>
#include "ZE_Core.h"
#include "ZE_Fraps.h"

using namespace std;

void Fraps::setUp()
{
	mText = new TextField("1", 0, 12, { 255, 255, 255 });
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