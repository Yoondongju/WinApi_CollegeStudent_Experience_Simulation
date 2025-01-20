#include "pch.h"
#include "CScene_Tool.h"
#include "CKeyMgr.h"
#include "CTile.h"
#include "CCore.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"

#include "CPlayer.h"

#include "resource.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

#include "CUIMgr.h"
#include "CCollider.h"

void ChangeScene(DWORD_PTR, DWORD_PTR);

CScene_Tool::CScene_Tool()
	: m_pUI(nullptr)
{

}

CScene_Tool::~CScene_Tool()
{

}

void CScene_Tool::update()
{
	CScene::update();

	 if (KEY_CHECK(KEY::ENTER, KEY_STATE::TAP))
	 {
	 	ChangeScene(SCENE_TYPE::START);
	 }

	SetTileIdx();

	if (KEY_CHECK(KEY::ESC, KEY_STATE::TAP)) // 강제로 포커싱 줘버리기~
	{
		CUIMgr::GetInst()->SetFocusedUI(m_pUI); 
	}

	if (KEY_CHECK(KEY::LSHIFT, KEY_STATE::TAP)) // 파일 저장하기
	{
		SaveTileData();
	}

	if (KEY_CHECK(KEY::CTRL, KEY_STATE::TAP)) // 파일 불러오기
	{
		LoadTileData();
	}

}

void CScene_Tool::Enter()
{
	// 메뉴 바 생성
	//CCore::GetInst()->DockingMenu();
	//
	//
	//// UI 만들어보기
	//POINT pResolution = CCore::GetInst()->GetResolution();
	//Vec2  vResolution;
	//vResolution = pResolution;
	//
	//
	//pG_Player = new CPlayer;		// Player가 지워지는 시점도 고려해봐야함
	//pG_Player->SetName(L"Player");
	//pG_Player->SetPos(Vec2(100.f, 100.f));
	//pG_Player->SetScale(Vec2(50.f, 75.f));
	//
	//AddObject(pG_Player, GROUP_TYPE::PLAYER);
	//RegisterPlayer(pG_Player);
	


	//CreateTile();


	// 복사본 UI
	//CUI* pClonePanel = pPanelUI->Clone();
	//pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-500.f, 0.f));
	//AddObject(pClonePanel, GROUP_TYPE::UI);



	// Camera LookAt지정
	//CCamera::GetInst()->SetLookAt(vResolution / 2.f);

}


void CScene_Tool::Exit()
{
	CCore::GetInst()->DividMenu();

	DeleteAll();

}



void CScene_Tool::SetTileIdx()
{
	 //if (KEY_CHECK(KEY::LBTN, KEY_STATE::TAP))
	 //{
	 //	Vec2 vMousePos = MOUSE_POS;
	 //	vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
	 //
	 //	int iTileX = (int)GetTileX();
	 //	int iTileY = (int)GetTileY();
	 //
	 //	
	 //
	 //	int iCol = (int)vMousePos.x / TILE_SIZE;  // 열 정보
	 //	int iRow = (int)vMousePos.y / TILE_SIZE;  // 행 정보
	 //
	 //
	 //	if (vMousePos.x < 0.f || iTileX <= iCol
	 //		|| vMousePos.y < 0.f || iTileY <= iRow)
	 //	{
	 //		return;
	 //	}
	 //
	 //	UINT iIdx = (iRow * iTileX) + iCol;
	 //
	 //	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
	 //	((CTile*)vecTile[iIdx])->AddImgIdx();
	 //}

	



}


void CScene_Tool::SaveTileData()
{
	OPENFILENAME ofn = {};

	wchar_t szName[256] = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";   // 아무 이름과 아무 확장자 모두 가능하다는 의미
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal 방식 
	if (GetSaveFileName(&ofn))  // 저장 버튼 누르면 true 반환되서 이때 진짜 저장해야함
	{
		SaveTile(szName);
	}
	 



	
}

void CScene_Tool::SaveTile(const wstring& _strFilePath)
{

	// 커널 오브젝트
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");
	
	if (nullptr == pFile)
	{
		// 파일 열기 실패
		assert(pFile);
	}
		

	// 데이터 저장 어떤걸 저장해야하지?

	UINT xCount = GetTileX();
	UINT yCount = GetTileY();
	
	fwrite(&xCount, sizeof(UINT), 1, pFile);
	fwrite(&yCount, sizeof(UINT), 1, pFile);
	
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Save(pFile);

	}


	fclose(pFile);
}

void CScene_Tool::LoadTileData()
{
	OPENFILENAME ofn = {};

	wchar_t szName[256] = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";   // 아무 이름과 아무 확장자 모두 가능하다는 의미
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	// Modal 방식 
	if (GetOpenFileName(&ofn))  // 저장 버튼 누르면 true 반환되서 이때 진짜 저장해야함
	{
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		LoadTile(strRelativePath);
	}
}



// ======================================
// Tile Count Window Proc 전역함수임 멤버함수 X
// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			UINT iXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			
			// ToolScene 인지 확인해봄 다운캐스팅으로
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);


			pToolScene->DeleteGroup(GROUP_TYPE::TILE);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		else if(LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		break;
	}



	return (INT_PTR)FALSE;
}
