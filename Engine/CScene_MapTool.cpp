#include "pch.h"
#include "CScene_MapTool.h"

#include "CCore.h"
#include "CCamera.h"


CScene_MapTool::CScene_MapTool()
{
}

CScene_MapTool::~CScene_MapTool()
{
}

void CScene_MapTool::Enter()
{
	// UI 만들어보기
	POINT pResolution = CCore::GetInst()->GetResolution();
	Vec2  vResolution;
	vResolution = pResolution;



	// 타일 생성
	CreateTile();

}


void CScene_MapTool::Exit()
{
	CCore::GetInst()->DividMenu();

	DeleteAll();
}


