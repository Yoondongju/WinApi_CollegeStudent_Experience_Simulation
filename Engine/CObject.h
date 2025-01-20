#pragma once

#include "CCamera.h"

class CCollider;
class CAnimator;
class CRigidBody;

class CObject
{
private:
	wstring	m_strName;

	Vec2	m_vEarlyPos;  // �ʱ���ġ
	Vec2	m_vPos;
	Vec2	m_vScale;


	// Component
	CCollider*		m_pCollider;
	CAnimator*		m_pAnimator;
	CRigidBody*		m_pRigidBody;

	bool	m_bAlive;

	
	bool			m_bCamAffectedObj; // ī�޶� ����޴�?

	
	BLENDFUNCTION	m_bf;			// Object ���� ���ĺ��� ���� �ٸ��� �����ϱ� ����

	int			m_MyConversationID;  // ���� npc�� ������ ��ȭâ�� �����Ҽ� �ְ� id�� �ο��Ұ���

public:
	void SetEarlyPos(Vec2 _vPos) { m_vEarlyPos = _vPos; }
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }


	Vec2 GetEarlyPos() { return m_vEarlyPos; }
	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }


	void SetBf(BLENDFUNCTION _bf) { m_bf = _bf; }
	BLENDFUNCTION GetBf() { return m_bf; }

	void SetCamAffectedObj(bool _b) { m_bCamAffectedObj = _b; }
	bool GetCamAffectedObj() { return m_bCamAffectedObj; }


	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() {return m_strName;}


	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CRigidBody* GetRigidBody() { return m_pRigidBody; }


	bool IsDead() { return !m_bAlive; }

	void CreateCollider();
	void CreateAnimator();
	void CreateRigidBody();


	virtual void OnCollision(CCollider* _pOther) {}
	virtual void OnCollisionEnter(CCollider* _pOther) {}
	virtual void OnCollisionExit(CCollider* _pOther) {}

private:
	void SetDead() { m_bAlive = false; }


public:
	void SetMyConversationID(int _n) { m_MyConversationID = _n; }
	int GetMyConversationID() { return m_MyConversationID; }


public:
	virtual void start() {};  // Scene�� ���۵Ǳ� ������ ȣ��Ǵ� �Լ� // �ʿ��� �༮�� �������̵��ؼ� ����
	virtual void update() = 0;
	virtual void finalupdate();
	virtual void render(HDC _dc);

	void component_render(HDC _dc);

	virtual CObject* Clone() = 0;

public:
	CObject();
	CObject(const CObject& _origin);

	virtual ~CObject();

	friend class CEventMgr;

};

