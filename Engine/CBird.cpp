#include "pch.h"
#include "CBird.h"

#include "CCore.h"
#include "CTimeMgr.h"

#include  <cstdlib>
#include  <ctime>

#include "CCollider.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include <math.h>

CBird::CBird()
	: m_fSpeed(100.f)
	, m_barrive(false)
{

	BLENDFUNCTION bf = GetBf();

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;

	SetBf(bf);


	srand((unsigned int)time(NULL));

	SetName(L"Bird");


	CreateTargetPos();
	CreateCollider();

	GetCollider()->SetScale(Vec2(15.f, 15.f));

	CreateAnimator();

}






CBird::~CBird()
{

}


void CBird::update()
{
	if (m_barrive)
		CreateTargetPos();
		
	update_state();
	update_animation();

	
}

void CBird::update_state()
{
	Vec2 vCurPos = GetPos();
	
	Vec2 vTarget = m_target;

	Vec2 vDir = m_target - vCurPos; //  방향구함
	vDir.Normalize();   // 방향 정규화


	vCurPos += vDir * m_fSpeed * fDT;

	if (0.f < vDir.x && vDir.y == 0.f) // 우 방향
	{
		m_Dir = DIR::RIGHT;
		if (vCurPos.x >= m_target.x)
		{
			m_barrive = true;
		}
	}

	else if (0.f < vDir.x && 0.f < vDir.y) // 우 하단방향
	{
		m_Dir = DIR::RIGHT_BOTTOM;
		if (vCurPos.x >= m_target.x && vCurPos.y >= m_target.y)
		{
			m_barrive = true;
		}

	}
	else if (0.f < vDir.x && 0.f > vDir.y) // 우 상단방향
	{
		m_Dir = DIR::RIGHT_TOP;
		if (vCurPos.x >= m_target.x && vCurPos.y <= m_target.y)
		{
			m_barrive = true;
		}
	}

	else if (0.f > vDir.x && vDir.y == 0.f) // 좌 방향
	{
		m_Dir = DIR::LEFT;
		if (vCurPos.x <= m_target.x)
		{
			m_barrive = true;
		}
	}

	else if (0.f > vDir.x && 0.f < vDir.y) // 좌 하단방향
	{
		m_Dir = DIR::LEFT_BOTTOM;
		if (vCurPos.x <= m_target.x && vCurPos.y >= m_target.y)
		{
			m_barrive = true;
		}
	}
	else if (0.f > vDir.x && 0.f > vDir.y) // 좌 상단방향
	{
		m_Dir = DIR::LEFT_TOP;
		if (vCurPos.x <= m_target.x && vCurPos.y <= m_target.y)
		{
			m_barrive = true;
		}
	}

	



	SetPos(vCurPos);
}


void CBird::update_animation()
{
	switch (m_Dir)
	{
	case DIR::LEFT_TOP:
		GetAnimator()->Play(L"BIRD_LEFT_TOP", true);
		break;
	case DIR::LEFT:
		GetAnimator()->Play(L"BIRD_LEFT", true);
		break;
	case DIR::LEFT_BOTTOM:
		GetAnimator()->Play(L"BIRD_LEFT_BOTTOM", true);
		break;
	case DIR::RIGHT_TOP:
		GetAnimator()->Play(L"BIRD_RIGHT_TOP", true);
		break;
	case DIR::RIGHT:
		GetAnimator()->Play(L"BIRD_RIGHT", true);
		break;
	case DIR::RIGHT_BOTTOM:
		GetAnimator()->Play(L"BIRD_RIGHT_BOTTOM", true);
		break;

	default:
		break;
	}

}


void CBird::render(HDC _dc)
{

	component_render(_dc);

}



void CBird::CreateTargetPos()
{
	m_barrive = false;
	m_Dir = DIR::NONE;

	Vec2 vClientResolution = CCore::GetInst()->GetResolution();


	m_target.x = rand() % (int)vClientResolution.x;
	m_target.y = rand() % (int)vClientResolution.y;
}
