#pragma once
#include "CRes.h"

class CTexture : public CRes
{
private:
	HDC			m_dc;
	HBITMAP		m_hBit; 
	BITMAP tInfo = {};
	BLENDFUNCTION	 m_bf;
	

public:
	void Load(const wstring& _strFilePath);
	void Create(UINT _iWidth, UINT _iHeight);

	UINT Width() { return tInfo.bmWidth; }
	UINT Height() { return tInfo.bmHeight; }

	HDC GetDC() { return m_dc; }




public:
	void SetBf(BLENDFUNCTION _bf) { m_bf = _bf; }
	BLENDFUNCTION GetBf() { return m_bf; }



private:
	CTexture();
	virtual ~CTexture();

	friend class CResMgr;

};

