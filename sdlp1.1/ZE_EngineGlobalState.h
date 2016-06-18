#pragma once

#include <memory>
#include <atomic>
#include <deque>
#include <SDL.h>


class Sprite;
class Error;
class EventManager;
class Controller;
class Font;
class ZeroEngine;



// 试验性
// 封装SDL指针到智能指针

template<typename PtrType, typename DestoryType>
auto make_sdl_unique_ptr(PtrType* ptr, DestoryType destory_func)
{
	return std::move(unique_ptr<PtrType, DestoryType>(ptr, destory_func));
}



// 引擎的全局状态管理类
// 管理引擎全局变量以解决对象构造与虚构顺序问题
class EngineGlobalState
{
public:
	// 阻止重复构造
	EngineGlobalState();
	~EngineGlobalState();
	// 阻止移动语义与拷贝语义
	EngineGlobalState(const EngineGlobalState&) = delete;
	EngineGlobalState(EngineGlobalState&&) = delete;
	EngineGlobalState& operator=(const EngineGlobalState&) = delete;
	EngineGlobalState& operator=(EngineGlobalState&&) = delete;

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
	//[Global]到引擎实例的指针
	std::unique_ptr<ZeroEngine> g_Engine_ptr;
};







