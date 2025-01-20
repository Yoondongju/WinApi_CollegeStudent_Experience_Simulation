#pragma once

class CObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	map<wstring, CAnimation*>	m_mapAnim;		// 모든 Animation
	CAnimation*					m_pCurAnim;		// 현재 사용중인 Animation
	CObject*					m_pOwner;		// Animator 소유 오브젝트
	bool						m_bRepeat;		// 반복재생 여부

public:
	CObject* GetObj() { return m_pOwner; }


public:
	void CreateAnimation(const wstring& _strName , CTexture* _pTex, Vec2 _vLT, Vec2 _vScliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);
	void LoadAnimation(const wstring& _strRelativePath);

	CAnimation* FindAnimation(const wstring& _strName);
	void Play(const wstring& _strName , bool _bRepeat);


	void init_Animation();  // 다른 애니메이션 추가할시 이 함수 내부 코드 주석풀고 다시 컴파일해야함.

	void update();
	void finalupdate();
	void render(HDC _dc);

	CAnimation* GetCurAnimaion() { return m_pCurAnim; }
	

public:
	CAnimator();
	CAnimator(const CAnimator& _origin);
	~CAnimator();


	friend class CObject;
};

