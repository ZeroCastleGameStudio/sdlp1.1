#include <SDL_image.h>
#include "ZE_Core.h"

using namespace std;

//[Goble]ZE�汾��
string ZE_version = "1.0.0";
//[Goble]����SDL�����ָ��
SDL_Window* g_ZE_Window = NULL;
//[Goble]����SDL��Surface��ָ��
SDL_Surface* g_ZE_MainSurface = NULL;
//[Goble]������Ҳ��֪�������⵽�׸ý�ʲô���ͽ���Ⱦ�����ˣ���������Ⱦ�����󶨵���window
SDL_Renderer* g_ZE_MainRenderer = NULL;
//[Goble]stage����Ψһ
Sprite ZE_stage;
//[Goble]error����(Ӧ����Ψһ�������������Ҳ��private)
Error ZE_error;
//[Goble]�¼�����������Ψһ
Event ZE_eventHandler;
//[Goble]�˳���ѭ�����ж�����
bool ZE_QUIT_MAIN_LOOP = false;
//[Goble]�������е��ֱ�ָ��
deque<unique_ptr<Controller>> ZE_Controllers;
//[Goble]ϵͳĬ������
shared_ptr<Font> defaultFont;


bool ZeroEngine::Init(string Title, int windowWidth, int windowHeight, bool useVSync, std::string defaultFontFile)
{
	bool success = true;
	if (!Init_SDL(Title, windowWidth, windowHeight))
	{
		success = false;
	}
	else if (!Init_SDL_Image(useVSync))
	{
		success = false;
	}
	else if (!Init_SDL_ttf())
	{
		success = false;
	}
	else if (!Init_SDL_Mixer())
	{
		success = false;
	}

	// ��������
	defaultFont = make_shared<Font>("default", defaultFontFile);

	function<void(SDL_Event)> addJoyStick = [](SDL_Event evt)->void
	{
		ZE_Controllers.emplace_back(new Controller(SDL_NumJoysticks() - 1));
	};

	function<void(SDL_Event)> removeJoyStick = [](SDL_Event evt)->void
	{
		//delete(ZE_Controllers[evt.jdevice.which]);
		//ZE_Controllers[evt.jdevice.which].reset();
		// FIXME ����ʹ���±���з��ʲ�����ĳ�������ըô
		ZE_Controllers.erase(ZE_Controllers.begin() + evt.jdevice.which);
	};
	ZE_stage.addEventListener(SDL_JOYDEVICEADDED, addJoyStick);
	ZE_stage.addEventListener(SDL_JOYDEVICEREMOVED, removeJoyStick);

	return success;
}

bool ZeroEngine::Init_SDL(string Title, int windowWidth, int windowHeight)
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_AUDIO) < 0)
	{
		//Ŀǰ������VIDEO flag����ֻ��ʼ��SDL����Ƶģ��
		ZE_error.PopDebugConsole_SDLError("SDL could not initialize!");
		//����ʼ��ʧ�������error�൯�������Ժ�һ��
		success = false;
	}
	else
	{
		const char* tit = Title.c_str();
		//��ΪSDL�Ǵ�C����������Ҫ��stringת��ΪC��char�����㲻���ô�C���������������ֻ���ܴ�C
		g_ZE_Window = SDL_CreateWindow(tit, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
		//�������壺���ڱ��⣬���ڳ���λ�õ�X����(�ñ������岻��)�����ڳ���λ�õ�Y����(�ñ������岻��)�����ڿ�ȣ�
		//���ڸ߶ȣ���������(���������ʾ�����򿪴��ڣ����Ǿ���������ʲô�������滻��ʲô���滻����ʲô���ö���֪��)
		if (g_ZE_Window == NULL)
		{
			ZE_error.PopDebugConsole_SDLError("Window could not be created!");
			success = false;
		}
		else
		{
			g_ZE_MainSurface = SDL_GetWindowSurface(g_ZE_Window);
			//ץȡ���ڵ���Surface�����еĻ��ƶ����������surface��
		}
	}

	return success;
}

bool ZeroEngine::Init_SDL_Image(bool useVSync)
{
	bool success = true;

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) == imgFlags))
	{
		//����ֻ��ʼ����PNG��IMG�ĳ�ʼ���������᷵�ض�Ӧ��FLAG����ʾ�ɹ�
		//�в�֪������������������������ã���������ô��������������ʲô
		//����һ��SDL_IMAGE֧�ֶ��ָ�ʽ�治�Ǹǵģ����˳���JPG PNG֮���Ȼ��֧��GIF������ټ�������һЩ�����еĸ�ʽ
		//BTW��IMG_INIT_PNG == 2
		ZE_error.PopDebugConsole_SDL_ImageError("SDL_image could not initialize!");
		success = false;
	}

	int renderFlag;
	if (useVSync)
		renderFlag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	else
		renderFlag = SDL_RENDERER_ACCELERATED;
	g_ZE_MainRenderer = SDL_CreateRenderer(g_ZE_Window, -1, renderFlag);
	//��ʼ��������Ⱦ��
	if (g_ZE_MainRenderer == NULL)
	{
		ZE_error.PopDebugConsole_SDLError("Renderer could not be created!");
		success = false;
	}
	else
	{
		SDL_SetRenderDrawColor(g_ZE_MainRenderer, stageColor.red, stageColor.green, stageColor.blue, 1);
		//������Ⱦ���Ļ�����ɫ
		//Ҫ����Ⱦ����Ĭ��ͼ�εĻ�����SDL_RENDERDRAWRECT�����Ҫ���ϵ��л�������ɫ��
		//����Ҫ��̨�ǰ�ɫ�ģ������Ǻ�ɫ�ģ���Ҫ�Ȼ�����̨���ٻ��췽�飬��һ��ѭ�����л��ذ�ɫ
	}

	return success;
}

bool ZeroEngine::Init_SDL_ttf()
{
	bool success = true;
	if (TTF_Init() == -1)
	{
		ZE_error.PopDebugConsole_SDL_ttfError("SDL_ttf could not initialize!");
		success = false;
	}
	return success;
}

bool ZeroEngine::Init_SDL_Mixer()
{
	bool success = true;
	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) < 0)
	{
		success = false;
		ZE_error.PopDebugConsole_SDL_MixerError("SDL_mixer could not initialize!");
	}
	else
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			success = false;
			ZE_error.PopDebugConsole_SDL_MixerError("cuold not open audio on SDL_mixer!");
		}
	}

	return success;
}

void ZeroEngine::Start(Game* userGame)
{
	maingame = userGame;
	fraps.setUp();
	while (!ZE_QUIT_MAIN_LOOP)
	{

		//�����¼�
		ZE_eventHandler.handleEvent();
		//���
		SDL_RenderClear(g_ZE_MainRenderer);
		//���߼�
		maingame->MainLoop();
		//�ٻ�
		ZE_stage.Render();
		//��frapsִ���߼�
		fraps.setText();
		fraps.Render();
		//�ı��Ĭ����ɫ
		SDL_SetRenderDrawColor(g_ZE_MainRenderer, stageColor.red, stageColor.green, stageColor.blue, 1);
		//��ˢ��
		SDL_RenderPresent(g_ZE_MainRenderer);
	}
	Close();
}

void ZeroEngine::Close()
{
	//for (auto& con : ZE_Controllers)
	//{
	//	//delete(con);
	//	//con = NULL;
	//	con.reset();
	//}
	ZE_Controllers.clear();

	//������̨
	ZE_stage.dispose();
	//�����û��Ĺرշ���
	maingame->Close();

	SDL_DestroyWindow(g_ZE_Window);
	SDL_DestroyRenderer(g_ZE_MainRenderer);
	//ɾ����Ⱦ��
	g_ZE_MainRenderer = NULL;
	//ɾ��SDL����
	g_ZE_Window = NULL;
	//���뽫ָ����շ����Ұָ��
	Mix_CloseAudio();
	while (Mix_Init(0))
	{
		Mix_Quit();
	}
	//���Ĭ������  
	//��Ϊ�����ʹ����SDL�ռ䣬Ȼ��������ȫ�ֱ����������ڱ����泤��������Ҫ��SDL�ر�ǰ�ֶ��ͷ�
	//�����Զ��ͷŻᷢ����SDL�رպ�exe����ǰ
	defaultFont.reset();

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	//�˳�
}