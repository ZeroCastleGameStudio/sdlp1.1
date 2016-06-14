#pragma once
#include "ZE_DisplayObject.h"

using namespace std;

//模仿starling的轻量级sprite类
class Sprite : public DisplayObject
{
public:
	virtual int getWidth();
	virtual int getHeight();

	virtual void Render();
};