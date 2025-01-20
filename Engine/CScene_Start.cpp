#include "pch.h"
#include "CCore.h"
#include "CScene_Start.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTexture.h"
#include "CPathMgr.h"

#include "CCollisionMgr.h"

#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CResMgr.h"
#include "CCamera.h"

#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"

#include "CRigidBody.h"

#include "SelectGDI.h"
#include "CTimeMgr.h"

#include "CGround.h"

#include "CUI.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

#include "CTalkUI.h"


void ChangeScene(DWORD_PTR _pa, DWORD_PTR _pa2);

CScene_Start::CScene_Start()
	: m_bUseForce(false)
	, m_fCurRadius(0.f)
	, m_fForceRadius(500.f)
{
}

CScene_Start::~CScene_Start()
{
}



void CScene_Start::update()
{
	CScene::update();


	CUI* pMainUI = CSceneMgr::GetInst()->GetCurScene()->GetMainTalkUI();

	if (nullptr != pMainUI)
	{
		if (pMainUI->IsDead())
		{
			PostQuitMessage(0);
		}
	}

}



void CScene_Start::render(HDC _dc)
{
	Vec2 vClientResolution = CCore::GetInst()->GetResolution();

	BLENDFUNCTION bf = {};

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;


	AlphaBlend(_dc
		, 0
		, 0
		, (int)vClientResolution.x
		, (int)vClientResolution.y
		, m_pBackground_Texture->GetDC()
		, 0
		, 0
		,(int)m_pBackground_Texture->Width()
		,(int)m_pBackground_Texture->Height()
		, bf);



	CScene::render(_dc);

}


void CScene_Start::Enter()
{
	CCamera::GetInst()->FadeIn(3.f);

	CTexture* pTex = CCore::GetInst()->GetBackBuffTex();



	Vec2 vResolution = CCore::GetInst()->GetResolution();

	// 자기 방안 배경 텍스쳐 입히기
	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"home", L"texture\\home.png");

	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"TalkUI", L"texture\\TalkUI.png");

	// Player Object 추가
	// pG_Player = new CPlayer;
	// pG_Player->SetName(L"Player");
	pG_Player->SetPos(Vec2(980.f,190.f));
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);

	
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
	CreateObject(pTalkUI, GROUP_TYPE::UI);


	// // Player 복사
	// CObject* pCloneObj = pObj->Clone();
	// pCloneObj->SetPos(Vec2(740.f, 384.f));
	// AddObject(pCloneObj, GROUP_TYPE::PLAYER);

}


void CScene_Start::Exit()
{

	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}

void CScene_Start::DeleteAll()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{

		// 이펙트는 씬이 바뀔때 여기서 삭제하는것이 아니라 그때그때
		// 예를들면 오브젝트에 마우스를 가져다 놓거나 어떤 특정상황일때 오너 오브젝트와 함께 지울것임
		// 오너가 없는 이펙트를 나중에 씬전환시 삭제되는부분은 아직 생각할 부분이 너무 많아 보류 하겠음.

		DeleteGroup((GROUP_TYPE)i);
	}
}


void CScene_Start::CreateForce()
{

	m_vForcePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
}
