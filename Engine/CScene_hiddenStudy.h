#pragma once
#include "CScene_Study.h"

class CBook;

class CScene_hiddenStudy : public CScene_Study
{
private:
	CBook*  m_pBook;

public:
	virtual void update();
	virtual void render(HDC _dc);



public:
	virtual void Enter();
	virtual void Exit();


public:
	CScene_hiddenStudy();
	~CScene_hiddenStudy();


};

