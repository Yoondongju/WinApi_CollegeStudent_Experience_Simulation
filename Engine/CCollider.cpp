#include "pch.h"
#include "CCollider.h"

#include "CObject.h"
#include "SelectGDI.h"
#include "CCore.h"
#include "CCamera.h"

#include "CTexture.h"


UINT CCollider::g_iNextID = 0;

CCollider::CCollider()  
	: m_pOwner(nullptr)
	, m_iID(g_iNextID++)
	, m_iCol(0)
	, m_bActive(true)
	// Scale�� Offset ������ ������ ������ collider�� ũ�⸦ object ũ�� �״�� ������������
	// ��¥�� Vec2 ����ü �����ڿ� ���� offset�� scale�� 0���� �ʱ�ȭ��
{

}


CCollider::CCollider(const CCollider& _origin)
	 : m_pOwner(nullptr)
	 , m_vOffsetPos(_origin.m_vOffsetPos)
	 , m_vScale(_origin.m_vScale)   // �浹ü�� ũ������	
	 , m_iID(g_iNextID++)		   // �浹ü�� �����ϰ� �ִ� ������ ID ��
	 , m_bActive(_origin.m_bActive)
{
}


CCollider::~CCollider()
{

}



void CCollider::finalupdate()
{
	// Object�� ��ġ�� ���� 
	

	m_vFinalPos = m_pOwner->GetPos() + m_vOffsetPos;
		
	assert(0 <= m_iCol);
		
}


void CCollider::render(HDC _dc)
{
	PEN_TYPE ePen = PEN_TYPE::GREEN;

	

	if (m_iCol)
	{
		ePen = PEN_TYPE::RED;
	}


	SelectGDI p(_dc, ePen);
	SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);



	Vec2 vRenderPos;

	//if (m_pOwner->GetCamAffectedObj())
	//	vRenderPos = CCamera::GetInst()->GetRenderPos(m_vFinalPos);
	//else
	vRenderPos = m_vFinalPos;


	Rectangle(_dc
		, (int)(vRenderPos.x - m_vScale.x / 2.f)
		, (int)(vRenderPos.y - m_vScale.y / 2.f)
		, (int)(vRenderPos.x + m_vScale.x / 2.f)
		, (int)(vRenderPos.y + m_vScale.y / 2.f));
}





void CCollider::OnCollision(CCollider* _pOther)
{
	m_pOwner->OnCollision(_pOther);

}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	++m_iCol;
	m_pOwner->OnCollisionEnter(_pOther);
	
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	--m_iCol;
	m_pOwner->OnCollisionExit(_pOther);
	
}
