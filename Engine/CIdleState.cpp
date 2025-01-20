#include "pch.h"
#include "CIdleState.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"

extern CPlayer* pG_Player;

CIdleState::CIdleState()
	: CState(MON_STATE::IDLE)
{

}

CIdleState::~CIdleState()
{

}


void CIdleState::update()
{

	Vec2 vPlayerPos = pG_Player->GetPos();
	
	// 몬스터의 범위 안에 들어오면 추적상태로 전환
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();
	
	Vec2 vDiff = vPlayerPos - vMonPos;
	float fLen = vDiff.Length();

	// 플레이어가 몬스터의 인식범위 안으로 진입
	if (fLen < pMonster->GetInfo().fRecogRange)
	{
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}

}

void CIdleState::Enter()
{


}

void CIdleState::Exit()
{


}

