#pragma once

// 프레임 동기화를 위해 필요함
// 키 입력 이벤트 처리
// tap, hold, away

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,


	A,
	S,
	D,
	F,
	G,
	Z,
	X,
	C,
	V,
	B,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	LBTN,
	RBTN,


	LAST,
};




enum class KEY_STATE 
{
	NONE, // 눌리지않고 이전에도 안눌림
	TAP,  // 막 누른시점
	HOLD, // 누르고 있는
	AWAY, // 막 뗀 시점
	
};

struct tKeyInfo
{
	KEY_STATE	eState;  // 키의 상태값
	bool		bPrev;   // 이전 프레임에서 눌렸는지 안눌렸는지 
};


class CKeyMgr
{
	SINGLE(CKeyMgr);

private:
	vector<tKeyInfo>	m_vecKey;
	Vec2				m_vCurMousePos;

	
public:
	void init();
	void update();

public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	Vec2	GetMousePos() { return m_vCurMousePos; }

};

