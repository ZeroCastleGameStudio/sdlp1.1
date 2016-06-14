#include "ZE_Core.h"
#include "ZE_InputBox.h"

using namespace std;

InputBox::InputBox(int x, int y, int w, int h, SDL_Color fontColor, SDL_Color bgColor)
{
	this->x = x;
	this->y = y;
	this->mWidth = w;
	this->mHeight = h;
	this->fontColor = fontColor;
	this->bgColor = bgColor;
}

void InputBox::changeText()
{
	freeTexture();
	mTexture = new Texture();
	SDL_Texture* temptex;

	TTF_Font* tempfont = defaultFont->getFont(12);
	SDL_Surface* tempsur = NULL;
	tempsur = TTF_RenderUTF8_Solid(tempfont, mText.c_str(), fontColor);
	temptex = AssetManager::Surface2SDLTexture(tempsur, &mWidth, &mHeight);

	mTexture->Init("", temptex, mWidth, mHeight);
	nowUsingSubData = setDefaultSubData();
}

void InputBox::Render()
{
	SDL_SetRenderDrawColor(g_ZE_MainRenderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_Rect temp = { x, y, mWidth, mHeight };
	SDL_RenderFillRect(g_ZE_MainRenderer, &temp);
}