#pragma once

#include "CCamera.h"

class CCollider;
class CAnimator;
class CRigidBody;

class CObject
{
private:
	wstring	m_strName;

	Vec2	m_vEarlyPos;  // 초기위치
	Vec2	m_vPos;
	Vec2	m_vScale;


	// Component
	CCollider*		m_pCollider;
	CAnimator*		m_pAnimator;
	CRigidBody*		m_pRigidBody;

	bool	m_bAlive;

	
	bool			m_bCamAffectedObj; // 카메라 영향받니?

	
	BLENDFUNCTION	m_bf;			// Object 마다 알파블랜딩 값을 다르게 조정하기 위해

	int			m_MyConversationID;  // 각각 npc는 본인의 대화창을 구별할수 있게 id를 부여할것임

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
	virtual void start() {};  // Scene이 시작되기 직전에 호출되는 함수 // 필요한 녀석만 오버라이딩해서 써줌
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

