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
	//���ø���ķ��������������̨�������λ�ã��������
	//����Ĳ���Ӧ���ǵ�ǰ֡��WH����ʹ�þ�̬ͼƬʱ���ö�ֵ��mWmH���
	SDL_Rect renderRect = setRenderRect(nowUsingSubData.width, nowUsingSubData.height);

	//���ʹ����ˮƽ��ת�����߸����е�ĳһ�������˷�ת�Ļ�����������ķ����������Լ�Ҳ�������˷�ת��
	if (flip == SDL_FLIP_HORIZONTAL)
	{
		//���ȣ���̨λ�õ�XӦ�ò����������ELSEִ�У��������˷�ת����������Ĵ���ִ��
		//��������ˮƽ��תʱ��ʹ�õ���̨��XӦ���϶������ܿ�ȼ�ȥ����д�ı��е���͸�����֣�Ȼ���ټ�ȥ��֡��ʵ�ʿ��
		//�͵õ�����һ�뱻������͸�����֡�
		//��ͼƬΪ��̬ʱ��frameXΪ0��subdata.width����mW�����Ը��㷨�����0
		renderRect.x += mWidth - abs(nowUsingSubData.frameX) - nowUsingSubData.width;
		//Ȼ��ͼƬ������ԭ��X����з�ת��ֻ���ȥmW����
		renderRect.x -= mWidth;
		renderRect.y += abs(nowUsingSubData.frameY);
	}
	//���ʹ���˴�ֱ��ת���㷨����ͬ��
	else if (flip == SDL_FLIP_VERTICAL)
	{
		renderRect.x += abs(nowUsingSubData.frameX);
		renderRect.y += mHeight - abs(nowUsingSubData.frameY) - nowUsingSubData.height;
		renderRect.y -= mHeight;
	}
	else
	{
		//��̨λ��XӦ�ü�����һ֡��ͼƬ����ȡ����͸�����֣��Ի����ȷ��X
		//frameX����texturePackager����е�͸�����ֺ��Զ����ɵģ���ֵΪ��ֵ����ʾ��ȥ�˶���
		//��̬ͼƬʱ����ֵΪ0
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
	// ����ָ�뻯
	//if (mTexture != NULL)
	//{
	//	mTexture->~Texture();
	//	delete(mTexture);
	//	mTexture = NULL;
	//}
}