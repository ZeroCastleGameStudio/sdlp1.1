#pragma once
#include "ZE_TextureContainer.h"
#include "ZE_Timer.h"

using namespace std;

//Image的参数结构体
struct textureStruct
{
	//接收的贴图
	shared_ptr<Texture> texture;
	//贴图的读取信息
	SubTexture subData;
};

//ZE的图片类，基本与STARING的图片类一致
class Image : public TextureContainer
{
public:
	//储存所有的贴图截取数据
	deque<textureStruct> subData;
	//当前对象是否是动图
	bool isMoveble = false;
	// TODO texture需不需要使用引用模式
	Image(weak_ptr<ZeroEngine> core_engine_weak_ptr, textureStruct texture);
	Image(weak_ptr<ZeroEngine> core_engine_weak_ptr, deque<textureStruct> textureData, unsigned int frameSpeed = 12);
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