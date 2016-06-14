#pragma once
#include "ZE_TextureContainer.h"
#include "ZE_Timer.h"

using namespace std;

//Image�Ĳ����ṹ��
struct textureStruct
{
	shared_ptr<Texture> texture;//���յ���ͼ
	SubTexture subData;//��ͼ�Ķ�ȡ��Ϣ
};

//ZE��ͼƬ�࣬������STARING��ͼƬ��һ��
class Image : public TextureContainer
{
public:
	//�������е���ͼ��ȡ����
	deque<textureStruct> subData;
	//��ǰ�����Ƿ��Ƕ�ͼ
	bool isMoveble = false;
	Image(textureStruct);
	Image(deque<textureStruct>, unsigned int frameSpeed = 12);
	virtual int getWidth();
	virtual int getHeight();
	//SDL��blendmode�ƺ������࣬��źö඼���Լ�ʵ��
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