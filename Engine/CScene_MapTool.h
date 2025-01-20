#pragma once
#include "CScene.h"
class CScene_MapTool : public CScene
{


public:
	// virtual void start();
	// virtual void update();
	// virtual void finalupdate();
	// virtual void render(HDC _dc);



	virtual void Enter();
	virtual void Exit();


public:
	CScene_MapTool();
	~CScene_MapTool();
};

