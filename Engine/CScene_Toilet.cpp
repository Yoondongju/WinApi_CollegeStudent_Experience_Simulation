#include "pch.h"
#include "CScene_Toilet.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CCore.h"
#include "CPotal.h"
#include "CResMgr.h"

#include "CTexture.h"

#include "CMonster.h"

#include "AI.h"
#include "CCodeModeState.h"

#include "CAnimator.h"

#include "CTalkUI.h"

#include "CGround.h"
#include "CCollider.h"

#include "CSound.h"


CScene_Toilet::CScene_Toilet()
	:m_ToiletFriend(nullptr)
{
}

CScene_Toilet::~CScene_Toilet()
{
}

void CScene_Toilet::Enter()
{
	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Toilet_BGM");
	//pNewSound->Play();
	pNewSound->PlayToBGM(true);
	//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	pNewSound->SetVolume(50.f); // 소리 볼륨 크기

	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"Toilet_Scene", L"texture\\Toilet_scene.png");

	CCamera::GetInst()->FadeIn(7.f);
	CCamera::GetInst()->SetTarget(nullptr);


	Vec2 vResolution = CCore::GetInst()->GetResolution();

	// ================================== 벽 만들기 ================================
	CGround* pOutline = new CGround;
	pOutline->SetPos(Vec2(vResolution.x / 2.f, 200.f));
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
	pOutline4->SetPos(Vec2(155.f, vResolution.y / 2.f));
	pOutline4->SetScale(Vec2(10.f, vResolution.y));
	pOutline4->GetCollider()->SetScale(Vec2(pOutline4->GetScale()));
	AddObject(pOutline4, GROUP_TYPE::GROUND);




	CGround* pOutline5 = new CGround;
	pOutline5->SetPos(Vec2(486.5f, 370.f));
	pOutline5->SetScale(Vec2(183.f, 210.f));
	pOutline5->GetCollider()->SetScale(Vec2(pOutline5->GetScale()));
	AddObject(pOutline5, GROUP_TYPE::GROUND);


	// ================================== 벽 만들기 ================================



	pG_Player;
	pG_Player->SetPos(Vec2(650.f, 320.f));
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);


	vector<CObject*> vec = GetUIGroup();



	

	CMonster* pManager = new CMonster;
	pManager->SetName(L"friend_2");
	pManager->SetPos(Vec2(715.f, 320.f));
	pManager->SetEarlyPos(pManager->GetPos());
	pManager->SetScale(Vec2(60.f, 70.f));
	pManager->SetFineTarget(true);
	pManager->SetDissapear_conversation(false);
	pManager->SetCurState(CUSTOMER_STATE::SAY);
	pManager->SetDir(CUSTOMER_DIR::RIGHT);
	pManager->GetAnimator()->init_Animation();
	pManager->SetMyConversationID(2);
	AddObject(pManager, GROUP_TYPE::MONSTER);


	m_ToiletFriend = pManager;



	m_bWenttoilet = true;

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::POTAL);
}

void CScene_Toilet::update()
{



	CScene::update();
	

	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);


	if (KEY_CHECK(KEY::ENTER, KEY_STATE::TAP))
	{
		ChangeScene(SCENE_TYPE::STAGE_LOBBY);
	}



	// 화장실로 들어오기전 현재 대화가 뭐였는지에 따라 다름
	







	CTalkUI* UI = GetMainTalkUI();
	

	if (nullptr != UI)
	{
		if (!UI->IsDead())
		{
			Conversation CurConversation = UI->GetCurConversation();

			if (CurConversation.text == L"\n친구2: (입을 틀어막으며 변기로 달려간다..)\n 우욱!!!!")
			{
				AI* pAI = new AI;
				pAI->AddState(new CCodeModeState);
				pAI->SetCurState(MON_STATE::CODEMODE);

				CMonster* pFriend = (CMonster*)m_ToiletFriend;
				pFriend->SetAI(pAI);
				pFriend->SetTarGetPos(Vec2(852.f, 306.f));				
			}

			if (CurConversation.MondissApearTriger)
			{
				CMonster* pFriend = (CMonster*)m_ToiletFriend;
				pFriend->SetDissapear_conversation(true);
			}
		}
	}

	


	static bool count = false;

	if (nullptr != UI)
	{
		if (UI->IsDead())
		{
			if (L"\n        (...술이 들어갈 수록 점점 정신이 몽롱해진다...)" == UI->GetPreConversation().text)
			{
				if (!count)
				{
					pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);

					AI* pAI = new AI;
					pAI->AddState(new CCodeModeState);
					pAI->SetCurState(MON_STATE::CODEMODE);

					///  AI ====================================
					CMonster* toiletMan = new CMonster;
					toiletMan->SetName(L"toiletMan");
					toiletMan->SetPos(Vec2(1122.f, 330.f));
					toiletMan->SetEarlyPos(toiletMan->GetPos());
					toiletMan->SetScale(Vec2(50.f, 60.f));
					toiletMan->SetAI(pAI);
					toiletMan->SetFineTarget(false);
					toiletMan->SetCurState(CUSTOMER_STATE::SAY);
					toiletMan->SetDir(CUSTOMER_DIR::RIGHT);
					toiletMan->GetAnimator()->init_Animation();
					toiletMan->SetMyConversationID(2);
					CreateObject(toiletMan, GROUP_TYPE::MONSTER);
					count = true;
				}

			}

			else if (ButtonType::NO == UI->GetPreConversation().CurButtonType)
			{
				// 대화중에 친구가 토하러간다
				if (nullptr != UI)
				{
					if (UI->IsDead())
					{
						if (0 == GetGroupObject(GROUP_TYPE::POTAL).size())
						{
							pG_Player->SetCurState(PLAYER_STATE::IDLE);
							CPotal* pPotal = new CPotal;
							pPotal->SetPos(Vec2(1050.f, 650.f));
							pPotal->SetScale(Vec2(100.f, 100.f));
							AddObject(pPotal, GROUP_TYPE::POTAL);
						}
					}
				}
			}
		}

	}

	const vector<CObject*> vectoiletMan = GetGroupObject(GROUP_TYPE::MONSTER);
	for (size_t i = 0; i < vectoiletMan.size(); ++i)
	{
		if (L"toiletMan" == vectoiletMan[i]->GetName() && vectoiletMan[i]->IsDead())
		{
			// 모든 컨텐츠 씬에는 항상 마지막에 끝을 알리는 어떤 트리거적인 요소가 필요한데
			// 이것을 씬 배치의 어떤 물체로 놔둘것임
			if (0 == GetGroupObject(GROUP_TYPE::POTAL).size())
			{
				pG_Player->SetCurState(PLAYER_STATE::IDLE);
				CPotal* pPotal = new CPotal;
				pPotal->SetPos(Vec2(1050.f, 650.f));
				pPotal->SetScale(Vec2(100.f, 100.f));
				AddObject(pPotal, GROUP_TYPE::POTAL);
			}
		}
	}



}

void CScene_Toilet::render(HDC _dc)
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



void CScene_Toilet::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}
