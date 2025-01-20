#pragma once
#include "CObject.h"

enum class PLAYER_STATE
{
	NONE,
	INCAPACITATION,   // �ൿ �Ұ��� ����  (��Ŀ�н����̼�)
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
	NAME = 0,   // < ������
	GRADE,
	MBTI,
	HAVE_FRIEND_COUNT,
	HAVE_GIRL_FRIEND,



	AMOUNT_KNOWLEDGE,	 	 // ���� 
	AMOUNT_SOCIABILIT,		 // ��ȸ�� 
	AMOUNT_ATTRACTIVENESS,	 // ������
	AMOUNT_PROBLEM_SOLVING,  // ���� �ذ� �ɷ�
	AMOUNT_RESPONSIBILITY,	 // å�Ӱ�
	AMOUNT_LEADERSHIP,		 // ������  //     (�굵 ���� üũ X)
	



	AMOUNT_SELF_GROWTH,		 // �ھ� ���� (Player�� ���� 6������ ������ ���� ������ �ھƼ����� �ϰԵ�)


	ALCOHOL_TOLERANCE, // �� �߸��ô��� �����ô���
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

	PLAYER_DIR		m_Dir;		// ����  
	PLAYER_DIR		m_PreDir;   // ���� ����

	PLAYER_MBTI		m_Mbti;		// Player Mbti

	float			m_fSpeed;

	double			m_PlyerInfo[(UINT)PLAYER_INFO::END]; // ���� Player�� ���� ��ġ


	wstring			m_ePlayerInfo[(UINT)PLAYER_INFO::END];	// UI�� ����ϱ� ���� str
	//float			m_fPlayerInfo[(UINT)PLAYER_INFO::END];	// ���� Playe�� ������ �ִ� ���ڷ� �� ���� (�̰ɰ����� ����Ұ���)

	CUI*			m_pUI;  // �÷��̾� �Ӹ����� ���ִ� ��ȭ UI

	CEffect*		m_starEffect; // ���� �ٷ� ��Ÿ!
	bool			m_bAuto;   // �ڵ�ȭ �̵�������? 
	Vec2			m_vAutoTargetPos;

	bool			m_onlymove; // Auto���� �����̴°Ÿ� üũ�Ұ��ΰ�


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


extern CPlayer* pG_Player; // �۷ι� ����Player  �� �̵����� ��� Player������ �־����

