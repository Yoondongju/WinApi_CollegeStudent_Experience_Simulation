#pragma once
#include "CUI.h"

class CScene;
class CMonster;



class CTalkUI : public CUI
{
private:
	multimap<int,Conversation>		m_strTex[(UINT)SCENE_TYPE::END];  // �� ���� ��ȭUI�� �ʿ��� �ؽ�Ʈ // Ű ���� � ��ȣ�ۿ��ϴ� ģ���� �ѹ��� ��ȭ�ΰɷ� üũ�Ұ���
	CScene*							m_pCurScene;					  // ���� ��

	SCENE_TYPE			m_PreSceneCount;    // ���� �� ��ȭ 
	SCENE_TYPE			m_CurSceneCount;    // ���� �� ��ȭ

	UINT				m_MaxTextcount;

	UINT				m_PreTextcount;	 // ���� ��ȭâ
	UINT				m_CurTextcount;	 // ���� ��ȭ 
	UINT				m_itextSize;	 // �ؽ�Ʈ ����


	Conversation		m_PreConversation;  // ������ȭ
	Conversation		m_CurConversation;  // ���� ���ִ� ��ȭ			

	float				m_fTime;
	Vec2				m_vSclie;
				
	static bool			m_bStart;				// ���� ��ŸƮ�ߴ��� üũ�ϱ����� ����

	bool				m_isChoiceCreated;		// ���� ��ư �ִ�?
	bool				m_isUIRecreated;		// next ��ư �ִ�?

	CObject*			m_TalkingOwner;			// ���� ��ȭ�ϰ� �ִ� NPC ���ʰ� �������� �˾ƾ� ��ȭ�� ��Ī��ų������.



public:
	bool IsStart()
	{
		if (m_bStart)
			return true;
		else
			return false;
	}


public:
	virtual void update();
	//virtual void finalupdate();
	virtual void render(HDC _dc);
	virtual void render_CamZoomIn(HDC _dc);


public:
	void OnChildBtnClick(CUI* pUI);

	void ApplyChoiceToPlayer_InFo(CUI* _pBtnUI);
	

	void StartGame(CUI* pUI);


public:
	void SetText();
	const multimap<int,Conversation> GetText(SCENE_TYPE _type) { return m_strTex[(UINT)_type]; }

	void SetTextCount(UINT _i) { m_CurTextcount = _i; };
	UINT GetTextCount() { return m_CurTextcount; }


public:
	void SetNpcOwner(CObject* _npc) { m_TalkingOwner = _npc; }
	CObject* GetNpcOwner() { return m_TalkingOwner; }



	virtual void RenderText(HDC _dc, Conversation _str);
	void RenderMbti(HDC _dc);
	void RenderChoiceText(HDC _dc);


public:
	Conversation	GetPreConversation() { return m_PreConversation; }
	Conversation	GetCurConversation() { return m_CurConversation; }


public:
	virtual void MouseOn();
	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();



public:
	void MoveSpeechBubble();
	void CreateChoices();
	void RecreateUI();


public:
	bool IsEnd_of_Conversation() 
	{ 
		if (ButtonType::FINAL_END == m_CurConversation.NextButtonType)
			return true;

		else
			return false;
	}

	void End_of_Conversation();


public:
	CLONE(CTalkUI)

public:
	CTalkUI();
	~CTalkUI();

};







