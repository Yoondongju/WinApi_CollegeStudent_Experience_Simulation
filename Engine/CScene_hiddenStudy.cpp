#include "pch.h"
#include "CScene_hiddenStudy.h"

#include "CSound.h"

#include "CResMgr.h"
#include "CCore.h"
#include "CGround.h"

#include "CCollider.h"

#include "CCamera.h"

#include "CCollisionMgr.h"

#include "CBook.h"

#include "CTexture.h"

#include "CSound.h"

#include "CBtnUI.h"
#include "CTalkUI.h"

CScene_hiddenStudy::CScene_hiddenStudy()
{
}

CScene_hiddenStudy::~CScene_hiddenStudy()
{
}

void CScene_hiddenStudy::update()
{
	CScene::update();

	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);


	static char num = 5;
	m_pBook->SetMyConversationID(num);

	CTalkUI* pMainUI = GetMainTalkUI();

	if (nullptr != pMainUI)
	{
		if (pMainUI->IsDead())
		{
			if (L"정답입니다. 다음단계로 넘어가세요." == pMainUI->GetCurConversation().text)
				num++;

			if (true == pMainUI->GetCurConversation().MondissApearTriger)
			{
				ChangeScene(SCENE_TYPE::STAGE_LOBBY);
			}
		}

	}

}

void CScene_hiddenStudy::render(HDC _dc)
{
	Vec2 vClientResolution = CCore::GetInst()->GetResolution();


	CObject* _pPlayer = GetPlayer();


	Vec2 vPos = _pPlayer->GetPos();
	Vec2 vScale = _pPlayer->GetScale();



	Vec2 renderLT = vPos - 500.f;
	Vec2 renderRB = vPos + 500.f;


	if (renderLT.x <= 0.f)
	{
		renderRB.x += abs(renderLT.x);
		renderLT.x = 0.f;
	}
	else if (renderRB.x >= vClientResolution.x)
	{
		renderLT.x -= abs(vClientResolution.x - renderRB.x);
		renderRB.x = vClientResolution.x;
	}


	AlphaBlend(_dc
		, 0
		, 0
		, (int)vClientResolution.x   // 출력할 가로크기
		, (int)vClientResolution.y  // 출력할 세로크기
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

void CScene_hiddenStudy::Enter()
{

	CCamera::GetInst()->FadeIn(3.f);
	CCamera::GetInst()->SetTarget(nullptr);
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(Vec2(vResolution / 2.f));


	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"HiddenStudy_BGM");
	//pNewSound->Play();
	pNewSound->PlayToBGM(true);
	//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	pNewSound->SetVolume(50.f); // 소리 볼륨 크기


	// ================================== 벽 만들기 ================================
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
	pOutline5->SetPos(Vec2(vResolution.x / 2.f, 525.f));
	pOutline5->SetScale(Vec2(vResolution.x, 1.f));
	pOutline5->GetCollider()->SetScale(Vec2(pOutline5->GetScale()));
	pOutline4->SetCamAffectedObj(false);
	AddObject(pOutline5, GROUP_TYPE::GROUND);

	// ================================== 벽 만들기 ================================


	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"hiddenStudy_Scene", L"texture\\hiddenStudy_scene.png");

	

	//pG_Player = new CPlayer();				 // 임시작업 여기 부분 지워야함
	pG_Player->SetScale(Vec2(50.f, 75.f));	 // 임시작업 여기 부분 지워야함
	pG_Player->SetPos(Vec2(65.f, 565.f));
	pG_Player->SetCurState(PLAYER_STATE::IDLE);
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);


	
	
	CBook* pQuizeBook = new CBook;
	pQuizeBook->SetName(L"QuizBook");
	pQuizeBook->SetMyConversationID(5);
	m_pBook = pQuizeBook;


	CBtnUI* pBookBtn = new CBtnUI;
	pBookBtn->SetName(L"Book_UI");
	pBookBtn->SetScale(Vec2(150.f, 150.f));
	pBookBtn->SetPos(Vec2(640.f,300.f));
	pBookBtn->SetMovement(false);
	pBookBtn->SetHasText(false);
	pBookBtn->SetHasTexture(false);
	pBookBtn->SetClickedCallBack(pQuizeBook, (OBJECT_MEMFUNC)&CBook::CreateQuizUI);
	CreateObject(pBookBtn, GROUP_TYPE::UI); // 등록해주는 느낌


	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::POTAL);

}

void CScene_hiddenStudy::Exit()
{
	DeleteAll();
	CCollisionMgr::GetInst()->Reset();
}


