#include "ZE_Font.h"
#include "ZE_Core.h"
#include "ZE_Error.h"

using namespace std;

Font::Font(weak_ptr<ZeroEngine> core_engine, string name, string path)
	:name(name), core_engine(core_engine), mPath(path)
{
}

// 不得析构返回指针
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
			core_engine.lock()->ZE_error->PopDebugConsole_SDL_ttfError("Failed to load ttf font!");
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
		if (a.second != nullptr)
		{
			TTF_CloseFont(a.second);
			a.second = nullptr;
		}
	}
}