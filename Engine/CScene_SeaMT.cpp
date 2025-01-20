#include "pch.h"
#include "CScene_SeaMT.h"

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
CScene_SeaMT::CScene_SeaMT()
{

}

CScene_SeaMT::~CScene_SeaMT()
{

}

void CScene_SeaMT::Enter()
{
	if (nullptr == pG_Player) // Loding 2 복구하면 지워야함
	{
		pG_Player = new CPlayer(PLAYER_MBTI::INTJ);  //  << 임시작업용 
	}

	pG_Player->SetScale(Vec2(50.f, 75.f));
	pG_Player->SetPos(Vec2(50.f, 520.f));
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
	pG_Player->SetMyConversationID(-1);
	pG_Player->SetspeechBubble();
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);



	CCamera::GetInst()->FadeIn(3.f);


	// ================================== 벽 만들기 ================================

	Vec2 vResolution = CCore::GetInst()->GetResolution();

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


	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"MT_serious_BGM");
	pNewSound->Play();
	pNewSound->PlayToBGM(true);
	//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	pNewSound->SetVolume(85.f); // 소리 볼륨 크기



	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"SeaMT_scene", L"texture\\SeaMT_scene.png");





	// PlayerInfo 변화함

	// Camera Look 지정

	CCamera::GetInst()->SetLookAt(Vec2(vResolution / 2.f));

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);

	// start(); 필요시 구현해서 사용 (아직 미구현)






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
	pCodeModestate6->SetOnlyRecognize(true);  // 움직임 없이 인식만 하는설정

	AI* pAI6 = new AI;
	pAI6->AddState(new CCodeModeState);
	pAI6->SetCurState(MON_STATE::CODEMODE);
	CCodeModeState* pCodeModestate7 = (CCodeModeState*)(pAI6->GetState(MON_STATE::CODEMODE));
	pCodeModestate7->SetOnlyRecognize(true);  // 움직임 없이 인식만 하는설정


	// 위치 잡아주고 앉는거 넣어주고

	// 학생 위치 잡아주기 좌표 


	CMonster* pUnknown_friend_4 = new CMonster;
	pUnknown_friend_4->SetName(L"Earnest_friend");
	pUnknown_friend_4->SetPos(Vec2(200.f, 550.f));
	pUnknown_friend_4->SetEarlyPos(pUnknown_friend_4->GetPos());
	pUnknown_friend_4->SetScale(Vec2(50.f, 70.f));
	pUnknown_friend_4->SetAI(pAI3);                     
	pUnknown_friend_4->SetFineTarget(false);
	pUnknown_friend_4->SetDissapear_conversation(false);
	pUnknown_friend_4->SetCurState(CUSTOMER_STATE::SAY);
	pUnknown_friend_4->SetDir(CUSTOMER_DIR::LEFT);
	pUnknown_friend_4->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
	pUnknown_friend_4->SetMyConversationID(0);
	pUnknown_friend_4->SetZoomInMon(false);
	pProneMon = pUnknown_friend_4;
	AddObject(pUnknown_friend_4, GROUP_TYPE::MONSTER);


	CMonster* pfriend = new CMonster;
	pfriend->SetName(L"Earnest_friend_woman2");
	pfriend->SetPos(Vec2(400.f, 550.f));
	pfriend->SetEarlyPos(pfriend->GetPos());
	pfriend->SetScale(Vec2(50.f, 70.f));
	//pfriend->SetAI(pAI6);
	pfriend->SetFineTarget(false);
	pfriend->SetDissapear_conversation(false);
	pfriend->SetCurState(CUSTOMER_STATE::SAY);
	pfriend->SetDir(CUSTOMER_DIR::RIGHT);
	pfriend->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
	pfriend->SetMyConversationID(-2);
	pfriend->SetZoomInMon(false);
	pfriend->SetSpeed(70.f);
	pHaesu = pfriend;
	AddObject(pfriend, GROUP_TYPE::MONSTER);



	CMonster* pUnknown_friend_6 = new CMonster;
	pUnknown_friend_6->SetName(L"Earnest_friend_woman");
	pUnknown_friend_6->SetPos(Vec2(500.f, 400.f));
	pUnknown_friend_6->SetEarlyPos(pUnknown_friend_6->GetPos());
	pUnknown_friend_6->SetScale(Vec2(50.f, 70.f));
	pUnknown_friend_6->SetAI(pAI5);
	pUnknown_friend_6->SetFineTarget(false);
	pUnknown_friend_6->SetDissapear_conversation(false);
	pUnknown_friend_6->SetCurState(CUSTOMER_STATE::SAY);
	pUnknown_friend_6->SetDir(CUSTOMER_DIR::LEFT);
	pUnknown_friend_6->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
	pUnknown_friend_6->SetMyConversationID(1);
	pUnknown_friend_6->SetSpeed(80.f);
	AddObject(pUnknown_friend_6, GROUP_TYPE::MONSTER);

	CMonster* pfriend_2 = new CMonster;
	pfriend_2->SetName(L"CuteBoy");
	pfriend_2->SetPos(Vec2(450.f, 400.f));
	pfriend_2->SetEarlyPos(pfriend_2->GetPos());
	pfriend_2->SetScale(Vec2(50.f, 70.f));
	pfriend_2->SetAI(pAI6);
	pfriend_2->SetFineTarget(false);
	pfriend_2->SetDissapear_conversation(false);
	pfriend_2->SetCurState(CUSTOMER_STATE::SAY);
	pfriend_2->SetDir(CUSTOMER_DIR::RIGHT);
	pfriend_2->GetAnimator()->init_Animation();   //  < 이름 변경에 따라 따로 추가로 넣어줘야함
	pfriend_2->SetMyConversationID(1);
	pfriend_2->SetZoomInMon(false);
	pfriend_2->SetSpeed(70.f);
	AddObject(pfriend_2, GROUP_TYPE::MONSTER);



	if (0 < pG_Player->GetPlayer_EnumInfo(PLAYER_INFO::FRIEND_SETING))
	{
		pfriend->SetMyConversationID(-5);
		pUnknown_friend_4->SetMyConversationID(-3);  // 친구 안챙겨줫을때
	}

}

void CScene_SeaMT::update()
{
	CTalkUI* UI = GetMainTalkUI();

	static bool bcheck = false;

	if (nullptr != UI)
	{
		if (UI->IsDead())
		{
			CObject* pOwner = UI->GetNpcOwner();
			static CMonster* pCustomer;

			int id = pOwner->GetMyConversationID();

			if (id == 0 || id == 1 || id == 2 || id == 3 || id == 4)
			{
				id++;
				pOwner->SetMyConversationID(id);

				if (pHaesu && (id - 1 == 0))
				{
					AI* pAI6 = new AI;
					pAI6->AddState(new CCodeModeState);
					pAI6->SetCurState(MON_STATE::CODEMODE);
					CCodeModeState* pCodeModestate7 = (CCodeModeState*)(pAI6->GetState(MON_STATE::CODEMODE));
					pCodeModestate7->SetOnlyRecognize(false);  // 움직임 없이 인식만 하는설정
					pHaesu->SetAI(pAI6);
					pHaesu->SetTarGetPos((Vec2(241.f, 550.f)));
				}
			}
			
			else if (id == -3)
			{
				id = -4;
				pOwner->SetMyConversationID(id);

				if (pHaesu && (id == -4))
				{
					AI* pAI6 = new AI;
					pAI6->AddState(new CCodeModeState);
					pAI6->SetCurState(MON_STATE::CODEMODE);
					CCodeModeState* pCodeModestate7 = (CCodeModeState*)(pAI6->GetState(MON_STATE::CODEMODE));
					pCodeModestate7->SetOnlyRecognize(false);  // 움직임 없이 인식만 하는설정
					pHaesu->SetAI(pAI6);
					pHaesu->SetTarGetPos((Vec2(241.f, 550.f)));
				}
			}

			else if (id == -4)
			{
				id = 2;
				pOwner->SetMyConversationID(id);

			}

			else if (id == 5)   // 여기서 최종 대화 체크 해주고
			{
				vector<CObject*>& vecMon =  GetMonsterGroup();

				for (UINT i = 0; i < vecMon.size(); ++i)
				{
					CMonster* pMon = (CMonster*)vecMon[i];

					AI* pAI = pMon->GetAI();
					CCodeModeState* pCodeModestate4 = (CCodeModeState*)(pAI->GetState(MON_STATE::CODEMODE));
					pCodeModestate4->SetOnlyRecognize(false);  // 움직임 없이 인식만 하는설정
					pMon->SetTarGetPos((Vec2(1500.f, 550.f)));

					if (L"Earnest_friend" == pMon->GetName())
					{
						pMon->SetTarGetPos((Vec2(400.f, 550.f)));
					}

				}
				
				if (nullptr != pProneMon)
				{
					pG_Player->SetAuto(true);
					pG_Player->SetAutoPos(Vec2(1400.f, pG_Player->GetPos().y));  // 의자 앉혀주기
				}
			}

			else if (id == 6)
			{
				pProneMon->SetTarGetPos((Vec2(1400.f, 550.f)));
				pProneMon->SetSpeed(70.f);
				pG_Player->SetAuto(true);
				pG_Player->SetAutoPos(Vec2(1400.f, pG_Player->GetPos().y));  // 의자 앉혀주기
				CCamera::GetInst()->FadeOut(4.f);

				bcheck = true;
			}
		}
	}

	if (pProneMon)
	{
		if (CUSTOMER_STATE::VOMIT == pProneMon->GetCurState())
		{
			pG_Player->SetAuto(false);
			pG_Player->SetPreState(PLAYER_STATE::WALK);
			pG_Player->SetCurState(PLAYER_STATE::IDLE);
			pProneMon->SetMyConversationID(6);
		}
	}


	if (bcheck && CCamera::GetInst()->IsAllCamEffect_End())
	{
		m_endMT = true;
		ChangeScene(SCENE_TYPE::MAINMT);
	}


	





	CScene::update();

	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);
}

void CScene_SeaMT::render(HDC _dc)
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



void CScene_SeaMT::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();

}


