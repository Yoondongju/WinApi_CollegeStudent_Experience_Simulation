#include "pch.h"
#include "CSoundMgr.h"
#include "CResMgr.h"
#include "CSound.h" 

#include "CCore.h"

CSoundMgr::CSoundMgr()
	: m_pSound(nullptr)
	, m_pBGM(nullptr)
{
}

CSoundMgr::~CSoundMgr()
{
}

int CSoundMgr::init(void)
{
	if (FAILED(DirectSoundCreate8(NULL, &m_pSound, NULL)))
	{
		MessageBox(NULL, L"사운드디바이스생성실패", L"SYSTEM ERROR", MB_OK);
		return false;
	}

	// 사운드 디바이스 협조레벨 설정.
	HWND hWnd = CCore::GetInst()->GetMainHwnd();
	if (FAILED(m_pSound->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE))) // Flag값 정리
	{
		MessageBox(NULL, L"사운드디바이스 협조레벨 설정", L"SYSTEM ERROR", MB_OK);
		return false;
	}


	CResMgr::GetInst()->LoadSound(L"Lodding_BGM", L"sound\\Lodding_sound.wav");
	CResMgr::GetInst()->LoadSound(L"Job_BGM", L"sound\\Job_sound.wav");
	CResMgr::GetInst()->LoadSound(L"Lobby_BGM", L"sound\\Lobby_sound.wav");

	CResMgr::GetInst()->LoadSound(L"Drinking_BGM", L"sound\\drinking_sound.wav");
	CResMgr::GetInst()->LoadSound(L"Toilet_BGM", L"sound\\toilet_sound.wav");
	CResMgr::GetInst()->LoadSound(L"Smoking_BGM", L"sound\\smoking_sound.wav");

	CResMgr::GetInst()->LoadSound(L"Study_BGM", L"sound\\study_sound.wav");
	CResMgr::GetInst()->LoadSound(L"HiddenStudy_BGM", L"sound\\hiddenStudy_sound.wav");


	CResMgr::GetInst()->LoadSound(L"MT_going_BGM", L"sound\\MT_going.wav");
	CResMgr::GetInst()->LoadSound(L"MT_dance_BGM", L"sound\\MT_dance.wav");
	CResMgr::GetInst()->LoadSound(L"MT_main_BGM", L"sound\\MT_main.wav");
	CResMgr::GetInst()->LoadSound(L"MT_serious_BGM", L"sound\\MT_serious.wav");
	CResMgr::GetInst()->LoadSound(L"MT_end_BGM", L"sound\\MT_end.wav");



	CResMgr::GetInst()->LoadSound(L"Date_going_BGM", L"sound\\date_sound.wav");
	CResMgr::GetInst()->LoadSound(L"Date_gosu_BGM", L"sound\\date_gosu.wav");


	CResMgr::GetInst()->LoadSound(L"BUTTON_CLICK_SOUND", L"sound\\button_sound.wav");
	CResMgr::GetInst()->LoadSound(L"TEXT_SOUND", L"sound\\text_sound.wav");

	CResMgr::GetInst()->LoadSound(L"ENDDING_BGM", L"sound\\borderless_sound.wav"); 


	return true;
}

void CSoundMgr::RegisterToBGM(CSound* _pSound)
{
	if (m_pBGM != nullptr)
		m_pBGM->Stop(true);

	m_pBGM = _pSound;
}