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
	// UI ������
	POINT pResolution = CCore::GetInst()->GetResolution();
	Vec2  vResolution;
	vResolution = pResolution;



	// Ÿ�� ����
	CreateTile();

}


void CScene_MapTool::Exit()
{
	CCore::GetInst()->DividMenu();

	DeleteAll();
}


