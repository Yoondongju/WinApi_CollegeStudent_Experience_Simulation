#include "pch.h"
#include "CContactReactionState.h"

#include "CMonster.h"

CContactReactionState::CContactReactionState()
	:CState(MON_STATE::IDLE)
{

}

CContactReactionState::~CContactReactionState()
{

}

void CContactReactionState::update()
{
	Vec2 vPlayerPos = pG_Player->GetPos();
	Vec2 vMonPos = GetMonster()->GetPos();

	float xRange = abs(vPlayerPos.x - vMonPos.x);
	float yRange = abs(vPlayerPos.y - vMonPos.y);




	if (xRange < 40.f && yRange < 40.f)
	{
		GetMonster()->SetFineTarget(true);
		
		GetMonster()->SetCurState(CUSTOMER_STATE::SAY);
		GetMonster()->SetDir(GetMonster()->GetDir());

	}
	else
		GetMonster()->SetFineTarget(false);
}

void CContactReactionState::Enter()
{

}

void CContactReactionState::Exit()
{

}


