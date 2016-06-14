#pragma once
#include <string>
#include <deque>
#include <memory>
#include <array>
#include <unordered_map>
#include <SDL_Extension/SDL_ttf.h>

using namespace std;

class Font
	: public enable_shared_from_this<Font>
{
public:
	Font(string, string);
	~Font();
	TTF_Font* getFont(int size = 15);
	string name;

private:
	//����������������治ͬ���ֺ�
	unordered_map<int, TTF_Font*> mFont;
	string mPath;

	void setNewSizeOfFont(int);
};