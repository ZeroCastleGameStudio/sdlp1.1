#include "tuanzi.h"
#include "mygame.h"
#include "ZE_Global.h"
#include "ZE_EngineGlobalState.h"
#include "ZE_Sprite.h"
#include "ZE_DisplayObject.h"


void tuanzi::registerEventListener()
{

	// 在这里传入智能指针 让其持有对象的智能指针以保证对象不被提前析构，且保证对象能够被析构
	// 需要将父类智能指针转换为子类智能指针以便调用器能找到对应的函数
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_UP, std::bind(&tuanzi::up, this, std::dynamic_pointer_cast<tuanzi>(shared_from_this()), std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_DOWN, std::bind(&tuanzi::down, this, std::dynamic_pointer_cast<tuanzi>(shared_from_this()), std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_LEFT, std::bind(&tuanzi::left, this, std::dynamic_pointer_cast<tuanzi>(shared_from_this()), std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_RIGHT, std::bind(&tuanzi::right, this, std::dynamic_pointer_cast<tuanzi>(shared_from_this()), std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::EveryLoop, 0, std::bind(&tuanzi::do_inertia, this, std::dynamic_pointer_cast<tuanzi>(shared_from_this()), std::placeholders::_1));

}

tuanzi::tuanzi(const textureStruct& texture_struct)
	: Image{ texture_struct }
{
}

tuanzi::tuanzi(const deque<textureStruct>& texture_structs, unsigned frameSpeed)
	: Image{ texture_structs, frameSpeed }
{
}

void tuanzi::do_inertia(shared_ptr<tuanzi>& my, SDL_Event evt)
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

void tuanzi::check_inertia()
{
	for (auto& a : inertia)
	{
		if (a > 10)
		{
			a = 10;
		}
	}
}

void tuanzi::left(shared_ptr<tuanzi>& my, SDL_Event evt)
{
	inertia[0] += base_inertia;
	flip = SDL_FLIP_NONE;
}

void tuanzi::right(shared_ptr<tuanzi>& my, SDL_Event evt)
{
	inertia[1] += base_inertia;
	flip = SDL_FLIP_HORIZONTAL;
}

void tuanzi::up(shared_ptr<tuanzi>& my, SDL_Event evt)
{
	inertia[2] += base_inertia;
}

void tuanzi::down(shared_ptr<tuanzi>& my, SDL_Event evt)
{
	inertia[3] += base_inertia;
}
