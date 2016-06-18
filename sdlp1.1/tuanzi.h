#pragma once
#include <memory>
#include <array>
#include "ZE_Image.h"
#include "mygame.h"
#include "ZE_Global.h"


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

inline void tuanzi::registerEventListener()
{

	// 在这里传入智能指针 让其持有对象的智能指针以保证对象不被提前析构，且保证对象能够被析构
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_UP, std::bind(&tuanzi::up, this, shared_from_this(), std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_DOWN, std::bind(&tuanzi::down, this, shared_from_this(), std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_LEFT, std::bind(&tuanzi::left, this, shared_from_this(), std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_RIGHT, std::bind(&tuanzi::right, this, shared_from_this(), std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::EveryLoop, 0, std::bind(&tuanzi::do_inertia, this, shared_from_this(), std::placeholders::_1));

}

inline tuanzi::tuanzi(const textureStruct& texture_struct)
: Image{texture_struct}
{
}

inline tuanzi::tuanzi(const deque<textureStruct>& texture_structs, unsigned frameSpeed)
	: Image{texture_structs, frameSpeed}
{
}

inline void tuanzi::do_inertia(shared_ptr<tuanzi>& my, SDL_Event evt)
{
	if (inertia[0] > 1)
	{
		x -= inertia[0]--;
	}
	if (inertia[1] > 1)
	{
		x += inertia[1]--;
	}
	if (inertia[2] > 1)
	{
		y -= inertia[2]--;
	}
	if (inertia[3] > 1)
	{
		y += inertia[3]--;
	}
	check_inertia();
}

inline void tuanzi::check_inertia()
{
	for (auto& a : inertia)
	{
		if (a > 10)
		{
			a = 10;
		}
	}
}

inline void tuanzi::left(shared_ptr<tuanzi>& my, SDL_Event evt)
{
	inertia[0] += base_inertia;
	flip = SDL_FLIP_NONE;
}

inline void tuanzi::right(shared_ptr<tuanzi>& my, SDL_Event evt)
{
	inertia[1] += base_inertia;
	flip = SDL_FLIP_HORIZONTAL;
}

inline void tuanzi::up(shared_ptr<tuanzi>& my, SDL_Event evt)
{
	inertia[2] += base_inertia;
}

inline void tuanzi::down(shared_ptr<tuanzi>& my, SDL_Event evt)
{
	inertia[3] += base_inertia;
}
