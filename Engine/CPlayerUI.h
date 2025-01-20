#pragma once
#include "CUI.h"
class CPlayerUI : public CUI
{

private:
	Vec2		m_vOffset;    // 부모 UI의 오프셋




public:
	void SetOffset(Vec2 _v) { m_vOffset = _v; }
	Vec2 GetOffset() { return m_vOffset; }


public:
	virtual void render_CamZoomIn(HDC _dc);


public:
	virtual void MouseOn();

	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();

public:
	CLONE(CPlayerUI)

public:
	CPlayerUI();
	~CPlayerUI();


};

