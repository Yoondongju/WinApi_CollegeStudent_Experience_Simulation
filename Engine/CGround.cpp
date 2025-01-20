#include "pch.h"
#include "CGround.h"

#include "CCollider.h"
#include "CRigidBody.h"

CGround::CGround()
{
	CreateCollider();
	SetName(L"Ground");
	
}

CGround::~CGround()
{

}


void CGround::start()
{

	
}


void CGround::update()
{
	

}


void CGround::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Player" || pOtherObj->GetName() == L"Customer1"
		|| pOtherObj->GetName() == L"Customer2" || pOtherObj->GetName() == L"Customer3")
	{
		// DT에 의해 Player가 바닥 충돌체와 조금더 파고 들었을 경우 Player의 위치를 바닥에 딱 붙혀서 올려 놔야하는 작업 추가 필요

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		float fLen_y = vObjScale.y / 2.f + vScale.y / 2.f - abs(vObjPos.y - vPos.y);
		float fLen_x = vObjScale.x / 2.f + vScale.x / 2.f - abs(vObjPos.x - vPos.x);

		float pOther_y = pOtherObj->GetPos().y;
		float pOther_x = pOtherObj->GetPos().x;



		if (vObjPos.y <= vPos.y && fLen_y < 10.f)
		{
			pOther_y = pOtherObj->GetPos().y - fLen_y; // Obj가 위에서 아래 물체에 충돌할때 위로올려줌		
		}
		else if (vObjPos.y >= vPos.y && fLen_y < 10.f)
		{
			pOther_y = pOtherObj->GetPos().y + fLen_y; // Obj가 아래에서 위 물체에 충돌할때 아래로내려줌			
		}
		

		
		if (vObjPos.x <= vPos.x && fLen_x < 10.f)
		{
			pOther_x = pOtherObj->GetPos().x - fLen_x; // Obj가 아래에서 위 물체에 충돌할때 아래로내려줌
			
		}
		else if (vObjPos.x >= vPos.x && fLen_x < 10.f)
		{
			pOther_x = pOtherObj->GetPos().x + fLen_x; // Obj가 아래에서 위 물체에 충돌할때 아래로내려줌
			
		}

		// 충돌후 위치 다시 셋팅
		pOtherObj->SetPos(Vec2(pOther_x, pOther_y));
	

	}

}


void CGround::OnCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Player" || pOtherObj->GetName() == L"Customer1"
		|| pOtherObj->GetName() == L"Customer2" || pOtherObj->GetName() == L"Customer3")
	{
		// DT에 의해 Player가 바닥 충돌체와 조금더 파고 들었을 경우 Player의 위치를 바닥에 딱 붙혀서 올려 놔야하는 작업 추가 필요

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		float fLen_y = vObjScale.y / 2.f + vScale.y / 2.f - abs(vObjPos.y - vPos.y);
		float fLen_x = vObjScale.x / 2.f + vScale.x / 2.f - abs(vObjPos.x - vPos.x);

		float pOther_y = pOtherObj->GetPos().y;
		float pOther_x = pOtherObj->GetPos().x;

		

		if (vObjPos.y <= vPos.y && fLen_y < 10.f)
		{
			pOther_y = pOtherObj->GetPos().y - fLen_y; // Obj가 위에서 아래 물체에 충돌할때 위로올려줌		
		}
		else if (vObjPos.y >= vPos.y && fLen_y < 10.f)
		{
			pOther_y = pOtherObj->GetPos().y + fLen_y; // Obj가 아래에서 위 물체에 충돌할때 아래로내려줌			
		}



		if (vObjPos.x <= vPos.x && fLen_x < 10.f)
		{
			pOther_x = pOtherObj->GetPos().x - fLen_x; // Obj가 아래에서 위 물체에 충돌할때 아래로내려줌

		}
		else if (vObjPos.x >= vPos.x && fLen_x < 10.f)
		{
			pOther_x = pOtherObj->GetPos().x + fLen_x; // Obj가 아래에서 위 물체에 충돌할때 아래로내려줌

		}


		// 충돌후 위치 다시 셋팅
		pOtherObj->SetPos(Vec2(pOther_x, pOther_y));

	}


}

void CGround::OncollisionExit(CCollider* _pOther)
{


}

