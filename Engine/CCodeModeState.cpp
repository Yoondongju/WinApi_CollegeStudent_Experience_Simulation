#include "pch.h"
#include "CCodeModeState.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTimeMgr.h"
#include "CAnimator.h"

#include "AI.h"

CCodeModeState::CCodeModeState()
	:CState(MON_STATE::CODEMODE)
	, m_OnlyRecognize(false)
	, m_reTarget(false)
	, m_barrive(false)
{

}

CCodeModeState::~CCodeModeState()
{

}


void CCodeModeState::update()
{	

	if (m_OnlyRecognize)  // 이동 없이 인식만 하는놈임? 생성과 동시에 AI에서 결정해줌
	{
		m_barrive = true;
		Recognize_Scope();
		return;
	}
	

	
	CreateTargetPos();
	update_move();
	Recognize_Scope();
}

void CCodeModeState::Enter()
{
}

void CCodeModeState::Exit()
{
}

void CCodeModeState::CreateTargetPos()
{
	CMonster* pMon = GetMonster();
	m_barrive = false;



	if (0.f != pMon->GetTarGetPos().x || 0.f != pMon->GetTarGetPos().y)  // 내가 가야할 타겟 pos가 있는경우
	{
		m_target = pMon->GetTarGetPos();
	}
	else 
		m_target = pG_Player->GetPos();


}

void CCodeModeState::update_move()
{
	CMonster* pMon = GetMonster();

	Vec2 vMonPos = pMon->GetPos();
	float vMonSpeed = pMon->GetSpeed();

	if (abs(m_target.x - vMonPos.x) < 40.f)      //   << Player와 npc의 거리가 아닌 타겟 좌표와 npc와의 거리
	{
		if (L"friend_2" == pMon->GetName() || L"Earnest_friend" == pMon->GetName())
			pMon->SetCurState(CUSTOMER_STATE::VOMIT);

		else
		{
			if (L"OutsideBoy" != pMon->GetName())
			{
				pMon->SetCurState(CUSTOMER_STATE::SAY);
			}
			else if (L"OutsideBoy" == pMon->GetName())
			{
				pMon->SetCurState(CUSTOMER_STATE::SIT);
			}

		}


		m_barrive = true;
	}


	if (m_barrive) // 타겟이랑 마주쳤으면 움직이지않기   // (MT에서 QutsideBoy 순찰상태엿다가 Player와 마주치면 Codemode 상태로 바뀌는데 순찰 상태에서 bFind가 true가 되버려서 들어옴)
	{
		if (L"OutsideBoy" == pMon->GetName())
		{
			Vec2 vPlayerPos = pG_Player->GetPos();
			Vec2 vMonPos = GetMonster()->GetPos();

			float xRange = abs(vPlayerPos.x - vMonPos.x);
			float yRange = abs(vPlayerPos.y - vMonPos.y);

			if (pMon->GetPos().x < 760.f)
			{
				pMon->SetEmotion(MON_EMOTION::CRY);   // AI지워지는 시점 생각해봐야함

			}

			else if (pMon->GetPos().x > 1040.f)
			{
				pMon->SetEmotion(MON_EMOTION::NONE);

			}
			pMon->SetIsDeleteAI(true);
		}



		
		
		return;
	}
		





	if (m_target.x > vMonPos.x) // 오른쪽 방향
	{
		pMon->SetDir(CUSTOMER_DIR::RIGHT);
	}
	else if (m_target.x < vMonPos.x) // 왼쪽 방향
	{
		pMon->SetDir(CUSTOMER_DIR::LEFT);
	}

	int Dir = (int)(pMon->GetDir());
	vMonPos.x += Dir * vMonSpeed * fDT * 2;
	pMon->SetCurState(CUSTOMER_STATE::WALK);
	pMon->SetPos(vMonPos);
	GetMonster()->SetFineTarget(false);
	
}

void CCodeModeState::Recognize_Scope()
{
	if (!m_barrive)
		return;

	Vec2 vPlayerPos = pG_Player->GetPos();
	Vec2 vMonPos = GetMonster()->GetPos();

	float xRange = abs(vPlayerPos.x - vMonPos.x);
	float yRange = abs(vPlayerPos.y - vMonPos.y);
	


	if (xRange < 40.f && yRange < 40.f)    //  <<  Player와 npc의 거리
	{
		GetMonster()->SetFineTarget(true);
		
		if (L"friend_2" == GetMonster()->GetName())
		{
			if (L"Toilet Scene" == CSceneMgr::GetInst()->GetCurScene()->GetName())
			{
				GetMonster()->SetCurState(CUSTOMER_STATE::VOMIT);
				GetMonster()->SetMyConversationID(3);
			}		
		}
		else
		{
			if( CUSTOMER_STATE::WALK == GetMonster()->GetCurState())
				GetMonster()->SetCurState(CUSTOMER_STATE::SAY);
		}
			

		GetMonster()->SetDir(GetMonster()->GetDir());
	}
	else
		GetMonster()->SetFineTarget(false);

}

