#include <iostream>
#include <math.h>
#include "ZE_Core.h"
#include "ZE_Image.h"

using namespace std;

Image::Image(textureStruct texture)
{
	this->mTexture = texture.texture;
	this->nowUsingSubData = texture.subData;
	this->mWidth = nowUsingSubData.width;
	this->mHeight = nowUsingSubData.height;
	this->setBlendMode(SDL_BLENDMODE_BLEND);
}

Image::Image(deque<textureStruct> textureData, unsigned int frameSpeed)
{
	this->isMoveble = true;
	this->subData = textureData;
	this->mTexture = subData[0].texture;
	this->frame = frameSpeed;
	this->nowUsingSubData = subData[0].subData;
	for (auto bigest : subData)
	{
		if (bigest.subData.frameWidth > mWidth)
			mWidth = bigest.subData.frameWidth;
		if (bigest.subData.frameHeight > mHeight)
			mHeight = bigest.subData.frameHeight;
	}
	this->setBlendMode(SDL_BLENDMODE_BLEND);
}

int Image::getWidth() { return mWidth; }
int Image::getHeight() { return mHeight; }

void Image::setBlendMode(SDL_BlendMode mode)
{
	SDL_SetTextureBlendMode(mTexture->getTexture(), mode);
}

void Image::Render()
{
	if (this->isMoveble)
	{
		if (this->playing && frameTimer.getTicks() >= (int)(1000 / frame))
		{
			changeCurrentFrame(this->currentFrame + 1);
			frameTimer.start();
		}
	}

	TextureContainer::Render();
}

void Image::changeCurrentFrame(unsigned int frame)
{
	if (this->isMoveble)
	{
		if (frame == subData.size())
			frame = 0;
		else if (frame > subData.size())
			ZE_error.PopDebugConsole_Error("Frame index out of range");

		this->mTexture = subData[frame].texture;
		this->nowUsingSubData = subData[frame].subData;
		this->currentFrame += 1;
		if (currentFrame == subData.size())
			currentFrame = 0;
	}
	else
		ZE_error.PopDebutConsole_Warning("Can't edit a stable image");
}

void Image::play()
{
	if (this->isMoveble)
	{
		this->playing = true;
		frameTimer.start();
	}
	else
		ZE_error.PopDebutConsole_Warning("Can't edit a stable image");
}
void Image::stop()
{
	if (this->isMoveble)
	{
		this->playing = false;
		frameTimer.reset();
	}
	else
		ZE_error.PopDebutConsole_Warning("Can't edit a stable image");
}

bool Image::playState() { return playing; }