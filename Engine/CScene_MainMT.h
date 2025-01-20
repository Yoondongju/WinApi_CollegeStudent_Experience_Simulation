#pragma once
#include "CScene_MT.h"

class CScene_MainMT : public CScene_MT
{
private:



public:
	// virtual void start();
	// virtual void finalupdate();
	virtual void update();
	virtual void render(HDC _dc);



	virtual void Enter();
	virtual void Exit();


	virtual void ItemDeployment();

public:
	CScene_MainMT();
	~CScene_MainMT();




};

