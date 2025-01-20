#pragma once
#include "CScene.h"

class CScene_Start :public CScene
{
private:
	Vec2	m_vForcePos;
	bool	m_bUseForce;

	float	m_fForceRadius;
	float	m_fCurRadius;

public:
	virtual void update();

	virtual void render(HDC _dc);

	virtual void Enter();
	virtual void Exit();

	virtual void DeleteAll();

public:
	void CreateForce();


public:
	CScene_Start();
	~CScene_Start();

};

