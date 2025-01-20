#pragma once
#include "CUI.h"

class CScene;
class CMonster;



class CTalkUI : public CUI
{
private:
	multimap<int,Conversation>		m_strTex[(UINT)SCENE_TYPE::END];  // 씬 마다 대화UI에 필요한 텍스트 // 키 값은 어떤 상호작용하는 친구와 한묶음 대화인걸로 체크할것임
	CScene*							m_pCurScene;					  // 현재 씬

	SCENE_TYPE			m_PreSceneCount;    // 이전 씬 대화 
	SCENE_TYPE			m_CurSceneCount;    // 현재 씬 대화

	UINT				m_MaxTextcount;

	UINT				m_PreTextcount;	 // 이전 대화창
	UINT				m_CurTextcount;	 // 현재 대화 
	UINT				m_itextSize;	 // 텍스트 길이


	Conversation		m_PreConversation;  // 이전대화
	Conversation		m_CurConversation;  // 지금 떠있는 대화			

	float				m_fTime;
	Vec2				m_vSclie;
				
	static bool			m_bStart;				// 게임 스타트했는지 체크하기위한 변수

	bool				m_isChoiceCreated;		// 선택 버튼 있니?
	bool				m_isUIRecreated;		// next 버튼 있니?

	CObject*			m_TalkingOwner;			// 현재 대화하고 있는 NPC 오너가 누구인지 알아야 대화를 매칭시킬수있음.



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







