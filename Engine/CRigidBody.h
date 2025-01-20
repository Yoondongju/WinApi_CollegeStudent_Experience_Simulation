#pragma once

class CObject;

class CRigidBody
{
private:	
	CObject*	m_pOwner;

	Vec2		m_vForce;		 // ���⼺ ���� ����  (����,ũ�⸦ �Ѵ� ����������  ��? �������Ͱ� �ƴϴϱ�)
	Vec2		m_vAccel;		 // ���ӵ�  (ũ�⸦ �������� ����)
	Vec2		m_vVelocity;	 // �ӵ� ( ũ��ӷ°�, ������ ���� )


	float		m_fMaxSpeed;	 // ��ü �ִ� �ӷ�

	float		m_fMass;		 // ���� 
	float		m_fFricCoeff;	 // ���� ���
	float		m_gravity;		 // �߷�
	//float		m_gravityAccel;  // �߷� ���ӵ�

	bool		m_bGround;		 // �� ���� ���ִ���? üũ


	// F = M * A
	// V += A * DT


public:
	void AddForce(Vec2 _vF) { m_vForce += _vF; }
	void Move();

	void SetMass(float _fMass) { m_fMass = _fMass; }
	float GetMass() { return m_fMass; }

	void SetVelocity(Vec2 _v) { m_vVelocity = _v; }
	void AddVelocity(Vec2 _v) { m_vVelocity += _v; }
	void SetMaxSpeed(float _f) { m_fMaxSpeed = _f; }

	void SetGravity(float _f) { m_gravity = _f; }
	float GetGravity() { return m_gravity; }

	void SetGround(bool _b) { m_bGround = _b; }
	bool GetGround() { return m_bGround; }


public:
	void finalupdate();
	
	void update_gravity();


public:
	CRigidBody();
	~CRigidBody();


	friend class CObject;

};

