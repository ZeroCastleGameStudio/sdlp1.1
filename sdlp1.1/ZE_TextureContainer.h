#pragma once
#include "ZE_Texture.h"
#include "ZE_DisplayObject.h"

class TextureContainer : public DisplayObject
{
public:
	Texture* mTexture = NULL;
	int stageX;
	int stageY;
	int mWidth;
	int mHeight;
	//当前使用的贴图截取数据
	SubTexture nowUsingSubData;

	virtual SubTexture setDefaultSubData();
	virtual void Render();
	virtual void freeTexture();
	void setAlpha(float);
};