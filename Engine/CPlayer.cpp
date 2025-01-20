#include "pch.h"
#include "CPlayer.h"

#include "CSceneMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CMissile.h"
#include "CScene.h"


#include "CTexture.h"
#include "CResMgr.h"
#include "CCollider.h"

#include "CAnimator.h"
#include "CAnimation.h"

#include "CRigidBody.h"

#include "CUI.h"
#include "CTalkUI.h"

#include "CEffect.h"

CPlayer* pG_Player = nullptr; // 전역 변수를 정의합니다.


int CPlayer::Final_info_calculate()
{
	// 사회성, 자존감, 문제해결능력, 책임감 위 4가지 능력치가 메인 능력치가 될것임
	// 각 능력치의 MAX 능력치는 10임 
	 
	double dToalInfo = 0.f;
	double mainInfo = 0.f;
	UINT temp = 0;
	wstring strMainInfo;

	dToalInfo += m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE];			 // 지식
	dToalInfo += m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT];			 // 사회성
	dToalInfo += m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS];		 // 자존감
	dToalInfo += m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING];	 // 문제해결능력
	dToalInfo += m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY];		 // 책임감



	double avg = dToalInfo / 5.f;		   // avg는 내가 player해서 얻은 수치의 평균
	double maxAvg = (50 / 5.f) / 2.f;	   // 게임 플레이의 수치중 상하를 나눌 평균치 
	

	// 먼저 메인 능력치를 골라야함
	for (int i=6; i<=9; ++i)
	{	
		if (mainInfo < m_PlyerInfo[i])
		{
			mainInfo = m_PlyerInfo[i];
			temp = i;
			// 이때 i의 값으로 어떤 능력치인지 판별 i가 5가 나오면안됨 (지식은 빼기로 햇으니깡)	
		}
		
	}

	
	if (avg > maxAvg)		 // 평균 이상
	{
		mainInfo; // main 능력치 뽑아냈음

		switch (temp)
		{
		
		case 6:
		{
			strMainInfo = L"사회성";
			return (UINT)PLAYER_INFO::AMOUNT_SOCIABILIT;
		}
		break;

		case 7:
		{
			strMainInfo = L"자존감";
			return (UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS;
		}

		case 8:
		{
			strMainInfo = L"문제해결능력";
			return (UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING;
		}
		break;

		case 9:
		{
			strMainInfo = L"책임감";
			return (UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY;
		}
		break;



		default:
			break;
		}
	}
	else if (avg < maxAvg)	 // 평균 이하
	{
		mainInfo; // main 능력치 뽑아냈음
		

		switch (temp)
		{

		case 6:
		{
			strMainInfo = L"사회성";
			(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT;
			return -6;
		}
		break;

		case 7:
		{
			strMainInfo = L"자존감";
			return -7;
		}

		case 8:
		{
			strMainInfo = L"문제해결능력";
			return -8;
		}
		break;

		case 9:
		{
			strMainInfo = L"책임감";
			return -9;
		}
		break;



		default:
			break;
		}
	}

}


CPlayer::CPlayer(PLAYER_MBTI _mbti)
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePreState(PLAYER_STATE::NONE)
	, m_Dir()
	, m_PreDir()
	, m_fSpeed(210.f)
	, m_pUI(nullptr)
	, m_bAuto(false)
	, m_onlymove(false)
{
	BLENDFUNCTION bf = GetBf();

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;


	SetName(L"Player");
	SetBf(bf);

	CreateCollider();
	CreateAnimator();


	GetCollider()->SetOffsetPos(Vec2(0.f, 10.f));
	GetCollider()->SetScale(Vec2(30.f, 60.f));

	

	GetAnimator()->Play(L"IDLE_LEFT",true);

	SetCamAffectedObj(false);


	//	NAME,
	// 	GRADE,
	// 	HAVE_FRIEND_COUNT,
	// 	HAVE_GIRL_FRIEND,
	// 
	// 
	//  컨텐츠를 즐기고 단순히 정보를 오르고 내리고만하지말고
	//  대화창같은걸로 내가 느낌점을 UI로 보여주면서 이유를 설명을해주고 그다음에 올림
	//
	// 
	// 	AMOUNT_KNOWLEDGE,	 	 // 지식 
	// 	AMOUNT_SOCIABILIT,		 // 사회성, 사교성 
	// 	AMOUNT_ATTRACTIVENESS,	 // 매력도
	// 	AMOUNT_PROBLEM_SOLVING,  // 문제 해결 능력
	// 	AMOUNT_LEADERSHIP,		 // 리더쉽
	// 	AMOUNT_RESPONSIBILITY,	 // 책임감   
	// 
	// 
	// 
	// 
	// 	AMOUNT_SELF_GROWTH,		 // 자아 성찰 (Player는 위의 6가지의 정보를 토대로 마지막 자아성찰을 하게됨)
	// 
	// 	END,
	 

	

	m_ePlayerInfo[(UINT)PLAYER_INFO::NAME] = L"이름: 김한신";
	m_ePlayerInfo[(UINT)PLAYER_INFO::GRADE] = L"학년: 1학년";

	wstring strMbti = EnumToString(m_Mbti);
	
	m_ePlayerInfo[(UINT)PLAYER_INFO::MBTI] = L"MBTI: "+ strMbti;

	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE] = L"지식: ";
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT] = L"사회성: ";
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS] = L"자존감: ";
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING] = L"문제해결능력: ";
	//m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_LEADERSHIP] = L"리더쉽: ";
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY] = L"책임감: ";

	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_SELF_GROWTH] = L"자아성찰: ???";


	// Panel ui에 나오는 컨텐츠 str를 누가 관리해야할까


	
	



	// mbti에 따라 초기능력치 줘야함
	switch (_mbti)
	{
	case PLAYER_MBTI::ISTJ:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING] = 1.5;
	}
		break;
	case PLAYER_MBTI::ISTP:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY] = 1.5;
	}
		break;
	case PLAYER_MBTI::INFJ:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE] = 1.5;
	}
		break;
	case PLAYER_MBTI::INTJ:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING] = 1.5;
	}
		break;
	case PLAYER_MBTI::ISFJ:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING] = 1.5;
	}
		break;
	case PLAYER_MBTI::ISFP:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY] = 1.5;
	}
		break;
	case PLAYER_MBTI::INFP:
	{
		// 인프피는 아무 능력치 안줌;
	}
		break;
	case PLAYER_MBTI::INTP:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS] = 1.5;
	}
		break;
	case PLAYER_MBTI::ESTJ:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING] = 1.5;
	}
		break;
	case PLAYER_MBTI::ESFP:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT] = 1.5;
	}
		break;
	case PLAYER_MBTI::ENFP:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS] = 1.5;
	}
		break;
	case PLAYER_MBTI::ENTP:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS] = 1.5;
	}
		break;
	case PLAYER_MBTI::ESFJ:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY] = 1.5;
	}
		break;
	case PLAYER_MBTI::ESTP:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY] = 1.5;
	}
		break;
	case PLAYER_MBTI::ENFJ:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY] = 1.5;
	}
		break;
	case PLAYER_MBTI::ENTJ:
	{
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE] = 1.5;
		m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS] = 1.5;
	}
		break;
	
	default:
		break;
	}

	

	

	// 강체주기
	//CreateRigidBody();

}


CPlayer::~CPlayer()
{
	

}



void CPlayer::update()
{
	if (nullptr == m_starEffect)
	{
		CEffect* starEffect = new CEffect;
		starEffect->SetName(L"Effect");
		starEffect->SetOwner(this);
		m_starEffect = starEffect;
		CreateObject(starEffect, GROUP_TYPE::EFFECT);		
	}


	if (PLAYER_STATE::INCAPACITATION == m_eCurState)
	{	
		static bool dir = true; // true == letf   //  false == right

		if (m_ePreState == m_eCurState && m_PreDir == m_Dir)
			return;

		if (m_ePreState == PLAYER_STATE::WALK)
			m_ePreState = PLAYER_STATE::IDLE;

		switch (m_ePreState)
		{
		case PLAYER_STATE::IDLE:
		{
			if (m_Dir == PLAYER_DIR::LEFT)
				GetAnimator()->Play(L"IDLE_LEFT", true);
			else if (m_Dir == PLAYER_DIR::RIGHT)
				GetAnimator()->Play(L"IDLE_RIGHT", true);
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::LEFT)
				GetAnimator()->Play(L"IDLE_LEFT", true);
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::RIGHT)
				GetAnimator()->Play(L"IDLE_RIGHT", true);
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::FRONT && dir == true)
				GetAnimator()->Play(L"IDLE_LEFT", true);
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::FRONT && dir == false)
				GetAnimator()->Play(L"IDLE_RIGHT", true);
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::BACK && dir == true)
				GetAnimator()->Play(L"IDLE_LEFT", true);
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::BACK && dir == false)
				GetAnimator()->Play(L"IDLE_RIGHT", true);

			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::LEFT)
				GetAnimator()->Play(L"IDLE_LEFT", true);
			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::RIGHT)
				GetAnimator()->Play(L"IDLE_RIGHT", true);
			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::FRONT && dir == true)
				GetAnimator()->Play(L"IDLE_LEFT", true);
			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::FRONT && dir == false)
				GetAnimator()->Play(L"IDLE_RIGHT", true);
			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::BACK && dir == true)
				GetAnimator()->Play(L"IDLE_LEFT", true);
			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::BACK && dir == false)
				GetAnimator()->Play(L"IDLE_RIGHT", true);


		}
		break;

		case PLAYER_STATE::WALK:
		{
			if (m_Dir == PLAYER_DIR::LEFT)
			{
				GetAnimator()->Play(L"WALK_LEFT", true);
				dir = true;
			}
			else if (m_Dir == PLAYER_DIR::RIGHT)
			{
				GetAnimator()->Play(L"WALK_RIGHT", true);
				dir = false;
			}
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::LEFT)
			{
				GetAnimator()->Play(L"WALK_LEFT", true);
				dir = true;
			}
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::RIGHT)
			{
				GetAnimator()->Play(L"WALK_RIGHT", true);
				dir = false;
			}
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::FRONT && dir == true)
			{
				GetAnimator()->Play(L"WALK_LEFT", true);
				dir = true;
			}
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::FRONT && dir == false)
			{
				GetAnimator()->Play(L"WALK_RIGHT", true);
				dir = false;
			}
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::BACK && dir == true)
			{
				GetAnimator()->Play(L"WALK_LEFT", true);
				dir = true;
			}
			else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::BACK && dir == false)
			{
				GetAnimator()->Play(L"WALK_RIGHT", true);
				dir = false;
			}
			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::LEFT)
			{
				GetAnimator()->Play(L"WALK_LEFT", true);
				dir = true;
			}
			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::RIGHT)
			{
				GetAnimator()->Play(L"WALK_RIGHT", true);
				dir = false;
			}
			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::FRONT && dir == true)
			{
				GetAnimator()->Play(L"WALK_LEFT", true);
				dir = true;
			}
			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::FRONT && dir == false)
			{
				GetAnimator()->Play(L"WALK_RIGHT", true);
				dir = false;
			}
			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::BACK && dir == true)
			{
				GetAnimator()->Play(L"WALK_LEFT", true);
				dir = true;
			}
			else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::BACK && dir == false)
			{
				GetAnimator()->Play(L"WALK_RIGHT", true);
				dir = false;
			}


		}
		break;

		case PLAYER_STATE::INCAPACITATION:
		{

		}
		break;

		case PLAYER_STATE::ATTACK:

			break;

		case PLAYER_STATE::SIT:
		{
			if (m_Dir == PLAYER_DIR::LEFT)
			{
				GetAnimator()->Play(L"SIT_LEFT", true);
				dir = true;
			}
			else if (m_Dir == PLAYER_DIR::RIGHT)
			{
				GetAnimator()->Play(L"SIT_RIGHT", true);
				dir = false;
			}
		}
		break;

		case PLAYER_STATE::DEAD:

			break;

		}


		return;
	}
		
	if (m_bAuto)
	{
		AutomationPlayer_Action(m_vAutoTargetPos);
		return;
	}
	

	update_state();
	update_animation();
	update_move();
	


	if (KEY_CHECK(KEY::SPACE, KEY_STATE::TAP))
	{
		CreateMissile();
	}

	GetAnimator()->update();



	m_ePreState = m_eCurState;
	m_PreDir = m_Dir;



	// 어떤 다른 물체랑 상호작용 했는지에 따라 PlayerInfo 변화함



	wstring a = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE]);
	wstring b = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT]);
	wstring c = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS]);
	wstring d = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING]);
	wstring e = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_LEADERSHIP]);
	wstring f = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY]);
	wstring g = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SELF_GROWTH]);
	


	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE] = L"지식: " + a;
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT] = L"사회성: " + b;
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS] = L"자존감: " + c;
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING] = L"문제해결능력: " + d;
	//m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_LEADERSHIP] = L"리더쉽: " + e;
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY] = L"책임감: " + f;

	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_SELF_GROWTH] = L"자아성찰: ???";
	



}

void CPlayer::render(HDC _dc)
{
	// 컴포넌트 랜더함수 호출
	component_render(_dc);
	
}


void CPlayer::SetspeechBubble()
{
	// Player는 자기 머리위에 떠있는 대화UI를 자기자신이 관리함
	m_pUI = new CTalkUI();
	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Speech_Bubble_UI", L"texture\\Textbox_8.png");


	Vec2 vPos;
	if(GetCamAffectedObj())
		vPos = CCamera::GetInst()->GetRenderPos(GetPos());

	CTalkUI* pTalkUI = new CTalkUI; // 말풍선 Talk_UI
	pTalkUI->SetName(L"Speech_Bubble_UI");
	pTalkUI->SetPos(Vec2(vPos.x + 125.f, vPos.y - 110.f));
	pTalkUI->SetScale(Vec2(300.f,170.f)); 
	pTalkUI->SetUItex(UItex);
	pTalkUI->SetMovement(false);
	pTalkUI->SetHasText(true);
	pTalkUI->SetHasTexture(true);
	pTalkUI->CreateChildUI(pTalkUI);
	pTalkUI->SetMyConversationID(0);
	pTalkUI->SetCamAffectedObj(true);
	pTalkUI->SetNpcOwner(this);
	CreateObject(pTalkUI, GROUP_TYPE::UI);


}




void CPlayer::AutomationPlayer_Action(Vec2 _tagetPos)
{
	float fPlayerX = GetPos().x;

	if (5.f > abs(fPlayerX - _tagetPos.x))  // 오차 5 
	{
		m_Dir = PLAYER_DIR::LEFT;
		m_ePreState = PLAYER_STATE::SIT;
		m_eCurState = PLAYER_STATE::INCAPACITATION;

		if (m_onlymove)
		{
			m_Dir = PLAYER_DIR::RIGHT;
			m_ePreState = PLAYER_STATE::IDLE;
			m_eCurState = PLAYER_STATE::INCAPACITATION;
		}


		m_bAuto = false;
		return;
	}
	

	int dir = 0;
	if (fPlayerX > _tagetPos.x)
	{
		m_Dir = PLAYER_DIR::LEFT;
		dir = -1;
	}
	else
	{
		m_Dir = PLAYER_DIR::RIGHT;
		dir = 1;
	}
		
	fPlayerX += 100.f * fDT * 2.f * dir;
	m_eCurState = PLAYER_STATE::WALK;
	SetPos(Vec2(fPlayerX, GetPos().y));
	update_animation();
}



void CPlayer::CreateMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;

	// Missile Object
	CMissile* pMissile = new CMissile;
	pMissile->SetName(L"Missile_Player");
	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(25.f, 25.f));
	pMissile->SetDir(Vec2(0.f, -1.f));
	

	// CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	// pCurScene->AddObject(pMissile, GROUP_TYPE::DEFAULT);     이벤트 매니저가 처리해줄거임

	CreateObject(pMissile,GROUP_TYPE::PROJ_PLAYER);

}

void CPlayer::update_state()
{
	// 방향전환 상태

	// 키를 때면 IDLE상태로 바꿈
	if (KEY_CHECK(KEY::W, KEY_STATE::AWAY))
	{
		m_Dir = PLAYER_DIR::BACK;
		m_eCurState = PLAYER_STATE::IDLE;
	}

	else if (KEY_CHECK(KEY::S, KEY_STATE::AWAY))
	{
		m_Dir = PLAYER_DIR::FRONT;
		m_eCurState = PLAYER_STATE::IDLE;
	}

	else if (KEY_CHECK(KEY::A, KEY_STATE::AWAY))
	{
		m_Dir = PLAYER_DIR::LEFT;
		m_eCurState = PLAYER_STATE::IDLE;
	}

	else if (KEY_CHECK(KEY::D, KEY_STATE::AWAY))
	{
		m_Dir = PLAYER_DIR::RIGHT;
		m_eCurState = PLAYER_STATE::IDLE;
	}


	// 키를 누르고있을때 WALK Animation 재생하기 위한 상태체크
	else if (KEY_CHECK(KEY::W, KEY_STATE::HOLD))
	{
		m_Dir = PLAYER_DIR::BACK;
		m_eCurState = PLAYER_STATE::WALK;
	}

	else if (KEY_CHECK(KEY::S, KEY_STATE::HOLD))
	{
		m_Dir = PLAYER_DIR::FRONT;
		m_eCurState = PLAYER_STATE::WALK;
	}

	else if (KEY_CHECK(KEY::A, KEY_STATE::HOLD))
	{
		m_Dir = PLAYER_DIR::LEFT;
		m_eCurState = PLAYER_STATE::WALK;
	}

	else if (KEY_CHECK(KEY::D, KEY_STATE::HOLD))
	{
		m_Dir = PLAYER_DIR::RIGHT;
		m_eCurState = PLAYER_STATE::WALK;
	}

	




}



void CPlayer::update_move()
{
	Vec2 vPos = GetPos();
	// CRigidBody* pRigid = GetRigidBody();

	if (KEY_CHECK(KEY::W, KEY_STATE::HOLD))
	{
		vPos.y -= m_fSpeed * fDT;

	}

	else if (KEY_CHECK(KEY::S, KEY_STATE::HOLD))
	{
		vPos.y += m_fSpeed * fDT;

	}


	else if (KEY_CHECK(KEY::A, KEY_STATE::HOLD))
	{
		vPos.x -= m_fSpeed * fDT;

	}


	else if (KEY_CHECK(KEY::D, KEY_STATE::HOLD))
	{
		vPos.x += m_fSpeed * fDT;

	}


	SetPos(vPos);

	// if (KEY_CHECK(KEY::W, KEY_STATE::TAP))
	// {
	// 	
	// }
	// 
	// if (KEY_CHECK(KEY::S, KEY_STATE::TAP))
	// {
	// 	
	// }
	// 
	// if (KEY_CHECK(KEY::A, KEY_STATE::TAP))
	// {
	// 	
	// }
	// 
	// if (KEY_CHECK(KEY::D, KEY_STATE::TAP))
	// {
	// 	
	// }

}


void CPlayer::update_animation()
{
	static bool dir = true; // true == letf   //  false == right

	// 상태 변경 감지
	if (m_ePreState == m_eCurState && m_PreDir == m_Dir)
		return;

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
	{
		if(m_Dir == PLAYER_DIR::LEFT)
			GetAnimator()->Play(L"IDLE_LEFT", true);
		else if (m_Dir == PLAYER_DIR::RIGHT)
			GetAnimator()->Play(L"IDLE_RIGHT", true);
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::LEFT)
			GetAnimator()->Play(L"IDLE_LEFT", true);
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::RIGHT)
			GetAnimator()->Play(L"IDLE_RIGHT", true);
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::FRONT && dir == true)
			GetAnimator()->Play(L"IDLE_LEFT", true);
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::FRONT && dir == false)
			GetAnimator()->Play(L"IDLE_RIGHT", true);
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::BACK && dir == true)
			GetAnimator()->Play(L"IDLE_LEFT", true);
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::BACK && dir == false)
			GetAnimator()->Play(L"IDLE_RIGHT", true);

		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::LEFT)
			GetAnimator()->Play(L"IDLE_LEFT", true);
		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::RIGHT)
			GetAnimator()->Play(L"IDLE_RIGHT", true);
		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::FRONT && dir == true)
			GetAnimator()->Play(L"IDLE_LEFT", true);
		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::FRONT && dir == false)
			GetAnimator()->Play(L"IDLE_RIGHT", true);
		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::BACK && dir == true)
			GetAnimator()->Play(L"IDLE_LEFT", true);
		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::BACK && dir == false)
			GetAnimator()->Play(L"IDLE_RIGHT", true);


	}
		break;

	case PLAYER_STATE::WALK:
	{
		if (m_Dir == PLAYER_DIR::LEFT)
		{
			GetAnimator()->Play(L"WALK_LEFT", true);
			dir = true;
		}	
		else if (m_Dir == PLAYER_DIR::RIGHT)
		{
			GetAnimator()->Play(L"WALK_RIGHT", true);
			dir = false;
		}
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::LEFT)
		{
			GetAnimator()->Play(L"WALK_LEFT", true);
			dir = true;
		}
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::RIGHT)
		{
			GetAnimator()->Play(L"WALK_RIGHT", true);
			dir = false;
		}
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::FRONT && dir == true)
		{
			GetAnimator()->Play(L"WALK_LEFT", true);
			dir = true;
		}
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::FRONT && dir == false)
		{
			GetAnimator()->Play(L"WALK_RIGHT", true);
			dir = false;
		}
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::BACK && dir == true)
		{
			GetAnimator()->Play(L"WALK_LEFT", true);
			dir = true;
		}
		else if (m_Dir == PLAYER_DIR::FRONT && m_PreDir == PLAYER_DIR::BACK && dir == false)
		{
			GetAnimator()->Play(L"WALK_RIGHT", true);
			dir = false;
		}
		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::LEFT)
		{
			GetAnimator()->Play(L"WALK_LEFT", true);
			dir = true;
		}
		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::RIGHT)
		{
			GetAnimator()->Play(L"WALK_RIGHT", true);
			dir = false;
		}
		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::FRONT && dir == true)
		{
			GetAnimator()->Play(L"WALK_LEFT", true);
			dir = true;
		}
		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::FRONT && dir == false)
		{
			GetAnimator()->Play(L"WALK_RIGHT", true);
			dir = false;
		}
		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::BACK && dir == true)
		{
			GetAnimator()->Play(L"WALK_LEFT", true);
			dir = true;
		}
		else if (m_Dir == PLAYER_DIR::BACK && m_PreDir == PLAYER_DIR::BACK && dir == false)
		{
			GetAnimator()->Play(L"WALK_RIGHT", true);
			dir = false;
		}
		

	}
		break;

	case PLAYER_STATE::INCAPACITATION:
	{
		
	}
		break;

	case PLAYER_STATE::ATTACK:

		break;

	case PLAYER_STATE::SIT:
	{
		if (m_Dir == PLAYER_DIR::LEFT)
		{
			GetAnimator()->Play(L"SIT_LEFT", true);
			dir = true;
		}
		else if (m_Dir == PLAYER_DIR::RIGHT)
		{
			GetAnimator()->Play(L"SIT_RIGHT", true);
			dir = false;
		}
	}
		break;

	case PLAYER_STATE::DEAD:

		break;

	}

}



