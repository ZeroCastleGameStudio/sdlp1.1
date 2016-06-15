#include "ZE_EngineGlobalState.h"
#include <assert.h>

#include <iostream>

std::atomic_bool EngineGlobalState_Singleton{ false };

EngineGlobalState::EngineGlobalState()
{
	if (EngineGlobalState_Singleton.exchange(true))
	{
		throw std::runtime_error("EngineGlobalState Re Ctor");
	}
	cout << "EngineGlobalState::EngineGlobalState()" << endl;
}


EngineGlobalState::~EngineGlobalState()
{
	// 垃圾回收断言 此断言在DEBUG模式下检查断言内容是否为真
	// 在这里会检查如下的指针是否被正确置空
	assert(nullptr == g_ZE_Window);
	assert(nullptr == g_ZE_MainSurface);
	assert(nullptr == g_ZE_MainRenderer);
	cout << "EngineGlobalState::~EngineGlobalState()" << endl;
}
