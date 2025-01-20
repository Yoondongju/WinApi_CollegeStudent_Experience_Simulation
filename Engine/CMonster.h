#pragma once
#include "CObject.h"

class AI;
class CUI;


enum class CUSTOMER_STATE    
{ 
	NONE,
	SAY,
	WALK,
	VOMIT,
	SIT,
	DANCE,
	DEAD,
};


enum class CUSTOMER_DIR
{
	LEFT = -1,
	RIGHT = 1,
};


struct tMonInfo
{
	float			fSpeed;
	float			fHP;
	float			fRecogRange;  // 인지 범위
	float			fAttRange;	// 공격 범위
	float			fAtt;			// 공격력

};
enum class MON_EMOTION
{
	NONE,
	LOVE,
	CRY,
	END
};




class CMonster : public CObject   // 현재는 Customer를 Monster class로 구현하였다, 하지만 추후 다른 Scene에서 Npc가 필요할경우 
								  // Monster를 상속받는 추가 class를 설계할것인지 그대로 이 클래스를 쓸것인지 고려해봐야함
{
private:
	CUSTOMER_STATE	m_eCurState;
	CUSTOMER_STATE	m_ePreState;

	CUSTOMER_DIR	m_Dir;		// 방향  
	CUSTOMER_DIR	m_PreDir;   // 이전 방향

	float			m_waitTime; // 대기시간


	tMonInfo		m_tInfo;
	AI*				m_pAI;


	Vec2			m_TargetPos;
	bool			m_bFindTarget; // 내가 원하는 Target의 인식범위를 찾았는지 ?
	bool			m_bDoingTalk;  // 대화중인지?

	bool			m_ableMultipleMarkCreate; // 두번 이상 대화 마크가 뜨는지?


	CUI*			m_MarkUI;	   // 물음표 UI


	bool			m_Dissappear_End_of_conversation; // true면 사라지는놈, false면 계속 생존하는놈
	bool			m_zoomInMon;

	MON_EMOTION		m_emotion;

	bool			m_bDeleteAI;


public:
	float GetSpeed() { return m_tInfo.fSpeed; }
	void SetSpeed(float _f) { m_tInfo.fSpeed = _f; }

	void SetAI(AI* _AI);
	AI* GetAI() { return m_pAI; }
	const tMonInfo& GetInfo() { return m_tInfo; }

	void SetDir(CUSTOMER_DIR _dir) { m_Dir = _dir; }
	CUSTOMER_DIR GetDir() { return m_Dir; }


	void SetCurState(CUSTOMER_STATE _state) { m_eCurState = _state; }
	CUSTOMER_STATE GetCurState() { return m_eCurState; }

	void SetPreState(CUSTOMER_STATE _state) { m_ePreState = _state; }
	CUSTOMER_STATE GetPreState() { return m_ePreState; }

	float GetWaitTime() { return m_waitTime; }
	void  SetWaitTime(float _f) { m_waitTime = _f; }


	void SetTarGetPos(Vec2 vPos) { m_TargetPos = vPos; }
	Vec2 GetTarGetPos() { return m_TargetPos; }


	void SetMultipleMarkCreate(bool _b) { m_ableMultipleMarkCreate = _b; }
	bool GetMultipleMarkCreate() { return m_ableMultipleMarkCreate; }


	void SetFineTarget(bool _b) { m_bFindTarget = _b; }
	bool GetFineTarget() { return m_bFindTarget; }

	void SetDoingTalk(bool _b) { m_bDoingTalk = _b; }
	bool GetDoingTalk() { return m_bDoingTalk; }

	CUI*& GetMarkUI() { return m_MarkUI; }


public:
	bool IsZoomInMon() { return m_zoomInMon; }
	void SetZoomInMon(bool _b) { m_zoomInMon = _b; }


public:
	void SetDissapear_conversation(bool _b) { m_Dissappear_End_of_conversation = _b; }
	bool IsDissapear_conversation() { return m_Dissappear_End_of_conversation; }



private:
	void SetMonInfo(const tMonInfo& _info)
	{
		m_tInfo = _info;
	}


public:
	void SetEmotion(MON_EMOTION _e) { m_emotion = _e; }


public:
	void IsDeleteAI()
	{
		if (m_bDeleteAI)
		{
			SetAI(nullptr);

			if(GetCurState() == CUSTOMER_STATE::WALK)
				SetCurState(CUSTOMER_STATE::SAY);
		}
	}
	void SetIsDeleteAI(bool _b) { m_bDeleteAI = _b; }


public:
	virtual void OnCollision(CCollider* _pOther) {}
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther) {}


public:
	void CreateMarkUI();


	

public:
	virtual void update();
	void update_animation();
	void update_state();
	void update_move();


	CLONE(CMonster);

public:
	CMonster();
	~CMonster();

	friend class CMonFactory;

};

