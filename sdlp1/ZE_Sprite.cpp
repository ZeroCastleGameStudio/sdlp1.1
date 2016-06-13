#include "ZE_Core.h"

using namespace std;

void Sprite::Render()
{
	for (unsigned int i = 0; i < addedObjects.size(); i++)
	{
		addedObjects[i]->Render();
	}
}

int Sprite::getWidth()
{
	int tempWidth = 0;
	for (unsigned int i = 0; i < addedObjects.size(); i++)
	{
		if (addedObjects[i]->x + addedObjects[i]->getWidth() > tempWidth)
		{
			tempWidth = addedObjects[i]->x + addedObjects[i]->getWidth();
		}
	}
	return tempWidth;
}

int Sprite::getHeight()
{
	int tempHeight = 0;
	for (unsigned int i = 0; i < addedObjects.size(); i++)
	{
		if (addedObjects[i]->y + addedObjects[i]->getHeight() > tempHeight)
		{
			tempHeight = addedObjects[i]->y + addedObjects[i]->getHeight();
		}
	}
	return tempHeight;
}