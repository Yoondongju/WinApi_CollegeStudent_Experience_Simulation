#include "pch.h"
#include "CCore.h"
#include "CPanelUI.h"
#include "CKeyMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"

#include "CTexture.h"

extern CPlayer* pG_Player;

CPanelUI::CPanelUI(PANEL_UI_TYPE _e) // Panel UI는 항상 자신이 어떤 Panel의 type인지 확인해야함 타입없는 Panel 생성 못함
	:CUI(false)
	, m_pCurScene(nullptr)
{
	BLENDFUNCTION bf = GetBf();
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;
	SetBf(bf);





	
	m_pCurScene = CSceneMgr::GetInst()->GetCurScene();  // 현재 씬 가져와
	SetPanelUI_type(_e);   // Panel UI는 항상 자신이 어떤 type인지 체크를 먼저해야함

	if (L"Lobby Scene" == m_pCurScene->GetName())	// Lobby Scene 이면? 
	{
		if (PANEL_UI_TYPE::PLAYER_UI == GetPanelUI_type())  // Lobby Scene의 많은 Panel UI중 어떤 UI ?
		{		
			

			pG_Player = dynamic_cast<CPlayer*>(m_pCurScene->GetPlayer());
			wstring* pPlayerInfo = pG_Player->GetTotalPlayerInfo();

			// PlayerInfo가 바뀌는 시점은 컨텐츠를 하거나 또는 오브젝트와 상호작용했을때
			for (size_t i = 0; i < (UINT)PLAYER_INFO::END; ++i)
			{
				m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(pPlayerInfo[i]);
			}
		}
		
		else if (PANEL_UI_TYPE::CONTENT_UI == GetPanelUI_type())
		{
			// 컨텐츠를 즐기고 왔을때 정보가 다르게 작성되야함.. 체크 해야함 여기서
			// 씬을 나갔다가 다시 들어와도 어짜피 다시 만들어짐 글고 얘는 버튼 누르고 지울때마다 어짜피 다시 만들어짐
			
			// Content UI가 내가 할수 있는 컨텐츠 목록 을 생성해냄
			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"MT 다녀오기");
			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"소개팅 하러가기");
			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"공부 하기");
			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"알바 하러가기");
			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"친구랑 술마시기");

			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"내가 하고싶은걸 체험해보자!");
		}

		else if (PANEL_UI_TYPE::MESSAGEBOX_UI == GetPanelUI_type())
		{
			// Content UI가 내가 할수 있는 컨텐츠 목록 을 생성해냄

			m_strMessageBox[0] = L"이미 MT를 다녀왔어..!!";
			m_strMessageBox[1] = L"이제 소개팅은 지겨워..!!";
			m_strMessageBox[2] = L"더 이상 공부는 하기 싫어..!!";
			m_strMessageBox[3] = L"알바 말고 다른걸 하고싶어..!!";
			m_strMessageBox[4] = L"과음했더니 몸이 안좋아..술 안먹을래..!!";


			m_strMessageBox[5] = L"모든 컨텐츠를해야할수있을것같다..";

		}
	}

}


CPanelUI::~CPanelUI()
{

}

void CPanelUI::update()
{
	CUI::update();

	
	
	


	
}

//void CPanelUI::finalupdate()
//{
//}
//

void CPanelUI::render(HDC _dc)
{
	bool a = CCamera::GetInst()->GetZoomIn();
	if (CCamera::GetInst()->GetZoomIn())
		return;


	CUI::render(_dc);

	if (GetHasText())
		RenderText(_dc, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY]); // 해당 씬의 대화 텍스트가 담긴 벡터를 보냄

}


void CPanelUI::RenderText(HDC _dc, const vector<wstring> _str)
{

	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();



	Vec2 vPosLT = vPos - vScale / 2.f; // Player_Panel_UI의 좌상단
	Vec2 vPosRB = vPos + vScale / 2.f; // Player_Panel_UI의 우하단

	float width; 
	float height;
	

	RECT rt;
	

		// 일단 그럼 저 아래 4개만 출력하기 위해 rt 다시 잡아봄
		// 이름,학년,친구,여친 출력담에 rt의 범위를 다시잡는방법
		
	// 지금 아래처럼 구현하면 모든 Panel UI가 같은 방식으로 그려지게될것임.. 문제있음 수정이 필요함

	if (PANEL_UI_TYPE::PLAYER_UI == GetPanelUI_type())
	{
		width = GetChildUI()[0]->GetScale().x;	// Player_UI 초상화 가로길이
		height = GetChildUI()[0]->GetScale().y;	// Player_UI 초상화 세로길이

		for (size_t i = 0; i <= (UINT)PLAYER_INFO::HAVE_GIRL_FRIEND; ++i)
		{
			rt = {
			  (LONG)(vPosLT.x + width + 10)
			, (LONG)(vPosLT.y + 20 * (i + 1))
			, (LONG)(vPosRB.x)
			, (LONG)(vPosRB.y) };

			DrawTextW(_dc
				, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][i].c_str() // m_Textcount는 버튼누르면 하나씩 증가 다음 대화를 보여줌
				, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][i].size()
				, &rt
				, DT_LEFT | DT_WORDBREAK);
		}

		int line_count = 0;
		for (size_t i = (UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE; i <= (UINT)PLAYER_INFO::AMOUNT_SELF_GROWTH; ++i)
		{
			rt = {
			  (LONG)(vPosLT.x + 10)
			, (LONG)(vPosLT.y + height + 20 * (line_count + 1))
			, (LONG)(vPosRB.x)
			, (LONG)(vPosRB.y) };

			DrawTextW(_dc
				, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][i].c_str() // m_Textcount는 버튼누르면 하나씩 증가 다음 대화를 보여줌
				, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][i].size()
				, &rt
				, DT_LEFT | DT_WORDBREAK);

			line_count++;
		}
	}

	else if (PANEL_UI_TYPE::CONTENT_UI == GetPanelUI_type())
	{

		for (size_t i = 0; i < (UINT)CONTENT_TYPE::END; ++i)
		{
			rt = {
			  (LONG)(vPosLT.x + 50)
			, (LONG)(vPosLT.y + 30 * (i + 3))
			, (LONG)(vPosRB.x)
			, (LONG)(vPosRB.y) };

			DrawTextW(_dc
				, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][i].c_str() // m_Textcount는 버튼누르면 하나씩 증가 다음 대화를 보여줌
				, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][i].size()
				, &rt
				, DT_LEFT | DT_WORDBREAK);
		}

		rt = {
			  (LONG)(vPosLT.x + 150.f)
			, (LONG)(vPosRB.y - 100.f)
			, (LONG)(vPosRB.x)
			, (LONG)(vPosRB.y) };

		DrawTextW(_dc
			, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][(UINT)CONTENT_TYPE::END].c_str() // m_Textcount는 버튼누르면 하나씩 증가 다음 대화를 보여줌
			, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][(UINT)CONTENT_TYPE::END].size()
			, &rt
			, DT_LEFT | DT_WORDBREAK);
	}

	else if (PANEL_UI_TYPE::MESSAGEBOX_UI == GetPanelUI_type())
	{
		rt = {
			  (LONG)vPosLT.x + 30
			, (LONG)vPosLT.y + 70
			, (LONG)vPosRB.x
			, (LONG)vPosRB.y};

		int i;
		if (L"MessageBox_MT_UI" == this->GetName())
		{
			i = 0;
		}
		else if (L"MessageBox_MT_not_met_UI" == this->GetName())
		{
			i = 5;
		}
		else if (L"MessageBox_Date_UI" == this->GetName())
		{
			i = 1;
		}
		else if (L"MessageBox_Study_UI" == this->GetName())
		{
			i = 2;
		}
		else if (L"MessageBox_Job_UI" == this->GetName())
		{
			i = 3;
		}
		else if (L"MessageBox_Drinking_UI" == this->GetName())
		{
			i = 4;
		}

		DrawTextW(_dc
			, m_strMessageBox[i].c_str() // m_Textcount는 버튼누르면 하나씩 증가 다음 대화를 보여줌
			, m_strMessageBox[i].size()
			, &rt
			, DT_LEFT | DT_WORDBREAK);
	}
		
}


void CPanelUI::MouseOn()
{
	
	if (!GetMovement())
		return;


	Vec2  v = MOUSE_POS;

	if (IsLbtnDown())
	{
		Vec2 vDiff = MOUSE_POS - m_vDragStart;

		Vec2 vCurPos = GetPos();
		vCurPos += vDiff;
		SetPos(vCurPos);

		m_vDragStart = MOUSE_POS;
	}
}

void CPanelUI::MouseLbtnDown()
{
	m_vDragStart = MOUSE_POS;
}

void CPanelUI::MouseLbtnUp()
{
}



