#pragma once
#include "CItem.h"
class CTable : public CItem
{
private:
	CTexture* m_TableTexture;


public:
	virtual void update();
	virtual void render(HDC _dc);

	void CreateBookTexture();
	void CreateStudyUI();


public:
	virtual void OnCollisionEnter(CCollider* _pOther);




public:
	CLONE(CTable)

public:
	CTable();
	~CTable();



};

