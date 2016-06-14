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

using namespace std;

mygame::mygame() {};

void mygame::Init()
{
	resourses.Init("data/amText.xml");

	Image* tuanzi;

	Image* background = new Image(resourses.getTexture("background"));
	ZE_stage.addChild(background);

	tuanzi = new Image(resourses.getTextures("bug_fly1_"), 24);

	TextField* text = new TextField(u8"然后我是一个显示中文的内部Text");
	ZE_stage.addChild(text);

	ZE_stage.addChild(tuanzi);
	tuanzi->play();

	Image* test1 = new Image(resourses.getTextures("test1_"), 24);
	test1->scaleX = 0.2;
	test1->scaleY = 0.2;
	ZE_stage.addChild(test1);
	test1->play();


	auto temp = resourses.getSound("testsound");

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
		else
		{
			switch (evt.key.keysym.sym)
			{
			case SDLK_UP: tuanzi->y -= 1; break;
			case SDLK_DOWN: tuanzi->y += 1; break;
			case SDLK_LEFT:
			{
				tuanzi->x -= 1;
				tuanzi->flip = SDL_FLIP_NONE;
			}break;
			case SDLK_RIGHT:
			{
				tuanzi->x += 1;
				tuanzi->flip = SDL_FLIP_HORIZONTAL;
			}break;
			}
		}
	};
	function <void(SDL_Event)> eventtest2 = [=](SDL_Event evt) 
	{
		int b = evt.jbutton.button;
		cout << b << endl;
		ZE_Controllers[evt.jbutton.which]->rumble();
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

	ZE_stage.addEventListener(SDL_KEYDOWN, eventtest);

	ZE_stage.addEventListener(SDL_JOYBUTTONDOWN, eventtest2);
	ZE_stage.addEventListener(SDL_JOYAXISMOTION, eventtest3);
	ZE_stage.addEventListener(SDL_JOYHATMOTION, eventtest4);

	resourses.getSound("bgm5")->play();
}

void mygame::MainLoop()
{

}

void mygame::Close()
{
	resourses.dispose();
}