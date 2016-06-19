#pragma once
#include <memory>
#include <array>
#include "ZE_Image.h"


class tuanzi
	:public Image
{
public:

	explicit tuanzi(const textureStruct& texture_struct);

	tuanzi(const deque<textureStruct>& texture_structs, unsigned frameSpeed);

	void registerEventListener();

	void do_inertia(shared_ptr<tuanzi>& my, SDL_Event evt);

	void check_inertia();

	// 即使没有使用也在这里传入智能指针以保证对象不被析构
	void left(shared_ptr<tuanzi>& my, SDL_Event evt);

	void right(shared_ptr<tuanzi>& my, SDL_Event evt);

	void up(shared_ptr<tuanzi>& my, SDL_Event evt);

	void down(shared_ptr<tuanzi>& my, SDL_Event evt);
private:
	std::array<int, 4> inertia{ 0,0,0,0 };
	int base_inertia{ 5 };
};
