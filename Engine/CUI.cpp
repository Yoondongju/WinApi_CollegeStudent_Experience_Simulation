#include "pch.h"
#include "CUI.h"

#include "CKeyMgr.h"
#include "CCamera.h"

#include "SelectGDI.h"
#include "CPathMgr.h"

#include "CCore.h"

#include "CTexture.h"
#include "CBtnUI.h"

#include "CResMgr.h"

#include "CTalkUI.h"
#include "CPanelUI.h"

#include "CAnimator.h"

#include "CMonster.h"
#include "CEffect.h"

CUI::CUI(bool _bCamAff)
	: m_pParentUI(nullptr)
	, m_bCamAffected(_bCamAff)
	, m_bMouseOn(false)
	, m_UItex(nullptr)
	, m_bMovement(false) // 기본적으로 UI는 움직일수없음
	, m_bhasText(false)  // 기본적으로 UI는 text를 가지고있지 않음
	, m_bhasTexture(false)
	
{


}

CUI::CUI(const CUI& _origin)
	: CObject(_origin)
	, m_pParentUI(nullptr)
	, m_bCamAffected(_origin.m_bCamAffected) // UI가 카메라에 영향을 받는지
	, m_bMouseOn(false)	 // UI 위에 마우스가 있는지
	, m_bLbtnDown(false) // UI에 왼쪽버튼이 눌러져 있는지
	, m_bMovement(_origin.m_bMovement)   // 원본꺼 따라감
	, m_bhasText(_origin.m_bhasText)	 // 원본꺼 따라감
{

	for (size_t i = 0; i < _origin.m_vecChildUI.size(); ++i)
	{		
		AddChild(_origin.m_vecChildUI[i]->Clone());  // 자식 UI 깊은 복사 구현
	}
	

}


CUI::~CUI()
{
	Safe_Delete_Vec(m_vecChildUI);

}


void CUI::update()
{



	update_child();
}

void CUI::finalupdate()
{
	CObject::finalupdate();

	// UI의 최종 좌표를 구한다.
	m_vFinalPos = GetPos();

	if (GetParent())
	{
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}
	

	// UI Mouse 체크
	MouseOnCheck();

	


	finalupdate_child();
}

void CUI::render(HDC _dc)
{
	if (CCamera::GetInst()->GetZoomIn()) // UI는 카메라가 줌인되있을땐 카메라쪽에서 따로 렌더링해줄것임
		return;

	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	Vec2 vClientResolution = CCore::GetInst()->GetResolution();

	BLENDFUNCTION bf = GetBf();
	

	if (m_bCamAffected)
	{
		vPos = CCamera::GetInst()->GetRenderPos(vPos);
	}

	if (m_bLbtnDown)
	{
		//SelectGDI select(_dc, PEN_TYPE::GREEN);


		if (m_bhasTexture)
		{
			AlphaBlend(_dc
				, int(vPos.x - vScale.x / 2.f)
				, int(vPos.y - vScale.y / 2.f)
				, vScale.x    // 출력할 가로크기
				, vScale.y   // 출력할 세로크기
				, m_UItex->GetDC()
				, 0
				, 0
				, (int)m_UItex->Width()
				, (int)m_UItex->Height()
				, bf);
		}

		//else
		//{
		//	Rectangle(_dc
		//		, vPos.x - vScale.x / 2.f
		//		, vPos.y - vScale.y / 2.f
		//		, vPos.x + vScale.x / 2.f
		//		, vPos.y + vScale.y / 2.f
		//	);
		//}


	}

	else
	{
	
		if (m_bhasTexture)
		{
			AlphaBlend(_dc
				, int(vPos.x - vScale.x / 2.f)
				, int(vPos.y - vScale.y / 2.f)
				, vScale.x   // 출력할 가로크기
				, vScale.y   // 출력할 세로크기
				, m_UItex->GetDC()
				, 0
				, 0
				, (int)m_UItex->Width()
				, (int)m_UItex->Height()
				, bf);
		}

		 else
		 {
		 	//Rectangle(_dc
		 	//	, vPos.x - vScale.x / 2.f
		 	//	, vPos.y - vScale.y / 2.f
		 	//	, vPos.x + vScale.x / 2.f
		 	//	, vPos.y + vScale.y / 2.f
		 	//);
		 }

	}

	//if (m_bhasText)
	//{
	//
	//	Vec2 vPos = GetFinalPos();
	//	Vec2 vScale = GetScale();
	//
	//	Vec2 vPosLT = vPos - vScale / 2.2f; // 좌상단
	//	Vec2 vPosRB = vPos + vScale / 2.2f; // 우하단
	//
	//
	//	RECT rt = {
	//	  vPosLT.x
	//	, vPosLT.y 
	//	, vPosLT.x + 100
	//	, vPosRB.y + 100 };
	//
	//	DrawTextW(_dc
	//		, &m_CurConversation.text[i] // m_Textcount는 버튼누르면 하나씩 증가 다음 대화를 보여줌
	//		, 1
	//		, &rt
	//		, DT_LEFT | DT_WORDBREAK);
	//
	//	vPosLT.x += 18;
	//
	//}

	
	component_render(_dc);

	// child ui render
	render_child(_dc);
}


void CUI::render_CamZoomIn(HDC _dc)
{
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	Vec2 vClientResolution = CCore::GetInst()->GetResolution();

	BLENDFUNCTION bf = GetBf();


	if (m_bCamAffected)
	{
		vPos = CCamera::GetInst()->GetRenderPos(vPos);
	}

	if (m_bLbtnDown)
	{
		SelectGDI select(_dc, PEN_TYPE::GREEN);


		if (m_bhasTexture)
		{
			AlphaBlend(_dc
				, int(vPos.x - vScale.x / 2.f)
				, int(vPos.y - vScale.y / 2.f)
				, vScale.x    // 출력할 가로크기
				, vScale.y   // 출력할 세로크기
				, m_UItex->GetDC()
				, 0
				, 0
				, (int)m_UItex->Width()
				, (int)m_UItex->Height()
				, bf);
		}

		else
		{
			Rectangle(_dc
				, vPos.x - vScale.x / 2.f
				, vPos.y - vScale.y / 2.f
				, vPos.x + vScale.x / 2.f
				, vPos.y + vScale.y / 2.f
			);
		}


	}

	else
	{
		if (m_bhasTexture)
		{
			AlphaBlend(_dc
				, int(vPos.x - vScale.x / 2.f)
				, int(vPos.y - vScale.y / 2.f)
				, vScale.x   // 출력할 가로크기
				, vScale.y   // 출력할 세로크기
				, m_UItex->GetDC()
				, 0
				, 0
				, (int)m_UItex->Width()
				, (int)m_UItex->Height()
				, bf);
		}

		else
		{
			//Rectangle(_dc
			//	, vPos.x - vScale.x / 2.f
			//	, vPos.y - vScale.y / 2.f
			//	, vPos.x + vScale.x / 2.f
			//	, vPos.y + vScale.y / 2.f
			//);
		}
	}

	CBtnUI* p  = dynamic_cast<CBtnUI *>(this);
	if (nullptr != p)
	{
		if (!p->IsDead())
		{
			if (p->GetEffect())
			{
				p->GetEffect()->render_CamZoomIn(_dc);
			}
		}
	}


	// child ui render
	render_child_CamZoomIn(_dc);
}


void CUI::update_child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		if (!m_vecChildUI[i]->IsDead())
		{
			m_vecChildUI[i]->update();
		}
	}
}

void CUI::finalupdate_child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		if (!m_vecChildUI[i]->IsDead())
		{
			m_vecChildUI[i]->finalupdate();
		}
	
	}
}


void CUI::render_child(HDC _dc)
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		if(!m_vecChildUI[i]->IsDead())
			m_vecChildUI[i]->render(_dc);
	}
}

void CUI::render_child_CamZoomIn(HDC _dc)
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		if (!m_vecChildUI[i]->IsDead())
			m_vecChildUI[i]->render_CamZoomIn(_dc);
	}
}


void CUI::MouseOnCheck() // 마우스가 UI 영역에 들어왔는지
{
	
	Vec2 vMousePos = MOUSE_POS;
	Vec2 vScale = GetScale();


	if (m_bCamAffected)
	{
		vMousePos = CCamera::GetInst()->GetRenderPos(vMousePos);	
	}


	float x_left = m_vFinalPos.x - vScale.x / 2.f;
	float x_right = m_vFinalPos.x + vScale.x / 2.f;

	float y_top = m_vFinalPos.y - vScale.y / 2.f;
	float y_bottom = m_vFinalPos.y + vScale.y / 2.f;


	if (x_left <= vMousePos.x &&  x_right >= vMousePos.x  
		&& y_top <= vMousePos.y && y_bottom >= vMousePos.y)
	{
		m_bMouseOn = true;
	
	}
	else
	{
		m_bMouseOn = false;
	}

}

void CUI::MouseOn()
{
}

void CUI::MouseOut()
{
}

void CUI::MouseLbtnDown()
{
}

void CUI::MouseLbtnUp()
{
}

void CUI::MouseLbtnClicked()
{
}



void CUI::CreateChildUI(CUI* _pParent) // 버튼 UI 만임 
{
	
	Vec2 vParentPos = _pParent->GetPos();
	Vec2 vParentScale = _pParent->GetScale();
	Vec2 vParentRB = vParentPos + vParentScale / 2.f;

	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Talk_Btn_UI", L"texture\\BtnUI.png");

	
	
	Vec2 vOffset = Vec2(vParentRB.x - vParentScale.x / 1.2f
		, vParentRB.y - vParentScale.y / 1.2f);
	vOffset = vParentPos - vOffset;



	CBtnUI* pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"Talk_Btn_UI");
	pBtnUI->SetScale(Vec2(30.f,30.f)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
	pBtnUI->SetOffset(Vec2(vOffset));  // FinalUpdate에서 부모위치를 이미 더해주니까 순수 오프셋만 정해주자 // 근데 또 부모에 따라 달라져야하는데..
	pBtnUI->SetPos(vOffset);
	pBtnUI->SetUItex(UItex);
	pBtnUI->SetMovement(false);
	pBtnUI->SetHasText(true);
	pBtnUI->SetHasTexture(true);

	if (_pParent->GetCamAffectedObj())
	{
		pBtnUI->SetCamAffectedObj(true);
	}

	pBtnUI->SetClickedCallBack(this, (OBJECT_MEMFUNC_ARG)&CTalkUI::OnChildBtnClick, pBtnUI);

	_pParent->AddChild(pBtnUI);
}


void CUI::CreateChildCloseUI(CUI* _pParent)
{
	Vec2 vParentPos = _pParent->GetPos();
	Vec2 vParentScale = _pParent->GetScale();
	Vec2 vParentRB = vParentPos + vParentScale / 2.f;

	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Close_Btn_UI", L"texture\\close.png");



	Vec2 vOffset = Vec2(0.f, 50.f);



	CBtnUI* pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"Close_Btn_UI");
	pBtnUI->SetScale(Vec2(30.f, 30.f)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
	pBtnUI->SetOffset(Vec2(vOffset));  // FinalUpdate에서 부모위치를 이미 더해주니까 순수 오프셋만 정해주자 // 근데 또 부모에 따라 달라져야하는데..
	pBtnUI->SetPos(vOffset);
	pBtnUI->SetUItex(UItex);
	pBtnUI->SetMovement(false);
	pBtnUI->SetHasText(false);
	pBtnUI->SetHasTexture(true);
	pBtnUI->SetClickedCallBack(this, (OBJECT_MEMFUNC)&CTalkUI::DeleteUI);

	_pParent->AddChild(pBtnUI);

}


void CUI::DeleteUI() // 버튼으로 유아이 삭제하는거
{
	if (this)
	{
		DeleteObject(this);
	}

}



void CUI::CreateChildChoiceBtn(CUI* _pParent)
{
	
	Vec2 vParentPos = _pParent->GetPos();
	Vec2 vParentScale = _pParent->GetScale();
	Vec2 vParentRB = vParentPos + vParentScale / 2.f;

	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Talk_Btn_UI", L"texture\\BtnUI.png");

	



	Vec2 vLeftOffset = Vec2(-350.f, 15.f);

	CBtnUI* pBtnLeftUI = new CBtnUI;
	pBtnLeftUI->SetName(L"Choice_Left_Btn_UI");
	pBtnLeftUI->SetScale(Vec2(30.f, 30.f)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
	pBtnLeftUI->SetOffset(vLeftOffset);  // FinalUpdate에서 부모위치를 이미 더해주니까 순수 오프셋만 정해주자 // 근데 또 부모에 따라 달라져야하는데..
	pBtnLeftUI->SetPos(vLeftOffset);
	pBtnLeftUI->SetUItex(UItex);
	pBtnLeftUI->SetMovement(false);
	pBtnLeftUI->SetHasText(true);
	pBtnLeftUI->SetHasTexture(true);
	pBtnLeftUI->SetClickedCallBack(this, (OBJECT_MEMFUNC_ARG)&CTalkUI::OnChildBtnClick, pBtnLeftUI);
	_pParent->AddChild(pBtnLeftUI);




	Vec2 vRightOffset = Vec2(-350.f, 55.f);

	CBtnUI* pBtnRightUI = new CBtnUI;
	pBtnRightUI->SetName(L"Choice_Right_Btn_UI");
	pBtnRightUI->SetScale(Vec2(30.f, 30.f)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
	pBtnRightUI->SetOffset(vRightOffset);  // FinalUpdate에서 부모위치를 이미 더해주니까 순수 오프셋만 정해주자 // 근데 또 부모에 따라 달라져야하는데..
	pBtnRightUI->SetPos(vRightOffset);
	pBtnRightUI->SetUItex(UItex);
	pBtnRightUI->SetMovement(false);
	pBtnRightUI->SetHasText(true);
	pBtnRightUI->SetHasTexture(true);
	pBtnRightUI->SetClickedCallBack(this, (OBJECT_MEMFUNC_ARG)&CTalkUI::OnChildBtnClick, pBtnRightUI);
	_pParent->AddChild(pBtnRightUI);





	CTalkUI* pUI = (CTalkUI*)_pParent;
	Conversation text = pUI->GetCurConversation();

	

	if (text.three_button)
	{
		Vec2 vRightOffset = Vec2(-350.f, 95.f);

		CBtnUI* pBtnRightUI = new CBtnUI;
		pBtnRightUI->SetName(L"Choice_Bottom_Btn_UI");
		pBtnRightUI->SetScale(Vec2(30.f, 30.f)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
		pBtnRightUI->SetOffset(vRightOffset);  // FinalUpdate에서 부모위치를 이미 더해주니까 순수 오프셋만 정해주자 // 근데 또 부모에 따라 달라져야하는데..
		pBtnRightUI->SetPos(vRightOffset);
		pBtnRightUI->SetUItex(UItex);
		pBtnRightUI->SetMovement(false);
		pBtnRightUI->SetHasText(true);
		pBtnRightUI->SetHasTexture(true);
		pBtnRightUI->SetClickedCallBack(this, (OBJECT_MEMFUNC_ARG)&CTalkUI::OnChildBtnClick, pBtnRightUI);
		_pParent->AddChild(pBtnRightUI);
	}

}



void CUI::CreateMbtiUI(CUI* _pParent)
{

	Vec2 vParentPos = _pParent->GetPos();
	Vec2 vParentScale = _pParent->GetScale();
	Vec2 vParentRB = vParentPos + vParentScale / 2.f;


	Vec2 vOffset = Vec2(-130.f, 60.f);

	Vec2 vOrigin = vOffset;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			CBtnUI* pBtnUI = new CBtnUI;
			pBtnUI->SetName(L"MBTI_UI");
			pBtnUI->SetScale(Vec2(50.f, 40.f));
			pBtnUI->SetOffset(Vec2(vOffset));  // FinalUpdate에서 부모위치를 이미 더해주니까 순수 오프셋만 정해주자 // 근데 또 부모에 따라 달라져야하는데..
			pBtnUI->SetPos(vOffset);
			pBtnUI->SetMovement(false);
			pBtnUI->SetHasText(true);
			pBtnUI->SetHasTexture(false);
			pBtnUI->CreateAnimator();
			pBtnUI->GetAnimator()->Play(L"MBTI_UI", true);	
			pBtnUI->SetClickedCallBack(this, (OBJECT_MEMFUNC_ARG)&CTalkUI::StartGame , pBtnUI); // Player한테 mbti 줘야하고 게임 시작알림용 함수
			pBtnUI->SetUI_HasMbti((PLAYER_MBTI)((i * 4) + j));
			_pParent->AddChild(pBtnUI);

			
			vOffset.x = vOffset.x + 100.f;
		}
		vOffset.x = vOrigin.x;
		vOffset.y = vOffset.y + 60.f;
	}
	

}


// void CUI::Create_Font(PANEL_UI_TYPE _e)
// {
// 	
// 	if (PANEL_UI_TYPE::PLAYER_UI == _e)
// 	{
// 		UINT align = TA_LEFT | TA_TOP;  // 왼쪽 상단에 텍스트를 정렬
// 		SetTextAlign(CCore::GetInst()->GetBackBuffTex()->GetDC(), align);
// 
// 		m_hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0,
// 			HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("Maplestory"));
// 		m_oldFont = (HFONT)SelectObject(CCore::GetInst()->GetBackBuffTex()->GetDC(), m_hFont);
// 		SetTextColor(CCore::GetInst()->GetBackBuffTex()->GetDC(), RGB(0, 0, 0));		// 폰트 색 검은색
// 	}
// 	
// 
// 	else if (PANEL_UI_TYPE::CONTENT_UI == _e)
// 	{
// 		UINT align = TA_CENTER;  // 왼쪽 상단에 텍스트를 정렬
// 		SetTextAlign(CCore::GetInst()->GetBackBuffTex()->GetDC(), align);
// 
// 		m_hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0,
// 			HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("koverwatch"));
// 		m_oldFont = (HFONT)SelectObject(CCore::GetInst()->GetBackBuffTex()->GetDC(), m_hFont);
// 		SetTextColor(CCore::GetInst()->GetBackBuffTex()->GetDC(), RGB(0, 0, 0));		// 폰트 색 검은색
// 
// 	}
// 
// }

