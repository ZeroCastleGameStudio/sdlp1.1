#include <iostream>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL_Extension/SDL_image.h>
#include "ZE_Core.h"
#include "ZE_Sprite.h"
#include "ZE_Image.h"
#include "ZE_Texture.h"
#include "ZE_TextField.h"
#include "ZE_Sound.h"
#include "mygame.h"

using namespace std;

mygame::mygame() {};

Sound* temp;
Image* tuanzi;

void mygame::eventtest(SDL_Event evt)
{
	if (evt.key.keysym.sym == SDLK_SPACE)
	{
		cout << "space" << endl;
		temp->play();
	}
}

void mygame::eventtest2(SDL_Event evt)
{
	if (evt.key.keysym.sym == SDLK_RETURN)
	{
		cout << "enter" << endl;
	}
}

void mygame::eventtest3(SDL_Event evt)
{
	int b = evt.jbutton.button;
	cout << b << endl;
	ZE_Controllers[evt.jbutton.which]->rumble();
}

void mygame::eventtest4(SDL_Event evt)
{
	int a = evt.jaxis.axis;
	cout << a << endl;
}

void mygame::eventtest5(SDL_Event evt)
{
	int c = evt.jhat.value;
	cout << c << endl;
}

void mygame::eventtest6(SDL_Event evt)
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

void mygame::Init()
{
	resourses.Init("data/amText.xml");

	Image* background = new Image(resourses.getTexture("background"));
	ZE_stage.addChild(background);

	tuanzi = new Image(resourses.getTextures("bug_fly1_"), 24);

	TextField* text = new TextField(u8"然后我是一个显示中文的内部Text");
	ZE_stage.addChild(text);

	ZE_stage.addChild(tuanzi);
	tuanzi->play();

	temp = resourses.getSound("testsound");

	function <void(SDL_Event)> tt = [&](SDL_Event evt)->void
	{
		if (evt.key.keysym.sym == SDLK_SPACE)
		{
			cout << "space" << endl;
			temp->play();
		}
	};

	ZE_stage.addEventListener(SDL_KEYDOWN, tt);
	/*SDL_Event xx;
	SDL_PollEvent(&xx);
	tt(xx);*/
	/*ZE_stage.addEventListener(SDL_KEYDOWN, eventtest2);
	ZE_stage.addEventListener(SDL_KEYDOWN, eventtest6);

	ZE_stage.addEventListener(SDL_JOYBUTTONDOWN, eventtest3);
	ZE_stage.addEventListener(SDL_JOYAXISMOTION, eventtest4);
	ZE_stage.addEventListener(SDL_JOYHATMOTION, eventtest5);*/

	resourses.getSound("bgm5")->play();
}

void mygame::MainLoop()
{

}

void mygame::Close()
{
	resourses.dispose();
}