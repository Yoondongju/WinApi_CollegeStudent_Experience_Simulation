#include "pch.h"
#include "CBtnUI.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CScene_Tool.h"

#include "CAnimator.h"

#include "CEffect.h"

#include "CSound.h"
#include "CResMgr.h"

CBtnUI::CBtnUI()
	: CUI(false)
	, m_pFunc(nullptr)
	, m_param1(0)
	, m_param2(0)
	, m_pSceneInst(nullptr)
	, m_pSceneFunc(nullptr)
	, m_pObjInst(nullptr)
	, m_pObjFunc(nullptr)
{
	BLENDFUNCTION bf = GetBf();
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;

	SetBf(bf);


	


}

CBtnUI::~CBtnUI()
{
	

}

//void CBtnUI::update()
//{
//}
//
//void CBtnUI::finalupdate()
//{
//}


void CBtnUI::render(HDC _dc)
{
	CUI::render(_dc);

}

void CBtnUI::render_CamZoomIn(HDC _dc)
{
	CUI::render_CamZoomIn(_dc);
}

void CBtnUI::MouseOn()
{
	if (nullptr == m_pEffect)
	{
		m_pEffect = new CEffect;


		
		m_pEffect->SetName(L"Effect");
		m_pEffect->SetPos(GetFinalPos());
		m_pEffect->SetScale(GetScale()); // 버튼 크기만큼 이펙트도 동일한 Scale 지정
		m_pEffect->SetOwner(this);
		CreateObject(m_pEffect, GROUP_TYPE::EFFECT);
		m_pEffect->GetAnimator()->Play(L"Effect_UI", true);
	}

	

}


void CBtnUI::MouseOut()
{
	if (nullptr != m_pEffect)
	{
		if (!m_pEffect->IsDead())
		{
			DeleteObject(m_pEffect);
			m_pEffect = nullptr;
		}
		
	}
		
	
	
}


void CBtnUI::MouseLbtnDown()
{
	
}

void CBtnUI::MouseLbtnUp()
{
	if (nullptr != m_pEffect)
	{
		if (!m_pEffect->IsDead())
		{
			DeleteObject(m_pEffect);
			m_pEffect = nullptr;
		}
	}
}


void CBtnUI::MouseLbtnClicked()
{
	if (nullptr != m_pFunc)
	{
		m_pFunc(m_param1, m_param2);
	}

	if (m_pSceneInst && m_pSceneFunc )
	{
		(m_pSceneInst->*m_pSceneFunc)();
	}

	if (m_pObjInst && m_pObjFunc)
	{
		(m_pObjInst->*m_pObjFunc)();
	}

	if (m_pObjInst && m_pObjFunc_Arg && m_pObjarg)
	{
		(m_pObjInst->*m_pObjFunc_Arg)(m_pObjarg);
	}

	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"BUTTON_CLICK_SOUND");
	pNewSound->Play(false);
	pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	pNewSound->SetVolume(30.f); // 소리 볼륨 크기
}



