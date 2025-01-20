#pragma once
#include "CItem.h"


class CTexture;

class CBook : public CItem
{
private:
	static UINT num;
	static int m_count;
	CTexture* m_bookTexture;


public:
	virtual void update();
	virtual void render(HDC _dc);

	void CreateBookTexture();  // << 안쓰는 함수

	void CreateStudyUI();
	void CreateQuizUI();


public:
	static int GetBookCheckcount() { return m_count; }


public:
	virtual void OnCollisionEnter(CCollider* _pOther);




public:
	CLONE(CBook)

public:
	CBook();
	~CBook();



};

