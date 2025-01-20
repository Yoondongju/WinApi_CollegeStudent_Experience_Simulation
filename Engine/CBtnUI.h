#pragma once
#include "CUI.h"

#include "CScene.h"	   // ���� ������� ��� ������ �����ѵ� ��� �Լ������͸� ����ϱ����� ��¿������ ����..
#include "CObject.h"   // ���� ������� ��� ������ �����ѵ� ��� �Լ������͸� ����ϱ����� ��¿������ ����..


// �Լ� ������
typedef void(*BTN_FUNC) (DWORD_PTR, DWORD_PTR);

// Scene ��� �Լ�������
typedef void(CScene:: *SCENE_MEMFUNC)(void);

// Object ��� �Լ�������
typedef void(CObject::* OBJECT_MEMFUNC)(void);

// Object ��� �Լ�������
typedef void(CObject::* OBJECT_MEMFUNC_ARG)(CObject *);

class CEffect;

class CBtnUI : public CUI
{

private:
	BTN_FUNC	m_pFunc;  // ���� �Լ�������
	DWORD_PTR	m_param1;
	DWORD_PTR	m_param2;


	SCENE_MEMFUNC			m_pSceneFunc; // ��� �Լ�������
	CScene*					m_pSceneInst; // ��� �Լ��� �θ������� �� �ν��Ͻ�


	OBJECT_MEMFUNC			m_pObjFunc; // ��� �Լ�������
	CObject*				m_pObjInst; // ��� �Լ��� �θ������� �� �ν��Ͻ�

	OBJECT_MEMFUNC_ARG		m_pObjFunc_Arg; // ��� �Լ������� �Ű�������
	CObject*				m_pObjarg;		// �Ű�������    


	Vec2		m_vOffset;    // �θ� UI�� ������

	CEffect*	m_pEffect;	  // ��ư�� ����ִ� ����Ʈ

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
	void SetClickedCallBack(BTN_FUNC _pFunc , DWORD_PTR _param1,  DWORD_PTR _param2) // �����Լ� ��Ͽ�
	{
		m_pFunc = _pFunc;
		m_param1 = _param1;
		m_param2 = _param2;
	}
	

	void SetClickedCallBack(CScene* _Scene, SCENE_MEMFUNC _pSceneFunc) // Scene ����Լ� ��Ͽ�
	{
		m_pSceneInst = _Scene;
		m_pSceneFunc = _pSceneFunc;
	}

	void SetClickedCallBack(CObject* _Obj, OBJECT_MEMFUNC _pObjFunc) // Object ����Լ� ��Ͽ�
	{
		m_pObjInst = _Obj;
		m_pObjFunc = _pObjFunc;
	}

	void SetClickedCallBack(CObject* _Obj, OBJECT_MEMFUNC_ARG _pObjFunc, CObject* obj) // Object ����Լ� �Ű����� �ִ¹���? ��Ͽ�
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

