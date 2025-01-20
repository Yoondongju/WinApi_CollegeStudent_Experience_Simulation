#include "pch.h"
#include "CBook.h"
#include "CResMgr.h"

#include "CTexture.h"

#include "CCore.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CTalkUI.h"


UINT CBook::num = 1;
int CBook::m_count = 0;

CBook::CBook()
{
	if (L"QuizeBook" == GetName())
		return;


	wstring wstrOriginbookKey = L"Book";
	wstring wstrnum = std::to_wstring(num);

	m_bookTexture = CResMgr::GetInst()->LoadTexture(L"Book" + wstrnum, L"texture\\book" + wstrnum + L".png");

	num++;

	if (num == 5)
		num = 1;
}

CBook::~CBook()
{
}

void CBook::update()
{

	


	
}

void CBook::render(HDC _dc)
{
	Vec2 vPos = GetPos();

	Vec2 vScale = GetScale();

	AlphaBlend(_dc
		, int(vPos.x - vScale.x / 2.f)
		, int(vPos.y - vScale.y / 2.f)
		, vScale.x    // 출력할 가로크기
		, vScale.y   // 출력할 세로크기
		, m_bookTexture->GetDC()
		, 0
		, 0
		, (int)m_bookTexture->Width()
		, (int)m_bookTexture->Height()
		, m_bookTexture->GetBf());
}

void CBook::CreateBookTexture()
{
	
}



void CBook::CreateStudyUI()
{
	static int count = 0;
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);  // main Player 행동 불가능 설정

	if (L"book1" == GetName())
	{
		CTexture* pPlayer_Content_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Contet_UI", L"texture\\Content_UI.png");
		

		Vec2 vResolution = CCore::GetInst()->GetResolution();

		vector<CObject*>& vecObject = CSceneMgr::GetInst()->GetCurScene()->GetUIGroup();

		for (size_t i = 0; i < vecObject.size(); ++i)
		{
			if (L"Study_UI" == vecObject[i]->GetName())
			{	
				// Content_UI 발견시 삭제
				DeleteObject(vecObject[i]);
				return;
			}

		}

		static bool check = false;
		if (!check)
		{
			count++;
			check = true;
		}

		this->SetMyConversationID(1);
		CTalkUI* pPlayer_Study_UI = new CTalkUI();  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
		pPlayer_Study_UI->SetName(L"Study_UI");
		pPlayer_Study_UI->SetScale(Vec2(1050.f, 500.f));
		pPlayer_Study_UI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f));
		pPlayer_Study_UI->SetUItex(pPlayer_Content_UItex);
		pPlayer_Study_UI->SetMovement(true);
		pPlayer_Study_UI->SetHasText(true);
		pPlayer_Study_UI->SetHasTexture(true);
		pPlayer_Study_UI->SetNpcOwner(this);
		pPlayer_Study_UI->CreateChildUI(pPlayer_Study_UI);
		CreateObject(pPlayer_Study_UI, GROUP_TYPE::UI);
	}



	else if (L"book2" == GetName())
	{
		CTexture* pPlayer_Content_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Contet_UI", L"texture\\Content_UI.png");


		Vec2 vResolution = CCore::GetInst()->GetResolution();

		vector<CObject*>& vecObject = CSceneMgr::GetInst()->GetCurScene()->GetUIGroup();

		for (size_t i = 0; i < vecObject.size(); ++i)
		{
			if (L"Study_UI" == vecObject[i]->GetName())
			{				
				// Content_UI 발견시 삭제
				DeleteObject(vecObject[i]);
				return;
			}

		}

		static bool check = false;
		if (!check)
		{
			count++;
			check = true;
		}

		this->SetMyConversationID(2);
		CTalkUI* pPlayer_Study_UI = new CTalkUI();  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
		pPlayer_Study_UI->SetName(L"Study_UI");
		pPlayer_Study_UI->SetScale(Vec2(1050.f, 500.f));
		pPlayer_Study_UI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f));
		pPlayer_Study_UI->SetUItex(pPlayer_Content_UItex);
		pPlayer_Study_UI->SetMovement(true);
		pPlayer_Study_UI->SetHasText(true);
		pPlayer_Study_UI->SetHasTexture(true);
		pPlayer_Study_UI->SetNpcOwner(this);
		pPlayer_Study_UI->CreateChildUI(pPlayer_Study_UI);
		CreateObject(pPlayer_Study_UI, GROUP_TYPE::UI);
	}
	else if (L"book3" == GetName())
	{
		CTexture* pPlayer_Content_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Contet_UI", L"texture\\Content_UI.png");


		Vec2 vResolution = CCore::GetInst()->GetResolution();

		vector<CObject*>& vecObject = CSceneMgr::GetInst()->GetCurScene()->GetUIGroup();

		for (size_t i = 0; i < vecObject.size(); ++i)
		{
			if (L"Study_UI" == vecObject[i]->GetName())
			{
		
				// Content_UI 발견시 삭제
				DeleteObject(vecObject[i]);
				return;
			}

		}

		static bool check = false;
		if (!check)
		{
			count++;
			check = true;
		}

		this->SetMyConversationID(3);
		CTalkUI* pPlayer_Study_UI = new CTalkUI();  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
		pPlayer_Study_UI->SetName(L"Study_UI");
		pPlayer_Study_UI->SetScale(Vec2(1050.f, 500.f));
		pPlayer_Study_UI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f));
		pPlayer_Study_UI->SetUItex(pPlayer_Content_UItex);
		pPlayer_Study_UI->SetMovement(true);
		pPlayer_Study_UI->SetHasText(true);
		pPlayer_Study_UI->SetHasTexture(true);
		pPlayer_Study_UI->SetNpcOwner(this);
		pPlayer_Study_UI->CreateChildUI(pPlayer_Study_UI);
		CreateObject(pPlayer_Study_UI, GROUP_TYPE::UI);
	}

	else if (L"book4" == GetName())
	{
		CTexture* pPlayer_Content_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Contet_UI", L"texture\\Content_UI.png");

		Vec2 vResolution = CCore::GetInst()->GetResolution();

		vector<CObject*>& vecObject = CSceneMgr::GetInst()->GetCurScene()->GetUIGroup();

		for (size_t i = 0; i < vecObject.size(); ++i)
		{
			if (L"Study_UI" == vecObject[i]->GetName())
			{

				// Content_UI 발견시 삭제
				DeleteObject(vecObject[i]);
				return;
			}

		}

		static bool check = false;
		if (!check)
		{
			count++;
			check = true;
		}

		this->SetMyConversationID(4);
		CTalkUI* pPlayer_Study_UI = new CTalkUI();  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
		pPlayer_Study_UI->SetName(L"Study_UI");
		pPlayer_Study_UI->SetScale(Vec2(1050.f, 500.f));
		pPlayer_Study_UI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f));
		pPlayer_Study_UI->SetUItex(pPlayer_Content_UItex);
		pPlayer_Study_UI->SetMovement(true);
		pPlayer_Study_UI->SetHasText(true);
		pPlayer_Study_UI->SetHasTexture(true);
		pPlayer_Study_UI->SetNpcOwner(this);
		pPlayer_Study_UI->CreateChildUI(pPlayer_Study_UI);
		CreateObject(pPlayer_Study_UI, GROUP_TYPE::UI);
	}

	m_count = count;
	
}



void CBook::CreateQuizUI()
{
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);  // main Player 행동 불가능 설정

	CTexture* pPlayer_Content_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Contet_UI", L"texture\\Content_UI.png");

	Vec2 vResolution = CCore::GetInst()->GetResolution();

	vector<CObject*>& vecObject = CSceneMgr::GetInst()->GetCurScene()->GetUIGroup();

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		if (L"Quiz_UI" == vecObject[i]->GetName())
		{
			// Content_UI 발견시 삭제
			DeleteObject(vecObject[i]);
			return;
		}

	}


	



	
	CTalkUI* pPlayer_Quiz_UI = new CTalkUI();  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
	pPlayer_Quiz_UI->SetName(L"Quiz_UI");
	pPlayer_Quiz_UI->SetScale(Vec2(800.f, 500.f));
	pPlayer_Quiz_UI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f));
	pPlayer_Quiz_UI->SetUItex(pPlayer_Content_UItex);
	pPlayer_Quiz_UI->SetMovement(true);
	pPlayer_Quiz_UI->SetHasText(true);
	pPlayer_Quiz_UI->SetHasTexture(true);
	pPlayer_Quiz_UI->SetNpcOwner(this);
	pPlayer_Quiz_UI->CreateChildUI(pPlayer_Quiz_UI);
	CreateObject(pPlayer_Quiz_UI, GROUP_TYPE::UI);

}



void CBook::OnCollisionEnter(CCollider* _pOther)
{
}


