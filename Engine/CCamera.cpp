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
	

	// SelectGDI gdi(m_pVeilTex->GetDC(), BRUSH_TYPE::WHITE);   << 해결못하는중
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


	// 화면 중앙좌표와 카메라 LookAt 좌표값 계산
	CalDiff();

	
}


void CCamera::render(HDC _dc)
{
	// Player와 Monster가 접촉했을때 카메라 변화체크
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
					// 화장실에선 위치 바꾸면 안될듯
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


	float fRatio = 0.f; // 이펙트 진행 비율
	fRatio = effect.m_fCurTime / effect.m_fDuration;

	int iAlpha = 0;

	if (fRatio < 0.f)	 // 비율 값은 항상 0~1 사이에 나와야함
		fRatio = 0.f;	 // 비율 값은 항상 0~1 사이에 나와야함
	if (fRatio > 1.f)	 // 비율 값은 항상 0~1 사이에 나와야함
		fRatio = 1.f;	 // 비율 값은 항상 0~1 사이에 나와야함

	// 255 불투명  0 투명
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


	// 진행 시간이 이펙트 최대 지정 시간을 넘어선 경우
	if (effect.m_fDuration <= effect.m_fCurTime)
	{
		// 효과종료
		m_listCamEffect.pop_front();
		m_CamEffect_End = true;
	}



	

		
}


void CCamera::CalDiff()
{
	// 이전 LookAt 과 현재 LookAt 차이값 보정 현재의 LookAt구해야함
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
	


	// 이전 LookAt 과 현재 Look의 차이값을 보정해서 현재의 LookAt을 구한다.
	Vec2 vResolution = CCore::GetInst()->GetResolution(); //  현재 윈도우의 해상도를 가져온다
	Vec2  vCenter = vResolution / 2.f; // 현재 해상도에서 크기의 절반 중심을 카메라가 바라보는 Center로 넣어줌

	
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


	
	if (vTargetLT.x + srcWidth >= vClientResolution.x) // x만큼의 출력할 거리가 우리 최대 해상도를 넘은경우
	{
		srcWidth = vClientResolution.x - vTargetLT.x;
	}
	else if (vTargetLT.x <= 0.f)
	{
		vTargetLT.x = 0.f;
	}
	

	if (vTargetLT.y + srcHeight >= vClientResolution.y) // y만큼의 출력할 거리가 우리 최대 해상도를 넘은경우
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



	 // 1. 기본 백버퍼 텍스쳐에 그림을 그림
	 // 2. 백퍼버에 그린 그림을 메인 _dc에 옮김
	 // 3. 난 근데 카메라를 줌인해서 또다른 백백버퍼에 그림을 그리고싶음
	 // 4. 기존 백버퍼에 있는 그림을 백백버퍼에 옮김 (확대해서)
	 // 5. 백백버퍼에 있는 공간에 UI를 추가로 그려줌 근데 이래도 똑같을것임 백버퍼에 UI가 잇으니가
	 // 6. 백백버퍼에만 그려야함 


	 for (size_t i = 0; i < vecUI.size(); ++i)
	 {		 
		 CUI* pUI = (CUI*)vecUI[i];

		if(!pUI->IsDead())
			pUI->render_CamZoomIn(_dc);  // 하 모르겠다.
	 }


	 
}


void CCamera::ZoomInCameraSetPos(HDC _dc, CScene* _pScene)
{

	


}
