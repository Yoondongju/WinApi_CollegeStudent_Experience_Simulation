#pragma once
#include "CState.h"
class CPatrol : public CState
{
private:
	bool	m_barrive;
	Vec2	m_target;


	


public:
	virtual void update();
	virtual void Enter();
	virtual void Exit();


public:
	virtual void CreateTargetPos();
	virtual void update_move();
	virtual void Recognize_Scope();


public:
	CPatrol();
	~CPatrol();

	

};

