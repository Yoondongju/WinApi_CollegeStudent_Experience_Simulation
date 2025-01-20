#include "pch.h"
#include "CCore.h"
#include "CObject.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"

#include "CTexture.h"
#include "CSound.h"

#include "CResMgr.h"

#include "SelectGDI.h"
#include "resource.h"

#include "CAnimator.h"

#include "CScene_Tool.h"


CCore::CCore()
	: m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)
	, m_pMemTex(nullptr)
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd,m_hDC);

	// DeleteDC(m_memDC);
	// DeleteObject(m_hBit);



	// Object 삭제
	for (int i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}

	DestroyMenu(m_hMenu);
	


	
}


int CCore::init(HWND _hWnd,POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	

	// 해상도에 맞게 윈도우 크기 조정
	ChangeWindowsSize(Vec2((float)_ptResolution.x, (float)_ptResolution.y), false);

	// 메뉴바 생성
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_ENGINE));

	m_hDC = GetDC(m_hWnd);

	// 이중 버퍼링 용도의 텍스쳐 한장을 만든다.
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer", (UINT)m_ptResolution.x, (UINT)m_ptResolution.y);
	

	// 이중 버퍼링 용도의 비트맵과 DC를 만듬
	// m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y); // 비트맵 id 할당
	// m_memDC = CreateCompatibleDC(m_hDC);										// 비트맵에 그릴 DC id 할당
	//HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);   // 내가 그리고싶은 비트맵과 비트맵에 그릴 DC를 연결해줌
	//DeleteObject(hOldBit);



	// 자주 사용할 펜 및 브러쉬 생성
	CreateBrushPen();

	

	// Mgr 초기화
	CPathMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CCamera::GetInst()->init();


	CSoundMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();	
	CCollisionMgr::GetInst()->init();



	





	//CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Lodding_BGM");
	////pNewSound->Play();
	//pNewSound->PlayToBGM(true);
	////pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	//pNewSound->SetVolume(50.f); // 소리 볼륨 크기




	// 폰트 가져오기
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"pont\\Maplestory.ttf"; 
	AddFontResource(strFilePath.c_str());
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
	
	strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"pont\\koverwatch.ttf";
	AddFontResource(strFilePath.c_str());
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);



	// =========================================================
	SetBkMode(m_pMemTex->GetDC(), TRANSPARENT);					// 폰트 배경 투명색
	
	
	

	UINT align = TA_LEFT | TA_TOP;  // 왼쪽 상단에 텍스트를 정렬
	SetTextAlign(m_pMemTex->GetDC(), align);

	HFONT hFont, oldFont;
	hFont = CreateFont(18, 0, 0, 0, 500, 0, 0, 0,
		HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("koverwatch"));
	oldFont = (HFONT)SelectObject(m_pMemTex->GetDC(), hFont);

	SetTextColor(m_pMemTex->GetDC(), RGB(255, 255, 255));		// 폰트 색 검은색


	return S_OK;
}


void CCore::progress()
{
	// Manager update
	CTimeMgr::GetInst()->update(); // DT 구해주는거 DT는 한 프레임당 걸리는시간
	CKeyMgr::GetInst()->update();
	CCamera::GetInst()->update();


	// Scene Update
	CSceneMgr::GetInst()->update();

	// 충돌체크
	CCollisionMgr::GetInst()->update(); // 여기서 미사일 문제생김

	// UI 이벤트 처리
	CUIMgr::GetInst()->update();





	// =============
	// Rendering
	// =============
	// 화면 Clear
	Clear();

	CSceneMgr::GetInst()->render(m_pMemTex->GetDC()); // 어떤 비트맵에다 다 그려놓고와서
	CCamera::GetInst()->render(m_pMemTex->GetDC());

	// ======================== 맵 툴 전 용 씬 ==============================
	


	
	// 우리가 진짜 관리하고있는 mainDC에다 보내버림
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_pMemTex->GetDC(), 0, 0, SRCCOPY); // ccore가 관리하는 dc에 옮김


	 
	CTimeMgr::GetInst()->render();	

	// ===============================================
	// 이벤트 지연처리
	CEventMgr::GetInst()->update();

	CSceneMgr::GetInst()->subsequent_update(); // 씬 전환부분 ?
}



void CCore::Clear()
{
	SelectGDI gdi2(m_pMemTex->GetDC(), BRUSH_TYPE::BLACK);
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);


}


void CCore::CreateBrushPen()
{
	
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::WHITE] = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);
	

	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::BULE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	
}

void CCore::DockingMenu()
{
	SetMenu(m_hWnd , m_hMenu);
	ChangeWindowsSize(GetResolution(), true);

}


void CCore::DividMenu()
{
	SetMenu(m_hWnd, nullptr);
	ChangeWindowsSize(GetResolution(), false);
}


void CCore::ChangeWindowsSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rt = { 0,0,_vResolution.x,_vResolution.y };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	SetWindowPos(m_hWnd, nullptr,
		100,
		100,
		rt.right - rt.left,
		rt.bottom - rt.top, 0);
}


