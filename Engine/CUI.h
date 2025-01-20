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
	PLAYER_INFO player_Info[3];   // � �ɷ�ġ �ø���?   �ִ� 3������
	bool Increase_or_decrease;    // �ø��� ������  true�� �ø��� false�� ������
	float Figure;				  // �󸶳� ��ġ �������� 

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

	bool			m_bCamAffected;  // UI�� ī�޶� ������ �޴���
	bool			m_bMouseOn;		 // UI ���� ���콺�� �ִ���
	bool			m_bLbtnDown;	 // UI�� ���ʹ�ư�� ������ �ִ���

	bool			m_bMovement;     // UI�� �����ϼ� �ִ���?
	bool			m_bhasTexture;	 // UI�� texture�� �����ϰ��ִ°� ?
	bool			m_bhasText;		 // UI�� text�� �����ϰ��ִ°�?

	PLAYER_MBTI		m_UI_HasMbti;		 // Player���� �޾��� Mbti�� ����ϴ� UI ���Mbti


protected:
	CTexture*		m_UItex;		// UI�� ����ִ� �ؽ���
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
	void CreateChildUI(CUI* _pParent);		// ��ȭâ �������� �Ѿ�� ��ư ����� �Լ�
	void CreateChildCloseUI(CUI* _pParent);	// UI ���� Close ��ư ����� �Լ�
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

