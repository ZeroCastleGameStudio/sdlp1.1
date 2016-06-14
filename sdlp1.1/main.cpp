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

ZeroEngine ZEROENG;
mygame game;

int main(int argc, char* args[])
{
	ZEROENG.Init(u8"我TM不但是一个SDL窗口还是一个TM中英文全半角混合标题的窗口卧槽！!！", SCREEN_WIDTH, SCREEN_HEIGHT, true);

	//可以设置舞台默认颜色，也没什么卵用
	ZEROENG.stageColor = { 0, 0, 0 };

	game.Init();

	ZEROENG.Start(&game);

	return 0;
}