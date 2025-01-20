#pragma once
#include "CUI.h"

#include "CScene.h"	   // 원래 헤더에서 헤더 참조가 위험한데 멤버 함수포인터를 사용하기위해 어쩔수없이 써줌..
#include "CObject.h"   // 원래 헤더에서 헤더 참조가 위험한데 멤버 함수포인터를 사용하기위해 어쩔수없이 써줌..


// 함수 포인터
typedef void(*BTN_FUNC) (DWORD_PTR, DWORD_PTR);

// Scene 멤버 함수포인터
typedef void(CScene:: *SCENE_MEMFUNC)(void);

// Object 멤버 함수포인터
typedef void(CObject::* OBJECT_MEMFUNC)(void);

// Object 멤버 함수포인터
typedef void(CObject::* OBJECT_MEMFUNC_ARG)(CObject *);

class CEffect;

class CBtnUI : public CUI
{

private:
	BTN_FUNC	m_pFunc;  // 전역 함수포인터
	DWORD_PTR	m_param1;
	DWORD_PTR	m_param2;


	SCENE_MEMFUNC			m_pSceneFunc; // 멤버 함수포인터
	CScene*					m_pSceneInst; // 멤버 함수를 부르기위한 씬 인스턴스


	OBJECT_MEMFUNC			m_pObjFunc; // 멤버 함수포인터
	CObject*				m_pObjInst; // 멤버 함수를 부르기위한 씬 인스턴스

	OBJECT_MEMFUNC_ARG		m_pObjFunc_Arg; // 멤버 함수포인터 매개변수용
	CObject*				m_pObjarg;		// 매개변수용    


	Vec2		m_vOffset;    // 부모 UI의 오프셋

	CEffect*	m_pEffect;	  // 버튼이 들고있는 이펙트

	Conversation	m_ChildConversation;

	


public:
	//virtual void update();
	//virtual void finalupdate();
	virtual void render(HDC _dc);
	virtual void render_CamZoomIn(HDC _dc);

	
public:
	void SetBtnConversation(Conversation _conversation) { m_ChildConversation = _conversation; }
	Conversation GetBtnConversation() { return m_ChildConversation; }


public:
	void SetOffset(Vec2 _v) { m_vOffset = _v; }
	Vec2 GetOffset() { return m_vOffset; }

	void SetEffect(CEffect* _effect) { m_pEffect = _effect; }
	CEffect* GetEffect() { return m_pEffect; }

public:
	virtual void MouseOn();
	virtual void MouseOut();

	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();
	virtual void MouseLbtnClicked();




public:
	void SetClickedCallBack(BTN_FUNC _pFunc , DWORD_PTR _param1,  DWORD_PTR _param2) // 전역함수 등록용
	{
		m_pFunc = _pFunc;
		m_param1 = _param1;
		m_param2 = _param2;
	}
	

	void SetClickedCallBack(CScene* _Scene, SCENE_MEMFUNC _pSceneFunc) // Scene 멤버함수 등록용
	{
		m_pSceneInst = _Scene;
		m_pSceneFunc = _pSceneFunc;
	}

	void SetClickedCallBack(CObject* _Obj, OBJECT_MEMFUNC _pObjFunc) // Object 멤버함수 등록용
	{
		m_pObjInst = _Obj;
		m_pObjFunc = _pObjFunc;
	}

	void SetClickedCallBack(CObject* _Obj, OBJECT_MEMFUNC_ARG _pObjFunc, CObject* obj) // Object 멤버함수 매개변수 있는버전? 등록용
	{
		m_pObjInst = _Obj;
		m_pObjFunc_Arg = _pObjFunc;
		m_pObjarg = obj;
	}

	
	


	CLONE(CBtnUI)

public:
	CBtnUI();
	~CBtnUI();


};

