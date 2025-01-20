#pragma once
#include "CScene.h"

class CUI;


class CScene_Loding : public CScene
{
private:
	CUI* m_pUI;


public:
	virtual void update();
	virtual void Enter();
	virtual void Exit();

	void Exit_game() 
	{
		PostQuitMessage(0);
	}

	virtual void render(HDC _dc);


public:
	CScene_Loding();
	~CScene_Loding();


};

