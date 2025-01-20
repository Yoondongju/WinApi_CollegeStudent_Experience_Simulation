#include "pch.h"
#include "CScene_Job.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "CCollisionMgr.h"

#include "CCamera.h"
#include "CTexture.h"
#include "CResMgr.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CGround.h"
#include "AI.h"
#include "CPatrol.h"
#include "CGetDirections.h"

#include "CPotal.h"

#include "CSound.h"

#include "CContactReactionState.h"

CScene_Job::CScene_Job()
{
	
}


CScene_Job::~CScene_Job()
{
	
}

void CScene_Job::Enter()
{
	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Job_BGM");
	//pNewSound->Play();
	pNewSound->PlayToBGM(true);
	//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	pNewSound->SetVolume(50.f); // 소리 볼륨 크기







	Vec2 vResolution = CCore::GetInst()->GetResolution();

	
	CCamera::GetInst()->FadeIn(3.f);
	CCamera::GetInst()->SetTarget(nullptr);


	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"Job_Scene", L"texture\\Job_scene.png");


	// ================================== 벽 만들기 ================================
	CGround* pOutline = new CGround;
	pOutline->SetPos(Vec2(vResolution.x / 2.f, 60.f));
	pOutline->SetScale(Vec2(vResolution.x, 130.f));
	pOutline->GetCollider()->SetScale(Vec2(pOutline->GetScale()));	
	AddObject(pOutline, GROUP_TYPE::GROUND);


	CGround* pOutline2 = new CGround;
	pOutline2->SetPos(Vec2(vResolution.x - 5.f, vResolution.y /2.f));
	pOutline2->SetScale(Vec2(10.f, vResolution.y));
	pOutline2->GetCollider()->SetScale(Vec2(pOutline2->GetScale()));
	AddObject(pOutline2, GROUP_TYPE::GROUND);


	CGround* pOutline3 = new CGround;
	pOutline3->SetPos(Vec2(vResolution.x / 2.f, 750.f));
	pOutline3->SetScale(Vec2(vResolution.x, 30.f));
	pOutline3->GetCollider()->SetScale(Vec2(pOutline3->GetScale()));
	AddObject(pOutline3, GROUP_TYPE::GROUND);
	
	
	CGround* pOutline4 = new CGround;
	pOutline4->SetPos(Vec2(5.f , vResolution.y / 2.f));
	pOutline4->SetScale(Vec2(10.f, vResolution.y));
	pOutline4->GetCollider()->SetScale(Vec2(pOutline4->GetScale()));
	AddObject(pOutline4, GROUP_TYPE::GROUND);
	// ================================== 벽 만들기 ================================




	// =============================== Object 충돌체 배치 ==========================
	CGround* pObjectCollider = new CGround;    
	pObjectCollider->SetPos(Vec2(945.f,311.f));
	pObjectCollider->SetScale(Vec2(248.f,85.f));
	pObjectCollider->GetCollider()->SetScale(Vec2(pObjectCollider->GetScale()));
	AddObject(pObjectCollider, GROUP_TYPE::GROUND);

	CGround* pObjectCollider2 = new CGround;    
	pObjectCollider2->SetPos(Vec2(935.f, 570.f));
	pObjectCollider2->SetScale(Vec2(150.f, 170.f));
	pObjectCollider2->GetCollider()->SetScale(Vec2(pObjectCollider2->GetScale()));
	AddObject(pObjectCollider2, GROUP_TYPE::GROUND);


	CGround* pObjectCollider3 = new CGround;    
	pObjectCollider3->SetPos(Vec2(1200.f, 600.f));
	pObjectCollider3->SetScale(Vec2(75.f, 85.f));
	pObjectCollider3->GetCollider()->SetScale(Vec2(pObjectCollider3->GetScale()));
	AddObject(pObjectCollider3, GROUP_TYPE::GROUND);

	CGround* pObjectCollider4 = new CGround;    
	pObjectCollider4->SetPos(Vec2(90.f, 660.f));
	pObjectCollider4->SetScale(Vec2(66.f, 72.f));
	pObjectCollider4->GetCollider()->SetScale(Vec2(pObjectCollider4->GetScale()));
	AddObject(pObjectCollider4, GROUP_TYPE::GROUND);


	CGround* pObjectCollider5 = new CGround;    
	pObjectCollider5->SetPos(Vec2(511.f, 484.f));
	pObjectCollider5->SetScale(Vec2(524.f, 74.f));
	pObjectCollider5->GetCollider()->SetScale(Vec2(pObjectCollider5->GetScale()));
	AddObject(pObjectCollider5, GROUP_TYPE::GROUND);

	CGround* pObjectCollider6 = new CGround;    
	pObjectCollider6->SetPos(Vec2(524.f, 630.f));
	pObjectCollider6->SetScale(Vec2(500.f, 65.f));
	pObjectCollider6->GetCollider()->SetScale(Vec2(pObjectCollider6->GetScale()));
	AddObject(pObjectCollider6, GROUP_TYPE::GROUND);

	CGround* pObjectCollider7 = new CGround;    
	pObjectCollider7->SetPos(Vec2(90.f, 245.f));
	pObjectCollider7->SetScale(Vec2(64.f, 276.f));
	pObjectCollider7->GetCollider()->SetScale(Vec2(pObjectCollider7->GetScale()));
	AddObject(pObjectCollider7, GROUP_TYPE::GROUND);

	CGround* pObjectCollider8 = new CGround;    
	pObjectCollider8->SetPos(Vec2(200.f, 354.f));
	pObjectCollider8->SetScale(Vec2(276.f, 64.f));
	pObjectCollider8->GetCollider()->SetScale(Vec2(pObjectCollider8->GetScale()));
	AddObject(pObjectCollider8, GROUP_TYPE::GROUND);

	CGround* pObjectCollider9 = new CGround;    
	pObjectCollider9->SetPos(Vec2(311.f, 290.f));
	pObjectCollider9->SetScale(Vec2(60.f, 190.f));
	pObjectCollider9->GetCollider()->SetScale(Vec2(pObjectCollider9->GetScale()));
	AddObject(pObjectCollider9, GROUP_TYPE::GROUND);
	// =============================== Object 충돌체 배치 ==========================




	//pG_Player = new CPlayer;		// Player가 지워지는 시점도 고려해봐야함
	//pG_Player->SetName(L"Player");
	pG_Player->SetPos(Vec2(220.f, 250.f));
	pG_Player->SetScale(Vec2(50.f, 75.f));
	pG_Player->SetMyConversationID(4594);
	//pG_Player->SetspeechBubble();
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);  // main Player 행동 불가능
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);


	//pG_Player; // 얘 가지고 놀면댐
	//pG_Player->SetPos(Vec2(180.f, 170.f));
	//AddObject(pG_Player, GROUP_TYPE::PLAYER);
	////pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
	//RegisterPlayer(pG_Player);
	




	// ============================== 손님 배치 =============================
	AI* pAI0 = new AI;
	pAI0->AddState(new CGetDirections);
	pAI0->SetCurState(MON_STATE::GETDIRECTIONS);     // 어떻게 동작할건지 정의해야함

	AI* pAI = new AI;
	pAI->AddState(new CPatrol);
	pAI->SetCurState(MON_STATE::PATROL);

	AI* pAI2 = new AI;
	pAI2->AddState(new CPatrol);
	pAI2->SetCurState(MON_STATE::PATROL);

	AI* pAI3 = new AI;
	pAI3->AddState(new CPatrol);
	pAI3->SetCurState(MON_STATE::PATROL);

	///  AI ====================================

	CMonster* pManager = new CMonster;
	pManager->SetName(L"Manager");
	pManager->SetPos(Vec2(250.f, 260.f));
	pManager->SetEarlyPos(pManager->GetPos());
	pManager->SetScale(Vec2(50.f, 80.f));
	pManager->SetAI(pAI0);
	pManager->SetFineTarget(true);
	pManager->SetCurState(CUSTOMER_STATE::SAY);
	pManager->SetDir(CUSTOMER_DIR::RIGHT);
	pManager->SetMyConversationID(0);
	AddObject(pManager, GROUP_TYPE::MONSTER);
	
	
	CMonster* pCustomer = new CMonster;
	pCustomer->SetName(L"Customer1");
	pCustomer->SetPos(Vec2(900.f, 200.f));
	pCustomer->SetEarlyPos(pCustomer->GetPos());
	pCustomer->SetScale(Vec2(35.f, 70.f));
	pCustomer->SetAI(pAI);
	pCustomer->SetMyConversationID(1);
	AddObject(pCustomer, GROUP_TYPE::MONSTER);
	
	
	CMonster* pCustomer2 = new CMonster;
	pCustomer2->SetName(L"Customer2");
	pCustomer2->SetPos(Vec2(560.f, 380.f));
	pCustomer2->SetEarlyPos(pCustomer2->GetPos());
	pCustomer2->SetScale(Vec2(35.f, 70.f));
	pCustomer2->SetAI(pAI2);
	pCustomer2->SetMyConversationID(2);
	AddObject(pCustomer2, GROUP_TYPE::MONSTER);
	
	
	CMonster* pCustomer3 = new CMonster;
	pCustomer3->SetName(L"Customer3");
	pCustomer3->SetPos(Vec2(670.f, 550.f));
	pCustomer3->SetEarlyPos(pCustomer3->GetPos());
	pCustomer3->SetScale(Vec2(35.f, 70.f));
	pCustomer3->SetAI(pAI3);
	pCustomer3->SetMyConversationID(3);
	AddObject(pCustomer3, GROUP_TYPE::MONSTER);
	// ============================== 손님 배치 =============================

	



	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::POTAL);

}

void CScene_Job::update()
{
	CScene::update();

	if (KEY_CHECK(KEY::ENTER, KEY_STATE::TAP))
	{
		ChangeScene(SCENE_TYPE::STAGE_LOBBY);
	}
	
	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);


	static bool isFirstCall = false;
	if (0 == GetMonsterGroup().size() && !isFirstCall)  // NPC가 모두 삭제 되었을때 진상 손님 추가해야함
	{
		isFirstCall = true;
	
		AI* pAI = new AI;
		pAI->AddState(new CContactReactionState);
		pAI->SetCurState(MON_STATE::IDLE);
	
		CMonster* pbadCustomer = new CMonster;
		pbadCustomer->SetName(L"badCustomer");
		pbadCustomer->SetPos(Vec2(395.f, 284.f));
		pbadCustomer->SetEarlyPos(pbadCustomer->GetPos());
		pbadCustomer->SetScale(Vec2(35.f, 70.f));
		pbadCustomer->SetCurState(CUSTOMER_STATE::SAY);
		pbadCustomer->SetDir(CUSTOMER_DIR::LEFT);
		pbadCustomer->SetAI(pAI);
		pbadCustomer->SetMyConversationID(4);
		AddObject(pbadCustomer, GROUP_TYPE::MONSTER);
	}


	if (0 == GetMonsterGroup().size()) // 모든 Npc가 사라졌을때 알바 컨텐츠 종료
	{
		// 모든 컨텐츠 씬에는 항상 마지막에 끝을 알리는 어떤 트리거적인 요소가 필요한데
		// 이것을 씬 배치의 어떤 물체로 놔둘것임
		if (0 == GetGroupObject(GROUP_TYPE::POTAL).size())
		{
			CPotal* pPotal = new CPotal;
			pPotal->SetPos(Vec2(1050.f, 650.f));
			pPotal->SetScale(Vec2(100.f, 100.f));
			AddObject(pPotal, GROUP_TYPE::POTAL);
		}
	}



}

void CScene_Job::render(HDC _dc)
{
	
	Vec2 vClientResolution = CCore::GetInst()->GetResolution();

	AlphaBlend(_dc
		, 0
		, 0
		, (int)vClientResolution.x
		, (int)vClientResolution.y
		, m_pBackground_Texture->GetDC()
		, 0
		, 0
		, (int)m_pBackground_Texture->Width()
		, (int)m_pBackground_Texture->Height()
		, m_pBackground_Texture->GetBf());

	
	//wchar_t buffer[256];
	//swprintf(buffer, sizeof(buffer), L"Mouse Position: (%d, %d)", m_cursorPos.x, m_cursorPos.y);
	//TextOutW(_dc, m_cursorPos.x, m_cursorPos.y, (LPCWSTR)buffer , wcslen(buffer));

	CScene::render(_dc);
}



void CScene_Job::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}


