#pragma once
#include "CScene_MT.h"
class CScene_EndMT :
    public CScene_MT
{

public:
	// virtual void start();
	virtual void update();
	// virtual void finalupdate();
	virtual void render(HDC _dc);



	virtual void Enter();
	virtual void Exit();


public:
	CScene_EndMT();
	~CScene_EndMT();

};

