#include "ZE_Sprite.h"

using namespace std;

void Sprite::Render()
{
	for (auto&a : addedObjects)
	{
		a.second->Render();
	}
}

int Sprite::getWidth()
{
	int tempWidth = 0;
	// FIXME 这么干会不会有性能问题
	for (auto&a : addedObjects)
	{
		if (a.second->x + a.second->getWidth() > tempWidth)
		{
			tempWidth = a.second->x + a.second->getWidth();
		}
	}
	return tempWidth;
}

int Sprite::getHeight()
{
	int tempHeight = 0;
	for (auto&a : addedObjects)
	{
		if (a.second->y + a.second->getHeight() > tempHeight)
		{
			tempHeight = a.second->y + a.second->getHeight();
		}
	}
	return tempHeight;
}