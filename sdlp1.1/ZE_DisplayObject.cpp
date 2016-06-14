#include <iostream>
#include "ZE_Core.h"

using namespace std;

void DisplayObject::addChild(DisplayObject* object)
{
	object->setParent(this);
	addedObjects.push_back(object);
}

void DisplayObject::setParent(DisplayObject* mum)
{
	this->parent = mum;
}

SDL_Rect DisplayObject::setRenderRect(int childWidth, int childHeight)
{
	//含金量极高的一段代码
	//首先，遍历父节点，获取我应该缩放的比例和所有父亲的值
	int parentX = 0;
	int parentY = 0;
	float parentScaleX = 1;
	float parentScaleY = 1;
	if (parent != NULL)
	{
		DisplayObject* mum = parent;
		while (true)
		{
			//先获取现在的爸爸的值
			parentX += mum->x;
			parentY += mum->y;
			parentScaleX *= mum->scaleX;
			parentScaleY *= mum->scaleY;
			//如果当前父级使用了水平翻转的话
			if (mum->flip == SDL_FLIP_HORIZONTAL)
			{
				//给子级也设置成水平翻转
				flip = SDL_FLIP_HORIZONTAL;
				//当前的舞台坐标减去一个负的我的X*2
				//然后应该再减去一个宽度，不过IMAGE子类已经子级减了
				parentX += 0 - (x * 2);
			}
			//垂直翻转，同上
			else if (mum->flip == SDL_FLIP_VERTICAL)
			{
				flip = SDL_FLIP_VERTICAL;
				parentY += 0 - (y * 2);
			}
			//旋转，获得父级的原点坐标并赋值给自己
			if (mum->rotationAngle != 0)
			{
				this->rotationPoint.x = 0 - x;
				this->rotationPoint.y = 0 - y;
				this->rotationAngle = mum->rotationAngle;
			}
			//需要说明的是，ZENG还差得远，只有我一个人维护啊大哥。
			//所以对于旋转+翻转+缩放这一块支持不是很好。
			//首先因为SDL原生面向过程，封装为面向对象本身就需要更多更复杂的算法。
			//说实在的其实也没有哪个底层图形API支持那么丰富
			//缩放现在应该没什么问题，基本上应该会按比例正确缩放
			//翻转有些缺陷，若有如下代码
			//spr1.add(spr2); spr2.x = 100; spr1.flip;
			//这是无法正常实现的，会错位。
			//具体需要更详尽的分析和更复杂的算法
			//所以尽量不要在需要翻转的spr里再套一层spr
			//如果一定要的话，那也不要再另外设置坐标
			//因为SDL本身也不支持负值scale，所以没办法，这里只能暂时坑着了
			//旋转也有上面的问题，子级不能嵌套spr。
			//另外，千万不要把动态image旋转，整个抽搐成狗
			//动态image获得坐标的算法已经很复杂了，再加的话就是向量级运算了，out of my hand

			//这两个问题构成的原因基本一致，如果把parentX加入算式的话，它们就会以舞台坐标为原点进行翻转或旋转
			//按说当前循环中计算的parentX就应该是进行操作的那一层父级的数据，但是不是，它会一直上朔到stage
			//以翻转举例，具体流程应该是这样
			//首先，询问父级是否翻转，如果为否则不执行，继续下一次循环上朔父级
			//假如某一级被设置为翻转了，那首先应该把该父级的所有子级都设置为翻转(当前的算法中没有执行这一步，
			//尝试在sprite的render方法中加入执行本方法的代码，出现了更严重的错位，应该是算法没有考虑周全)
			//然后，获取我相对于该父级的坐标，比如水平翻转，就应该获取我在该父级坐标系里的X，然后
			//获得这个坐标的负值，即0 - （相对的X*2），再由image自己减去一个自己的宽度就可以正确翻转了
			//理论上这个值就应该是parentX，因为本方法是从子级向上问，所以当执行到这里的时候，parentX应该就是
			//我相对于这个父级的X，当继续上朔之后最终会获得相对于舞台的X，但当继续上朔的时候，更高层的父级
			//没有被翻转，所以应该就不会再继续执行翻转算法了。可是不知道为什么最终都会上朔到舞台
			//这就是问题所在了，如果将来一定要解决这个问题的话，就断点测试一点点的分析吧

			//另外，建议最好不要混用旋转+翻转+缩放，可能性太多了没办法一一测试太耽误时间了
			//基本翻转也是这个原因
			//可能会出现更多的坐标计算错误，所以还是不要混用了

			//如果现在的爸爸没有爸爸了，就退出
			if (mum->parent == NULL)
			{
				break;
			}
			//否则就把现在的爸爸暂存，然后把现在的爸爸换成爸爸的爸爸
			else
			{
				DisplayObject* tempMum = mum;
				mum = tempMum->parent;
			}
		}
	}
	SDL_Rect renderRect =
	{
		(int)((x+parentX)*parentScaleX),
		(int)((y+parentY)*parentScaleY),
		(int)(childWidth*scaleX*parentScaleX),
		(int)(childHeight*scaleY*parentScaleY)
	};

	return renderRect;
}

void DisplayObject::removeFromParent(bool disposeMe)
{
	if (parent == NULL)
	{
		ZE_error.PopDebutConsole_Warning("DisplayObject:" + this->name + " do not have a parent!");
	}
	else
	{
		parent->removeChild(this, disposeMe);
	}
}

void DisplayObject::removeChild(DisplayObject* targetChild, bool disposeIt)
{
	/*if (addedObjects.size() != 0)
	{
		bool finded = false;
		for (unsigned int i = 0; i < addedObjects.size(); i++)
		{
			if (targetChild == addedObjects[i])
			{
				finded = true;
				if (disposeIt)
				{
					addedObjects[i]->dispose();
					delete(addedObjects[i]);
					addedObjects[i] = NULL;
				}
				deque <DisplayObject*>::iterator ite;
				ite = addedObjects.begin();
				addedObjects.erase(ite + i);
				break;
			}
		}
		if (finded == false)
		{
			error.PopDebugConsole_Error("Can't find Child:" + targetChild->name);
		}
	}*/

	auto it = find(addedObjects.begin(), addedObjects.end(), targetChild);
	if (it != addedObjects.end())
	{
		if (disposeIt)
		{
			(*it)->dispose();
			delete(*it);
		}
		addedObjects.erase(it);
	}

	else
	{
		ZE_error.PopDebugConsole_Error("Can't find Child:" + targetChild->name);
	}
}

void DisplayObject::dispose()
{
	/*if (addedObjects.size() != 0)
	{
		while (addedObjects.size() != 0)
		{
			addedObjects[0]->dispose();
			delete(addedObjects[0]);
			addedObjects[0] = NULL;
			addedObjects.erase(addedObjects.begin());
		}
	}*/

	for (auto obj : addedObjects)
	{
		obj->dispose();
		delete(obj);
		obj = NULL;
	}
	addedObjects.clear();
}

void DisplayObject::Render() {}
int DisplayObject::getWidth() { return 0; }
int DisplayObject::getHeight() { return 0; }

DisplayObject::~DisplayObject()
{
	dispose();
}