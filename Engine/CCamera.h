#pragma once

class CObject;
class CTexture;
class CScene;

enum class CAM_EFFECT
{
	FADE_IN,  // ���� ������°�
	FADE_OUT, // ���� ��ο����°�
	NONE,
};

struct tCamEffect
{
	CAM_EFFECT  eEfeect;		// ī�޶� ����Ʈȿ��
	float		m_fDuration;	// ī�޶� ȿ�� �ִ� ���� �ð�
	float		m_fCurTime;		// ī�޷� ȿ�� ���� ����� �ð�

};



class CCamera
{
	SINGLE(CCamera);
private:
	Vec2		m_vLookAt;				// ī�޶� �ٶ󺸰��ִ� ��ġ
	Vec2		m_vCurLookAt;			// ������ġ�� ������ġ ���� ��ġ
	Vec2		m_vPrevLookAt;			// ī�޶� ���� ���� ������ ��ġ

	CObject*	m_pTargetObj;			// ī�޶��� Ÿ�� ������Ʈ
	Vec2		m_vDiff;				// �ػ� �߽���ġ��, ī�޶� LookAt ���� ���� ��
	
	float		m_fTime;				// Ÿ���� ���󰡴µ� �ɸ��� �ð� 
	float		m_fSpeed;				// Ÿ���� ���󰡴� �ӵ�
	float		m_fAccTime;				// �����ð�

	CTexture*	m_pVeilTex;			    // ī�޶� ������ (�帷 �ؽ���)



	list<tCamEffect> m_listCamEffect;   // ī�޶� ����Ʈ
	bool		m_CamEffect_End;		// ����Ʈ �� ǥ���ߴ���?


	bool		m_bZoomIn;				// ���� �Ǿ�����


	RECT		m_vLookAtZoomInRange;	// ī�޶� ���� �ϴ� ���� 




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

