#pragma once
#include "ZE_Core.h"
#include "ZE_AssetManager.h"

using namespace std;

class mygame : public Game
{
public:
	AssetManager resourses;

	mygame();
	void Init();
	void MainLoop() override;
	void Close() override;
	virtual ~mygame();
};