#pragma once
#define UI_WIDTH 900.f
#define UI_HEIGHT 250.f

#include "CObject.h"
#include "CPlayer.h"

enum class PANEL_UI_TYPE
{
	DEFAULT_UI,
	PLAYER_UI,
	CONTENT_UI,
	MESSAGEBOX_UI,

	END
};

enum class ButtonType
{
	NONE,
	Choice,
	YES,
	NO,
	FIGHT,
	END,
	FINAL_END,
};

struct PLAYERINFO_DATA
{
	PLAYER_INFO player_Info[3];   // 어떤 능력치 올릴지?   최대 3개까지
	bool Increase_or_decrease;    // 올릴지 내릴지  true는 올리기 false는 내리기
	float Figure;				  // 얼마나 수치 변경할지 

};

struct Conversation
{
	PLAYERINFO_DATA playerInfo_data;
	int id;
	wstring text;

	ButtonType CurButtonType;
	ButtonType NextButtonType;
	ButtonType PreButtonType;

	bool three_button = false;
	bool MondissApearTriger = false;
};





class CTexture;


class CUI : public CObject
{
private:
	vector<CUI*>	m_vecChildUI;
	CUI*			m_pParentUI;

	Vec2			m_vFinalPos;

	bool			m_bCamAffected;  // UI가 카메라에 영향을 받는지
	bool			m_bMouseOn;		 // UI 위에 마우스가 있는지
	bool			m_bLbtnDown;	 // UI에 왼쪽버튼이 눌러져 있는지

	bool			m_bMovement;     // UI가 움직일수 있는지?
	bool			m_bhasTexture;	 // UI가 texture을 보유하고있는가 ?
	bool			m_bhasText;		 // UI가 text를 보유하고있는가?

	PLAYER_MBTI		m_UI_HasMbti;		 // Player에게 달아줄 Mbti를 기억하는 UI 멤버Mbti


protected:
	CTexture*		m_UItex;		// UI가 들고있는 텍스쳐
	HFONT			m_hFont;
	HFONT			m_oldFont;


public:
	Vec2 GetFinalPos() { return m_vFinalPos; }
	CUI* GetParent() { return m_pParentUI; }
	bool IsMouseOn() { return m_bMouseOn; }
	bool IsLbtnDown() { return m_bLbtnDown; }

public:

	void SetCamAffected(bool _b) { m_bCamAffected = _b; }
	bool GetCamAffected() { return m_bCamAffected; }

	void SetMovement(bool _b) { m_bMovement = _b; }
	bool GetMovement() { return m_bMovement; }

	void SetHasText(bool _b) { m_bhasText = _b; }
	bool GetHasText() { return m_bhasText; }

	void SetHasTexture(bool _b) { m_bhasTexture = _b; }
	bool GetHasTexture() { return m_bhasTexture; }

	void SetUI_HasMbti(PLAYER_MBTI _e) { m_UI_HasMbti = _e; }
	PLAYER_MBTI GetUI_HasMbti() { return m_UI_HasMbti; }

public:
	void CreateChildUI(CUI* _pParent);		// 대화창 다음으로 넘어가는 버튼 만드는 함수
	void CreateChildCloseUI(CUI* _pParent);	// UI 끄는 Close 버튼 만드는 함수
	void DeleteUI();

	void CreateChildChoiceBtn(CUI* _pParent);

	void CreateMbtiUI(CUI* _pParent);
	void Create_Font(PANEL_UI_TYPE _e);


	void AddChild(CUI* _pUI) { m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this; }
	vector<CUI*>& GetChildUI() { return m_vecChildUI; }


	void SetUItex(CTexture* _tex) { m_UItex = _tex; }
	CTexture* GetUItex() { return m_UItex; }
	 
public:
	virtual void update();
	virtual void finalupdate();
	virtual void render(HDC _dc);
	virtual void render_CamZoomIn(HDC _dc);


private:
	void update_child();
	void finalupdate_child();
	void render_child(HDC _dc);
	void render_child_CamZoomIn(HDC _dc);

	void MouseOnCheck();


public:
	virtual void MouseOn();
	virtual void MouseOut();
	
	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();
	virtual void MouseLbtnClicked();


public:
	virtual void RenderText(HDC _dc, const Conversation& _str) {}


	virtual CUI* Clone() = 0;




public:
	CUI(bool _bCamAff);
	CUI(const CUI& _origin);
	virtual ~CUI();


	friend class CUIMgr;

};

