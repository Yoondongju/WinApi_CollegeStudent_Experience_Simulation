
#include "pch.h"
#include "CCore.h"
#include "CMapMgr.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CScene_MapTool.h"
#include "CTexture.h"

#include "SelectGDI.h"

CMapMgr::CMapMgr()
{


}

CMapMgr::~CMapMgr()
{
	ReleaseDC(m_hwnd, m_hdc);


}


int CMapMgr::init(HWND _hwnd, POINT _ptResolution)
{
	m_hwnd = _hwnd;
	m_hdc = GetDC(m_hwnd);

	// 이중 버퍼링 용도의 텍스쳐 한장을 만든다.
	m_backTexture = CResMgr::GetInst()->CreateTexture(L"SupBackBuffer", 1200,1000);


	m_core = CCore::GetInst();








	CSceneMgr::GetInst()->SetCurScene(SCENE_TYPE::MAP_TOOL);
	CSceneMgr::GetInst()->m_pMapScene->Enter();

	return S_OK;
}

void CMapMgr::progress()
{
	SelectGDI gdi2(m_backTexture->GetDC(), BRUSH_TYPE::BLACK);
	Rectangle(m_backTexture->GetDC(), -1, -1, 1200 + 1, 1000 + 1);


	
	
	CSceneMgr::GetInst()->m_pMapScene->render(m_backTexture->GetDC());


	// 우리가 진짜 관리하고있는 mainDC에다 보내버림
	BitBlt(m_hdc, 0, 0, 1200, 1000, m_backTexture->GetDC(), 0, 0, SRCCOPY); // ccore가 관리하는 dc에 옮김
}
