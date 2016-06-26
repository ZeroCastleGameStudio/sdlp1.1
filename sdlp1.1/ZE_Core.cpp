
#include <SDL_image.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "ZE_Core.h"
#include "ZE_EventManager.h"
#include "ZE_Error.h"
#include "ZE_Fraps.h"
#include "ZE_Sprite.h"
#include "ZE_Font.h"
#include "ZE_Controller.h"

//[Global]ZE版本号
std::string ZE_version = "2.0.0";

using namespace std;

ZeroEngine::ZeroEngine()
{
	// 构造函数中不能使用shared_from_this()
}

ZeroEngine::~ZeroEngine()
{
	//清除默认字体
	defaultFont.reset();
	fraps.reset();
	ZE_stage.reset();
	ZE_eventHandler.reset();
	ZE_error.reset();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	//退出
	cout << "ZeroEngine::~ZeroEngine()" << endl;
}

bool ZeroEngine::Init(ZeroEngineInitArgs& init_args, std::shared_ptr<Game>& userGame)
{
	userGame = userGame;

	// 做构造函数路没做完的事情
	ZE_error.reset(new Error);
	ZE_eventHandler.reset(new EventManager(shared_from_this()));
	ZE_stage.reset(new Sprite(shared_from_this()));
	this->stageColor = init_args.stageColor;


	if (!Init_SDL(init_args.title, init_args.windowWidth, init_args.windowHeight))
	{
		throw std::runtime_error("SDL Init Error.");
	}

	if (!Init_SDL_Image(init_args.useVSync))
	{
		throw std::runtime_error("SDL Image Init Error.");
	}

	if (!Init_SDL_ttf())
	{
		throw std::runtime_error("SDL Ttf Init Error.");
	}

	if (!Init_SDL_Mixer())
	{
		throw std::runtime_error("SDL Mixer Init Error.");
	}


	fraps = make_unique<Fraps>(shared_from_this(), this->getNewDisplayObjectIndex());

	// 加载字体
	defaultFont = make_shared<Font>(shared_from_this(), "default", init_args.defaultFontFile);


	// 内部初始化结束  开始初始化用户类

	userGame->Init(shared_from_this());


	//function<void(SDL_Event)> addJoyStick = [&](SDL_Event evt)->void
	//{
	//	ZE_Controllers.push_back(make_unique<Controller>(SDL_NumJoysticks() - 1));
	//	//(new Controller(SDL_NumJoysticks() - 1));
	//};

	//function<void(SDL_Event)> removeJoyStick = [&](SDL_Event evt)->void
	//{
	//	//delete(ZE_Controllers[evt.jdevice.which]);
	//	//ZE_Controllers[evt.jdevice.which].reset();
	//	// FIXME 这里使用下标进行访问不会在某种情况下炸么
	//	ZE_Controllers.erase(ZE_Controllers.begin() + evt.jdevice.which);
	//};
	//ZE_stage->addEventListener(EventMode::RawEventMode, SDL_JOYDEVICEADDED, addJoyStick);
	//ZE_stage->addEventListener(EventMode::RawEventMode, SDL_JOYDEVICEREMOVED, removeJoyStick);

	return true;
}

bool ZeroEngine::Init_SDL(string Title, int windowWidth, int windowHeight)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_AUDIO) < 0)
	{
		ZE_error->PopDebugConsole_SDLError("SDL could not initialize!");
		return false;
	}
	//变量含义：窗口标题，窗口默认坐标，窗口宽高，窗口初始状态
	g_ZE_Window.reset(SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN));
	if (!g_ZE_Window)
	{
		ZE_error->PopDebugConsole_SDLError("Window could not be created!");
		return false;
	}
	//获取窗口的主Surface，所有的绘制都会绘制在主surface上
	g_ZE_MainSurface.reset(SDL_GetWindowSurface(g_ZE_Window.get()));

	return true;
}

bool ZeroEngine::Init_SDL_Image(bool useVSync)
{
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) == imgFlags))
	{
		//这里只初始化了PNG，IMG的初始化方法将会返回对应的FLAG即表示成功
		//尚不知载入多个会怎样，载入多个如何用，载入多个怎么做，载入多个返回什么
		//看了一下SDL_IMAGE支持多种格式真不是盖的，除了常见JPG PNG之外居然还支持GIF，这可少见，还有一些不流行的格式
		//BTW，IMG_INIT_PNG == 2
		ZE_error->PopDebugConsole_SDL_ImageError("SDL_image could not initialize!");
		return false;
	}

	int renderFlag;
	if (useVSync)
		renderFlag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	else
		renderFlag = SDL_RENDERER_ACCELERATED;
	g_ZE_MainRenderer.reset(SDL_CreateRenderer(g_ZE_Window.get(), -1, renderFlag));
	//初始化并绑定渲染器
	if (g_ZE_MainRenderer == nullptr)
	{
		ZE_error->PopDebugConsole_SDLError("Renderer could not be created!");
		return false;
	}
	//设置渲染器的画笔颜色
	//要用渲染器画默认图形的话比如SDL_RENDERDRAWRECT这就需要不断的切换画笔颜色。
	//比如要舞台是白色的，方块是红色的，就要先画白舞台，再画红方块，下一个循环再切换回白色
	SDL_SetRenderDrawColor(g_ZE_MainRenderer.get(), stageColor.red, stageColor.green, stageColor.blue, 1);

	return true;
}

bool ZeroEngine::Init_SDL_ttf() const
{
	if (TTF_Init() == -1)
	{
		ZE_error->PopDebugConsole_SDL_ttfError("SDL_ttf could not initialize!");
		return false;
	}
	return true;
}

bool ZeroEngine::Init_SDL_Mixer() const
{
	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) < 0)
	{
		ZE_error->PopDebugConsole_SDL_MixerError("SDL_mixer could not initialize!");
		return false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		ZE_error->PopDebugConsole_SDL_MixerError("cuold not open audio on SDL_mixer!");
		return false;
	}
	return true;
}

void ZeroEngine::Run()
{
	fraps->setUp();
	while (!ZE_QUIT_MAIN_LOOP)
	{

		//处理事件
		ZE_eventHandler->handleEvent();
		//清空
		SDL_RenderClear(g_ZE_MainRenderer.get());
		//走逻辑
		userGame->MainLoop();
		//重绘
		ZE_stage->Render();
		//让fraps执行逻辑
		fraps->setText();
		fraps->Render();
		//改变回默认颜色
		SDL_SetRenderDrawColor(g_ZE_MainRenderer.get(), stageColor.red, stageColor.green, stageColor.blue, 1);
		//再刷新
		SDL_RenderPresent(g_ZE_MainRenderer.get());
	}
	Close();
}

size_t ZeroEngine::getNewDisplayObjectIndex()
{
	return ++display_object_index;
}

void ZeroEngine::Close()
{
	//调用用户的关闭方法
	userGame->Close();
	// 释放用户指针 用户对象可能在此析构
	userGame.reset();

	ZE_Controllers.clear();

	//清理舞台
	ZE_stage->dispose();

	// 这里的销毁循序应该与初始化顺序相反
	g_ZE_MainSurface.reset();
	//删除渲染器
	g_ZE_MainRenderer.reset();
	//删除SDL窗口
	g_ZE_Window.reset();
	Mix_CloseAudio();
	while (Mix_Init(0))
	{
		Mix_Quit();
	}


}
