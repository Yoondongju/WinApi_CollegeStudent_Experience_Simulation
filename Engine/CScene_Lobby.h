#pragma once
#include "CScene.h"

class CScene_Lobby : public CScene
{
private:
	static bool	m_bCheckMT;			// MT 
	static bool	m_bCheckDate;		// 소개팅
	static bool	m_bCheckStudy;		// 공부
	static bool	m_bCheckJob;		// 알바
	static bool	m_bCheckDrinking;	// 술자리

	CMonster*	 m_mysteriboy;


public:
	virtual void update();


	virtual void render(HDC _dc);


	virtual void Enter();
	virtual void Exit();


public:
	static void SetCheckMT(bool _b) { m_bCheckMT = _b; }
	static void SetCheckDate(bool _b) { m_bCheckDate = _b; }
	static void SetCheckStudy(bool _b) { m_bCheckStudy = _b; }
	static void SetCheckJob(bool _b) { m_bCheckJob = _b; }
	static void SetCheckDrinking(bool _b) { m_bCheckDrinking = _b; }
	 
	static bool GetCheckMT() { return m_bCheckMT; }
	static bool GetCheckDate() { return m_bCheckDate; }
	static bool GetCheckStudy() { return m_bCheckStudy; }
	static bool GetCheckJob() { return m_bCheckJob; }
	static bool GetCheckDrinking() { return m_bCheckDrinking; }



	


public:
	


public:
	CScene_Lobby();
	~CScene_Lobby();


};

