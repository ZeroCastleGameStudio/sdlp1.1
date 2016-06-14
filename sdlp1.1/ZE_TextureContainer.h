#pragma once
#include "ZE_Texture.h"
#include "ZE_DisplayObject.h"

class TextureContainer : public DisplayObject
{
public:
	shared_ptr<Texture> mTexture{ nullptr };
	int stageX;
	int stageY;
	int mWidth;
	int mHeight;
	//��ǰʹ�õ���ͼ��ȡ����
	SubTexture nowUsingSubData;

	virtual SubTexture setDefaultSubData();
	virtual void Render();
	virtual void freeTexture();
	void setAlpha(float);
};