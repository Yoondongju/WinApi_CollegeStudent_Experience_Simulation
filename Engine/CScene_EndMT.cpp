#include "pch.h"
#include "CScene_EndMT.h"


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
#include "CMTItem.h"
#include "CAnimator.h"

#include "AI.h"
#include "CMonster.h"
#include "CCodeModeState.h"

#include "CBtnUI.h"
#include "CPatrol.h"

CScene_EndMT::CScene_EndMT()
{
}

CScene_EndMT::~CScene_EndMT()
{
}

void CScene_EndMT::Enter()
{
	if (nullptr == pG_Player) // Loding 2 복구하면 지워야함
	{
		pG_Player = new CPlayer(PLAYER_MBTI::INTJ);  //  << 임시작업용 
	}

	pG_Player->SetScale(Vec2(50.f, 75.f));
	pG_Player->SetPos(Vec2(1250.f, 660.f));
	pG_Player->SetCurState(PLAYER_STATE::IDLE);
	pG_Player->SetMyConversationID(-1);
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);



	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CCamera::GetInst()->FadeIn(3.f);


	// ================================== 벽 만들기 ================================



	CGround* pOutline = new CGround;
	pOutline->SetPos(Vec2(vResolution.x / 2.f, 440.f));
	pOutline->SetScale(Vec2(vResolution.x, 5.f));
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


	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"MT_end_BGM");
	pNewSound->Play();
	pNewSound->PlayToBGM(true);
	//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	pNewSound->SetVolume(40.f); // 소리 볼륨 크기



	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"EndMT_scene", L"texture\\EndMT_scene.png");





	// PlayerInfo 변화함

	// Camera Look 지정

	CCamera::GetInst()->SetLookAt(Vec2(vResolution / 2.f));
	CCamera::GetInst()->SetTarget(pG_Player);

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);



	// 버스 안 학생들 배치도 // 

	AI* pAI3 = new AI;
	pAI3->AddState(new CCodeModeState);
	pAI3->SetCurState(MON_STATE::CODEMODE);
	CCodeModeState* pCodeModestate4 = (CCodeModeState*)(pAI3->GetState(MON_STATE::CODEMODE));
	pCodeModestate4->SetOnlyRecognize(true);  // 움직임 없이 인식만 하는설정


	AI* pAI5 = new AI;
	pAI5->AddState(new CCodeModeState);
	pAI5->SetCurState(MON_STATE::CODEMODE);
	CCodeModeState* pCodeModestate6 = (CCodeModeState*)(pAI5->GetState(MON_STATE::CODEMODE));
	pCodeModestate6->SetOnlyRecognize(false);  // 움직임 없이 인식만 하는설정

	AI* pAI6 = new AI;
	pAI6->AddState(new CCodeModeState);
	pAI6->SetCurState(MON_STATE::CODEMODE);
	CCodeModeState* pCodeModestate7 = (CCodeModeState*)(pAI6->GetState(MON_STATE::CODEMODE));
	pCodeModestate7->SetOnlyRecognize(false);  // 움직임 없이 인식만 하는설정

	AI* pAI7 = new AI;
	pAI7->AddState(new CCodeModeState);
	pAI7->SetCurState(MON_STATE::CODEMODE);
	CCodeModeState* pCodeModestate8 = (CCodeModeState*)(pAI7->GetState(MON_STATE::CODEMODE));
	pCodeModestate8->SetOnlyRecognize(false);  // 움직임 없이 인식만 하는설정


	// 위치 잡아주고 앉는거 넣어주고

	// 학생 위치 잡아주기 좌표 




	CMonster* pUnknown_friend_4 = new CMonster;
	pUnknown_friend_4->SetName(L"Earnest_friend");
	pUnknown_friend_4->SetPos(Vec2(730.f, 660.f));
	pUnknown_friend_4->SetEarlyPos(pUnknown_friend_4->GetPos());
	pUnknown_friend_4->SetScale(Vec2(50.f, 70.f));
	//pUnknown_friend_4->SetAI(pAI3);
	pUnknown_friend_4->SetFineTarget(false);
	pUnknown_friend_4->SetDissapear_conversation(false);
	pUnknown_friend_4->SetCurState(CUSTOMER_STATE::SAY);
	pUnknown_friend_4->SetDir(CUSTOMER_DIR::LEFT);
	pUnknown_friend_4->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
	pUnknown_friend_4->SetMyConversationID(0);
	pUnknown_friend_4->SetSpeed(60.f);
	AddObject(pUnknown_friend_4, GROUP_TYPE::MONSTER);



	CMonster* pUnknown_friend_6 = new CMonster;
	pUnknown_friend_6->SetName(L"Earnest_friend_woman");
	pUnknown_friend_6->SetPos(Vec2(1300.f, 660.f));
	pUnknown_friend_6->SetEarlyPos(pUnknown_friend_6->GetPos());
	pUnknown_friend_6->SetScale(Vec2(50.f, 70.f));
	pUnknown_friend_6->SetAI(pAI5);
	pUnknown_friend_6->SetFineTarget(false);
	pUnknown_friend_6->SetDissapear_conversation(false);
	pUnknown_friend_6->SetCurState(CUSTOMER_STATE::SAY);
	pUnknown_friend_6->SetDir(CUSTOMER_DIR::RIGHT);
	pUnknown_friend_6->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
	pUnknown_friend_6->SetMyConversationID(1);
	pUnknown_friend_6->SetSpeed(60.f);
	pUnknown_friend_6->SetTarGetPos(Vec2(620.f, 660.f));
	AddObject(pUnknown_friend_6, GROUP_TYPE::MONSTER);


	CMonster* pfriend = new CMonster;
	pfriend->SetName(L"Earnest_friend_woman2");
	pfriend->SetPos(Vec2(1200.f, 660.f));
	pfriend->SetEarlyPos(pfriend->GetPos());
	pfriend->SetScale(Vec2(50.f, 70.f));
	pfriend->SetAI(pAI7);
	pfriend->SetFineTarget(false);
	pfriend->SetDissapear_conversation(false);
	pfriend->SetCurState(CUSTOMER_STATE::SAY);
	pfriend->SetDir(CUSTOMER_DIR::RIGHT);
	pfriend->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
	pfriend->SetMyConversationID(2);
	pfriend->SetZoomInMon(false);
	pfriend->SetSpeed(70.f);
	pfriend->SetTarGetPos(Vec2(500.f, 660.f));
	AddObject(pfriend, GROUP_TYPE::MONSTER);


	CMonster* pfriend_cute = new CMonster;
	pfriend_cute->SetName(L"CuteBoy");
	pfriend_cute->SetPos(Vec2(1350.f, 660.f));
	pfriend_cute->SetEarlyPos(pfriend_cute->GetPos());
	pfriend_cute->SetScale(Vec2(50.f, 70.f));
	pfriend_cute->SetAI(pAI6);
	pfriend_cute->SetFineTarget(false);
	pfriend_cute->SetDissapear_conversation(false);
	pfriend_cute->SetCurState(CUSTOMER_STATE::SAY);
	pfriend_cute->SetDir(CUSTOMER_DIR::LEFT);
	pfriend_cute->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
	pfriend_cute->SetMyConversationID(3);
	pfriend_cute->SetSpeed(50.f);
	pfriend_cute->SetTarGetPos(Vec2(550.f, 660.f));
	AddObject(pfriend_cute, GROUP_TYPE::MONSTER);



	pG_Player->SetAuto(true);
	pG_Player->SetOnlyMove(true);
	pG_Player->SetAutoPos(Vec2(450.f, pG_Player->GetPos().y));  // 의자 앉혀주기


}

void CScene_EndMT::update()
{
	static bool bCheck = false;
	static bool bCheck_1 = false;

	CTalkUI *pUI = GetMainTalkUI();
	
	if (pUI)
	{
		if (pUI->IsDead())
		{
			bCheck_1 = true;
			CCamera::GetInst()->FadeOut(2.f);		
		}
	}

	if (CCamera::GetInst()->IsAllCamEffect_End())
	{
		if(bCheck_1)
			ChangeScene(SCENE_TYPE::STAGE_LOBBY);
	}


	if (!pG_Player->GetAuto())
	{
		
		if (!bCheck)
		{
			Vec2 vResolution = CCore::GetInst()->GetResolution();

			CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"text_box", L"texture\\Textbox_8.png");
			CTalkUI* pTalkUI = new CTalkUI;
			pTalkUI->SetName(L"Drinking_Talk_UI");
			pTalkUI->SetPos(Vec2(vResolution.x / 2.f, 200.f));
			pTalkUI->SetScale(Vec2(UI_WIDTH, 200.f)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
			pTalkUI->SetUItex(UItex);
			pTalkUI->SetMovement(false);
			pTalkUI->SetHasText(true);
			pTalkUI->SetHasTexture(true);
			pTalkUI->CreateChildUI(pTalkUI);
			pTalkUI->SetMyConversationID(0);
			pTalkUI->SetNpcOwner(pTalkUI);
			m_MonologueTalkUI = pTalkUI;
			CreateObject(pTalkUI, GROUP_TYPE::UI);

			bCheck = true;
		}
	}


	





	CScene::update();

	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);
}

void CScene_EndMT::render(HDC _dc)
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



void CScene_EndMT::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}


