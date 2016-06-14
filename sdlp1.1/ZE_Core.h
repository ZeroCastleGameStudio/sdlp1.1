#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <deque>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "ZE_Error.h"
#include "ZE_Sprite.h"
#include "ZE_Event.h"
#include "ZE_Controller.h"
#include "ZE_Fraps.h"
#include "ZE_Font.h"

using namespace std;

//[Goble]ZE�汾��
extern string ZE_version;
//[Goble]����SDL�����ָ��
extern SDL_Window* g_ZE_Window;
//[Goble]����SDL��Surface��ָ��
extern SDL_Surface* g_ZE_MainSurface;
//[Goble]������Ҳ��֪�������⵽�׸ý�ʲô���ͽ���Ⱦ�����ˣ���������Ⱦ�����󶨵���window
extern SDL_Renderer* g_ZE_MainRenderer;
//[Goble]stage����Ψһ
extern Sprite ZE_stage;
//[Goble]error����(Ӧ����Ψһ�������������Ҳ��private)
extern Error ZE_error;
//[Goble]�¼�����������Ψһ
extern EventManager ZE_eventHandler;
//[Goble]�˳���ѭ�����ж�����
extern bool ZE_QUIT_MAIN_LOOP;
//[Goble]�������е��ֱ�ָ��
extern deque<unique_ptr<Controller>> ZE_Controllers;
//[Goble]ϵͳĬ������
extern shared_ptr<Font> defaultFont;


//��ɫ�ṹ�壬Ҳ��������̨��ɫ��ʱ������
struct Color
{
	int red;
	int green;
	int blue;
};

//��Game�࣬�û�Ӧ�̳д��ಢ��дMainLoop��Close������Ȼ��һ�������ָ�봫�ݸ�start����
class Game
{
public:
	virtual void MainLoop() {};
	virtual void Close() {};
};

//ͷ�ļ��������࣬cpp�ж����࣬ȫ��һ��
//���ļ���ZE�ĺ����࣬��������������Ķ����ڴ�����
//ʹ��ʱ���̳б��ಢ����Draw()����������ͼ����д��Draw������
class ZeroEngine
{
public:
	//��̨��ɫ��Ĭ��Ϊ��
	Color stageColor = { 0, 0, 0 };
	//��ʼ��������ʹ��ZEǰ������ô˷���
	//�÷��������˱����е����������������ֱ��ʼ�����ⲿ��SDL��SDL_IMAGE
	//�÷����᷵��һ��boolֵ��������ģ���ʼ���ɹ��򷵻�true�����򷵻�false
	//�ⲿ��ı���д�ڶ�Ӧ������
	bool Init(string, int, int, bool, std::string defaultFontFile = "data/ttf/SourceHanSansSC-Normal.otf");
	//�÷�����ZE����ѭ��������Ŀǰ�����ð�
	void Start(Game*);

private:
	//�����Ⱦ����̨�ϣ���ʾһЩ���ӱ���
	Fraps fraps;
	//SDL�ĳ�ʼ���������÷��������������ֱ��Ǵ��ڱ��⡢���ڿ�ȡ����ڸ߶�
	bool Init_SDL(string, int, int);
	//��ʼ����SDL_IMAGE��������
	bool Init_SDL_Image(bool);
	//��ʼ����SDL_TTF��������
	bool Init_SDL_ttf();
	bool Init_SDL_Mixer();
	//����������ȫ�Ĺر�SDL���˳�����
	void Close();
	//�û�����Ϸ����
	Game* maingame;
};