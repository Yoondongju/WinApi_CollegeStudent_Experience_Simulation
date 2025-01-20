#pragma once
#include "CScene.h"
class CScene_MT : public CScene
{
protected:
	static bool m_endMT;



public:
	// virtual void start();
	 virtual void update();
	// virtual void finalupdate();
	 virtual void render(HDC _dc);



	virtual void Enter();
	virtual void Exit();


public:
	CScene_MT();
	~CScene_MT();

};

