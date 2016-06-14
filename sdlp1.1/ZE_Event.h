#pragma once
#include <deque>
#include <functional>
#include <SDL.h>
#include "ZE_EventDispatcher.h"

using namespace std;

//事件统筹类
//所有eventDispatcher对象注册监听时，都调用该类的add方法
//监听方法的指针将会被保存在统筹数组里，捕获事件后就对比然后挨个调用
class EventManager
{
public:
	void handleEvent();
	void addEventFunction(SDL_EventType type, EventDispatcher*, function<void(SDL_Event)>);
	void removeEventOfObject(SDL_EventType type, EventDispatcher*);
	void removeAllEventOfObject(EventDispatcher*);

	/*在经历了（很大的）一番周折后，最终（不幸的）确定了event的修改方案
	参照starling的方法，当注册一个事件的时候，应该传入一个函数。
	使用C++的话，相对应的需要传入函数指针。
	然而这个做法的局限性很大，最大的一个局限性就是，当目标函数为成员函数时无法调用，
	这是因为Cpp中调用一般函数与成员函数的方法不同。
	因为这个办法完全不够面向对象，所以开始寻找解决方案。
	
	之后知道了如果在声明形参的时候加上一个className::即可正确使用，然而这个办法不适合我。
	因为事实上我传入的方法是eventdispatcher子类的方法，并不真正的属于ED。
	在点拨之下知道了C11的新特性function。
	
	尝试使用function传递方法。然而function包装成员函数很复杂，所以改为使用lambda表达式，即匿名函数。
	如此一来实现了正确的调用和传递，但是因为function天生动态，无法进行相等判断，
	所以用户便无法使用注册时的函数来进行移除。使用指针也不行，function无法通过指针正确调用。
	
	权衡再三以后，决定移除removeEventListener方法。用户可以注册不同的方法监听同一个事件，
	但移除的时候只能按事件类型移除所有监听事件。
	
	有一个终极解决方案。给EventDispatcher声明数个(分门别类)或一个处理事件的虚函数，
	然后add方法只传递给eventHandler一个this指针。
	所有想注册事件的元素都重写该虚函数，然后eventHandler将会按事件类型调用所有已注册的函数。
	这个方法最稳定最安全，但是最不灵活，因为每一个事件都要对应一个虚函数，
	而我的方法可以直接使用原生SDL事件，也就是只要SDL支持我的方法就支持。
	cocos用的就是这个办法，讨厌cocos
	写在这里当做备选方案
	
	BTW，今天落枕了，脖子疼得不行*/
	// TODO 

	/*也不知道写到哪，就写到这好了
	SDL很基础，所以ZE目前对鼠标点击事件的支持不是很好
	因为所有的事件都是由该类统一调度处理的，也就是说
	当一个鼠标事件被触发时，它只会向所有已注册的方法派发此事件
	也就是说，不管图像的表现如何，即使A元素被B元素盖着，A元素也会接收到鼠标事件
	换句话说，就是不存在“遮住鼠标”的概念
	这个东西的实现感觉会很复杂，因为以目前的状况来看，它只能在触发时由子至父遍历一遍所有舞台元素，
	才能知道谁盖住了谁。或者反向遍历events数组，然后只派发一个之类的
	总之目前先不做了*/

	// Jeremie:
	// 鼠标点击事件肯定要处理遮蔽的问题，常见的方法是对所有对象以遮挡方式构建堆叠树，然后在树上遍历查找响应事件的对象
	// 但这个方法不适合这里，我觉得可以现对图像分层，然后按层筛选来获得响应事件的对象，
	// 或者给所有需要响应点击事件的对象如同构建碰撞箱一样构造一个事件响应箱，然后直接查看点击点处有哪些响应箱，然后根据层叠关系来发送事件

private:
	//保存所有的事件
	deque<EventData> AllEvents;
};