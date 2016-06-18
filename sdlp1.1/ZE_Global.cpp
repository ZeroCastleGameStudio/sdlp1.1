#include "ZE_Global.h"

#include "ZE_EngineGlobalState.h"


//[Global]ZE版本号
std::string ZE_version = "1.0.0";

//[Global]引擎全局状态变量
std::unique_ptr<EngineGlobalState> GlobalState{ new EngineGlobalState() };

