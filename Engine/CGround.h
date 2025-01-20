#pragma once
#include "CObject.h"



class CGround : public CObject
{
private:



public:
	virtual void start();
	virtual void update();
	

public:
	

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OncollisionExit(CCollider* _pOther);


	CLONE(CGround);

public:
	CGround();
	~CGround();

};

