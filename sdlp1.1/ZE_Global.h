#pragma once
#include <string>
#include <memory>


// 声明提前
class ZeroEngine;
class EngineGlobalState;

//[Global]ZE版本号
extern std::string ZE_version;
//[Global]引擎全局状态变量
extern std::unique_ptr<EngineGlobalState> GlobalState;

