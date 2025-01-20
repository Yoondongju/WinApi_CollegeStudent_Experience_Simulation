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
	
	// ������ ���� �ȿ� ������ �������·� ��ȯ
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();
	
	Vec2 vDiff = vPlayerPos - vMonPos;
	float fLen = vDiff.Length();

	// �÷��̾ ������ �νĹ��� ������ ����
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

