#pragma once
#include "ZE_TextureContainer.h"
#include "ZE_Timer.h"

using namespace std;

//Image的参数结构体
struct textureStruct
{
	Texture* texture;//接收的贴图
	SubTexture subData;//贴图的读取信息
};

//ZE的图片类，基本与STARING的图片类一致
class Image : public TextureContainer
{
public:
	//储存所有的贴图截取数据
	deque<textureStruct> subData;
	//当前对象是否是动图
	bool isMoveble = false;
	Image(textureStruct);
	Image(deque<textureStruct>, unsigned int frameSpeed = 12);
	virtual int getWidth();
	virtual int getHeight();
	//SDL的blendmode似乎并不多，大概好多都能自己实现
	void setBlendMode(SDL_BlendMode);
	virtual void Render();
	bool playState();
	void changeCurrentFrame(unsigned int);
	void play();
	void stop();

private:
	int currentFrame = 0;
	bool playing = false;
	unsigned int frame;
	Timer frameTimer;
};