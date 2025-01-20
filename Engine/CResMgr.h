#pragma once

class CRes;
class CTexture;
class CSound;

class CResMgr
{
	SINGLE(CResMgr);

private:
	map<wstring, CRes*> m_mapTex;
	map<wstring, CRes*> m_mapSound;



public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);  // 원하는 png 사진을 불러오기 용
	CTexture* CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight);    // 백버퍼 텍스쳐 용 텍스쳐 만들기 용
	CTexture* FindTexture(const wstring& _strKey);



	CSound* LoadSound(const wstring& _strKey, const wstring& _strRelativePath);
	CSound* FindSound(const wstring& _strKey);
};

