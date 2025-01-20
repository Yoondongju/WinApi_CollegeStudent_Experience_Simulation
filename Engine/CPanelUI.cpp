#include "pch.h"
#include "CCore.h"
#include "CPanelUI.h"
#include "CKeyMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"

#include "CTexture.h"

extern CPlayer* pG_Player;

CPanelUI::CPanelUI(PANEL_UI_TYPE _e) // Panel UI�� �׻� �ڽ��� � Panel�� type���� Ȯ���ؾ��� Ÿ�Ծ��� Panel ���� ����
	:CUI(false)
	, m_pCurScene(nullptr)
{
	BLENDFUNCTION bf = GetBf();
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;
	SetBf(bf);





	
	m_pCurScene = CSceneMgr::GetInst()->GetCurScene();  // ���� �� ������
	SetPanelUI_type(_e);   // Panel UI�� �׻� �ڽ��� � type���� üũ�� �����ؾ���

	if (L"Lobby Scene" == m_pCurScene->GetName())	// Lobby Scene �̸�? 
	{
		if (PANEL_UI_TYPE::PLAYER_UI == GetPanelUI_type())  // Lobby Scene�� ���� Panel UI�� � UI ?
		{		
			

			pG_Player = dynamic_cast<CPlayer*>(m_pCurScene->GetPlayer());
			wstring* pPlayerInfo = pG_Player->GetTotalPlayerInfo();

			// PlayerInfo�� �ٲ�� ������ �������� �ϰų� �Ǵ� ������Ʈ�� ��ȣ�ۿ�������
			for (size_t i = 0; i < (UINT)PLAYER_INFO::END; ++i)
			{
				m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(pPlayerInfo[i]);
			}
		}
		
		else if (PANEL_UI_TYPE::CONTENT_UI == GetPanelUI_type())
		{
			// �������� ���� ������ ������ �ٸ��� �ۼ��Ǿ���.. üũ �ؾ��� ���⼭
			// ���� �����ٰ� �ٽ� ���͵� ��¥�� �ٽ� ������� �۰� ��� ��ư ������ ���ﶧ���� ��¥�� �ٽ� �������
			
			// Content UI�� ���� �Ҽ� �ִ� ������ ��� �� �����س�
			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"MT �ٳ����");
			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"�Ұ��� �Ϸ�����");
			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"���� �ϱ�");
			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"�˹� �Ϸ�����");
			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"ģ���� �����ñ�");

			m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].push_back(L"���� �ϰ������ ü���غ���!");
		}

		else if (PANEL_UI_TYPE::MESSAGEBOX_UI == GetPanelUI_type())
		{
			// Content UI�� ���� �Ҽ� �ִ� ������ ��� �� �����س�

			m_strMessageBox[0] = L"�̹� MT�� �ٳ�Ծ�..!!";
			m_strMessageBox[1] = L"���� �Ұ����� ���ܿ�..!!";
			m_strMessageBox[2] = L"�� �̻� ���δ� �ϱ� �Ⱦ�..!!";
			m_strMessageBox[3] = L"�˹� ���� �ٸ��� �ϰ�;�..!!";
			m_strMessageBox[4] = L"�����ߴ��� ���� ������..�� �ȸ�����..!!";


			m_strMessageBox[5] = L"��� ���������ؾ��Ҽ������Ͱ���..";

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
		RenderText(_dc, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY]); // �ش� ���� ��ȭ �ؽ�Ʈ�� ��� ���͸� ����

}


void CPanelUI::RenderText(HDC _dc, const vector<wstring> _str)
{

	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();



	Vec2 vPosLT = vPos - vScale / 2.f; // Player_Panel_UI�� �»��
	Vec2 vPosRB = vPos + vScale / 2.f; // Player_Panel_UI�� ���ϴ�

	float width; 
	float height;
	

	RECT rt;
	

		// �ϴ� �׷� �� �Ʒ� 4���� ����ϱ� ���� rt �ٽ� ��ƺ�
		// �̸�,�г�,ģ��,��ģ ��´㿡 rt�� ������ �ٽ���¹��
		
	// ���� �Ʒ�ó�� �����ϸ� ��� Panel UI�� ���� ������� �׷����Եɰ���.. �������� ������ �ʿ���

	if (PANEL_UI_TYPE::PLAYER_UI == GetPanelUI_type())
	{
		width = GetChildUI()[0]->GetScale().x;	// Player_UI �ʻ�ȭ ���α���
		height = GetChildUI()[0]->GetScale().y;	// Player_UI �ʻ�ȭ ���α���

		for (size_t i = 0; i <= (UINT)PLAYER_INFO::HAVE_GIRL_FRIEND; ++i)
		{
			rt = {
			  (LONG)(vPosLT.x + width + 10)
			, (LONG)(vPosLT.y + 20 * (i + 1))
			, (LONG)(vPosRB.x)
			, (LONG)(vPosRB.y) };

			DrawTextW(_dc
				, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][i].c_str() // m_Textcount�� ��ư������ �ϳ��� ���� ���� ��ȭ�� ������
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
				, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][i].c_str() // m_Textcount�� ��ư������ �ϳ��� ���� ���� ��ȭ�� ������
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
				, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][i].c_str() // m_Textcount�� ��ư������ �ϳ��� ���� ���� ��ȭ�� ������
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
			, m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY][(UINT)CONTENT_TYPE::END].c_str() // m_Textcount�� ��ư������ �ϳ��� ���� ���� ��ȭ�� ������
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
			, m_strMessageBox[i].c_str() // m_Textcount�� ��ư������ �ϳ��� ���� ���� ��ȭ�� ������
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



