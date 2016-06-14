#pragma once
#include "ZE_Texture.h"
#include "ZE_Image.h"
#include "ZE_Font.h"
#include "ZE_Sound.h"

using namespace std;

//����starling����Դ������
//���û���ʹ����Դʱ��Ӧ��������һ������Ķ���
class AssetManager
{
public:
	~AssetManager();
	//�÷�������һ��xml�ļ���ַ���ļ��б�ע�˸ö�����Ҫ�����ȫ��ͼƬ�ļ��ĵ�ַ
	//xml�ļ���ʽ����
	//<?xml version="1.0"?>
	//    <resourses name="testxml">
	//    <texture name="background" path="data/images/background.png" xml="data/images/tinylight.xml">
	//    <texture name="pngtest" path="data/images/pngtest.png" xml="data/images/tinylight.xml">
	//    <texture name="foo" path="data/images/foo.png" xml="data/images/tinylight.xml">
	//</resourses>
	void Init(string xmlPath);
	//��ȡһ����ע���texture������subtexture
	textureStruct getTexture(string name);
	//��ȡ��ע���������ɵ�һ����ͼ
	SDL_Texture* getTTFTexture(string text, string name, int size, SDL_Color color, int* width, int* height,
		unsigned int effectLevel = 0);
	//��ȡһ����������
	shared_ptr<Sound> getSound(string name);
	//��ȡ������ע���texture
	deque<textureStruct> getTextures(string partOfName);
	//�÷�����һ��surfaceָ��ת��ΪSDLtextureָ�벢����
	static SDL_Texture* Surface2SDLTexture(SDL_Surface*, int*, int*);
	//���
	void dispose();

private:
	//���е���ͼ������AM����ͳһ���д������������٣�������Ϊ����������ע����ͼ������
	// �Ѿ�ʹ������ָ���Զ�������Դ������
	deque<shared_ptr<Texture>> TEXTURES;
	//�����������
	deque<shared_ptr<Font>> FONT;
	//������������
	deque<shared_ptr<Sound>> SOUNDS;

	//��ȡ����������ͼ���ݵķ���������XML�ļ���ַ��Ϊ����
	deque<SubTexture> SubXmlReader(string xml);
	//����ͼƬ�ķ���������ͼƬӲ�̵�ַ��Ϊ����
	void LoadTexture(string name, string path, string xml);
	//��������
	void LoadTTF(string name, string path);
	//��������
	void LoadSound(bool isMusic, string name, string path);
	//�ֱ�ͼƬ��ʽ�ķ�����������һ��Surfaceָ��
	//�û����õ��ô˷���
	SDL_Surface* ImageReader(string, int extNameLength = 3);
	//��ȡ������һ��FONT����
	shared_ptr<Font> getFont(string);

	//ɾ����һtexture������ֻ������ķ�������
	void DeleteTexture(int index);
	//ɾ������ע���Texture��С��ʹ��
	void DeleteAllTextures();
	//ɾ����һfont������ֻ������ķ�������
	void DeleteFont(int index);
	//ɾ������ע���font��С��ʹ��
	void DeleteAllFonts();
	//ɾ����һsound������ֻ������ķ�������
	void DeleteSound(int index);
	//ɾ������ע���sound��С��ʹ��
	void DeleteAllSounds();
};