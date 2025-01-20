#pragma once

class CObject;


class CCollider
{
private:
	static UINT g_iNextID;

	CObject* m_pOwner;
	Vec2	m_vOffsetPos;
	Vec2	m_vFinalPos;

	Vec2	m_vScale;			// �浹ü�� ũ������
	Vec2	m_vOffsetScale;		// �浹ü ������	

	UINT	m_iID;		 // �浹ü�� �����ϰ� �ִ� ������ ID ��
	UINT	m_iCol;

	bool	m_bActive;   // �浹ü Ȱ��ȭ üũ

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
	// �浹 ���� �Լ�
	void OnCollision(CCollider* _pOther); // �浹 ���ΰ�� ȣ��Ǵ� �Լ�
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

	CCollider& operator = (CCollider& _orgin) = delete;

public:
	CCollider();
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class CObject;
};

