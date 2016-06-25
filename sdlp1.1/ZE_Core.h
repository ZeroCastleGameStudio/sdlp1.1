#pragma once
#include <iostream>
#include <memory>
#include <deque>
#include <atomic>

using namespace std;

class Fraps;
class Sprite;
class Error;
class EventManager;
class Controller;
class Font;
class ZeroEngine;

//颜色结构体，设置舞台颜色的时候使用
struct Color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

//主Game类，用户应继承此类并重写方法
class Game
{
public:
	// 初始化时需要访问核心引擎数据的部分
	virtual void Init(weak_ptr<ZeroEngine> core_engine) = 0;
	// 主渲染循环
	virtual void MainLoop() = 0;
	// 需要在引擎关闭前关闭的部分
	virtual void Close() = 0;
	virtual ~Game() {}
};

// 引擎核心初始化参数
struct ZeroEngineInitArgs
{
	string title{ "ZeroEngine" };
	int windowWidth{ 800 };
	int windowHeight{ 600 };
	bool useVSync{ false };
	std::string defaultFontFile{ "data/ttf/SourceHanSansSC-Normal.otf" };
	Color stageColor{ 0,0,0 };
};



// 引擎核心
class ZeroEngine
	: public std::enable_shared_from_this<ZeroEngine>
{
public:
	explicit ZeroEngine(ZeroEngineInitArgs& init_args);
	~ZeroEngine();
	// 阻止移动语义与拷贝语义
	ZeroEngine(const ZeroEngine&) = delete;
	ZeroEngine(ZeroEngine&&) = delete;
	ZeroEngine& operator=(const ZeroEngine&) = delete;
	ZeroEngine& operator=(ZeroEngine&&) = delete;

	//初始化方法，使用ZE前必须调用此方法
	//该方法调用了本类中的其它两个方法，分别初始化了外部库SDL和SDL_IMAGE
	//该方法会返回一个bool值，若所有模块初始化成功则返回true，否则返回false
	//外部库的报错写在对应方法里
	bool Init(std::shared_ptr<Game>& userGame);
	//该方法是ZE的主循环方法，目前是暂用版
	void Run();
	// 挂号
	size_t getNewDisplayObjectIndex();

private:
	//SDL的初始化方法，该方法的三个参数分别是窗口标题、窗口宽度、窗口高度
	bool Init_SDL(string, int, int);
	//初始化了SDL_IMAGE第三方库
	bool Init_SDL_Image(bool);
	//初始化了SDL_TTF第三方库
	bool Init_SDL_ttf() const;
	//初始化音频库
	bool Init_SDL_Mixer() const;
	//清理
	void Close();

private:
	//舞台颜色，默认为黑
	Color stageColor{ 0, 0, 0 };
	//最后渲染在舞台上，显示一些监视变量
	std::unique_ptr<Fraps> fraps;
	//用户的游戏对象
	std::shared_ptr<Game> userGame;
	// 显示对象index发放器
	std::atomic_size_t display_object_index{ 0 };


public:

	//[Global]保存SDL窗体的指针
	std::unique_ptr<SDL_Window, decltype(SDL_DestroyWindow)*> g_ZE_Window{ nullptr,SDL_DestroyWindow };
	//[Global]保存SDL主Surface的指针
	std::unique_ptr<SDL_Surface, decltype(SDL_FreeSurface)*> g_ZE_MainSurface{ nullptr,SDL_FreeSurface };
	//[Global]主渲染器，绑定到主window
	std::unique_ptr<SDL_Renderer, decltype(SDL_DestroyRenderer)*> g_ZE_MainRenderer{ nullptr,SDL_DestroyRenderer };
	//[Global]stage对象，唯一
	std::unique_ptr<Sprite> ZE_stage;
	//[Global]error对象(应该是唯一，其它类就算有也是private)
	std::unique_ptr<Error> ZE_error;
	//[Global]事件处理器对象，唯一
	std::unique_ptr<EventManager> ZE_eventHandler;
	//[Global]退出主循环的判定变量
	std::atomic_bool ZE_QUIT_MAIN_LOOP{ false };
	//[Global]保存所有的手柄指针
	std::deque<std::unique_ptr<Controller>> ZE_Controllers;
	//[Global]系统默认字体
	std::shared_ptr<Font> defaultFont;
};