#pragma once
#include "CObject.h"

enum class DIR
{
	NONE,
	LEFT_TOP,
	LEFT,
	LEFT_BOTTOM,
	RIGHT_TOP,
	RIGHT,
	RIGHT_BOTTOM,

};


class CBird : public CObject
{
private:
	float	m_fSpeed;
	bool	m_barrive;

	DIR		m_Dir;

	Vec2	m_target;


public:
	void SetSpeed(float _f) { m_fSpeed = _f; }
	float GetSpeed() { return m_fSpeed; }
	
public:
	virtual void update();
	void update_state();
	void update_animation();

	virtual void render(HDC _dc);

public:
	void CreateTargetPos();

public:
	CLONE(CBird);

public:
	CBird();
	~CBird();



};

