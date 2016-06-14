#pragma once
#include <string>
#include <deque>
#include <SDL.h>

using namespace std;

//����ͼ�ṹ�壬����ı�����������������ͼ�����ݽṹ
//���⣬�������ͼҲ����ʹ������ṹ��
struct SubTexture
{
	//С��ͼ���ƣ�����ʱ��Ψһ���
	string name = "";
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	int frameX = 0;
	int frameY = 0;
	int frameWidth = 0;
	int frameHeight = 0;
};

//��ͼ�࣬������̨Ԫ����image�ȶ�����������һ��texture
//texture��assetManager�Ķ���ͳһ����
//texture�������ҽ�����assetManager������д�������������
class Texture
	:enable_shared_from_this<Texture>
{
public:
	~Texture();

	//��Ϊtexture���ļ���
	string name;

	//����5����������texture�����ƣ���texture����ͼָ�룬
	//����ͼ�Ŀ��ߣ��͸�texture��sub�Ľṹ��
	void Init(string, SDL_Texture*, int, int, deque<SubTexture> = {});
	//��ȡ���
	int getWidth();
	//��ȡ�߶�
	int getHeight();
	//��ȡ��ͼ
	SDL_Texture* getTexture();

	//���������������������ͼ�Ķ�ȡ��Ϣ
	SubTexture mySubInfo;
	//��������ṹ������飬��AM����Ͷ�ȡ
	//��û��С��ͼ�������Ϊ��
	deque<SubTexture> subTextures;

private:
	//������ͼ
	SDL_Texture* mTexture{ NULL };
	//���
	int mWidth;
	//�߶�
	int mHeight;
};