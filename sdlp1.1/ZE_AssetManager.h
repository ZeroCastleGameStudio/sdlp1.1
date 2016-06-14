#pragma once
#include "ZE_Texture.h"
#include "ZE_Image.h"
#include "ZE_Font.h"
#include "ZE_Sound.h"

using namespace std;

//仿制starling的资源管理器
//当用户想使用资源时，应首先声明一个本类的对象
class AssetManager
{
public:
	~AssetManager();
	//该方法接收一个xml文件地址，文件中标注了该对象需要载入的全部图片文件的地址
	//xml文件格式如下
	//<?xml version="1.0"?>
	//    <resourses name="testxml">
	//    <texture name="background" path="data/images/background.png" xml="data/images/tinylight.xml">
	//    <texture name="pngtest" path="data/images/pngtest.png" xml="data/images/tinylight.xml">
	//    <texture name="foo" path="data/images/foo.png" xml="data/images/tinylight.xml">
	//</resourses>
	void Init(string xmlPath);
	//获取一个已注册的texture，包含subtexture
	textureStruct getTexture(string name);
	//获取已注册字体生成的一个贴图
	SDL_Texture* getTTFTexture(string text, string name, int size, SDL_Color color, int* width, int* height,
		unsigned int effectLevel = 0);
	//获取一个声音对象
	Sound* getSound(string name);
	//获取数个已注册的texture
	deque<textureStruct> getTextures(string partOfName);
	//该方法将一个surface指针转换为SDLtexture指针并返回
	static SDL_Texture* Surface2SDLTexture(SDL_Surface*, int*, int*);
	//清除
	void dispose();

private:
	//所有的贴图对象都由AM对象统一进行创建、管理、销毁，该数组为保存所有已注册贴图的数组
	deque<Texture*> TEXTURES;
	//储存字体对象
	deque<Font*> FONT;
	//储存声音对象
	deque<Sound*> SOUNDS;

	//读取并返回子贴图数据的方法，接收XML文件地址作为参数
	deque<SubTexture> SubXmlReader(string xml);
	//载入图片的方法，接收图片硬盘地址作为参数
	void LoadTexture(string name, string path, string xml);
	//载入字体
	void LoadTTF(string name, string path);
	//载入声音
	void LoadSound(bool isMusic, string name, string path);
	//分辨图片格式的方法，将返回一个Surface指针
	//用户不该调用此方法
	SDL_Surface* ImageReader(string, int extNameLength = 3);
	//获取并返回一个FONT对象
	Font* getFont(string);

	//删除单一texture，基本只被下面的方法调用
	void DeleteTexture(int index);
	//删除所有注册的Texture，小心使用
	void DeleteAllTextures();
	//删除单一font，基本只被下面的方法调用
	void DeleteFont(int index);
	//删除所有注册的font，小心使用
	void DeleteAllFonts();
	//删除单一sound，基本只被下面的方法调用
	void DeleteSound(int index);
	//删除所有注册的sound，小心使用
	void DeleteAllSounds();
};