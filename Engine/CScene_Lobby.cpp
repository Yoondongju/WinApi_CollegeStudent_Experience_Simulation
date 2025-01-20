#include "pch.h"
#include "CScene_Lobby.h"

#include "CCamera.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CCore.h"

#include "CTile.h"

#include "CPlayer.h"
#include "CCollisionMgr.h"

#include "CCollider.h"

#include "CSceneMgr.h"

#include "CUI.h"
#include "CTalkUI.h"
#include "CPanelUI.h"
#include "CBtnUI.h"
#include "CPlayerUI.h"

#include "CSound.h"

#include "CGround.h"

#include "CAnimator.h"

#include "CMonster.h"

#include "AI.h"
#include "CCodeModeState.h"

#include "SelectGDI.h"


extern CPlayer* p_GPlayer;

bool CScene_Lobby::m_bCheckMT = false;
bool CScene_Lobby::m_bCheckDate = false;
bool CScene_Lobby::m_bCheckStudy = false;
bool CScene_Lobby::m_bCheckJob = false;
bool CScene_Lobby::m_bCheckDrinking = false;


void CreateObject(DWORD_PTR _pa, DWORD_PTR _pa2);
void ChangeScene(DWORD_PTR _pa, DWORD_PTR _pa2);


CScene_Lobby::CScene_Lobby()
	:m_mysteriboy(nullptr)
{
	
}

CScene_Lobby::~CScene_Lobby()
{
}


void CScene_Lobby::Enter()
{
	if (nullptr == pG_Player) // Loding 2 복구하면 지워야함
	{
		pG_Player = new CPlayer(PLAYER_MBTI::INTJ);  //  << 임시작업용 
	}


	pG_Player->SetScale(Vec2(50.f, 75.f));
	pG_Player->SetPos(Vec2(600.f, 300.f));
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
	pG_Player->SetMyConversationID(0);
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);

	
	static bool firstCall = false;

	if (!firstCall)
	{
		CCamera::GetInst()->FadeIn(2.f);
		CCamera::GetInst()->FadeOut(2.f);

		firstCall = true;
		pG_Player->SetspeechBubble();
	}
	else
		CCamera::GetInst()->FadeIn(3.f);
	





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
	pOutline3->SetPos(Vec2(vResolution.x / 2.f, 750.f));
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
	// ================================== 벽 만들기 ================================


	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Lobby_BGM");
	//pNewSound->Play();
	pNewSound->PlayToBGM(true);
	//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	pNewSound->SetVolume(50.f); // 소리 볼륨 크기

	

	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"Lobby", L"texture\\DarkSpace_A.png");
	CTexture* pPlayer_State_UItex = CResMgr::GetInst()->LoadTexture(L"Player_State_UI", L"texture\\123_UI.png");
	CTexture* pPlayer_UItex = CResMgr::GetInst()->LoadTexture(L"Player_UI", L"texture\\Player\\playerUI.png");
	CTexture* pPlayer_Btn_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Btn_UI", L"texture\\book.png");


	
	
	CPanelUI* pPlayer_Panel_UI = new CPanelUI(PANEL_UI_TYPE::PLAYER_UI);  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
	pPlayer_Panel_UI->SetName(L"Player_Panel_UI");
	pPlayer_Panel_UI->SetScale(Vec2(400.f, 300.f));
	pPlayer_Panel_UI->SetPos(Vec2(200.f, 150.f));
	pPlayer_Panel_UI->SetUItex(pPlayer_State_UItex);
	pPlayer_Panel_UI->SetMovement(true);
	pPlayer_Panel_UI->SetHasText(true);
	pPlayer_Panel_UI->SetHasTexture(true);
	pPlayer_Panel_UI->SetPanelUI_type(PANEL_UI_TYPE::PLAYER_UI);
	
	
	Vec2 vPos = pPlayer_Panel_UI->GetFinalPos();
	Vec2 vScale = pPlayer_Panel_UI->GetScale();

	Vec2 fParentLT = vPos - vScale / 2.f; // 부모의 좌상단
	Vec2 fParentRB = vPos + vScale / 2.f; // 부모의 우하단


	CPlayerUI* pPlayerUI = new CPlayerUI;
	pPlayerUI->SetName(L"Player_Panel_UI");
	pPlayerUI->SetScale(Vec2(100.f, 100.f));
	pPlayerUI->SetOffset(Vec2(fParentLT + 60.f));
	pPlayerUI->SetPos(pPlayerUI->GetOffset());
	pPlayerUI->SetUItex(pPlayer_UItex);
	pPlayerUI->SetMovement(false);
	pPlayerUI->SetHasText(false);
	pPlayerUI->SetHasTexture(true);
	pPlayer_Panel_UI->AddChild(pPlayerUI);
	

	

	CBtnUI* pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"Player_Btn_UI");
	pBtnUI->SetScale(Vec2(50.f, 50.f)); 
	pBtnUI->SetOffset(Vec2(fParentRB - 60.f));
	pBtnUI->SetPos(pBtnUI->GetOffset());
	pBtnUI->SetUItex(pPlayer_Btn_UItex);
	pBtnUI->SetMovement(false);
	pBtnUI->SetHasText(false);
	pBtnUI->SetHasTexture(true);
	pBtnUI->SetCamAffectedObj(false);
	pBtnUI->SetClickedCallBack(CreateObject, 0, 0);
	pPlayer_Panel_UI->AddChild(pBtnUI);

	AddObject(pPlayer_Panel_UI, GROUP_TYPE::UI);




	// PlayerInfo 변화함

	// Camera Look 지정

	//CCamera::GetInst()->SetLookAt(Vec2(vResolution / 2.f));
	//CCamera::GetInst()->SetTarget(pG_Player);

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	
	// start(); 필요시 구현해서 사용 (아직 미구현)

	
}

void CScene_Lobby::update()
{

	if (CCamera::GetInst()->IsAllCamEffect_End())
	{
		CTalkUI* pMainUI = GetMainTalkUI();
		if (nullptr == pMainUI)
		{
			pG_Player->SetCurState(PLAYER_STATE::IDLE);
		}

	}
		


	CScene::update();

	if (nullptr != m_mysteriboy)
		if (m_mysteriboy->IsDead())
		{
			if (L"future_me" == m_mysteriboy->GetName())
			{
				ChangeScene(SCENE_TYPE::ENDDING);
				return;
			}
				
		}
			

	if ( CScene_Lobby::GetCheckDrinking() && CScene_Lobby::GetCheckStudy() && CScene_Lobby::GetCheckJob() && CScene_Lobby::GetCheckDate() && CScene_Lobby::GetCheckMT()) // 현재는 공부,알바,술자리만 구현했음 추후에 여기 수정작업 필요
	{
		static bool b = false; 
		if (!b)
		{
			CSound* pNewSound = CResMgr::GetInst()->FindSound(L"ENDDING_BGM");
			//pNewSound->Play();
			pNewSound->PlayToBGM(true);
			//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
			pNewSound->SetVolume(70.f); // 소리 볼륨 크기

			// 좌상단 탁자 아이들
			AI* pAI = new AI;
			pAI->AddState(new CCodeModeState);
			pAI->SetCurState(MON_STATE::CODEMODE);
			CCodeModeState* pCodeModestate = (CCodeModeState*)(pAI->GetState(MON_STATE::CODEMODE));
			pCodeModestate->SetOnlyRecognize(true);  // 움직임 없이 인식만 하는설정

			CMonster* pMysterious_boy = new CMonster;
			pMysterious_boy->SetName(L"mysterious_boy");
			pMysterious_boy->SetPos(Vec2(700.f, 600.f));
			pMysterious_boy->SetScale(Vec2(130.f, 170.f));
			pMysterious_boy->SetDissapear_conversation(true);
			pMysterious_boy->GetAnimator()->init_Animation();
			pMysterious_boy->SetAI(pAI);
			pMysterious_boy->SetCurState(CUSTOMER_STATE::SAY);
			pMysterious_boy->GetCollider()->SetScale(Vec2(30.f, 40.f));
			pMysterious_boy->SetMyConversationID(pG_Player->Final_info_calculate()); //  미스테리 미래의 나가 대화 설정

			m_mysteriboy = pMysterious_boy;
			AddObject(pMysterious_boy, GROUP_TYPE::MONSTER);
			b = true;
		}


		if (nullptr != m_mysteriboy)
		{
			if (m_mysteriboy->IsDead())
			{
				m_mysteriboy = nullptr;
				pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
				CCamera::GetInst()->FadeOut(4.f);

			}
		}


		if (CCamera::GetInst()->IsCamEffect_End())
		{
			if (nullptr == m_mysteriboy)
			{
				pG_Player->SetCurState(PLAYER_STATE::IDLE);
				
				AI* pAI = new AI;
				pAI->AddState(new CCodeModeState);
				pAI->SetCurState(MON_STATE::CODEMODE);
				CCodeModeState* pCodeModestate = (CCodeModeState*)(pAI->GetState(MON_STATE::CODEMODE));
				pCodeModestate->SetOnlyRecognize(true);  // 움직임 없이 인식만 하는설정

				CMonster* pfuture_me = new CMonster;
				pfuture_me->SetName(L"future_me");
				pfuture_me->SetPos(Vec2(700.f, 600.f));
				pfuture_me->SetScale(Vec2(50.f, 80.f));
				pfuture_me->SetDissapear_conversation(true);
				pfuture_me->GetAnimator()->init_Animation();
				pfuture_me->SetAI(pAI);
				pfuture_me->SetCurState(CUSTOMER_STATE::SAY);
				pfuture_me->GetCollider()->SetScale(Vec2(30.f, 40.f));


				if (0 > pG_Player->Final_info_calculate())
				{
					pfuture_me->SetMyConversationID(2);
				}
				else
					pfuture_me->SetMyConversationID(1);


				m_mysteriboy = pfuture_me;
				AddObject(pfuture_me, GROUP_TYPE::MONSTER);

			}

		}

	}

}

void CScene_Lobby::render(HDC _dc)
{
	Vec2 vClientResolution = CCore::GetInst()->GetResolution();

	
	


	// AlphaBlend(_dc
	//	 , 0
	//	 , 0
	//	 , (int)vClientResolution.x   // 출력할 가로크기
	//	 , (int)vClientResolution.y  // 출력할 세로크기
	//	 , m_pBackground_Texture->GetDC()
	//	 , 0
	//	 , 0
	//	 , (int)m_pBackground_Texture->Width()
	//	 , (int)m_pBackground_Texture->Height()
	//	 , m_pBackground_Texture->GetBf());


	 CScene::render(_dc);




	// Vec2 vCamLook = CCamera::GetInst()->GetLookAt(); // 카메라가 보고있는곳
	// Vec2 vResolution = CCore::GetInst()->GetResolution(); // 우리 게임 해상도


	// CObject* _pPlayer = GetPlayer();
	// 
	// 
	// Vec2 vPos = _pPlayer->GetPos();
	// // 텍스쳐 크기 넘어가는부분은 예외처리 또 해줘야함.
	//
	// if (0.f >= vPos.x)
	// {
	//	 vPos.x = 0.f;
	// }
	// else if (m_pBackground_Texture->Width() <= vPos.x + 900.f)
	// {
	//	 vPos.x = m_pBackground_Texture->Width() - 900.f;
	// }
	// 
	// if (0.f >= vPos.y)
	// {
	//	 vPos.y = 0.f;
	// }
	// else if (m_pBackground_Texture->Height() <= vPos.y + 900.f)
	// {
	//	 vPos.y = m_pBackground_Texture->Height() - 900.f;
	// }

	 // 오픈월드 하려고했을때 캐릭터 위치에따라 맵을 그려주는 사진이 달라지는건데 임시보류 하고 
	 // 원래 포탈 대로
	 // SetStretchBltMode(_dc, COLORONCOLOR);
	 // StretchBlt(_dc, 0, 0, vClientResolution.x, vClientResolution.y
	 // 	, m_pBackground_Texture->GetDC(), vPos.x, vPos.y, 900, 900, SRCCOPY); // 부분 확대
	 

}


void CScene_Lobby::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}




void CreateObject(DWORD_PTR _pa, DWORD_PTR _pa2)
{
	CTexture* pPlayer_Content_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Contet_UI", L"texture\\Content_UI.png");
	CTexture* pPlayer_Contett_Btn_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Contet_Btn_UI", L"texture\\BtnUI.png");

	Vec2 vResolution = CCore::GetInst()->GetResolution();

	vector<CObject*>&  vecObject = CSceneMgr::GetInst()->GetCurScene()->GetUIGroup();

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		if (L"Player_Content_UI" == vecObject[i]->GetName()) 
		{
			// Content_UI 발견시 삭제
			DeleteObject(vecObject[i]);
			return;
		}
		
	}
	

	CPanelUI* pPlayer_Content_UI = new CPanelUI(PANEL_UI_TYPE::CONTENT_UI);  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
	pPlayer_Content_UI->SetName(L"Player_Content_UI");
	pPlayer_Content_UI->SetScale(Vec2(500.f, 500.f));
	pPlayer_Content_UI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f));
	pPlayer_Content_UI->SetUItex(pPlayer_Content_UItex);
	pPlayer_Content_UI->SetMovement(true);
	pPlayer_Content_UI->SetHasText(true);
	pPlayer_Content_UI->SetHasTexture(true);
	pPlayer_Content_UI->SetPanelUI_type(PANEL_UI_TYPE::CONTENT_UI);
	
	
	Vec2 vPos = pPlayer_Content_UI->GetFinalPos();
	Vec2 vScale = pPlayer_Content_UI->GetScale();

	Vec2 fParentLT = vPos - vScale / 2.f; // 부모의 좌상단
	Vec2 fParentRB = vPos + vScale / 2.f; // 부모의 우하단
	


	fParentLT.y -= 5.f;

	// MT 다녀오기 버튼
	CBtnUI* pBtn_MT_UI = new CBtnUI;
	pBtn_MT_UI->SetName(L"Player_Btn_MT_UI");
	pBtn_MT_UI->SetScale(Vec2(20.f, 20.f));
	pBtn_MT_UI->SetOffset(Vec2(fParentLT.x + 270.f , fParentLT.y + 100.f));
	pBtn_MT_UI->SetPos(pBtn_MT_UI->GetOffset());
	pBtn_MT_UI->SetUItex(pPlayer_Contett_Btn_UItex);
	pBtn_MT_UI->SetMovement(false);
	pBtn_MT_UI->SetHasText(false);
	pBtn_MT_UI->SetHasTexture(true);
	pBtn_MT_UI->SetCamAffectedObj(false);
	pBtn_MT_UI->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::MT, (DWORD_PTR)pBtn_MT_UI);
	pPlayer_Content_UI->AddChild(pBtn_MT_UI);

	// 소개팅 다녀오기 
	CBtnUI* pBtn_Date_UI = new CBtnUI;
	pBtn_Date_UI->SetName(L"Player_Btn_Date_UI");
	pBtn_Date_UI->SetScale(Vec2(20.f, 20.f));
	pBtn_Date_UI->SetOffset(Vec2(fParentLT.x + 270.f, fParentLT.y + 130.f));
	pBtn_Date_UI->SetPos(pBtn_Date_UI->GetOffset());
	pBtn_Date_UI->SetUItex(pPlayer_Contett_Btn_UItex);
	pBtn_Date_UI->SetMovement(false);
	pBtn_Date_UI->SetHasText(false);
	pBtn_Date_UI->SetHasTexture(true);
	pBtn_Date_UI->SetCamAffectedObj(false);
	pBtn_Date_UI->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::DATE, (DWORD_PTR)pBtn_Date_UI);
	pPlayer_Content_UI->AddChild(pBtn_Date_UI);

	// 공부 하러가기
	CBtnUI* pBtn_Study_UI = new CBtnUI;
	pBtn_Study_UI->SetName(L"Player_Btn_Study_UI");
	pBtn_Study_UI->SetScale(Vec2(20.f, 20.f));
	pBtn_Study_UI->SetOffset(Vec2(fParentLT.x + 270.f, fParentLT.y + 160.f));
	pBtn_Study_UI->SetPos(pBtn_Study_UI->GetOffset());
	pBtn_Study_UI->SetUItex(pPlayer_Contett_Btn_UItex);
	pBtn_Study_UI->SetMovement(false);
	pBtn_Study_UI->SetHasText(false);
	pBtn_Study_UI->SetHasTexture(true);
	pBtn_Study_UI->SetCamAffectedObj(false);
	pBtn_Study_UI->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::STUDY, (DWORD_PTR)pBtn_Study_UI);
	pPlayer_Content_UI->AddChild(pBtn_Study_UI);
	
	// 알바하러가기
	CBtnUI* pBtn_Job_UI = new CBtnUI;
	pBtn_Job_UI->SetName(L"Player_Btn_Job_UI");
	pBtn_Job_UI->SetScale(Vec2(20.f, 20.f));
	pBtn_Job_UI->SetOffset(Vec2(fParentLT.x + 270.f, fParentLT.y + 190.f));
	pBtn_Job_UI->SetPos(pBtn_Job_UI->GetOffset());
	pBtn_Job_UI->SetUItex(pPlayer_Contett_Btn_UItex);
	pBtn_Job_UI->SetMovement(false);
	pBtn_Job_UI->SetHasText(false);
	pBtn_Job_UI->SetHasTexture(true);
	pBtn_Job_UI->SetCamAffectedObj(false);
	pBtn_Job_UI->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::JOB, (DWORD_PTR)pBtn_Job_UI);
	pPlayer_Content_UI->AddChild(pBtn_Job_UI);
	
	// 친구랑 술마시러 가기
	CBtnUI* pBtn_Drinking_UI = new CBtnUI;
	pBtn_Drinking_UI->SetName(L"Player_Btn_Drinking_UI");
	pBtn_Drinking_UI->SetScale(Vec2(20.f, 20.f));
	pBtn_Drinking_UI->SetOffset(Vec2(fParentLT.x + 270.f, fParentLT.y + 220.f));
	pBtn_Drinking_UI->SetPos(pBtn_Drinking_UI->GetOffset());
	pBtn_Drinking_UI->SetUItex(pPlayer_Contett_Btn_UItex);
	pBtn_Drinking_UI->SetMovement(false);
	pBtn_Drinking_UI->SetHasText(false);
	pBtn_Drinking_UI->SetHasTexture(true);
	pBtn_Drinking_UI->SetCamAffectedObj(false);
	pBtn_Drinking_UI->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::DRINKING, (DWORD_PTR)pBtn_Drinking_UI);
	pPlayer_Content_UI->AddChild(pBtn_Drinking_UI);


	CreateObject(pPlayer_Content_UI, GROUP_TYPE::UI);
}


void ChangeScene(DWORD_PTR _pa, DWORD_PTR _pa2)
{
	// _pa2 <- 이 함수를 부른버튼
	CBtnUI* pBtn = (CBtnUI*)_pa2;

	if (nullptr != pBtn)
	{
		if (L"Player_Btn_MT_UI" == pBtn->GetName())
		{
			if (CScene_Lobby::GetCheckMT())
			{
				CTexture* pPlayer_Btn_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Message_UI", L"texture\\Resource\\Messagebox.png");

				CPanelUI* pMessageBox_MT_UI = new CPanelUI(PANEL_UI_TYPE::MESSAGEBOX_UI);  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
				pMessageBox_MT_UI->SetName(L"MessageBox_MT_UI");
				pMessageBox_MT_UI->SetScale(Vec2(300.f, 200.f));
				pMessageBox_MT_UI->SetPos(Vec2(pBtn->GetFinalPos()));
				pMessageBox_MT_UI->SetUItex(pPlayer_Btn_UItex);
				pMessageBox_MT_UI->SetMovement(true);
				pMessageBox_MT_UI->SetHasText(true);
				pMessageBox_MT_UI->SetHasTexture(true);
				pMessageBox_MT_UI->CreateChildCloseUI(pMessageBox_MT_UI);
				pMessageBox_MT_UI->SetPanelUI_type(PANEL_UI_TYPE::MESSAGEBOX_UI);
				CreateObject(pMessageBox_MT_UI, GROUP_TYPE::UI);
				return;
			}
			else if (!CScene_Lobby::GetCheckDate() || !CScene_Lobby::GetCheckStudy() || !CScene_Lobby::GetCheckJob() || !CScene_Lobby::GetCheckDrinking())
			{
				CTexture* pPlayer_Btn_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Message_UI", L"texture\\TalkUI.png");

				CPanelUI* pMessageBox_MT_UI = new CPanelUI(PANEL_UI_TYPE::MESSAGEBOX_UI);  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
				pMessageBox_MT_UI->SetName(L"MessageBox_MT_not_met_UI");
				pMessageBox_MT_UI->SetScale(Vec2(300.f, 200.f));
				pMessageBox_MT_UI->SetPos(Vec2(pBtn->GetFinalPos()));
				pMessageBox_MT_UI->SetUItex(pPlayer_Btn_UItex);
				pMessageBox_MT_UI->SetMovement(true);
				pMessageBox_MT_UI->SetHasText(true);
				pMessageBox_MT_UI->SetHasTexture(true);
				pMessageBox_MT_UI->CreateChildCloseUI(pMessageBox_MT_UI);
				pMessageBox_MT_UI->SetPanelUI_type(PANEL_UI_TYPE::MESSAGEBOX_UI);
				CreateObject(pMessageBox_MT_UI, GROUP_TYPE::UI);
				return;
			}
			else
				CScene_Lobby::SetCheckMT(true);
		}
		else if (L"Player_Btn_Date_UI" == pBtn->GetName())
		{
			if (CScene_Lobby::GetCheckDate())
			{
				CTexture* pPlayer_Btn_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Message_UI", L"texture\\Resource\\Messagebox.png");

				CPanelUI* pMessageBox_Date_UI = new CPanelUI(PANEL_UI_TYPE::MESSAGEBOX_UI);  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
				pMessageBox_Date_UI->SetName(L"MessageBox_Date_UI");
				pMessageBox_Date_UI->SetScale(Vec2(300.f, 200.f));
				pMessageBox_Date_UI->SetPos(Vec2(pBtn->GetFinalPos()));
				pMessageBox_Date_UI->SetUItex(pPlayer_Btn_UItex);
				pMessageBox_Date_UI->SetMovement(true);
				pMessageBox_Date_UI->SetHasText(true);
				pMessageBox_Date_UI->SetHasTexture(true);
				pMessageBox_Date_UI->CreateChildCloseUI(pMessageBox_Date_UI);
				pMessageBox_Date_UI->SetPanelUI_type(PANEL_UI_TYPE::MESSAGEBOX_UI);
				CreateObject(pMessageBox_Date_UI, GROUP_TYPE::UI);
				return;
			}
			else
				CScene_Lobby::SetCheckDate(true);
		}
		else if (L"Player_Btn_Study_UI" == pBtn->GetName())
		{
			if (CScene_Lobby::GetCheckStudy())
			{
				CTexture* pPlayer_Btn_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Message_UI", L"texture\\Resource\\Messagebox.png");

				CPanelUI* pMessageBox_Study_UI = new CPanelUI(PANEL_UI_TYPE::MESSAGEBOX_UI);  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
				pMessageBox_Study_UI->SetName(L"MessageBox_Study_UI");
				pMessageBox_Study_UI->SetScale(Vec2(300.f, 200.f));
				pMessageBox_Study_UI->SetPos(Vec2(pBtn->GetFinalPos()));
				pMessageBox_Study_UI->SetUItex(pPlayer_Btn_UItex);
				pMessageBox_Study_UI->SetMovement(true);
				pMessageBox_Study_UI->SetHasText(true);
				pMessageBox_Study_UI->SetHasTexture(true);
				pMessageBox_Study_UI->CreateChildCloseUI(pMessageBox_Study_UI);
				pMessageBox_Study_UI->SetPanelUI_type(PANEL_UI_TYPE::MESSAGEBOX_UI);
				CreateObject(pMessageBox_Study_UI, GROUP_TYPE::UI);
				return;
			}
			else
				CScene_Lobby::SetCheckStudy(true);
		}
		else if (L"Player_Btn_Job_UI" == pBtn->GetName())
		{
			if (CScene_Lobby::GetCheckJob())
			{
				CTexture* pPlayer_Btn_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Message_UI", L"texture\\Resource\\Messagebox.png");

				CPanelUI* pMessageBox_Job_UI = new CPanelUI(PANEL_UI_TYPE::MESSAGEBOX_UI);  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
				pMessageBox_Job_UI->SetName(L"MessageBox_Job_UI");
				pMessageBox_Job_UI->SetScale(Vec2(300.f, 200.f));
				pMessageBox_Job_UI->SetPos(Vec2(pBtn->GetFinalPos()));
				pMessageBox_Job_UI->SetUItex(pPlayer_Btn_UItex);
				pMessageBox_Job_UI->SetMovement(true);
				pMessageBox_Job_UI->SetHasText(true);
				pMessageBox_Job_UI->SetHasTexture(true);
				pMessageBox_Job_UI->CreateChildCloseUI(pMessageBox_Job_UI);
				pMessageBox_Job_UI->SetPanelUI_type(PANEL_UI_TYPE::MESSAGEBOX_UI);
				CreateObject(pMessageBox_Job_UI, GROUP_TYPE::UI);
				return;
			}
			else
				CScene_Lobby::SetCheckJob(true);
		}
		else if (L"Player_Btn_Drinking_UI" == pBtn->GetName())
		{
			if (CScene_Lobby::GetCheckDrinking())
			{
				CTexture* pPlayer_Btn_UItex = CResMgr::GetInst()->LoadTexture(L"Player_Message_UI", L"texture\\Resource\\Messagebox.png");

				CPanelUI* pMessageBox_Drinking_UI = new CPanelUI(PANEL_UI_TYPE::MESSAGEBOX_UI);  // 항상 UI가 뜨게하지말고 버튼을 눌렀을때만 뜨게하는거 고민 해봐야함
				pMessageBox_Drinking_UI->SetName(L"MessageBox_Drinking_UI");
				pMessageBox_Drinking_UI->SetScale(Vec2(300.f, 200.f));
				pMessageBox_Drinking_UI->SetPos(Vec2(pBtn->GetFinalPos()));
				pMessageBox_Drinking_UI->SetUItex(pPlayer_Btn_UItex);
				pMessageBox_Drinking_UI->SetMovement(true);
				pMessageBox_Drinking_UI->SetHasText(true);
				pMessageBox_Drinking_UI->SetHasTexture(true);
				pMessageBox_Drinking_UI->CreateChildCloseUI(pMessageBox_Drinking_UI);
				pMessageBox_Drinking_UI->SetPanelUI_type(PANEL_UI_TYPE::MESSAGEBOX_UI);
				CreateObject(pMessageBox_Drinking_UI, GROUP_TYPE::UI);
				return;
			}
			else
				CScene_Lobby::SetCheckDrinking(true);
		}
	}

	
	SCENE_TYPE scene_type = (SCENE_TYPE)_pa;
	ChangeScene(scene_type);
}


void CALLBACK LobbyMapRegulate(HWND hWnd, UINT message, WPARAM wParam)
{

	// 임시 보류 카메라 원근감 구현하려면 깊이 버퍼 공부해야함 일단 보류


	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//	
	//if (L"Lobby Scene" != pCurScene->GetName())
	//	return;
	//
	//Vec2 vPos = CCamera::GetInst()->GetLookAt();
	//
	//
	//if ((SHORT)wParam > 0) // 휠을 위로 올렸을때
	//{
	//	//vPos = vPos * 2.f;
	//
	//}
	//
	//else if ((SHORT)wParam < 0) // 휠을 아래로 내렸을때
	//{
	//	//vPos = vPos / 2.f;
	//}
	//
	//
	//CCamera::GetInst()->SetLookAt(vPos);
}

