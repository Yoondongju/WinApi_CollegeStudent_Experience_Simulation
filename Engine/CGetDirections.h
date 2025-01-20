#pragma once
#include "CState.h"
class CGetDirections : public CState
{



public:
	virtual void update();
	virtual void Enter();
	virtual void Exit();

public:
	CGetDirections();
	~CGetDirections();

};

