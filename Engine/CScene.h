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
	vector<CObject*> m_arrObj[(UINT)GROUP_TYPE::END]; // ������Ʈ�� ���� �� ������ ���͸� �׷� ������ŭ ����
	wstring			 m_strName; // Scene�̸�
	
	UINT			 m_iTileX;  // Ÿ�� ���ΰ���
	UINT			 m_iTileY;  // Ÿ�� ���ΰ���

	CObject*		 m_pPlayer;
	CTalkUI*		 m_MainTalkUI;  // ���� ��ȭ UI
	
protected:
	CTexture*		 m_pBackground_Texture; // �� ����̹���
	CGround*		 m_pOutline;			// �� �ƿ����� �浹ü
	POINT			 m_cursorPos;			// ���콺 ��ġ
	CTalkUI*		 m_MonologueTalkUI;		// �����ȭ â



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

