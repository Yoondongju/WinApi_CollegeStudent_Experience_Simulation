#include "pch.h"
#include "CScene_MT.h"

#include "CCollisionMgr.h"
#include "CCamera.h"

#include "CGround.h"

#include "CCore.h"

#include "CCollider.h"
#include "CSound.h"
#include "CResMgr.h"

#include "CTexture.h"
#include "CTalkUI.h"

#include "CItem.h"
#include "CAnimator.h"

#include "AI.h"
#include "CMonster.h"
#include "CCodeModeState.h"
#include "CCodeModeMT.h"
#include "CPatrol.h"

bool CScene_MT::m_endMT = false;

CScene_MT::CScene_MT()
{

}

CScene_MT::~CScene_MT()
{

}


void CScene_MT::Enter()
{
	if (nullptr == m_MonologueTalkUI)  // 맨처음 입장시
	{
		if (nullptr == pG_Player) // Loding 2 복구하면 지워야함
		{
			pG_Player = new CPlayer(PLAYER_MBTI::INTJ);  //  << 임시작업용 
		}
	
		CSound* pNewSound = CResMgr::GetInst()->FindSound(L"MT_going_BGM");
		pNewSound->Play();
		pNewSound->PlayToBGM(true);
		//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
		pNewSound->SetVolume(50.f); // 소리 볼륨 크기
	
	
		Vec2 vResolution = CCore::GetInst()->GetResolution();
		//CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Mono", L"texture\\Mono.png");
	
		CTalkUI* pTalkUI = new CTalkUI;
		pTalkUI->SetName(L"Drinking_Talk_UI");
		pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f));
		pTalkUI->SetScale(Vec2(UI_WIDTH, 500.f)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
		//pTalkUI->SetUItex(UItex);
		pTalkUI->SetMovement(false);
		pTalkUI->SetHasText(true);
		pTalkUI->SetHasTexture(false);
		pTalkUI->CreateChildUI(pTalkUI);
		pTalkUI->SetMyConversationID(-5);
		pTalkUI->SetNpcOwner(pTalkUI);
		m_MonologueTalkUI = pTalkUI;
	
		CreateObject(pTalkUI, GROUP_TYPE::UI);
		return;
	}


	if (nullptr == pG_Player) // Loding 2 복구하면 지워야함
	{
		pG_Player = new CPlayer(PLAYER_MBTI::INTJ);  //  << 임시작업용 
	}

	//CSound* pNewSound = CResMgr::GetInst()->FindSound(L"MT_going_BGM");
	//pNewSound->Play();
	//pNewSound->PlayToBGM(true);
	////pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	//pNewSound->SetVolume(50.f); // 소리 볼륨 크기
	pG_Player->SetScale(Vec2(50.f, 75.f));
	pG_Player->SetPos(Vec2(50.f, 700.f));
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
	pG_Player->SetMyConversationID(0);
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);

	pG_Player->SetspeechBubble();

	CCamera::GetInst()->FadeIn(4.f);
	
	
	
	






	// ================================== 벽 만들기 ================================

	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CGround* pOutline = new CGround;
	pOutline->SetPos(Vec2(vResolution.x / 2.f, 500.f));
	pOutline->SetScale(Vec2(vResolution.x, 130.f));
	pOutline->GetCollider()->SetScale(Vec2(pOutline->GetScale()));
	AddObject(pOutline, GROUP_TYPE::GROUND);


	CGround* pOutline2 = new CGround;
	pOutline2->SetPos(Vec2(vResolution.x - 5.f, vResolution.y / 2.f));
	pOutline2->SetScale(Vec2(10.f, vResolution.y));
	pOutline2->GetCollider()->SetScale(Vec2(pOutline2->GetScale()));
	AddObject(pOutline2, GROUP_TYPE::GROUND);


	CGround* pOutline3 = new CGround;
	pOutline3->SetPos(Vec2(vResolution.x / 2.f, 750.f));
	pOutline3->SetScale(Vec2(vResolution.x, 30.f));
	pOutline3->GetCollider()->SetScale(Vec2(pOutline3->GetScale()));
	AddObject(pOutline3, GROUP_TYPE::GROUND);


	CGround* pOutline4 = new CGround;
	pOutline4->SetPos(Vec2(5.f, vResolution.y / 2.f));
	pOutline4->SetScale(Vec2(10.f, vResolution.y));
	pOutline4->GetCollider()->SetScale(Vec2(pOutline4->GetScale()));
	AddObject(pOutline4, GROUP_TYPE::GROUND);
	// ================================== 벽 만들기 ================================

	


	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"MT_scene", L"texture\\MT_scene.png");




	// PlayerInfo 변화함

	// Camera Look 지정

	CCamera::GetInst()->SetLookAt(Vec2(vResolution / 2.f));
	// CCamera::GetInst()->SetTarget(pG_Player);

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);

	// start(); 필요시 구현해서 사용 (아직 미구현)

	m_MonologueTalkUI = nullptr;
}


void CScene_MT::update()
{
	if (nullptr != m_MonologueTalkUI)
	{
		if (m_MonologueTalkUI->IsDead())
		{
			this->Enter(); // 다시 들어감	
		}
	}


	CTalkUI* pMainUI = GetMainTalkUI();
	static CItem* pBus = nullptr;

	static bool bCheck = false;
	static bool bMainMTCheck = false;
	static bool bstudentPresidentCheck = false;


	if (pMainUI)
	{
		if (pMainUI->IsDead())
		{
			static CMonster* pPresident = nullptr;
			static CMonster* pVicePresident = nullptr;

			if (0 == pG_Player->GetMyConversationID())
			{
				pBus = new CItem();
				pBus->SetName(L"Bus");
				pBus->SetScale(Vec2(230.f, 140.f));
				pBus->SetPos(Vec2(1300.f, 660.f));
				pBus->CreateAnimator();
				pBus->GetAnimator()->Play(L"Bus_updown", true);
				CreateObject(pBus, GROUP_TYPE::ITEM);
			}

			else if (7 == pMainUI->GetNpcOwner()->GetMyConversationID())  // id가 5인값은 학생회장, 부학생회장 나왔을때임 // 여기가 최종 대화 끝나고 Main MT 넘어갈때임
			{
				bMainMTCheck = true;
				CCamera::GetInst()->FadeOut(3.f);			
			}

			else if (1 == pG_Player->GetMyConversationID())
			{
				pG_Player->SetMyConversationID(-1);
				DeleteObject(pBus);
				pBus = nullptr;
				CCamera::GetInst()->FadeOut(3.f);
			}

			else if (2 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				CObject* pObj = pMainUI->GetNpcOwner();
				CMonster* pMon = (CMonster*)pObj;
				pMon->SetIsDeleteAI(true);
			}


			else if (4 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				if (!bstudentPresidentCheck)
				{
					AI* pAI = new AI;
					pAI->AddState(new CCodeModeMT);
					pAI->SetCurState(MON_STATE::CODEMODE_MT);
					pPresident = new CMonster;
					pPresident->SetName(L"President");
					pPresident->SetPos(Vec2(-10.f, 515.f));
					pPresident->SetEarlyPos(pPresident->GetPos());
					pPresident->SetScale(Vec2(70.f, 83.f));
					pPresident->SetAI(pAI);
					pPresident->SetCurState(CUSTOMER_STATE::WALK);
					pPresident->GetAnimator()->init_Animation();
					pPresident->SetMyConversationID(5);
					pPresident->SetDissapear_conversation(false);
					pPresident->SetZoomInMon(false);
					pPresident->SetTarGetPos(Vec2(660.f, 545.f));
					pPresident->SetSpeed(40.f);
					pPresident->GetAI()->GetState(MON_STATE::CODEMODE_MT)->SetcheckState(true);
					AddObject(pPresident, GROUP_TYPE::MONSTER);


					AI* pAI2 = new AI;
					pAI2->AddState(new CCodeModeMT);
					pAI2->SetCurState(MON_STATE::CODEMODE_MT);
					pVicePresident = new CMonster;
					pVicePresident->SetName(L"VicePresident");
					pVicePresident->SetPos(Vec2(-10.f, 515.f));
					pVicePresident->SetEarlyPos(pVicePresident->GetPos());
					pVicePresident->SetScale(Vec2(55.f, 75.f));
					pVicePresident->SetAI(pAI2);
					pVicePresident->SetCurState(CUSTOMER_STATE::WALK);
					pVicePresident->GetAnimator()->init_Animation();
					pVicePresident->SetMyConversationID(5);
					pVicePresident->SetDissapear_conversation(false);
					pVicePresident->SetZoomInMon(false);
					pVicePresident->SetTarGetPos(Vec2(600.f, 545.f));
					pVicePresident->SetSpeed(30.f);
					pVicePresident->GetAI()->GetState(MON_STATE::CODEMODE_MT)->SetcheckState(true);
					AddObject(pVicePresident, GROUP_TYPE::MONSTER);

					bstudentPresidentCheck = true;
				}

			}


			else if (5 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				pPresident->SetMyConversationID(6);	
				Waiting_Time(0.5f);

				pPresident->GetAI()->GetState(MON_STATE::CODEMODE_MT)->SetcheckState(true);
				
			}

			else if (6 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				pPresident->SetMyConversationID(7);
				Waiting_Time(0.5f);

				CSound* pNewSound = CResMgr::GetInst()->FindSound(L"MT_dance_BGM");
				pNewSound->Play();
				pNewSound->PlayToBGM(true);
				//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
				pNewSound->SetVolume(50.f); // 소리 볼륨 크기

				pPresident->GetAI()->GetState(MON_STATE::CODEMODE_MT)->SetcheckState(true);

				pPresident->SetCurState(CUSTOMER_STATE::DANCE);
				pVicePresident->SetCurState(CUSTOMER_STATE::DANCE);
			}



		}


		else
		{
			static CMonster* pCustomer = nullptr;

			if (pMainUI->GetCurConversation().text == L"나: (옆자리에 앉으며)\n너 근데 얼굴이 왜그래..?")
			{										  
				
				pG_Player->SetCurState(PLAYER_STATE::SIT);
			}

			else if (pMainUI->GetCurConversation().text == L"			(이야기를 주고 받던중 친구 한명이 자리를 못앉고 있다..)\n나: 어떡하지?..")
			{
				static bool check = false;
				if (!check)
				{
					AI* pAI = new AI;
					pAI->AddState(new CPatrol);
					pAI->AddState(new CCodeModeState);
					pAI->SetCurState(MON_STATE::PATROL);
					pCustomer = new CMonster;
					pCustomer->SetName(L"OutsideBoy");
					pCustomer->SetPos(Vec2(600.f, 515.f));
					pCustomer->SetEarlyPos(pCustomer->GetPos());
					pCustomer->SetScale(Vec2(35.f, 70.f));
					pCustomer->SetAI(pAI);
					pCustomer->SetCurState(CUSTOMER_STATE::WALK);
					pCustomer->GetAnimator()->init_Animation();
					pCustomer->SetMyConversationID(4);
					pCustomer->SetDissapear_conversation(false);
					pCustomer->SetZoomInMon(false);
					AddObject(pCustomer, GROUP_TYPE::MONSTER);
					check = true;
				}
			}

			else if (pMainUI->GetCurConversation().text == L"아싸: 힝.. 난 또 혼자야...")
			{		
				bool static check = false;
				if (!check)
				{
					pCustomer->GetAI()->SetCurState(MON_STATE::CODEMODE);	 // Change 상태에서 Targer Pos  false 체크 햇음
					pCustomer->SetTarGetPos(Vec2(775.f, 515.f));
					pCustomer->SetSpeed(45.f);
					pG_Player->SetAuto(true);
					pG_Player->SetAutoPos(Vec2(1090.f, pG_Player->GetPos().y));
					check = true;
				}
			}

			else if (pMainUI->GetCurConversation().text == L"아싸: 고마워!! 옆으로 갈게!")
			{
				bool static check = false;
				if (!check)
				{
					pCustomer->GetAI()->SetCurState(MON_STATE::CODEMODE);
					pCustomer->SetTarGetPos(Vec2(1075.f, 515.f));
					pCustomer->SetSpeed(45.f);
					pG_Player->SetAuto(true);
					pG_Player->SetAutoPos(Vec2(1090.f, pG_Player->GetPos().y));
					check = true;
				}
			}

		}
	}

	else  // UI 없을때
	{
		if (!pBus && CCamera::GetInst()->IsAllCamEffect_End() && !bCheck)
		{
			bCheck = true;
			m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"Bus_scene", L"texture\\Bus_scene.png");
			pG_Player->SetPos(Vec2(90.f, 515.f));
			pG_Player->SetCurState(PLAYER_STATE::IDLE);

			DeleteGroup(GROUP_TYPE::GROUND);  // 충돌체 바꾸기


			Vec2 vResolution = CCore::GetInst()->GetResolution();

			CGround* pOutline = new CGround;
			pOutline->SetPos(Vec2(vResolution.x / 2.f, 425.f));
			pOutline->SetScale(Vec2(vResolution.x, 5.f));
			pOutline->GetCollider()->SetScale(Vec2(pOutline->GetScale()));
			AddObject(pOutline, GROUP_TYPE::GROUND);


			CGround* pOutline2 = new CGround;
			pOutline2->SetPos(Vec2(1180.f, vResolution.y / 2.f));
			pOutline2->SetScale(Vec2(10.f, vResolution.y));
			pOutline2->GetCollider()->SetScale(Vec2(pOutline2->GetScale()));
			AddObject(pOutline2, GROUP_TYPE::GROUND);


			CGround* pOutline3 = new CGround;
			pOutline3->SetPos(Vec2(vResolution.x / 2.f, 590.f));
			pOutline3->SetScale(Vec2(vResolution.x, 30.f));
			pOutline3->GetCollider()->SetScale(Vec2(pOutline3->GetScale()));
			AddObject(pOutline3, GROUP_TYPE::GROUND);


			CGround* pOutline4 = new CGround;
			pOutline4->SetPos(Vec2(50.f, vResolution.y / 2.f));
			pOutline4->SetScale(Vec2(10.f, vResolution.y));
			pOutline4->GetCollider()->SetScale(Vec2(pOutline4->GetScale()));
			AddObject(pOutline4, GROUP_TYPE::GROUND);


			

		
			AI* pAI6 = new AI;
			pAI6->AddState(new CCodeModeState);
			pAI6->SetCurState(MON_STATE::CODEMODE);
			CCodeModeState* pCodeModestate7 = (CCodeModeState*)(pAI6->GetState(MON_STATE::CODEMODE));
			pCodeModestate7->SetOnlyRecognize(true);  // 움직임 없이 인식만 하는설정

			AI* pAI7 = new AI;
			pAI7->AddState(new CCodeModeState);
			pAI7->SetCurState(MON_STATE::CODEMODE);
			CCodeModeState* pCodeModestate8 = (CCodeModeState*)(pAI7->GetState(MON_STATE::CODEMODE));
			pCodeModestate8->SetOnlyRecognize(true);  // 움직임 없이 인식만 하는설정

			// 위치 잡아주고 앉는거 넣어주고

			// 학생 위치 잡아주기 좌표 


			CMonster* pUnknown_friend_3 = new CMonster;
			pUnknown_friend_3->SetName(L"unknown_friend");
			pUnknown_friend_3->SetPos(Vec2(205.f, 520.f));
			pUnknown_friend_3->SetEarlyPos(pUnknown_friend_3->GetPos());
			pUnknown_friend_3->SetScale(Vec2(50.f, 60.f));
			pUnknown_friend_3->SetFineTarget(false);
			pUnknown_friend_3->SetAI(pAI6);
			pUnknown_friend_3->SetDissapear_conversation(false);
			pUnknown_friend_3->SetCurState(CUSTOMER_STATE::SIT);
			pUnknown_friend_3->SetDir(CUSTOMER_DIR::LEFT);
			pUnknown_friend_3->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
			pUnknown_friend_3->SetMyConversationID(3);
			AddObject(pUnknown_friend_3, GROUP_TYPE::MONSTER);



			CMonster* pUnknown_friend_4 = new CMonster;
			pUnknown_friend_4->SetName(L"unknown_friend_2");
			pUnknown_friend_4->SetPos(Vec2(465.f, 515.f));
			pUnknown_friend_4->SetEarlyPos(pUnknown_friend_4->GetPos());
			pUnknown_friend_4->SetScale(Vec2(50.f, 60.f));
			pUnknown_friend_4->SetFineTarget(false);
			pUnknown_friend_4->SetDissapear_conversation(false);
			pUnknown_friend_4->SetCurState(CUSTOMER_STATE::SIT);
			pUnknown_friend_4->SetDir(CUSTOMER_DIR::LEFT);
			pUnknown_friend_4->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
			AddObject(pUnknown_friend_4, GROUP_TYPE::MONSTER);



			CMonster* pUnknown_friend_6 = new CMonster;
			pUnknown_friend_6->SetName(L"unknown_friend_4");
			pUnknown_friend_6->SetPos(Vec2(992.f, 477.f));
			pUnknown_friend_6->SetEarlyPos(pUnknown_friend_6->GetPos());
			pUnknown_friend_6->SetScale(Vec2(50.f, 60.f));
			pUnknown_friend_6->SetFineTarget(false);
			pUnknown_friend_6->SetDissapear_conversation(false);
			pUnknown_friend_6->SetCurState(CUSTOMER_STATE::SIT);
			pUnknown_friend_6->SetDir(CUSTOMER_DIR::RIGHT);
			pUnknown_friend_6->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
			AddObject(pUnknown_friend_6, GROUP_TYPE::MONSTER);





			CMonster* pfriend_2 = new CMonster;
			pfriend_2->SetName(L"CuteBoy");
			pfriend_2->SetPos(Vec2(1150.f, 477.f));
			pfriend_2->SetEarlyPos(pfriend_2->GetPos());
			pfriend_2->SetScale(Vec2(50.f, 70.f));
			pfriend_2->SetAI(pAI7);
			pfriend_2->SetFineTarget(false);
			pfriend_2->SetDissapear_conversation(false);
			pfriend_2->SetCurState(CUSTOMER_STATE::SIT);
			pfriend_2->SetDir(CUSTOMER_DIR::LEFT);
			pfriend_2->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
			pfriend_2->SetMyConversationID(2);
			pfriend_2->SetZoomInMon(false);  // 이걸로 카메라 줌인 안시킬수잇음
			AddObject(pfriend_2, GROUP_TYPE::MONSTER);

		}

		else if (CCamera::GetInst()->IsAllCamEffect_End() && bMainMTCheck)
		{
			ChangeScene(SCENE_TYPE::MAINMT);
		}

	}
		




	CScene::update();

	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);

}

void CScene_MT::render(HDC _dc)
{
	Vec2 vClientResolution = CCore::GetInst()->GetResolution();

	if (m_pBackground_Texture)
	{
		AlphaBlend(_dc
			, 0
			, 0
			, (int)vClientResolution.x  // 출력할 가로크기
			, (int)vClientResolution.y  // 출력할 세로크기
			, m_pBackground_Texture->GetDC()
			, 0
			, 0
			, (int)m_pBackground_Texture->Width()
			, (int)m_pBackground_Texture->Height()
			, m_pBackground_Texture->GetBf());
	}


	//wchar_t buffer[256];
	//swprintf(buffer, sizeof(buffer), L"Mouse Position: (%d, %d)", m_cursorPos.x, m_cursorPos.y);
	//TextOutW(_dc, m_cursorPos.x, m_cursorPos.y, (LPCWSTR)buffer, wcslen(buffer));

	CScene::render(_dc);

}



void CScene_MT::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}
