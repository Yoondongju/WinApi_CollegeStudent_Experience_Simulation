#include "pch.h"
#include "CRigidBody.h"

#include "CTimeMgr.h"

#include "CObject.h"


CRigidBody::CRigidBody()
	: m_pOwner(nullptr)
	, m_fMass(1.f)
	, m_fFricCoeff(100.f)
	, m_fMaxSpeed(200.f)
	, m_gravity(1.f)
	, m_bGround(false)
{


}

CRigidBody::~CRigidBody()
{


}


void CRigidBody::finalupdate()
{
	float fForce = m_vForce.Length();	// 힘의 크기

	if (0.f != fForce)
	{
		m_vForce.Normalize();				// 힘의 방향 ( 노말라이즈하면 레퍼런스로 반환함 )

		float fAccel = fForce / m_fMass;	// 가속도

		// 가속도
		m_vAccel = m_vForce * fAccel;

		// 속도
		m_vVelocity += m_vAccel *fDT;
	}



	// 마찰력에 의한 반대 방향으로의 가속도 
	if (!m_vVelocity.IsZero())
	{
		Vec2 vFricDir = m_vVelocity;

		Vec2 vFriction = -vFricDir.Normalize() * m_fFricCoeff * fDT;

		if (m_vVelocity.Length() <= vFriction.Length())
		{
			// 마찰 가속도가 원래 속도보다 더 클 경우
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			m_vVelocity += vFriction;
		}

	}
	

	// 속도 제한 (공기 저항 느낌 내기위해)
	if (m_fMaxSpeed < m_vVelocity.Length())
	{
		m_vVelocity = m_vVelocity.Normalize() * m_fMaxSpeed;
	}


	// 속도에 따른 이동
	Move();

	// 힘 초기화
	m_vForce = Vec2(0.f, 0.f);

}


void CRigidBody::update_gravity()
{
	 

}


void CRigidBody::Move()
{
	Vec2 vPos = m_pOwner->GetPos();

	vPos += m_vVelocity * fDT;

	m_pOwner->SetPos(vPos);
}

