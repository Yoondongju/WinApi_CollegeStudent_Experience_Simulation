#pragma once

class CTexture;



class CCore
{
	SINGLE(CCore);

private:
	HWND		m_hWnd;
	HWND		m_SuphWnd;

	POINT		m_ptResolution;

	HDC			m_hDC;
	HDC			m_SuphDC;

	CTexture*	m_pMemTex;		// ����� �ؽ���



public:
	CTexture*	m_SupMemTex;    // ���� ����� �ؽ���

	


	// ���� ����ϴ� �� GDI Object
	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];

	// �޴�
	HMENU	m_hMenu;  // menu bar



public:
	int init(HWND _hWnd,POINT _ptResolution);
	void progress();

private:
	void Clear();
	void CreateBrushPen();

public:
	void DockingMenu();
	void DividMenu();
	void ChangeWindowsSize(Vec2 _vResolution, bool _bMenu);


public:
	POINT GetResolution() { return m_ptResolution; }

	HWND GetMainHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	HMENU GetMenu() { return m_hMenu; }
	CTexture* GetBackBuffTex() { return m_pMemTex; }


	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }

};


