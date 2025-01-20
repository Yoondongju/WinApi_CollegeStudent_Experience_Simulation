#include "pch.h"
#include "CTraceState.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTimeMgr.h"

extern CPlayer* pG_Player;

CTraceState::CTraceState()
	:CState(MON_STATE::TRACE)
{

}

CTraceState::~CTraceState()
{

}

void CTraceState::update()
{
	Vec2 vPlayerPos = pG_Player->GetPos();

	Vec2 vMonPos = GetMonster()->GetPos();

	Vec2 vMonDir = vPlayerPos - vMonPos;
	vMonDir.Normalize();

	vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed* fDT;

	GetMonster()->SetPos(vMonPos);
	
}

void CTraceState::Enter()
{

}

void CTraceState::Exit()
{

}
