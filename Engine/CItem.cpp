#include "pch.h"
#include "CItem.h"

#include "CAnimator.h"

#include "CCollider.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CEffect.h"

#include "CTexture.h"

#include "CCore.h"
#include "CCamera.h"

#include "CTimeMgr.h"

CItem::CItem()
{
	BLENDFUNCTION bf = GetBf();
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;

	SetBf(bf);

	SetName(L"Item");

	// CreateCollider();
	// GetCollider()->SetOffsetPos(Vec2(0.f, 20.f));
	// GetCollider()->SetScale(Vec2(30.f, 30.f));

	// CreateAnimator();
	// GetAnimator()->Play(L"POTAL", true);

	// CEffect* pEffect = new CEffect;
	// pEffect->SetName(L"Effect");
	// pEffect->SetOwner(this);
	// pEffect->SetPos(GetPos());
	// pEffect->SetScale(GetScale()); // 버튼 크기만큼 이펙트도 동일한 Scale 지정
	// CreateObject(pEffect, GROUP_TYPE::EFFECT);
	// pEffect->GetAnimator()->Play(L"Teleport", true);
}

CItem::~CItem()
{

}


void CItem::update()
{
	if (L"Bus" == GetName() || L"Taxi" == GetName())
	{
		static bool bCheck = false;
		Vec2 vPos = GetPos();

		if (CSceneMgr::GetInst()->GetCurScene()->GetName() == L"MT Scene")
		{
			if (bCheck)
			{

				static bool a = false;

				if (!a)
				{
					bCheck = false;
					a = true;
				}
				
			}
			
		}


		if (200.f > abs(pG_Player->GetPos().x - vPos.x))
		{
			if (!bCheck)
			{
				pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);
				pG_Player->SetMyConversationID(1);
				pG_Player->SetspeechBubble();
				bCheck = true;
				return;
			}
		}

		if (900.f > vPos.x)
			return;

		vPos.x -= 80 * fDT;
		SetPos(vPos);
	}

}

void CItem::render(HDC _dc)
{
	if (!GetAnimator())
	{
		Vec2 vPos = GetPos();
		Vec2 vScale = GetScale();

		AlphaBlend(_dc
			, int(vPos.x - vScale.x / 2.f)
			, int(vPos.y - vScale.y / 2.f)
			, vScale.x    // 출력할 가로크기
			, vScale.y   // 출력할 세로크기
			, pTex->GetDC()
			, 0
			, 0
			, (int)pTex->Width()
			, (int)pTex->Height()
			, pTex->GetBf());
	}
	else
		component_render(_dc);
}


void CItem::OnCollisionEnter(CCollider* _pOther)
{

}


