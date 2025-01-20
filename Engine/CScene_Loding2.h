#pragma once
#include "CScene.h"
class CScene_Loding2 : public CScene
{
private:




public:
	virtual void update();

	virtual void subsequent_update();

	virtual void render(HDC _dc);
	virtual void Enter();
	virtual void Exit();
	
	

public:
	CScene_Loding2();
	~CScene_Loding2();


};

