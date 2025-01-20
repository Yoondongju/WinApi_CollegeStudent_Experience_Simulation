#pragma once
#include "CScene_Date.h"
class Scene_MainDate : public CScene_Date
{
public:
	bool	m_bQuestSuces;
	static bool	m_bRestart;

public:
	// virtual void start();
	virtual void update();
	// virtual void finalupdate();
	virtual void render(HDC _dc);



	virtual void Enter();
	virtual void Exit();


public:
	void CreateTalkUI(DWORD_PTR _pa, DWORD_PTR _pa2);
	void CreateNoteUI(DWORD_PTR _pa, DWORD_PTR _pa2);

public:
	void CreateHiddenQuest();


public:
	Scene_MainDate();
	~Scene_MainDate();

};

