#include "pch.h"
#include "CScene_Loding2.h"
#include "CCamera.h"
#include "CResMgr.h"
#include "CCore.h"

#include "CUI.h"
#include "CTalkUI.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include <iostream>
#include <conio.h>

#include <chrono>
#include <thread>

#include "CSound.h"



CScene_Loding2::CScene_Loding2()
{
}

CScene_Loding2::~CScene_Loding2()
{
}



void CScene_Loding2::update()
{
	CScene::update();

	if (KEY_CHECK(KEY::Z, KEY_STATE::TAP))
	{
		ChangeScene(SCENE_TYPE::STAGE_LOBBY);
	}

}

void CScene_Loding2::subsequent_update()
{
	static bool a = false;
	
	vector<CObject*> vecObj = GetUIGroup();
	for (size_t i = 0; i < vecObj.size(); ++i)
	{
		CTalkUI* pTalkUI = dynamic_cast<CTalkUI*>(vecObj[i]);
		if (pTalkUI->IsStart() && !a)
		{
			a = true;
			Waiting_Time(1.0f);
			ChangeScene(SCENE_TYPE::STAGE_LOBBY);
		}
	
	}
}



void CScene_Loding2::render(HDC _dc)
{
	CScene::render(_dc);

	
}


void CScene_Loding2::Enter()
{

	//CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Lodding_BGM");
	////pNewSound->Play();
	//pNewSound->PlayToBGM(true);
	////pNewSound->SetPosition(1.f); // ��з� , �Ҹ���ġ
	//pNewSound->SetVolume(50.f); // �Ҹ� ���� ũ��

	Vec2  vResolution = CCore::GetInst()->GetResolution();

	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"Loding_scene2", L"texture\\Loding_scene2.png");


	// Panel UI ����
	CUI* pTalkUI = new CTalkUI;
	pTalkUI->SetName(L"Loding2_TalkUI");
	pTalkUI->SetScale(Vec2(vResolution.x, vResolution.y)); 
	pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f));
	pTalkUI->SetMovement(false);   // ���� �� �����̴µ� �׽�Ʈ������ �ϴ� �����ϼ� �ְ� �س���
	pTalkUI->SetHasText(true);
	pTalkUI->SetUItex(pTex);
	pTalkUI->SetHasTexture(true);
	pTalkUI->CreateChildUI(pTalkUI);
	AddObject(pTalkUI, GROUP_TYPE::UI);


	// Camera LookAt����
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
}





void CScene_Loding2::Exit()
{
	DeleteAll();
}
