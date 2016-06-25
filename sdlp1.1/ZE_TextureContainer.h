#pragma once
#include "ZE_Texture.h"
#include "ZE_DisplayObject.h"

class TextureContainer : public DisplayObject
{
public:
	using DisplayObject::DisplayObject;
	shared_ptr<Texture> mTexture{ nullptr };
	int stageX;
	int stageY;
	int mWidth;
	int mHeight;
	//当前使用的贴图截取数据
	SubTexture nowUsingSubData;

	virtual SubTexture setDefaultSubData();
	virtual void Render();
	void setAlpha(double);
};