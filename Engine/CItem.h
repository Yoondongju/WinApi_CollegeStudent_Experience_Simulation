#pragma once
#include "CObject.h"

class CTexture;

class CItem : public CObject
{
protected:
	CTexture* pTex; 
	POINT MousPos;

public:
	virtual void update();
	virtual void render(HDC _dc);


public:
	void SetTexItem(CTexture* _tex) { pTex = _tex; }



public:
	virtual void OnCollisionEnter(CCollider* _pOther);




public:
	CLONE(CItem)

public:
	CItem();
	~CItem();





};

