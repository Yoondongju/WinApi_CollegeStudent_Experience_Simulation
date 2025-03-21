#include "pch.h"
#include "CObject.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"

#include "CResMgr.h"
#include "CAnimation.h"

CObject::CObject()
	: m_vPos{}
	, m_vScale{}
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_bAlive(true)
	, m_bCamAffectedObj(true)
{

}

CObject::CObject(const CObject& _origin)
	: m_strName(_origin.m_strName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_bAlive(true)
	, m_bf(_origin.m_bf)
{
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}

	
	// m_pAnimator ���������������
	if (_origin.m_pAnimator)
	{
		CreateAnimator();	
	}

	if (_origin.m_pRigidBody)
	{
		m_pRigidBody = new CRigidBody(*_origin.m_pRigidBody);
		m_pRigidBody->m_pOwner = this;
	}

}

CObject::~CObject()
{
	if (nullptr != m_pCollider)
		delete m_pCollider;

	if (nullptr != m_pAnimator)
		delete m_pAnimator;

	if (nullptr != m_pRigidBody)
		delete m_pRigidBody;

}

void CObject::finalupdate()
{
	if (m_pCollider)
		m_pCollider->finalupdate();

	if (m_pAnimator)
		m_pAnimator->finalupdate();
	
	if (m_pRigidBody)
		m_pRigidBody->finalupdate();

}

void CObject::render(HDC _dc)
{
	//Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPos);

	
	component_render(_dc);
}

void CObject::component_render(HDC _dc)
{

	if (nullptr != m_pAnimator)
	{
		m_pAnimator->render(_dc);
	}

	if (nullptr != m_pCollider)
	{
		//m_pCollider->render(_dc);
	}

}

void CObject::CreateCollider()
{
	if (nullptr == m_pCollider)
	{
		m_pCollider = new CCollider;
		m_pCollider->m_pOwner = this;
	}
	
}

void CObject::CreateAnimator()
{
	if (nullptr == m_pAnimator)
	{
		m_pAnimator = new CAnimator;
		m_pAnimator->m_pOwner = this;

		m_pAnimator->init_Animation();
	}
}

void CObject::CreateRigidBody()
{
	if (nullptr == m_pRigidBody)
	{
		m_pRigidBody = new CRigidBody;
		m_pRigidBody->m_pOwner = this;
	}
}


