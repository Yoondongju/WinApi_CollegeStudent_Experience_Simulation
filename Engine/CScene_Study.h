#pragma once
#include "CScene.h"
class CScene_Study : public CScene
{


public:
	// virtual void start();
	 virtual void update();
	// virtual void finalupdate();
	 virtual void render(HDC _dc);



public:
	void CreateBook();




	virtual void Enter();
	virtual void Exit();


public:
	CScene_Study();
	~CScene_Study();
};

