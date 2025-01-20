#pragma once

// ���� ���������� ���μ����� ����ɶ� �޸𸮿� �ö󰡰� ���μ��� ������ ������
#define SINGLE(type) public:\
					 static type* GetInst()\
					 {\
						static type mgr;\
						return &mgr;\
					 }\
					private:\
						type();\
						~type();

#define fDT CTimeMgr::GetInst()->GetfDT()
#define DT CTimeMgr::GetInst()->GetDT()

#define CLONE(type) virtual type* Clone() { return new type(*this); }



#define KEY_CHECK(key,state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define MOUSE_POS CKeyMgr::GetInst()->GetMousePos()


#define PI 3.141592

#define TILE_SIZE 64

enum class GROUP_TYPE  // �׸��� ���� �ǵڰ� ���� �������� �׷��� 
{
	DEFAULT,
	TILE,
	GROUND,
	ITEM,
	BIRD,
	MONSTER,
	PLAYER,
	
	PROJ_PLAYER,
	PROJ_MONSTER,


	POTAL = 31,
	EFFECT = 32,
	UI = 33,
	END,
};


enum class SCENE_TYPE
{
	LODING,
	LODING_2,
	STAGE_LOBBY,

	MT,
	MAINMT,
	SEA_MT,
	END_MT,


	DATE,
	MAINDATE,

	STUDY,
	HIDDENSTUDY,

	JOB,

	DRINKING,
	SMOKING,
	TOILET,


	START,
	TOOL,
	MAP_TOOL,

	ENDDING,
	END,
};


enum class CONTENT_TYPE
{
	MT,				// MT
	BLINE_DATE,		// ����
	STUDY,			// ����
	PART_TIME_JOB,	// �˹�
	DRINK_ALCOHOL,  // �����ñ�
	// .. . .  �߰� ����
	END,
};


enum class BRUSH_TYPE
{
	HOLLOW,
	BLACK,
	WHITE,
	END
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BULE,
	END
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	WAITING_TIME,
	CHANCE_AI_STATE,



	END,
};



enum class MON_STATE
{
	IDLE,	 // �⺻
	PATROL,  // ����
	TRACE,   // ����
	CODEMODE, // CodeMode �� ���� § �ڵ忡 ���� ���丮�� ����ɼ� �ְ� ���� ������
	CODEMODE_MT, // �л�ȸ�� ���л�ȸ���� �����ϴ� AI
	GETDIRECTIONS, // �� ã��
	ATT,	 // ����
	RUN,     // ��������
	DEAD,    // ���

	END
};