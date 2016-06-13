#include "ZE_Core.h"
#include "ZE_TextureContainer.h"

using namespace std;

SubTexture TextureContainer::setDefaultSubData()
{
	SubTexture data;
	data.width = mWidth;
	data.height = mHeight;
	data.frameWidth = mWidth;
	data.frameHeight = mHeight;
	return data;
}

void TextureContainer::setAlpha(float alphaNum)
{
	Uint8 tempalpha = (int)(alphaNum * 255);
	SDL_SetTextureAlphaMod(mTexture->getTexture(), tempalpha);
}

void TextureContainer::Render()
{
	//调用父类的方法设置相对于舞台主坐标的位置，详见父类
	//传入的参数应该是当前帧的WH，当使用静态图片时，该二值与mWmH相等
	SDL_Rect renderRect = setRenderRect(nowUsingSubData.width, nowUsingSubData.height);

	//如果使用了水平翻转，或者父级中的某一级设置了翻转的话（经过上面的方法调用我自己也被设置了翻转）
	if (flip == SDL_FLIP_HORIZONTAL)
	{
		//首先，舞台位置的X应该参照最下面的ELSE执行，但设置了翻转，就由下面的代码执行
		//当设置了水平翻转时，使用的舞台的X应加上动画的总宽度减去下面写的被切掉的透明部分，然后再减去该帧的实际宽度
		//就得到了另一半被剪掉的透明部分。
		//当图片为静态时，frameX为0，subdata.width等于mW，所以该算法将获得0
		renderRect.x += mWidth - abs(nowUsingSubData.frameX) - nowUsingSubData.width;
		//然后，图片将基于原点X轴进行翻转，只需减去mW即可
		renderRect.x -= mWidth;
		renderRect.y += abs(nowUsingSubData.frameY);
	}
	//如果使用了垂直翻转，算法基本同上
	else if (flip == SDL_FLIP_VERTICAL)
	{
		renderRect.x += abs(nowUsingSubData.frameX);
		renderRect.y += mHeight - abs(nowUsingSubData.frameY) - nowUsingSubData.height;
		renderRect.y -= mHeight;
	}
	else
	{
		//舞台位置X应该加上这一帧的图片被截取掉的透明部分，以获得正确的X
		//frameX是由texturePackager软件切掉透明部分后自动生成的，该值为负值，表示切去了多少
		//静态图片时，该值为0
		renderRect.x += abs(nowUsingSubData.frameX);
		renderRect.y += abs(nowUsingSubData.frameY);
	}
	SDL_Rect subTextureClip = { nowUsingSubData.x, nowUsingSubData.y, nowUsingSubData.width, nowUsingSubData.height };
	setAlpha(this->alpah);
	SDL_RenderCopyEx(g_ZE_MainRenderer, mTexture->getTexture(), &subTextureClip, &renderRect, rotationAngle, &rotationPoint, flip);
	stageX = renderRect.x;
	stageY = renderRect.y;
}

void TextureContainer::freeTexture()
{
	if (mTexture != NULL)
	{
		mTexture->~Texture();
		delete(mTexture);
		mTexture = NULL;
	}
}