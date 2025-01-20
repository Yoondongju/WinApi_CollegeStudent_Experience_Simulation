#include "pch.h"
#include "CMonster.h"
#include "CCore.h"

#include "CTimeMgr.h"
#include "CCollider.h"
#include "CAnimation.h"
#include "CAnimator.h"

#include "AI.h"
#include "CState.h"
#include "CPatrol.h"

#include "CResMgr.h"

#include "CPlayer.h"

#include "CUI.h"
#include "CBtnUI.h"
#include "CTalkUI.h"

#include "CSceneMgr.h"
#include "CScene.h"


void CreateTalkUI(DWORD_PTR _pa, DWORD_PTR _pa2);

CMonster::CMonster()
	: m_tInfo{}
	, m_pAI(nullptr)
	, m_waitTime(0.f)
	, m_bFindTarget(false)
	, m_Dissappear_End_of_conversation(true)
	, m_ableMultipleMarkCreate(true)
	, m_zoomInMon(true)
	, m_bDeleteAI(false)
{
	BLENDFUNCTION bf = GetBf();

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;

	SetName(L"Customer");
	SetBf(bf);

	m_tInfo.fSpeed = 30.f;


	CreateCollider();
	CreateAnimator();

	
	GetCollider()->SetOffsetPos(Vec2(0.f, 10.f));
	GetCollider()->SetScale(Vec2(15.f, 33.f));

	


}

CMonster::~CMonster()
{
	if (nullptr != m_pAI)
		delete m_pAI;
}




void CMonster::update()
{
	if (nullptr != m_pAI)
	{
		IsDeleteAI();  // AI가 이때 지워져야함

		if(m_pAI)
			m_pAI->update();
		
	}
		



	update_animation();


	if (m_bFindTarget && !m_bDoingTalk)
	{		
		CreateMarkUI();
	}
	
	if (!m_bFindTarget || m_bDoingTalk) // Dead Object는 다음 프레임에 처리해줘야하는데 여기서 바로걸림
	{
		if (nullptr != m_MarkUI)
		{
			if (!m_MarkUI->IsDead())
			{
				DeleteObject(m_MarkUI);
				m_MarkUI = nullptr;
			}
			
		}
	}

}


void CMonster::update_animation()
{
	// 상태 변경 감지
	if (m_ePreState == m_eCurState && m_PreDir == m_Dir)
		return;

	if (L"Customer1" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"CUSTOMER1_SAY", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"CUSTOMER1_MOVE_LEFT", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"CUSTOMER1_MOVE_RIGHT", true);
			
		}
		break;

		case CUSTOMER_STATE::DEAD:
			break;
		}
	}
	
	else if (L"Customer2" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"CUSTOMER2_SAY", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"CUSTOMER2_MOVE_LEFT", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"CUSTOMER2_MOVE_RIGHT", true);

		}
		break;

		case CUSTOMER_STATE::DEAD:
			break;
		}
	}

	else if (L"Customer3" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"CUSTOMER3_SAY", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"CUSTOMER3_MOVE_LEFT", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"CUSTOMER3_MOVE_RIGHT", true);

		}
		break;

		case CUSTOMER_STATE::DEAD:
			break;
		}
	}

	else if (L"badCustomer" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"BAD_CUSTOMER_SAY", true);
		}
		break;

		case CUSTOMER_STATE::DEAD:
			break;
		}

	}


	else if (L"Manager" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"MANAGER_SAY", true);
		}
		break;

		case CUSTOMER_STATE::DEAD:
			break;
		}

	}

	else if (L"unknown_friend" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SIT:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"UNKNOWN_FRIEND_SIT_LEFT", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"UNKNOWN_FRIEND_SIT_RIGHT", true);
		}			
			break;
		}

	}

	else if (L"unknown_friend_2" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SIT:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"UNKNOWN_FRIEND_2_SIT_LEFT", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"UNKNOWN_FRIEND_2_SIT_RIGHT", true);
		}
		break;
		}

	}

	else if (L"unknown_friend_3" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SIT:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"UNKNOWN_FRIEND_3_SIT_LEFT", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"UNKNOWN_FRIEND_3_SIT_RIGHT", true);
		}
		break;
		}

	}

	else if (L"unknown_friend_4" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SIT:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"UNKNOWN_FRIEND_4_SIT_LEFT", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"UNKNOWN_FRIEND_4_SIT_RIGHT", true);
		}
		break;
		}

	}

	else if (L"friend_2" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SIT:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"FRIEND2_SIT_LEFT", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"FRIEND2_SIT_RIGHT", true);
		}
			break;


		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"FRIEND2_IDLE", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"FRIEND2_WALK_LEFT", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"FRIEND2_WALK_RIGHT", true);
		}
		break;

		case CUSTOMER_STATE::VOMIT:
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"FRIEND2_VOMIT_LEFT", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"FRIEND2_VOMIT_RIGHT", true);
			break;

		case CUSTOMER_STATE::DEAD:
			break;
		}

	}
 
	else if (L"CuteBoy" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"CuteBoy_stand", true);
		}
		break;


		case CUSTOMER_STATE::SIT:
		{
			GetAnimator()->Play(L"CuteBoy_sit", true);
		}
		break;
		

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"CuteBoy_Walk", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"CuteBoy_Walk_RIGHT", true);
		}
		break;
		}
	}


	else if (L"OutsideBoy" == GetName())
	{
		switch (m_eCurState)
		{

		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"OutsideBoy_STAND", true);
		}
		break;

		case CUSTOMER_STATE::SIT:
		{
			if (m_emotion == MON_EMOTION::CRY)
				GetAnimator()->Play(L"OutsideBoy_SIT_CRY", true);
	
			else if (m_emotion == MON_EMOTION::NONE)
				GetAnimator()->Play(L"OutsideBoy_SIT", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"OutsideBoy_WALK_LEFT", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"OutsideBoy_WALK_RIGHT", true);
		}
		break;
		}
	}



	else if (L"President" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"President_STAND", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"President_WALK", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"President_WALK", true);
		}
		break;

		case CUSTOMER_STATE::DANCE:
		{
			GetAnimator()->Play(L"President_DANCE", true);
		}
		break;
		}

	}

	else if (L"VicePresident" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"VicePresident_STAND", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"VicePresident_WALK", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"VicePresident_WALK", true);
		}
		break;

		case CUSTOMER_STATE::DANCE:
		{
			GetAnimator()->Play(L"VicePresident_DANCE", true);
		}
		break;

		}

	}
	




	else if (L"Earnest_friend" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"Earnest_man_stand", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"Earnest_man_walk_left", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"Earnest_man_walk_right", true);
		}
		break;

		case CUSTOMER_STATE::VOMIT:
		{
			GetAnimator()->Play(L"Earnest_man_prone_left", true);
		}
		break;


		}

	}

	else if (L"Earnest_friend_woman" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"Earnest_woman_stand", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"Earnest_woman_walk_left", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"Earnest_woman_walk_right", true);
		}
		break;
		}

	}

	else if (L"Earnest_friend_woman2" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"Earnest_woman2_stand", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"Earnest_woman2_walk_left", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"Earnest_woman2_walk_right", true);
		}
		break;
		}

	}


	else if (L"DateGirl" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"DateGirl_stand", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			if (m_Dir == CUSTOMER_DIR::LEFT)
				GetAnimator()->Play(L"DateGirl_walk_left", true);
			else if (m_Dir == CUSTOMER_DIR::RIGHT)
				GetAnimator()->Play(L"DateGirl_walk_right", true);
		}
		break;

		case CUSTOMER_STATE::SIT:
		{
			if (m_emotion == MON_EMOTION::CRY)
				GetAnimator()->Play(L"OutsideBoy_SIT_CRY", true);

			else if (m_emotion == MON_EMOTION::NONE)
				GetAnimator()->Play(L"DateGirl_sit", true);
		}
		}
	}




	else if (L"toiletMan" == GetName())
	{
	switch (m_eCurState)
	{
	case CUSTOMER_STATE::SAY:
	{
		GetAnimator()->Play(L"TOILETMAN_IDLE", true);
	}
	break;

	case CUSTOMER_STATE::WALK:
	{
		GetAnimator()->Play(L"TOILETMAN_WALK", true);
	}
	break;

	case CUSTOMER_STATE::DEAD:
		break;
	}

	}

	else if (L"smokingGirl" == GetName())
	{
		switch (m_eCurState)
		{
		case CUSTOMER_STATE::SAY:
		{
			GetAnimator()->Play(L"SMOKINGGIRL_IDLE", true);
		}
		break;

		case CUSTOMER_STATE::WALK:
		{
			GetAnimator()->Play(L"SMOKINGGIRL_WALK", true);
		}
		break;

		case CUSTOMER_STATE::DEAD:
			break;
		}

	}


	else if (L"mysterious_boy" == GetName())
	{
		GetAnimator()->Play(L"MYSTERIOUS_BOY", true);
	}
	
	else if (L"future_me" == GetName())
	{
		GetAnimator()->Play(L"FUTUREME_IDLE_LEFT", true);
	}

}


void CMonster::update_state()
{
	// 랜덤으로 움직이는 움직임에 따라 애니메이션을 조정할 상태값도 조정해야함

}



void CMonster::update_move()
{
	// 랜덤으로 움직이게 해야함
	// 몬스터가 부착한 AI의 Patrol Class가 자유이동 진행

}



void CMonster::SetAI(AI* _AI)
{
	if (nullptr == _AI)
	{		
		delete m_pAI;
		m_pAI = nullptr;  // 이건 Monster가 들고있는 멤버 AI니까  State가 들고잇는 멤버 AI도 nullptr 밀어줘야댐

		return;
	}


	m_pAI = _AI;

	if(m_pAI)
		m_pAI->m_pOwner = this;

}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{	 
	 CObject* pOtherObj = _pOther->GetObj();
	 
	 if (pOtherObj->GetName() == L"Missile_Player")
	 {

	 		DeleteObject(this);
	 
	 }
	 else if (pOtherObj->GetName() == L"Ground")
	 {
		 Vec2 vPos = (GetPos().x + 3 * -(int)m_Dir, GetPos().y); // x 방향만 체크
		 SetPos(vPos);

		 if (m_pAI)
		 {
			 CState* monState = m_pAI->GetState(MON_STATE::PATROL);
			 CPatrol* monPatrol = (CPatrol*)monState;

			 if(monPatrol)
				monPatrol->CreateTargetPos();
		 }
		 
	 }


}

void CMonster::CreateMarkUI()
{
	if (nullptr != m_MarkUI)
		return;

	CTexture* pMark_UI = CResMgr::GetInst()->LoadTexture(L"Mark_UI", L"texture\\Mark.png");

	Vec2 vPos = GetPos();

	CBtnUI* pBtn_ExclamationMark_UI = new CBtnUI;
	pBtn_ExclamationMark_UI->SetName(L"ExclamationMark_UI");
	pBtn_ExclamationMark_UI->SetScale(Vec2(50.f, 45.f));
	pBtn_ExclamationMark_UI->SetPos(Vec2(vPos.x, vPos.y - 30.f));
	pBtn_ExclamationMark_UI->SetUItex(pMark_UI);
	pBtn_ExclamationMark_UI->SetMovement(false);
	pBtn_ExclamationMark_UI->SetHasText(false);
	pBtn_ExclamationMark_UI->SetHasTexture(true);
	pBtn_ExclamationMark_UI->SetClickedCallBack(CreateTalkUI, (DWORD_PTR)this, 0);
	m_MarkUI = pBtn_ExclamationMark_UI;

	CreateObject(pBtn_ExclamationMark_UI, GROUP_TYPE::UI); // 등록해주는 느낌
}



// 몬스터와 대화하였을때 생기는 대화UI는 말풍선 대화UI가 아니기때문에 센터하단에 위치 무조건 고정
void CreateTalkUI(DWORD_PTR _pa, DWORD_PTR _pa2)
{
	// npc 대화중 설정//////////////////
	CMonster* pMon = (CMonster*)_pa;
	pMon->SetDoingTalk(true);

	if(pMon->IsZoomInMon())
	{
		CCamera::GetInst()->SetZoomIn(true); // 시점이 여기서 줌인체크를 해줘야 다른곳에서 먼저 렌더링 되지않음
	}
	
	
		
	// npc 대화 false 설정하는부분은 따로 없음 대화를 하고 손님의 니즈를 다 들어주면 그냥 바로 삭제해버릴거임
	// npc 대화상태가 true인 애들을 TalkUI cpp의 End of Conversation에서 삭제 처리해줄거임




	pG_Player->SetCurState(PLAYER_STATE::INCAPACITATION);  // main Player 행동 불가능 설정


	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CTexture* UItex = CResMgr::GetInst()->LoadTexture(L"Job_Talk_UI", L"texture\\TalkUI.png");

	CTalkUI* pTalkUI = new CTalkUI;
	pTalkUI->SetName(L"Job_Talk_UI");
	pTalkUI->SetPos(Vec2(vResolution.x / 2.f, vResolution.y / 2.f + vResolution.y / 4.f + 50));
	pTalkUI->SetScale(Vec2(UI_WIDTH, UI_HEIGHT)); // 대화창 UI의 크기는 무조건 이 사이즈로 고정
	pTalkUI->SetUItex(UItex);
	pTalkUI->SetMovement(false);
	pTalkUI->SetHasText(true);
	pTalkUI->SetHasTexture(true);
	pTalkUI->CreateChildUI(pTalkUI);
	pTalkUI->SetNpcOwner(pMon);

	CreateObject(pTalkUI, GROUP_TYPE::UI);

}