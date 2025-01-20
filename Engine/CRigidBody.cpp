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
	float fForce = m_vForce.Length();	// ���� ũ��

	if (0.f != fForce)
	{
		m_vForce.Normalize();				// ���� ���� ( �븻�������ϸ� ���۷����� ��ȯ�� )

		float fAccel = fForce / m_fMass;	// ���ӵ�

		// ���ӵ�
		m_vAccel = m_vForce * fAccel;

		// �ӵ�
		m_vVelocity += m_vAccel *fDT;
	}



	// �����¿� ���� �ݴ� ���������� ���ӵ� 
	if (!m_vVelocity.IsZero())
	{
		Vec2 vFricDir = m_vVelocity;

		Vec2 vFriction = -vFricDir.Normalize() * m_fFricCoeff * fDT;

		if (m_vVelocity.Length() <= vFriction.Length())
		{
			// ���� ���ӵ��� ���� �ӵ����� �� Ŭ ���
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			m_vVelocity += vFriction;
		}

	}
	

	// �ӵ� ���� (���� ���� ���� ��������)
	if (m_fMaxSpeed < m_vVelocity.Length())
	{
		m_vVelocity = m_vVelocity.Normalize() * m_fMaxSpeed;
	}


	// �ӵ��� ���� �̵�
	Move();

	// �� �ʱ�ȭ
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

