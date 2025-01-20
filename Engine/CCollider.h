#pragma once

class CObject;


class CCollider
{
private:
	static UINT g_iNextID;

	CObject* m_pOwner;
	Vec2	m_vOffsetPos;
	Vec2	m_vFinalPos;

	Vec2	m_vScale;			// 충돌체의 크기정보
	Vec2	m_vOffsetScale;		// 충돌체 오프셋	

	UINT	m_iID;		 // 충돌체가 보유하고 있는 보유한 ID 값
	UINT	m_iCol;

	bool	m_bActive;   // 충돌체 활성화 체크

public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	

	Vec2 GetOffetPos() { return m_vOffsetPos;}
	Vec2 GetScale() { return m_vScale; }

	Vec2 GetFinalPos() { return m_vFinalPos; }


	CObject* GetObj() { return m_pOwner; }

	UINT GetID() {return m_iID;}

public:
	void finalupdate();
	void render(HDC _dc);



public:
	// 충돌 시점 함수
	void OnCollision(CCollider* _pOther); // 충돌 중인경우 호출되는 함수
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

	CCollider& operator = (CCollider& _orgin) = delete;

public:
	CCollider();
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class CObject;
};

