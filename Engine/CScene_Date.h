#pragma once
#include "CScene.h"
class CScene_Date : public CScene
{

public:
	// virtual void start();
	 virtual void update();
	// virtual void finalupdate();
	 virtual void render(HDC _dc);



	virtual void Enter();
	virtual void Exit();


public:
	CScene_Date();
	~CScene_Date();

};

