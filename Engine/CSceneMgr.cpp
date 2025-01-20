#include "pch.h"
#include "CSceneMgr.h"

#include "CScene_Loding.h"
#include "CScene_Loding2.h"
#include "CScene_Lobby.h"

#include "CScene_MT.h"
#include "CScene_MainMT.h"
#include "CScene_SeaMT.h"
#include "CScene_EndMT.h"

#include "CScene_Date.h"
#include "Scene_MainDate.h"

#include "CScene_Study.h"
#include "CScene_hiddenStudy.h"

#include "CScene_Job.h"

#include "CScene_Drinking.h"
#include "CScene_Smoking.h"
#include "CScene_Toilet.h"



#include "CScene_Start.h"
#include "CScene_Tool.h"


#include "CEventMgr.h"

#include "CTimeMgr.h"

#include "CItem.h"

#include "CResMgr.h"




CSceneMgr::CSceneMgr()
	: m_arrScene{}
	, m_pCurScene(nullptr)
	, m_fSceneChangeTime(0.f)
{


}


CSceneMgr::~CSceneMgr()
{
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; i++)
	{
		if (m_arrScene[i] != nullptr)
		{
			delete m_arrScene[i];
		}
	}
}


void CSceneMgr::init()
{
	// Scene 생성
	m_arrScene[(UINT)SCENE_TYPE::LODING] = new CScene_Loding;
	m_arrScene[(UINT)SCENE_TYPE::LODING]->SetName(L"Loding Scene");

	m_arrScene[(UINT)SCENE_TYPE::LODING_2] = new CScene_Loding2;
	m_arrScene[(UINT)SCENE_TYPE::LODING_2]->SetName(L"Loding2 Scene");

	m_arrScene[(UINT)SCENE_TYPE::STAGE_LOBBY] = new CScene_Lobby;
	m_arrScene[(UINT)SCENE_TYPE::STAGE_LOBBY]->SetName(L"Lobby Scene");

	// ======================================================

	m_arrScene[(UINT)SCENE_TYPE::MT] = new CScene_MT;
	m_arrScene[(UINT)SCENE_TYPE::MT]->SetName(L"MT Scene");
	m_arrScene[(UINT)SCENE_TYPE::MAINMT] = new CScene_MainMT;
	m_arrScene[(UINT)SCENE_TYPE::MAINMT]->SetName(L"MainMT Scene");
	m_arrScene[(UINT)SCENE_TYPE::SEA_MT] = new CScene_SeaMT;
	m_arrScene[(UINT)SCENE_TYPE::SEA_MT]->SetName(L"SeaMT Scene");
	m_arrScene[(UINT)SCENE_TYPE::END_MT] = new CScene_EndMT;
	m_arrScene[(UINT)SCENE_TYPE::END_MT]->SetName(L"EndMT Scene");

	
	m_arrScene[(UINT)SCENE_TYPE::DATE] = new CScene_Date;
	m_arrScene[(UINT)SCENE_TYPE::DATE]->SetName(L"Date Scene");
	m_arrScene[(UINT)SCENE_TYPE::MAINDATE] = new Scene_MainDate;
	m_arrScene[(UINT)SCENE_TYPE::MAINDATE]->SetName(L"MainDate Scene");


	m_arrScene[(UINT)SCENE_TYPE::STUDY] = new CScene_Study;
	m_arrScene[(UINT)SCENE_TYPE::STUDY]->SetName(L"Study Scene");
	m_arrScene[(UINT)SCENE_TYPE::HIDDENSTUDY] = new CScene_hiddenStudy;
	m_arrScene[(UINT)SCENE_TYPE::HIDDENSTUDY]->SetName(L"HiddenStudy Scene");


	m_arrScene[(UINT)SCENE_TYPE::JOB] = new CScene_Job;
	m_arrScene[(UINT)SCENE_TYPE::JOB]->SetName(L"Job Scene");



	m_arrScene[(UINT)SCENE_TYPE::DRINKING] = new CScene_Drinking;
	m_arrScene[(UINT)SCENE_TYPE::DRINKING]->SetName(L"Drinking Scene");
	m_arrScene[(UINT)SCENE_TYPE::SMOKING] = new CScene_Smoking;
	m_arrScene[(UINT)SCENE_TYPE::SMOKING]->SetName(L"Smoking Scene");
	m_arrScene[(UINT)SCENE_TYPE::TOILET] = new CScene_Toilet;
	m_arrScene[(UINT)SCENE_TYPE::TOILET]->SetName(L"Toilet Scene");





	m_arrScene[(UINT)SCENE_TYPE::ENDDING] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::ENDDING]->SetName(L"Endding Scene");


	// ==========================================================

	// m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	// m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");
	// 
	// m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CScene_Tool;
	// m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetName(L"Tool Scene");






	// 현재 씬 설정
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::LODING];
	m_pCurScene->Enter();

}


void CSceneMgr::update()
{
	m_pCurScene->update();

	m_pCurScene->finalupdate();

}

void CSceneMgr::subsequent_update()
{
	m_pCurScene->subsequent_update();
}



void CSceneMgr::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}


void CSceneMgr::ChangeScene(SCENE_TYPE _Next)
{

	m_pCurScene->Exit();

	m_pCurScene = m_arrScene[(UINT)_Next];

	Sleep(1000); // 1 초대기
	

	m_pCurScene->Enter();
	
}


