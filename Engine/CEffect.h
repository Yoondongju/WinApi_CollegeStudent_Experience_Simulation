#pragma once
#include "CObject.h"

class CObject;

class CEffect : public CObject
{
private:
	CObject*	m_Owner;


public:
	virtual void update();
	

	virtual void render(HDC _dc);
	virtual void render_CamZoomIn(HDC _dc);


public:
	void SetOwner(CObject* _pOwner) { m_Owner = _pOwner; }
	CObject* GetOwner() { m_Owner; }




public:
	CLONE(CEffect)

public:
	CEffect();
	~CEffect();


	friend class CBtnUI;
};

