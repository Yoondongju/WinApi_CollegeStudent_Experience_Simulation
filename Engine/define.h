#pragma once

// 정적 지역변수는 프로세스가 실행될때 메모리에 올라가고 프로세스 꺼져야 없어짐
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

enum class GROUP_TYPE  // 그리는 순서 맨뒤가 가장 마지막에 그려짐 
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
	BLINE_DATE,		// 과팅
	STUDY,			// 공부
	PART_TIME_JOB,	// 알바
	DRINK_ALCOHOL,  // 술마시기
	// .. . .  추가 예정
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
	IDLE,	 // 기본
	PATROL,  // 순찰
	TRACE,   // 추적
	CODEMODE, // CodeMode 걍 내가 짠 코드에 의해 스토리가 진행될수 있게 만든 상태임
	CODEMODE_MT, // 학생회장 부학생회장이 동작하는 AI
	GETDIRECTIONS, // 길 찾기
	ATT,	 // 공격
	RUN,     // 도망가기
	DEAD,    // 사망

	END
};