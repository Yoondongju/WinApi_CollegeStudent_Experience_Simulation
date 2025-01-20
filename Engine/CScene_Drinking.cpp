#include "pch.h"
#include "CScene_Drinking.h"


#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CCore.h"
#include "CPotal.h"

#include "CMonster.h"
#include "AI.h"
#include "CPatrol.h"
#include "CCodeModeState.h"

#include "CTalkUI.h"
#include "CUI.h"

#include "CResMgr.h"

#include "CTexture.h"

#include "CGround.h"
#include "CCollider.h"

#include "CSound.h"

#include "CAnimator.h"


#include "CTable.h"


bool CScene_Drinking::m_bWenttoilet = false;
bool CScene_Drinking::m_bWentsmoking = false;


CScene_Drinking::CScene_Drinking()
{

}

CScene_Drinking::~CScene_Drinking()
{

}


void CScene_Drinking::Enter()
{

	// ================================== �� ����� ================================
	Vec2 vResolution = CCore::GetInst()->GetResolution();

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
	pOutline5->SetPos(Vec2(400.f, 325.f));
	pOutline5->SetScale(Vec2(800.f, 1.f));
	pOutline5->GetCollider()->SetScale(Vec2(pOutline5->GetScale()));
	AddObject(pOutline5, GROUP_TYPE::GROUND);

	CGround* pOutline6 = new CGround;
	pOutline6->SetPos(Vec2(1090.f, 325.f));
	pOutline6->SetScale(Vec2(380.f, 1.f));
	pOutline6->GetCollider()->SetScale(Vec2(pOutline6->GetScale()));
	AddObject(pOutline6, GROUP_TYPE::GROUND);

	CGround* pOutline5_1 = new CGround;
	pOutline5_1->SetPos(Vec2(400.f, 513.f));
	pOutline5_1->SetScale(Vec2(800.f, 1.f));
	pOutline5_1->GetCollider()->SetScale(Vec2(pOutline5_1->GetScale()));
	AddObject(pOutline5_1, GROUP_TYPE::GROUND);

	CGround* pOutline6_1 = new CGround;
	pOutline6_1->SetPos(Vec2(1090.f, 513.f));
	pOutline6_1->SetScale(Vec2(380.f, 1.f));
	pOutline6_1->GetCollider()->SetScale(Vec2(pOutline6_1->GetScale()));
	AddObject(pOutline6_1, GROUP_TYPE::GROUND);



	CGround* pOutline7 = new CGround;
	pOutline7->SetPos(Vec2(800.f, 414.f));
	pOutline7->SetScale(Vec2(1.f, 190.f));
	pOutline7->GetCollider()->SetScale(Vec2(pOutline7->GetScale()));
	AddObject(pOutline7, GROUP_TYPE::GROUND);

	CGround* pOutline8 = new CGround;
	pOutline8->SetPos(Vec2(900.f, 414.f));
	pOutline8->SetScale(Vec2(1.f, 190.f));
	pOutline8->GetCollider()->SetScale(Vec2(pOutline8->GetScale()));
	AddObject(pOutline8, GROUP_TYPE::GROUND);


	CGround* pOutline9 = new CGround;
	pOutline9->SetPos(Vec2(640.f, 235.f));
	pOutline9->SetScale(Vec2(1280.f, 1.f));
	pOutline9->GetCollider()->SetScale(Vec2(pOutline9->GetScale()));
	AddObject(pOutline9, GROUP_TYPE::GROUND);


	// ================================== �� ����� ================================
							

	if (nullptr == m_MonologueTalkUI && !m_bWenttoilet)  // ��ó�� �����
	{
		
		CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Drinking_BGM");
		//pNewSound->Play();
		pNewSound->PlayToBGM(true);
		//pNewSound->SetPosition(1.f); // ��з� , �Ҹ���ġ
		pNewSound->SetVolume(50.f); // �Ҹ� ���� ũ��

		EnterMonologue();
		return;
	}
	if (m_bWenttoilet || m_bWentsmoking)
	{
		CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Drinking_BGM");
		//pNewSound->Play();
		pNewSound->PlayToBGM(true);
		//pNewSound->SetPosition(1.f); // ��з� , �Ҹ���ġ
		pNewSound->SetVolume(50.f); // �Ҹ� ���� ũ��
	}
	if (m_MonologueTalkUI || m_bWenttoilet || m_bWentsmoking) // ���α� ��ȭ�� ���� ���� ����
	{
		CCamera::GetInst()->FadeIn(5.f);
		CCamera::GetInst()->SetTarget(nullptr);
		m_pBackground_Texture = CResMgr::GetInst()->LoadTexture(L"Drinking_Scene", L"texture\\000.png");
		pG_Player->SetPos(Vec2(50.f, 287.f));
		AddObject(pG_Player, GROUP_TYPE::PLAYER);
		RegisterPlayer(pG_Player);


		CTable* pTable1 = new CTable();  // �׻� UI�� �߰��������� ��ư�� ���������� �߰��ϴ°� ��� �غ�����
		pTable1->SetName(L"Table");
		pTable1->SetScale(Vec2(230.f, 140.f));
		pTable1->SetPos(Vec2(250.f,255.f));
		CreateObject(pTable1, GROUP_TYPE::ITEM);


		CTable* pTable2 = new CTable();  // �׻� UI�� �߰��������� ��ư�� ���������� �߰��ϴ°� ��� �غ�����
		pTable2->SetName(L"Table");
		pTable2->SetScale(Vec2(230.f, 140.f));
		pTable2->SetPos(Vec2(1030.f , 255.f));
		CreateObject(pTable2, GROUP_TYPE::ITEM);


		CTable* pTable3 = new CTable();  // �׻� UI�� �߰��������� ��ư�� ���������� �߰��ϴ°� ��� �غ�����
		pTable3->SetName(L"Table");
		pTable3->SetScale(Vec2(230.f, 140.f));
		pTable3->SetPos(Vec2(250.f,525.f));
		CreateObject(pTable3, GROUP_TYPE::ITEM);


		CTable* pTable4 = new CTable();  // �׻� UI�� �߰��������� ��ư�� ���������� �߰��ϴ°� ��� �غ�����
		pTable4->SetName(L"Table");
		pTable4->SetScale(Vec2(230.f, 140.f));
		pTable4->SetPos(Vec2(1030.f,525.f));
		CreateObject(pTable4, GROUP_TYPE::ITEM);



		// �»�� Ź�� ���̵�
		AI* pAI = new AI;
		pAI->AddState(new CCodeModeState);
		pAI->SetCurState(MON_STATE::CODEMODE);
		CCodeModeState* pCodeModestate = (CCodeModeState*)(pAI->GetState(MON_STATE::CODEMODE));
		pCodeModestate->SetOnlyRecognize(true);  // ������ ���� �νĸ� �ϴ¼���

		AI* pAI1 = new AI;
		pAI1->AddState(new CCodeModeState);
		pAI1->SetCurState(MON_STATE::CODEMODE);
		CCodeModeState* pCodeModestate2 = (CCodeModeState*)(pAI1->GetState(MON_STATE::CODEMODE));
		pCodeModestate2->SetOnlyRecognize(true);  // ������ ���� �νĸ� �ϴ¼���

		AI* pAI2 = new AI;
		pAI2->AddState(new CCodeModeState);
		pAI2->SetCurState(MON_STATE::CODEMODE);
		CCodeModeState* pCodeModestate3 = (CCodeModeState*)(pAI2->GetState(MON_STATE::CODEMODE));
		pCodeModestate3->SetOnlyRecognize(true);  // ������ ���� �νĸ� �ϴ¼���

		AI* pAI3 = new AI;
		pAI3->AddState(new CCodeModeState);
		pAI3->SetCurState(MON_STATE::CODEMODE);
		CCodeModeState* pCodeModestate4 = (CCodeModeState*)(pAI3->GetState(MON_STATE::CODEMODE));
		pCodeModestate4->SetOnlyRecognize(true);  // ������ ���� �νĸ� �ϴ¼���

		AI* pAI4 = new AI;
		pAI4->AddState(new CCodeModeState);
		pAI4->SetCurState(MON_STATE::CODEMODE);
		CCodeModeState* pCodeModestate5 = (CCodeModeState*)(pAI4->GetState(MON_STATE::CODEMODE));
		pCodeModestate5->SetOnlyRecognize(true);  // ������ ���� �νĸ� �ϴ¼���

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

		AI* pAI7 = new AI;
		pAI7->AddState(new CCodeModeState);
		pAI7->SetCurState(MON_STATE::CODEMODE);
		CCodeModeState* pCodeModestate8 = (CCodeModeState*)(pAI7->GetState(MON_STATE::CODEMODE));
		pCodeModestate8->SetOnlyRecognize(true);  // ������ ���� �νĸ� �ϴ¼���

		// ��ġ ����ְ� �ɴ°� �־��ְ�

		CMonster* pUnknown_friend = new CMonster;
		pUnknown_friend->SetName(L"unknown_friend");
		pUnknown_friend->SetPos(Vec2(185.f, 260.f));
		pUnknown_friend->SetEarlyPos(pUnknown_friend->GetPos());
		pUnknown_friend->SetScale(Vec2(50.f, 60.f));
		pUnknown_friend->SetAI(pAI);
		pUnknown_friend->SetFineTarget(false);
		pUnknown_friend->SetDissapear_conversation(false);
		pUnknown_friend->SetCurState(CUSTOMER_STATE::SIT);
		pUnknown_friend->SetDir(CUSTOMER_DIR::RIGHT);
		pUnknown_friend->GetAnimator()->init_Animation();   //  < �̸� ���濡 ���� ���� �߰��� �־������
		pUnknown_friend->SetMyConversationID(-1);
		AddObject(pUnknown_friend, GROUP_TYPE::MONSTER);


		CMonster* pUnknown_friend_2 = new CMonster;
		pUnknown_friend_2->SetName(L"unknown_friend_2");
		pUnknown_friend_2->SetPos(Vec2(315.f, 260.f));
		pUnknown_friend_2->SetEarlyPos(pUnknown_friend_2->GetPos());
		pUnknown_friend_2->SetScale(Vec2(50.f, 60.f));
		pUnknown_friend_2->SetAI(pAI1);
		pUnknown_friend_2->SetFineTarget(false);
		pUnknown_friend_2->SetDissapear_conversation(false);
		pUnknown_friend_2->SetCurState(CUSTOMER_STATE::SIT);
		pUnknown_friend_2->SetDir(CUSTOMER_DIR::LEFT);
		pUnknown_friend_2->GetAnimator()->init_Animation();   //  < �̸� ���濡 ���� ���� �߰��� �־������
		pUnknown_friend_2->SetMyConversationID(-1);
		AddObject(pUnknown_friend_2, GROUP_TYPE::MONSTER);



		

		CMonster* pUnknown_friend_3 = new CMonster;
		pUnknown_friend_3->SetName(L"unknown_friend");
		pUnknown_friend_3->SetPos(Vec2(940.f, 260.f));
		pUnknown_friend_3->SetEarlyPos(pUnknown_friend_3->GetPos());
		pUnknown_friend_3->SetScale(Vec2(50.f, 60.f));
		pUnknown_friend_3->SetAI(pAI2);
		pUnknown_friend_3->SetFineTarget(false);
		pUnknown_friend_3->SetDissapear_conversation(false);
		pUnknown_friend_3->SetCurState(CUSTOMER_STATE::SIT);
		pUnknown_friend_3->SetDir(CUSTOMER_DIR::RIGHT);
		pUnknown_friend_3->GetAnimator()->init_Animation();   //  < �̸� ���濡 ���� ���� �߰��� �־������
		pUnknown_friend_3->SetMyConversationID(-1);
		AddObject(pUnknown_friend_3, GROUP_TYPE::MONSTER);



		CMonster* pUnknown_friend_4 = new CMonster;
		pUnknown_friend_4->SetName(L"unknown_friend_2");
		pUnknown_friend_4->SetPos(Vec2(1110.f, 260.f));
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



		////////////////////////////////////////////

		CMonster* pUnknown_friend_5 = new CMonster;
		pUnknown_friend_5->SetName(L"unknown_friend_3");
		pUnknown_friend_5->SetPos(Vec2(175.f, 525.f));
		pUnknown_friend_5->SetEarlyPos(pUnknown_friend_5->GetPos());
		pUnknown_friend_5->SetScale(Vec2(50.f, 60.f));
		pUnknown_friend_5->SetAI(pAI4);
		pUnknown_friend_5->SetFineTarget(false);
		pUnknown_friend_5->SetDissapear_conversation(false);
		pUnknown_friend_5->SetCurState(CUSTOMER_STATE::SIT);
		pUnknown_friend_5->SetDir(CUSTOMER_DIR::RIGHT);
		pUnknown_friend_5->GetAnimator()->init_Animation();   //  < �̸� ���濡 ���� ���� �߰��� �־������
		pUnknown_friend_5->SetMyConversationID(-1);
		AddObject(pUnknown_friend_5, GROUP_TYPE::MONSTER);


		CMonster* pUnknown_friend_6 = new CMonster;
		pUnknown_friend_6->SetName(L"unknown_friend_4");
		pUnknown_friend_6->SetPos(Vec2(325.f, 525.f));
		pUnknown_friend_6->SetEarlyPos(pUnknown_friend_6->GetPos());
		pUnknown_friend_6->SetScale(Vec2(50.f, 60.f));
		pUnknown_friend_6->SetAI(pAI5);
		pUnknown_friend_6->SetFineTarget(false);
		pUnknown_friend_6->SetDissapear_conversation(false);
		pUnknown_friend_6->SetCurState(CUSTOMER_STATE::SIT);
		pUnknown_friend_6->SetDir(CUSTOMER_DIR::LEFT);
		pUnknown_friend_6->GetAnimator()->init_Animation();   //  < �̸� ���濡 ���� ���� �߰��� �־������
		pUnknown_friend_6->SetMyConversationID(-1);
		AddObject(pUnknown_friend_6, GROUP_TYPE::MONSTER);


		// ==================== �츮 �л�  =============
		CMonster* pfriend = new CMonster;
		pfriend->SetName(L"unknown_friend_3");
		pfriend->SetPos(Vec2(950.f, 525.f));
		pfriend->SetEarlyPos(pfriend->GetPos());
		pfriend->SetScale(Vec2(50.f, 70.f));
		pfriend->SetAI(pAI6);
		pfriend->SetFineTarget(false);
		pfriend->SetDissapear_conversation(false);
		pfriend->SetCurState(CUSTOMER_STATE::SIT);
		pfriend->SetDir(CUSTOMER_DIR::RIGHT);
		pfriend->GetAnimator()->init_Animation();   //  < �̸� ���濡 ���� ���� �߰��� �־������
		pfriend->SetMyConversationID(1);
		AddObject(pfriend, GROUP_TYPE::MONSTER);



		CMonster* pfriend_2 = new CMonster;
		pfriend_2->SetName(L"friend_2");
		pfriend_2->SetPos(Vec2(1110.f, 525.f));
		pfriend_2->SetEarlyPos(pfriend_2->GetPos());
		pfriend_2->SetScale(Vec2(50.f, 70.f));
		pfriend_2->SetAI(pAI7);
		pfriend_2->SetFineTarget(false);
		pfriend_2->SetDissapear_conversation(false);
		pfriend_2->SetCurState(CUSTOMER_STATE::SIT);
		pfriend_2->SetDir(CUSTOMER_DIR::LEFT);
		pfriend_2->GetAnimator()->init_Animation();   //  < �̸� ���濡 ���� ���� �߰��� �־������
		pfriend_2->SetMyConversationID(1);
		AddObject(pfriend_2, GROUP_TYPE::MONSTER);
		// ==================== �츮 �л�  =============

		if (!m_bWenttoilet && !m_bWentsmoking) // ȭ��ǵ� �Ȱ��� ���嵵 �Ȱ�����
			pfriend->SetMyConversationID(1);
		else if (m_bWenttoilet && !m_bWentsmoking)
			pfriend->SetMyConversationID(4);
		else if (m_bWenttoilet && m_bWentsmoking)
			pfriend->SetMyConversationID(5);

		if (!m_bWenttoilet && !m_bWentsmoking) // ȭ��ǵ� �Ȱ��� ���嵵 �Ȱ�����
			pfriend_2->SetMyConversationID(1);
		else if (m_bWenttoilet && !m_bWentsmoking)
			pfriend_2->SetMyConversationID(4);
		else if (m_bWenttoilet && m_bWentsmoking)
			pfriend_2->SetMyConversationID(5);
	
	}



	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::GROUND);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::POTAL);

	m_MonologueTalkUI = nullptr;
	// pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);  // main Player �ൿ �Ұ��� ����

}



void CScene_Drinking::EnterMonologue()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Job_Talk_UI", L"texture\\TalkUI.png");

	CTalkUI* pTalkUI = new CTalkUI;
	pTalkUI->SetName(L"Drinking_Talk_UI");
	pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f + vResolution.y / 4.f + 50));
	pTalkUI->SetScale(Vec2(UI_WIDTH, UI_HEIGHT)); // ��ȭâ UI�� ũ��� ������ �� ������� ����
	pTalkUI->SetUItex(UItex);
	pTalkUI->SetMovement(false);
	pTalkUI->SetHasText(true);
	pTalkUI->SetHasTexture(true);
	pTalkUI->CreateChildUI(pTalkUI);
	pTalkUI->SetNpcOwner(nullptr);
	m_MonologueTalkUI = pTalkUI;
	
	CreateObject(pTalkUI, GROUP_TYPE::UI);
}



void CScene_Drinking::update()
{
	CScene::update();


	GetCursorPos(&m_cursorPos);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &m_cursorPos);


	if (nullptr != m_MonologueTalkUI)
	{
		if (m_MonologueTalkUI->IsDead())
		{
			this->Enter(); // �ٽ� ��	
		}
	}
	else
	{
		CTalkUI* UI = GetMainTalkUI();
		if (nullptr != UI)
		{
			if (UI->IsDead())
			{
				if (1 == UI->GetNpcOwner()->GetMyConversationID())
				{
					if (L"\n            (����ϴ� �ֺ� ģ������ �̾߱⳪ ����..)" != UI->GetCurConversation().text)
						ChangeScene(SCENE_TYPE::TOILET);
				}

				else if (4 == UI->GetNpcOwner()->GetMyConversationID())
					ChangeScene(SCENE_TYPE::SMOKING);


				else if (5 == UI->GetNpcOwner()->GetMyConversationID())
				{
					if (0 == GetGroupObject(GROUP_TYPE::POTAL).size())
					{
						CPotal* pPotal = new CPotal;
						pPotal->SetPos(Vec2(615.f,555.f));
						pPotal->SetScale(Vec2(100.f, 100.f));
						AddObject(pPotal, GROUP_TYPE::POTAL);
					}
				}
			}
		}

	}

}

void CScene_Drinking::render(HDC _dc)
{
	Vec2 vClientResolution = CCore::GetInst()->GetResolution();

	if (m_pBackground_Texture)
	{
		AlphaBlend(_dc
			, -1100
			, 0
			, 3900  // ����� ����ũ��
			, (int)vClientResolution.y  // ����� ����ũ��
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


void CScene_Drinking::Exit()
{

	DeleteAll();
	CCollisionMgr::GetInst()->Reset();
}


