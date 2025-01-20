#include "pch.h"

#include "CScene_MainMT.h"

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

#include "CPotal.h"

CScene_MainMT::CScene_MainMT()
{

}

CScene_MainMT::~CScene_MainMT()
{

}


void CScene_MainMT::Enter()
{

	if (nullptr == pG_Player) // Loding 2 �����ϸ� ��������
	{
		pG_Player = new CPlayer(PLAYER_MBTI::INTJ);  //  << �ӽ��۾��� 
	}

	pG_Player->SetScale(Vec2(50.f, 75.f));
	pG_Player->SetPos(Vec2(50.f, 520.f));
	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
	pG_Player->SetMyConversationID(-1);
	AddObject(pG_Player, GROUP_TYPE::PLAYER);
	RegisterPlayer(pG_Player);


	if (m_endMT)
	{
		pG_Player->SetPos(Vec2(1200.f, 520.f)); // mt ���� ��ġ

		Vec2 vResolution = CCore::GetInst()->GetResolution();
		CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Job_Talk_UI", L"texture\\TalkUI.png");
		CTalkUI* pTalkUI = new CTalkUI;
		pTalkUI->SetName(L"Job_Talk_UI");
		pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f - vResolution.y / 3.f));
		pTalkUI->SetScale(Vec2(UI_WIDTH, UI_HEIGHT)); // ��ȭâ UI�� ũ��� ������ �� ������� ����
		pTalkUI->SetUItex(UItex);
		pTalkUI->SetMovement(false);
		pTalkUI->SetHasText(true);
		pTalkUI->SetHasTexture(true);
		pTalkUI->CreateChildUI(pTalkUI);
		pTalkUI->SetMyConversationID(-3);
		pTalkUI->SetNpcOwner(pTalkUI);
		CreateObject(pTalkUI, GROUP_TYPE::UI);

		CCamera::GetInst()->FadeIn(3.f);


		// ================================== �� ����� ================================



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
		// ================================== �� ����� ================================


		CSound* pNewSound = CResMgr::GetInst()->FindSound(L"MT_main_BGM");
		pNewSound->Play();
		pNewSound->PlayToBGM(true);
		//pNewSound->SetPosition(1.f); // ��з� , �Ҹ���ġ
		pNewSound->SetVolume(40.f); // �Ҹ� ���� ũ��



		m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"MainMT_scene", L"texture\\MainMT_scene.png");


		// PlayerInfo ��ȭ��

		// Camera Look ����

		CCamera::GetInst()->SetLookAt(Vec2(vResolution / 2.f));
		CCamera::GetInst()->SetTarget(pG_Player);

		CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
		CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::POTAL);

		// start(); �ʿ�� �����ؼ� ��� (���� �̱���)

		return;
	}

	
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Job_Talk_UI", L"texture\\TalkUI.png");
	CTalkUI* pTalkUI = new CTalkUI;
	pTalkUI->SetName(L"Job_Talk_UI");
	pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f - vResolution.y / 3.f));
	pTalkUI->SetScale(Vec2(UI_WIDTH, UI_HEIGHT)); // ��ȭâ UI�� ũ��� ������ �� ������� ����
	pTalkUI->SetUItex(UItex);
	pTalkUI->SetMovement(false);
	pTalkUI->SetHasText(true);
	pTalkUI->SetHasTexture(true);
	pTalkUI->CreateChildUI(pTalkUI);
	CreateObject(pTalkUI, GROUP_TYPE::UI);




	CCamera::GetInst()->FadeIn(3.f);


	// ================================== �� ����� ================================

	

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
	// ================================== �� ����� ================================


	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"MT_main_BGM");
	pNewSound->Play();
	pNewSound->PlayToBGM(true);
	//pNewSound->SetPosition(1.f); // ��з� , �Ҹ���ġ
	pNewSound->SetVolume(40.f); // �Ҹ� ���� ũ��



	m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"MainMT_scene", L"texture\\MainMT_scene.png");





	// PlayerInfo ��ȭ��

	// Camera Look ����

	CCamera::GetInst()->SetLookAt(Vec2(vResolution / 2.f));
	CCamera::GetInst()->SetTarget(pG_Player);

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	// start(); �ʿ�� �����ؼ� ��� (���� �̱���)

}


void CScene_MainMT::update()
{
	if (!m_endMT)
	{
		CTalkUI* UI = GetMainTalkUI();

		if (pG_Player->GetPos().x >= 490.f && pG_Player->GetPos().x <= 640.f)
		{
			if (6 == pG_Player->GetMyConversationID())
			{
				static bool bDeleteCheck = false;
				if (!bDeleteCheck)
				{
					DeleteGroup(GROUP_TYPE::ITEM);
					DeleteGroup(GROUP_TYPE::MONSTER);

					{
						DeleteGroup(GROUP_TYPE::UI);
						UI = nullptr;
						SetMainTalkUI(nullptr);
					}

					bDeleteCheck = true;
				}
				pG_Player->SetCurState(PLAYER_STATE::IDLE);
			}
			else
				pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);

			static bool bItemCheck = false;

			if (!bItemCheck)
			{
				ItemDeployment();
				bItemCheck = true;
				pG_Player->SetMyConversationID(1);
				pG_Player->SetspeechBubble();
			}
		}



		if (nullptr != UI)
		{
			Conversation CurConversation = UI->GetCurConversation();

			if (UI->IsDead())
			{
				CObject* pOwner = UI->GetNpcOwner();
				static CMonster* pCustomer;

				if (nullptr != pOwner)  // Owner�� ������
				{
					int id = pOwner->GetMyConversationID();
					if (id == 2)
					{
						// �̶� �������� �ڸ�ã�� ģ�� ����
						AI* pAI = new AI;
						pAI->AddState(new CPatrol);
						pAI->AddState(new CCodeModeState);
						pAI->SetCurState(MON_STATE::CODEMODE);
						CCodeModeState* pCodeModestate4 = (CCodeModeState*)(pAI->GetState(MON_STATE::CODEMODE));
						pCodeModestate4->SetReTarget(true);

						pCustomer = new CMonster;
						pCustomer->SetName(L"Earnest_friend_woman2");
						pCustomer->SetPos(Vec2(1300.f, 520.f));
						pCustomer->SetEarlyPos(pCustomer->GetPos());
						pCustomer->SetScale(Vec2(45.f, 70.f));
						pCustomer->SetAI(pAI);
						pCustomer->SetCurState(CUSTOMER_STATE::WALK);
						pCustomer->GetAnimator()->init_Animation();
						pCustomer->SetMyConversationID(-2);
						pCustomer->SetDissapear_conversation(false);
						pCustomer->SetZoomInMon(false);
						pCustomer->SetTarGetPos(Vec2(490.f, 520.f));
						pCustomer->SetSpeed(100.f);
						AddObject(pCustomer, GROUP_TYPE::MONSTER);
					}
					else if (id == 7)  // �Ϳ���¦ģ�� ReTargerPos
					{
						CMonster* pMon = dynamic_cast<CMonster*>(pOwner);
						pMon->SetTarGetPos(Vec2(1250.f, 520.f));
						UINT n = pMon->GetMyConversationID();
						pMon->SetMyConversationID(++n);
					}

					if (CurConversation.text == L"							(ģ��2�� ������ ���Ǵ� ���ư���...)")
					{
						pCustomer->SetTarGetPos(Vec2(1400.f, 520.f));
					}
					else if (CurConversation.text == L"��: ���ְ� �Ծ�~!")
					{
						pCustomer->SetIsDeleteAI(true);
					}
					else if (CurConversation.text == L"��: ����ͱ���..?\n��..�׷�! ���ְԸԾ�!")
					{
						pCustomer->SetIsDeleteAI(true);
					}






				}
				else if (nullptr == pOwner)
				{
					pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
					pG_Player->SetspeechBubble();
				}


			}
		}



		if (pG_Player->GetPos().x >= 800.f)
		{
			static bool bItemCheck = false;
			if (!bItemCheck)
			{

				AI* pAI3 = new AI;
				pAI3->AddState(new CCodeModeState);
				pAI3->SetCurState(MON_STATE::CODEMODE);
				CCodeModeState* pCodeModestate4 = (CCodeModeState*)(pAI3->GetState(MON_STATE::CODEMODE));
				pCodeModestate4->SetOnlyRecognize(false);  // Player���� �;���
				pCodeModestate4->SetReTarget(true);

				CMonster* pCuteBoy = new CMonster;
				pCuteBoy->SetName(L"CuteBoy");
				pCuteBoy->SetPos(Vec2(1400.f, 520.f));
				pCuteBoy->SetEarlyPos(pCuteBoy->GetPos());
				pCuteBoy->SetScale(Vec2(50.f, 60.f));
				pCuteBoy->SetSpeed(85.f);
				pCuteBoy->SetAI(pAI3);
				pCuteBoy->SetFineTarget(false);
				pCuteBoy->SetDissapear_conversation(false);
				pCuteBoy->SetCurState(CUSTOMER_STATE::WALK);
				pCuteBoy->SetDir(CUSTOMER_DIR::LEFT);
				pCuteBoy->GetAnimator()->init_Animation();   //  < �̸� ���濡 ���� ���� �߰��� �־������
				pCuteBoy->SetMyConversationID(7);
				AddObject(pCuteBoy, GROUP_TYPE::MONSTER);

				bItemCheck = true;
			}

			vector<CObject*>  vecMon = GetGroupObject(GROUP_TYPE::MONSTER);
			if (nullptr != UI)
			{
				Conversation CurConversation = UI->GetCurConversation();

				if (!UI->IsDead())
				{
					if (CurConversation.MondissApearTriger)
					{
						CMonster* pCuteboy = (CMonster*)vecMon[0];
						pCuteboy->SetDissapear_conversation(true);
					}
				}
			}

			if (0 == vecMon.size())
			{
				ChangeScene(SCENE_TYPE::SEA_MT);
			}
		}

	}

	else if (m_endMT)
	{
		CTalkUI* UI = GetMainTalkUI();
		if (nullptr != UI)
		{
			if (UI->IsDead())
			{
				CPotal* pPotal = new CPotal;
				pPotal->SetPos(Vec2(1050.f, 520.f));
				pPotal->SetScale(Vec2(100.f, 100.f));
				AddObject(pPotal, GROUP_TYPE::POTAL);
			}
		}
	}
	


	CScene::update();

	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);

}





void CScene_MainMT::render(HDC _dc)
{
	Vec2 vClientResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vClientResolution / 2.f;
	float xCamera = pG_Player->GetPos().x;

	float LT = xCamera;																			// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
	float vRatio = (LT) / (int)m_pBackground_Texture->Width();									// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
																								// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
	LT += ((int)m_pBackground_Texture->Width() - (int)vClientResolution.x) * vRatio;			// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
																								// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
	if (LT <= 0.f)																				// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
		LT = 0.f;																				// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
																								// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
	else if (LT + (int)vClientResolution.x >= (int)m_pBackground_Texture->Width())				// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
	{																							// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
		float gap = (LT + (int)vClientResolution.x) - (int)m_pBackground_Texture->Width();		// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
		LT -= gap;																				// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
	}																							// ���� �����س��� ��ĳ�޴��� �𸣰���  �۵� ok
	


	if (m_pBackground_Texture)
	{
		AlphaBlend(_dc
			, 0
			, 0
			, (int)vClientResolution.x  // ����� ����ũ��
			, (int)vClientResolution.y  // ����� ����ũ��
			, m_pBackground_Texture->GetDC()
			, LT								  // ���Ⱑ �ٲ�����
			, 0									  // ���Ⱑ �ٲ�����
			, (int)vClientResolution.x
			, (int)m_pBackground_Texture->Height()// ���Ⱑ �ٲ�����
			, m_pBackground_Texture->GetBf());

	}


	//wchar_t buffer[256];
	//swprintf(buffer, sizeof(buffer), L"Mouse Position: (%d, %d)", m_cursorPos.x, m_cursorPos.y);
	//TextOutW(_dc, m_cursorPos.x, m_cursorPos.y, (LPCWSTR)buffer, wcslen(buffer));

	CScene::render(_dc);

}



void CScene_MainMT::ItemDeployment()
{
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"Meet", L"texture\\meet.png");
	CTexture* pTexDrink = CResMgr::GetInst()->LoadTexture(L"Drink", L"texture\\Drink.png");

	CMTItem* pMeet = new CMTItem(); 
	pMeet->SetName(L"Meet");
	pMeet->SetScale(Vec2(40.f, 40.f));
	pMeet->SetPos(Vec2(500.f, 420.f));
	pMeet->SetTexItem(pTex);
	CreateObject(pMeet, GROUP_TYPE::ITEM);

	CBtnUI* pMeetBtn = new CBtnUI;
	pMeetBtn->SetName(L"Book_UI");
	pMeetBtn->SetScale(Vec2(40.f, 40.f));
	pMeetBtn->SetPos(pMeet->GetPos());
	pMeetBtn->SetMovement(false);
	pMeetBtn->SetHasText(false);
	pMeetBtn->SetHasTexture(false);
	pMeetBtn->SetClickedCallBack(pMeet, (OBJECT_MEMFUNC)&CMTItem::ItemClickSpeech);
	CreateObject(pMeetBtn, GROUP_TYPE::UI); // ������ִ� ����



	CMTItem* pMeet1 = new CMTItem();
	pMeet1->SetName(L"Meet");
	pMeet1->SetScale(Vec2(40.f, 40.f));
	pMeet1->SetPos(Vec2(550.f, 420.f));
	pMeet1->SetTexItem(pTex);
	CreateObject(pMeet1, GROUP_TYPE::ITEM);

	CBtnUI* pMeet1Btn = new CBtnUI;
	pMeet1Btn->SetName(L"Book_UI");
	pMeet1Btn->SetScale(Vec2(40.f, 40.f));
	pMeet1Btn->SetPos(pMeet1->GetPos());
	pMeet1Btn->SetMovement(false);
	pMeet1Btn->SetHasText(false);
	pMeet1Btn->SetHasTexture(false);
	pMeet1Btn->SetClickedCallBack(pMeet1, (OBJECT_MEMFUNC)&CMTItem::ItemClickSpeech);
	CreateObject(pMeet1Btn, GROUP_TYPE::UI); // ������ִ� ����



	CMTItem* pMeet2 = new CMTItem();
	pMeet2->SetName(L"Meet");
	pMeet2->SetScale(Vec2(40.f, 40.f));
	pMeet2->SetPos(Vec2(640.f, 420.f));
	pMeet2->SetTexItem(pTex);
	CreateObject(pMeet2, GROUP_TYPE::ITEM);

	CBtnUI* pMeet2Btn = new CBtnUI;
	pMeet2Btn->SetName(L"Book_UI");
	pMeet2Btn->SetScale(Vec2(40.f, 40.f));
	pMeet2Btn->SetPos(pMeet2->GetPos());
	pMeet2Btn->SetMovement(false);
	pMeet2Btn->SetHasText(false);
	pMeet2Btn->SetHasTexture(false);
	pMeet2Btn->SetClickedCallBack(pMeet2, (OBJECT_MEMFUNC)&CMTItem::ItemClickSpeech);
	CreateObject(pMeet2Btn, GROUP_TYPE::UI); // ������ִ� ����



	CMTItem* pDrink = new CMTItem();
	pDrink->SetName(L"Drink");
	pDrink->SetScale(Vec2(40.f, 40.f));
	pDrink->SetPos(Vec2(720.f, 420.f));
	pDrink->SetTexItem(pTexDrink);
	CreateObject(pDrink, GROUP_TYPE::ITEM);

	CBtnUI* pDrinkBtn = new CBtnUI;
	pDrinkBtn->SetName(L"Book_UI");
	pDrinkBtn->SetScale(Vec2(40.f, 40.f));
	pDrinkBtn->SetPos(pDrink->GetPos());
	pDrinkBtn->SetMovement(false);
	pDrinkBtn->SetHasText(false);
	pDrinkBtn->SetHasTexture(false);
	pDrinkBtn->SetClickedCallBack(pDrink, (OBJECT_MEMFUNC)&CMTItem::ItemClickSpeech);
	CreateObject(pDrinkBtn, GROUP_TYPE::UI); // ������ִ� ����








	// ���� �� �л��� ��ġ�� // 

	AI* pAI3 = new AI;
	pAI3->AddState(new CCodeModeState);
	pAI3->SetCurState(MON_STATE::CODEMODE);
	CCodeModeState* pCodeModestate4 = (CCodeModeState*)(pAI3->GetState(MON_STATE::CODEMODE));
	pCodeModestate4->SetOnlyRecognize(true);  // ������ ���� �νĸ� �ϴ¼���


	AI* pAI5 = new AI;
	pAI5->AddState(new CCodeModeState);
	pAI5->SetCurState(MON_STATE::CODEMODE);
	CCodeModeState* pCodeModestate6 = (CCodeModeState*)(pAI5->GetState(MON_STATE::CODEMODE));
	pCodeModestate6->SetOnlyRecognize(true);  // ������ ���� �νĸ� �ϴ¼���

	AI* pAI6 = new AI;
	pAI6->AddState(new CCodeModeState);
	pAI6->SetCurState(MON_STATE::CODEMODE);
	CCodeModeState* pCodeModestate7 = (CCodeModeState*)(pAI6->GetState(MON_STATE::CODEMODE));
	pCodeModestate7->SetOnlyRecognize(true);  // ������ ���� �νĸ� �ϴ¼���


	// ��ġ ����ְ� �ɴ°� �־��ְ�

	// �л� ��ġ ����ֱ� ��ǥ 




	CMonster* pUnknown_friend_4 = new CMonster;
	pUnknown_friend_4->SetName(L"unknown_friend_2");
	pUnknown_friend_4->SetPos(Vec2(595.f, 450.f));
	pUnknown_friend_4->SetEarlyPos(pUnknown_friend_4->GetPos());
	pUnknown_friend_4->SetScale(Vec2(50.f, 60.f));
	pUnknown_friend_4->SetAI(pAI3);
	pUnknown_friend_4->SetFineTarget(false);
	pUnknown_friend_4->SetDissapear_conversation(false);
	pUnknown_friend_4->SetCurState(CUSTOMER_STATE::SIT);
	pUnknown_friend_4->SetDir(CUSTOMER_DIR::LEFT);
	pUnknown_friend_4->GetAnimator()->init_Animation();   //  < �̸� ���濡 ���� ���� �߰��� �־������
	pUnknown_friend_4->SetMyConversationID(-1);
	AddObject(pUnknown_friend_4, GROUP_TYPE::MONSTER);




	CMonster* pUnknown_friend_6 = new CMonster;
	pUnknown_friend_6->SetName(L"unknown_friend_4");
	pUnknown_friend_6->SetPos(Vec2(675.f, 450.f));
	pUnknown_friend_6->SetEarlyPos(pUnknown_friend_6->GetPos());
	pUnknown_friend_6->SetScale(Vec2(50.f, 60.f));
	pUnknown_friend_6->SetAI(pAI5);
	pUnknown_friend_6->SetFineTarget(false);
	pUnknown_friend_6->SetDissapear_conversation(false);
	pUnknown_friend_6->SetCurState(CUSTOMER_STATE::SIT);
	pUnknown_friend_6->SetDir(CUSTOMER_DIR::RIGHT);
	pUnknown_friend_6->GetAnimator()->init_Animation();   //  < �̸� ���濡 ���� ���� �߰��� �־������
	pUnknown_friend_6->SetMyConversationID(-2);
	AddObject(pUnknown_friend_6, GROUP_TYPE::MONSTER);



	CMonster* pfriend = new CMonster;
	pfriend->SetName(L"unknown_friend_3");
	pfriend->SetPos(Vec2(790.f, 450.f));
	pfriend->SetEarlyPos(pfriend->GetPos());
	pfriend->SetScale(Vec2(50.f, 70.f));
	pfriend->SetAI(pAI6);
	pfriend->SetFineTarget(false);
	pfriend->SetDissapear_conversation(false);
	pfriend->SetCurState(CUSTOMER_STATE::SIT);
	pfriend->SetDir(CUSTOMER_DIR::LEFT);
	pfriend->GetAnimator()->init_Animation();   //  < �̸� ���濡 ���� ���� �߰��� �־������
	pfriend->SetMyConversationID(-3);
	AddObject(pfriend, GROUP_TYPE::MONSTER);



}







void CScene_MainMT::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();

}





