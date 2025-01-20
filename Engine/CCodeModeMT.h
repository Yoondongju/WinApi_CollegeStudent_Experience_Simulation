#pragma once
#include "CState.h"
class CCodeModeMT :
    public CState
{

private:
	bool	m_barrive;
	Vec2	m_target;

	bool	m_bDance;



public:
	virtual void update();
	virtual void Enter();
	virtual void Exit();


public:
	void CreateTargetPos();
	void update_move();
	void CheckState();

	void CheckDateGirl();



public:
	CCodeModeMT();
	~CCodeModeMT();


};

