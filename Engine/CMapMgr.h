#pragma once

class CTexture;
class CCore;

class CMapMgr
{
	SINGLE(CMapMgr);

private:
	HDC			m_hdc;
	HWND		m_hwnd;
	CTexture*	m_backTexture;

	CCore*		m_core; // 메인 코어

public:
	int init(HWND _hwnd, POINT _point);
	void progress();


};

