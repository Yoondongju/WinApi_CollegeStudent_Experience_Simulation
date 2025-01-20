#pragma once
#include "CObject.h"

class CPotal : public CObject
{
private:




public:
	virtual void update() {}
	virtual void render(HDC _dc);



public:
	virtual void OnCollisionEnter(CCollider* _pOther);




public:
	CLONE(CPotal)

public:
	CPotal();
	~CPotal();


};

