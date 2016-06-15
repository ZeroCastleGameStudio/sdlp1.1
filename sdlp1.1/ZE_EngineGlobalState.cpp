#include "ZE_EngineGlobalState.h"
#include <assert.h>

#include <type_traits>

std::atomic_bool EngineGlobalState_Singleton{ false };


// 测试试验性功能用
void test()
{

	auto z = make_sdl_unique_ptr((SDL_Texture*)nullptr, SDL_DestroyTexture);

	

	auto a = unique_ptr<SDL_Texture, decltype(SDL_DestroyTexture)*>((SDL_Texture*)nullptr, SDL_DestroyTexture);
	//auto b = unique_ptr<int>();
	//auto c = unique_ptr<int>();

	z.reset();

}




EngineGlobalState::EngineGlobalState()
{
	if (EngineGlobalState_Singleton.exchange(true))
	{
		throw std::runtime_error("EngineGlobalState Re Ctor");
	}
	test();
}


EngineGlobalState::~EngineGlobalState()
{
	// 垃圾回收断言 此断言在DEBUG模式下检查断言内容是否为真
	// 在这里会检查如下的指针是否被正确置空
	assert(NULL == g_ZE_Window);
	assert(NULL == g_ZE_MainSurface);
	assert(NULL == g_ZE_MainRenderer);
}
