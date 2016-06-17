#include <iostream>
#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include "ZE_Core.h"
#include "ZE_Sprite.h"
#include "ZE_Image.h"
#include "ZE_Texture.h"
#include "ZE_TextField.h"
#include "ZE_Sound.h"
#include "mygame.h"
#include <functional>

using namespace std;

mygame::mygame() {}

void mygame::Init()
{
	resourses.Init("data/amText.xml");

	Image* tuanzi;

	Image* background = new Image(resourses.getTexture("background"));
	GlobalState->ZE_stage->addChild(background);

	tuanzi = new Image(resourses.getTextures("bug_fly1_"), 24);

	TextField* text = new TextField(u8"然后我是一个显示中文的内部Text");
	GlobalState->ZE_stage->addChild(text);

	GlobalState->ZE_stage->addChild(tuanzi);
	tuanzi->play();

	Image* test1 = new Image(resourses.getTextures("test1_"), 24);
	test1->scaleX = 0.2;
	test1->scaleY = 0.2;
	GlobalState->ZE_stage->addChild(test1);
	test1->play();


	auto temp = resourses.getSound("testsound");

	// 对象类实现的按键事件处理  需要启用智能指针保证对象不被提前析构
	class tuanzi_event
		: enable_shared_from_this<tuanzi_event>
	{
	public:
		tuanzi_event(Image* tuanzi) : tuanzi(tuanzi) {}
		Image* tuanzi;

		array<int, 4> inertia{{ 0,0,0,0 }};
		int base_inertia{ 5 };
		void do_inertia(shared_ptr<tuanzi_event>& my, SDL_Event evt)
		{
			if (inertia[0] > 1)
			{
				tuanzi->x -= inertia[0]--;
			}
			if (inertia[1] > 1)
			{
				tuanzi->x += inertia[1]--;
			}
			if (inertia[2] > 1)
			{
				tuanzi->y -= inertia[2]--;
			}
			if (inertia[3] > 1)
			{
				tuanzi->y += inertia[3]--;
			}
			check_inertia();
		}
		void check_inertia()
		{
			for (auto&a : inertia)
			{
				if (a > 10)
				{
					a = 10;
				}
			}
		}

		// 即使没有使用也在这里传入智能指针以保证对象不被析构
		void left(shared_ptr<tuanzi_event>& my, SDL_Event evt)
		{
			inertia[0] += base_inertia;
			tuanzi->flip = SDL_FLIP_NONE;
		}
		void right(shared_ptr<tuanzi_event>& my, SDL_Event evt)
		{
			inertia[1] += base_inertia;
			tuanzi->flip = SDL_FLIP_HORIZONTAL;
		}
		void up(shared_ptr<tuanzi_event>& my, SDL_Event evt)
		{
			inertia[2] += base_inertia;
		}
		void down(shared_ptr<tuanzi_event>& my, SDL_Event evt)
		{
			inertia[3] += base_inertia;
		}
	};
	auto tuanzi_event_obj = make_shared<tuanzi_event>(tuanzi);

	function <void(SDL_Event)> eventtest = [=](SDL_Event evt)->void
	{
		if (evt.key.keysym.sym == SDLK_SPACE)
		{
			cout << "space" << endl;
			temp->play();
		}
		else if (evt.key.keysym.sym == SDLK_RETURN)
		{
			cout << "enter" << endl;
		}
	};
	function <void(SDL_Event)> eventtest2 = [=](SDL_Event evt)
	{
		int b = evt.jbutton.button;
		cout << b << endl;
		GlobalState->ZE_Controllers[evt.jbutton.which]->rumble();
	};
	function <void(SDL_Event)> eventtest3 = [=](SDL_Event evt)
	{
		int a = evt.jaxis.axis;
		cout << a << endl;
	};
	function <void(SDL_Event)> eventtest4 = [=](SDL_Event evt)
	{
		int c = evt.jhat.value;
		cout << c << endl;
	};

	// 在这里传入智能指针 让其持有对象的智能指针以保证对象不被提前析构，且保证对象能够被析构
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_UP, std::bind(&tuanzi_event::up, tuanzi_event_obj.get(), tuanzi_event_obj, std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_DOWN, std::bind(&tuanzi_event::down, tuanzi_event_obj.get(), tuanzi_event_obj, std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_LEFT, std::bind(&tuanzi_event::left, tuanzi_event_obj.get(), tuanzi_event_obj, std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::KeyboardStateMode, SDL_SCANCODE_RIGHT, std::bind(&tuanzi_event::right, tuanzi_event_obj.get(), tuanzi_event_obj, std::placeholders::_1));
	GlobalState->ZE_stage->addEventListener(EventMode::EveryLoop, 0, std::bind(&tuanzi_event::do_inertia, tuanzi_event_obj.get(), tuanzi_event_obj, std::placeholders::_1));

	GlobalState->ZE_stage->addEventListener(EventMode::RawEventMode, SDL_JOYBUTTONDOWN, eventtest2);
	GlobalState->ZE_stage->addEventListener(EventMode::RawEventMode, SDL_JOYAXISMOTION, eventtest3);
	GlobalState->ZE_stage->addEventListener(EventMode::RawEventMode, SDL_JOYHATMOTION, eventtest4);

	resourses.getSound("bgm5")->play();
}

void mygame::MainLoop()
{

}

void mygame::Close()
{
	resourses.dispose();
}

mygame::~mygame()
{
}
