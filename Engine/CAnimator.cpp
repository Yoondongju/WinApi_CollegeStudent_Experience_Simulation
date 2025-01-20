#include "pch.h"
#include "CAnimator.h"

#include "CAnimation.h"

#include "CResMgr.h"
#include "CObject.h"


CAnimator::CAnimator()
	: m_pCurAnim(nullptr)
	, m_pOwner(nullptr)
	, m_bRepeat(false)
{

}

CAnimator::CAnimator(const CAnimator& _origin)
	: m_pOwner(this->m_pOwner)
	, m_bRepeat(_origin.m_bRepeat)
	, m_pCurAnim(nullptr)

{
	
	init_Animation();

	// Animator가 만들어질때 init_Animation 호출하고
	// init_Animation 호출시점에 CreateAnimation 호출 다음으로 Animation 만들어짐
}


CAnimator::~CAnimator()
{

	Safe_Delete_Map(m_mapAnim);

}




void CAnimator::update()
{
	
	
}

void CAnimator::finalupdate()
{
	if (nullptr != m_pCurAnim)
	{
		m_pCurAnim->update();

		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->SetFrame(0);

			if (L"MBTI_UI" == m_pCurAnim->GetName())  // 별모양 물방울 이미지가 ㅈ같아서 어거지로 수정함
			{
				Play(L"MBTI_UI2", true);
			}
			else if (L"MBTI_UI2" == m_pCurAnim->GetName())  // 별모양 물방울 이미지가 ㅈ같아서 어거지로 수정함
			{
				Play(L"MBTI_UI", true);
			}
		}
	}
}

void CAnimator::render(HDC _dc)
{
	if (nullptr != m_pCurAnim)
	{
		m_pCurAnim->render(_dc);
	}
}


void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLT, Vec2 _vScliceSize, Vec2 _vStep, float _fDuration , UINT _iFrameCount)
{
	CAnimation* pAnim = FindAnimation(_strName);
	assert(nullptr == pAnim);


	pAnim = new CAnimation();

	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;
	pAnim->Create(_pTex, _vLT, _vScliceSize, _vStep, _fDuration ,_iFrameCount);


	m_mapAnim.insert(make_pair(_strName, pAnim)); // 잘 모르겠음
}


void CAnimator::LoadAnimation(const wstring& _strRelativePath)
{
	CAnimation* pAnim = new CAnimation;
	pAnim->Load(_strRelativePath);

	pAnim->m_pAnimator = this;
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim)); // 잘 모르겠음

}


CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
		return nullptr;


	return iter->second;
}


void CAnimator::Play(const wstring& _strName , bool _bRepeat)
{
	m_pCurAnim = FindAnimation(_strName);
	assert(m_pCurAnim);

	
	m_bRepeat = _bRepeat;

}


void CAnimator::init_Animation()
{
	// Player
	// 이미지의 크기가 지금 144 x 384임
	// 가로 Slice는 48이고 세로 Sclice는 96임
	// =======================================

	// Bird
	// 이미지의 크기가 지금 721 x 567임
	// 가로 90 세로 95
	// =======================================


	// Customer
	// 이미지의 크기가 97 x 124
	// 가로 32.4 세로 31
	// =======================================



	// ResMgr를통한 Texture 로딩하기
	CTexture* pPlayer_stand_left = CResMgr::GetInst()->LoadTexture(L"Player_stand_left", L"texture\\Player\\stand\\player_stand_left.png"); // 애니메이션을 만들라고 사진 로딩함
	CTexture* pPlayer_stand_right = CResMgr::GetInst()->LoadTexture(L"Player_stand_right", L"texture\\Player\\stand\\player_stand_right.png");


	CTexture* pPlayer_sit_left = CResMgr::GetInst()->LoadTexture(L"Player_sit_left", L"texture\\Player\\sit\\sit_left.png");
	CTexture* pPlayer_sit_right = CResMgr::GetInst()->LoadTexture(L"Player_sit_right", L"texture\\Player\\sit\\sit_right.png");


	CTexture* pfuture_Player_stand_left = CResMgr::GetInst()->LoadTexture(L"future_Player_stand_left", L"texture\\Player\\stand\\future_player_stand_left.png");

	CTexture* pPlayer_walk_left = CResMgr::GetInst()->LoadTexture(L"Player_walk_left", L"texture\\Player\\walk\\player_walk_left.png"); // 애니메이션을 만들라고 사진 로딩함
	CTexture* pPlayer_walk_right = CResMgr::GetInst()->LoadTexture(L"Player_walk_right", L"texture\\Player\\walk\\player_walk_right.png");








	CTexture* pBird_left = CResMgr::GetInst()->LoadTexture(L"Bird_left", L"texture\\Bird_left.png");
	CTexture* pBird_right = CResMgr::GetInst()->LoadTexture(L"Bird_right", L"texture\\Bird_right.png");

	
	CTexture* pPlayer_Btn_Effect_UI = CResMgr::GetInst()->LoadTexture(L"Player_Btn_Effect_UI", L"texture\\123.png");

	CTexture* pLightning = CResMgr::GetInst()->LoadTexture(L"Lightning", L"texture\\Lightning.png");


	CTexture* Mbti_UI = CResMgr::GetInst()->LoadTexture(L"Mbti_UI", L"texture\\mbti.png");




	// Effect // 
	CTexture* Star = CResMgr::GetInst()->LoadTexture(L"Star", L"texture\\Effect\\star.png");
	CTexture* Teleport = CResMgr::GetInst()->LoadTexture(L"Teleport", L"texture\\Effect\\Teleport.png");

	// Effect //





	CTexture* mysterious = CResMgr::GetInst()->LoadTexture(L"MYSTERIOUS_BOY", L"texture\\MYSTERIOUS_AP.png");








	/// <JOB>
	CTexture* pJob_Manager = CResMgr::GetInst()->LoadTexture(L"JobManager_say", L"texture\\npc\\Job_Scene_npc\\manager_say.png");


	CTexture* pCustomer1_say = CResMgr::GetInst()->LoadTexture(L"Customer1_say", L"texture\\npc\\Job_Scene_npc\\customer1_say.png");
	CTexture* pCustomer1_move_left = CResMgr::GetInst()->LoadTexture(L"Customer1_move_left", L"texture\\npc\\Job_Scene_npc\\customer1_move_left.png");
	CTexture* pCustomer1_move_right = CResMgr::GetInst()->LoadTexture(L"Customer1_move_right", L"texture\\npc\\Job_Scene_npc\\customer1_move_right.png");


	CTexture* pCustomer2_say = CResMgr::GetInst()->LoadTexture(L"Customer2_say", L"texture\\npc\\Job_Scene_npc\\customer2_say.png");
	CTexture* pCustomer2_move_left = CResMgr::GetInst()->LoadTexture(L"Customer2_move_left", L"texture\\npc\\Job_Scene_npc\\customer2_move_left.png");
	CTexture* pCustomer2_move_right = CResMgr::GetInst()->LoadTexture(L"Customer2_move_right", L"texture\\npc\\Job_Scene_npc\\customer2_move_right.png");


	CTexture* pCustomer3_say = CResMgr::GetInst()->LoadTexture(L"Customer3_say", L"texture\\npc\\Job_Scene_npc\\customer3_say.png");
	CTexture* pCustomer3_move_left = CResMgr::GetInst()->LoadTexture(L"Customer3_move_left", L"texture\\npc\\Job_Scene_npc\\customer3_move_left.png");
	CTexture* pCustomer3_move_right = CResMgr::GetInst()->LoadTexture(L"Customer3_move_right", L"texture\\npc\\Job_Scene_npc\\customer3_move_right.png");


	CTexture* pbadCustomer = CResMgr::GetInst()->LoadTexture(L"badCustomer", L"texture\\npc\\Job_Scene_npc\\badcustomer_say.png");
	/// <JOB>
	




	// <Drinking>

	CTexture* punknown_friend_1_sit_left = CResMgr::GetInst()->LoadTexture(L"punknown_friend_1_sit_left", L"texture\\npc\\man1\\sit_left.png");
	CTexture* punknown_friend_1_sit_right = CResMgr::GetInst()->LoadTexture(L"punknown_friend_1_sit_right", L"texture\\npc\\man1\\sit_right.png");

	CTexture* punknown_friend_2_sit_left = CResMgr::GetInst()->LoadTexture(L"punknown_friend_2_sit_left", L"texture\\npc\\man4\\sit_left.png");
	CTexture* punknown_friend_2_sit_right = CResMgr::GetInst()->LoadTexture(L"punknown_friend_2_sit_right", L"texture\\npc\\man4\\sit_right.png");


	CTexture* punknown_friend_3_sit_left = CResMgr::GetInst()->LoadTexture(L"punknown_friend_3_sit_left", L"texture\\npc\\woman1\\sit_left.png");
	CTexture* punknown_friend_3_sit_right = CResMgr::GetInst()->LoadTexture(L"punknown_friend_3_sit_right", L"texture\\npc\\woman1\\sit_right.png");

	CTexture* punknown_friend_4_sit_left = CResMgr::GetInst()->LoadTexture(L"punknown_friend_4_sit_left", L"texture\\npc\\woman2\\sit_left.png");
	CTexture* punknown_friend_4_sit_right = CResMgr::GetInst()->LoadTexture(L"punknown_friend_4_sit_right", L"texture\\npc\\woman2\\sit_right.png");






	CTexture* friend_2_sit_left = CResMgr::GetInst()->LoadTexture(L"friend_2_sit_left", L"texture\\npc\\man5\\sit_left.png");
	CTexture* friend_2_sit_right = CResMgr::GetInst()->LoadTexture(L"friend_2_sit_right", L"texture\\npc\\man5\\sit_right.png");


	CTexture* pfriend_2_idle = CResMgr::GetInst()->LoadTexture(L"friend_2_idle", L"texture\\npc\\man5\\stand\\stand_idle_left.png");

	CTexture* pfriend_2_vomit_left = CResMgr::GetInst()->LoadTexture(L"friend_2_vomit_left", L"texture\\npc\\man5\\stand\\stand_vomit_left.png");
	CTexture* pfriend_2_vomit_right = CResMgr::GetInst()->LoadTexture(L"friend_2_vomit_right", L"texture\\npc\\man5\\stand\\stand_vomit_right.png");
	
	CTexture* pfriend_2_walk_left = CResMgr::GetInst()->LoadTexture(L"friend_2_walk_left", L"texture\\npc\\man5\\walk\\walk_left.png");
	CTexture* pfriend_2_walk_right = CResMgr::GetInst()->LoadTexture(L"friend_2_walk_right", L"texture\\npc\\man5\\walk\\walk_right.png");

	
	// <Drinking>


	/// <Drinking_toilet>
	CTexture* toiletManIdle = CResMgr::GetInst()->LoadTexture(L"toiletManIdle", L"texture\\npc\\toiletMan_Idle.png");
	CTexture* toiletManWalk = CResMgr::GetInst()->LoadTexture(L"toiletManWalk", L"texture\\npc\\toiletMan_walk.png");
	/// <Drinking_toilet>







	/// <Drinking_Smoking>
	CTexture* smokingGirlIdle = CResMgr::GetInst()->LoadTexture(L"smokingGirlIdle", L"texture\\npc\\smoking_girl\\stand\\stand_left.png");
	CTexture* smokingGirlWalk = CResMgr::GetInst()->LoadTexture(L"smokingGirlWalk", L"texture\\npc\\smoking_girl\\walk\\walk_left.png");
	/// <Drinking_Smoking>




	CTexture* pPotal = CResMgr::GetInst()->LoadTexture(L"Potal", L"texture\\Potal.png");




	// MT
	CTexture* pBus = CResMgr::GetInst()->LoadTexture(L"Bus", L"texture\\Bus_updown.png");
	CTexture* pCuteBoy = CResMgr::GetInst()->LoadTexture(L"CuteBoy", L"texture\\npc\\man2\\stand\\stand_left.png");
	CTexture* pCuteBoy_sit = CResMgr::GetInst()->LoadTexture(L"CuteBoy_sit", L"texture\\npc\\man2\\sit.png");
	CTexture* pCuteBoyWalk = CResMgr::GetInst()->LoadTexture(L"CuteBoy_walk", L"texture\\npc\\man2\\walk\\walk_left.png");
	CTexture* pCuteBoyWalk_right = CResMgr::GetInst()->LoadTexture(L"CuteBoy_walk_right", L"texture\\npc\\man2\\walk\\walk_right.png");


	CTexture* pOutsideBoy_stand = CResMgr::GetInst()->LoadTexture(L"pOutsideBoy_stand", L"texture\\npc\\bus_man\\walk\\stand.png");
	CTexture* pOutsideBoy_walk_left = CResMgr::GetInst()->LoadTexture(L"pOutsideBoy_walk_left", L"texture\\npc\\bus_man\\walk\\walk_left.png");
	CTexture* pOutsideBoy_walk_right = CResMgr::GetInst()->LoadTexture(L"pOutsideBoy_walk_right", L"texture\\npc\\bus_man\\walk\\walk_right.png");
	CTexture* pOutsideBoy_sit = CResMgr::GetInst()->LoadTexture(L"pOutsideBoy_sit", L"texture\\npc\\bus_man\\sit\\sit_left.png");
	CTexture* pOutsideBoy_sit_cry = CResMgr::GetInst()->LoadTexture(L"pOutsideBoy_sit_cry", L"texture\\npc\\bus_man\\sit\\sit_cry.png");



	CTexture* pPresident_stand_right = CResMgr::GetInst()->LoadTexture(L"pPresident_stand_right", L"texture\\npc\\president\\stand\\stand_right.png");
	CTexture* pPresident_walk_right = CResMgr::GetInst()->LoadTexture(L"pPresident_walk_right", L"texture\\npc\\president\\walk\\walk_right.png");
	CTexture* pPresident_dance_right = CResMgr::GetInst()->LoadTexture(L"pPresident_dance_right", L"texture\\npc\\president\\dance\\dance_right.png");

	//CTexture* pVicePresident_stand_right = CResMgr::GetInst()->LoadTexture(L"pVicePresident_stand_right", L"texture\\npc\\vicepresident\\stand\\stand_right.png");
	//CTexture* pVicePresident_walk_right  = CResMgr::GetInst()->LoadTexture(L"pVicePresident_walk_right", L"texture\\npc\\vicepresident\\walk\\walk_right.png");
	//CTexture* pVicePresident_dance_right = CResMgr::GetInst()->LoadTexture(L"pVicePresident_dance_right", L"texture\\npc\\vicepresident\\dance\\dance_right.png");

	// MT

	// DATE 

	CTexture* pTaxi = CResMgr::GetInst()->LoadTexture(L"Taxi", L"texture\\Taxi.png");

	CTexture* pDategirl_stand_right = CResMgr::GetInst()->LoadTexture(L"Dategirl_stand", L"texture\\npc\\Date\\stand\\stand_right.png");
	CTexture* pDategirl_walk_left = CResMgr::GetInst()->LoadTexture(L"Dategirl_walk_left", L"texture\\npc\\Date\\walk\\walk_left.png");
	CTexture* pDategirl_walk_right = CResMgr::GetInst()->LoadTexture(L"Dategirl_walk_right", L"texture\\npc\\Date\\walk\\walk_right.png");
	CTexture* pDategirl_sit = CResMgr::GetInst()->LoadTexture(L"Dategirl_sit", L"texture\\npc\\Date\\sit\\sit_right.png");

	CTexture* pNumber = CResMgr::GetInst()->LoadTexture(L"Number", L"texture\\num\\123.png");


	// DATE



	// Sea MT //
	CTexture* pEarnest_man_stand = CResMgr::GetInst()->LoadTexture(L"Earnest_man_stand", L"texture\\npc\\man3\\stand\\stand_left.png");
	CTexture* pEarnest_man_walk_left = CResMgr::GetInst()->LoadTexture(L"Earnest_man_walk_left", L"texture\\npc\\man3\\walk\\walk_left.png");
	CTexture* pEarnest_man_walk_right = CResMgr::GetInst()->LoadTexture(L"Earnest_man_walk_right", L"texture\\npc\\man3\\walk\\walk_right.png");
	CTexture* pEarnest_prone_left = CResMgr::GetInst()->LoadTexture(L"Earnest_man_prone_left", L"texture\\npc\\man3\\prone\\prone.png");

	

	CTexture* pEarnest_woman_stand = CResMgr::GetInst()->LoadTexture(L"Earnest_woman_stand", L"texture\\npc\\woman4\\stand\\stand_left.png");
	CTexture* pEarnest_woman_walk_left = CResMgr::GetInst()->LoadTexture(L"Earnest_woman_walk_left", L"texture\\npc\\woman4\\walk\\walk_left.png");
	CTexture* pEarnest_woman_walk_right = CResMgr::GetInst()->LoadTexture(L"Earnest_woman_walk_right", L"texture\\npc\\woman4\\walk\\walk_right.png");

	CTexture* pEarnest_woman2_stand = CResMgr::GetInst()->LoadTexture(L"Earnest_woman2_stand", L"texture\\npc\\woman5\\stand\\stand_left.png");
	CTexture* pEarnest_woman2_walk_left = CResMgr::GetInst()->LoadTexture(L"Earnest_woman2_walk_left", L"texture\\npc\\woman5\\walk\\walk_left.png");
	CTexture* pEarnest_woman2_walk_right = CResMgr::GetInst()->LoadTexture(L"Earnest_woman2_walk_right", L"texture\\npc\\woman5\\walk\\walk_right.png");
	// Sea MT //


	







	// Object들은 각자 하나의 애니메이터를 보유하는데
	// 해당 애니메이터는 그 Object와 맞는 애니메이션만 보유하면됨
	// 하지만 현재 코드는 Player Object의 애니메이터가 Player 애니메이션, 새 애니메이션, 이펙트 애니메이션 등등
	// Bird Object의 애니메이터도 자기 자신이 쓸 애니메이션만 보유하면되는데 쓸데없이 보유중임, 이거 수정해야함

	
	wstring a = GetObj()->GetName();
	
	if (L"Player" == GetObj()->GetName())
	{
		// Player =======================================
		// Player 기본 IDLE STAND 상태
		CreateAnimation(L"IDLE_LEFT", pPlayer_stand_left, Vec2(0.f, 0.f), Vec2(64.f, 78.f), Vec2(0.f, 78.f), 0.8f, 3);
		CreateAnimation(L"IDLE_RIGHT", pPlayer_stand_right, Vec2(0.f, 0.f), Vec2(64.f, 78.f), Vec2(0.f, 78.f), 0.8f, 3);

		CreateAnimation(L"SIT_LEFT", pPlayer_sit_left, Vec2(0.f, 0.f), Vec2(64.f, 71.f), Vec2(0.f, 0.f), 0.1f, 1);
		CreateAnimation(L"SIT_RIGHT", pPlayer_sit_right, Vec2(0.f, 0.f), Vec2(64.f, 71.f), Vec2(0.f, 0.f), 0.1f, 1);
		

		// Player WALK 상태
		CreateAnimation(L"WALK_LEFT", pPlayer_walk_left, Vec2(0.f, 0.f), Vec2(65.f, 78.f), Vec2(65.f, 0.f), 0.07f, 4);
		CreateAnimation(L"WALK_RIGHT", pPlayer_walk_right, Vec2(0.f, 0.f), Vec2(65.f, 78.f), Vec2(65.f, 0.f), 0.07f, 4);

	}

	else if (L"future_me" == GetObj()->GetName())
	{
		CreateAnimation(L"FUTUREME_IDLE_LEFT", pfuture_Player_stand_left, Vec2(0.f, 0.f), Vec2(61.f, 74.f), Vec2(0.f, 74.f), 0.8f, 3);
	}


	else if (L"Bird" == GetObj()->GetName())
	{
		// Bird =============================================
		// Bird_LEFT=========================================
		CreateAnimation(L"BIRD_LEFT_TOP", pBird_left, Vec2(0.f, 290.f), Vec2(90.f, 95.f), Vec2(92.f, 0.f), 0.13f, 8);
		CreateAnimation(L"BIRD_LEFT", pBird_left, Vec2(0.f, 190.f), Vec2(90.f, 95.f), Vec2(90.f, 0.f), 0.13f, 8);
		CreateAnimation(L"BIRD_LEFT_BOTTOM", pBird_left, Vec2(0.f, 95.f), Vec2(90.f, 95.f), Vec2(90.f, 0.f), 0.13f, 8);

		// Bird_RIGHT=========================================
		CreateAnimation(L"BIRD_RIGHT_TOP", pBird_right, Vec2(0.f, 290.f), Vec2(90.f, 95.f), Vec2(92.f, 0.f), 0.13f, 8);
		CreateAnimation(L"BIRD_RIGHT", pBird_right, Vec2(0.f, 190.f), Vec2(90.f, 95.f), Vec2(90.f, 0.f), 0.13f, 8);
		CreateAnimation(L"BIRD_RIGHT_BOTTOM", pBird_right, Vec2(0.f, 95.f), Vec2(90.f, 95.f), Vec2(90.f, 0.f), 0.13f, 8);

	}

	else if (L"Effect" == GetObj()->GetName())
	{
		// Effect UI=======================================
		//CreateAnimation(L"Effect_UI", pPlayer_Btn_Effect_UI, Vec2(0.f, 576.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.1f, 5);

		CreateAnimation(L"Effect_UI", pPlayer_Btn_Effect_UI, Vec2(0.f, 0.f), Vec2(1868.75f, 1331.f), Vec2(1868.75f, 0.f), 0.1f, 4);
		CreateAnimation(L"Star", Star, Vec2(0.f, 0.f), Vec2(139.f, 114.f), Vec2(139.f, 0.f), 0.07f, 12);
		CreateAnimation(L"Teleport", Teleport, Vec2(0.f, 0.f), Vec2(136.f, 264.f), Vec2(0.f, 264.f), 0.15f, 5);
		
		
	}

	else if (L"MBTI_UI" == GetObj()->GetName())
	{
		CreateAnimation(L"MBTI_UI", Mbti_UI, Vec2(0.f, 0.f), Vec2(1500.f, 1500.f), Vec2(1500.f, 0.f), 0.12f, 4);
		CreateAnimation(L"MBTI_UI2", Mbti_UI, Vec2(0.f, 1500.f), Vec2(1500.f, 1500.f), Vec2(1500.f, 0.f), 0.12f, 4);
	}

	else if (L"Customer" == GetObj()->GetName())
	{

		// Manager =======================================
		CreateAnimation(L"MANAGER_SAY", pJob_Manager, Vec2(0.f, 0.f), Vec2(51.f, 67.f), Vec2(0.f, 67.f), 0.4f, 4);


		// Customer1 =======================================	
		// SAY 상태
		CreateAnimation(L"CUSTOMER1_SAY", pCustomer1_say, Vec2(0.f, 0.f), Vec2(49.f, 80.f), Vec2(0.f, 80.f), 0.2f, 18);
		// MOVE 상태
		CreateAnimation(L"CUSTOMER1_MOVE_LEFT", pCustomer1_move_left, Vec2(0.f, 0.f), Vec2(49.f, 80.f), Vec2(0.f, 80.f), 0.2f, 4);
		CreateAnimation(L"CUSTOMER1_MOVE_RIGHT", pCustomer1_move_right, Vec2(0.f, 0.f), Vec2(49.f, 80.f), Vec2(0.f, 80.f), 0.2f, 4);
		

		// Customer2 =======================================
		// SAY 상태
		CreateAnimation(L"CUSTOMER2_SAY", pCustomer2_say, Vec2(0.f, 0.f), Vec2(42.f, 70.f), Vec2(0.f, 0.f), 0.1f, 1);
		// MOVE 상태
		CreateAnimation(L"CUSTOMER2_MOVE_LEFT", pCustomer2_move_left, Vec2(0.f, 0.f), Vec2(42.f, 70.f), Vec2(0.f, 70.f), 0.2f, 4);
		CreateAnimation(L"CUSTOMER2_MOVE_RIGHT", pCustomer2_move_right, Vec2(0.f, 0.f), Vec2(42.f, 70.f), Vec2(0.f, 70.f), 0.2f, 4);


		// Customer3 =======================================
		// SAY 상태
		CreateAnimation(L"CUSTOMER3_SAY", pCustomer3_say, Vec2(0.f, 0.f), Vec2(50.f, 74.f), Vec2(0.f, 0.f), 0.1f, 1);
		// MOVE 상태
		CreateAnimation(L"CUSTOMER3_MOVE_LEFT", pCustomer3_move_left, Vec2(0.f, 0.f), Vec2(50.f, 74.f), Vec2(0.f, 74.f), 0.2f, 4);
		CreateAnimation(L"CUSTOMER3_MOVE_RIGHT", pCustomer3_move_right, Vec2(0.f, 0.f), Vec2(50.f, 74.f), Vec2(0.f, 74.f), 0.2f, 4);



		// badCustomer =======================================
		// 32.4 , 31
		CreateAnimation(L"BAD_CUSTOMER_SAY", pbadCustomer, Vec2(0.f, 0.f), Vec2(43.f, 85.f), Vec2(0.f, 85.f), 0.8f, 8);
		
	}
	
	else if (L"unknown_friend" == GetObj()->GetName())
	{
		CreateAnimation(L"UNKNOWN_FRIEND_SIT_LEFT", punknown_friend_1_sit_left, Vec2(0.f, 0.f), Vec2(44.f, 62.f), Vec2(0.f, 0.f), 0.1f, 1);
		CreateAnimation(L"UNKNOWN_FRIEND_SIT_RIGHT", punknown_friend_1_sit_right, Vec2(0.f, 0.f), Vec2(44.f, 62.f), Vec2(0.f, 0.f), 0.1f, 1);
	}
	else if (L"unknown_friend_2" == GetObj()->GetName())
	{
		CreateAnimation(L"UNKNOWN_FRIEND_2_SIT_LEFT", punknown_friend_2_sit_left, Vec2(0.f, 0.f), Vec2(58.f, 65.f), Vec2(0.f, 65.f), 1.f, 3);
		CreateAnimation(L"UNKNOWN_FRIEND_2_SIT_RIGHT", punknown_friend_2_sit_right, Vec2(0.f, 0.f), Vec2(58.f, 65.f), Vec2(0.f, 65.f), 1.f, 3);
	}
	else if (L"unknown_friend_3" == GetObj()->GetName())
	{
		CreateAnimation(L"UNKNOWN_FRIEND_3_SIT_LEFT", punknown_friend_3_sit_left, Vec2(0.f, 0.f), Vec2(52.f, 66.f), Vec2(0.f, 66.f), 1.5f, 3);
		CreateAnimation(L"UNKNOWN_FRIEND_3_SIT_RIGHT", punknown_friend_3_sit_right, Vec2(0.f, 0.f), Vec2(52.f, 66.f), Vec2(0.f, 66.f), 1.5f, 3);
	}
	else if (L"unknown_friend_4" == GetObj()->GetName())
	{
		CreateAnimation(L"UNKNOWN_FRIEND_4_SIT_LEFT", punknown_friend_4_sit_left, Vec2(0.f, 0.f), Vec2(63.f, 76.f), Vec2(0.f, 76.f), 1.3f, 3);
		CreateAnimation(L"UNKNOWN_FRIEND_4_SIT_RIGHT", punknown_friend_4_sit_right, Vec2(0.f, 0.f), Vec2(63.f, 76.f), Vec2(0.f, 76.f), 1.3f, 3);
	}





	else if (L"friend_2" == GetObj()->GetName())
	{
		CreateAnimation(L"FRIEND2_SIT_LEFT", friend_2_sit_left, Vec2(0.f, 0.f), Vec2(45.f, 65.f), Vec2(0.f, 65.f), 1.2f, 3);
		CreateAnimation(L"FRIEND2_SIT_RIGHT", friend_2_sit_right, Vec2(0.f, 0.f), Vec2(45.f, 65.f), Vec2(0.f, 65.f), 1.2f, 3);



		CreateAnimation(L"FRIEND2_IDLE", pfriend_2_idle, Vec2(0.f, 0.f), Vec2(45.f, 72.f), Vec2(0.f, 0.f), 0.1f, 1);


		CreateAnimation(L"FRIEND2_VOMIT_LEFT", pfriend_2_vomit_left, Vec2(0.f, 0.f), Vec2(45.f, 72.f), Vec2(0.f, 72.f), 0.3f, 2);
		CreateAnimation(L"FRIEND2_VOMIT_RIGHT", pfriend_2_vomit_right, Vec2(0.f, 0.f), Vec2(45.f, 72.f), Vec2(0.f, 72.f), 0.3f, 2);



		CreateAnimation(L"FRIEND2_WALK_LEFT", pfriend_2_walk_left, Vec2(0.f, 0.f), Vec2(45.f, 71.5f), Vec2(0.f, 71.5f), 0.15f, 4);
		CreateAnimation(L"FRIEND2_WALK_RIGHT", pfriend_2_walk_right, Vec2(0.f, 0.f), Vec2(45.f, 71.5f), Vec2(0.f, 71.5f), 0.15f, 4);	
	}


	else if (L"toiletMan" == GetObj()->GetName())
	{
		CreateAnimation(L"TOILETMAN_IDLE", toiletManIdle, Vec2(0.f, 0.f), Vec2(47.7f, 69.f), Vec2(47.7f, 0.f), 0.15f, 6);
		CreateAnimation(L"TOILETMAN_WALK", toiletManWalk, Vec2(0.f, 0.f), Vec2(46.5f, 68.f), Vec2(46.5f, 0.f), 0.15f, 4);

	}

	else if (L"smokingGirl" == GetObj()->GetName())
	{
		CreateAnimation(L"SMOKINGGIRL_IDLE", smokingGirlIdle, Vec2(0.f, 0.f), Vec2(54.f, 69.f), Vec2(0.f, 69.f), 0.15f, 3);
		CreateAnimation(L"SMOKINGGIRL_WALK", smokingGirlWalk, Vec2(0.f, 0.f), Vec2(54.f, 68.5f), Vec2(0.f, 68.5f), 0.15f, 4);
	}


	else if (L"Potal" == GetObj()->GetName())
	{
		CreateAnimation(L"POTAL", pPotal, Vec2(0.f, 1440.f), Vec2(480.f, 480.f), Vec2(480.f, 0.f), 0.5f, 4);
	}


	else if (L"mysterious_boy" == GetObj()->GetName())
	{
		CreateAnimation(L"MYSTERIOUS_BOY", mysterious, Vec2(0.f, 0.f), Vec2(145.f, 142.f), Vec2(0.f, 0.f), 1.f, 1);
	}

	else if (L"Bus" == GetObj()->GetName())
	{
		CreateAnimation(L"Bus_updown", pBus, Vec2(0.f, 0.f), Vec2(436.f, 223.f), Vec2(436.f, 0.f), 0.2f, 2);
	}

	else if (L"Taxi" == GetObj()->GetName())
	{
		CreateAnimation(L"Taxi", pTaxi, Vec2(0.f, 0.f), Vec2(134.f, 90.f), Vec2(0.f, 0.f), 0.1f, 1);
	}

	else if (L"CuteBoy" == GetObj()->GetName())
	{
		CreateAnimation(L"CuteBoy_stand", pCuteBoy, Vec2(0.f, 0.f), Vec2(47.f, 69.f), Vec2(0.f, 69.f), 0.2f, 3);
		CreateAnimation(L"CuteBoy_sit", pCuteBoy_sit, Vec2(0.f, 0.f), Vec2(47.f, 62.f), Vec2(0.f, 0.f), 0.1f, 1);
		CreateAnimation(L"CuteBoy_Walk", pCuteBoyWalk, Vec2(0.f, 0.f), Vec2(47.f, 68.5f), Vec2(0.f, 68.5f), 0.07f, 4);
		CreateAnimation(L"CuteBoy_Walk_RIGHT", pCuteBoyWalk_right, Vec2(0.f, 0.f), Vec2(47.f, 68.5f), Vec2(0.f, 68.5f), 0.07f, 4);
	}

	else if (L"OutsideBoy" == GetObj()->GetName())
	{
		CreateAnimation(L"OutsideBoy_STAND", pOutsideBoy_stand, Vec2(0.f, 0.f), Vec2(43.f, 69.f), Vec2(0.f, 69.f), 0.1f, 1);
		CreateAnimation(L"OutsideBoy_WALK_LEFT", pOutsideBoy_walk_left, Vec2(0.f, 0.f), Vec2(43.f, 69.5f), Vec2(0.f, 69.5f), 0.2f, 4);
		CreateAnimation(L"OutsideBoy_WALK_RIGHT", pOutsideBoy_walk_right, Vec2(0.f, 0.f), Vec2(43.f, 69.5f), Vec2(0.f, 69.5f), 0.2f, 4);
		CreateAnimation(L"OutsideBoy_SIT", pOutsideBoy_sit, Vec2(0.f, 0.f), Vec2(43.f, 63.f), Vec2(0.f, 0.f), 0.1f, 1);
		CreateAnimation(L"OutsideBoy_SIT_CRY", pOutsideBoy_sit_cry, Vec2(0.f, 0.f), Vec2(43.f, 63.f), Vec2(0.f, 0.f), 0.1f, 1);
	}


	else if (L"President" == GetObj()->GetName())
	{
		CreateAnimation(L"President_STAND", pPresident_stand_right, Vec2(0.f, 0.f), Vec2(59.34f, 93.f), Vec2(59.34f, 0.f), 0.5f, 3);
		CreateAnimation(L"President_WALK", pPresident_walk_right, Vec2(0.f, 0.f), Vec2(62.f, 87.f), Vec2(62.f, 0.f), 0.2f, 4);
		CreateAnimation(L"President_DANCE", pPresident_dance_right, Vec2(0.f, 0.f), Vec2(83.5f, 107.f), Vec2(83.5f, 0.f), 0.1f, 10);
	}

	else if (L"VicePresident" == GetObj()->GetName())
	{
		//CreateAnimation(L"VicePresident_STAND", pVicePresident_stand_right, Vec2(0.f, 0.f), Vec2(62.f, 77.f), Vec2(0.f, 77.f), 1.5f, 9);
		//CreateAnimation(L"VicePresident_WALK", pVicePresident_walk_right, Vec2(0.f, 0.f), Vec2(62.5f, 72.f), Vec2(62.5f, 0.f), 0.2f, 4);
		//CreateAnimation(L"VicePresident_DANCE", pVicePresident_dance_right, Vec2(0.f, 0.f), Vec2(71.f, 76.5f), Vec2(0.f, 76.5f), 0.1f, 10);
	}
	
	else if (L"Earnest_friend" == GetObj()->GetName())
	{
		CreateAnimation(L"Earnest_man_stand", pEarnest_man_stand, Vec2(0.f, 0.f), Vec2(61.f, 75.f), Vec2(0.f, 75.f), 0.2f, 3);
		CreateAnimation(L"Earnest_man_walk_left", pEarnest_man_walk_left, Vec2(0.f, 0.f), Vec2(57.5f, 75.f), Vec2(57.5f, 0.f), 0.1f, 4);
		CreateAnimation(L"Earnest_man_walk_right", pEarnest_man_walk_right, Vec2(0.f, 0.f), Vec2(57.5f, 75.f), Vec2(57.5f, 0.f), 0.1f, 4);

		CreateAnimation(L"Earnest_man_prone_left", pEarnest_prone_left, Vec2(0.f, 0.f), Vec2(68.f, 46.f), Vec2(0.f, 0.f), 1.f, 1);
	}

	else if (L"Earnest_friend_woman" == GetObj()->GetName())
	{

		CreateAnimation(L"Earnest_woman_stand", pEarnest_woman_stand, Vec2(0.f, 0.f), Vec2(63.f, 75.f), Vec2(0.f, 75.f), 0.2f, 3);
		CreateAnimation(L"Earnest_woman_walk_left", pEarnest_woman_walk_left, Vec2(0.f, 0.f), Vec2(62.5f, 75.f), Vec2(62.5f, 0.f), 0.1f, 4);
		CreateAnimation(L"Earnest_woman_walk_right", pEarnest_woman_walk_right, Vec2(0.f, 0.f), Vec2(62.5f, 75.f), Vec2(62.5f, 0.f), 0.1f, 4);
	}

	else if (L"Earnest_friend_woman2" == GetObj()->GetName())
	{
		CreateAnimation(L"Earnest_woman2_stand", pEarnest_woman2_stand, Vec2(0.f, 0.f), Vec2(61.f, 69.f), Vec2(0.f, 69.f), 0.2f, 3);
		CreateAnimation(L"Earnest_woman2_walk_left", pEarnest_woman2_walk_left, Vec2(0.f, 0.f), Vec2(61.f, 68.5f), Vec2(0.f, 68.5f), 0.1f, 4);
		CreateAnimation(L"Earnest_woman2_walk_right", pEarnest_woman2_walk_right, Vec2(0.f, 0.f), Vec2(61.f, 68.5f), Vec2(0.f, 68.5f), 0.1f, 4);
	}

	else if (L"DateGirl" == GetObj()->GetName())
	{
		CreateAnimation(L"DateGirl_stand", pDategirl_stand_right, Vec2(0.f, 0.f), Vec2(54.f, 70.f), Vec2(0.f, 70.f), 0.2f, 3);
		CreateAnimation(L"DateGirl_walk_left", pDategirl_walk_left, Vec2(0.f, 0.f), Vec2(54.f, 69.6f), Vec2(0.f, 69.6f), 0.15f, 3);
		CreateAnimation(L"DateGirl_walk_right", pDategirl_walk_right, Vec2(0.f, 0.f), Vec2(54.f, 69.6f), Vec2(0.f, 69.6f), 0.15f, 3);
		CreateAnimation(L"DateGirl_sit", pDategirl_sit, Vec2(0.f, 0.f), Vec2(54.f, 66.f), Vec2(0.f, 0.f), 0.1f, 1);
	}

	else if (L"Number" == GetObj()->GetName())
	{
		CreateAnimation(L"Number", pNumber, Vec2(5120.f, 0.f), Vec2(512.f, 512.f), Vec2(-512.f, 0.f), 1.f, 11);
	}

	


	



	
	












	// Animation 저장하기
	// FindAnimation(L"IDLE_FRONT")->Save(L"animation\\player_idle.front.anim");
	// FindAnimation(L"IDLE_LEFT")->Save(L"animation\\player_idle.left.anim");
	// FindAnimation(L"IDLE_RIGHT")->Save(L"animation\\player_idle.right.anim");
	// FindAnimation(L"IDLE_BACK")->Save(L"animation\\player_idle.back.anim");
	// 
	// 
	// FindAnimation(L"WALK_FRONT")->Save(L"animation\\player_walk.front.anim");
	// FindAnimation(L"WALK_LEFT")->Save(L"animation\\player_walk.left.anim");
	// FindAnimation(L"WALK_RIGHT")->Save(L"animation\\player_walk.right.anim");
	// FindAnimation(L"WALK_BACK")->Save(L"animation\\player_walk.back.anim");


}
