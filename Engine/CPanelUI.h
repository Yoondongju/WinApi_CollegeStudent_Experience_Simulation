#pragma once
#include "CUI.h"

class CScene;



class CPanelUI : public CUI
{
private:
	Vec2	m_vDragStart;
	
	vector<wstring>		m_strTex[(UINT)SCENE_TYPE::END];  // 씬 마다 대화UI에 필요한 텍스트
	wstring				m_strMessageBox[6];				  // 컨텐츠 한번 이상 할경우 뜨는 메세지박스 텍스쳐



	CScene*				m_pCurScene;	// 현재 씬

	
	
	PANEL_UI_TYPE		m_PanelUI_type;	// UI 타입

	
	UINT				m_MaxTextcount;

	UINT				m_PreTextcount;	 // 이전 대화창
	UINT				m_CurTextcount;	 // 현재 대화창
	UINT				m_itextSize;	 // 텍스트 길이




public:
	void SetPanelUI_type(PANEL_UI_TYPE _e) { m_PanelUI_type = _e; }
	PANEL_UI_TYPE GetPanelUI_type() { return m_PanelUI_type; }




public:
	virtual void update();
	//virtual void finalupdate();
	virtual void render(HDC _dc);


public:
	virtual void MouseOn();
	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();


	virtual void RenderText(HDC _dc, const vector<wstring> _str);

public:

	CLONE(CPanelUI)
	
public:
	CPanelUI(PANEL_UI_TYPE _e);
	~CPanelUI();

};

