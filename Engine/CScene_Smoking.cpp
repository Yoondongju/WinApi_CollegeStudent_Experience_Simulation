#include "pch.h"
#include "CScene_Smoking.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CCore.h"
#include "CPotal.h"
#include "CResMgr.h"

#include "CTexture.h"

#include "CTalkUI.h"


#include "CGround.h"
#include "CCollider.h"

#include "CSound.h"

#include "AI.h"
#include "CCodeModeState.h"
#include "CMonster.h"
#include "CAnimator.h"


CScene_Smoking::CScene_Smoking()
	: m_smokingGirl(nullptr)
{
}

CScene_Smoking::~CScene_Smoking()
{
}

void CScene_Smoking::Enter()
{
	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"Smoking_scene", L"texture\\Smoking_scene.png");

	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Smoking_BGM");
	//pNewSound->Play();
	pNewSound->PlayToBGM(true);
	//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	pNewSound->SetVolume(50.f); // 소리 볼륨 크기

	// ================================== 벽 만들기 ================================
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CGround* pOutline = new CGround;
	pOutline->SetPos(Vec2(vResolution.x / 2.f, 60.f));
	pOutline->SetScale(Vec2(vResolution.x, 130.f));
	pOutline->GetCollider()->SetScale(Vec2(pOutline->GetScale()));
	AddObject(pOutline, GROUP_TYPE::GROUND);


	CGround* pOutline2 = new CGround;
	pOutline2->SetPos(Vec2(vResolution.x - 5.f, vResolution.y / 2.f));
	pOutline2->SetScale(Vec2(10.f, vResolution.y));
	pOutline2->GetCollider()->SetScale(Vec2(pOutline2->GetScale()));
	AddObject(pOutline2, GROUP_TYPE::GROUND);


	CGround* pOutline3 = new CGround;
	pOutline3->SetPos(Vec2(vResolution.x / 2.f, 615.f));
	pOutline3->SetScale(Vec2(vResolution.x, 30.f));
	pOutline3->GetCollider()->SetScale(Vec2(pOutline3->GetScale()));
	AddObject(pOutline3, GROUP_TYPE::GROUND);


	CGround* pOutline4 = new CGround;
	pOutline4->SetPos(Vec2(5.f, vResolution.y / 2.f));
	pOutline4->SetScale(Vec2(10.f, vResolution.y));
	pOutline4->GetCollider()->SetScale(Vec2(pOutline4->GetScale()));
	AddObject(pOutline4, GROUP_TYPE::GROUND);



	CGround* pOutline5 = new CGround;
	pOutline5->SetPos(Vec2(vResolution.x / 2.f, 515.f));
	pOutline5->SetScale(Vec2(vResolution.x, 1.f));
	pOutline5->GetCollider()->SetScale(Vec2(pOutline5->GetScale()));
	AddObject(pOutline5, GROUP_TYPE::GROUND);

	// ================================== 벽 만들기 ================================


	//m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"Smoking_Scene", L"texture\\Smoking_scene.png");

	CCamera::GetInst()->FadeIn(2.f);
	CCamera::GetInst()->SetTarget(nullptr);
	CCamera::GetInst()->SetLookAt(Vec2(vResolution / 2.f));
	

	//pG_Player = new CPlayer();				 // 임시작업 여기 부분 지워야함
	pG_Player->SetScale(Vec2(50.f, 75.f));	 // 임시작업 여기 부분 지워야함
	pG_Player->SetPos(Vec2(65.f, 565.f));
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);   //  << 임시작업 용

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::POTAL);

	m_bWentsmoking = true;
}

void CScene_Smoking::update()
{

	if (CCamera::GetInst()->IsCamEffect_End())
	{
		Vec2 vResolution = CCore::GetInst()->GetResolution();
		CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Job_Talk_UI", L"texture\\TalkUI.png");

		if (nullptr == m_MonologueTalkUI)
		{
			CTalkUI* pTalkUI = new CTalkUI;
			pTalkUI->SetName(L"Job_Talk_UI");
			pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f + vResolution.y / 4.f + 50));
			pTalkUI->SetScale(Vec2(UI_WIDTH, UI_HEIGHT)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
			pTalkUI->SetUItex(UItex);
			pTalkUI->SetMovement(false);
			pTalkUI->SetHasText(true);
			pTalkUI->SetHasTexture(true);
			pTalkUI->CreateChildUI(pTalkUI);
			pTalkUI->SetNpcOwner(nullptr);

			m_MonologueTalkUI = pTalkUI;
			CreateObject(pTalkUI, GROUP_TYPE::UI);
		}

	}


	if (nullptr != m_MonologueTalkUI)
	{
		if (m_MonologueTalkUI->IsDead())
		{

			const double* pPlayerInfo = pG_Player->GetPlayer_Info();


			AI* pAI = new AI;
			pAI->AddState(new CCodeModeState);
			pAI->SetCurState(MON_STATE::CODEMODE);

			///  AI ====================================
			CMonster* smokingGirl = new CMonster;
			smokingGirl->SetName(L"smokingGirl");
			smokingGirl->SetPos(Vec2(1200.f, 565.f));
			smokingGirl->SetEarlyPos(smokingGirl->GetPos());
			smokingGirl->SetScale(Vec2(50.f, 60.f));
			smokingGirl->SetAI(pAI);
			smokingGirl->SetTarGetPos(Vec2(980.f, 565.f));
			smokingGirl->SetFineTarget(false);
			smokingGirl->SetCurState(CUSTOMER_STATE::SAY);
			smokingGirl->SetDir(CUSTOMER_DIR::RIGHT);
			smokingGirl->GetAnimator()->init_Animation();

			if (1 ==  pPlayerInfo[(UINT)PLAYER_INFO::ALCOHOL_TOLERANCE])			
			{																	
				smokingGirl->SetMyConversationID(1);							
			}																	
			else if (0 == pPlayerInfo[(UINT)PLAYER_INFO::ALCOHOL_TOLERANCE])	
			{																	
				smokingGirl->SetMyConversationID(2);							
			}

			smokingGirl->SetDissapear_conversation(false);
			m_smokingGirl = smokingGirl;
			CreateObject(smokingGirl, GROUP_TYPE::MONSTER);
			m_MonologueTalkUI = nullptr;

		}


	}




	CTalkUI* UI = GetMainTalkUI();
	if (nullptr != UI)
	{
		if (!UI->IsDead())
		{
			Conversation CurConversation = UI->GetCurConversation();

			if (CurConversation.MondissApearTriger)
			{
				CMonster* smokingGirl = (CMonster*)m_smokingGirl;
				smokingGirl->SetDissapear_conversation(true);


				if (0 == GetGroupObject(GROUP_TYPE::POTAL).size())
				{
					CPotal* pPotal = new CPotal;
					pPotal->SetPos(Vec2(1170.f, 550.f));
					pPotal->SetScale(Vec2(100.f, 100.f));
					AddObject(pPotal, GROUP_TYPE::POTAL);
				}
			}
		}
	}



	CScene::update();

	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);


	if (KEY_CHECK(KEY::ENTER, KEY_STATE::TAP))
	{
		ChangeScene(SCENE_TYPE::STAGE_LOBBY);
	}


}

void CScene_Smoking::render(HDC _dc)
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
	//TextOutW(_dc, m_cursorPos.x, m_cursorPos.y, (LPCWSTR)buffer, wcslen(buffer));

	CScene::render(_dc);
}



void CScene_Smoking::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}


