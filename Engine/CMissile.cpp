#include "pch.h"
#include "CMissile.h"

#include "CTimeMgr.h"
#include "CCollider.h"
#include "CTile.h"

CMissile::CMissile()
	:m_fTheta(PI/4.f)
	,m_vDir(Vec2(1.f,1.f))
{
	m_vDir.Normalize();
	CreateCollider();

	GetCollider()->SetScale(Vec2(15.f, 15.f));
}

CMissile::~CMissile()
{
}

void CMissile::update()
{

	Vec2 vPos = GetPos();

	vPos.x += 300.f * fDT * m_vDir.x;
	vPos.y += 300.f * fDT * m_vDir.y;

	//vPos.x += 300.f * fDT * cosf(m_fTheta);
	//vPos.y -= 300.f * fDT * sinf(m_fTheta);

	SetPos(vPos);
}

void CMissile::render(HDC _dc)
{

	Vec2 vPos = GetPos();
	Vec2 Scale = GetScale(); 



	component_render(_dc);
}


void CMissile::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();


	if (pOtherObj->GetName() == L"Monster")
	{
		DeleteObject(this);
	}

}
