#include "ZE_Core.h"
#include "ZE_TextField.h"
#include "ZE_AssetManager.h"

using namespace std;

TextField::TextField(
	weak_ptr<ZeroEngine> core_engine,
	string text_f, unsigned int effectLevel_f, int fontSize_f, SDL_Color color_f,
	AssetManager* ass_f, string fontName_f)
	:TextureContainer(core_engine)
{
	Text = text_f;
	EffectLevel = effectLevel_f;
	FontSize = fontSize_f;
	TextColor = color_f;
	Ass = ass_f;
	FontName = fontName_f;

	changeText(text_f);
}

void TextField::changeText(string text)
{
	mTexture = make_shared<Texture>();
	std::unique_ptr<SDL_Texture, decltype(SDL_DestroyTexture)*> temptex{ nullptr,SDL_DestroyTexture };
	if (!Ass)
	{
		TTF_Font* tempfont = core_engine.lock()->defaultFont->getFont(FontSize);
		std::unique_ptr<SDL_Surface, decltype(SDL_FreeSurface)*> tempsur{ nullptr,SDL_FreeSurface };
		if (EffectLevel == 0)
			tempsur.reset(TTF_RenderUTF8_Solid(tempfont, text.c_str(), TextColor));
		else if (EffectLevel == 1)
			tempsur.reset(TTF_RenderUTF8_Shaded(tempfont, text.c_str(), TextColor, { 0, 0, 0 }));
		else if (EffectLevel == 2)
			tempsur.reset(TTF_RenderUTF8_Blended(tempfont, text.c_str(), TextColor));
		temptex = AssetManager::Surface2SDLTexture(core_engine, tempsur, &mWidth, &mHeight);
	}
	else
	{
		temptex = Ass->getTTFTexture(text, FontName, FontSize, TextColor, &mWidth, &mHeight, EffectLevel);
	}
	mTexture->Init("", temptex, mWidth, mHeight);
	nowUsingSubData = setDefaultSubData();
}

int TextField::getWidth()
{
	// TODO
	return 0;
}

int TextField::getHeight()
{
	// TODO
	return 0;
}

TextField::~TextField()
{
}
