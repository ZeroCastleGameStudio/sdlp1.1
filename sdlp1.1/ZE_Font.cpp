#include "ZE_Font.h"
#include "ZE_Core.h"

using namespace std;

Font::Font(string name, string path)
{
	for (int i = 0; i < 100; i++)
	{
		mFont[i] = NULL;
	}
	this->name = name;
	this->mPath = path;
}

TTF_Font* Font::getFont(int size)
{
	if (mFont[size] != NULL)
	{
		return mFont[size];
	}
	else
	{
		setNewSizeOfFont(size);
		return mFont[size];
	}
}

void Font::setNewSizeOfFont(int size)
{
	if (mFont[size] == NULL)
	{
		mFont[size] = TTF_OpenFont(mPath.c_str(), size);
		if (mFont[size] == NULL)
		{
			ZE_error.PopDebugConsole_SDL_ttfError("Failed to load ttf font!");
		}
	}
}

Font::~Font()
{
	for (int i = 0; i < 100; i++)
	{
		if (mFont[i] != NULL)
		{
			TTF_CloseFont(mFont[i]);
			mFont[i] = NULL;
		}
	}
}