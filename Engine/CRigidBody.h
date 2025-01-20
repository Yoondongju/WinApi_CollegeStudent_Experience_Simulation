#pragma once

class CObject;

class CRigidBody
{
private:	
	CObject*	m_pOwner;

	Vec2		m_vForce;		 // 방향성 정보 벡터  (방향,크기를 둘다 가지고있음  왜? 단위벡터가 아니니까)
	Vec2		m_vAccel;		 // 가속도  (크기를 질량으로 나눔)
	Vec2		m_vVelocity;	 // 속도 ( 크기속력과, 방향이 있음 )


	float		m_fMaxSpeed;	 // 물체 최대 속력

	float		m_fMass;		 // 질량 
	float		m_fFricCoeff;	 // 마찰 계수
	float		m_gravity;		 // 중력
	//float		m_gravityAccel;  // 중력 가속도

	bool		m_bGround;		 // 땅 위에 서있는지? 체크


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

