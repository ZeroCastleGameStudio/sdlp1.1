#include <SDL_image.h>
#include "ZE_Core.h"

using namespace std;

//[Global]ZE版本号
string ZE_version = "1.0.0";

//[Global]引擎全局状态变量
unique_ptr<EngineGlobalState> GlobalState;

////[Global]保存SDL窗体的指针
//SDL_Window* g_ZE_Window = NULL;
////[Global]保存SDL主Surface的指针
//SDL_Surface* g_ZE_MainSurface = NULL;
////[Global]到现在也不知道这玩意到底该叫什么，就叫渲染器好了，这是主渲染器，绑定到主window
//SDL_Renderer* g_ZE_MainRenderer = NULL;
////[Global]stage对象，唯一
//Sprite ZE_stage;
////[Global]error对象(应该是唯一，其它类就算有也是private)
//Error ZE_error;
////[Global]事件处理器对象，唯一
//EventManager ZE_eventHandler;
////[Global]退出主循环的判定变量
//bool ZE_QUIT_MAIN_LOOP = false;
////[Global]保存所有的手柄指针
//deque<unique_ptr<Controller>> ZE_Controllers;
////[Global]系统默认字体
//shared_ptr<Font> defaultFont;


bool ZeroEngine::Init(string Title, int windowWidth, int windowHeight, bool useVSync, std::string defaultFontFile)
{
	bool success = true;
	GlobalState.reset(new EngineGlobalState());
	if (!Init_SDL(Title, windowWidth, windowHeight))
	{
		success = false;
	}
	else if (!Init_SDL_Image(useVSync))
	{
		success = false;
	}
	else if (!Init_SDL_ttf())
	{
		success = false;
	}
	else if (!Init_SDL_Mixer())
	{
		success = false;
	}


	// 加载字体
	GlobalState->defaultFont = make_shared<Font>("default", defaultFontFile);

	GlobalState->ZE_eventHandler.reset(new EventManager);

	GlobalState->ZE_stage.reset(new Sprite);
	GlobalState->ZE_error.reset(new Error);

	function<void(SDL_Event)> addJoyStick = [](SDL_Event evt)->void
	{
		GlobalState->ZE_Controllers.emplace_back(new Controller(SDL_NumJoysticks() - 1));
	};

	function<void(SDL_Event)> removeJoyStick = [](SDL_Event evt)->void
	{
		//delete(ZE_Controllers[evt.jdevice.which]);
		//ZE_Controllers[evt.jdevice.which].reset();
		// FIXME 这里使用下标进行访问不会在某种情况下炸么
		GlobalState->ZE_Controllers.erase(GlobalState->ZE_Controllers.begin() + evt.jdevice.which);
	};
	GlobalState->ZE_stage->addEventListener(SDL_JOYDEVICEADDED, addJoyStick);
	GlobalState->ZE_stage->addEventListener(SDL_JOYDEVICEREMOVED, removeJoyStick);

	return success;
}

bool ZeroEngine::Init_SDL(string Title, int windowWidth, int windowHeight)
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_AUDIO) < 0)
	{
		//目前仅传入VIDEO flag，即只初始化SDL的视频模块
		GlobalState->ZE_error->PopDebugConsole_SDLError("SDL could not initialize!");
		//若初始化失败则调用error类弹出错误，以后都一样
		success = false;
	}
	else
	{
		const char* tit = Title.c_str();
		//因为SDL是纯C，所以这里要把string转换为C的char，就算不想用纯C但是下面这个函数只接受纯C
		GlobalState->g_ZE_Window = SDL_CreateWindow(tit, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
		//变量含义：窗口标题，窗口出现位置的X坐标(该变量含义不明)，窗口出现位置的Y坐标(该变量含义不明)，窗口宽度，
		//窗口高度，不明变量(这个变量表示立即打开窗口，但是具体内容是什么，可以替换成什么，替换了有什么作用都不知道)
		if (GlobalState->g_ZE_Window == NULL)
		{
			GlobalState->ZE_error->PopDebugConsole_SDLError("Window could not be created!");
			success = false;
		}
		else
		{
			GlobalState->g_ZE_MainSurface = SDL_GetWindowSurface(GlobalState->g_ZE_Window);
			//抓取窗口的主Surface，所有的绘制都会绘制在主surface上
		}
	}

	return success;
}

bool ZeroEngine::Init_SDL_Image(bool useVSync)
{
	bool success = true;

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) == imgFlags))
	{
		//这里只初始化了PNG，IMG的初始化方法将会返回对应的FLAG即表示成功
		//尚不知载入多个会怎样，载入多个如何用，载入多个怎么做，载入多个返回什么
		//看了一下SDL_IMAGE支持多种格式真不是盖的，除了常见JPG PNG之外居然还支持GIF，这可少见，还有一些不流行的格式
		//BTW，IMG_INIT_PNG == 2
		GlobalState->ZE_error->PopDebugConsole_SDL_ImageError("SDL_image could not initialize!");
		success = false;
	}

	int renderFlag;
	if (useVSync)
		renderFlag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	else
		renderFlag = SDL_RENDERER_ACCELERATED;
	GlobalState->g_ZE_MainRenderer = SDL_CreateRenderer(GlobalState->g_ZE_Window, -1, renderFlag);
	//初始化并绑定渲染器
	if (GlobalState->g_ZE_MainRenderer == NULL)
	{
		GlobalState->ZE_error->PopDebugConsole_SDLError("Renderer could not be created!");
		success = false;
	}
	else
	{
		SDL_SetRenderDrawColor(GlobalState->g_ZE_MainRenderer, stageColor.red, stageColor.green, stageColor.blue, 1);
		//设置渲染器的画笔颜色
		//要用渲染器画默认图形的话比如SDL_RENDERDRAWRECT这就需要不断的切换画笔颜色。
		//比如要舞台是白色的，方块是红色的，就要先画白舞台，再画红方块，下一个循环再切换回白色
	}

	return success;
}

bool ZeroEngine::Init_SDL_ttf()
{
	bool success = true;
	if (TTF_Init() == -1)
	{
		GlobalState->ZE_error->PopDebugConsole_SDL_ttfError("SDL_ttf could not initialize!");
		success = false;
	}
	return success;
}

bool ZeroEngine::Init_SDL_Mixer()
{
	bool success = true;
	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) < 0)
	{
		success = false;
		GlobalState->ZE_error->PopDebugConsole_SDL_MixerError("SDL_mixer could not initialize!");
	}
	else
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			success = false;
			GlobalState->ZE_error->PopDebugConsole_SDL_MixerError("cuold not open audio on SDL_mixer!");
		}
	}

	return success;
}

void ZeroEngine::Start(Game* userGame)
{
	maingame = userGame;
	fraps.setUp();
	while (!GlobalState->ZE_QUIT_MAIN_LOOP)
	{

		//处理事件
		GlobalState->ZE_eventHandler->handleEvent();
		//清空
		SDL_RenderClear(GlobalState->g_ZE_MainRenderer);
		//走逻辑
		maingame->MainLoop();
		//重绘
		GlobalState->ZE_stage->Render();
		//让fraps执行逻辑
		fraps.setText();
		fraps.Render();
		//改变回默认颜色
		SDL_SetRenderDrawColor(GlobalState->g_ZE_MainRenderer, stageColor.red, stageColor.green, stageColor.blue, 1);
		//再刷新
		SDL_RenderPresent(GlobalState->g_ZE_MainRenderer);
	}
	Close();
}

void ZeroEngine::Close()
{
	//for (auto& con : ZE_Controllers)
	//{
	//	//delete(con);
	//	//con = NULL;
	//	con.reset();
	//}
	GlobalState->ZE_Controllers.clear();

	//清理舞台
	GlobalState->ZE_stage->dispose();
	//调用用户的关闭方法
	maingame->Close();

	SDL_DestroyWindow(GlobalState->g_ZE_Window);
	SDL_DestroyRenderer(GlobalState->g_ZE_MainRenderer);
	//删除渲染器
	GlobalState->g_ZE_MainRenderer = NULL;
	//删除SDL窗口
	GlobalState->g_ZE_Window = NULL;
	//必须将指针清空否则变野指针
	Mix_CloseAudio();
	while (Mix_Init(0))
	{
		Mix_Quit();
	}
	//清除默认字体  
	//因为字体库使用了SDL空间，然而这里是全局变量生命周期比引擎长，所以需要在SDL关闭前手动释放
	//否则自动释放会发生在SDL关闭后exe结束前
	GlobalState->defaultFont.reset();

	GlobalState->ZE_error.reset();
	GlobalState->ZE_stage.reset();

	GlobalState->ZE_eventHandler.reset();
	GlobalState.reset();

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	//退出
}