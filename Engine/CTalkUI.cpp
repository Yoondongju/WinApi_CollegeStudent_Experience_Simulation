#include "pch.h"
#include "CTalkUI.h"

#include "CScene.h"
#include "CSceneMgr.h"
#include "CTimeMgr.h"

#include "CCore.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CSound.h"
#include "CResMgr.h"

#include "CBtnUI.h"

#include "AI.h"
#include "CCodeModeState.h"

#include <chrono>
#include <thread>

bool CTalkUI::m_bStart = false;

static Conversation g_LastConversationInfo;

CTalkUI::CTalkUI()
	:CUI(false)
	, m_PreTextcount(0)
	, m_CurTextcount(0)
	, m_PreSceneCount(SCENE_TYPE::LODING)
	, m_CurSceneCount(SCENE_TYPE::LODING)
	, m_MaxTextcount(6)
	, m_pCurScene(nullptr)
	, m_fTime(0.f)
	, m_itextSize(1)
	, m_isUIRecreated(false)
	, m_isChoiceCreated(false)
	, m_TalkingOwner(nullptr)
	
{

	BLENDFUNCTION bf = GetBf();
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 200;

	SetBf(bf);




	
	m_PreConversation = g_LastConversationInfo;
	


	m_pCurScene = CSceneMgr::GetInst()->GetCurScene();  // 현재 씬에 맞게 대화창 텍스쳐 세팅

	m_pCurScene->SetMainTalkUI(this);  // Scene마다 TalkUI는 무조건 1개이기때문 세팅

	// Talk UI는 Scene에 따라 말풍선형식의 자기 머리위에 떠있는 대화 UI가 될수도있고
	// Center 하단에 배치하는 사각형형식의 대화 UI가 될수도 있음  이건 추후 생각해 봐야할 문제

	if (L"Loding2 Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::LODING_2;

		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n\n\n            나와 정통이는 초등학교때 부터 항상 같이 붙어다니던 죽마고우이다.\n\n            심지어 나와 정통이는 집이 가까워 등하교도 같이하며 학창시절을 함께했다.\n\n            하지만 고등학교때 정통이는 꿈을 쫓아 대학교에 진학하였고,\n\n            나는 형편상 바로 돈을 벌어야했기에 대학교 진학을 포기하고 취업준비를하였다.\n\n            일에 치어살고있던 나는 오랜만에 정통이가 보고싶어 연락을 하게되었다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n\n\n            나: 여보세요? 정통아 오늘 뭐해?\n\n            김정통: 나 오늘 학교 친구들이랑 약속있어!\n\n            나: 흠.. 그럼 이번 주말에는??\n\n            김정통: 주말에는 과 MT가서 안될것같은데..?\n\n            나: 아..그래..? MT가서 뭐하는데?", ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n\n\n            김정통: 음.. 뭐 과 선배,동기랑 같이 고기도먹고\n            술도 먹으면서 같이 시간보내는거지 뭐..!\n\n            나: 와.. 재밌겠다 나도 MT 가보고싶다.. 난 요즘 회사에서 일배우느라 정신이없는데..\n\n            김정통: 멋있는데?? 벌써부터 돈도벌고 좋겠다! 난 공부도해야해..\n            알바도 해야해.. 이래저래 걱정이 많아",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n\n\n            나: 그런가.. 난 너가 부러운데.. 나도 대학교 다니고싶다!\n\n            김정통: 아니면 지금이라도 대학교 입학해보는건 어때?\n\n            나: 아냐.. 내 형편에는 무리일것같아, 그냥 회사나 열심히 다녀야지..\n\n            김정통: 그래그래 나중에 밥 한번 먹자 한신아!\n\n            나: (애써 괜찮은척하며) 그래 나중에 너 시간되면 밥말고 술한잔하자! 내가 쏠게! ㅎㅎ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n\n\n            (전화가 종료되고 자취방에 홀로 누워있는 나는 공허함에 빠진다.)\n\n            나: 하.. 나도 정통이처럼 대학교를 갔으면 어땠을까..?\n\n            나: (바닥이 꺼질듯 한숨을 쉬며)\n\n            후.. 졸리다.....", ButtonType::NONE }));


		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n\n\n\n\n\n\n																						당신의 MBTI를 선택하세요!",ButtonType::NONE,ButtonType::FINAL_END }));

	

	}



	else if (L"Lobby Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::STAGE_LOBBY;

		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"여긴 어디지??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"어둡다..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"분명 무슨소리가 들렸는데..\n",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"왼쪽에\n낡은 책자가 있는데?",ButtonType::NONE ,ButtonType::FINAL_END}));





		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: 안녕?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 누구..세요?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(갑자기 낯선이가 자신의 이야기를 시작한다.)" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: 난 어릴때,사람들과 어울리는 게 어려웠어." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n???: 하지만 사람들과 조금씩 어울려보려고 노력했지." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: 상대방과 대화할 땐 공감대를 형성하고,긍정적인 사고를 통해\n에너지를 주고 받았어." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n???: 많은 사람들과 감정을 주고 받다보니 후엔,\n 나를 통해 밝아지는사람도 생기게 됐어." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n나: (뭔가..낯이 익은 느낌인데...) " ,ButtonType::NONE}));  // 이 시점에서 화면이 어두워지다 다시 밝아지고 미래의 나 자신을 마주한다.
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n???: 난 부족했던 사회성을 기르기위해 사람들에게 먼저 다가가보기도하고,\n어울리기 힘들었던 것을 극복하고자 열심히 살았어." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: 이를 계기로 난 다른 사람들에게 사랑 받는 사람이 됐고,\n그런 내모습에 만족하고 살아가고있어!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n나: ....?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n나: 설마 당신은?!" ,ButtonType::NONE ,ButtonType::FINAL_END}));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: 안녕?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 누구..세요?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(갑자기 낯선이가 자신의 이야기를 시작한다.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: 나 자신을 믿는 건 어려운 일이었어.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n???: 하지만 자주 어릴 적의 나에게로 돌아가서\n격려의 말을 건네주곤 했어." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: 자존감을 키우기 위해 노력하다보니\n내 안에 무한한 자신감이 생겨지더라" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n???: 자존감이 높아지고 어떤 어려움도 극복할수 있게되었어" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n나: (뭔가..낯이 익은 느낌인데...) " ,ButtonType::NONE }));  // 이 시점에서 화면이 어두워지다 다시 밝아지고 미래의 나 자신을 마주한다.
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n???: 난 부족했던 자존감을 기르기위해 먼저 나서서 용기를 얻는방법을 배웠고,\n그런 당돌한 내모습에 사람들은 나를 따랐어" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: 이를 계기로 난 다른 사람들앞에서 용기낼수 있었고,\n그런 내모습에 만족하고 살아가고있어!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n나: ....?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n나: 설마 당신은?!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: 안녕?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 누구..세요?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(갑자기 낯선이가 자신의 이야기를 시작한다.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: 문제 앞에서 난 얼굴을 가리지 않았어." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n???: 어려운 상화이라도 한 발짝 물러서서 생각하고 행동했지." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: 문제를 해결하면서 더 나은 해결책을 찾고,\n자신감을 얻게 되었어." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n???: 어떤 문제든 해결할 수 있다는 믿음을 갖고,\n앞으로 나아가서 좋은 결과를 얻게되었어!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n나: (뭔가..낯이 익은 느낌인데...) " ,ButtonType::NONE }));  // 이 시점에서 화면이 어두워지다 다시 밝아지고 미래의 나 자신을 마주한다.
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n???: 난 부족했던 문제해결능력을 문제에 맞서고,\n그런 모습에 어려운 상황에 놓여도 항상 해결하였어" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: 이를 계기로 난 내자신을 더 믿게되었고,\n그런 내모습에 만족하고 살아가고있어!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n나: ....?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n나: 설마 당신은?!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: 안녕?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 누구..세요?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(갑자기 낯선이가 자신의 이야기를 시작한다.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: 나는 언제나 내 책임을 질 준비가 되어 있었어." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n???: 다른 사람들이 의지할 수 있는 존재가 되고싶어서,\n언제나 책임을 져왔지." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: 책임감을 가지고 행동하다보니,\n나 자신도 믿음직한 사람이 되었어." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n???: 자신감 있게 책임을 질 수 있다면\n어떤 여러움도 극복할 수 있을거야." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n나: (뭔가..낯이 익은 느낌인데...) " ,ButtonType::NONE }));  // 이 시점에서 화면이 어두워지다 다시 밝아지고 미래의 나 자신을 마주한다.
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n???: 난 부족했던 책임감을 기르기위해 주어진 상황에 항상 최선을 다하고,\n이를 외면하지 않고 책임지려 노력했어." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: 이를 계기로 난 다른 사람들에게 신뢰를 얻었고,\n그런 내모습에 만족하고 살아가고있어!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n나: ....?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n나: 설마 당신은?!" ,ButtonType::NONE ,ButtonType::FINAL_END }));








		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: 안녕?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 누구..세요?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(갑자기 낯선이가 자신의 이야기를 시작한다.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: 나는 대학생활을 하면서 항상 어려웠어.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: 네??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: 사람들을 만나도 사고치고 그들이랑 어울리는게 힘들었지.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n나: ....괜찮아요..! 살다보면 그럴수도 있죠!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n???: 난 인생에서 성공할수 없는 사람인가봐.." ,ButtonType::NONE }));  
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n나: (위로하며) 아니에요!! 당신은 충분히 가치있는사람이에요!\n무슨말을 그렇게해요!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: 고..고마워..그래도..나 사회성만큼은 충분히 기른것같아" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n나: 그래요!! 기죽지마요!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n나: 근데 누구..시죠?" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: 안녕?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 누구..세요?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(갑자기 낯선이가 자신의 이야기를 시작한다.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: 나는 대학생활을 하면서 항상 어려웠어.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: 네??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: 사람들을 만나도 사고치고 그들이랑 어울리는게 힘들었지.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n나: ....괜찮아요..! 살다보면 그럴수도 있죠!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n???: 난 인생에서 성공할수 없는 사람인가봐.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n나: (위로하며) 아니에요!! 당신은 충분히 가치있는사람이에요!\n무슨말을 그렇게해요!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: 고..고마워..그래도..나 자존감만큼은 충분히 기른것같아" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n나: 그래요!! 기죽지마요!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n나: 근데 누구..시죠?" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: 안녕?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 누구..세요?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(갑자기 낯선이가 자신의 이야기를 시작한다.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: 나는 대학생활을 하면서 항상 어려웠어.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: 네??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: 사람들을 만나도 사고치고 그들이랑 어울리는게 힘들었지.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n나: ....괜찮아요..! 살다보면 그럴수도 있죠!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n???: 난 인생에서 성공할수 없는 사람인가봐.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n나: (위로하며) 아니에요!! 당신은 충분히 가치있는사람이에요!\n무슨말을 그렇게해요!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: 고..고마워..그래도..나 문제해결능력만큼은 충분히 기른것같아" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n나: 그래요!! 기죽지마요!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n나: 근데 누구..시죠?" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: 안녕?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 누구..세요?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(갑자기 낯선이가 자신의 이야기를 시작한다.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: 나는 대학생활을 하면서 항상 어려웠어.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: 네??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: 사람들을 만나도 사고치고 그들이랑 어울리는게 힘들었지.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n나: ....괜찮아요..! 살다보면 그럴수도 있죠!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n???: 난 인생에서 성공할수 없는 사람인가봐.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n나: (위로하며) 아니에요!! 당신은 충분히 가치있는사람이에요!\n무슨말을 그렇게해요!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: 고..고마워..그래도..나 책임감만큼은 충분히 기른것같아" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n나: 그래요!! 기죽지마요!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n나: 근데 누구..시죠?" ,ButtonType::NONE,ButtonType::FINAL_END }));





		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n미래의 나: 안녕 한신아!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: ..? 미래의 나..? 이게 대체.. 무슨상황이지?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n미래의 나: 넌 지금도 훌륭하게 성장하고 있는것같네" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n미래의 나: 현재에 충실하고 지금처럼 열심히 살면\n그게바로 의미있는 삶이야" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n미래의 나: 지금 이 짧은 순간순간을 특별하게 여기며 행복하게 살아!" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n					 (알수없는 기분에 몸이 풀린다...)" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n미래의 나: 안녕 한신아..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: ..? 미래의 나..? 이게 대체.. 무슨상황이지?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n미래의 나: 너 덕분에 힘이나,, 위로해줘서 고마워!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n미래의 나: 난 너에게 앞으로 일어날일에\n조금더 신중하고 올바른 결정을 했으면해," ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n미래의 나: 난 비록 이미 시간이 흘렀지만..\n넌 아직 기회가 많잖아? 너의 그 장점을 살려!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n					 (알수없는 기분에 몸이 풀린다...)" ,ButtonType::NONE,ButtonType::FINAL_END }));
	}

	
	else if (L"Job Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::JOB;

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4594, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n여긴 편의점 이잖아?!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4594, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n(매니저한테 말을 걸어야\n할것같다)",ButtonType::NONE ,ButtonType::FINAL_END }));

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n매니저: 안녕 너가 이번에 오후타임으로 들어온 한신이 구나!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 네, 매니저님 안녕하세요!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n매니저: 편의점 알바는 처음이니?",ButtonType::NONE , ButtonType::Choice }));



		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice })); // 버튼 삽입 (네 / 아니오) 
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"네.. 처음이에요!",ButtonType::Choice })); // 지식 소량증가
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 2,L"아뇨 알바 해본적있어요!",ButtonType::Choice }));
		



		// 네 선택시///
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n매니저: 그럼 일단 각 진열대마다 어떤 상품이 있는지 살펴보자.\n 왼쪽 사이드부터 밀키트, 삼각김밥, 즉석식품, 유제품, 중간엔\n 아이스크림 및,냉동식품, 저기밑엔 과자, 라면,통조림, 비조리식품들이 있고,\n 마지막에 있는 진열대에는 음료 및 생필품 등이 있어.",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n매니저: 말하니까 많아보이지?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: 네...... 너무 많아요....",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n매니저: 괜찮아 그래도 위치는 대략적인 것만 알면 돼.",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: 네 ㅎㅎ ....",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n매니저: 포스기 찍는 방법은 이렇게 하는거고 자율 계산대는\n 손님이 많을때 이용하게 하면 돼.\n 그 외의 것들은 노트에 자세하게 적혀있어!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n나: 아하 감사합니다!",ButtonType::YES ,ButtonType::END}));
		/////////////



		// 아니오 선택시 //
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n매니저: 오~ 믿음직스러운데~ 그런데 여기는 다른곳이랑 조금 다를걸~\n",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 에,, 어떤점이요,,? ",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n매니저: 그건 너가 일하면서 직접 경험해봐~~",ButtonType::NO ,ButtonType::END }));
		/////////////////
	



		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n매니저: 그리고 손님들이 상품 구매하고 상품이 진열대에 없으면\n뒤에 창고에 가서 채워 놓기만 해주면돼. ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: 아하 알겠습니다 ! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n매니저: 대학생이라 용돈 때문에 알바하는거야? ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n나: 용돈도 벌고 경험도 쌓고 싶어서 하는거에요. ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n매니저: 대단하네 ~!! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n나: 헤헤.. ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n매니저: 근데 우리 편의점은 고객들에게\n최대한의 서비스를 제공해줘야 해서 손님들의 니즈를 맞춰줘야 돼. ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n나: 필요한 게 있으면 먼저 가서 서비스를 제공하라고 말씀하시는거죠? ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n매니저: 응 맞아 ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"\n나: ( 무슨 편의점이 이래... ) ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 13,L"\n매니저: 혹시 궁금한 게 더 있으면 언제든지 말해도 돼! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 14,L"\n매니저: 아 오후타임은 물품 들어오는게 없어서\n재고정리는 안 해도 돼! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 15,L"\n나: 알겠습니다 ! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 16,L"\n매니저: 그럼 잘 부탁할게 ! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 17,L"\n나: 네~ ",ButtonType::NONE , ButtonType::FINAL_END }));
		//////// 매니저와 대화 종료//////////////





		// <빨간머리 손님과 대화> //
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: 안녕하세요 혹시 필요한 거 있으세요?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n손님1: 육개장 사려고 하는데 육개장 밀키트는 없나요?",ButtonType::NONE ,ButtonType::Choice,ButtonType::NONE,true }));
										

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice })); // 찾아본다, 안찾아본다
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true , 0.f}, 1,L"(귀찮아 하며) 네 손님 여기 진열대에 없으면 없는거에요..",ButtonType::Choice })); 
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true , 0.5f}, 2,L"(대충 한 번 두리번거리고 온다) 찾아 봤는데 없네요~",ButtonType::Choice }));  
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true,1.f}, 3,L"(매니저한테 전화를 하며) 잠시만요~",ButtonType::Choice })); 
										


		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n손님1: 아 그럼 어쩔수 없죠~ 감사합니다!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 네~", ButtonType::YES , ButtonType::FINAL_END }));
												   
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n손님1: 아 그럼 어쩔수 없죠~ 감사합니다!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 네~", ButtonType::NO , ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"             (통화가 연결된다)",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 매니저님 혹시 육개장 밀키트 어디 있나요?",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n매니저: 창고에 라면 바로 옆에 있어!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n나: 알겠습니다~ 감사해요 !!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n매니저: 응 ~ㅎㅎ",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: (육개장 밀키트를 갖고 와서)\n 여기 있습니다 손님!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n손님1: 감사합니다~", ButtonType::FIGHT}));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n나: 네~", ButtonType::FIGHT , ButtonType::FINAL_END }));



		// <빨간머리 손님과 대화 종료> //



		// <민트머리 손님과 대화> //
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: 안녕하세요 혹시 필요한거 있으세요?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n손님2: 와인을 사려고 하는데 혹시 여기 있는게 전부인가요?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: 네, 손님 여기있는게 전부에요",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n손님2: 그럼 혹시 와인 오프너도 같이 팔고 있나요?",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true }));
												   

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice })); // 매니저에게 물어본다, 그냥 혼자 대처한다
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true,1.f}, 1,L"(매니저한테 전화를 걸며) 저희가 오프너는 따로판매하고 있지 않은것 같은데.. 잠시만요...",ButtonType::Choice })); // 책임감 증가
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true, 0.f}, 2,L"(웃긴 영상을 보며) 거기 없으면 없는 거에요~",ButtonType::Choice })); // 책임감 감소
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true,0.5f}, 3,L"(진열장을 찾아보며) 오프너는 따로 없는 거 같네요",ButtonType::Choice })); // 책임감 감소


												   
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: 여보세요? ",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n매니저: 응? 무슨 일이야?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: 와인 오프너를 찾고 계셔서 혹시 어디있는지 알려주실수 있나요?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n매니저: 와인 진열장 바로 아래 서랍에 있어!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: 넵!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n            (전화를 끊는다)",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n나: 여기 있습니다 손님!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n손님2: 감사합니다~",ButtonType::YES ,ButtonType::FINAL_END}));
												   
												   
										   

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n손님2: 아 그래요? 알겠습니다..",ButtonType::NO ,ButtonType::FINAL_END}));

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n손님2: 아 그래요? 알겠습니다..",ButtonType::FIGHT ,ButtonType::FINAL_END }));

		// <민트머리 손님과 대화 종료> //






		// <노란머리 손님과 대화> //
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: 혹시 찾고 계시는 게 있나요?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n의문의 소년: .....",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: ...?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n의문의 소년: (중얼거리며...)\n친구가 있으면 좋겠...",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: (귀를 기울이며)\n 네 ??",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n의문의 소년: 류타쿤.. 나와 친구가 되어주겠어...?",ButtonType::NONE ,ButtonType::Choice}));


		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ...(뭐라고 하는거지..?)",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.5f}, 1,L"(좀 더 자세히 물어본다.)",ButtonType::Choice })); // 문제 해결능력 증가
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true, 0.f}, 2,L"(무시하고 카운터로 간다.)",ButtonType::Choice })); // 문제 해결능력 감소 
		


		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: (대충 류타라고 적힌 걸 보여주며..)\n혹시 류타가 이거 맞나요?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n의문의 소년: 오옷..! 류타군 드디어 나한테 관심을 가져주는거냐능...!!!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: 하하...",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n의문의 소년: 꺄~~ 카와이! 당신은 내 생명의 은인이라능!!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: 하하..",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: (계산대로 돌아가며..)\n뭐지 저 사람은..?",ButtonType::YES ,ButtonType::FINAL_END}));


		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n의문의 소년: (고개를 떨구며)\n 역시 아무도 나한테 관심을 안 가져주는거냐능..",ButtonType::NO}));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: (당황해하며)\n ...?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n             (의문의 남자가 울상인 표정으로 가게를 나간다)",ButtonType::NO ,ButtonType::FINAL_END}));
		// <노란머리 손님과 대화 종료> //





		// <진상 손님과 대화> //
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: 어서오세요!",ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n진상: (흥얼흥얼...)\n나는 개똥벌레 친구가..(딸국!..) 없네~",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (또야..?)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n진상: 소주 하나 가져와! (딸꾹!..)",ButtonType::NONE ,ButtonType::Choice , ButtonType::NONE, true}));

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ...",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true, 0.5f}, 1,L"(눈치보며..) 저기 좌측 코너에 있습니다",ButtonType::Choice })); // YES
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true,1.f}, 2,L"(생긋 웃으며..) 제가 가져다 드릴게요.",ButtonType::Choice }));  // NO  책임감증가,사회성증가
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true, 0.f}, 3,L"(급발진 하며..) 니가 가져와!! 어딜 가져와라 마라야!!",ButtonType::Choice })); // 싸우기 // 사회성감소
												   

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n진상: 내가 직접 가져오라고 !?\n알바면 네가 직접 가져와야지!! 어린놈이..(딸꾹!)",ButtonType::YES ,ButtonType::Choice ,ButtonType::NONE, true }));




		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n진상: 너가 좋아하는 걸로 아무거나 가져와!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: (생긋 웃으며)\n네 손님~",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n진상: (흥얼흥얼.. 딸꾹!)",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n나: 삑! 1800원입니다!\n오늘 기분 좋은 일 있으셨나봐요!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n진상: (눈살을 찌푸리며...)\n뭐라고 !?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: (당황해하며..)\n아..아니에요..",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n진상: (카드를 던지며...)\n계산해",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n나: ... 계산되었습니다 안녕히가세요..",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n진상: (봉투를 집어들며..)\n나는 개똥벌레 친구가..(딸꾹!)..없네~",ButtonType::NO,ButtonType::FINAL_END }));






		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n진상: (얼굴을 붉히며..)\n뭐..!? 뭐라고?",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n진상: (삿대질하며..)\n너 손님한테 그따위로 말하는 게 맞아?",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (째려보며...)\n당신이야말로 술먹고 들어와서 어딜 행패짓이야!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n진상: (화를내며)\n너 가만히 있어 여기 사장 나오라고 해! 당장!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: (신속하게 112를 누르며..)\n경찰서죠? 여기 행복 편의점인데요.\n술취한 취객이 난동을 피우고 있어서요,",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n진상: (당황해하며..)\n야..야..! 이게 어디서 신고를..!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n진상: (급하게 나가며..)\n내가 너 다음에 오기만 하면 가만안둬!!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n나: 하....",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n나: 오늘 첫 출근인데 매니저님한테 뭐라하지...",ButtonType::FIGHT ,ButtonType::FINAL_END}));
		// <진상 손님과 대화 종료> //





		m_MaxTextcount = 28;
	}

	else if (L"Drinking Scene" == m_pCurScene->GetName() || L"Toilet Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::DRINKING;

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n(뚜르르르르....)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n(뚜르르르르....)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n(딸깍)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n친구2: 여보세요?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: 응 왜 전화했어 ?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n친구2: 오늘 개강총회잖아! 학교앞에서 술먹는데 너도 올래?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n나: 아아! 맞다! 나도 가도되려나..?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n친구2: 애들이 너랑 친해지고 싶다고 하니까\n 수업끝나면 양산골로 와!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n나: 알겠어!!",ButtonType::NONE ,ButtonType::FINAL_END }));
		


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: 누구니 넌..?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n         (자리를 잘못 찾은듯 하다..)",ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n(...웅성웅성...)",ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n친구2: (친구들이 환영해준다)\n어! 왔다!",ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n친구4: 너가 한신이 구나! 이야기 많이 들었어!!",ButtonType::NONE }));

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n나: 안녕 난 김한신이야~! 오늘 보니까 너무 좋다 얘들아~\n  친해지고싶었어!ㅎㅎ",ButtonType::NONE })); // MBTI  E 사용자
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: 안녕 반가워.. 애들아.. 만나서,, ㅂ,,반가워!",ButtonType::NONE })); // MBTI  I 사용자
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n             (각자 자기 소개를 하며 얘기한다.)",ButtonType::NONE, ButtonType::Choice}));
		

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::ALCOHOL_TOLERANCE},true, 1.f}, 1,L"(내가 주도해서 친구들이랑 가까워져야겠다..!)",ButtonType::Choice })); // YES  // 사회성,리더쉽,자존감 중 어떤게 올라야하는지
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::ALCOHOL_TOLERANCE},true ,0.f}, 2,L"(낯가려서 말을 잘 못하겠는데..)",ButtonType::Choice })); // NO

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n            (어색하니 주변 친구들의 이야기나 들어보자..)",ButtonType::NO, ButtonType::FINAL_END,})); // NO


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나:(친구들 눈치를 살피며)\n눈치게임 1 !!!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n친구1: 아 뭐야~",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n친구1,친구2: (동시에)\n 2 !!!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n친구들: 아~~ 걸렸네~~ 러브샷 ! 러브샷 !",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n친구1,친구2: 한잔하자!! ",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: 얘들아 ~",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: (신나하며)\n 하늘에서 내려온 토끼가 하는말 !!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n친구들 무리: 움치치!! 움치치!!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n친구들 무리: 바니 바니!!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: 당근 당근!!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n친구1: 아~ 걸렸네 !! 한잔해",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: (꿀꺽꿀꺽..)\nㅎ..하 취한다..",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n        (...술이 들어갈 수록 점점 정신이 몽롱해진다...)",ButtonType::YES, ButtonType::FINAL_END})); // YES // 화장실로 장면 전환


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n        (비틀거리며 친구2와 화장실로 들어온다)",ButtonType::NONE ,ButtonType::NONE,ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n친구2: (딸국질을 하며..)\n오늘.. 딸꾹!.. 어땠..어?",ButtonType::NONE ,ButtonType::NONE,ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (눈이 풀린 상태로..)\n너무...조아써...",ButtonType::NONE ,ButtonType::NONE,ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n친구2: (소변을 보며..)\n 영 ~~ 차!",ButtonType::NONE ,ButtonType::NONE,ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: (친구와 웃으며 장난을 친다)\nㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ  하지마 더러워!!!",ButtonType::NONE ,ButtonType::NONE,ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n        (친구와 장난을 치며 소변을 보는 중에 갑자기 누가 들어온다)",ButtonType::NONE ,ButtonType::FINAL_END,ButtonType::YES })); // YES
		//////////////////////////////// 화장실 남자 들어옴////////////////////////////////////////////////////////////////////////////////////


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n남자: 야 장난치지 말고 볼 일 다 봤으면 나와 급하니까",ButtonType::NONE ,ButtonType::NONE})); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n친구2: (뒤를 돌아보며)\n 아아 .. 넵..",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE , true })); // YES

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true, 0.f}, 1,L"(상대방을 째려보며..) 니가 뭔데 명령이야?!",ButtonType::Choice  })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true, 1.f}, 2,L"(친구를 보며 생각한다) 친구도 있으니까 참자..",ButtonType::Choice })); // NO
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true, 1.f}, 3,L"(다가오는 남자를 보며) 이 사람도 취했네",ButtonType::Choice })); // FIGHT

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n남자: (갑자기 흥분을하며)\n 뭐 이 자식아?",ButtonType::YES}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 초면에 어디서 반말이야 이 자식아",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n남자: (가까이 다가오며)\n 죽고 싶어?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n(친구2가 상황이 이상해짐을 느낀다)",ButtonType::YES  }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n(양 쪽의 친구들이 서로 말리며 상황이	 종료된다)",ButtonType::YES,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n(뭐지..? 술 때문에 감정 컨트롤이 안 된다...)",ButtonType::NO  }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n남자: (갑자기 흥분을하며)\n 뭐 이 자식아?",ButtonType::NO  }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: 초면에 어디서 반말이야 이 자식아",ButtonType::NO  }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n남자: (가까이 다가오며)\n 죽고 싶어?",ButtonType::NO  }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n(친구2가 상황이 이상해짐을 느낀다)",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n(양 쪽의 친구들이 서로 말리며 상황이	 종료된다)",ButtonType::NO,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: (걱정하는 말투로)\n많이 취하셨네요",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n남자: (손가락 욕을 하며)\n엿이나 먹어",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (...?)",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n친구2: (...?)",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n친구2: (손짓하며)\n야야,, 그냥 피하자",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: (한숨을 쉬며)\n그래..;;",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n남자: (큰 목소리로)\n내가 예전엔 말이야!!..!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n          (혼잣말 하는 남자를 뒤로하고 자리를 피한다)",ButtonType::FIGHT ,ButtonType::FINAL_END }));

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n          (웅성웅성..)",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n          (친구들이 서로의 이상형 이야기를한다)",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n친구2: (눈치를 살피며)\n 너 연애 중이야?",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n나: (옅은 웃음을 띄우며)\n 아니..",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n친구1: (친구2를 건들며)\n 그런 걸 왜 물어봐",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: (호탕하게 웃으며)\n 물어봐도 돼 ~ㅎㅎ",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n친구2: 넌 이상형이 어떻게 돼?",ButtonType::NONE,ButtonType::Choice,ButtonType::NO }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true, 1.f}, 1,L"얼굴이 크면서 우람한 여자! ",ButtonType::Choice, ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"얼굴이 작으면서 외소한 여자! ",ButtonType::Choice, ButtonType::NONE,ButtonType::NO }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n친구들: (의아해하는 표정으로)\n 장난치지마~~ 솔직하게!!",ButtonType::YES ,ButtonType::Choice ,ButtonType::NO }));

		
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ㅅ..사실 귀여운 스타일을 좋아해!",ButtonType::NO  ,ButtonType::NONE ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n친구들: (공감하며)\n 역시 너도 남자구나 ㅎㅎ",ButtonType::NO  ,ButtonType::NONE ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n         (친구들과 잔잔하게 대화를 하며 밤이 깊어진다)",ButtonType::NO ,ButtonType::FINAL_END ,ButtonType::NO })); // 화장실로 화면전환




		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n         (친구와 화장실로 들어온다)",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n친구2: (딸국질을 하며..)\n 오늘.. 어땠..어?",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: 너무 재미있었어 ㅎㅎ 근데 너 괜찮아..?",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n친구2: (속이 울렁거린다..)\n 나 술고..래야..!",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n        (갑자기 변기를 보며 멍을 때린다) ",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: ??",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n나: (걱정하며)\n 왜그래..?",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n친구2: (멍때리며..)\n 아니야 괜찮.....",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n친구2: (입을 틀어막으며 변기로 달려간다..)\n 우욱!!!!",ButtonType::NONE ,ButtonType::FINAL_END,ButtonType::NO }));



		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: (걱정하며)\n 왜그래..?",ButtonType::NONE ,ButtonType::Choice,ButtonType::NONE,true}));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::AMOUNT_SOCIABILIT},true, 0.5f}, 1,L"(눈살을 찌푸리며) 그러게 조금만먹지..",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::AMOUNT_SOCIABILIT},true, 1.f}, 2,L"(등을 토닥여주며) 그러게 조금만먹지..",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::AMOUNT_SOCIABILIT},true, 0.f}, 3,L"(더러워하는 표정으로 자리를 피하며) 으.. 휴지가져다 줄게..",ButtonType::Choice }));



		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n친구2: (눈물을 글썽이며)\n 미안.. 너무 많이 마셨나봐..",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 괜찮아 그럴 수도 있지~",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: 자리로 가서 물 좀 마시자",ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::NONE,false,true }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n친구2: (눈물을 글썽이며)\n 미안.. 너무 많이 마셨나봐..",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 괜찮아 그럴 수도 있지~",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: 자리로 가서 물 좀 마시자",ButtonType::NO ,ButtonType::FINAL_END  ,ButtonType::NONE,false,true }));




		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n친구2: (눈물을 글썽이며)\n 미안해..",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n친구2: (눈치를 살피며)\n 내가 술을 처음 마셔봐서 다음부턴 조절할게..",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (한숨을 쉬며)\n 아니야 대충 정리하고 나와\n 먼저 가 있을게",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n친구2: (눈치를 살피며)\n 미안해.. 얼굴만 씻고 갈게..",ButtonType::FIGHT ,ButtonType::FINAL_END  ,ButtonType::NONE,false,true }));
		




		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		// 내가 술에 얼마나 취했냐에 따라 다르게 표현해야함
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n친구3: 화장실에서 무슨일 있었어?\n 왜이렇게 안와 ㅋㅋㅋ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 별일 아냐 ㅋㅋㅋ 어후 힘들다..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n친구3: 밖에 바람이나 쐬고와 그럼~",ButtonType::NONE ,ButtonType::Choice}));

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"(어지러웠는데 마침 나가고싶었어)",ButtonType::Choice}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(아.. 귀찮은데 그냥 여기있을까..)",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: 나 그럼 잠깐 다녀올게!",ButtonType::YES ,ButtonType::FINAL_END}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: 아냐 술도 깰겸 나갔다오자!",ButtonType::NO ,ButtonType::FINAL_END}));





		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n친구들: 어 왔어 ?\n 한참 기다렸잖아~",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 응,,ㅎㅎ 너네들 언제 갈거야?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n친구들: 이제 슬슬가야지~ 오늘 재밌었다 ㅎㅎ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n나: 그래그래 나도 이제가야겠다\n 우리 다음에 또 만나서 놀자!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n             (친구들과 인사하며 술자리가 종료된다)",ButtonType::NONE ,ButtonType::FINAL_END , ButtonType::NONE ,false,true }));


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	else if (L"Smoking Scene" == m_pCurScene->GetName())
	{
		// case  1  ( 술취함 )
		m_CurSceneCount = SCENE_TYPE::SMOKING;

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n          (시원한 공기가 달궈진 몸을 식힌다)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: (혼자 베시시 웃으며 혼잣말로 중얼거리며)\n이게 대학 생활인가.. 어지러운데 기분은 좋네 ㅎㅎ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (밤 하늘을 바라보다 바닥으로 머리를 숙이며)\n후 ...",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n        (술집 입구에서 이상형에 가까운 여자가 나온다)",ButtonType::NONE ,ButtonType::FINAL_END }));

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: (눈을 비비며 벌어진 입을 닫는다)\n!!?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n여자: (핸드폰을 만지며 담배에 불을 붙인다)\n쓰읍... 후...",ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.5f}, 1,L"일단 아무 생각 없이 다가간다.",ButtonType::Choice }));  // YES
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME},true,1.f}, 2,L"라이터를 빌리는척 다가간다.",ButtonType::Choice }));    // NO




		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: (성큼성큼 다가가 앞에 선다)\n ...",ButtonType::YES}));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n여자: (흠칫 놀라는 표정으로)\n누구세요..?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (베시시 웃으며) 예뻐요 그쪽!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n여자: ...?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n여자: 감사합니다..",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: (눈을 지그시 쳐다보며 흐린 발음으로)\n혹시..남자친구..있어요..?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n여자: (당황해하는 표정으로)\n아.. 네.. 저 남자친구 있어요",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n나: (실망한 표정으로)아.. 알겠습니다..",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n여자: 네...",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n        (급하게 전화하는 척을 하며 여자가 시야에서 사라진다)",ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::NONE,false,true }));
		// (자존감 하락)




		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: (성금성큼 다가간다)\n 저...저기요!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n여자: (이어폰을 빼며)\n네..?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (휘청거리며)\n라이터 좀 빌려주실래요?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n여자: (잠시 머뭇하다가 라이터를 건네준며)\n아..네..여기요",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: (큰 소리로 90도 인사를하며)\n감사합니다!!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n여자: (머쓱해하며)\n아니에요 ㅎㅎ",ButtonType::NO  ,ButtonType::FINAL_END }));




		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: 저.. 라이터 잘썼습니다 !!", ButtonType::NONE , ButtonType::NONE , ButtonType::NO}));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n여자: (귀엽게 나를보며)\n 뭘요 ㅎㅎ", ButtonType::NONE , ButtonType::NONE , ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (하.. 너무 내 이상형인데.. 어떡하지)", ButtonType::NONE ,ButtonType::Choice , ButtonType::NO , true}));

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice ,ButtonType::NONE , ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.5f}, 1,L"(흐린 발음으로 핸드폰을 건네며) 혹시 번호..좀..",ButtonType::Choice ,ButtonType::NONE , ButtonType::NO }));  // YES
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME},true,0.5f}, 2,L"(흐린 발음으로 핸드폰을 건네며) 인스타 맞팔해요!",ButtonType::Choice ,ButtonType::NONE , ButtonType::NO }));        // NO
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_SOCIABILIT},true,1.f}, 3,L"(눈을 쳐다보며) 나중에 커피 사고 싶은데 괜찮으시면 연락주세요!",ButtonType::Choice ,ButtonType::NONE , ButtonType::NO }));        // NO


		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n여자: (당황해하며)\n죄송합니다.. 친구가 기다리고 있어서요..",ButtonType::YES,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: (멍을 때리며 벙찐 표정으로 서있는다)\n...",ButtonType::YES ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n여자: (담배 불을 끈다)\n치익...",ButtonType::YES ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n        (가볍게 목례를 하며 여자가 사라진다)",ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::NO ,false,true }));
		// (자존감 하락)

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n여자: (눈이 풀린 나를 바라보며)\n아.. 저 sns 잘 안해요.. 죄송해요..",ButtonType::NO ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 아.. 그럼 번호...",ButtonType::NO ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n여자: (말을 끊으며)\n친구가 기다리고 있어서요..",ButtonType::NO ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n여자: (담배 불을 끈다)\n치익...",ButtonType::NO ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n        (가볍게 목례를 하며 여자가 사라진다)",ButtonType::NO ,ButtonType::FINAL_END ,ButtonType::NO ,false,true }));


		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n여자: (고민을 하는 표정으로)\n 아 네..",ButtonType::FIGHT ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n여자: (핸드폰을 받으며 번호를 입력한다)",ButtonType::FIGHT ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (환하게 웃으며)\n 감사해요 연락 기다리고 있을게요!",ButtonType::FIGHT ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n여자: (피식하며)\n 네 ㅎㅎ...",ButtonType::FIGHT ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n        (가볍게 목례를 하며 여자가 사라진다)",ButtonType::FIGHT ,ButtonType::FINAL_END ,ButtonType::NO ,false,true }));   //  << 마지막 true tirger부분임
		// (자존감 하락)






		// case 2
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n        (시원한 공기가 달궈진 몸을 식힌다)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: (혼자 베시시 웃으며 혼잣말로 중얼거리며)\n이게 대학 생활인가.. 어지러운데 기분은 좋네 ㅎㅎ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (밤 하늘을 바라보다 바닥으로 머리를 숙이며)\n후 ...",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n        (술집 입구에서 이상형에 가까운 여자가 나온다)",ButtonType::NONE ,ButtonType::FINAL_END }));
			
		
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: (조심스럽게 다가가며 미소를 지으며)\n안녕하세요, 혹시 라이터 좀 빌릴수 있을까요?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n여자: (흔쾌히 건네어준다)\n아! 여기요!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (가볍게 목례를 하며)\n 감사합니다!",ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::NAME},true,0.5f}, 1,L"(스윗하게 말하며) 조심스럽게 말을 걸어본다..",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true, 0.f}, 2,L"(아무런 행동도 하지 않으며 한참을 서있는다) ...",ButtonType::Choice }));
		
		

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: (라이터를 건네주며)\n감사합니다.. 여기 자주 오세요?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n여자: (고개를 끄덕이며)\n학교 앞에서 술 마실 땐 자주 와요~",ButtonType::YES}));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: 아하.. 저는 오늘 여기 처음와봤어요!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n나: (부담스럽게 다가가며)\n 그럼 저도 자주 와야겠네요~!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n여자: (핸드폰을 보며 스타카토 웃음으로)\n 하..하..하.. 네..",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n        (잠시 정적이 흐른다)",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n나: (빠르게 담배를 태우고 자리를 벗어나며)\n 아 그럼 다음에 또 봬요~",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n여자:   (핸드폰을 하며 말 없이 고개를 끄덕인다)",ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::NONE,false,true }));


		// 자존감 감소
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n여자: (웃으며)\n 푸흡..!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: ...?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n여자: (귀여워 하며)\n 제 라이터 왜 계속 가지고 계세요 ㅋㅋㅋ",ButtonType::NO })); // << 여기 수정해야함
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n나: (당황하며)\n 아..아.. 여기요!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: (너무 내 이상형인데.. 어떡하지..)",ButtonType::NO , ButtonType::FINAL_END }));




		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: 저기요!!!",ButtonType::NONE,ButtonType::Choice, ButtonType::NO }));


		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: ",ButtonType::Choice ,ButtonType::NONE,ButtonType::NO}));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME},true,1.f}, 1,L"(적극적으로 대쉬하자!) 진짜 이쁘세요 그쪽!",ButtonType::Choice ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME},true,0.5f}, 2,L"(부끄러워하며) ㄱ..귀여워요 그쪽!",ButtonType::Choice  ,ButtonType::NONE,ButtonType::NO }));
		
		

		
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n여자: (멋쩍게 웃으며)\n하하...",ButtonType::YES ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: (여자의 반응을 살피며)\n놀라셨죠 죄송해요..",ButtonType::YES ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n여자: (핸드폰을 보며 관심없다는 표정으로)\n아..넵..",ButtonType::YES ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n나: (무안한 표정으로)\n다음에 또 보면 인사할게요!",ButtonType::YES ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n여자: (살짝 훑어보며)\n네~",ButtonType::YES ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n        (민망함에 자리를 벗어난다)",ButtonType::YES ,ButtonType::FINAL_END  ,ButtonType::NO,false,true }));
		
		

		
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n여자: (당황해하며 손으로 입을 가린다)\n하핳...ㅎㅎ",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: 혹시 남자친구 있으세요?",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n여자: 없어요",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n나: (속으로 깊은 한숨을 내쉰다)\n다행이다!",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: (당당하게)\n혹시 너무 제 이상형이셔서 그런데 번호 남겨드릴테니까\n연락한번 주실래요?",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n여자: (옅은 미소를 띄우며)\n네..ㅎㅎ",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n        (환하게 웃으며 자신감이 붙는다)",ButtonType::NO ,ButtonType::FINAL_END  ,ButtonType::NO,false,true }));

		// 자존감 상승
	
	}


	else if (L"Study Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::STUDY;

		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"어.. ? 저건 뭐지?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"(앞쪽에 수상한 책이 있다)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"무슨책을 볼까?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"골라보자..",ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"전공 서적에 대한 내용이 담겨져있는 책이다\n\n1.C언어의 자료형에는 int,double,float,char 등이있다.\n\n2.C언어의 조건문과 반복문은 if문,switch문을 사용한다.\n\n3.C언어의 배열과 포인터는 밀접한 연관이있다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"전공 서적에 대한 내용이 담겨져있는 책이다\n\n1-1.정수자료형은 int,실수는 double,float,문자는 char이다.\n1-2.정수자료형과 실수자료형을 연산할경우 ^캐스팅^ 이라는것이 필요하다.\n1-3.int의 크기는4Byte,double은8Byte,char는1Byte이다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"전공 서적에 대한 내용이 담겨져있는 책이다\n\n2-1.if문의 조건식을 판별하는 기준은 true,false로 구분한다.\n2-2.하지만 C언어는 true,false가 없으므로 0과1로 구분한다.\n2-3.switch를 사용할경우 case라는 키워드를 사용해야한다",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"전공 서적에 대한 내용이 담겨져있는 책이다\n\n3-1.배열은 어떤 변수를 여러개 만들때 사용하면 엄청나게 유용하다.\n3-2.포인터는 어떤 변수를 가르키는 ^주소^ 라는 개념이있다.\n3-3.포인터로 어떤 배열을 가르키면 그배열의 모든 요소에 접근할수있다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"전공 서적에 대한 내용이 담겨져있는 책이다\n\n\n이 다음부분은 찢어져 있어 내용을 알아보기 어렵다.",ButtonType::NONE ,ButtonType::FINAL_END}));



		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"심리학서적에 대한 내용이 담겨져있는 책이다\n\n1.프로이트는 마음의 영역을 의식,전의식,무의식으로 나누었다.\n\n2.성격장애중 ^편집성 성격장애^는 타인에게부당한의심을 과도하게갖는다.\n\n3.알츠하이머질병은 흔히 ^치매^라 불리며 기억력 장애를 유발시킨다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"심리학서적에 대한 내용이 담겨져있는 책이다\n\n1-1.무의식은 정신내용의 대부분에 해당하며 인간 행동의 동기가 된다.\n1-2.전의식은 의식과 무의식의 중간지점에 있으면서 두가지의 교량역할을 한다.\n1-3.의식은 우리가 인식하고 있는 영역에서 나타나는 정신적 활동을 의미한다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"심리학서적에 대한 내용이 담겨져있는 책이다\n\n2-1. ^편집성 성격장애^는 충분한 근거없이 타인들이 자신을 착취,상해 또는 속인다고 의심한다.\n2-2. ^편집성 성격장애^는 친구나 동료의 성실성이나 신용에 대한부당한의심에 집착한다.\n2-3. ^편집성 성격장애^는 정당한이유 없이 배우자나 성적파트너의 정절에 대해 반복적으로 의심한다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"심리학서적에 대한 내용이 담겨져있는 책이다\n\n3-1.^치매^로 짐작되는 현상에는 일상생활에 악영향을 끼칠정도로 건망증이 심해진다.\n3-2.^치매^는 언어,전두엽,공간지각 능력에 대한 장애가 발생한다.\n3-3.^치매^를 일으키는 원인중 알코올중독,우울증,갑상샘 기능 저하증등이 있다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"심리학서적에 대한 내용이 담겨져있는 책이다\n\n\n이 다음부분은 찢어져 있어 내용을 알아보기 어렵다.",ButtonType::NONE ,ButtonType::FINAL_END }));
		

		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"사자성어에 대한 내용이 담겨져있는 책이다\n\n1.勞心焦思(노심초사):어떤일을 할때에 매우애를 쓰고 속을 태운다는 말입니다.\n\n2.同床異夢(동상이몽):겉으로는 같이행동하면서 속으로는 서로다른 생각을품음을 말합니다.\n\n3.武陵桃源(무릉도원):모든사람들이 아무걱정 없이 평화롭게 살아가는 이상향을 뜻합니다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"사자성어에 대한 내용이 담겨져있는 책이다\n\n4.萬壽無疆(만수무강):누구에게나 한없이 오래살것을 축원한다는 말입니다.\n\n5.桃園結義(도원결의):큰일을 도모하기위해 뜻이맞는사람들끼리 서로의리로서 맺는일을 말합니다.\n\n6.大同小異(대동소이):작은 부분에서만 다르고 전체적으로는 같음을 말합니다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"사자성어에 대한 내용이 담겨져있는 책이다\n\n\n이 다음부분은 찢어져 있어 내용을 알아보기 어렵다.",ButtonType::NONE ,ButtonType::FINAL_END }));
		

		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"???에 대한 내용이 담겨져있는 책이다\n\n1.학생들이 싫어하는 피자는 ___입니다.\n\n2.세상에서 가장 뜨거운 바다는 ___입니다.\n\n3.자동차를 톡 하고 치면 ___입니다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"???에 대한 내용이 담겨져있는 책이다\n\n4.육지에 사는 고래는 ___입니다.\n\n5.도둑이 몰래 훔친돈은 ___입니다.\n\n6.경찰서의 반대말은? ___입니다.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"???에 대한 내용이 담겨져있는 책이다\n\n\n\n\n이 다음부분은 찢어져 있어 내용을 알아보기 어렵다.",ButtonType::NONE ,ButtonType::FINAL_END }));



		
	}

	else if (L"HiddenStudy Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::HIDDENSTUDY;

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n전공서적에 대한 퀴즈이다\n\n문제1: C언어에서 다른자료형끼리 연산할경우 ___\n이라는것이 필요하다.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"정답:캐스파",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 2,L"정답:캐스팅",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 3,L"정답:캐시팅",ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답입니다. 다음단계로 넘어가세요.",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::FIGHT ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n전공서적에 대한 퀴즈이다\n\n문제2: C언어에서 변수를 여러개 만들때 ___\n을사용하면 유용하다.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 1,L"정답:배열",ButtonType::Choice}));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 2,L"정답:포인터",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 3,L"정답:자료형",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답입니다. 다음단계로 넘어가세요.",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::NO ,ButtonType::FINAL_END}));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::FIGHT ,ButtonType::FINAL_END}));
		////////////////////////////////////////////



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n심리학서적에 대한 퀴즈이다\n\n문제1: 알츠하이머를 유발시키는 원인중 ___\n이있다.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 1,L"정답:갑상샘 기능 저하증",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 2,L"정답:인슐린 저하증",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 3,L"정답:편집성 장애",ButtonType::Choice }));
																 
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답입니다. 다음단계로 넘어가세요.",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::FIGHT ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n심리학서적에 대한 퀴즈이다\n\n문제2: ___은 정신내용의 대부분에 해당하며 인간행동의 동기가된다",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"정답:의식",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 2,L"정답:전의식",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 3,L"정답:무의식",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답입니다. 다음단계로 넘어가세요.",ButtonType::FIGHT ,ButtonType::FINAL_END }));
		////////////////////////////////////////////



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n사자성어에 대한 퀴즈이다\n\n문제1: ___은 큰일을 도모하기위해 뜻이맞는사람들끼리\n서로 의리로서 맺는일을 말합니다.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"정답:무릉도원",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 2,L"정답:죽마고우",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 3,L"정답:도원결의",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답입니다. 다음단계로 넘어가세요.",ButtonType::FIGHT ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n사자성어에 대한 퀴즈이다\n\n문제2: ___은 겉으로는 같이행동하면서 속으로는\n서로다른 생각을품음을 말합니다.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"정답:일심동체",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 2,L"정답:동상이몽",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 3,L"정답:겉바속촉",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답입니다. 다음단계로 넘어가세요.",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::FIGHT ,ButtonType::FINAL_END }));
		////////////////////////////////////////////



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n넌센스에 대한 퀴즈이다\n\n문제1: 자동차를 톡하고 치면? ___입니다.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"정답:차차오톡",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 2,L"정답:차아파",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 3,L"정답:카톡",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답입니다. 다음단계로 넘어가세요.",ButtonType::FIGHT ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n넌센스에 대한 퀴즈이다\n\n문제2: 학생들이 제일 싫어하는 피자는? ___입니다.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"정답:도미노피자",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 2,L"정답:책피자",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 3,L"정답:팔자피자",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::YES ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답입니다. 모든문제를 해결하였습니다.",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n\n지식을 얻은 소감이 어떠신가요?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n\n인생을 살면서 무언가에 몰입하고\n공부하는것이 가장 행복한 삶의태도입니다.",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n\n남은 대학생활 열심히하시고!\n\n 안녕히가세요!",ButtonType::NO,ButtonType::FINAL_END ,ButtonType::NONE,false,true }));


		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"정답이 아닙니다. 처음부터 푸세요",ButtonType::FIGHT ,ButtonType::FINAL_END }));
		////////////////////////////////////////////

	}
	
	else if (L"MT Scene" == m_pCurScene->GetName())
	{
		
		m_CurSceneCount = SCENE_TYPE::MT;
		
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n나: (긴 한숨을 내밷으며)\n드디어 내일이 엠티네.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n나: (첫 엠티라 그런지 설렘 반 걱정 반이야..)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n나: (혼잣말로 중얼거리며)\n일단 챙겨야될 게 뭐가 있었더라.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n나: (중얼중얼)\n치약,칫솔,속옷,수건,잠옷..." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n나: (핸드폰 속 연락처를 보며)\n얘가 동주고..얘가 현찬이고..얘가 우진이.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n나: (고개를 저으며)\n이름이랑 얼굴도 매칭이 안되는데..\n내일어떡하냐 나 정말..ㅠㅠ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n나: (울상인 표정으로)\n일단 자자..내일 일찍 일어나야해!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n나: (...)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n나: (...zzZ)" ,ButtonType::NONE ,ButtonType::FINAL_END }));





		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"드디어 기다리던 MT..!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"애들은 어디있지?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"저 멀리 버스가\n오는것같다.." ,ButtonType::NONE ,ButtonType::FINAL_END }));

	
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"버스에 타면 되는건가?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"읏차!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"무서운 선배: 어이 한신이 왔는가?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 아..앗 안녕하세요 선배님!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"친구1: (손을 흔들며)\n한신아!! 여기야!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 어 먼저 타고 있었네!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: (옆자리에 앉으며)\n너 근데 얼굴이 왜그래..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"친구1: (하품을 하며)\n오늘 엠티라 어제 밤을 설쳤어..ㅎ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"나: (공감하며)\n나도 어제 잘 못잤어 너무 긴장해서..ㅎㅎ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"친구1: (조용한 목소리로)\n오늘 선배님들도 몇분 보이네..?" ,ButtonType::NONE,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"(걱정하는 목소리로) 그러게.. 첫 엠티에 선배들도계시니까 무섭다.. " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 2,L"(들뜬 목소리로) 그러게! 오늘 선배님들이랑도 가까워지면 좋겠다!!" ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"			(이야기를 주고 받던중 친구 한명이 자리를 못앉고 있다..)\n나: 어떡하지?.." ,ButtonType::YES ,ButtonType::FINAL_END}));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"			(이야기를 주고 받던중 친구 한명이 자리를 못앉고 있다..)\n나: 어떡하지?.." ,ButtonType::NO  ,ButtonType::FINAL_END}));



		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"아싸: 힝.. 나 누구랑 앉아..ㅜㅜ" ,ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"(고개를 숙이며 피하듯이) 저기 그냥 빈자리 아무곳이나 가.." ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,2.f}, 2,L"(격려하듯 친구를 챙기며) 우리 옆자리로 올래??!" ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"아싸: 힝.. 난 또 혼자야..." ,ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"아싸: 고마워!! 옆으로 갈게!" ,ButtonType::NO  ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"학생회장: (마이크를 킨다)\n아 아 마이크 테스트!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"학생회장: (우렁찬 목소리로)\n안녕하십니까!!\n오늘 엠티 진행을 맡게된 학생회장 윤야수!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"부학생회장: (우렁찬 목소리로)\n부학생회장 김미녀입니다!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"학생회장: (우렁찬 목소리로)\n오늘 엠티에 참여해주신\n여러분들 진심으로 감사드립니다~!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"학생들: (박수를 치며 환호한다)\n꺄아아아아~~ 우와~~" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"학생회장: (우렁찬 목소리로)\n출발하기 전에 인원체크 먼저 한번 할게요~!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"부학생회장: (혼잣말로 조용하게)\n1명,2명,3명,4,5..." ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"부학생회장: (고개를 끄덕이며)\n다왔구만!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"학생회장: (명단을 건네 받으며)\n빠진 인원은 없는 거 같으니 바로 출발할게요!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"학생회장: (큰 목소리로 안내하며)\n안전벨트는 도착하기 전까지 절대 풀지마세요~!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"학생들: (들뜬 목소리로)\n 네~~~~!!" ,ButtonType::NONE,ButtonType::FINAL_END  }));

		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"학생회장: 오늘 일정 간단하게 말씀드립니다~!\n숙소에 도착하면 각자 짐을 풀고 저희가 준비한 레크레이션을 즐긴뒤,\n저녁으로 고기를 먹을 예정입니다!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"학생들: (몇 명만 대답하며)\n네~~!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"학생회장: (실망한 목소리로)\n목소리가 작네요..??\n다시.. 즐.길.준.비 되셨나요!?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"학생들: (비명을 지르며 환호한다)\n네 !!!!!!!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"학생회장: 감사합니다~! 가봅시다!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"							(갑자기 신나는 음악이 흘러나온다)..." ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"학생회장: (목을 가다듬으며)\n크흠흠!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"부학생회장: (옷 무새를 다듬으며)\n후훗..!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"학생회장: 여러분!!!! 보십시오!!\n 제 화.려.한.춤을!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"학생들: 와아아아~!~!와아와아아아~!~!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"학생회장: (흘러나오는 노래에 리듬을 타며)\n두둠칫 두둠칫 두두둠칫!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"학생들: (환호하며)\n와아아아아아!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"부학생회장: (질수 없다는듯이)\n여러분!!! 제 기타실력을 느껴보세요!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"학생들: 와아아!!!!\n꺄아앙아아악" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"학생회장,부학생회장: (분위기를 띄우며)\n 앉아 있는 친구들 모두!!!!\n솔직하게 STAND UP !!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"					(학생회의 신나는 무대를 보며 숙소로 향한다...)" ,ButtonType::NONE ,ButtonType::FINAL_END}));
	
	}

	else if (L"MainMT Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::MAINMT;

		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"숙소 내 방송: (지직..)\n여러분~ 오늘 MT 일정 마무리해주세요!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"숙소 내 방송: (지직..)\n오늘 하루 재밌는 시간 보내셨죠 다들~?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"숙소 내 방송: (지직..)\n내일은 기상후 각자 개인적으로 귀가하시면됩니다!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"숙소 내 방송:						모두 수고하셨습니다!" ,ButtonType::NONE ,ButtonType::FINAL_END}));



		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"!!!!!!!!!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"여기가 MT 숙소구나!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"엄청 넓은데..구경좀 해볼까?" ,ButtonType::NONE ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"숙소 내 방송: (지지직..)\n아아~! 마이크 테스트" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"숙소 내 방송: (지지직..)\n오늘 준비한 레크레이션은 괜찮으셨나요~??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"학생들: (웅성웅성)\n네~!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"숙소 내 방송: (지지직..)\n재미있게 노느라 배고프셨죠?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"숙소 내 방송: (지지직..)\n이제 각자 준비된 음식 드시고,\n8시까지 다시 집합 해주시면 감사하겠습니다 !!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"								(방송이 끝나고 사람들이 분주하게 움직인다)" ,ButtonType::NONE ,ButtonType::FINAL_END}));





		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"(음식이 놓여져있는 테이블을 쳐다본다)\n..." ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"(꼬르륵..)\n배고프다 얼른 먹어보자!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"냠냠.. 맛있따 맛있어!!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"고기는 한우인가?" ,ButtonType::NONE ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"맛있어.. 슬슬 배부르다.." ,ButtonType::NONE ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"자리가 슬슬 마무리 되는것같은데?" ,ButtonType::NONE ,ButtonType::FINAL_END }));
		



		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"					(음식을 먹고있는중 모르는 친구가 자리를 찾는다..)" ,ButtonType::NONE ,ButtonType::Choice,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나:" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"(친구를 쳐다보며) 나 다 먹었어!! 여기 앉아서 먹어 ㅎㅎ" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 2,L"(먹으니까 더 배고파.. 다른 사람들이 비켜주겠지 뭐!..)" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::FRIEND_SETING,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 3,L"(전혀 신경쓰지않는다...)" ,ButtonType::Choice }));




		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"친구2: (손사래 치며) 아니야 !! 기다렸다가 이따가 먹을게!\n생각해줘서 고마워 ㅎㅎ" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 아니야!! 나 아까부터 먹고 있었어! 얼른먹어!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"친구2: (활짝웃으며)\n 아 그래!? 고마워 ㅎㅎ" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"친구2: 넌 이름이 뭐야?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"나: (미소를 지으며) 한신이야..ㅎㅎ" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"나: (궁금해하는 표정으로) 넌 이름이 뭐야?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"친구2: 난 김혜수!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"나: (화들짝 놀라며)\n아! 어제 카톡 명단에서 봤다!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"친구2: (같이 놀라며)\n정말?? 나도 아까부터 친해지고싶었는데!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"나: 다행이다! 앞으로 만나면 인사할게!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"친구2: (흔쾌히) 웅! 나도 인사할게!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"나: 맛있게 먹어~!" ,ButtonType::YES ,ButtonType::FINAL_END}));




		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"							(친구2가 돌아가지 않고 서서 자리를 기다린다)" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: (신경을쓰며..) 자리 없으면 여기서 먹을래?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"친구2: (기다렸다는 표정으로) 아 그래? 고마워 !!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: (옅은 미소를 띄우며) 웅웅 맛있게먹어~" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"친구2: 응!! 근데 넌 이름이 뭐야?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"나: 난 한신이야! 넌?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"친구2: (개걸스럽게 먹으며..) 쩝쩝.. 난 김혜수!!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"나: 배고팠구나..?\n그..그래! 맛있게먹어!" ,ButtonType::NO ,ButtonType::FINAL_END}));



		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"							(친구2가 주위를 살피다 돌아간다...)" ,ButtonType::FIGHT ,ButtonType::FINAL_END }));
		






		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"귀염뽀짝친구: 한신아!!! 언제온거야!!!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 어..방금 와서 고기먹고있었어!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"귀염뽀짝친구: 애들이랑 바다보러가자!!\n다들 밖에있어!!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: 구랭!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"귀염뽀짝친구: 나 따라와~~!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"귀염뽀짝친구: 지금나가겠어?" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"귀염뽀짝친구: 바다보러 가자!!" ,ButtonType::NONE ,ButtonType::FINAL_END  ,ButtonType::NONE,false,true }));
	}


	else if (L"SeaMT Scene" == m_pCurScene->GetName())
	{

		m_CurSceneCount = SCENE_TYPE::SEA_MT;



		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: (주위를 둘러보며)\n대학 입학하고 처음으로 많은 사람사람들을 마주하는것같아" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"친구1: (공감하며)\n맞아맞아! 나도 이번 엠티 정말 잘 온것같아!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"친구1: (눈을 마주치며)\n한신이 너는 여기서 친해진 친구있어?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: (곰곰히 생각해본다..)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"나: (장난스런 웃음) 음.. 너..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"친구1: ㅋㅋㅋㅋㅋㅋㅋ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"나: ㅋㅋㅋㅋㅋㅋㅋ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"친구1: (웃음) 나도 마찬가지야 ㅎㅎ.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"								(아까 밥먹을때 본 친구가 옆에 보인다)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"나: 옆에 저친구 누군지 알아?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"친구1: 혜수라는 친구야!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"나: 아하.. 아까 밥먹을때 내가 자리를 양보 안해줘서.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"친구1: 그래?? 불러볼까??" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: 아..앗 안녕..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"혜수: 안녕?? 아까 자리가 없어서 밥을 못먹었어..ㅜ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: 아..미안,, 내가 자리를 양보해줄걸 그랬나봐" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"혜수: 아냐아냐 내가 너무 늦게 도착했어" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"친구1: 그래~~ 이따 들어가서 다른거 먹으면되지!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"							(친구1의 도움으로 분위기가 만회된다..)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"나: 어우 근데 날씨가 좀 추운것같아" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"친구1: 그러게 바닷가 앞이라 쌀쌀하다.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"혜수: 맞아,, 그건 그렇고 요즘 너네 공부는 잘 돼가?" ,ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"(고민하며) 이론은 괜찮은데, 실습만하면 막히는 부분이 많더라.." ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(고개를저으며) 나도 잘 모르겠어.." ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"혜수: 나는 대학들어오고 전공이 너무 안 맞는 느낌이야.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"친구1: (공감하며)\n나도 그렇게 생각해.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: 우리모두 신입생이니까 그럴수 있다고 생각해!!!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: (친구들을 쳐다보며)\n먼저 관심있는 과목에 집중하고\n열심히 공부하면 전공에 더 흥미를 가질수 있지 않을까?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"친구1,혜수: (고개를 끄덕이며)\n그렇겠지..?" ,ButtonType::YES ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"혜수: 나는 대학들어오고 전공이 너무 안 맞는 느낌이야.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"친구1: (공감하며)\n나도 그렇게 생각해.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: (쓴웃음 지으며)\n전공공부 할때 뭐부터 시작해야할지 모르겠어.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"친구1,혜수: (고개를 끄덕이며)\n맞아.." ,ButtonType::NO ,ButtonType::FINAL_END }));




		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"혜수: (아까.. 매몰차게 자리양보를 안해준 녀석이군..)" ,ButtonType::NONE,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"혜수: 안녕 한신아!!\n아까 밥먹을때 도와줘서 고마워♥" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"우와..바다다" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"물결이 너무 이쁘다" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"저기 친구들인가?" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: (주위를 둘러보며)\n대학 입학하고 처음으로 많은 사람사람들을 마주하는것같아" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"친구1: (공감하며)\n맞아맞아! 나도 이번 엠티 정말 잘 온것같아!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"친구1: (눈을 마주치며)\n한신이 너는 여기서 친해진 친구있어?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: (끄덕이며)\n아까 밥먹다가 만난..." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"나: (손으로 혜수를 가르킨다)\n저기 옆에 양갈래친구!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"나: (큰 목소리로)\n혜수야!" ,ButtonType::NONE ,ButtonType::FINAL_END})); // 혜수는 음식먹을때 옆에오던 여자애임

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: 마침 너 얘기 나와서 하고 있었는데 잘 됐다~!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"혜수: (놀란 표정으로)\n아 정말??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"혜수: (무언가 생각난 표정으로)\n아! 그리고 아까 덕분에 밥 먹을수 있었어..!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"혜수: 너 아니였으면 제 시간에 밥 먹기 힘들었을거야.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"나: (미소를 지으며)\n그랬다면 다행이다ㅎㅎ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"나: (친구들을 번갈아 쳐다보며)\n너희 둘도 인사한번해!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"친구1,혜수: (동시에 웃으며)\n안녕 ㅎㅎㅎ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"나: 우리 셋이 같이 산책할까?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"친구1,혜수: 그래 좋아!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"								(몇 마디의 자기 소개가 이어진다..)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"혜수: 너넨 전공 공부는 잘 돼가?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"친구1: (웃음 지으며)\n난 아직 모르겠어.." ,ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"(고민하며) 이론은 괜찮은데, 실습만하면 막히는 부분이 많더라.." ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(고개를저으며) 나도 잘 모르겠어.." ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"혜수: 나는 대학들어오고 전공이 너무 안 맞는 느낌이야.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"친구1: (공감하며)\n나도 그렇게 생각해.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: 우리모두 신입생이니까 그럴수 있다고 생각해!!!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: (친구들을 쳐다보며)\n먼저 관심있는 과목에 집중하고\n열심히 공부하면 전공에 더 흥미를 가질수 있지 않을까?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"친구1,혜수: (고개를 끄덕이며)\n그렇겠지..?" ,ButtonType::YES ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"혜수: 나는 대학들어오고 전공이 너무 안 맞는 느낌이야.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"친구1: (공감하며)\n나도 그렇게 생각해.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: (쓴웃음 지으며)\n전공공부 할때 뭐부터 시작해야할지 모르겠어.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"친구1,혜수: (고개를 끄덕이며)\n맞아.." ,ButtonType::NO ,ButtonType::FINAL_END}));
		



		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"혜수: 그럼 공부말고, 너네 알바는 어떻게 하고있어?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"친구1: 난 아직 안해봤어!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: 나는 편의점!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"혜수: (놀라며)\n아 정말? 나도 편의점 알바 지원했는데\n편의점 알바는 어때?" ,ButtonType::NONE,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"그냥 진상 손님 응대하는 거 말고는 별거 없더라~" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"다른 알바 찾아봐..꽤 힘들어.." ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"혜수: 진짜?? 역시 한신이 자신감 멋져!!" ,ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"혜수: 그래?? 도서관알바나 알아볼까.." ,ButtonType::NO ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"							  (많은 얘기를 하며 시간이 흐른다)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"친구1: (궁금해하는 표정으로)\n너넨 꿈이 뭐야?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"혜수: 나는 스시 집 차릴거야 ㅎㅎ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"친구1: (웃음)\n나중에 놀러갈게!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"친구1: 한신이는?" ,ButtonType::NONE ,ButtonType::Choice}));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나는 코딩하는 게 재밌있어서 IT 개발자 생각하고 있어!" ,ButtonType::Choice  }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나는 아직 더 공부하면서 생각해봐야 될 것 같아!" ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"친구1: 그렇구나.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 너는!?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"친구1: ..." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"친구1: 글쎄..." ,ButtonType::YES ,ButtonType::FINAL_END}));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"친구1: 그렇구나.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 너는!?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"친구1: ..." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"친구1: 글쎄..." ,ButtonType::NO ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: 으음..." ,ButtonType::NONE,ButtonType::Choice ,ButtonType::YES}));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: 으음..." ,ButtonType::NONE ,ButtonType::Choice ,ButtonType::NO}));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice ,ButtonType::NONE,ButtonType::YES}));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"좋아하는 과목은 있어?" ,ButtonType::Choice,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(어깨를 토닥이며) 시간이 해결해줄거야..!" ,ButtonType::Choice,ButtonType::NONE,ButtonType::YES }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"좋아하는 과목은 있어?" ,ButtonType::Choice,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(어깨를 토닥이며) 시간이 해결해줄거야..!" ,ButtonType::Choice,ButtonType::NONE,ButtonType::NO }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"친구1: 나..교양수업말고는 ...없..어.." ,ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"친구1: (실망한 표정으로) 그렇겠지..훌쩍.." ,ButtonType::NO ,ButtonType::FINAL_END ,ButtonType::NO }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"친구1: 웹 프로그래밍 정도??" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 그럼 웹 디자인 쪽이 잘 맞을수 있겠네!" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"친구1: 지금은 할만한데 나중엔 그것도 어렵지 않을까..?" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: 지금 배우는 거에 흥미만 있으면 되지 않을까?" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"친구1: (의미심장한 표정으로)\n무슨 말이야..?" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"나: 지금 관심있는 분야 쪽으로 공부하다보면\n나중에 경험과 노하우가 쌓여서 어렵게 느껴지지않을거야!" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"친구1: (깨달은표정으로)\n그래 처음부터 쉬운게 어디있겠어!!\n고마워 한신아!!ㅎㅎ" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"나: (장난스러운 표정으로)\n그럼 나중에 밥 사 ㅎㅎ" ,ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::YES}));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"친구1: (실망한 표정으로)\n그렇겠지..훌쩍.." ,ButtonType::NO ,ButtonType::FINAL_END ,ButtonType::YES }));



		


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: 그럼이제 슬슬 들어갈까?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"친구1,혜수: 그래!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"친구1: (넘어지며) 아야!!! ㅠㅠ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: (걱정하는 표정으로) 괜찮아???" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"친구1: ..응.." ,ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"(웃음) 기다려봐 나 데일벤드 있어" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(걱정하는 표정으로) 앞을 보고 걸어야지..담부턴 조심해!" ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"							(친구 무릎에 벤드를 붙여준다)" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"친구1: 근데 벤드를 들고다녀!?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: 너 같은 애들 있을까봐 챙겨다니지 ㅋㅋㅎㅎ" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"친구1: (박장대소하며) 고마워 ㅎㅎㅎ 얼른가자!" ,ButtonType::YES ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"친구1: 훌쩍.. 알겠오! 얼른가자!" ,ButtonType::NO ,ButtonType::FINAL_END}));

	}

	else if (L"EndMT Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::END_MT;

		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: 이제 정말 헤어질 시간이네.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"								( 한신이가 급발진으로 울음을 터트려버린다.. )" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: (훌쩍훌쩍)\nㅠㅠㅠㅠ애들아 대학에와서 너희랑 친해지게 되서 너무기뻐!!.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: (울음을 다그치며)\n정말 꿈같은 MT였어 정말고마워!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"친구1: 울지마~~ 앞으로 학교에서 자주보자!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"혜수: 뭘 그렇게 울어!! 어짜피 만날건데!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"나: (훌쩍..)\n그렇..겠지..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"친구들 모두: 당연하지!! 한신아!" ,ButtonType::NONE,ButtonType::FINAL_END }));
		
	}


	else if (L"Date Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::DATE;

		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: 오늘은 대망의 소개팅날.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 약속시간이 애매한걸,," ,ButtonType::NONE,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"버스탄다" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 2,L"택시탄다" ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: 그래! 뭐 늦겠어? 돈도 아낄겸 버스타고가자!" ,ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: 첫인상이 제일 중요한데 지각하면 안돼!! 택시타자!" ,ButtonType::NO ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"얼른타고 가자!" ,ButtonType::NONE ,ButtonType::FINAL_END }));
	}

	else if (L"MainDate Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::MAINDATE;

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"지켜보던 사장님: 커흠!!" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"지켜보던 사장님: 자네 그렇게 대화하면 안돼 ~~" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: 누구..?세요?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"지켜보던 사장님: 흠흠!! 나로 말할것 같으면 양산동 카사노바 라고 할까!?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"나:	(....뭐야)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"나: 아저씨가 카사노바 라구요?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"지켜보던 사장님: 그럼!! 자네의 대화 방식이 잘못되었어!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"지켜보던 사장님: 여자가 좋아할법한 대화주제와 흥미를 끌어야지!!\n가끔 개그도 쳐주고!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"나: 말이 쉽죠!!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"지켜보던 사장님: 못 믿는군.. 그럼 한번 내 방식을 들어보겠나?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"나: (못 믿으며)\n그래요..뭐.. 한번 들어나 봅시다" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"양산동 카사노바: 우선!! 첫만남에 자기를 일정낮추면 겸손이 되긴하지만,\n자네처럼 베타메일처럼 행동하면 안돼..\n여자는 가치가 낮은남자는 본능적으로 끌리지 않게되어있어.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"양산동 카사노바: 그리고 두번째!!\n 처음본 상대에게 부담스럽게 대하지 말것!!!\n대화를 자연스럽게 여유를 가지고해야지!! 자넨 너무급해!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 13,L"양산동 카사노바: 마지막 세번째!!\n 여자가 관심도없어하는 취미를 신나서\n혼자 그렇게 말하면 어떡하나?!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 14,L"나: .....그런가요..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 15,L"나: 그럼 방법을 알려줘요!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 16,L"양산동 카사노바: 나는 답을 알려주지 않아..\n스스로 답을 한번 찾아보게!! 내가 문제를 내주지!\n꽤나 어려울것이야!!" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 17,L"양산동 카사노바: 무난한 소개팅 첫 장소는 어디라고 생각하나?" ,ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"시끌벅적한 술집" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"영화관" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 3,L"분위기 좋은 카페" ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"양산동 카사노바: 시끄러운 술집에 가서 대화도 안되는데..\n거길간다구?! 에잉..쯧..",ButtonType::YES ,ButtonType::Choice ,ButtonType::NONE, true}));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"양산동 카사노바: 첫 만남엔 대화가 생명이야!!\n영화관 가서 2시간동안 아무말도 안하고 영화를본다구?!",ButtonType::NO ,ButtonType::Choice ,ButtonType::NONE, true }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"양산동 카사노바: 그래!! 첫만남엔 분위기좋은 카페로 가서!!\n서서히 상대를 알아가는거지!\n일단 장소는 잘 골랐구만",ButtonType::FIGHT ,ButtonType::FINAL_END}));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"양산동 카사노바: 그다음! 가벼운 자기소래를 하면서 주의할점!!!\n너무 상대방에게 꼬치꼬치 캐묻지 말것!\n전 애인에대해 언급한다던가..가족관계를 묻는다던가..연애횟수를 물어본다던가!!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 우씨 그럼 할말이 없잖아요!!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"양산동 카사노바: 가볍게 이상형에 대해 물어본다던지..\n아니면 동물에 대한 주제로 대화할수도 있지!\n",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: 아.. 그렇구나..!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"양산동 카사노바: 또 다른 꿀팁을 배워보겠나?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"양산동 카사노바: 어느정도 자기 소개를 한후..\n상대방이 너에게 관심이없는지..확인하는 방법이랄까?\n한번 잘 생각해보게!",ButtonType::NONE,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"여자가 계속 머리를 만지는것?" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"여자가 핸드폰을 수시로 확인하는것?" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"여자가 털털하게 나를 대하는것?" ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"양산동 카사노바: 그래!! 머리를 계속 만진다는것은..\n자신의 외모를 계속 점검한다는 무의식적 행동이지!!\n그만큼 자네에게 잘 보이고 싶은것!!!",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"양산동 카사노바: 자네 바보인가?! 핸드폰을 수시로보는건\n얼른 이자리를 도망가고싶다는뜻!!",ButtonType::NO ,ButtonType::Choice ,ButtonType::NONE, true }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"양산동 카사노바: 에잉 쯧쯧.. 자네에게 이성적인 매력을 느끼지못하니..\n털털하게 나오는것이지! 매력이 있다면 되려 부끄러워할거야!!",ButtonType::FIGHT ,ButtonType::Choice ,ButtonType::NONE, true }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"양산동 카사노바: 이쯤이면 도움이 좀 되었나?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 뭔가 된것같기도..아닌것같기도..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: 사장님은 그럼 어떻게 여자에 대해 잘 아시는거에요?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"과거 사랑꾼이였던 남자: 나도.. 한때 사랑하는 그녀를 위해 노력했지..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"과거 사랑꾼이였던 남자: 그녀에게 첫눈에 반해 그녀의 마음을 얻기위해\n연애에 대한 공부를 참 많이했지..허허",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"과거 사랑꾼이였던 남자: 하지만 인연을 만난다는것은..\n어렵지..허..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"과거 사랑꾼이였던 남자: 아무튼!! 인연을 만나기전까지!!\n항상 준비를 해야한다 이말이야!!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"과거 사랑꾼이였던 남자: 내 인연은 언제 다시 오려나..~",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"과거 사랑꾼이였던 남자: 앞으로 기회가 많을 자네가 부럽구먼..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"나: 하하,,,",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"나: 근데 이분 왜이렇게 안오시죠?\n도망가신거 아니에요??",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"과거 사랑꾼이였던 남자: 저기 오네!! 잘해보게 친구!!",ButtonType::NONE ,ButtonType::FINAL_END}));






		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"두근두근.. 누가올까.." ,ButtonType::NONE ,ButtonType::FINAL_END}));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"소개팅녀: ㅇ..안녕하세요!\n여기 앉아도되나요?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: (와.. 이쁘다)\n당연하죠!!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		PLAYER_MBTI eMbti = pG_Player->GetMbti();
		wstring strMbti = EnumToString(eMbti);
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: ...." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"소개팅녀: ...." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"소개팅녀: 하하.. 어색하네요,," ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: (부끄러워하며)\n혹시 나이가 어떻게 되시나요!!?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"소개팅녀: 전 20살이에요!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"나: 헉!! 저도 20살인데! 친구네요!?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"나: 괜찮으시면 말 놓을까요..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"소개팅녀: 그래 ㅎㅎ 이름이 뭐야??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"나: 난 김한신!! 너는?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"소개팅녀: 난.. 김가현!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"나: 오!! 이름 이쁘다!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"나: MBTI 물어봐도 될까?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"소개팅녀: 난 ISTP야! 너는?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 13,L"나: 난 " + strMbti + L"야!" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 14,L"소개팅녀: 오~~ 내가 좋아하는 MBTI다!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 15,L"나: 진짜??! 우리 잘통하는것 같다!!\n근데 넌 인기 되게 많을것같은데.. 나 괜찮아?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 16,L"소개팅녀: (당황해하며)\n으..응? 갑자기?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 17,L"나: (이상함을 감지하며)\n아..미안! 좀 부담스러웠을려나..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 18,L"나: (눈치 없이)\n그럼 연애 몇번해봤어?!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 19,L"소개팅녀: (찡그리며)\n아..그건 나중에 말하고싶은데 ㅠㅠ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 20,L"소개팅녀: (애써 괜찮은척하며)\n아냐 ㅎㅎ..넌 취미가 어떻게 돼?" ,ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"애니 보기!" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"오토바이 타기!" ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"소개팅녀: (표정이 굳으며)\n응..? 애니..? 어떤거..?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: (자신감 붙은듯이)\n음!! 너무 많은데!!!\n던만추도 재밌고 !! 귀칼!! 아 귀칼을 이미 봤으려나?!\n학원물도있고..또..코드기어스!! 이건 엄청 유명ㅎ.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"소개팅녀: (더더욱 굳으며)\n잠깐만!! 뭐..뭐라고? 하나도 못알아 듣겠어.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: (영문을 모른채)\n응..? 아 미안 너무 마이너한 애니만 추천했나?\n 다른거 추천해줄까?!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"소개팅녀: 아니.. 나 화장실좀 갈게.." ,ButtonType::YES ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"소개팅녀: (관심 없는듯이)\n아~~ 오토바이 재밌어?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: (자신감이 붙으며)\n물론이지!! 역삼륜 오토바이 타는것도재밌구!!\n아 요즘 미니바이크도 유행이야!!\n맞아!! 스쿠터도 재밌어!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"소개팅녀: (한숨 쉬며)\n...난.. 잘 모르겠어 관심없는 분야야 !!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: (영문을 모른채)\n그래??! 그럼 내가 좀 자세히 설명해 줄까??" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"소개팅녀: 아니.. 나 화장실좀 갈게.." ,ButtonType::NO ,ButtonType::FINAL_END }));




		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n\n						Hidden Quest!\n\n						마우스로 주변을 탐색해보자..\n						제한시간 10초!!!!!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n\n						미션에 실패하셨습니다...\n						아직 소개팅을할 준비가 안된것같아요..\n						시간을 돌려드릴게요!" ,ButtonType::NONE ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"소개팅녀: 화장실 줄이 너무 길어서 늦었네요.. 죄송합니다!!.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 앗 괜찮습니다!! ㅎㅎ\n아까 제가 너무 자기 할말만 한것같아서 죄송해요,," ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"소개팅녀: 하하..아니에요 제가 그쪽 취미를 잘 몰라서,,\n저는 독서가 취미거든요!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: 헉 독서요!? 되게 이상적인 취미를 가지고계시네요!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"소개팅녀: ㅎㅎ..." ,ButtonType::NONE ,ButtonType::FINAL_END}));



		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: (하.. 이 어색한 침묵.. 뭘 물어봐야하지..?)" ,ButtonType::NONE,ButtonType::Choice ,ButtonType::NONE,true}));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"혹시 전남친이랑은 왜 헤어지게 된거야?" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME},true,1.f}, 2,L"너는 이상형이 어떻게 돼?" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"부모님은 무슨일 하셔?!" ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"소개팅녀: (한숨쉬며)\n아니..그게 왜 궁금한데 도대체?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 미안..하..소개팅어렵다.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: (어쩔줄 몰라하며)\n나..화장실좀!!.." ,ButtonType::YES,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"소개팅녀: 으음..외적인건 눈이이쁘고 키크고..\n내적인건 착하고 나만 바라봐주는사람?!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: (장난스럽게)\n눈이 이쁜건 나네 ㅎㅎ 나 엄청 다정한데!!..하핫" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"소개팅녀: 풉.. 넌 이상형이뭐야?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: 난..귀엽고 코가 이쁜사람! 성격은 나랑 티키타카가 잘 되는사람!!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"소개팅녀: 코가 이쁜사람이라니.. 특이하네 ㅎㅎ" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"소개팅녀: 또또!! 재밌는 이야기해줘!!" ,ButtonType::NO,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"소개팅녀: (불편해하며)\n돌아가셨어..2년전에.." ,ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 어?..미..미안" ,ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: (어쩔줄 몰라하며)\n나..화장실좀!!.." ,ButtonType::FIGHT,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(102, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n\n										그녀가 남긴쪽지\n\n안녕하세요, 한신씨.. 짧게나마 대화나눌수 있어서 감사했어요\n\n저는 그쪽과 잘 맞지않는 사람인것같아요 죄송해요..\n\n좋은분 만나세요..  -김가현 올림- ",ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: 으음..." ,ButtonType::NONE,ButtonType::Choice}));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: " , ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME},true,1.f}, 1,L"혹시 좋아하는 동물있어?" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"혹시 게임해??" ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"소개팅녀: (표정이 밝아지며)\n웅!! 나 강아지ㅠㅠㅠ 나 포메라니안 키우거든!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"소개팅녀: (표정이 밝아지며)\n나 동물 너무너무 사랑해!! 너는?? 좋아해??" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: (웃음을 지으며)\n응!! 나도 강아지 좋아해!! 키우진않지만\n강아지들 너무 귀엽잖아~~~!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"소개팅녀: 헐 정말?! 동물 안좋아할것 같았는데 의외네!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"나: 응!! 우리 나중에 같이 강아지카페 가자!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"소개팅녀: 헐 너무 좋아!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"									(그녀가 머리카락을 만지작 거린다)" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"나: (이 신호는..! 사장님이 말한!..)" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"소개팅녀: 우리 슬슬 이따가 저녁 먹으러 갈래?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"나: 좋아!! 마라탕 좋아해?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"소개팅녀: (깊은 호감을 보이며)\n당연하지! 나 마라탕 완전 좋아해 ㅠㅠㅠ" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"나: 우리 꽤 잘맞는거 같다!! 나도 좋아하거든!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"소개팅녀: 그러게!! ㅎㅎㅎ" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 13,L"나: 그럼 이제 밥 먹으러 나가자!!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 14,L"								(분위기가 무르익고 소개팅이 성공적으로 마무리된다...)" ,ButtonType::YES,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"소개팅녀: 아니 ㅎㅎ.. 넌?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: (눈치를 채며)\n나도 게임안해!! 우리 잘맞는걸..?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"소개팅녀: ㄱ..그런가..? ㅎㅎ" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: 이따 저녁 같이 먹을래?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"소개팅녀: 그럴까? 뭐좋아해?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"나: 난 곱창 좋아해!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"소개팅녀: 오! 진짜? 나도 좋아하는데!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"나: 그래?!! 그럼 곱창먹으러 가자!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"소개팅녀: (고민하며)\n음..오늘 바로??" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"나: (당황하며)\n앗..오늘은 좀 부담스러운가?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"소개팅녀: 혹시 다음번에 또 만나는날에 먹는건 어때?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"나: 그래 좋아!!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"								(다음을 기약하며 소개팅이 마무리된다...)" ,ButtonType::NO ,ButtonType::FINAL_END }));

	}

	else if (L"Endding Scene" == m_pCurScene->GetName())
	{
		// 6가지 능력치중 제일 높은 메인 능력치 1개를 뽑는다.
		// 각각 능력치를 모두더 했을때    Max 능력치의 합의 평균, Min 능력치의 합의평균을 낸다
		// 그 두가지 평균을 기준으로 상하를 나눈다 (이상,이하)
		// 메인 능력치를 기본으로 엔딩의 시작을 알리고  3번째 조건인 평균의 이상인지 이하인지 나눠서 총 12개의 엔딩을 낸다.

		// 밸런스문제 일단잇음

		// TEST 엔딩 1번째는 메인능력치: 사회성, 평균능력치의 이상일때 
		m_CurSceneCount = SCENE_TYPE::ENDDING;

		m_strTex[(UINT)SCENE_TYPE::ENDDING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"나: 어..? 여긴 내 방이잖아!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::ENDDING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"나: 이 모든게 다 꿈이였어..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::ENDDING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"나: 꿈속에서 대학생활을 했던거같아..\n미래의 나는 나에게 어떤말을 하고싶었던걸까?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::ENDDING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"나: ...." ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::ENDDING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"나:							( 그가 했던말을 상기하며.. 다시한번 세상밖으로 나간다)" ,ButtonType::NONE,ButtonType::FINAL_END }));
		
	}

}


CTalkUI::~CTalkUI()
{

}



//void CTalkUI::update()
//{
//}
//
//void CTalkUI::finalupdate()
//{
//}

void CTalkUI::StartGame(CUI* pUI)
{
	assert(pUI);

	
	PLAYER_MBTI eMbti = pUI->GetUI_HasMbti();
	// 글로벌 Player 처음 만드는순간
	if (nullptr == pG_Player)
	{
		
		pG_Player = new CPlayer(eMbti);		// Player가 지워지는 시점도 고려해봐야함
		pG_Player->SetName(L"Player");
		pG_Player->SetScale(Vec2(50.f, 70.f));
		pG_Player->SetMbti(eMbti);
	}

	wstring strMbti = EnumToString(eMbti);

	pG_Player->SetPlayerInfo(PLAYER_INFO::MBTI, L"MBTI: " + strMbti);


	m_strTex[(UINT)m_CurSceneCount].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f} ,6,L"\n\n\n\n				김한신씨 당신의 MBTI는\n\n				" + strMbti + L"입니다.",ButtonType::NONE ,ButtonType::FINAL_END }));
	m_MaxTextcount += 1;
	m_CurConversation.id++;
	m_itextSize = 1;


	vector<CUI*>& vecChildUI = GetChildUI();
	
	 for (size_t i = 0; i < vecChildUI.size(); ++i)
	 {
	 	DeleteObject(vecChildUI[i]);
	 }
	 vecChildUI.clear();
}



void CTalkUI::update()
{
	
	if (L"Speech_Bubble_UI" == GetName())
	{
		MoveSpeechBubble();
	}

	if (ButtonType::Choice == m_CurConversation.CurButtonType)
	{
		CreateChoices();
	}

	if (ButtonType::Choice != m_CurConversation.CurButtonType)
	{
		RecreateUI();
	}
	


	int CurConversation_Npc_Count;


	if (nullptr != m_TalkingOwner) // 대화 UI를 npc랑 진행중이면 보유 Owner가 있을것임
	{
		if (!m_TalkingOwner->IsDead())
		{
			CurConversation_Npc_Count = m_TalkingOwner->GetMyConversationID();
		}
		
	}
	else
		CurConversation_Npc_Count = 0; // npc가 없으면 무조건 0



	
	
	
	
	
	m_CurConversation.PreButtonType = g_LastConversationInfo.CurButtonType;

	auto range = m_strTex[(UINT)m_CurSceneCount].equal_range(CurConversation_Npc_Count);
	auto it = range.first;
	for (; it != range.second; ++it)   // 여기 로직에서 현재 대화를 정함 ( 버튼 상태값만으로 )
	{
		if ((*it).second.CurButtonType == m_CurConversation.CurButtonType
			&& (*it).second.id == m_CurConversation.id && (*it).second.PreButtonType == m_CurConversation.PreButtonType)
		{
			
			m_CurConversation = (*it).second;
			break;
		}	
	}

	if (it == range.second)
	{
		// 대화를 못찾았으면 무조건 그 범위의 첫번째 대화로 세팅
		m_CurConversation = (*(range.first)).second;
		g_LastConversationInfo = m_CurConversation;
	}

}


void CTalkUI::MoveSpeechBubble()
{
	if (pG_Player)
	{
		Vec2 vOwnerPos = pG_Player->GetPos();
		SetPos(Vec2(vOwnerPos.x + 125.f, vOwnerPos.y - 110.f));
	}
}


void CTalkUI::CreateChoices()
{
	if (!m_isChoiceCreated)
	{
		m_isChoiceCreated = true;

		vector<CUI*>& vecUI = GetChildUI();
		for (size_t i = 0; i < vecUI.size(); ++i)
		{
			DeleteObject(vecUI[i]);
			vecUI.clear();
		}


		CreateChildChoiceBtn(this);
		m_isUIRecreated = false;

	}

}


void CTalkUI::RecreateUI()
{

	if (!m_isUIRecreated && m_isChoiceCreated)
	{
		m_isUIRecreated = true;

		vector<CUI*>& vecUI = GetChildUI();
		vector<CUI*>::iterator iter = vecUI.begin();
		for (iter; iter != vecUI.end();)
		{
			DeleteObject(*iter);
			iter = vecUI.erase(iter);
		}


		CreateChildUI(this);
		m_isChoiceCreated = false;
	}
}



void CTalkUI::render(HDC _dc)
{
	static bool isFirstCall = true;

	if (CCamera::GetInst()->GetZoomIn())
		return;

	CUI::render(_dc);

	if (GetHasText())
		RenderText(_dc, m_CurConversation); // 해당 씬의 대화 텍스트가 담긴 벡터를 보냄

	
	if (ButtonType::Choice == m_CurConversation.CurButtonType)
	{

		RenderChoiceText(_dc);
	}


	if (IsEnd_of_Conversation() && L"Loding2 Scene" == m_pCurScene->GetName())
	{
		if (isFirstCall)
		{
			CreateMbtiUI(this);
			isFirstCall = false;			
		}	
		RenderMbti(_dc);
	}



	
}

void CTalkUI::render_CamZoomIn(HDC _dc)
{
	CUI::render_CamZoomIn(_dc);


	if (GetHasText())
		RenderText(_dc, m_CurConversation); // 해당 씬의 대화 텍스트가 담긴 벡터를 보냄

	if (ButtonType::Choice == m_CurConversation.CurButtonType)
	{
		
		RenderChoiceText(_dc);
	}
}



void CTalkUI::RenderText(HDC _dc, Conversation _str)
{
	// 현재 대화를 미리 다 정해서 보내줘야함 업데이트에 미리 현재 대화가 뭔지 찾아내야함

	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	
	m_fTime += fDT;

	
	// 0.1초 지연
	if (0.05f < m_fTime)
	{
		// 다음 글자 출력 // 한글자 씩 찍어도 어짜피 화면에 안뜸 왜? 백버퍼에 그리고 잇으니까

		m_fTime = 0.f;

		if (m_itextSize != m_CurConversation.text.size())  // 문자열 다 출력하면 리턴
			m_itextSize++;
		else
		{
			if(SCENE_TYPE::LODING_2 == m_CurSceneCount && nullptr != pG_Player)
				m_bStart = true;
		}
			
	}
	


	Vec2 vPosLT = vPos - vScale / 2.2f; // 좌상단
	Vec2 vPosRB = vPos + vScale / 2.2f; // 우하단

	

	for (int i = 0; i < m_itextSize; ++i)
	{
		if (m_CurConversation.text[i] == L'\n')
		{
			// '\n' 문자를 만났을 때 x 좌표를 다시 초기 값으로 설정하고 y 좌표를 내립니다.
			vPosLT.x = vPos.x - vScale.x / 2.2f;
			vPosLT.y += 30; // 내리고자 하는 y 좌표 간격을 설정합니다.
			vPosRB.y += 30; // 내리고자 하는 y 좌표 간격을 설정합니다.
			continue; // '\n' 문자는 그리지 않고 다음 문자로 넘어갑니다.
		}

		RECT rt = {
		  vPosLT.x
		, vPosLT.y + 40
		, vPosLT.x + 40
		, vPosRB.y + 60 };

		DrawTextW(_dc
			, &m_CurConversation.text[i] // m_Textcount는 버튼누르면 하나씩 증가 다음 대화를 보여줌
			, 1
			, &rt
			, DT_LEFT | DT_WORDBREAK);

		vPosLT.x += 18;
	}

	//static bool soundCheck = false;
	//
	////if (!soundCheck)
	//{
	//	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"TEXT_SOUND");
	//	pNewSound->Play(false);
	//	//pNewSound->SetPosition(1.f); // 백분률 , 소리위치
	//	pNewSound->SetVolume(70.f); // 소리 볼륨 크기
	//
	//	soundCheck = true;
	//}
	
}



void CTalkUI::RenderChoiceText(HDC _dc)
{
	// 현재 씬에서 내가 대화할수있는 npc의 최대 수를 알아야함 그 npc의 수가 곧 multimap 대화의 키값임
	// 동일한 npc와의 대화는 키값이 모두 같음 그 npc와 대화를 하면서 해당 키값으로 어떤 npc인지 알수있고
	// 그다음 그 npc에서 7777 이라는 선택대화만 빼올수있음

	wstring strChoice[3];
	int k = 0;

	CTalkUI* pMainTalkUI = CSceneMgr::GetInst()->GetCurScene()->GetMainTalkUI();


	UINT CurConversation_Npc_Count;
	if (nullptr != m_TalkingOwner) // 대화 UI를 npc랑 진행중이면 보유 Owner가 있을것임
	{
		if (!m_TalkingOwner->IsDead())
		{
			CurConversation_Npc_Count = m_TalkingOwner->GetMyConversationID();
		}

	}
	else
		CurConversation_Npc_Count = 0; // npc가 없으면 무조건 0

	auto range = m_strTex[(UINT)m_CurSceneCount].equal_range(CurConversation_Npc_Count);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (ButtonType::Choice == it->second.CurButtonType)
		{
			if (it->second.PreButtonType == m_CurConversation.PreButtonType)
			{
				if (0 != it->second.id) // id가 0인경우는 나: 이거니까 출력 X
				{	
					// 선택 대화
					if (k == it->second.id - 1)
					{
						strChoice[k] = it->second.text;
						k++;

						if (nullptr != pMainTalkUI)
						{
							if (!pMainTalkUI->IsDead())
							{
								vector<CUI*> vecBtnUI = pMainTalkUI->GetChildUI();

								for (size_t i = 0; i < vecBtnUI.size(); ++i)
								{
									CBtnUI* pBtnUI = dynamic_cast<CBtnUI*>(vecBtnUI[i]);

									if (nullptr != pBtnUI)
									{
										if (!pBtnUI->IsDead())
										{
											if (L"Choice_Left_Btn_UI" == pBtnUI->GetName() && 1 == it->second.id)
											{
												pBtnUI->SetBtnConversation(it->second);
											}
											else if (L"Choice_Right_Btn_UI" == pBtnUI->GetName() && 2 == it->second.id)
											{
												pBtnUI->SetBtnConversation(it->second);
											}
											else if (L"Choice_Bottom_Btn_UI" == pBtnUI->GetName() && 3 == it->second.id)
											{
												pBtnUI->SetBtnConversation(it->second);
											}

										}
									}
								}
							}
							
						}
							
					}
					
				}
				
			}
			
		}
	}

	Vec2 vPos = GetFinalPos();
	Vec2 vOriginPos = vPos;


	for (size_t i = 0; i < 3; ++i)
	{
		RECT rt = {
		 vPos.x - 330.f
		,vPos.y + 5.f
		,vPos.x
		,vPos.y + 40.f
		};

		DrawTextW(_dc
			, strChoice[i].c_str() // m_Textcount는 버튼누르면 하나씩 증가 다음 대화를 보여줌
			, strChoice[i].size()
			, &rt
			, DT_LEFT | DT_WORDBREAK);

		vPos.y = vPos.y + 35.f;
	}

}




void CTalkUI::RenderMbti(HDC _dc)
{
	if (m_bStart)
		return;


	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	vPos.x = vPos.x - 150.f;
	vPos.y = vPos.y + 50.f;
	Vec2 vOriginPos = vPos;

	wstring wstrMbti[4][4];
	wstrMbti[0][0] = L"ISTJ";
	wstrMbti[0][1] = L"ISTP";
	wstrMbti[0][2] = L"INFJ";
	wstrMbti[0][3] = L"INTJ";

	wstrMbti[1][0] = L"ISFJ";
	wstrMbti[1][1] = L"ISFP";
	wstrMbti[1][2] = L"INFP";
	wstrMbti[1][3] = L"INTP";

	wstrMbti[2][0] = L"ESTJ";
	wstrMbti[2][1] = L"ESFP";
	wstrMbti[2][2] = L"ENFP";
	wstrMbti[2][3] = L"ENTP";
							 
	wstrMbti[3][0] = L"ESFJ";
	wstrMbti[3][1] = L"ESTP";
	wstrMbti[3][2] = L"ENFJ";
	wstrMbti[3][3] = L"ENTJ";

	

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			RECT rt = {
				 vPos.x
				,vPos.y
				,vPos.x + 100
				,vPos.y + 60
			};

			DrawTextW(_dc
				, wstrMbti[i][j].c_str() // m_Textcount는 버튼누르면 하나씩 증가 다음 대화를 보여줌
				, wstrMbti[i][j].size()
				, &rt
				, DT_LEFT | DT_WORDBREAK);

			vPos.x = vPos.x + 100;
		}
		vPos.x = vOriginPos.x;
		vPos.y = vPos.y + 60;

	}

}



void CTalkUI::End_of_Conversation()
{
	if (nullptr == pG_Player)
		return;

	pG_Player->SetCurState(PLAYER_STATE::IDLE);

	CCamera::GetInst()->SetZoomIn(false);


	// Job Scene에선 대화가 끝난 NPC (Monster)를 삭제할거임
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();


	vector<CObject*>& vecObjMon = pCurScene->GetMonsterGroup();


	for (size_t i = 0; i < vecObjMon.size(); ++i)
	{
		CMonster* pMon = dynamic_cast<CMonster*>(vecObjMon[i]);

		if (L"Player" != GetNpcOwner()->GetName() && !pMon->IsDead())  // 죽지 않은 객체만 처리
		{
			if (pMon->GetDoingTalk())
			{
				pMon->SetFineTarget(false);
				pMon->SetDoingTalk(false);
				AI* pAI = pMon->GetAI();

				if (pAI)
				{
					CCodeModeState* pCodeModestate4 = (CCodeModeState*)(pAI->GetState(MON_STATE::CODEMODE));

					if (pCodeModestate4 && pCodeModestate4->GetbReTarget())
					{
						pMon->SetPos(Vec2(pMon->GetPos().x + 20.f, pMon->GetPos().y));
						pMon->SetTarGetPos(pMon->GetTarGetPos());						
					}

					if (pMon->IsDissapear_conversation())
					{
						DeleteObject(pMon);
					}
				}	
			}
		}
	}


	DeleteObject(this);
	//CSceneMgr::GetInst()->GetCurScene()->SetMainTalkUI(nullptr);
}


void CTalkUI::ApplyChoiceToPlayer_InFo(CUI* _pBtnUI) // 어떤 능력치를 어떻게 올릴지 알고리즘 생각해봐야함
{
	double* pPlayerInfo = pG_Player->GetPlayer_Info();
	

	// 버튼이 대화를 알아야함
	CBtnUI* pBtnUI = dynamic_cast<CBtnUI*>(_pBtnUI);


	switch (m_CurSceneCount)
	{
	case SCENE_TYPE::MT:
	{
		for (size_t i = 0; i < 3; ++i)
		{
			PLAYER_INFO data = pBtnUI->GetBtnConversation().playerInfo_data.player_Info[i];
			pPlayerInfo[(UINT)data] += pBtnUI->GetBtnConversation().playerInfo_data.Figure;
		}

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // 긍정  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // 부정
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // 싸움
		{
			m_CurConversation.CurButtonType = ButtonType::FIGHT;
		}
	}
	break;

	case SCENE_TYPE::MAINMT:
	{
		for (size_t i = 0; i < 3; ++i)
		{
			PLAYER_INFO data = pBtnUI->GetBtnConversation().playerInfo_data.player_Info[i];
			pPlayerInfo[(UINT)data] += pBtnUI->GetBtnConversation().playerInfo_data.Figure;
		}

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // 긍정  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // 부정
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // 싸움
		{
			m_CurConversation.CurButtonType = ButtonType::FIGHT;
		}
	}
	break;

	case SCENE_TYPE::SEA_MT:
	{
		for (size_t i = 0; i < 3; ++i)
		{
			PLAYER_INFO data = pBtnUI->GetBtnConversation().playerInfo_data.player_Info[i];
			pPlayerInfo[(UINT)data] += pBtnUI->GetBtnConversation().playerInfo_data.Figure;
		}

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // 긍정  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // 부정
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // 싸움
		{
			m_CurConversation.CurButtonType = ButtonType::FIGHT;
		}
	}
	break;
	
	case SCENE_TYPE::DATE:
	{
		for (size_t i = 0; i < 3; ++i)
		{
			PLAYER_INFO data = pBtnUI->GetBtnConversation().playerInfo_data.player_Info[i];
			pPlayerInfo[(UINT)data] += pBtnUI->GetBtnConversation().playerInfo_data.Figure;
		}

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // 긍정  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // 부정
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // 싸움
		{
			m_CurConversation.CurButtonType = ButtonType::FIGHT;
		}
	}
	break;

	case SCENE_TYPE::MAINDATE:
	{
		for (size_t i = 0; i < 3; ++i)
		{
			PLAYER_INFO data = pBtnUI->GetBtnConversation().playerInfo_data.player_Info[i];
			pPlayerInfo[(UINT)data] += pBtnUI->GetBtnConversation().playerInfo_data.Figure;
		}

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // 긍정  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // 부정
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // 싸움
		{
			m_CurConversation.CurButtonType = ButtonType::FIGHT;
		}
	}
	break;


	
	case SCENE_TYPE::STUDY:
	{
		for (size_t i = 0; i < 3; ++i)
		{
			PLAYER_INFO data = pBtnUI->GetBtnConversation().playerInfo_data.player_Info[i];
			pPlayerInfo[(UINT)data] += pBtnUI->GetBtnConversation().playerInfo_data.Figure;
		}

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // 긍정  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // 부정
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // 싸움
		{
			m_CurConversation.CurButtonType = ButtonType::FIGHT;
		}
	}
	break;
	
	case SCENE_TYPE::HIDDENSTUDY:
	{
		for (size_t i = 0; i < 3; ++i)
		{
			PLAYER_INFO data = pBtnUI->GetBtnConversation().playerInfo_data.player_Info[i];
			pPlayerInfo[(UINT)data] += pBtnUI->GetBtnConversation().playerInfo_data.Figure;
		}

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // 긍정  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // 부정
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // 싸움
		{
			m_CurConversation.CurButtonType = ButtonType::FIGHT;
		}
	}
	break;
	
	case SCENE_TYPE::JOB:
	{
		for (size_t i = 0; i < 3; ++i)
		{
			PLAYER_INFO data = pBtnUI->GetBtnConversation().playerInfo_data.player_Info[i];
			pPlayerInfo[(UINT)data] += pBtnUI->GetBtnConversation().playerInfo_data.Figure;
		}

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // 긍정  
		{	
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // 부정
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // 싸움
		{
			m_CurConversation.CurButtonType = ButtonType::FIGHT;
		}
	}
	break;
	
	case SCENE_TYPE::DRINKING:
	{

		for (size_t i = 0; i < 3; ++i)
		{
			PLAYER_INFO data = pBtnUI->GetBtnConversation().playerInfo_data.player_Info[i];
			pPlayerInfo[(UINT)data] += pBtnUI->GetBtnConversation().playerInfo_data.Figure;
		}

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // 긍정  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // 부정
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // 싸움
		{
			m_CurConversation.CurButtonType = ButtonType::FIGHT;
		}

	}
	break;
	
	case SCENE_TYPE::SMOKING:
	{
		for (size_t i = 0; i < 3; ++i)
		{
			PLAYER_INFO data = pBtnUI->GetBtnConversation().playerInfo_data.player_Info[i];
			pPlayerInfo[(UINT)data] += pBtnUI->GetBtnConversation().playerInfo_data.Figure;
		}

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // 긍정  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // 부정
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // 싸움
		{
			m_CurConversation.CurButtonType = ButtonType::FIGHT;
		}
	
	}
	break;
	
	
	default:
		break;
	}
}


void CTalkUI::OnChildBtnClick(CUI* pUI)
{
	 //this는 항상 똑같은 애 일거고 대화창은 1개니까 멤버도 다 똑같은애임
	// pUI는 이 함수를 부른 버튼UI를 뜻함
	static UINT noneId = 0;
	static UINT choiceId = 0;
	static UINT yesId = 0;
	static UINT noId = 0;
	static UINT fightId = 0;
	

	if (m_PreSceneCount != m_CurSceneCount)
	{
		noneId = 0;
		choiceId = 0;
		yesId = 0;
		noId = 0;
		fightId = 0;
	}



	m_PreSceneCount = m_CurSceneCount; // 이전씬이랑 현재씬 동기화
	ButtonType eType = m_CurConversation.CurButtonType;

	if (ButtonType::Choice == m_CurConversation.NextButtonType) // 다음 대화가 초이스상태 이니까
	{
		eType = ButtonType::Choice;
	}

	else if (ButtonType::END == m_CurConversation.NextButtonType)
	{
		eType = ButtonType::NONE;
	}
	else if (ButtonType::FINAL_END == m_CurConversation.NextButtonType)
	{ 

		eType = ButtonType::FINAL_END;		
	}




	switch (eType)
	{
	case ButtonType::NONE:
	{
		m_itextSize = 1;
		m_CurConversation.CurButtonType = ButtonType::NONE;
		noneId++;
		m_CurConversation.id = noneId;
		
	}
		break;

	case ButtonType::Choice:
	{
		// 고려해야할점: 일단 현재대화하는 씬이 어떤 씬인지, 어떤 npc와 어떤 대화를 나누고있는지 그에따라 Player의 어떤 정보를 바꿀것인지 생각해야함
		m_itextSize = 1;
		m_CurConversation.CurButtonType = ButtonType::Choice;

		ApplyChoiceToPlayer_InFo(pUI); // Scene 종류에 맞게 Player 정보 바꿔주는 로직이 담긴 함수
			
		m_CurConversation.id = choiceId;
	}
		break;

	case ButtonType::YES:
	{
		m_itextSize = 1;
		if (ButtonType::NO == m_CurConversation.PreButtonType)
		{
			noId++;
			m_CurConversation.id = noId;
			break;
		}

		yesId++;
		m_CurConversation.id = yesId;		
	}
		break;
	case ButtonType::NO:
	{
		m_itextSize = 1;
		if (ButtonType::YES == m_CurConversation.PreButtonType)
		{
			yesId++;
			m_CurConversation.id = yesId;
			break;
		}

		noId++;
		m_CurConversation.id = noId;	
	}
		break;

	case ButtonType::FIGHT:
	{
		m_itextSize = 1;
		fightId++;
		m_CurConversation.id = fightId;
	}
		break;
	case ButtonType::END:

		break;

	case ButtonType::FINAL_END:
	{
		// 대화가 끝났으면 뒤에 해줄 작업을 함수화 시키자
		noneId = 0;
		choiceId = 0;
		yesId = 0;
		noId = 0;
		fightId = 0;
		
		
		// 대화가 사라지더라도 마지막에 본인이 어떤 선택을 했는지 알아야함
		// 새로 만들어진 대화가 기억하고있던 그것을 참조해야함
		g_LastConversationInfo = m_CurConversation;
		End_of_Conversation();
	}
		

	default:
		break;
	}



	if (m_CurConversation.playerInfo_data.player_Info[0] == PLAYER_INFO::MBTI)
	{
		PLAYER_MBTI mbti = pG_Player->GetMbti();  // 0 ~7 I인사람 8~15 E인사람

		if (0 <= (UINT)mbti && (UINT)mbti <= 7)
		{
			m_CurConversation.id = noneId++;
		}
		else if (8 <= (UINT)mbti && (UINT)mbti <= 15)
		{
			if (m_CurConversation.id == 4)
			{
				m_CurConversation.id = noneId++;
			}
		}
	}

	

}




void CTalkUI::MouseOn()
{
}

void CTalkUI::MouseLbtnDown()
{
}

void CTalkUI::MouseLbtnUp()
{
}

