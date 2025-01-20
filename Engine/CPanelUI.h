#pragma once
#include "CUI.h"

class CScene;



class CPanelUI : public CUI
{
private:
	Vec2	m_vDragStart;
	
	vector<wstring>		m_strTex[(UINT)SCENE_TYPE::END];  // �� ���� ��ȭUI�� �ʿ��� �ؽ�Ʈ
	wstring				m_strMessageBox[6];				  // ������ �ѹ� �̻� �Ұ�� �ߴ� �޼����ڽ� �ؽ���



	CScene*				m_pCurScene;	// ���� ��

	
	
	PANEL_UI_TYPE		m_PanelUI_type;	// UI Ÿ��

	
	UINT				m_MaxTextcount;

	UINT				m_PreTextcount;	 // ���� ��ȭâ
	UINT				m_CurTextcount;	 // ���� ��ȭâ
	UINT				m_itextSize;	 // �ؽ�Ʈ ����




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

