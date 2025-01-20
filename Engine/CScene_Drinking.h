#pragma once
#include "CScene.h"

class CTalkUI;

class CScene_Drinking : public CScene
{

protected:
	static bool			m_bWenttoilet;	   // »≠¿ÂΩ« ¥Ÿ≥‡ø‘¥œ?
	static bool			m_bWentsmoking;	   // »Ìø¨¿Â ¥Ÿ≥‡ø»?




public:
	// virtual void start();
	// virtual void finalupdate();
	virtual void update();
	virtual void render(HDC _dc);



	virtual void EnterMonologue(); // µ∂πÈ æ¿ ¿‘¿Â
	virtual void Enter();
	virtual void Exit();


public:
	CScene_Drinking();
	~CScene_Drinking();

};

