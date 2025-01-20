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

CPlayer* pG_Player = nullptr; // ���� ������ �����մϴ�.


int CPlayer::Final_info_calculate()
{
	// ��ȸ��, ������, �����ذ�ɷ�, å�Ӱ� �� 4���� �ɷ�ġ�� ���� �ɷ�ġ�� �ɰ���
	// �� �ɷ�ġ�� MAX �ɷ�ġ�� 10�� 
	 
	double dToalInfo = 0.f;
	double mainInfo = 0.f;
	UINT temp = 0;
	wstring strMainInfo;

	dToalInfo += m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE];			 // ����
	dToalInfo += m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT];			 // ��ȸ��
	dToalInfo += m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS];		 // ������
	dToalInfo += m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING];	 // �����ذ�ɷ�
	dToalInfo += m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY];		 // å�Ӱ�



	double avg = dToalInfo / 5.f;		   // avg�� ���� player�ؼ� ���� ��ġ�� ���
	double maxAvg = (50 / 5.f) / 2.f;	   // ���� �÷����� ��ġ�� ���ϸ� ���� ���ġ 
	

	// ���� ���� �ɷ�ġ�� ������
	for (int i=6; i<=9; ++i)
	{	
		if (mainInfo < m_PlyerInfo[i])
		{
			mainInfo = m_PlyerInfo[i];
			temp = i;
			// �̶� i�� ������ � �ɷ�ġ���� �Ǻ� i�� 5�� ������ȵ� (������ ����� �����ϱ�)	
		}
		
	}

	
	if (avg > maxAvg)		 // ��� �̻�
	{
		mainInfo; // main �ɷ�ġ �̾Ƴ���

		switch (temp)
		{
		
		case 6:
		{
			strMainInfo = L"��ȸ��";
			return (UINT)PLAYER_INFO::AMOUNT_SOCIABILIT;
		}
		break;

		case 7:
		{
			strMainInfo = L"������";
			return (UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS;
		}

		case 8:
		{
			strMainInfo = L"�����ذ�ɷ�";
			return (UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING;
		}
		break;

		case 9:
		{
			strMainInfo = L"å�Ӱ�";
			return (UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY;
		}
		break;



		default:
			break;
		}
	}
	else if (avg < maxAvg)	 // ��� ����
	{
		mainInfo; // main �ɷ�ġ �̾Ƴ���
		

		switch (temp)
		{

		case 6:
		{
			strMainInfo = L"��ȸ��";
			(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT;
			return -6;
		}
		break;

		case 7:
		{
			strMainInfo = L"������";
			return -7;
		}

		case 8:
		{
			strMainInfo = L"�����ذ�ɷ�";
			return -8;
		}
		break;

		case 9:
		{
			strMainInfo = L"å�Ӱ�";
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
	//  �������� ���� �ܼ��� ������ ������ ��������������
	//  ��ȭâ�����ɷ� ���� �������� UI�� �����ָ鼭 ������ ���������ְ� �״����� �ø�
	//
	// 
	// 	AMOUNT_KNOWLEDGE,	 	 // ���� 
	// 	AMOUNT_SOCIABILIT,		 // ��ȸ��, �米�� 
	// 	AMOUNT_ATTRACTIVENESS,	 // �ŷµ�
	// 	AMOUNT_PROBLEM_SOLVING,  // ���� �ذ� �ɷ�
	// 	AMOUNT_LEADERSHIP,		 // ������
	// 	AMOUNT_RESPONSIBILITY,	 // å�Ӱ�   
	// 
	// 
	// 
	// 
	// 	AMOUNT_SELF_GROWTH,		 // �ھ� ���� (Player�� ���� 6������ ������ ���� ������ �ھƼ����� �ϰԵ�)
	// 
	// 	END,
	 

	

	m_ePlayerInfo[(UINT)PLAYER_INFO::NAME] = L"�̸�: ���ѽ�";
	m_ePlayerInfo[(UINT)PLAYER_INFO::GRADE] = L"�г�: 1�г�";

	wstring strMbti = EnumToString(m_Mbti);
	
	m_ePlayerInfo[(UINT)PLAYER_INFO::MBTI] = L"MBTI: "+ strMbti;

	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE] = L"����: ";
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT] = L"��ȸ��: ";
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS] = L"������: ";
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING] = L"�����ذ�ɷ�: ";
	//m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_LEADERSHIP] = L"������: ";
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY] = L"å�Ӱ�: ";

	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_SELF_GROWTH] = L"�ھƼ���: ???";


	// Panel ui�� ������ ������ str�� ���� �����ؾ��ұ�


	
	



	// mbti�� ���� �ʱ�ɷ�ġ �����
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
		// �����Ǵ� �ƹ� �ɷ�ġ ����;
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

	

	

	// ��ü�ֱ�
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



	// � �ٸ� ��ü�� ��ȣ�ۿ� �ߴ����� ���� PlayerInfo ��ȭ��



	wstring a = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE]);
	wstring b = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT]);
	wstring c = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS]);
	wstring d = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING]);
	wstring e = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_LEADERSHIP]);
	wstring f = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY]);
	wstring g = std::to_wstring(m_PlyerInfo[(UINT)PLAYER_INFO::AMOUNT_SELF_GROWTH]);
	


	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_KNOWLEDGE] = L"����: " + a;
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_SOCIABILIT] = L"��ȸ��: " + b;
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_ATTRACTIVENESS] = L"������: " + c;
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_PROBLEM_SOLVING] = L"�����ذ�ɷ�: " + d;
	//m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_LEADERSHIP] = L"������: " + e;
	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_RESPONSIBILITY] = L"å�Ӱ�: " + f;

	m_ePlayerInfo[(UINT)PLAYER_INFO::AMOUNT_SELF_GROWTH] = L"�ھƼ���: ???";
	



}

void CPlayer::render(HDC _dc)
{
	// ������Ʈ �����Լ� ȣ��
	component_render(_dc);
	
}


void CPlayer::SetspeechBubble()
{
	// Player�� �ڱ� �Ӹ����� ���ִ� ��ȭUI�� �ڱ��ڽ��� ������
	m_pUI = new CTalkUI();
	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Speech_Bubble_UI", L"texture\\Textbox_8.png");


	Vec2 vPos;
	if(GetCamAffectedObj())
		vPos = CCamera::GetInst()->GetRenderPos(GetPos());

	CTalkUI* pTalkUI = new CTalkUI; // ��ǳ�� Talk_UI
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

	if (5.f > abs(fPlayerX - _tagetPos.x))  // ���� 5 
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
	// pCurScene->AddObject(pMissile, GROUP_TYPE::DEFAULT);     �̺�Ʈ �Ŵ����� ó�����ٰ���

	CreateObject(pMissile,GROUP_TYPE::PROJ_PLAYER);

}

void CPlayer::update_state()
{
	// ������ȯ ����

	// Ű�� ���� IDLE���·� �ٲ�
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


	// Ű�� ������������ WALK Animation ����ϱ� ���� ����üũ
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

	// ���� ���� ����
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



