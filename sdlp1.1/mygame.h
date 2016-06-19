#pragma once
#include "ZE_Core.h"
using namespace std;

class AssetManager;

class mygame : public Game
{
public:
	unique_ptr<AssetManager> resourses;

	mygame();
	void Init();
	void MainLoop() override;
	void Close() override;
	virtual ~mygame();
};