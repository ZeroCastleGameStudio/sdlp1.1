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
#include "tuanzi.h"
#include "ZE_Global.h"
#include "ZE_Core.h"
#include "ZE_EngineGlobalState.h"
#include "ZE_Controller.h"
#include "ZE_AssetManager.h"

using namespace std;

mygame::mygame() :resourses(new AssetManager) {}

void mygame::Init()
{
	resourses->Init("data/amText.xml");

	auto background = make_shared<Image>(resourses->getTexture("background"));
	GlobalState->ZE_stage->addChild(background);

	auto text = make_shared<TextField>(u8"然后我是一个显示中文的内部Text");
	GlobalState->ZE_stage->addChild(text);

	auto test1 = make_shared<Image>(resourses->getTextures("test1_"), 24);
	test1->scaleX = 0.2;
	test1->scaleY = 0.2;
	GlobalState->ZE_stage->addChild(test1);
	test1->play();


	auto temp = resourses->getSound("testsound");

	auto tuanzi_obj = make_shared<tuanzi>(resourses->getTextures("bug_fly1_"), 24);
	GlobalState->ZE_stage->addChild(tuanzi_obj);
	tuanzi_obj->play();
	tuanzi_obj->registerEventListener();

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

	GlobalState->ZE_stage->addEventListener(EventMode::RawEventMode, SDL_JOYBUTTONDOWN, eventtest2);
	GlobalState->ZE_stage->addEventListener(EventMode::RawEventMode, SDL_JOYAXISMOTION, eventtest3);
	GlobalState->ZE_stage->addEventListener(EventMode::RawEventMode, SDL_JOYHATMOTION, eventtest4);

	resourses->getSound("bgm5")->play();
}

void mygame::MainLoop()
{

}

void mygame::Close()
{
	resourses->dispose();
}

mygame::~mygame()
{
	mygame::Close();
}
