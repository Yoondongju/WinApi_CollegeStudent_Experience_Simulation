#pragma once
#include "CScene_MT.h"
class CScene_SeaMT :
    public CScene_MT
{
private:
	CMonster* pProneMon;
	CMonster* pHaesu;


public:
	// virtual void start();
	virtual void update();
	// virtual void finalupdate();
	virtual void render(HDC _dc);



	virtual void Enter();
	virtual void Exit();


public:
	CScene_SeaMT();
	~CScene_SeaMT();

};

