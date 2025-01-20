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

	if (m_OnlyRecognize)  // �̵� ���� �νĸ� �ϴ³���? ������ ���ÿ� AI���� ��������
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



	if (0.f != pMon->GetTarGetPos().x || 0.f != pMon->GetTarGetPos().y)  // ���� ������ Ÿ�� pos�� �ִ°��
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

	if (abs(m_target.x - vMonPos.x) < 40.f)      //   << Player�� npc�� �Ÿ��� �ƴ� Ÿ�� ��ǥ�� npc���� �Ÿ�
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


	if (m_barrive) // Ÿ���̶� ���������� ���������ʱ�   // (MT���� QutsideBoy �������¿��ٰ� Player�� ����ġ�� Codemode ���·� �ٲ�µ� ���� ���¿��� bFind�� true�� �ǹ����� ����)
	{
		if (L"OutsideBoy" == pMon->GetName())
		{
			Vec2 vPlayerPos = pG_Player->GetPos();
			Vec2 vMonPos = GetMonster()->GetPos();

			float xRange = abs(vPlayerPos.x - vMonPos.x);
			float yRange = abs(vPlayerPos.y - vMonPos.y);

			if (pMon->GetPos().x < 760.f)
			{
				pMon->SetEmotion(MON_EMOTION::CRY);   // AI�������� ���� �����غ�����

			}

			else if (pMon->GetPos().x > 1040.f)
			{
				pMon->SetEmotion(MON_EMOTION::NONE);

			}
			pMon->SetIsDeleteAI(true);
		}



		
		
		return;
	}
		





	if (m_target.x > vMonPos.x) // ������ ����
	{
		pMon->SetDir(CUSTOMER_DIR::RIGHT);
	}
	else if (m_target.x < vMonPos.x) // ���� ����
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
	


	if (xRange < 40.f && yRange < 40.f)    //  <<  Player�� npc�� �Ÿ�
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

