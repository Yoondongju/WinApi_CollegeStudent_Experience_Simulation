#include "pch.h"
#include "CScene_Study.h"

#include "CCore.h"
#include "CResMgr.h"
#include "CTexture.h"

#include "CGround.h"
#include "CCollider.h"

#include "CCollisionMgr.h"

#include "CBook.h"

#include "CBtnUI.h"

#include "CPotal.h"

#include "CSound.h"


CScene_Study::CScene_Study()
{

}

CScene_Study::~CScene_Study()
{

}


void CScene_Study::Enter()
{
	CCamera::GetInst()->FadeIn(3.f);

	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Study_BGM");
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
	pOutline->SetCamAffectedObj(false);
	AddObject(pOutline, GROUP_TYPE::GROUND);


	CGround* pOutline2 = new CGround;
	pOutline2->SetPos(Vec2(vResolution.x - 5.f, vResolution.y / 2.f));
	pOutline2->SetScale(Vec2(10.f, vResolution.y));
	pOutline2->GetCollider()->SetScale(Vec2(pOutline2->GetScale()));
	pOutline2->SetCamAffectedObj(false);
	AddObject(pOutline2, GROUP_TYPE::GROUND);


	CGround* pOutline3 = new CGround;
	pOutline3->SetPos(Vec2(vResolution.x / 2.f, 650.f));
	pOutline3->SetScale(Vec2(vResolution.x, 30.f));
	pOutline3->GetCollider()->SetScale(Vec2(pOutline3->GetScale()));
	pOutline3->SetCamAffectedObj(false);
	AddObject(pOutline3, GROUP_TYPE::GROUND);


	CGround* pOutline4 = new CGround;
	pOutline4->SetPos(Vec2(5.f, vResolution.y / 2.f));
	pOutline4->SetScale(Vec2(10.f, vResolution.y));
	pOutline4->GetCollider()->SetScale(Vec2(pOutline4->GetScale()));
	pOutline4->SetCamAffectedObj(false);
	AddObject(pOutline4, GROUP_TYPE::GROUND);


	CGround* pOutline5 = new CGround;
	pOutline5->SetPos(Vec2(vResolution.x / 2.f, 560.f));
	pOutline5->SetScale(Vec2(vResolution.x, 1.f));
	pOutline5->GetCollider()->SetScale(Vec2(pOutline5->GetScale()));
	pOutline5->SetCamAffectedObj(false);
	AddObject(pOutline5, GROUP_TYPE::GROUND);
	// ================================== 벽 만들기 ================================

	

	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"Study", L"texture\\Study_scene.png");



	pG_Player->SetPos(Vec2(100.f, 600.f));
	pG_Player->SetCurState(PLAYER_STATE::IDLE);
	
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);

	
	//CCamera::GetInst()->SetTarget(pG_Player);
	
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::POTAL);

}

void CScene_Study::update()
{

	CObject* _pPlayer = GetPlayer();
	Vec2 vPos = _pPlayer->GetPos();

	if (vPos.x >= 935.f)
	{
		// 책 만들기 // 
		CreateBook();
		// 책 만들기 // 

		if (4 == CBook::GetBookCheckcount())
		{
			if (0 == GetGroupObject(GROUP_TYPE::POTAL).size())
			{
				CPotal* pPotal = new CPotal;
				pPotal->SetPos(Vec2(1150.f, 600.f));
				pPotal->SetScale(Vec2(100.f, 100.f));
				AddObject(pPotal, GROUP_TYPE::POTAL);
			}
		}
	}

	if (vPos.x < 935.f)
	{
		const vector<CObject*> pBookObj =  GetGroupObject(GROUP_TYPE::ITEM);
		const vector<CObject*> pUI = GetGroupObject(GROUP_TYPE::UI);
		const vector<CObject*> pPotal = GetGroupObject(GROUP_TYPE::POTAL);
		
		// 스터디 씬에서 일단 책바께 없으니까 책만 싹 지워줌
		for (size_t i = 0; i < pBookObj.size(); ++i)     //  << 책이랑 버튼갯수 동일하니까 같은 반복문에서 실행
		{
			CBtnUI* pBtn = dynamic_cast<CBtnUI*>(pUI[i]);

			if (!pBookObj[i]->IsDead() && !pBtn->IsDead())
			{
				DeleteObject(pBookObj[i]);
				DeleteObject(pBtn);
			}				
		}

		if (0 < pPotal.size())
		{
			if(!pPotal[0]->IsDead())
				DeleteObject(pPotal[0]);
		}
		
	}

	



	CScene::update();

	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);

}

void CScene_Study::render(HDC _dc)
{
	Vec2 vClientResolution = CCore::GetInst()->GetResolution();

	Vec2 vCenter = vClientResolution / 2.f;
	float xCamera = pG_Player->GetPos().x;

	float LT = xCamera - vCenter.x;

	if (LT <= 0.f)
		LT = 0.f;
	else if (LT + (int)vClientResolution.x >= (int)m_pBackground_Texture->Width())
	{
		float gap = LT + (int)vClientResolution.x - (int)m_pBackground_Texture->Width() ;
		LT -= gap;
	}


	if (m_pBackground_Texture)
	{
		AlphaBlend(_dc
			, 0
			, 0
			, (int)vClientResolution.x  // 출력할 가로크기
			, (int)vClientResolution.y  // 출력할 세로크기
			, m_pBackground_Texture->GetDC()
			, LT								  // 여기가 바뀌어야함
			, 0									  // 여기가 바뀌어야함
			, (int)vClientResolution.x
			, (int)m_pBackground_Texture->Height()// 여기가 바뀌어야함
			, m_pBackground_Texture->GetBf());

	}


	//wchar_t buffer[256];
	//swprintf(buffer, sizeof(buffer), L"Mouse Position: (%d, %d)", m_cursorPos.x, m_cursorPos.y);
	//TextOutW(_dc, m_cursorPos.x, m_cursorPos.y, (LPCWSTR)buffer, wcslen(buffer));

	CScene::render(_dc);

}




void CScene_Study::CreateBook()
{
	const vector<CObject*> pBookObj = GetGroupObject(GROUP_TYPE::ITEM);
	if (0 < pBookObj.size())
		return;

		
	pG_Player->SetMyConversationID(0);
	pG_Player->SetspeechBubble();
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);


	Vec2 vClientResolution = CCore::GetInst()->GetResolution();
	Vec2 vPos = pG_Player->GetPos();

	CObject* pBook1 = new CBook();
	pBook1->SetPos(Vec2(1120.f, 310.f));
	pBook1->SetScale(Vec2(30.f, 30.f));
	pBook1->SetName(L"book1");
	CreateObject(pBook1, GROUP_TYPE::ITEM);

	CBtnUI* pBookBtn= new CBtnUI;
	pBookBtn->SetName(L"Book_UI");
	pBookBtn->SetScale(Vec2(50.f, 50.f));
	pBookBtn->SetPos(pBook1->GetPos());
	pBookBtn->SetMovement(false);
	pBookBtn->SetHasText(false);
	pBookBtn->SetHasTexture(false);
	pBookBtn->SetClickedCallBack(pBook1, (OBJECT_MEMFUNC)&CBook::CreateStudyUI);
	CreateObject(pBookBtn, GROUP_TYPE::UI); // 등록해주는 느낌



	float bookY = pBook1->GetPos().y;

	CBook* pBook2 = new CBook();
	pBook2->SetPos(Vec2(1120.f, bookY + 50.f));
	pBook2->SetScale(Vec2(30.f, 30.f));
	pBook2->SetName(L"book2");
	CreateObject(pBook2, GROUP_TYPE::ITEM);

	CBtnUI* pBookBtn2 = new CBtnUI;
	pBookBtn2->SetName(L"Book_UI");
	pBookBtn2->SetScale(Vec2(50.f, 50.f));
	pBookBtn2->SetPos(pBook2->GetPos());
	pBookBtn2->SetMovement(false);
	pBookBtn2->SetHasText(false);
	pBookBtn2->SetHasTexture(false);
	pBookBtn2->SetClickedCallBack(pBook2, (OBJECT_MEMFUNC)&CBook::CreateStudyUI);
	CreateObject(pBookBtn2, GROUP_TYPE::UI);

	bookY = pBook2->GetPos().y;
	
	CBook* pBook3 = new CBook();
	pBook3->SetPos(Vec2(1120.f, bookY + 50.f));
	pBook3->SetScale(Vec2(30.f, 30.f));
	pBook3->SetName(L"book3");
	CreateObject(pBook3, GROUP_TYPE::ITEM);

	CBtnUI* pBookBtn3 = new CBtnUI;
	pBookBtn3->SetName(L"Book_UI");
	pBookBtn3->SetScale(Vec2(50.f, 50.f));
	pBookBtn3->SetPos(pBook3->GetPos());
	pBookBtn3->SetMovement(false);
	pBookBtn3->SetHasText(false);
	pBookBtn3->SetHasTexture(false);
	pBookBtn3->SetClickedCallBack(pBook3, (OBJECT_MEMFUNC)&CBook::CreateStudyUI);
	CreateObject(pBookBtn3, GROUP_TYPE::UI);

	bookY = pBook3->GetPos().y;
	
	CBook* pBook4 = new CBook();
	pBook4->SetPos(Vec2(1120.f, bookY + 50.f));
	pBook4->SetScale(Vec2(30.f, 30.f));
	pBook4->SetName(L"book4");
	CreateObject(pBook4, GROUP_TYPE::ITEM);

	CBtnUI* pBookBtn4 = new CBtnUI;
	pBookBtn4->SetName(L"Book_UI");
	pBookBtn4->SetScale(Vec2(50.f, 50.f));
	pBookBtn4->SetPos(pBook4->GetPos());
	pBookBtn4->SetMovement(false);
	pBookBtn4->SetHasText(false);
	pBookBtn4->SetHasTexture(false);
	pBookBtn4->SetClickedCallBack(pBook4, (OBJECT_MEMFUNC)&CBook::CreateStudyUI);
	CreateObject(pBookBtn4, GROUP_TYPE::UI);
	
	
}


void CScene_Study::Exit()
{
	DeleteAll();
	CCollisionMgr::GetInst()->Reset();
}


