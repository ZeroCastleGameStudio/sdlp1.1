#pragma once
#include <string>
#include <deque>
#include <SDL.h>

using namespace std;

//子贴图结构体，这里的变量仅用来定义子贴图的数据结构
//另外，整体的贴图也可以使用这个结构体
struct SubTexture
{
	//小贴图名称，索引时的唯一标记
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

//贴图类，所有舞台元素如image等都将包含至少一个texture
//texture由assetManager的对象统一管理
//texture对象能且仅能由assetManager对象进行创建、管理、销毁
class Texture
	:enable_shared_from_this<Texture>
{
public:
	~Texture();

	//作为texture的文件名
	string name;

	//接收5个参数，该texture的名称，该texture的贴图指针，
	//该贴图的宽，高，和该texture的sub的结构体
	void Init(string, SDL_Texture*, int, int, deque<SubTexture> = {});
	//获取宽度
	int getWidth();
	//获取高度
	int getHeight();
	//获取贴图
	SDL_Texture* getTexture();

	//这个数据用来保存整个贴图的读取信息
	SubTexture mySubInfo;
	//保存上面结构体的数组，由AM管理和读取
	//若没有小贴图则该数组为空
	deque<SubTexture> subTextures;

private:
	//保存贴图
	SDL_Texture* mTexture{ NULL };
	//宽度
	int mWidth;
	//高度
	int mHeight;
};