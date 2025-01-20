#pragma once
#include "CState.h"
class CContactReactionState : public CState
{


public:
	virtual void update();
	virtual void Enter();
	virtual void Exit();

public:
	CContactReactionState();
	~CContactReactionState();

};

