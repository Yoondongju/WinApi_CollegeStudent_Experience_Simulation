#pragma once
#include "CState.h"
class CCodeModeState : public CState
{
private:
	bool	m_barrive;
	Vec2	m_target;

	bool	m_OnlyRecognize;	// 이동없이 인식만 하는애인지 ?

	
	bool	m_reTarget;

public:
	virtual void update();
	virtual void Enter();
	virtual void Exit();


public:
	virtual void CreateTargetPos();
	virtual void update_move();

public:
	bool GetbReTarget() { return m_reTarget; }
	void SetReTarget(bool _b) { m_reTarget = _b; }

public:
	void SetOnlyRecognize(bool _b) { m_OnlyRecognize = _b; }
	

	virtual void Recognize_Scope();


public:
	CCodeModeState();
	~CCodeModeState();

};

