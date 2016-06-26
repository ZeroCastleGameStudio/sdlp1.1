////////////////////////////////////////////////////////////
//所有的鼠标提示弹出来的注释都写在类/方法/变量声明的上一行//
//说到底除我之外也不一定有人再用这个引擎了...             //
////////////////////////////////////////////////////////////

#include "ZE_Core.h"

#ifdef _DEBUG
#include <vld.h>
#endif // _DEBUG

#include "mygame.h"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


int main(int argc, char* args[])
{
	// 启动参数初始化
	ZeroEngineInitArgs engine_arg;
	engine_arg.stageColor = { 0,0,0 };
	engine_arg.title = u8"我TM不但是一个SDL窗口还是一个TM中英文全半角混合标题的窗口卧槽！!！";
	engine_arg.windowWidth = SCREEN_WIDTH;
	engine_arg.windowHeight = SCREEN_HEIGHT;
	engine_arg.useVSync = true;

	// 创建核心对象
	auto engine = make_shared<ZeroEngine>();

	{
		// 创建用户对象
		auto game = make_shared<mygame>();

		auto temp = std::dynamic_pointer_cast<Game>(game);
		engine->Init(engine_arg, temp);
		engine->Run();
	}

	return 0;
}