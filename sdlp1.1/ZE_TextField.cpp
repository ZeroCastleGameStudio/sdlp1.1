#include "ZE_Core.h"
#include "ZE_TextField.h"
#include "ZE_AssetManager.h"

using namespace std;

TextField::TextField(string text_f, unsigned int effectLevel_f, int fontSize_f, SDL_Color color_f,
	AssetManager* ass_f, string fontName_f)
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
	freeTexture();
	mTexture = new Texture();
	SDL_Texture* temptex;
	if (Ass == NULL)
	{
		TTF_Font* tempfont = defaultFont->getFont(FontSize);
		SDL_Surface* tempsur = NULL;
		if (EffectLevel == 0)
			tempsur = TTF_RenderUTF8_Solid(tempfont, text.c_str(), TextColor);
		else if (EffectLevel == 1)
			tempsur = TTF_RenderUTF8_Shaded(tempfont, text.c_str(), TextColor, { 0, 0, 0 });
		else if (EffectLevel == 2)
			tempsur = TTF_RenderUTF8_Blended(tempfont, text.c_str(), TextColor);
		temptex = AssetManager::Surface2SDLTexture(tempsur, &mWidth, &mHeight);
	}
	else
	{
		temptex = Ass->getTTFTexture(text, FontName, FontSize, TextColor, &mWidth, &mHeight, EffectLevel);
	}
	mTexture->Init("", temptex, mWidth, mHeight);
	nowUsingSubData = setDefaultSubData();
}

TextField::~TextField()
{
	freeTexture();
}