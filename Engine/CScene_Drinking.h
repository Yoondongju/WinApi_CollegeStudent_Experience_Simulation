#pragma once
#include "CScene.h"

class CTalkUI;

class CScene_Drinking : public CScene
{

protected:
	static bool			m_bWenttoilet;	   // ȭ��� �ٳ�Դ�?
	static bool			m_bWentsmoking;	   // ���� �ٳ��?




public:
	// virtual void start();
	// virtual void finalupdate();
	virtual void update();
	virtual void render(HDC _dc);



	virtual void EnterMonologue(); // ���� �� ����
	virtual void Enter();
	virtual void Exit();


public:
	CScene_Drinking();
	~CScene_Drinking();

};

