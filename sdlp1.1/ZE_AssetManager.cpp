#include <algorithm>
#include <SDL_image.h>
#include "ZE_Core.h"
#include "tinyxml2.h"
#include "ZE_AssetManager.h"

using namespace std;
using namespace tinyxml2;

void AssetManager::Init(string xmlPath)
{
	XMLDocument doc;
	if (XML_NO_ERROR != doc.LoadFile(xmlPath.c_str()))
	{
		throw std::runtime_error("XMLDocument LoadFile ERROR");
	}
	XMLElement* docroot = doc.RootElement();
	XMLElement* sub = docroot->FirstChildElement("element");
	while (sub)
	{
		string type;
		string name;
		string path;
		string xml;

		const XMLAttribute* attr = sub->FirstAttribute();
		type = attr->Value();
		attr = attr->Next();
		name = attr->Value();
		attr = attr->Next();
		path = attr->Value();
		attr = attr->Next();
		xml = attr->Value();

		if (type == "image")
			LoadTexture(name, path, xml);
		else if (type == "ttf")
			LoadTTF(name, path);
		else if (type == "music" || type == "sound")
		{
			bool isMusic = false;
			if (type == "music")
				isMusic = true;
			LoadSound(isMusic, name, path);
		}

		sub = sub->NextSiblingElement();
	}
}

textureStruct AssetManager::getTexture(string name)
{
	textureStruct temp;
	for (unsigned int i = 0; i < TEXTURES.size(); i++)
	{
		if (name == TEXTURES[i]->name)
		{
			temp.subData = TEXTURES[i]->mySubInfo;
			temp.texture = TEXTURES[i];
			return temp;
			break;
		}
		for (unsigned int j = 0; j < TEXTURES[i]->subTextures.size(); j++)
		{
			if (name == TEXTURES[i]->subTextures[j].name)
			{
				temp.subData = TEXTURES[i]->subTextures[j];
				temp.texture = TEXTURES[i];
				return temp;
				break;
			}
		}
	}
	ZE_error.PopDebugConsole_Error("Can't find Texture: " + name);
	return temp;
}

SDL_Texture* AssetManager::getTTFTexture(string text, string name, int size, SDL_Color color, int* width, int* height,
	unsigned int effectLevel)
{
	auto usingfont = getFont(name);
	TTF_Font* tempfont = usingfont->getFont(size);
	SDL_Surface* tempsur = NULL;
	if (effectLevel == 0)
		tempsur = TTF_RenderUTF8_Solid(tempfont, text.c_str(), color);
	else if (effectLevel == 1)
		tempsur = TTF_RenderUTF8_Shaded(tempfont, text.c_str(), color, { 0, 0, 0 });
	else if (effectLevel == 2)
		tempsur = TTF_RenderUTF8_Blended(tempfont, text.c_str(), color);
	if (tempsur == NULL)
	{
		ZE_error.PopDebugConsole_SDL_ttfError("Unable to render text surface!");
		return NULL;
	}
	else
	{
		SDL_Texture* temptexture = Surface2SDLTexture(tempsur, width, height);
		return temptexture;
	}
}

shared_ptr<Font> AssetManager::getFont(string name)
{
	for (auto i : FONT)
	{
		if (name == i->name)
		{
			return i;
		}
	}
	ZE_error.PopDebugConsole_Error("Can't find font name:" + name);
	return nullptr;
}

shared_ptr<Sound> AssetManager::getSound(string name)
{
	for (auto i : SOUNDS)
	{
		if (name == i->name)
		{
			return i;
		}
	}
	ZE_error.PopDebugConsole_Error("Can't find sound name:" + name);
	return nullptr;
}

//���ڶԱ������ṹ��Ĵ�С����getTextures����ʹ�ã���AС��B������true
//��Ϊsort�ƺ����ܷ���������ĺ���������д������
bool comparTextureStruce(textureStruct a, textureStruct b)
{
	return a.subData.name < b.subData.name;
}

deque<textureStruct> AssetManager::getTextures(string partOfName)
{
	deque<textureStruct> temp;
	for (unsigned int i = 0; i < TEXTURES.size(); i++)
	{
		textureStruct oneOfTemp;
		if (TEXTURES[i]->name.find(partOfName) == 0)
		{
			oneOfTemp.subData = TEXTURES[i]->mySubInfo;
			oneOfTemp.texture = TEXTURES[i];
			temp.push_back(oneOfTemp);
		}
		else
		{
			for (unsigned int j = 0; j < TEXTURES[i]->subTextures.size(); j++)
			{
				if (TEXTURES[i]->subTextures[j].name.find(partOfName) == 0)
				{
					oneOfTemp.subData = TEXTURES[i]->subTextures[j];
					oneOfTemp.texture = TEXTURES[i];
					temp.push_back(oneOfTemp);
				}
			}
		}
	}
	if (temp.size() == 0)
	{
		ZE_error.PopDebugConsole_Error("Can't find Texture has part of name: " + partOfName);
	}
	sort(temp.begin(), temp.end(), comparTextureStruce);
	return temp;
}

void AssetManager::LoadTexture(string name, string path, string xml)
{
	/*֮ǰ��TEXTURES��һ��һ��������������ָ�����顣
	�����������������һ���ֲ�Texture������Init��push��TEXTURE������
	��������ͼ�ᶪʧ��
	�Ҳ��룬������Ϊ�ֲ�����Texture�ڷ��������󼴱����٣�����push��TEXTURE��Ԫ����һ������Ұָ��Ĵ��ڡ�
	�����Ҳ��Ϊʲô��Ҷ���ָ���ԭ�򣬾ͺ�������A�������һ��¥��������B�������ܲ���˵��¥���ȥ��B����
	���Ծ���һ��ָ�룬����B��ַ�����Լ������ͺ��ˡ�*/

	auto temp_texture = make_shared<Texture>();

	//��֮��߱����SURFACE��ȡ����������������������S2T������ָ�봫����
	//ֱ�ӰѺ�������INIT��Ļ����޷���ȷ����WH���������Ե����ó�����ֵһ�顣
	int tempWidth = 0;
	int tempHeight = 0;

	//����һ������ͼ���飬���ڴ��ݸ���ͼ��
	deque <SubTexture> subtexture = SubXmlReader(xml);

	//��������Ķ�ȡ��ַ�������һ��surfaceָ�봫�ݸ�S2T��
	SDL_Texture* tempTexture = Surface2SDLTexture(ImageReader(path), &tempWidth, &tempHeight);
	// ��ʼ��
	temp_texture->Init(name, tempTexture, tempWidth, tempHeight, subtexture);
	// ����ָ��
	TEXTURES.push_back(temp_texture);
}

void AssetManager::LoadTTF(string name, string path)
{
	FONT.emplace_back(new Font(name, path));
}

void AssetManager::LoadSound(bool isMusic, string name, string path)
{
	if (isMusic)
	{
		Mix_Music* music = Mix_LoadMUS(path.c_str());
		SOUNDS.emplace_back(new Sound(name, music));
	}
	else
	{
		Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
		SOUNDS.emplace_back(new Sound(name, sound));
	}
}

SDL_Texture* AssetManager::Surface2SDLTexture(SDL_Surface* surface, int* getW, int* getH)
{
	SDL_Texture* newTexture = NULL;
	newTexture = SDL_CreateTextureFromSurface(g_ZE_MainRenderer, surface);
	//��Surfaceת��ΪTexture
	//ֱ�Ӵ�����ͼҲ�У�����Ҫ�Ĳ������Ӻܶ࣬����ֱ��ת��
	if (newTexture == NULL)
	{
		ZE_error.PopDebugConsole_SDLError("Unable to creat texture!");
	}
	//��ȡ��ߣ���ָ�봫��ȥ
	*getW = surface->w;
	*getH = surface->h;
	SDL_FreeSurface(surface);
	//����ʱ��Surface�ͷŵ���ʱ�̼ǵã�û���������ӣ�����������
	//������Ϊ�����Ǹ���ʽ�������Ḵ��һ��surface������ָ��ԭ���ĵ�ַ���Ͼ�ռ�õ��ڴ�������ܲ�һ��
	return newTexture;
}

SDL_Surface* AssetManager::ImageReader(string path, int extNameLength)
{
	string temp;
	for (int i = 0; i < extNameLength; i++)
	{
		//ѭ����ȡ�ַ����������λ��ֵ��temp��extNameLengthĬ��Ϊ3��
		//ͷ�ļ��еĺ�������������Ĭ�ϲ�������Ͳ����������ˣ���ס����Ȼ����һ��C2572����
		temp += path[path.length() - (extNameLength - i)];
		//���temp����һ����չ��
	}

	SDL_Surface* tempSurface = NULL;

	if (temp == "bmp" || temp == "BMP")
	{
		//����BMP�ķ���ʹ��SDLԭ�����뺯����SDLֻ������BMP
		tempSurface = SDL_LoadBMP(path.c_str());
		//��Ϊ�Ǵ�C��������string����������׸��
		if (tempSurface == NULL)
		{
			ZE_error.PopDebugConsole_SDLError("Unable to load bmp file:" + path + " |Load error");
		}
	}
	else if (temp == "png" || temp == "PNG")
	{
		//SDLֻ������BMP��PNG����SDL_IMAGE
		tempSurface = IMG_Load(path.c_str());
		if (tempSurface == NULL)
		{
			ZE_error.PopDebugConsole_SDL_ImageError("Unable to load png file:" + path + " |Load error");
		}
	}
	else
	{
		ZE_error.PopDebugConsole_Error("Unable to read " + temp + " file.");
		//if�Ƕ�û��ס�Ǿ���û��ĸ�ʽ��...
	}

	return tempSurface;
}

deque<SubTexture> AssetManager::SubXmlReader(string xml)
{
	deque<SubTexture> temp;
	if (xml == "")
	{
		return temp;
	}
	else
	{
		XMLDocument doc;
		doc.LoadFile(xml.c_str());
		XMLElement* resourses = doc.RootElement();
		XMLElement* texture = resourses->FirstChildElement("SubTexture");
		while (texture)
		{
			SubTexture tempSub;
			const XMLAttribute* attr = texture->FirstAttribute();
			tempSub.name = attr->Value();
			attr = attr->Next();
			tempSub.x = attr->IntValue();
			attr = attr->Next();
			tempSub.y = attr->IntValue();
			attr = attr->Next();
			tempSub.width = attr->IntValue();
			attr = attr->Next();
			tempSub.height = attr->IntValue();
			attr = attr->Next();
			if (attr == NULL)
			{
				tempSub.frameX = 0;
				tempSub.frameY = 0;
				tempSub.frameWidth = tempSub.width;
				tempSub.frameHeight = tempSub.height;
			}
			else
			{
				tempSub.frameX = attr->IntValue();
				attr = attr->Next();
				tempSub.frameY = attr->IntValue();
				attr = attr->Next();
				tempSub.frameWidth = attr->IntValue();
				attr = attr->Next();
				tempSub.frameHeight = attr->IntValue();
			}
			temp.push_back(tempSub);
			texture = texture->NextSiblingElement();
		}
		return temp;
	}
}

AssetManager::~AssetManager()
{
	dispose();
}

void AssetManager::dispose()
{
	DeleteAllTextures();
	DeleteAllFonts();
	DeleteAllSounds();
}

void AssetManager::DeleteSound(int index)
{
	// ����ָ�뻯
	//SOUNDS[index]->~Sound();
	//delete(SOUNDS[index]);
	//SOUNDS[index] = NULL;
	SOUNDS.erase(SOUNDS.begin() + index);
}

void AssetManager::DeleteAllSounds()
{
	while (SOUNDS.size() != 0)
	{
		DeleteSound(0);
	}
	SOUNDS.clear();
}

void AssetManager::DeleteFont(int index)
{
	// ����ָ�뻯
	//FONT[index]->~Font();
	//delete(FONT[index]);
	//FONT[index] = NULL;
	FONT.erase(FONT.begin() + index);
}

void AssetManager::DeleteAllFonts()
{
	while (FONT.size() != 0)
	{
		DeleteFont(0);
	}
	FONT.clear();
}

void AssetManager::DeleteTexture(int index)
{
	// ����ָ�뻯
	//TEXTURES[index]->~Texture();
	//delete(TEXTURES[index]);
	//TEXTURES[index] = NULL;
	//ɾ������Ԫ��
	TEXTURES.erase(TEXTURES.begin() + index);
}

void AssetManager::DeleteAllTextures()
{
	//AllTexture��һ��ָ��vector
	while (TEXTURES.size() != 0)
	{
		DeleteTexture(0);
		//��Զ�ǵã���ҪҰָ�룬��Ҫ�ڴ�й¶
	}

	/*��Ҫ�߳�һ���㣬��ǰ�������������ôд�ģ��򻯹���
	for(var i = 0; i < TEXTURES.size(); i++)
	{
		dispose(TEXTURES[i])
		remove(TEXTURES[i])
	}
	����Ϊֻ����ѧ��ʱ��Ż᷸����ɵ�ƴ���
	�����㲻�ϵ�ɾ������Ԫ�أ�SIZEԽ��ԽС��IԽ��Խ����ɾһ�벻���˳�ѭ������
	Ȼ���㻹ɾ����i���Բۣ�����Խ��ԽС������һ��I��ָ��Ƿ�������
	�����굼�µ��ڴ�й¶DEBUG�˿�����
	����ɵ����*/

	TEXTURES.clear();
}