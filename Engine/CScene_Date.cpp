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

#include "AI.h"
#include "CMonster.h"
#include "CCodeModeState.h"
#include "CCodeModeMT.h"
#include "CPatrol.h"
#include "CScene_Date.h"


CScene_Date::CScene_Date()
{

}

CScene_Date::~CScene_Date()
{

}


void CScene_Date::Enter()
{
	if (nullptr == pG_Player) // Loding 2 복구하면 지워야함
	{
		pG_Player = new CPlayer(PLAYER_MBTI::INTJ);  //  << 임시작업용 
	}

	pG_Player->SetScale(Vec2(50.f, 75.f));
	pG_Player->SetPos(Vec2(50.f, 700.f));
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
	pG_Player->SetMyConversationID(0);
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);


	CCamera::GetInst()->FadeIn(4.f);


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




	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"Date_scene", L"texture\\Date_going_scene.png");



	CCamera::GetInst()->SetLookAt(Vec2(vResolution / 2.f));
	// CCamera::GetInst()->SetTarget(pG_Player);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);




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



void CScene_Date::update()
{
	CTalkUI* pMainUI = GetMainTalkUI();
	static CItem* pBus = nullptr;

	if (pMainUI)
	{
		if (pMainUI->IsDead())
		{
			if (L"나: 그래! 뭐 늦겠어? 돈도 아낄겸 버스타고가자!" == pMainUI->GetCurConversation().text)
			{
				pBus = new CItem();
				pBus->SetName(L"Bus");
				pBus->SetScale(Vec2(250.f, 140.f));
				pBus->SetPos(Vec2(1300.f, 660.f));
				pBus->CreateAnimator();
				pBus->GetAnimator()->Play(L"Bus_updown", true);
				CreateObject(pBus, GROUP_TYPE::ITEM);
			}
			else if (L"나: 첫인상이 제일 중요한데 지각하면 안돼!! 택시타자!" == pMainUI->GetCurConversation().text)
			{
				pBus = new CItem();
				pBus->SetName(L"Taxi");
				pBus->SetScale(Vec2(180.f, 130.f));
				pBus->SetPos(Vec2(1300.f, 660.f));
				pBus->CreateAnimator();
				pBus->GetAnimator()->Play(L"Taxi", true);
				CreateObject(pBus, GROUP_TYPE::ITEM);
			}
			else if (L"얼른타고 가자!" == pMainUI->GetCurConversation().text)
			{
				ChangeScene(SCENE_TYPE::MAINDATE);
			}

		}
	}





	CScene::update();

	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);
}

void CScene_Date::render(HDC _dc)
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


void CScene_Date::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}


