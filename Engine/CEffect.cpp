#include "pch.h"
#include "CEffect.h"

#include "CAnimator.h"

#include "CBtnUI.h"


CEffect::CEffect()
	: m_Owner(nullptr)
{
	BLENDFUNCTION bf = GetBf();
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 180;

	SetBf(bf);

	SetName(L"Effect");
	CreateAnimator();

	
}

CEffect::~CEffect()
{

}


void CEffect::update()
{
	if (m_Owner && m_Owner->GetCamAffectedObj())
		SetCamAffectedObj(true);
	else if (m_Owner && !m_Owner->GetCamAffectedObj())
		SetCamAffectedObj(false);


	if (nullptr != m_Owner)
	{
		if (m_Owner->IsDead())
		{
			DeleteObject(this);
			// m_Owner = nullptr;
		}
			
	}

	
	// 어떤 오너냐에 따라 업데이트 되는게 달라야함
	if (L"Player" == m_Owner->GetName())
	{
		CAnimator* d = GetAnimator();


		Vec2 OwnerPos = m_Owner->GetPos();
		

		SetPos(Vec2(OwnerPos.x, OwnerPos.y - 70.f));
		SetScale(Vec2(100.f, 100.f));
		GetAnimator()->Play(L"Star", true);

	}
	else if (L"Potal" == m_Owner->GetName())
	{
		SetPos(Vec2(m_Owner->GetPos() + 18.f));
		SetScale(Vec2(100.f, 100.f));
	}
	
}


void CEffect::render(HDC _dc)
{

	CBtnUI* Owner = dynamic_cast<CBtnUI*>(m_Owner);

	
	if (CCamera::GetInst()->GetZoomIn() && nullptr == Owner)     // 버튼 Effect 만 안그려줄거임
	{
		component_render(_dc);
		return;
	}
	else if (CCamera::GetInst()->GetZoomIn() && nullptr != Owner)
		return;


	component_render(_dc);
}

void CEffect::render_CamZoomIn(HDC _dc)
{
	component_render(_dc);
}
