#pragma once
#include "CObject.h"

class CTexture;
class CUI;

class CTile : public CObject
{
private:
	CTexture*	m_pTileTex;
	int			m_iImgIdx;
	

public:
	void SetTexture(CTexture* _pTex) { m_pTileTex = _pTex; }
	void SetImgIdx(int _idx) { m_iImgIdx = _idx; }


	void AddImgIdx()
	{
		++m_iImgIdx;
	}



public:
	virtual void update();
	virtual void render(HDC _dc);
	virtual CObject* Clone() { return this; }


public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);


public:
	CTile();
	~CTile();


};

