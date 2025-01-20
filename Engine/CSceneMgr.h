#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr);
	
private:
	CScene*		m_arrScene[(UINT)SCENE_TYPE::END];  // ¸ðµç ¾À¸ñ·Ï
	CScene*		m_pCurScene;						// ÇöÀç ¾À
	

	float		m_fSceneChangeTime;					// ¾À ³Ñ¾î°¥¶§ °É¸®´Â½Ã°£



	CScene*		m_pMapScene;							// ¸Ê Àü¿ë¾À


public:
	void init();
	void update();

	void subsequent_update();

	
	void render(HDC _dc);

public:
	CScene* GetCurScene() { return m_pCurScene; }

	void SetCurScene(SCENE_TYPE _e) { m_pMapScene = m_arrScene[(UINT)_e]; }
	
private:
	void ChangeScene(SCENE_TYPE _Next);

	friend class CEventMgr;
	friend class CMapMgr;


};

