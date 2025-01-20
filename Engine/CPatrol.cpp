#include "pch.h"
#include "CPatrol.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTimeMgr.h"
#include "CCollider.h"

CPatrol::CPatrol()
	:CState(MON_STATE::PATROL)
{
	srand((unsigned int)time(NULL));
	m_barrive = true;
}

CPatrol::~CPatrol()
{

}

void CPatrol::update()
{
	if (m_barrive)
		CreateTargetPos();
	 
	update_move();
	Recognize_Scope();
}

void CPatrol::Enter()
{

}

void CPatrol::Exit()
{

}

void CPatrol::CreateTargetPos()
{
	GetMonster()->SetCurState(CUSTOMER_STATE::SAY);
	GetMonster()->SetDir(CUSTOMER_DIR::LEFT);


	float waitTime = GetMonster()->GetWaitTime();
	waitTime += fDT;

	GetMonster()->SetWaitTime(waitTime);

	if (GetMonster()->GetWaitTime() > 2.f)
	{
		GetMonster()->SetWaitTime(0.f);
		m_barrive = false;
		Vec2 vMonPos = GetMonster()->GetEarlyPos();

		float target_xRange = 200.f;


		bool Triger = rand() % 2;

		if (Triger)
		{
			m_target.x = rand() % (int)target_xRange + vMonPos.x;
		}
		else
			m_target.x = -(rand() % (int)target_xRange) + vMonPos.x;

	}

	
	
}

void CPatrol::update_move()
{
	if (GetMonster()->GetFineTarget()) // 타겟이랑 마주쳤으면 움직이지않기
		return;





	Vec2 vMonPos = GetMonster()->GetPos();
	float vMonSpeed = GetMonster()->GetSpeed();
	

	if (abs(m_target.x - vMonPos.x) < 1.f)
	{
		m_barrive = true;
		return;
	}
		

	if (m_target.x > vMonPos.x) // 오른쪽 방향
	{
		GetMonster()->SetDir(CUSTOMER_DIR::RIGHT);
	}
	else if (m_target.x < vMonPos.x) // 왼쪽 방향
	{
		GetMonster()->SetDir(CUSTOMER_DIR::LEFT);
	}


	int Dir = (int)(GetMonster()->GetDir());
	vMonPos.x += Dir * vMonSpeed * fDT;

	GetMonster()->SetCurState(CUSTOMER_STATE::WALK);
	GetMonster()->SetPos(vMonPos);


	

}


void CPatrol::Recognize_Scope()
{
	Vec2 vPlayerPos = pG_Player->GetPos();
	Vec2 vMonPos = GetMonster()->GetPos();
	
	float xRange = abs(vPlayerPos.x - vMonPos.x);
	float yRange = abs(vPlayerPos.y - vMonPos.y);

	


	if (xRange < 50.f && yRange < 50.f)
	{
		GetMonster()->SetFineTarget(true);
		// 인식하고 손님이 도움을 요청함
		GetMonster()->SetCurState(CUSTOMER_STATE::SAY);
		GetMonster()->SetDir(GetMonster()->GetDir());


		



	}
	else
		GetMonster()->SetFineTarget(false);


}


