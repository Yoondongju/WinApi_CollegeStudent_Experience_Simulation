#pragma once

#include "pch.h"

class CObject;
class CGround;
class CTexture;
class CUI;
class CPotal;
class CTalkUI;
class CMonster;


class CScene
{
private:
	vector<CObject*> m_arrObj[(UINT)GROUP_TYPE::END]; // 오브젝트를 저장 및 관리할 백터를 그룹 개수만큼 선언
	wstring			 m_strName; // Scene이름
	
	UINT			 m_iTileX;  // 타일 가로갯수
	UINT			 m_iTileY;  // 타일 세로갯수

	CObject*		 m_pPlayer;
	CTalkUI*		 m_MainTalkUI;  // 메인 대화 UI
	
protected:
	CTexture*		 m_pBackground_Texture; // 씬 배경이미지
	CGround*		 m_pOutline;			// 씬 아웃라인 충돌체
	POINT			 m_cursorPos;			// 마우스 위치
	CTalkUI*		 m_MonologueTalkUI;		// 독백대화 창



public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

	CObject* GetPlayer() { return m_pPlayer; }

	void SetMainTalkUI(CTalkUI* _talkUI) { m_MainTalkUI = _talkUI; }
	CTalkUI* GetMainTalkUI() { return m_MainTalkUI; }
	
	CTexture* GetTexture() { return m_pBackground_Texture; }

public:
	


public:
	virtual void start();
	virtual void update();
	virtual void finalupdate();

	virtual void subsequent_update();

	virtual void render(HDC _dc);

	void render_tile(HDC _dc);

	virtual void Enter() = 0;
	virtual void Exit() = 0;

public:

	void AddObject(CObject* _pObj, GROUP_TYPE _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}


	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }


	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType)
	{
		return m_arrObj[(UINT)_eType];
	}

	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();

	void CreateTile();
	void LoadTile(const wstring& _strRelativePath);

	vector<CObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }
	vector<CObject*>& GetMonsterGroup() { return m_arrObj[(UINT)GROUP_TYPE::MONSTER]; }


public:
	CScene();
	virtual ~CScene();

};

