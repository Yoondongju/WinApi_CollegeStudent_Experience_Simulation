#include "pch.h"
#include "CCamera.h"

#include "CObject.h"
#include "CCore.h"
#include "CMonster.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CTexture.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CTalkUI.h"

#include "SelectGDI.h"

CCamera::CCamera()
	: m_pTargetObj(nullptr)
	, m_fTime(1.f)
	, m_fSpeed(0.f)
	, m_fAccTime(1.f)
	, m_listCamEffect()
	, m_pVeilTex(nullptr)
	, m_bZoomIn(false)
{
	
}

CCamera::~CCamera()
{

}


void CCamera::init()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	m_pVeilTex = CResMgr::GetInst()->CreateTexture(L"CameraVeil", (UINT)vResolution.x, (UINT)vResolution.y);
	

	// SelectGDI gdi(m_pVeilTex->GetDC(), BRUSH_TYPE::WHITE);   << �ذ���ϴ���
}



void CCamera::update()
{
	//if (m_pTargetObj)
	//{
	//	if (m_pTargetObj->IsDead())
	//	{
	//		m_pTargetObj = nullptr;
	//	}
	//	else
	//	{
	//		m_vLookAt = m_pTargetObj->GetPos();
	//	}
	//}

	 if (KEY_CHECK(KEY::UP, KEY_STATE::HOLD))
	 	m_vLookAt.y -= 500.f * fDT;
	 if (KEY_CHECK(KEY::DOWN, KEY_STATE::HOLD))
	 	m_vLookAt.y += 500.f * fDT;
	 if (KEY_CHECK(KEY::LEFT, KEY_STATE::HOLD))
	 	m_vLookAt.x -= 500.f * fDT;
	 if (KEY_CHECK(KEY::RIGHT, KEY_STATE::HOLD))
	 	m_vLookAt.x += 500.f * fDT;


	// ȭ�� �߾���ǥ�� ī�޶� LookAt ��ǥ�� ���
	CalDiff();

	
}


void CCamera::render(HDC _dc)
{
	// Player�� Monster�� ���������� ī�޶� ��ȭüũ
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& pObjMonster = pCurScene->GetMonsterGroup();


	for (size_t i = 0; i < pObjMonster.size(); ++i)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(pObjMonster[i]);

		if (pMonster->GetFineTarget())
		{
			if (pMonster->GetDoingTalk())
			{
				m_pTargetObj = pMonster;
				if (L"Toilet Scene" != CSceneMgr::GetInst()->GetCurScene()->GetName())
				{
					// ȭ��ǿ��� ��ġ �ٲٸ� �ȵɵ�
					//if (pMonster->GetName() == L"Earnest_friend_woman2")
					//return;

					pG_Player->SetPos(Vec2(m_pTargetObj->GetPos().x - 35.f, m_pTargetObj->GetPos().y));
				}

				if (pMonster->IsZoomInMon())
				{
					ZoomInCameraToTargets(_dc, pCurScene);
					break;
				}
				else
					break;
				
			}
		}
		else
			m_bZoomIn = false;
	}


	


	if (m_listCamEffect.empty())
	{
		m_CamEffect_End = false;
		return;
	}
		

	tCamEffect& effect = m_listCamEffect.front();
	effect.m_fCurTime += fDT;


	float fRatio = 0.f; // ����Ʈ ���� ����
	fRatio = effect.m_fCurTime / effect.m_fDuration;

	int iAlpha = 0;

	if (fRatio < 0.f)	 // ���� ���� �׻� 0~1 ���̿� ���;���
		fRatio = 0.f;	 // ���� ���� �׻� 0~1 ���̿� ���;���
	if (fRatio > 1.f)	 // ���� ���� �׻� 0~1 ���̿� ���;���
		fRatio = 1.f;	 // ���� ���� �׻� 0~1 ���̿� ���;���

	// 255 ������  0 ����
	if (CAM_EFFECT::FADE_OUT == effect.eEfeect)
	{
		iAlpha = (int)(255.f * fRatio);
	}
	else if (CAM_EFFECT::FADE_IN == effect.eEfeect)
	{
		iAlpha = (int)(255.f * (1.f - fRatio));
	}
	

	BLENDFUNCTION bf = {};

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = iAlpha;



	

	AlphaBlend(_dc
		, 0
		, 0
		, (int)m_pVeilTex->Width()
		, (int)m_pVeilTex->Height()
		, m_pVeilTex->GetDC()
		, 0, 0, (int)m_pVeilTex->Width(), (int)m_pVeilTex->Height()
		, bf);


	// ���� �ð��� ����Ʈ �ִ� ���� �ð��� �Ѿ ���
	if (effect.m_fDuration <= effect.m_fCurTime)
	{
		// ȿ������
		m_listCamEffect.pop_front();
		m_CamEffect_End = true;
	}



	

		
}


void CCamera::CalDiff()
{
	// ���� LookAt �� ���� LookAt ���̰� ���� ������ LookAt���ؾ���
	m_fAccTime += fDT;

	if (m_fAccTime >= m_fTime)
	{
		m_vCurLookAt = m_vLookAt;
	}

	else
	{
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;

		if (!vLookDir.IsZero())
		{
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_fSpeed * fDT;
		}


	}
	


	// ���� LookAt �� ���� Look�� ���̰��� �����ؼ� ������ LookAt�� ���Ѵ�.
	Vec2 vResolution = CCore::GetInst()->GetResolution(); //  ���� �������� �ػ󵵸� �����´�
	Vec2  vCenter = vResolution / 2.f; // ���� �ػ󵵿��� ũ���� ���� �߽��� ī�޶� �ٶ󺸴� Center�� �־���

	
	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;

}


void CCamera::ZoomInCameraToTargets(HDC _dc , CScene* _pScene)
{
	m_bZoomIn = true;

	Vec2 vTargetLT = m_pTargetObj->GetPos() - 200.f;
	Vec2 vTargetRB = m_pTargetObj->GetPos() + 200.f;
	Vec2 vClientResolution = CCore::GetInst()->GetResolution();
	
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();
	

	int srcWidth = static_cast<int>(vTargetRB.x - vTargetLT.x);
	int srcHeight = static_cast<int>(vTargetRB.y - vTargetLT.y);


	
	if (vTargetLT.x + srcWidth >= vClientResolution.x) // x��ŭ�� ����� �Ÿ��� �츮 �ִ� �ػ󵵸� �������
	{
		srcWidth = vClientResolution.x - vTargetLT.x;
	}
	else if (vTargetLT.x <= 0.f)
	{
		vTargetLT.x = 0.f;
	}
	

	if (vTargetLT.y + srcHeight >= vClientResolution.y) // y��ŭ�� ����� �Ÿ��� �츮 �ִ� �ػ󵵸� �������
	{
		srcHeight = vClientResolution.y - vTargetLT.y;
	}
	else if (vTargetLT.y <= 0.f)
	{
		vTargetLT.y = 0.f;
	}
	

	SetStretchBltMode(_dc, COLORONCOLOR);
	StretchBlt(_dc, 0, 0, vClientResolution.x, vClientResolution.y,
		_dc, static_cast<int>(vTargetLT.x), static_cast<int>(vTargetLT.y),
		abs(srcWidth), abs(srcHeight), SRCCOPY);



	 // 1. �⺻ ����� �ؽ��Ŀ� �׸��� �׸�
	 // 2. ���۹��� �׸� �׸��� ���� _dc�� �ű�
	 // 3. �� �ٵ� ī�޶� �����ؼ� �Ǵٸ� �����ۿ� �׸��� �׸������
	 // 4. ���� ����ۿ� �ִ� �׸��� �����ۿ� �ű� (Ȯ���ؼ�)
	 // 5. �����ۿ� �ִ� ������ UI�� �߰��� �׷��� �ٵ� �̷��� �Ȱ������� ����ۿ� UI�� �����ϰ�
	 // 6. �����ۿ��� �׷����� 


	 for (size_t i = 0; i < vecUI.size(); ++i)
	 {		 
		 CUI* pUI = (CUI*)vecUI[i];

		if(!pUI->IsDead())
			pUI->render_CamZoomIn(_dc);  // �� �𸣰ڴ�.
	 }


	 
}


void CCamera::ZoomInCameraSetPos(HDC _dc, CScene* _pScene)
{

	


}
