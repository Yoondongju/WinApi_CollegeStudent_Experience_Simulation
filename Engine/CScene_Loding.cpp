#include "pch.h"
#include "CScene_Loding.h"
#include "CKeyMgr.h"
#include "CCamera.h"

#include "CResMgr.h"
#include "CTexture.h"

#include "CCore.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CBtnUI.h"
#include "CBird.h"

#include "CAnimator.h"
#include "CAnimation.h"
#include "CCollisionMgr.h"

#include "CSound.h"

void ChangeScene(DWORD_PTR _pa, DWORD_PTR _pa2);

CScene_Loding::CScene_Loding()
	: m_pUI(nullptr)
{

}

CScene_Loding::~CScene_Loding()
{

}


void CScene_Loding::Enter()
{
	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Lodding_BGM");
	//pNewSound->Play();
	pNewSound->PlayToBGM(true);
	//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	pNewSound->SetVolume(50.f); // 소리 볼륨 크기

	CCamera::GetInst()->FadeIn(4.f);

	// Bird Object 추가
	CObject* pBird = new CBird;
	pBird->SetName(L"Bird");
	pBird->SetPos(Vec2(640.f, 330.f));
	pBird->SetScale(Vec2(130.f, 130.f));
	AddObject(pBird, GROUP_TYPE::BIRD);

	// Bird Object 추가
	CObject* pBird2 = pBird->Clone();
	AddObject(pBird2, GROUP_TYPE::BIRD);
	
	// Bird Object 추가
	CObject* pBird3 = pBird->Clone();
	AddObject(pBird3, GROUP_TYPE::BIRD);
	
	// Bird Object 추가
	CObject* pBird4 = pBird->Clone();
	AddObject(pBird4, GROUP_TYPE::BIRD);
	
	// Bird Object 추가
	CObject* pBird5 = pBird->Clone();
	AddObject(pBird5, GROUP_TYPE::BIRD);
	
	// Bird Object 추가
	CObject* pBird6 = pBird->Clone();
	AddObject(pBird6, GROUP_TYPE::BIRD);


	// 배경 이미지 입히기
	//m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"Loding_Scene", L"texture\\Loding_scene.png");
	

	CTexture* pStartBtn = CResMgr::GetInst()->LoadTexture(L"SatrtBtn", L"texture\\ready.png");
	CTexture* pExitBtn = CResMgr::GetInst()->LoadTexture(L"ExitBtn", L"texture\\exit.png");

	// UI 만들어보기
	Vec2  vResolution = CCore::GetInst()->GetResolution();

	// Panel UI 생성
	CUI* pPanelUI = new CPanelUI(PANEL_UI_TYPE::DEFAULT_UI);

	pPanelUI->SetName(L"Loding_MenuUI");
	pPanelUI->SetScale(Vec2(300.f, 360.f));
	pPanelUI->SetPos(Vec2(vResolution.x/2.f , vResolution.y/2.f +100.f));
	pPanelUI->SetMovement(false);   // 원래 못 움직이는데 테스트용으로 일단 움직일수 있게 해놓음

	// Panel UI의 자식 BtnUI생성
	CBtnUI* pBtn_game_start = new CBtnUI;
	pBtn_game_start->SetName(L"Loding_BtnUI");
	pBtn_game_start->SetScale(Vec2(200.f, 60.f));
	pBtn_game_start->SetOffset(Vec2(0.f, 0.f));
	pBtn_game_start->SetUItex(pStartBtn);
	pBtn_game_start->SetHasTexture(true);
	pBtn_game_start->SetPos(pBtn_game_start->GetOffset());
	pBtn_game_start->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::LODING_2, 0); // 콜백함수에 내가 사용할 함수를 등록해놓음
	pPanelUI->AddChild(pBtn_game_start);

	// CBtnUI* pBtn_game_continue = new CBtnUI;
	// pBtn_game_continue->SetName(L"Loding_BtnUI");
	// pBtn_game_continue->SetScale(Vec2(200.f, 60.f));
	// pBtn_game_continue->SetOffset(Vec2(0.f, 0.f));
	// pBtn_game_continue->SetPos(pBtn_game_continue->GetOffset());
	// //pBtnUI->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);  // 여긴 좀 생각 해봐야함
	// pPanelUI->AddChild(pBtn_game_continue);

	//CBtnUI* pBtn_game_Exit = new CBtnUI;
	//pBtn_game_Exit->SetName(L"Loding_BtnUI");
	//pBtn_game_Exit->SetScale(Vec2(200.f, 60.f));
	//pBtn_game_Exit->SetOffset(Vec2(0.f, 100.f));
	//pBtn_game_Exit->SetPos(pBtn_game_Exit->GetOffset());
	//pBtn_game_Exit->SetUItex(pExitBtn);
	//pBtn_game_Exit->SetHasTexture(true);
	//pBtn_game_Exit->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Loding::Exit_game);
	//pPanelUI->AddChild(pBtn_game_Exit);
	//
	AddObject(pPanelUI, GROUP_TYPE::UI);

	


	// Camera LookAt지정
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
}

void CScene_Loding::Exit()
{
	DeleteAll();
	CCollisionMgr::GetInst()->Reset();

}

void CScene_Loding::update()
{
	CScene::update();

	if(KEY_CHECK(KEY::Z, KEY_STATE::TAP))
	{
		ChangeScene(SCENE_TYPE::LODING_2);
	}

}

void CScene_Loding::render(HDC _dc)
{
	// 배경만 그려줄거임 
	// BitBlt(_dc
	// 	, int(0)
	// 	, int(0)
	// 	, int(m_pBackground_Texture->Width())
	// 	, int(m_pBackground_Texture->Height())
	// 	, m_pBackground_Texture->GetDC()
	// 	, 0
	// 	, 0
	// 	, SRCCOPY);

	

	// Scene에 있는 Obj들 그려줄거임
	CScene::render(_dc);
}








