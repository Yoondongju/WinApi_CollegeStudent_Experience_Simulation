#pragma once

class CObject;
class CTexture;
class CScene;

enum class CAM_EFFECT
{
	FADE_IN,  // 점점 밝아지는거
	FADE_OUT, // 점점 어두워지는거
	NONE,
};

struct tCamEffect
{
	CAM_EFFECT  eEfeect;		// 카메라 이펙트효과
	float		m_fDuration;	// 카메라 효과 최대 진행 시간
	float		m_fCurTime;		// 카메로 효과 현재 진행된 시간

};



class CCamera
{
	SINGLE(CCamera);
private:
	Vec2		m_vLookAt;				// 카메라가 바라보고있는 위치
	Vec2		m_vCurLookAt;			// 이전위치와 현재위치 보정 위치
	Vec2		m_vPrevLookAt;			// 카메라가 보는 이전 프레임 위치

	CObject*	m_pTargetObj;			// 카메라의 타겟 오브젝트
	Vec2		m_vDiff;				// 해상도 중심위치와, 카메라 LookAt 간의 차이 값
	
	float		m_fTime;				// 타겟을 따라가는데 걸리는 시간 
	float		m_fSpeed;				// 타겟을 따라가는 속도
	float		m_fAccTime;				// 누적시간

	CTexture*	m_pVeilTex;			    // 카메라 가림막 (장막 텍스쳐)



	list<tCamEffect> m_listCamEffect;   // 카메라 이펙트
	bool		m_CamEffect_End;		// 이펙트 다 표현했는지?


	bool		m_bZoomIn;				// 줌인 되었는지


	RECT		m_vLookAtZoomInRange;	// 카메라가 줌인 하는 범위 




public:
	CTexture* GetVeilTex() { return m_pVeilTex; }


public:
	void SetLookAt(Vec2	_vLook) 
	{ 
		m_vLookAt = _vLook; 
		float fMoveDist = (m_vLookAt - m_vPrevLookAt).Length();
		m_fSpeed = fMoveDist / m_fTime;
		m_fAccTime = 0.f;

	}

	void SetTarget(CObject* _pTarget) { m_pTargetObj = _pTarget; }
	CObject* GetTarget() { return m_pTargetObj; }

	Vec2 GetLookAt() { return m_vCurLookAt; }
	Vec2 GetRenderPos(Vec2 _vObjPos) { return _vObjPos - m_vDiff; }
	Vec2 GetRealPos(Vec2 _vRenderPos) { return _vRenderPos + m_vDiff; }

	void SetZoomIn(bool _b) { this->m_bZoomIn = _b; }
	bool GetZoomIn() { return this->m_bZoomIn; }


	bool IsCamEffect_End() { return m_CamEffect_End; }

	bool IsAllCamEffect_End()
	{
		if (m_listCamEffect.empty())
			return true;
		else
			return false;
	}


	RECT GetZoomInRange() { return m_vLookAtZoomInRange; }

	void FadeIn(float _fDuration)
	{
		tCamEffect ef = {};
		ef.eEfeect = CAM_EFFECT::FADE_IN;
		ef.m_fDuration = _fDuration;
		ef.m_fCurTime = 0.f;

		m_listCamEffect.push_back(ef);

		if (0.f == _fDuration)
			assert(nullptr);
	}





	void FadeOut(float _fDuration)
	{
		tCamEffect ef = {};
		ef.eEfeect = CAM_EFFECT::FADE_OUT;
		ef.m_fDuration = _fDuration;
		ef.m_fCurTime = 0.f;

		m_listCamEffect.push_back(ef);

		if (0.f == _fDuration)
			assert(nullptr);
	}



public:
	void ZoomInCameraToTargets(HDC _dc , CScene* _pScene);
	void ZoomInCameraSetPos(HDC _dc, CScene* _pScene);


public:
	void init();
	void update();
	void render(HDC _dc);

private:
	void CalDiff();

};

