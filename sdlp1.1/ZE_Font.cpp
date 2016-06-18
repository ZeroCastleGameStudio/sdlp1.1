#include "ZE_Font.h"
#include "ZE_Core.h"
#include "ZE_Global.h"

using namespace std;

Font::Font(string name, string path)
{
	this->name = name;
	this->mPath = path;
}

TTF_Font* Font::getFont(int size)
{
	try
	{
		return mFont.at(size);
	}
	catch (const std::out_of_range&)
	{
		setNewSizeOfFont(size);
		return mFont.at(size);
	}
}

void Font::setNewSizeOfFont(int size)
{
	if (mFont.find(size) == mFont.end())
	{
		auto font = TTF_OpenFont(mPath.c_str(), size);
		if (!font)
		{
			GlobalState->ZE_error->PopDebugConsole_SDL_ttfError("Failed to load ttf font!");
			cout << "Failed to load ttf font!" << endl;
			mFont.emplace(size, nullptr);
			return;
		}
		mFont.emplace(size, font);
	}
}

Font::~Font()
{
	for (auto& a : mFont)
	{
		if (a.second != NULL)
		{
			TTF_CloseFont(a.second);
			a.second = NULL;
		}
	}
}