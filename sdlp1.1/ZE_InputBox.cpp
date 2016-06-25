#include "ZE_Core.h"
#include "ZE_InputBox.h"
#include "ZE_AssetManager.h"

using namespace std;

InputBox::InputBox(weak_ptr<ZeroEngine> core_engine,int x, int y, int w, int h, SDL_Color fontColor, SDL_Color bgColor)
	:TextureContainer(core_engine)
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

	TTF_Font* tempfont = core_engine.lock()->defaultFont->getFont(12);
	std::unique_ptr<SDL_Surface, decltype(SDL_FreeSurface)*> tempsur{
		TTF_RenderUTF8_Solid(tempfont, mText.c_str(), fontColor),
		SDL_FreeSurface
	};
	std::unique_ptr<SDL_Texture, decltype(SDL_DestroyTexture)*> temptex
		= AssetManager::Surface2SDLTexture(core_engine, tempsur, &mWidth, &mHeight);

	mTexture = make_shared<Texture>();
	mTexture->Init("", temptex, mWidth, mHeight);
	nowUsingSubData = setDefaultSubData();
}

void InputBox::Render()
{
	SDL_SetRenderDrawColor(core_engine.lock()->g_ZE_MainRenderer.get(), bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_Rect temp = { x, y, mWidth, mHeight };
	SDL_RenderFillRect(core_engine.lock()->g_ZE_MainRenderer.get(), &temp);
}