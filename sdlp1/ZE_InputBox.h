#pragma once
#include <SDL2/SDL.h>
#include "ZE_TextureContainer.h"

using namespace std;

class InputBox : public TextureContainer
{
public:
	InputBox(int x, int y, int w, int h, SDL_Color fontColor = { 0, 0, 0 }, SDL_Color bgColor = { 255, 255, 255 });
	virtual void Render();

	SDL_Color fontColor;
	SDL_Color bgColor;

private:
	string mText;

	void changeText();
};