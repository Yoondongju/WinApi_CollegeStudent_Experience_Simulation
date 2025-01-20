#include "pch.h"
#include "CTable.h"

#include "CTexture.h"

#include "CResMgr.h"


CTable::CTable()
	: m_TableTexture(nullptr)
{
	m_TableTexture = CResMgr::GetInst()->LoadTexture(L"Table", L"texture\\Table.png");
}

CTable::~CTable()
{
}



void CTable::update()
{


}

void CTable::render(HDC _dc)
{

	Vec2 vPos = GetPos();//CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();




	AlphaBlend(_dc
		, int(vPos.x - vScale.x / 2.f)
		, int(vPos.y - vScale.y / 2.f)
		, vScale.x    // 출력할 가로크기
		, vScale.y   // 출력할 세로크기
		, m_TableTexture->GetDC()
		, 0
		, 0
		, (int)m_TableTexture->Width()
		, (int)m_TableTexture->Height()
		, m_TableTexture->GetBf());

}

void CTable::OnCollisionEnter(CCollider* _pOther)
{


}


