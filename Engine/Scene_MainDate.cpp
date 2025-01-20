#include "pch.h"

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

#include "CTimeMgr.h"

#include "AI.h"
#include "CMonster.h"
#include "CCodeModeState.h"
#include "CCodeModeMT.h"
#include "CPatrol.h"

#include "CAnimator.h"
#include "Scene_MainDate.h"

#include "CItem.h"
#include "CAnimation.h"

#include "CBtnUI.h"

static CBtnUI* pBookBtn = nullptr;
bool Scene_MainDate::m_bRestart = false;

Scene_MainDate::Scene_MainDate()
	: m_bQuestSuces(false)
{

}

Scene_MainDate::~Scene_MainDate()
{

}


void Scene_MainDate::Enter()
{
	if (nullptr == pG_Player) // Loding 2 복구하면 지워야함
	{
		pG_Player = new CPlayer(PLAYER_MBTI::INTJ);  //  << 임시작업용 
	}

	pG_Player->SetScale(Vec2(50.f, 75.f));
	pG_Player->SetPos(Vec2(50.f, 680.f));
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
	pG_Player->SetMyConversationID(0);
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);


	CCamera::GetInst()->FadeIn(3.f);


	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Date_going_BGM");
	pNewSound->Play();
	pNewSound->PlayToBGM(true);
	//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	pNewSound->SetVolume(50.f); // 소리 볼륨 크기



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




	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"MainDate_scene", L"texture\\Date_scene.png");



	CCamera::GetInst()->SetLookAt(Vec2(vResolution / 2.f));
	// CCamera::GetInst()->SetTarget(pG_Player);

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	
}

void Scene_MainDate::update()
{
	static bool bCheck = false;
	static bool bCheck_2 = false;
	static bool bTimeCheck = false;
	static bool bTimeCheck_2 = false;
	static CMonster* pDategirl = nullptr;
	static CItem* pNum = nullptr;
	static bool bHiddenQuest = false;

	static bool bFailure = false;
	static bool bFinal = false;
	static bool abc = false;
	


	if (m_bRestart) // Codemode 에서 이미 false가 되어버림 거기서 체크해주면 안댐
	{
		static bool bfirst = false;
		if (!bfirst)
		{
			bCheck = false;
			bCheck_2 = false;
			bTimeCheck = false;
			bTimeCheck_2 = false;
			pDategirl = nullptr;
			bHiddenQuest = false;
			bFailure = false;
			bFinal = false;
			abc = false;
			pNum = nullptr;
			bfirst = true;			
		}
		
	}


	if (!bCheck && CCamera::GetInst()->IsAllCamEffect_End())
	{
		pG_Player->SetCurState(PLAYER_STATE::IDLE);
		pG_Player->SetAuto(true);
		pG_Player->SetOnlyMove(false);
		pG_Player->SetAutoPos(Vec2(562.f, pG_Player->GetPos().y));  // 의자 앉혀주기
		bCheck = true;

	}


	if (bCheck && !bCheck_2 && !pG_Player->GetAuto()) // 도착했어? == 자동이동 끝났어? (자리에 앉은 시점)
	{
		pG_Player->SetspeechBubble();
		bCheck_2 = true;
	}

	if (!pG_Player->GetAuto())
	{
		pG_Player->SetPreState(PLAYER_STATE::SIT);
		pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
	}

	if (bTimeCheck)
	{
		static float fTime = 0.f;
		fTime += fDT;

		if (fTime > 2.f)
		{
			Vec2 vResolution = CCore::GetInst()->GetResolution();
			CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Job_Talk_UI", L"texture\\TalkUI.png");

			CTalkUI* pTalkUI = new CTalkUI;
			pTalkUI->SetName(L"Job_Talk_UI");
			pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f + vResolution.y / 4.f + 50));
			pTalkUI->SetScale(Vec2(UI_WIDTH, UI_HEIGHT)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
			pTalkUI->SetUItex(UItex);
			pTalkUI->SetMovement(false);
			pTalkUI->SetHasText(true);
			pTalkUI->SetHasTexture(true);
			pTalkUI->CreateChildUI(pTalkUI);
			pTalkUI->SetNpcOwner(pDategirl);  
			CreateObject(pTalkUI, GROUP_TYPE::UI);
			
			fTime = 0.f;
			bTimeCheck = false;
		}
	}

	if (bTimeCheck_2)
	{
		static float fTime = 0.f;
		fTime += fDT;

		if (fTime > 2.5f)
		{
			CCamera::GetInst()->FadeOut(3.f);
			bTimeCheck_2 = false;
			bHiddenQuest = true;
		}
	}

	if (bCheck_2 && bHiddenQuest && CCamera::GetInst()->IsAllCamEffect_End())
	{
		CreateHiddenQuest();
		bHiddenQuest = false;
	}


	if (bFinal && CCamera::GetInst()->IsAllCamEffect_End())
	{
		ChangeScene(SCENE_TYPE::STAGE_LOBBY);
	}
	else if (bFailure && CCamera::GetInst()->IsAllCamEffect_End())
	{
		if (!abc)
		{
			DeleteObject(pDategirl);

			CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"Note", L"texture\\Note.png");
			CItem* pNum = new CItem();  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
			pNum->SetName(L"Note");
			pNum->SetScale(Vec2(40.f, 40.f));
			pNum->SetPos(Vec2(400.f, 665.f));
			pNum->SetTexItem(pTex);
			CreateObject(pNum, GROUP_TYPE::ITEM);

			pBookBtn = new CBtnUI;
			pBookBtn->SetName(L"Note_UI");
			pBookBtn->SetScale(Vec2(40.f, 40.f));
			pBookBtn->SetPos(Vec2(pNum->GetPos()));
			pBookBtn->SetMovement(false);
			pBookBtn->SetHasText(false);
			pBookBtn->SetHasTexture(false);
			pBookBtn->SetClickedCallBack(this, (SCENE_MEMFUNC)&Scene_MainDate::CreateNoteUI);
			CreateObject(pBookBtn, GROUP_TYPE::UI); // 등록해주는 느낌
			abc = true;
		}
		
	}
	
	
	


	CTalkUI* pMainUI = GetMainTalkUI();
	

	if (pMainUI)
	{
		if (pMainUI->IsDead())
		{

			if (abc && CCamera::GetInst()->IsAllCamEffect_End())
			{
				ChangeScene(SCENE_TYPE::STAGE_LOBBY);
			}
			

			if (0 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
			
				AI* pAI = new AI;
				pAI->AddState(new CCodeModeMT);
				pAI->SetCurState(MON_STATE::CODEMODE_MT);
				pDategirl = new CMonster;
				pDategirl->SetName(L"DateGirl");
				pDategirl->SetPos(Vec2(-10.f, 680.f));
				pDategirl->SetEarlyPos(pDategirl->GetPos());
				pDategirl->SetScale(Vec2(50.f, 70.f));
				pDategirl->SetAI(pAI);
				pDategirl->SetCurState(CUSTOMER_STATE::WALK);
				pDategirl->GetAnimator()->init_Animation();
				pDategirl->SetMyConversationID(1);
				pDategirl->SetDissapear_conversation(false);
				pDategirl->SetZoomInMon(false);
				pDategirl->SetTarGetPos(Vec2(400.f, 680.f));
				pDategirl->SetSpeed(40.f);
				AddObject(pDategirl, GROUP_TYPE::MONSTER);
			}
		

			else if (1 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				pDategirl->SetMyConversationID(2);
				bTimeCheck = true;		
			}

			else if (2 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				pDategirl->SetTarGetPos(Vec2(-50.f, 680.f));
				pDategirl->SetSpeed(70.f);
				bTimeCheck_2 = true;
			}

			else if (3 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				pNum = new CItem();  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
				pNum->SetName(L"Number");
				pNum->SetScale(Vec2(230.f, 140.f));
				pNum->SetPos(Vec2(640.f, 120.f));
				pNum->CreateAnimator();				// CITEM은 생성자에서 애니메이션 만들지 않아서 여기서 바로 init 애니메이션 호출함 
				pNum->GetAnimator()->Play(L"Number", false);
				CreateObject(pNum, GROUP_TYPE::ITEM);

				pBookBtn = new CBtnUI;
				pBookBtn->SetName(L"Book_UI");
				pBookBtn->SetScale(Vec2(110.f, 110.f));
				pBookBtn->SetPos(Vec2(795.f, 460.f));
				pBookBtn->SetMovement(false);
				pBookBtn->SetHasText(false);
				pBookBtn->SetHasTexture(false);
				pBookBtn->SetClickedCallBack(this,(SCENE_MEMFUNC)&Scene_MainDate::CreateTalkUI);
				CreateObject(pBookBtn, GROUP_TYPE::UI); // 등록해주는 느낌
			}

			else if (4 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				// 여기서 히든 퀘스트를 성공하지 못했다면 불이익을 줄것임
				ChangeScene(SCENE_TYPE::MAINDATE);
				m_bRestart = true;
			}

			else if (5 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				bTimeCheck = true;
				pDategirl->SetMyConversationID(6);
			}

			else if (6 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				if (pMainUI->GetCurConversation().text == L"나: (어쩔줄 몰라하며)\n나..화장실좀!!..")
				{
					// 쪽지 엔딩
					CCamera::GetInst()->FadeOut(3.f);
					bFailure = true;
				}
				else
				{
					bTimeCheck = true;
					pDategirl->SetMyConversationID(7);
				}
			}

			else if (7 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				CCamera::GetInst()->FadeOut(3.f);
				bFinal = true;
			}

			else if (99 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				Vec2 vResolution = CCore::GetInst()->GetResolution();
				CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Job_Talk_UI", L"texture\\TalkUI.png");

				CTalkUI* pTalkUI = new CTalkUI;
				pTalkUI->SetName(L"Job_Talk_UI");
				pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f + vResolution.y / 4.f + 50));
				pTalkUI->SetScale(Vec2(UI_WIDTH, UI_HEIGHT)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
				pTalkUI->SetUItex(UItex);
				pTalkUI->SetMovement(false);
				pTalkUI->SetHasText(true);
				pTalkUI->SetHasTexture(true);
				pTalkUI->CreateChildUI(pTalkUI);
				pTalkUI->SetMyConversationID(100);
				pTalkUI->SetNpcOwner(pTalkUI);
				CreateObject(pTalkUI, GROUP_TYPE::UI);
			}

			else if (100 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				Vec2 vResolution = CCore::GetInst()->GetResolution();
				CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Job_Talk_UI", L"texture\\TalkUI.png");

				CTalkUI* pTalkUI = new CTalkUI;
				pTalkUI->SetName(L"Job_Talk_UI");
				pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f + vResolution.y / 4.f + 50));
				pTalkUI->SetScale(Vec2(UI_WIDTH, UI_HEIGHT)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
				pTalkUI->SetUItex(UItex);
				pTalkUI->SetMovement(false);
				pTalkUI->SetHasText(true);
				pTalkUI->SetHasTexture(true);
				pTalkUI->CreateChildUI(pTalkUI);
				pTalkUI->SetMyConversationID(101);
				pTalkUI->SetNpcOwner(pTalkUI);
				CreateObject(pTalkUI, GROUP_TYPE::UI);
			}

			else if (101 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				pDategirl->SetTarGetPos(Vec2(400.f, 680.f));
				pDategirl->SetMyConversationID(5);
				bTimeCheck = true;
			}

			else if (102 == pMainUI->GetNpcOwner()->GetMyConversationID())
			{
				CCamera::GetInst()->FadeOut(3.f);
			}
		}
	}

	if(pNum && !m_bQuestSuces && pNum->GetAnimator()->GetCurAnimaion()->IsFinish())
	{
		static bool check = false;
		if (!check)
		{
			DeleteObject(pBookBtn);


			pG_Player->SetMyConversationID(4);  // 실패 

			Vec2 vResolution = CCore::GetInst()->GetResolution();
			CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"text_box", L"texture\\Textbox_8.png");

			CTalkUI* pTalkUI = new CTalkUI;
			pTalkUI->SetName(L"Job_Talk_UI");
			pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f));
			pTalkUI->SetScale(Vec2(700.f, 600.f)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
			pTalkUI->SetUItex(UItex);
			pTalkUI->SetMovement(false);
			pTalkUI->SetHasText(true);
			pTalkUI->SetHasTexture(true);
			pTalkUI->CreateChildUI(pTalkUI);
			pTalkUI->SetNpcOwner(pG_Player);
			CreateObject(pTalkUI, GROUP_TYPE::UI);
			check = true;
			m_bQuestSuces = false;
		}
	}
	




	CScene::update();

	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);
}



void Scene_MainDate::CreateHiddenQuest()
{
	pG_Player->SetMyConversationID(3);

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"text_box", L"texture\\Textbox_8.png");

	CTalkUI* pTalkUI = new CTalkUI;
	pTalkUI->SetName(L"Job_Talk_UI");
	pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f));
	pTalkUI->SetScale(Vec2(700.f, 600.f)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
	pTalkUI->SetUItex(UItex);
	pTalkUI->SetMovement(false);
	pTalkUI->SetHasText(true);
	pTalkUI->SetHasTexture(true);
	pTalkUI->CreateChildUI(pTalkUI);
	pTalkUI->SetNpcOwner(pG_Player);
	CreateObject(pTalkUI, GROUP_TYPE::UI);

}



void Scene_MainDate::render(HDC _dc)
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



void Scene_MainDate::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}



void Scene_MainDate::CreateTalkUI(DWORD_PTR _pa, DWORD_PTR _pa2)
{

	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Date_gosu_BGM");
	//pNewSound->Play();
	pNewSound->PlayToBGM(true);
	pNewSound->SetPosition(20.f); // 백분률 , 소리위치
	pNewSound->SetVolume(80.f); // 소리 볼륨 크기
	

	DeleteObject(pBookBtn);
	m_bQuestSuces = true;

	pG_Player->SetMyConversationID(99);

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Job_Talk_UI", L"texture\\TalkUI.png");

	CTalkUI* pTalkUI = new CTalkUI;
	pTalkUI->SetName(L"Job_Talk_UI");
	pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f + vResolution.y / 4.f + 50));
	pTalkUI->SetScale(Vec2(UI_WIDTH, UI_HEIGHT)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
	pTalkUI->SetUItex(UItex);
	pTalkUI->SetMovement(false);
	pTalkUI->SetHasText(true);
	pTalkUI->SetHasTexture(true);
	pTalkUI->CreateChildUI(pTalkUI);
	pTalkUI->SetNpcOwner(pG_Player);

	CreateObject(pTalkUI, GROUP_TYPE::UI);
}

void Scene_MainDate::CreateNoteUI(DWORD_PTR _pa, DWORD_PTR _pa2)
{

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"text_box", L"texture\\Textbox_8.png");

	CTalkUI* pTalkUI = new CTalkUI;
	pTalkUI->SetName(L"Job_Talk_UI");
	pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f));
	pTalkUI->SetScale(Vec2(700.f, 600.f)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
	pTalkUI->SetUItex(UItex);
	pTalkUI->SetMovement(false);
	pTalkUI->SetHasText(true);
	pTalkUI->SetHasTexture(true);
	pTalkUI->CreateChildUI(pTalkUI);
	pTalkUI->SetMyConversationID(102);
	pTalkUI->SetNpcOwner(pTalkUI);
	CreateObject(pTalkUI, GROUP_TYPE::UI);

}



