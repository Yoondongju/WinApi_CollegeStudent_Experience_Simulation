#include "pch.h"
#include "CPotal.h"

#include "CAnimator.h"

#include "CCollider.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CEffect.h"


CPotal::CPotal()
{
	BLENDFUNCTION bf = GetBf();
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 220;

	SetBf(bf);

	SetName(L"Potal");

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 20.f));
	GetCollider()->SetScale(Vec2(30.f, 30.f));

	CreateAnimator();

	GetAnimator()->Play(L"POTAL", true);



	CEffect* pEffect = new CEffect;
	pEffect->SetName(L"Effect");
	pEffect->SetOwner(this);
	pEffect->SetPos(Vec2(GetPos().x - 20.f, GetPos().y));
	pEffect->SetScale(GetScale()); // 버튼 크기만큼 이펙트도 동일한 Scale 지정
	CreateObject(pEffect, GROUP_TYPE::EFFECT);
	pEffect->GetAnimator()->Play(L"Teleport", true);
}

CPotal::~CPotal()
{

}


void CPotal::render(HDC _dc)
{
	component_render(_dc);
}


void CPotal::OnCollisionEnter(CCollider* _pOther)
{
	CScene* pCurscene = CSceneMgr::GetInst()->GetCurScene();

	if (L"Player" == _pOther->GetObj()->GetName())
	{
		if (L"Toilet Scene" == pCurscene->GetName() || L"Smoking Scene" == pCurscene->GetName())
		{
			ChangeScene(SCENE_TYPE::DRINKING);
		}
		else if (L"Study Scene" == pCurscene->GetName())
		{
			ChangeScene(SCENE_TYPE::HIDDENSTUDY);
		}
		else if (L"MainMT Scene" == pCurscene->GetName())
		{
			ChangeScene(SCENE_TYPE::END_MT);
		}
		else
			ChangeScene(SCENE_TYPE::STAGE_LOBBY);
	}
}
