#pragma once
#include <string>
#include <deque>
#include "ZE_EventDispatcher.h"

using namespace std;

//所有继承自本类的类都应重写Render方法
//比如sprite中的render就是循环访问数组中的元素并调用render方法
//image的render就是渲染自己的texture
//原来这TM就是个树节点我才意识到
class DisplayObject : public EventDispatcher
{
public:
	//方便用户的变量，完全没卵用
	string name;
	int x = 0;
	int y = 0;
	//宽度缩放倍数，默认为1
	float scaleX = 1;
	//高度缩放倍数，默认为1
	float scaleY = 1;
	//透明度
	float alpah = 1;
	//旋转点，该点指示了贴图以自己的左上为原点的一个坐标，并非舞台坐标
	SDL_Point rotationPoint = { 0, 0 };
	//旋转角度，接收0-360的角度变量
	double rotationAngle = 0;
	//翻转模式
	//无翻转:SDL_FLIP_NONE
	//左右翻转:SDL_FLIP_HORIZONTAL
	//上下翻转:SDL_FLIP_VERTICAL
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	//渲染模式
	SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

	//储存所有使用add方法加入的DO对象
	deque<DisplayObject*> addedObjects;
	//爸爸指针
	DisplayObject* parent = NULL;

	//给我一个DO类幼崽
	void addChild(DisplayObject*);
	//设置我爸爸
	void setParent(DisplayObject*);
	//设置我的真实渲染方块
	SDL_Rect setRenderRect(int, int);
	//删除一个子节点和它所有的子节点
	void removeChild(DisplayObject*, bool disposeIt = true);
	//将我从父节点中删除
	void removeFromParent(bool disposeMe = false);
	//清除该子节点所有资源
	virtual void Render();
	virtual void dispose();
	virtual int getWidth();
	virtual int getHeight();
	virtual ~DisplayObject();
};