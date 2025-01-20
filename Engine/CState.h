#pragma once

class AI;
class CMonster;

class CState
{
private:
	AI*			m_pAI;
	MON_STATE	m_eState;


protected:
	bool	m_checkState;


public:
	AI* GetAI() { return m_pAI; }
	MON_STATE GetType() { return m_eState; }
	CMonster* GetMonster();

public:
	virtual void update() = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;


public:
	void SetcheckState(bool _b) { m_checkState = _b; }


public:
	CState(MON_STATE _eState);
	virtual ~CState();

	friend class AI;

};

