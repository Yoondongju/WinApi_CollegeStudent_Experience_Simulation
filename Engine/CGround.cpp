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
		// DT�� ���� Player�� �ٴ� �浹ü�� ���ݴ� �İ� ����� ��� Player�� ��ġ�� �ٴڿ� �� ������ �÷� �����ϴ� �۾� �߰� �ʿ�

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
			pOther_y = pOtherObj->GetPos().y - fLen_y; // Obj�� ������ �Ʒ� ��ü�� �浹�Ҷ� ���ο÷���		
		}
		else if (vObjPos.y >= vPos.y && fLen_y < 10.f)
		{
			pOther_y = pOtherObj->GetPos().y + fLen_y; // Obj�� �Ʒ����� �� ��ü�� �浹�Ҷ� �Ʒ��γ�����			
		}
		

		
		if (vObjPos.x <= vPos.x && fLen_x < 10.f)
		{
			pOther_x = pOtherObj->GetPos().x - fLen_x; // Obj�� �Ʒ����� �� ��ü�� �浹�Ҷ� �Ʒ��γ�����
			
		}
		else if (vObjPos.x >= vPos.x && fLen_x < 10.f)
		{
			pOther_x = pOtherObj->GetPos().x + fLen_x; // Obj�� �Ʒ����� �� ��ü�� �浹�Ҷ� �Ʒ��γ�����
			
		}

		// �浹�� ��ġ �ٽ� ����
		pOtherObj->SetPos(Vec2(pOther_x, pOther_y));
	

	}

}


void CGround::OnCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Player" || pOtherObj->GetName() == L"Customer1"
		|| pOtherObj->GetName() == L"Customer2" || pOtherObj->GetName() == L"Customer3")
	{
		// DT�� ���� Player�� �ٴ� �浹ü�� ���ݴ� �İ� ����� ��� Player�� ��ġ�� �ٴڿ� �� ������ �÷� �����ϴ� �۾� �߰� �ʿ�

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
			pOther_y = pOtherObj->GetPos().y - fLen_y; // Obj�� ������ �Ʒ� ��ü�� �浹�Ҷ� ���ο÷���		
		}
		else if (vObjPos.y >= vPos.y && fLen_y < 10.f)
		{
			pOther_y = pOtherObj->GetPos().y + fLen_y; // Obj�� �Ʒ����� �� ��ü�� �浹�Ҷ� �Ʒ��γ�����			
		}



		if (vObjPos.x <= vPos.x && fLen_x < 10.f)
		{
			pOther_x = pOtherObj->GetPos().x - fLen_x; // Obj�� �Ʒ����� �� ��ü�� �浹�Ҷ� �Ʒ��γ�����

		}
		else if (vObjPos.x >= vPos.x && fLen_x < 10.f)
		{
			pOther_x = pOtherObj->GetPos().x + fLen_x; // Obj�� �Ʒ����� �� ��ü�� �浹�Ҷ� �Ʒ��γ�����

		}


		// �浹�� ��ġ �ٽ� ����
		pOtherObj->SetPos(Vec2(pOther_x, pOther_y));

	}


}

void CGround::OncollisionExit(CCollider* _pOther)
{


}

