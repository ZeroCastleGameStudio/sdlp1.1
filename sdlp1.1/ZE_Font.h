#pragma once
#include <string>
#include <deque>
#include <memory>
#include <array>
#include <unordered_map>
#include <SDL_Extension/SDL_ttf.h>

using namespace std;

class ZeroEngine;

class Font
	: public enable_shared_from_this<Font>
{
public:
	Font(weak_ptr<ZeroEngine> core_engine, string name, string path);
	~Font();
	// 不得析构返回指针
	TTF_Font* getFont(int size = 15);
	string name;

private:
	weak_ptr<ZeroEngine> core_engine;
	string mPath;
	//这里的数组用来储存不同的字号
	unordered_map<int, TTF_Font*> mFont;

	void setNewSizeOfFont(int);
};