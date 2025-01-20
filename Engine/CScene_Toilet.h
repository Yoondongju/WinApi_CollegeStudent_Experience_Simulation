#pragma once
#include "CScene_Drinking.h"

class CObject;

class CScene_Toilet : public CScene_Drinking
{
private:
	CObject*	m_ToiletFriend;



public:
	// virtual void start();
	// virtual void finalupdate();
	virtual void update();
	virtual void render(HDC _dc);



	virtual void Enter();
	virtual void Exit();


public:
	CScene_Toilet();
	~CScene_Toilet();

};

