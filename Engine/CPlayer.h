#pragma once
#include "CObject.h"

enum class PLAYER_STATE
{
	NONE,
	INCAPACITATION,   // 행동 불가능 상태  (인커패시테이션)
	IDLE,
	WALK,
	ATTACK,
	JUMP,
	SIT,
	DEAD,
};

enum class PLAYER_ATTACK_STATE
{
	NORMAL_ATT1,
	NORMAL_ATT2,
	NORMAL_ATT3,

	SKILL_ATT1,
};

enum class PLAYER_DIR
{
	FRONT,
	LEFT,
	RIGHT,
	BACK,

};

enum class PLAYER_MBTI
{
	ISTJ,  // 0
	ISTP,
	INFJ,
	INTJ,
	ISFJ,
	ISFP,
	INFP,
	INTP,  // 7
	ESTJ,  // 8 
	ESFP,
	ENFP,
	ENTP,
	ESFJ,
	ESTP,
	ENFJ,
	ENTJ,  // 15

	END,
};


enum class PLAYER_INFO
{
	NAME = 0,   // < 더미임
	GRADE,
	MBTI,
	HAVE_FRIEND_COUNT,
	HAVE_GIRL_FRIEND,



	AMOUNT_KNOWLEDGE,	 	 // 지식 
	AMOUNT_SOCIABILIT,		 // 사회성 
	AMOUNT_ATTRACTIVENESS,	 // 자존감
	AMOUNT_PROBLEM_SOLVING,  // 문제 해결 능력
	AMOUNT_RESPONSIBILITY,	 // 책임감
	AMOUNT_LEADERSHIP,		 // 리더쉽  //     (얘도 더미 체크 X)
	



	AMOUNT_SELF_GROWTH,		 // 자아 성찰 (Player는 위의 6가지의 정보를 토대로 마지막 자아성찰을 하게됨)


	ALCOHOL_TOLERANCE, // 술 잘마시는지 못마시는지
	SMOKING_TOLERANCE,
	FRIEND_SETING,
		

	END,
};






class CTexture;
class CUI;
class CEffect;

class CPlayer : public CObject
{
private:
	PLAYER_STATE	m_eCurState;
	PLAYER_STATE	m_ePreState;

	PLAYER_DIR		m_Dir;		// 방향  
	PLAYER_DIR		m_PreDir;   // 이전 방향

	PLAYER_MBTI		m_Mbti;		// Player Mbti

	float			m_fSpeed;

	double			m_PlyerInfo[(UINT)PLAYER_INFO::END]; // 실제 Player의 정보 수치


	wstring			m_ePlayerInfo[(UINT)PLAYER_INFO::END];	// UI에 출력하기 위한 str
	//float			m_fPlayerInfo[(UINT)PLAYER_INFO::END];	// 실제 Playe가 가지고 있는 숫자로 된 정보 (이걸가지고 계산할것임)

	CUI*			m_pUI;  // 플레이어 머리위에 떠있는 대화 UI

	CEffect*		m_starEffect; // 내가 바로 스타!
	bool			m_bAuto;   // 자동화 이동중인지? 
	Vec2			m_vAutoTargetPos;

	bool			m_onlymove; // Auto에서 움직이는거만 체크할것인가


public:
	virtual void update();
	virtual void render(HDC _dc);

public:
	void SetSpeed(float _f) { m_fSpeed = _f; }
	float GetSpeed() { return m_fSpeed; }

	void SetPreState(PLAYER_STATE _e) { m_ePreState = _e; }
	void SetCurState(PLAYER_STATE _e) { m_eCurState = _e; }


	void SetPlayerInfo(PLAYER_INFO _e, wstring _data) { m_ePlayerInfo[(UINT)_e] = _data; }
	wstring GetPlayerInfo(PLAYER_INFO _e) { return m_ePlayerInfo[(UINT)_e]; }

	wstring* GetTotalPlayerInfo() { return m_ePlayerInfo; }

	void SetspeechBubble();
	CUI* GetspeechUI() { return m_pUI; }

	void SetMbti(PLAYER_MBTI _e) { m_Mbti = _e; }
	PLAYER_MBTI GetMbti() { return m_Mbti; }


public:
	void SetAutoPos(Vec2 _v) { m_vAutoTargetPos = _v; }
	void AutomationPlayer_Action(Vec2 _tagetPos);

public:
	void SetstarEffect(CEffect* _input) { m_starEffect = _input; }

	
public:
	void SetAuto(bool _b) { m_bAuto = _b; }
	bool GetAuto() { return m_bAuto; }


	void SetOnlyMove(bool _b) { m_onlymove = _b; }

	
public:
	void SetPlayer_Info(PLAYER_INFO _e, double _d) { m_PlyerInfo[(UINT)_e] += _d; }
	double* GetPlayer_Info() { return m_PlyerInfo; }
	double GetPlayer_EnumInfo(PLAYER_INFO _e) { return m_PlyerInfo[(UINT)_e]; }

public:
	// virtual CPlayer* Clone() { return new CPlayer(*this); }
	CLONE(CPlayer);

private:
	void CreateMissile();
	void update_state();
	void update_move();
	void update_animation();


public:
	int Final_info_calculate();

public:
	CPlayer(PLAYER_MBTI _mbti);


	~CPlayer();

};


extern CPlayer* pG_Player; // 글로벌 전역Player  씬 이동간에 계속 Player정보가 있어야함

