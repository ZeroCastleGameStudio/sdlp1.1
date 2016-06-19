#pragma once
#include <iostream>
#include <memory>
#include <deque>
#include <atomic>

using namespace std;

class Fraps;

//颜色结构体，也就设置舞台颜色的时候用用
struct Color
{
	int red;
	int green;
	int blue;
};

//主Game类，用户应继承此类并重写MainLoop和Close方法，然后将一个对象的指针传递给start方法
class Game
{
public:
	virtual void MainLoop() = 0;
	virtual void Close() = 0;
	virtual ~Game() {}
};

//头文件中声明类，cpp中定义类，全都一样
//本文件是ZE的核心类，几乎所有其它类的对象都在此声明
//使用时，继承本类并重载Draw()方法，将绘图代码写在Draw方法中
class ZeroEngine
{
public:
	ZeroEngine();
	~ZeroEngine();
	ZeroEngine(const ZeroEngine&) = delete;
	ZeroEngine(ZeroEngine&&) = delete;

	//舞台颜色，默认为黑
	Color stageColor = { 0, 0, 0 };
	//初始化方法，使用ZE前必须调用此方法
	//该方法调用了本类中的其它两个方法，分别初始化了外部库SDL和SDL_IMAGE
	//该方法会返回一个bool值，若所有模块初始化成功则返回true，否则返回false
	//外部库的报错写在对应方法里
	bool Init(string, int, int, bool, std::string defaultFontFile = "data/ttf/SourceHanSansSC-Normal.otf") const;
	//该方法是ZE的主循环方法，目前是暂用版
	void Start(Game*);
	// 挂号
	size_t getNewDisplayObjectIndex();

private:
	//最后渲染在舞台上，显示一些监视变量
	unique_ptr<Fraps> fraps;
	//SDL的初始化方法，该方法的三个参数分别是窗口标题、窗口宽度、窗口高度
	static bool Init_SDL(string, int, int);
	//初始化了SDL_IMAGE第三方库
	bool Init_SDL_Image(bool) const;
	//初始化了SDL_TTF第三方库
	static bool Init_SDL_ttf();
	static bool Init_SDL_Mixer();
	//清理方法，安全的关闭SDL并退出程序
	void Close() const;
	//用户的游戏对象
	Game* maingame{ nullptr };
	// 显示对象index发放器
	std::atomic_size_t display_object_index{ 0 };
};