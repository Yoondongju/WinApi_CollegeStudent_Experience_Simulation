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
	// Scale과 Offset 설정을 안해준 이유는 collider의 크기를 object 크기 그대로 가져가기위해
	// 어짜피 Vec2 구조체 생성자에 의해 offset과 scale이 0으로 초기화됨
{

}


CCollider::CCollider(const CCollider& _origin)
	 : m_pOwner(nullptr)
	 , m_vOffsetPos(_origin.m_vOffsetPos)
	 , m_vScale(_origin.m_vScale)   // 충돌체의 크기정보	
	 , m_iID(g_iNextID++)		   // 충돌체가 보유하고 있는 보유한 ID 값
	 , m_bActive(_origin.m_bActive)
{
}


CCollider::~CCollider()
{

}



void CCollider::finalupdate()
{
	// Object의 위치를 따라감 
	

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
