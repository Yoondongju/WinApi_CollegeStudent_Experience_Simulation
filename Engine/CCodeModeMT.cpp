#include "pch.h"
#include "CCodeModeMT.h"

#include "CTimeMgr.h"
#include "CMonster.h"

#include "CCore.h"
#include "CResMgr.h"
#include "CTalkUI.h"

#include "CScene.h"
#include "CSceneMgr.h"

#include "Scene_MainDate.h"

CCodeModeMT::CCodeModeMT()
	:CState(MON_STATE::CODEMODE_MT)
	, m_bDance(false)
	, m_barrive(false)
{

}


CCodeModeMT::~CCodeModeMT()
{


}


void CCodeModeMT::Enter()
{


}


void CCodeModeMT::update()
{
	CreateTargetPos();
	update_move();

	
}



void CCodeModeMT::CreateTargetPos()
{
	CMonster* pMon = GetMonster();
	m_barrive = false;



	if (0.f != pMon->GetTarGetPos().x || 0.f != pMon->GetTarGetPos().y)  // ���� ������ Ÿ�� pos�� �ִ°��
	{
		m_target = pMon->GetTarGetPos();
	}
	else
		m_target = pG_Player->GetPos();

}


void CCodeModeMT::update_move()
{
	CMonster* pMon = GetMonster();
	Vec2 vMonPos = pMon->GetPos();
	float vMonSpeed = pMon->GetSpeed();

	if (abs(m_target.x - vMonPos.x) < 25.f)      //   << Player�� npc�� �Ÿ��� �ƴ� Ÿ�� ��ǥ�� npc���� �Ÿ�
	{
		m_barrive = true;
	}


	if (m_barrive) // �л�ȸ�� ���л�ȸ�� ����������
	{
		if(CUSTOMER_STATE::DANCE != pMon->GetCurState())
			pMon->SetCurState(CUSTOMER_STATE::SAY);

		if(m_checkState)
			CheckState();

		if (L"DateGirl" == pMon->GetName())
			CheckDateGirl();

		return;
	}



	if (m_target.x > vMonPos.x) // ������ ����
	{
		pMon->SetDir(CUSTOMER_DIR::RIGHT);
	}
	else if (m_target.x < vMonPos.x) // ���� ����
	{
		pMon->SetDir(CUSTOMER_DIR::LEFT);
	}

	int Dir = (int)(pMon->GetDir());
	vMonPos.x += Dir * vMonSpeed * fDT * 2;
	pMon->SetCurState(CUSTOMER_STATE::WALK);
	pMon->SetPos(vMonPos);

	
}

void CCodeModeMT::CheckState()
{
	CMonster* pMon = GetMonster();
	if (L"President" == pMon->GetName())
	{
		pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);  // main Player �ൿ �Ұ��� ����
		Vec2 vResolution = CCore::GetInst()->GetResolution();
		CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Job_Talk_UI", L"texture\\TalkUI.png");

		CTalkUI* pTalkUI = new CTalkUI;
		pTalkUI->SetName(L"Job_Talk_UI");
		pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f - vResolution.y / 3.f ));
		pTalkUI->SetScale(Vec2(UI_WIDTH, UI_HEIGHT)); // ��ȭâ UI�� ũ��� ������ �� ������� ����
		pTalkUI->SetUItex(UItex);
		pTalkUI->SetMovement(false);
		pTalkUI->SetHasText(true);
		pTalkUI->SetHasTexture(true);
		pTalkUI->CreateChildUI(pTalkUI);
		pTalkUI->SetNpcOwner(pMon);  // ���⼭ ���̵� 5�� ����
		CreateObject(pTalkUI, GROUP_TYPE::UI);
		m_checkState = false;
		return;
	}


}

void CCodeModeMT::CheckDateGirl()
{
	CMonster* pMon = GetMonster();
	pMon->SetCurState(CUSTOMER_STATE::SIT);

	static bool check = false;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	Scene_MainDate* pDateScene = (Scene_MainDate*)pCurScene;


	if (pDateScene && pDateScene->m_bRestart)
	{
		CTalkUI* pUI = pDateScene->GetMainTalkUI();

		if (!pUI)
		{	
			check = false;
			pDateScene->m_bRestart = false;
		}
	}



	if (!check)
	{
		Vec2 vResolution = CCore::GetInst()->GetResolution();
		CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Job_Talk_UI", L"texture\\TalkUI.png");

		CTalkUI* pTalkUI = new CTalkUI;
		pTalkUI->SetName(L"Job_Talk_UI");
		pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f + vResolution.y / 4.f + 50));
		pTalkUI->SetScale(Vec2(UI_WIDTH, UI_HEIGHT)); // ��ȭâ UI�� ũ��� ������ �� ������� ����
		pTalkUI->SetUItex(UItex);
		pTalkUI->SetMovement(false);
		pTalkUI->SetHasText(true);
		pTalkUI->SetHasTexture(true);
		pTalkUI->CreateChildUI(pTalkUI);
		pTalkUI->SetNpcOwner(pMon);  // ���⼭ ���̵� 5�� ����
		CreateObject(pTalkUI, GROUP_TYPE::UI);
		check = true;
	}

	return;
}


void CCodeModeMT::Exit()
{

}


