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
	


	m_pCurScene = CSceneMgr::GetInst()->GetCurScene();  // ���� ���� �°� ��ȭâ �ؽ��� ����

	m_pCurScene->SetMainTalkUI(this);  // Scene���� TalkUI�� ������ 1���̱⶧�� ����

	// Talk UI�� Scene�� ���� ��ǳ�������� �ڱ� �Ӹ����� ���ִ� ��ȭ UI�� �ɼ����ְ�
	// Center �ϴܿ� ��ġ�ϴ� �簢�������� ��ȭ UI�� �ɼ��� ����  �̰� ���� ������ ������ ����

	if (L"Loding2 Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::LODING_2;

		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n\n\n            ���� �����̴� �ʵ��б��� ���� �׻� ���� �پ�ٴϴ� �׸�����̴�.\n\n            ������ ���� �����̴� ���� ����� ���ϱ��� �����ϸ� ��â������ �Բ��ߴ�.\n\n            ������ ����б��� �����̴� ���� �Ѿ� ���б��� �����Ͽ���,\n\n            ���� ����� �ٷ� ���� ������߱⿡ ���б� ������ �����ϰ� ����غ��Ͽ���.\n\n            �Ͽ� ġ�����ִ� ���� �������� �����̰� ����;� ������ �ϰԵǾ���.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n\n\n            ��: ��������? ����� ���� ����?\n\n            ������: �� ���� �б� ģ�����̶� ����־�!\n\n            ��: ��.. �׷� �̹� �ָ�����??\n\n            ������: �ָ����� �� MT���� �ȵɰͰ�����..?\n\n            ��: ��..�׷�..? MT���� ���ϴµ�?", ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n\n\n            ������: ��.. �� �� ����,����� ���� ��⵵�԰�\n            ���� �����鼭 ���� �ð������°��� ��..!\n\n            ��: ��.. ��հڴ� ���� MT ������ʹ�.. �� ���� ȸ�翡�� �Ϲ����� �����̾��µ�..\n\n            ������: ���ִµ�?? ������� �������� ���ڴ�! �� ���ε��ؾ���..\n            �˹ٵ� �ؾ���.. �̷����� ������ ����",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n\n\n            ��: �׷���.. �� �ʰ� �η��.. ���� ���б� �ٴϰ�ʹ�!\n\n            ������: �ƴϸ� �����̶� ���б� �����غ��°� �?\n\n            ��: �Ƴ�.. �� ������ �����ϰͰ���, �׳� ȸ�糪 ������ �ٳ����..\n\n            ������: �׷��׷� ���߿� �� �ѹ� ���� �ѽž�!\n\n            ��: (�ֽ� ������ô�ϸ�) �׷� ���߿� �� �ð��Ǹ� �主�� ����������! ���� ���! ����",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n\n\n            (��ȭ�� ����ǰ� ����濡 Ȧ�� �����ִ� ���� �����Կ� ������.)\n\n            ��: ��.. ���� ������ó�� ���б��� ������ �����..?\n\n            ��: (�ٴ��� ������ �Ѽ��� ����)\n\n            ��.. ������.....", ButtonType::NONE }));


		m_strTex[(UINT)SCENE_TYPE::LODING_2].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n\n\n\n\n\n\n																						����� MBTI�� �����ϼ���!",ButtonType::NONE,ButtonType::FINAL_END }));

	

	}



	else if (L"Lobby Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::STAGE_LOBBY;

		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"���� �����??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��Ӵ�..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�и� �����Ҹ��� ��ȴµ�..\n",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"���ʿ�\n���� å�ڰ� �ִµ�?",ButtonType::NONE ,ButtonType::FINAL_END}));





		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: �ȳ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ����..����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(���ڱ� �����̰� �ڽ��� �̾߱⸦ �����Ѵ�.)" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: �� ���,������ ��︮�� �� �������." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n???: ������ ������ ���ݾ� ���������� �������." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: ����� ��ȭ�� �� �����븦 �����ϰ�,�������� ��� ����\n�������� �ְ� �޾Ҿ�." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n???: ���� ������ ������ �ְ� �޴ٺ��� �Ŀ�,\n ���� ���� ������»���� ����� �ƾ�." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n��: (����..���� ���� �����ε�...) " ,ButtonType::NONE}));  // �� �������� ȭ���� ��ο����� �ٽ� ������� �̷��� �� �ڽ��� �����Ѵ�.
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n???: �� �����ߴ� ��ȸ���� �⸣������ ����鿡�� ���� �ٰ������⵵�ϰ�,\n��︮�� ������� ���� �غ��ϰ��� ������ ��Ҿ�." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: �̸� ���� �� �ٸ� ����鿡�� ��� �޴� ����� �ư�,\n�׷� ������� �����ϰ� ��ư����־�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n��: ....?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n��: ���� �����?!" ,ButtonType::NONE ,ButtonType::FINAL_END}));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: �ȳ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ����..����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(���ڱ� �����̰� �ڽ��� �̾߱⸦ �����Ѵ�.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: �� �ڽ��� �ϴ� �� ����� ���̾���.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n???: ������ ���� � ���� �����Է� ���ư���\n�ݷ��� ���� �ǳ��ְ� �߾�." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: �������� Ű��� ���� ����ϴٺ���\n�� �ȿ� ������ �ڽŰ��� ����������" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n???: �������� �������� � ����� �غ��Ҽ� �ְԵǾ���" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n��: (����..���� ���� �����ε�...) " ,ButtonType::NONE }));  // �� �������� ȭ���� ��ο����� �ٽ� ������� �̷��� �� �ڽ��� �����Ѵ�.
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n???: �� �����ߴ� �������� �⸣������ ���� ������ ��⸦ ��¹���� �����,\n�׷� �絹�� ������� ������� ���� ������" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: �̸� ���� �� �ٸ� �����տ��� ��⳾�� �־���,\n�׷� ������� �����ϰ� ��ư����־�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n��: ....?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n��: ���� �����?!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: �ȳ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ����..����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(���ڱ� �����̰� �ڽ��� �̾߱⸦ �����Ѵ�.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: ���� �տ��� �� ���� ������ �ʾҾ�." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n???: ����� ��ȭ�̶� �� ��¦ �������� �����ϰ� �ൿ����." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: ������ �ذ��ϸ鼭 �� ���� �ذ�å�� ã��,\n�ڽŰ��� ��� �Ǿ���." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n???: � ������ �ذ��� �� �ִٴ� ������ ����,\n������ ���ư��� ���� ����� ��ԵǾ���!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n��: (����..���� ���� �����ε�...) " ,ButtonType::NONE }));  // �� �������� ȭ���� ��ο����� �ٽ� ������� �̷��� �� �ڽ��� �����Ѵ�.
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n???: �� �����ߴ� �����ذ�ɷ��� ������ �¼���,\n�׷� ����� ����� ��Ȳ�� ������ �׻� �ذ��Ͽ���" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: �̸� ���� �� ���ڽ��� �� �ϰԵǾ���,\n�׷� ������� �����ϰ� ��ư����־�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n��: ....?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n��: ���� �����?!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: �ȳ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ����..����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(���ڱ� �����̰� �ڽ��� �̾߱⸦ �����Ѵ�.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: ���� ������ �� å���� �� �غ� �Ǿ� �־���." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n???: �ٸ� ������� ������ �� �ִ� ���簡 �ǰ�;,\n������ å���� ������." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: å�Ӱ��� ������ �ൿ�ϴٺ���,\n�� �ڽŵ� �������� ����� �Ǿ���." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n???: �ڽŰ� �ְ� å���� �� �� �ִٸ�\n� ������ �غ��� �� �����ž�." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n��: (����..���� ���� �����ε�...) " ,ButtonType::NONE }));  // �� �������� ȭ���� ��ο����� �ٽ� ������� �̷��� �� �ڽ��� �����Ѵ�.
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n???: �� �����ߴ� å�Ӱ��� �⸣������ �־��� ��Ȳ�� �׻� �ּ��� ���ϰ�,\n�̸� �ܸ����� �ʰ� å������ ����߾�." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: �̸� ���� �� �ٸ� ����鿡�� �ŷڸ� �����,\n�׷� ������� �����ϰ� ��ư����־�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n��: ....?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n��: ���� �����?!" ,ButtonType::NONE ,ButtonType::FINAL_END }));








		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: �ȳ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ����..����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(���ڱ� �����̰� �ڽ��� �̾߱⸦ �����Ѵ�.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: ���� ���л�Ȱ�� �ϸ鼭 �׻� �������.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: ��??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: ������� ������ ���ġ�� �׵��̶� ��︮�°� �������.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n��: ....�����ƿ�..! ��ٺ��� �׷����� ����!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n???: �� �λ����� �����Ҽ� ���� ����ΰ���.." ,ButtonType::NONE }));  
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n��: (�����ϸ�) �ƴϿ���!! ����� ����� ��ġ�ִ»���̿���!\n�������� �׷����ؿ�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: ��..����..�׷���..�� ��ȸ����ŭ�� ����� �⸥�Ͱ���" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n��: �׷���!! ����������!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n��: �ٵ� ����..����?" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: �ȳ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ����..����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(���ڱ� �����̰� �ڽ��� �̾߱⸦ �����Ѵ�.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: ���� ���л�Ȱ�� �ϸ鼭 �׻� �������.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: ��??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: ������� ������ ���ġ�� �׵��̶� ��︮�°� �������.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n��: ....�����ƿ�..! ��ٺ��� �׷����� ����!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n???: �� �λ����� �����Ҽ� ���� ����ΰ���.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n��: (�����ϸ�) �ƴϿ���!! ����� ����� ��ġ�ִ»���̿���!\n�������� �׷����ؿ�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: ��..����..�׷���..�� ��������ŭ�� ����� �⸥�Ͱ���" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n��: �׷���!! ����������!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n��: �ٵ� ����..����?" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: �ȳ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ����..����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(���ڱ� �����̰� �ڽ��� �̾߱⸦ �����Ѵ�.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: ���� ���л�Ȱ�� �ϸ鼭 �׻� �������.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: ��??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: ������� ������ ���ġ�� �׵��̶� ��︮�°� �������.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n��: ....�����ƿ�..! ��ٺ��� �׷����� ����!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n???: �� �λ����� �����Ҽ� ���� ����ΰ���.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n��: (�����ϸ�) �ƴϿ���!! ����� ����� ��ġ�ִ»���̿���!\n�������� �׷����ؿ�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: ��..����..�׷���..�� �����ذ�ɷ¸�ŭ�� ����� �⸥�Ͱ���" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n��: �׷���!! ����������!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n��: �ٵ� ����..����?" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: �ȳ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ����..����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n		(���ڱ� �����̰� �ڽ��� �̾߱⸦ �����Ѵ�.)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n???: ���� ���л�Ȱ�� �ϸ鼭 �׻� �������.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: ��??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n???: ������� ������ ���ġ�� �׵��̶� ��︮�°� �������.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n��: ....�����ƿ�..! ��ٺ��� �׷����� ����!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n???: �� �λ����� �����Ҽ� ���� ����ΰ���.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n��: (�����ϸ�) �ƴϿ���!! ����� ����� ��ġ�ִ»���̿���!\n�������� �׷����ؿ�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n???: ��..����..�׷���..�� å�Ӱ���ŭ�� ����� �⸥�Ͱ���" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n��: �׷���!! ����������!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(-9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n��: �ٵ� ����..����?" ,ButtonType::NONE,ButtonType::FINAL_END }));





		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n�̷��� ��: �ȳ� �ѽž�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ..? �̷��� ��..? �̰� ��ü.. ������Ȳ����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n�̷��� ��: �� ���ݵ� �Ǹ��ϰ� �����ϰ� �ִ°Ͱ���" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n�̷��� ��: ���翡 ����ϰ� ����ó�� ������ ���\n�װԹٷ� �ǹ��ִ� ���̾�" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n�̷��� ��: ���� �� ª�� ���������� Ư���ϰ� ����� �ູ�ϰ� ���!" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n					 (�˼����� ��п� ���� Ǯ����...)" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n�̷��� ��: �ȳ� �ѽž�..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ..? �̷��� ��..? �̰� ��ü.. ������Ȳ����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n�̷��� ��: �� ���п� ���̳�,, �������༭ ����!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n�̷��� ��: �� �ʿ��� ������ �Ͼ�Ͽ�\n���ݴ� �����ϰ� �ùٸ� ������ ��������," ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n�̷��� ��: �� ��� �̹� �ð��� �귶����..\n�� ���� ��ȸ�� ���ݾ�? ���� �� ������ ���!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STAGE_LOBBY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n					 (�˼����� ��п� ���� Ǯ����...)" ,ButtonType::NONE,ButtonType::FINAL_END }));
	}

	
	else if (L"Job Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::JOB;

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4594, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n���� ������ ���ݾ�?!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4594, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n(�Ŵ������� ���� �ɾ��\n�ҰͰ���)",ButtonType::NONE ,ButtonType::FINAL_END }));

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n�Ŵ���: �ȳ� �ʰ� �̹��� ����Ÿ������ ���� �ѽ��� ����!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ��, �Ŵ����� �ȳ��ϼ���!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n�Ŵ���: ������ �˹ٴ� ó���̴�?",ButtonType::NONE , ButtonType::Choice }));



		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice })); // ��ư ���� (�� / �ƴϿ�) 
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��.. ó���̿���!",ButtonType::Choice })); // ���� �ҷ�����
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 2,L"�ƴ� �˹� �غ����־��!",ButtonType::Choice }));
		



		// �� ���ý�///
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n�Ŵ���: �׷� �ϴ� �� �����븶�� � ��ǰ�� �ִ��� ���캸��.\n ���� ���̵���� ��ŰƮ, �ﰢ���, �Ｎ��ǰ, ����ǰ, �߰���\n ���̽�ũ�� ��,�õ���ǰ, ����ؿ� ����, ���,������, ��������ǰ���� �ְ�,\n �������� �ִ� �����뿡�� ���� �� ����ǰ ���� �־�.",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n�Ŵ���: ���ϴϱ� ���ƺ�����?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: ��...... �ʹ� ���ƿ�....",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n�Ŵ���: ������ �׷��� ��ġ�� �뷫���� �͸� �˸� ��.",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: �� ���� ....",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n�Ŵ���: ������ ��� ����� �̷��� �ϴ°Ű� ���� �����\n �մ��� ������ �̿��ϰ� �ϸ� ��.\n �� ���� �͵��� ��Ʈ�� �ڼ��ϰ� �����־�!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n��: ���� �����մϴ�!",ButtonType::YES ,ButtonType::END}));
		/////////////



		// �ƴϿ� ���ý� //
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n�Ŵ���: ��~ �����������~ �׷��� ����� �ٸ����̶� ���� �ٸ���~\n",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ��,, ����̿�,,? ",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n�Ŵ���: �װ� �ʰ� ���ϸ鼭 ���� �����غ�~~",ButtonType::NO ,ButtonType::END }));
		/////////////////
	



		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n�Ŵ���: �׸��� �մԵ��� ��ǰ �����ϰ� ��ǰ�� �����뿡 ������\n�ڿ� â�� ���� ä�� ���⸸ ���ָ��. ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: ���� �˰ڽ��ϴ� ! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n�Ŵ���: ���л��̶� �뵷 ������ �˹��ϴ°ž�? ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n��: �뵷�� ���� ���赵 �װ� �; �ϴ°ſ���. ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n�Ŵ���: ����ϳ� ~!! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n��: ����.. ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n�Ŵ���: �ٵ� �츮 �������� ���鿡��\n�ִ����� ���񽺸� ��������� �ؼ� �մԵ��� ��� ������� ��. ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"\n��: �ʿ��� �� ������ ���� ���� ���񽺸� �����϶�� �����Ͻô°���? ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"\n�Ŵ���: �� �¾� ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"\n��: ( ���� �������� �̷�... ) ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 13,L"\n�Ŵ���: Ȥ�� �ñ��� �� �� ������ �������� ���ص� ��! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 14,L"\n�Ŵ���: �� ����Ÿ���� ��ǰ �����°� ���\n��������� �� �ص� ��! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 15,L"\n��: �˰ڽ��ϴ� ! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 16,L"\n�Ŵ���: �׷� �� ��Ź�Ұ� ! ",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 17,L"\n��: ��~ ",ButtonType::NONE , ButtonType::FINAL_END }));
		//////// �Ŵ����� ��ȭ ����//////////////





		// <�����Ӹ� �մ԰� ��ȭ> //
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: �ȳ��ϼ��� Ȥ�� �ʿ��� �� ��������?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n�մ�1: ������ ����� �ϴµ� ������ ��ŰƮ�� ������?",ButtonType::NONE ,ButtonType::Choice,ButtonType::NONE,true }));
										

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice })); // ã�ƺ���, ��ã�ƺ���
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true , 0.f}, 1,L"(������ �ϸ�) �� �մ� ���� �����뿡 ������ ���°ſ���..",ButtonType::Choice })); 
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true , 0.5f}, 2,L"(���� �� �� �θ����Ÿ��� �´�) ã�� �ôµ� ���׿�~",ButtonType::Choice }));  
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true,1.f}, 3,L"(�Ŵ������� ��ȭ�� �ϸ�) ��ø���~",ButtonType::Choice })); 
										


		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n�մ�1: �� �׷� ��¿�� ����~ �����մϴ�!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ��~", ButtonType::YES , ButtonType::FINAL_END }));
												   
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n�մ�1: �� �׷� ��¿�� ����~ �����մϴ�!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ��~", ButtonType::NO , ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"             (��ȭ�� ����ȴ�)",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: �Ŵ����� Ȥ�� ������ ��ŰƮ ��� �ֳ���?",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n�Ŵ���: â�� ��� �ٷ� ���� �־�!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n��: �˰ڽ��ϴ�~ �����ؿ� !!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n�Ŵ���: �� ~����",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: (������ ��ŰƮ�� ���� �ͼ�)\n ���� �ֽ��ϴ� �մ�!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n�մ�1: �����մϴ�~", ButtonType::FIGHT}));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n��: ��~", ButtonType::FIGHT , ButtonType::FINAL_END }));



		// <�����Ӹ� �մ԰� ��ȭ ����> //



		// <��Ʈ�Ӹ� �մ԰� ��ȭ> //
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: �ȳ��ϼ��� Ȥ�� �ʿ��Ѱ� ��������?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n�մ�2: ������ ����� �ϴµ� Ȥ�� ���� �ִ°� �����ΰ���?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: ��, �մ� �����ִ°� ���ο���",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n�մ�2: �׷� Ȥ�� ���� �����ʵ� ���� �Ȱ� �ֳ���?",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true }));
												   

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice })); // �Ŵ������� �����, �׳� ȥ�� ��ó�Ѵ�
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true,1.f}, 1,L"(�Ŵ������� ��ȭ�� �ɸ�) ���� �����ʴ� �����Ǹ��ϰ� ���� ������ ������.. ��ø���...",ButtonType::Choice })); // å�Ӱ� ����
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true, 0.f}, 2,L"(���� ������ ����) �ű� ������ ���� �ſ���~",ButtonType::Choice })); // å�Ӱ� ����
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true,0.5f}, 3,L"(�������� ã�ƺ���) �����ʴ� ���� ���� �� ���׿�",ButtonType::Choice })); // å�Ӱ� ����


												   
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ��������? ",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n�Ŵ���: ��? ���� ���̾�?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: ���� �����ʸ� ã�� ��ż� Ȥ�� ����ִ��� �˷��ֽǼ� �ֳ���?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n�Ŵ���: ���� ������ �ٷ� �Ʒ� ������ �־�!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: ��!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n            (��ȭ�� ���´�)",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n��: ���� �ֽ��ϴ� �մ�!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n�մ�2: �����մϴ�~",ButtonType::YES ,ButtonType::FINAL_END}));
												   
												   
										   

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n�մ�2: �� �׷���? �˰ڽ��ϴ�..",ButtonType::NO ,ButtonType::FINAL_END}));

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n�մ�2: �� �׷���? �˰ڽ��ϴ�..",ButtonType::FIGHT ,ButtonType::FINAL_END }));

		// <��Ʈ�Ӹ� �մ԰� ��ȭ ����> //






		// <����Ӹ� �մ԰� ��ȭ> //
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: Ȥ�� ã�� ��ô� �� �ֳ���?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n�ǹ��� �ҳ�: .....",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: ...?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n�ǹ��� �ҳ�: (�߾�Ÿ���...)\nģ���� ������ ����...",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: (�͸� ����̸�)\n �� ??",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n�ǹ��� �ҳ�: ��Ÿ��.. ���� ģ���� �Ǿ��ְھ�...?",ButtonType::NONE ,ButtonType::Choice}));


		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ...(����� �ϴ°���..?)",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.5f}, 1,L"(�� �� �ڼ��� �����.)",ButtonType::Choice })); // ���� �ذ�ɷ� ����
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true, 0.f}, 2,L"(�����ϰ� ī���ͷ� ����.)",ButtonType::Choice })); // ���� �ذ�ɷ� ���� 
		


		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: (���� ��Ÿ��� ���� �� �����ָ�..)\nȤ�� ��Ÿ�� �̰� �³���?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n�ǹ��� �ҳ�: ����..! ��Ÿ�� ���� ������ ������ �����ִ°ųĴ�...!!!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: ����...",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n�ǹ��� �ҳ�: ��~~ ī����! ����� �� ������ �����̶��!!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: ����..",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: (����� ���ư���..)\n���� �� �����..?",ButtonType::YES ,ButtonType::FINAL_END}));


		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n�ǹ��� �ҳ�: (���� ������)\n ���� �ƹ��� ������ ������ �� �����ִ°ųĴ�..",ButtonType::NO}));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: (��Ȳ���ϸ�)\n ...?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(3, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n             (�ǹ��� ���ڰ� ����� ǥ������ ���Ը� ������)",ButtonType::NO ,ButtonType::FINAL_END}));
		// <����Ӹ� �մ԰� ��ȭ ����> //





		// <���� �մ԰� ��ȭ> //
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: �������!",ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n����: (������...)\n���� ���˹��� ģ����..(����!..) ����~",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (�Ǿ�..?)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n����: ���� �ϳ� ������! (����!..)",ButtonType::NONE ,ButtonType::Choice , ButtonType::NONE, true}));

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ...",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true, 0.5f}, 1,L"(��ġ����..) ���� ���� �ڳʿ� �ֽ��ϴ�",ButtonType::Choice })); // YES
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true,1.f}, 2,L"(���� ������..) ���� ������ �帱�Կ�.",ButtonType::Choice }));  // NO  å�Ӱ�����,��ȸ������
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true, 0.f}, 3,L"(�޹��� �ϸ�..) �ϰ� ������!! ��� �����Ͷ� �����!!",ButtonType::Choice })); // �ο�� // ��ȸ������
												   

		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n����: ���� ���� ��������� !?\n�˹ٸ� �װ� ���� �����;���!! �����..(����!)",ButtonType::YES ,ButtonType::Choice ,ButtonType::NONE, true }));




		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n����: �ʰ� �����ϴ� �ɷ� �ƹ��ų� ������!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: (���� ������)\n�� �մ�~",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n����: (������.. ����!)",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n��: ��! 1800���Դϴ�!\n���� ��� ���� �� �����̳�����!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n����: (������ ��Ǫ����...)\n����� !?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: (��Ȳ���ϸ�..)\n��..�ƴϿ���..",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n����: (ī�带 ������...)\n�����",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n��: ... ���Ǿ����ϴ� �ȳ���������..",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n����: (������ ������..)\n���� ���˹��� ģ����..(����!)..����~",ButtonType::NO,ButtonType::FINAL_END }));






		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n����: (���� ������..)\n��..!? �����?",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n����: (������ϸ�..)\n�� �մ����� �׵����� ���ϴ� �� �¾�?",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (°������...)\n����̾߸��� ���԰� ���ͼ� ��� �������̾�!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n����: (ȭ������)\n�� ������ �־� ���� ���� ������� ��! ����!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: (�ż��ϰ� 112�� ������..)\n��������? ���� �ູ �������ε���.\n������ �밴�� ������ �ǿ�� �־��,",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n����: (��Ȳ���ϸ�..)\n��..��..! �̰� ��� �Ű�..!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n����: (���ϰ� ������..)\n���� �� ������ ���⸸ �ϸ� �����ȵ�!!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n��: ��....",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::JOB].insert(make_pair(4, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n��: ���� ù ����ε� �Ŵ��������� ��������...",ButtonType::FIGHT ,ButtonType::FINAL_END}));
		// <���� �մ԰� ��ȭ ����> //





		m_MaxTextcount = 28;
	}

	else if (L"Drinking Scene" == m_pCurScene->GetName() || L"Toilet Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::DRINKING;

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n(�Ѹ�������....)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n(�Ѹ�������....)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n(����)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\nģ��2: ��������?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: �� �� ��ȭ�߾� ?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\nģ��2: ���� ������ȸ�ݾ�! �б��տ��� ���Դµ� �ʵ� �÷�?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n��: �ƾ�! �´�! ���� �����Ƿ���..?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\nģ��2: �ֵ��� �ʶ� ģ������ �ʹٰ� �ϴϱ�\n ���������� ����� ��!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n��: �˰ھ�!!",ButtonType::NONE ,ButtonType::FINAL_END }));
		


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n???: ������ ��..?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n         (�ڸ��� �߸� ã���� �ϴ�..)",ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n(...��������...)",ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\nģ��2: (ģ������ ȯ�����ش�)\n��! �Դ�!",ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\nģ��4: �ʰ� �ѽ��� ����! �̾߱� ���� �����!!",ButtonType::NONE }));

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n��: �ȳ� �� ���ѽ��̾�~! ���� ���ϱ� �ʹ� ���� ����~\n  ģ������;���!����",ButtonType::NONE })); // MBTI  E �����
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: �ȳ� �ݰ���.. �ֵ��.. ������,, ��,,�ݰ���!",ButtonType::NONE })); // MBTI  I �����
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n             (���� �ڱ� �Ұ��� �ϸ� ����Ѵ�.)",ButtonType::NONE, ButtonType::Choice}));
		

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::ALCOHOL_TOLERANCE},true, 1.f}, 1,L"(���� �ֵ��ؼ� ģ�����̶� ��������߰ڴ�..!)",ButtonType::Choice })); // YES  // ��ȸ��,������,������ �� ��� �ö���ϴ���
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::ALCOHOL_TOLERANCE},true ,0.f}, 2,L"(�������� ���� �� ���ϰڴµ�..)",ButtonType::Choice })); // NO

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n            (����ϴ� �ֺ� ģ������ �̾߱⳪ ����..)",ButtonType::NO, ButtonType::FINAL_END,})); // NO


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��:(ģ���� ��ġ�� ���Ǹ�)\n��ġ���� 1 !!!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\nģ��1: �� ����~",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\nģ��1,ģ��2: (���ÿ�)\n 2 !!!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\nģ����: ��~~ �ɷȳ�~~ ���꼦 ! ���꼦 !",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\nģ��1,ģ��2: ��������!! ",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: ���� ~",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: (�ų��ϸ�)\n �ϴÿ��� ������ �䳢�� �ϴ¸� !!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\nģ���� ����: ��ġġ!! ��ġġ!!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\nģ���� ����: �ٴ� �ٴ�!!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: ��� ���!!",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\nģ��1: ��~ �ɷȳ� !! ������",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: (�ܲ��ܲ�..)\n��..�� ���Ѵ�..",ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n        (...���� �� ���� ���� ������ ����������...)",ButtonType::YES, ButtonType::FINAL_END})); // YES // ȭ��Ƿ� ��� ��ȯ


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n        (��Ʋ�Ÿ��� ģ��2�� ȭ��Ƿ� ���´�)",ButtonType::NONE ,ButtonType::NONE,ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\nģ��2: (�������� �ϸ�..)\n����.. ����!.. �..��?",ButtonType::NONE ,ButtonType::NONE,ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (���� Ǯ�� ���·�..)\n�ʹ�...���ƽ�...",ButtonType::NONE ,ButtonType::NONE,ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\nģ��2: (�Һ��� ����..)\n �� ~~ ��!",ButtonType::NONE ,ButtonType::NONE,ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: (ģ���� ������ �峭�� ģ��)\n��������������������  ������ ������!!!",ButtonType::NONE ,ButtonType::NONE,ButtonType::YES })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n        (ģ���� �峭�� ġ�� �Һ��� ���� �߿� ���ڱ� ���� ���´�)",ButtonType::NONE ,ButtonType::FINAL_END,ButtonType::YES })); // YES
		//////////////////////////////// ȭ��� ���� ����////////////////////////////////////////////////////////////////////////////////////


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n����: �� �峭ġ�� ���� �� �� �� ������ ���� ���ϴϱ�",ButtonType::NONE ,ButtonType::NONE})); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\nģ��2: (�ڸ� ���ƺ���)\n �ƾ� .. ��..",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE , true })); // YES

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true, 0.f}, 1,L"(������ °������..) �ϰ� ���� ����̾�?!",ButtonType::Choice  })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true, 1.f}, 2,L"(ģ���� ���� �����Ѵ�) ģ���� �����ϱ� ����..",ButtonType::Choice })); // NO
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::AMOUNT_RESPONSIBILITY},true, 1.f}, 3,L"(�ٰ����� ���ڸ� ����) �� ����� ���߳�",ButtonType::Choice })); // FIGHT

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n����: (���ڱ� ������ϸ�)\n �� �� �ڽľ�?",ButtonType::YES}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: �ʸ鿡 ��� �ݸ��̾� �� �ڽľ�",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n����: (������ �ٰ�����)\n �װ� �;�?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n(ģ��2�� ��Ȳ�� �̻������� ������)",ButtonType::YES  }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n(�� ���� ģ������ ���� ������ ��Ȳ��	 ����ȴ�)",ButtonType::YES,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n(����..? �� ������ ���� ��Ʈ���� �� �ȴ�...)",ButtonType::NO  }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n����: (���ڱ� ������ϸ�)\n �� �� �ڽľ�?",ButtonType::NO  }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: �ʸ鿡 ��� �ݸ��̾� �� �ڽľ�",ButtonType::NO  }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n����: (������ �ٰ�����)\n �װ� �;�?",ButtonType::NO  }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n(ģ��2�� ��Ȳ�� �̻������� ������)",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n(�� ���� ģ������ ���� ������ ��Ȳ��	 ����ȴ�)",ButtonType::NO,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: (�����ϴ� ������)\n���� ���ϼ̳׿�",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n����: (�հ��� ���� �ϸ�)\n���̳� �Ծ�",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (...?)",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\nģ��2: (...?)",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\nģ��2: (�����ϸ�)\n�߾�,, �׳� ������",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: (�Ѽ��� ����)\n�׷�..;;",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n����: (ū ��Ҹ���)\n���� ������ ���̾�!!..!",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n          (ȥ�㸻 �ϴ� ���ڸ� �ڷ��ϰ� �ڸ��� ���Ѵ�)",ButtonType::FIGHT ,ButtonType::FINAL_END }));

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n          (��������..)",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n          (ģ������ ������ �̻��� �̾߱⸦�Ѵ�)",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\nģ��2: (��ġ�� ���Ǹ�)\n �� ���� ���̾�?",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n��: (���� ������ ����)\n �ƴ�..",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\nģ��1: (ģ��2�� �ǵ��)\n �׷� �� �� �����",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: (ȣ���ϰ� ������)\n ������� �� ~����",ButtonType::NONE,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\nģ��2: �� �̻����� ��� ��?",ButtonType::NONE,ButtonType::Choice,ButtonType::NO }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true, 1.f}, 1,L"���� ũ�鼭 ����� ����! ",ButtonType::Choice, ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"���� �����鼭 �ܼ��� ����! ",ButtonType::Choice, ButtonType::NONE,ButtonType::NO }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\nģ����: (�Ǿ����ϴ� ǥ������)\n �峭ġ����~~ �����ϰ�!!",ButtonType::YES ,ButtonType::Choice ,ButtonType::NO }));

		
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ��..��� �Ϳ��� ��Ÿ���� ������!",ButtonType::NO  ,ButtonType::NONE ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\nģ����: (�����ϸ�)\n ���� �ʵ� ���ڱ��� ����",ButtonType::NO  ,ButtonType::NONE ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n         (ģ����� �����ϰ� ��ȭ�� �ϸ� ���� �������)",ButtonType::NO ,ButtonType::FINAL_END ,ButtonType::NO })); // ȭ��Ƿ� ȭ����ȯ




		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n         (ģ���� ȭ��Ƿ� ���´�)",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO })); // YES
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\nģ��2: (�������� �ϸ�..)\n ����.. �..��?",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: �ʹ� ����־��� ���� �ٵ� �� ������..?",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\nģ��2: (���� �ﷷ�Ÿ���..)\n �� ����..����..!",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n        (���ڱ� ���⸦ ���� ���� ������) ",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: ??",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n��: (�����ϸ�)\n �ֱ׷�..?",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\nģ��2: (�۶�����..)\n �ƴϾ� ����.....",ButtonType::NONE ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\nģ��2: (���� Ʋ����� ����� �޷�����..)\n ���!!!!",ButtonType::NONE ,ButtonType::FINAL_END,ButtonType::NO }));



		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: (�����ϸ�)\n �ֱ׷�..?",ButtonType::NONE ,ButtonType::Choice,ButtonType::NONE,true}));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::AMOUNT_SOCIABILIT},true, 0.5f}, 1,L"(������ ��Ǫ����) �׷��� ���ݸ�����..",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::AMOUNT_SOCIABILIT},true, 1.f}, 2,L"(���� ��ڿ��ָ�) �׷��� ���ݸ�����..",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::AMOUNT_SOCIABILIT},true, 0.f}, 3,L"(�������ϴ� ǥ������ �ڸ��� ���ϸ�) ��.. ���������� �ٰ�..",ButtonType::Choice }));



		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\nģ��2: (������ �۽��̸�)\n �̾�.. �ʹ� ���� ���̳���..",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ������ �׷� ���� ����~",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: �ڸ��� ���� �� �� ������",ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::NONE,false,true }));


		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\nģ��2: (������ �۽��̸�)\n �̾�.. �ʹ� ���� ���̳���..",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ������ �׷� ���� ����~",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: �ڸ��� ���� �� �� ������",ButtonType::NO ,ButtonType::FINAL_END  ,ButtonType::NONE,false,true }));




		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\nģ��2: (������ �۽��̸�)\n �̾���..",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\nģ��2: (��ġ�� ���Ǹ�)\n ���� ���� ó�� ���ź��� �������� �����Ұ�..",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (�Ѽ��� ����)\n �ƴϾ� ���� �����ϰ� ����\n ���� �� ������",ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\nģ��2: (��ġ�� ���Ǹ�)\n �̾���.. �󱼸� �İ� ����..",ButtonType::FIGHT ,ButtonType::FINAL_END  ,ButtonType::NONE,false,true }));
		




		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		// ���� ���� �󸶳� ���߳Ŀ� ���� �ٸ��� ǥ���ؾ���
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\nģ��3: ȭ��ǿ��� ������ �־���?\n ���̷��� �ȿ� ������",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ���� �Ƴ� ������ ���� �����..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\nģ��3: �ۿ� �ٶ��̳� ����� �׷�~",ButtonType::NONE ,ButtonType::Choice}));

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"(���������µ� ��ħ ������;���)",ButtonType::Choice}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(��.. �������� �׳� ����������..)",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: �� �׷� ��� �ٳ�ð�!",ButtonType::YES ,ButtonType::FINAL_END}));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: �Ƴ� ���� ���� �����ٿ���!",ButtonType::NO ,ButtonType::FINAL_END}));





		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\nģ����: �� �Ծ� ?\n ���� ��ٷ��ݾ�~",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ��,,���� �ʳ׵� ���� ���ž�?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\nģ����: ���� ����������~ ���� ��վ��� ����",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n��: �׷��׷� ���� �������߰ڴ�\n �츮 ������ �� ������ ����!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DRINKING].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n             (ģ����� �λ��ϸ� ���ڸ��� ����ȴ�)",ButtonType::NONE ,ButtonType::FINAL_END , ButtonType::NONE ,false,true }));


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	else if (L"Smoking Scene" == m_pCurScene->GetName())
	{
		// case  1  ( ������ )
		m_CurSceneCount = SCENE_TYPE::SMOKING;

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n          (�ÿ��� ���Ⱑ �ޱ��� ���� ������)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: (ȥ�� ���ý� ������ ȥ�㸻�� �߾�Ÿ���)\n�̰� ���� ��Ȱ�ΰ�.. ������� ����� ���� ����",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (�� �ϴ��� �ٶ󺸴� �ٴ����� �Ӹ��� ���̸�)\n�� ...",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n        (���� �Ա����� �̻����� ����� ���ڰ� ���´�)",ButtonType::NONE ,ButtonType::FINAL_END }));

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: (���� ���� ������ ���� �ݴ´�)\n!!?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n����: (�ڵ����� ������ ��迡 ���� ���δ�)\n����... ��...",ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.5f}, 1,L"�ϴ� �ƹ� ���� ���� �ٰ�����.",ButtonType::Choice }));  // YES
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME},true,1.f}, 2,L"�����͸� ������ô �ٰ�����.",ButtonType::Choice }));    // NO




		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: (��ŭ��ŭ �ٰ��� �տ� ����)\n ...",ButtonType::YES}));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n����: (��ĩ ���� ǥ������)\n��������..?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (���ý� ������) ������ ����!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n����: ...?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n����: �����մϴ�..",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: (���� ���׽� �Ĵٺ��� �帰 ��������)\nȤ��..����ģ��..�־��..?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n����: (��Ȳ���ϴ� ǥ������)\n��.. ��.. �� ����ģ�� �־��",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n��: (�Ǹ��� ǥ������)��.. �˰ڽ��ϴ�..",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n����: ��...",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"\n        (���ϰ� ��ȭ�ϴ� ô�� �ϸ� ���ڰ� �þ߿��� �������)",ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::NONE,false,true }));
		// (������ �϶�)




		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: (���ݼ�ŭ �ٰ�����)\n ��...�����!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n����: (�̾����� ����)\n��..?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (��û�Ÿ���)\n������ �� �����ֽǷ���?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n����: (��� �ӹ��ϴٰ� �����͸� �ǳ��ظ�)\n��..��..�����",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: (ū �Ҹ��� 90�� �λ縦�ϸ�)\n�����մϴ�!!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n����: (�Ӿ����ϸ�)\n�ƴϿ��� ����",ButtonType::NO  ,ButtonType::FINAL_END }));




		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ��.. ������ �߽���ϴ� !!", ButtonType::NONE , ButtonType::NONE , ButtonType::NO}));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n����: (�Ϳ��� ��������)\n ���� ����", ButtonType::NONE , ButtonType::NONE , ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (��.. �ʹ� �� �̻����ε�.. �����)", ButtonType::NONE ,ButtonType::Choice , ButtonType::NO , true}));

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice ,ButtonType::NONE , ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.5f}, 1,L"(�帰 �������� �ڵ����� �ǳ׸�) Ȥ�� ��ȣ..��..",ButtonType::Choice ,ButtonType::NONE , ButtonType::NO }));  // YES
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME},true,0.5f}, 2,L"(�帰 �������� �ڵ����� �ǳ׸�) �ν�Ÿ �����ؿ�!",ButtonType::Choice ,ButtonType::NONE , ButtonType::NO }));        // NO
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_SOCIABILIT},true,1.f}, 3,L"(���� �Ĵٺ���) ���߿� Ŀ�� ��� ������ �������ø� �����ּ���!",ButtonType::Choice ,ButtonType::NONE , ButtonType::NO }));        // NO


		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n����: (��Ȳ���ϸ�)\n�˼��մϴ�.. ģ���� ��ٸ��� �־��..",ButtonType::YES,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: (���� ������ ���� ǥ������ ���ִ´�)\n...",ButtonType::YES ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n����: (��� ���� ����)\nġ��...",ButtonType::YES ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n        (������ ��ʸ� �ϸ� ���ڰ� �������)",ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::NO ,false,true }));
		// (������ �϶�)

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n����: (���� Ǯ�� ���� �ٶ󺸸�)\n��.. �� sns �� ���ؿ�.. �˼��ؿ�..",ButtonType::NO ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ��.. �׷� ��ȣ...",ButtonType::NO ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n����: (���� ������)\nģ���� ��ٸ��� �־��..",ButtonType::NO ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n����: (��� ���� ����)\nġ��...",ButtonType::NO ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n        (������ ��ʸ� �ϸ� ���ڰ� �������)",ButtonType::NO ,ButtonType::FINAL_END ,ButtonType::NO ,false,true }));


		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n����: (����� �ϴ� ǥ������)\n �� ��..",ButtonType::FIGHT ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n����: (�ڵ����� ������ ��ȣ�� �Է��Ѵ�)",ButtonType::FIGHT ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (ȯ�ϰ� ������)\n �����ؿ� ���� ��ٸ��� �����Կ�!",ButtonType::FIGHT ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n����: (�ǽ��ϸ�)\n �� ����...",ButtonType::FIGHT ,ButtonType::NONE, ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n        (������ ��ʸ� �ϸ� ���ڰ� �������)",ButtonType::FIGHT ,ButtonType::FINAL_END ,ButtonType::NO ,false,true }));   //  << ������ true tirger�κ���
		// (������ �϶�)






		// case 2
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n        (�ÿ��� ���Ⱑ �ޱ��� ���� ������)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: (ȥ�� ���ý� ������ ȥ�㸻�� �߾�Ÿ���)\n�̰� ���� ��Ȱ�ΰ�.. ������� ����� ���� ����",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (�� �ϴ��� �ٶ󺸴� �ٴ����� �Ӹ��� ���̸�)\n�� ...",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n        (���� �Ա����� �̻����� ����� ���ڰ� ���´�)",ButtonType::NONE ,ButtonType::FINAL_END }));
			
		
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: (���ɽ����� �ٰ����� �̼Ҹ� ������)\n�ȳ��ϼ���, Ȥ�� ������ �� ������ �������?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n����: (������ �ǳ׾��ش�)\n��! �����!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (������ ��ʸ� �ϸ�)\n �����մϴ�!",ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::NAME},true,0.5f}, 1,L"(�����ϰ� ���ϸ�) ���ɽ����� ���� �ɾ��..",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true, 0.f}, 2,L"(�ƹ��� �ൿ�� ���� ������ ������ ���ִ´�) ...",ButtonType::Choice }));
		
		

		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: (�����͸� �ǳ��ָ�)\n�����մϴ�.. ���� ���� ������?",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n����: (���� �����̸�)\n�б� �տ��� �� ���� �� ���� �Ϳ�~",ButtonType::YES}));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: ����.. ���� ���� ���� ó���ͺþ��!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n��: (�δ㽺���� �ٰ�����)\n �׷� ���� ���� �;߰ڳ׿�~!",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n����: (�ڵ����� ���� ��Ÿī�� ��������)\n ��..��..��.. ��..",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n        (��� ������ �帥��)",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n��: (������ ��踦 �¿�� �ڸ��� �����)\n �� �׷� ������ �� �Ŀ�~",ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n����:   (�ڵ����� �ϸ� �� ���� ���� �����δ�)",ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::NONE,false,true }));


		// ������ ����
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n����: (������)\n Ǫ��..!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: ...?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n����: (�Ϳ��� �ϸ�)\n �� ������ �� ��� ������ �輼�� ������",ButtonType::NO })); // << ���� �����ؾ���
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n��: (��Ȳ�ϸ�)\n ��..��.. �����!",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: (�ʹ� �� �̻����ε�.. �����..)",ButtonType::NO , ButtonType::FINAL_END }));




		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: �����!!!",ButtonType::NONE,ButtonType::Choice, ButtonType::NO }));


		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: ",ButtonType::Choice ,ButtonType::NONE,ButtonType::NO}));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME},true,1.f}, 1,L"(���������� �뽬����!) ��¥ �̻ڼ��� ����!",ButtonType::Choice ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::NAME},true,0.5f}, 2,L"(�β������ϸ�) ��..�Ϳ����� ����!",ButtonType::Choice  ,ButtonType::NONE,ButtonType::NO }));
		
		

		
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n����: (��½�� ������)\n����...",ButtonType::YES ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: (������ ������ ���Ǹ�)\n������ �˼��ؿ�..",ButtonType::YES ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n����: (�ڵ����� ���� ���ɾ��ٴ� ǥ������)\n��..��..",ButtonType::YES ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n��: (������ ǥ������)\n������ �� ���� �λ��ҰԿ�!",ButtonType::YES ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n����: (��¦ �Ⱦ��)\n��~",ButtonType::YES ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n        (�θ��Կ� �ڸ��� �����)",ButtonType::YES ,ButtonType::FINAL_END  ,ButtonType::NO,false,true }));
		
		

		
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n����: (��Ȳ���ϸ� ������ ���� ������)\n���K...����",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: Ȥ�� ����ģ�� ��������?",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n����: �����",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n��: (������ ���� �Ѽ��� ������)\n�����̴�!",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: (����ϰ�)\nȤ�� �ʹ� �� �̻����̼ż� �׷��� ��ȣ ���ܵ帱�״ϱ�\n�����ѹ� �ֽǷ���?",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n����: (���� �̼Ҹ� ����)\n��..����",ButtonType::NO ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SMOKING].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n        (ȯ�ϰ� ������ �ڽŰ��� �ٴ´�)",ButtonType::NO ,ButtonType::FINAL_END  ,ButtonType::NO,false,true }));

		// ������ ���
	
	}


	else if (L"Study Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::STUDY;

		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��.. ? ���� ����?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"(���ʿ� ������ å�� �ִ�)",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"����å�� ����?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"�����..",ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"���� ������ ���� ������ ������ִ� å�̴�\n\n1.C����� �ڷ������� int,double,float,char �����ִ�.\n\n2.C����� ���ǹ��� �ݺ����� if��,switch���� ����Ѵ�.\n\n3.C����� �迭�� �����ʹ� ������ �������ִ�.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"���� ������ ���� ������ ������ִ� å�̴�\n\n1-1.�����ڷ����� int,�Ǽ��� double,float,���ڴ� char�̴�.\n1-2.�����ڷ����� �Ǽ��ڷ����� �����Ұ�� ^ĳ����^ �̶�°��� �ʿ��ϴ�.\n1-3.int�� ũ���4Byte,double��8Byte,char��1Byte�̴�.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"���� ������ ���� ������ ������ִ� å�̴�\n\n2-1.if���� ���ǽ��� �Ǻ��ϴ� ������ true,false�� �����Ѵ�.\n2-2.������ C���� true,false�� �����Ƿ� 0��1�� �����Ѵ�.\n2-3.switch�� ����Ұ�� case��� Ű���带 ����ؾ��Ѵ�",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"���� ������ ���� ������ ������ִ� å�̴�\n\n3-1.�迭�� � ������ ������ ���鶧 ����ϸ� ��û���� �����ϴ�.\n3-2.�����ʹ� � ������ ����Ű�� ^�ּ�^ ��� �������ִ�.\n3-3.�����ͷ� � �迭�� ����Ű�� �׹迭�� ��� ��ҿ� �����Ҽ��ִ�.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"���� ������ ���� ������ ������ִ� å�̴�\n\n\n�� �����κ��� ������ �־� ������ �˾ƺ��� ��ƴ�.",ButtonType::NONE ,ButtonType::FINAL_END}));



		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�ɸ��м����� ���� ������ ������ִ� å�̴�\n\n1.������Ʈ�� ������ ������ �ǽ�,���ǽ�,���ǽ����� ��������.\n\n2.��������� ^������ �������^�� Ÿ�ο��Ժδ����ǽ��� �����ϰ԰��´�.\n\n3.�������̸������� ���� ^ġ��^�� �Ҹ��� ���� ��ָ� ���߽�Ų��.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"�ɸ��м����� ���� ������ ������ִ� å�̴�\n\n1-1.���ǽ��� ���ų����� ��κп� �ش��ϸ� �ΰ� �ൿ�� ���Ⱑ �ȴ�.\n1-2.���ǽ��� �ǽİ� ���ǽ��� �߰������� �����鼭 �ΰ����� ���������� �Ѵ�.\n1-3.�ǽ��� �츮�� �ν��ϰ� �ִ� �������� ��Ÿ���� ������ Ȱ���� �ǹ��Ѵ�.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�ɸ��м����� ���� ������ ������ִ� å�̴�\n\n2-1. ^������ �������^�� ����� �ٰž��� Ÿ�ε��� �ڽ��� ����,���� �Ǵ� ���δٰ� �ǽ��Ѵ�.\n2-2. ^������ �������^�� ģ���� ������ ���Ǽ��̳� �ſ뿡 ���Ѻδ����ǽɿ� �����Ѵ�.\n2-3. ^������ �������^�� ���������� ���� ����ڳ� ������Ʈ���� ������ ���� �ݺ������� �ǽ��Ѵ�.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"�ɸ��м����� ���� ������ ������ִ� å�̴�\n\n3-1.^ġ��^�� ���۵Ǵ� ���󿡴� �ϻ��Ȱ�� �ǿ����� ��ĥ������ �Ǹ����� ��������.\n3-2.^ġ��^�� ���,���ο�,�������� �ɷ¿� ���� ��ְ� �߻��Ѵ�.\n3-3.^ġ��^�� ����Ű�� ������ ���ڿ��ߵ�,�����,����� ��� ���������� �ִ�.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"�ɸ��м����� ���� ������ ������ִ� å�̴�\n\n\n�� �����κ��� ������ �־� ������ �˾ƺ��� ��ƴ�.",ButtonType::NONE ,ButtonType::FINAL_END }));
		

		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"���ڼ�� ���� ������ ������ִ� å�̴�\n\n1.��������(����ʻ�):����� �Ҷ��� �ſ�ָ� ���� ���� �¿�ٴ� ���Դϴ�.\n\n2.�������(�����̸�):�����δ� �����ൿ�ϸ鼭 �����δ� ���δٸ� ������ǰ���� ���մϴ�.\n\n3.�������(��������):��������� �ƹ����� ���� ��ȭ�Ӱ� ��ư��� �̻����� ���մϴ�.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"���ڼ�� ���� ������ ������ִ� å�̴�\n\n4.ؿ����˲(��������):�������Գ� �Ѿ��� ��������� ����Ѵٴ� ���Դϴ�.\n\n5.���̿��(��������):ū���� �����ϱ����� ���̸´»���鳢�� �����Ǹ��μ� �δ����� ���մϴ�.\n\n6.������(�뵿����):���� �κп����� �ٸ��� ��ü�����δ� ������ ���մϴ�.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"���ڼ�� ���� ������ ������ִ� å�̴�\n\n\n�� �����κ��� ������ �־� ������ �˾ƺ��� ��ƴ�.",ButtonType::NONE ,ButtonType::FINAL_END }));
		

		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"???�� ���� ������ ������ִ� å�̴�\n\n1.�л����� �Ⱦ��ϴ� ���ڴ� ___�Դϴ�.\n\n2.���󿡼� ���� �߰ſ� �ٴٴ� ___�Դϴ�.\n\n3.�ڵ����� �� �ϰ� ġ�� ___�Դϴ�.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"???�� ���� ������ ������ִ� å�̴�\n\n4.������ ��� ���� ___�Դϴ�.\n\n5.������ ���� ��ģ���� ___�Դϴ�.\n\n6.�������� �ݴ븻��? ___�Դϴ�.",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::STUDY].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"???�� ���� ������ ������ִ� å�̴�\n\n\n\n\n�� �����κ��� ������ �־� ������ �˾ƺ��� ��ƴ�.",ButtonType::NONE ,ButtonType::FINAL_END }));



		
	}

	else if (L"HiddenStudy Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::HIDDENSTUDY;

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n���������� ���� �����̴�\n\n����1: C���� �ٸ��ڷ������� �����Ұ�� ___\n�̶�°��� �ʿ��ϴ�.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"����:ĳ����",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 2,L"����:ĳ����",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 3,L"����:ĳ����",ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�����Դϴ�. �����ܰ�� �Ѿ����.",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::FIGHT ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n���������� ���� �����̴�\n\n����2: C���� ������ ������ ���鶧 ___\n������ϸ� �����ϴ�.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 1,L"����:�迭",ButtonType::Choice}));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 2,L"����:������",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 3,L"����:�ڷ���",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�����Դϴ�. �����ܰ�� �Ѿ����.",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::NO ,ButtonType::FINAL_END}));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::FIGHT ,ButtonType::FINAL_END}));
		////////////////////////////////////////////



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n�ɸ��м����� ���� �����̴�\n\n����1: �������̸Ӹ� ���߽�Ű�� ������ ___\n���ִ�.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 1,L"����:����� ��� ������",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 2,L"����:�ν��� ������",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 3,L"����:������ ���",ButtonType::Choice }));
																 
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�����Դϴ�. �����ܰ�� �Ѿ����.",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::FIGHT ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n�ɸ��м����� ���� �����̴�\n\n����2: ___�� ���ų����� ��κп� �ش��ϸ� �ΰ��ൿ�� ���Ⱑ�ȴ�",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"����:�ǽ�",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 2,L"����:���ǽ�",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 3,L"����:���ǽ�",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�����Դϴ�. �����ܰ�� �Ѿ����.",ButtonType::FIGHT ,ButtonType::FINAL_END }));
		////////////////////////////////////////////



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n���ڼ�� ���� �����̴�\n\n����1: ___�� ū���� �����ϱ����� ���̸´»���鳢��\n���� �Ǹ��μ� �δ����� ���մϴ�.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"����:��������",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 2,L"����:�׸����",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 3,L"����:��������",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(9, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�����Դϴ�. �����ܰ�� �Ѿ����.",ButtonType::FIGHT ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n���ڼ�� ���� �����̴�\n\n����2: ___�� �����δ� �����ൿ�ϸ鼭 �����δ�\n���δٸ� ������ǰ���� ���մϴ�.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"����:�Ͻɵ�ü",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 2,L"����:�����̸�",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 3,L"����:�ѹټ���",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�����Դϴ�. �����ܰ�� �Ѿ����.",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::FIGHT ,ButtonType::FINAL_END }));
		////////////////////////////////////////////



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n�ͼ����� ���� �����̴�\n\n����1: �ڵ����� ���ϰ� ġ��? ___�Դϴ�.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"����:��������",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 2,L"����:������",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 3,L"����:ī��",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::NO ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(11, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�����Դϴ�. �����ܰ�� �Ѿ����.",ButtonType::FIGHT ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"QUIZ_BOOK\n�ͼ����� ���� �����̴�\n\n����2: �л����� ���� �Ⱦ��ϴ� ���ڴ�? ___�Դϴ�.",ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 1,L"����:���̳�����",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.25f}, 2,L"����:å����",ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::AMOUNT_KNOWLEDGE,PLAYER_INFO::NAME,PLAYER_INFO::NAME},false, -0.25f}, 3,L"����:��������",ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::YES ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�����Դϴ�. ��繮���� �ذ��Ͽ����ϴ�.",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n\n������ ���� �Ұ��� ��Ű���?",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n\n�λ��� ��鼭 ���𰡿� �����ϰ�\n�����ϴ°��� ���� �ູ�� �����µ��Դϴ�.",ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(12, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n\n���� ���л�Ȱ �������Ͻð�!\n\n �ȳ���������!",ButtonType::NO,ButtonType::FINAL_END ,ButtonType::NONE,false,true }));


		m_strTex[(UINT)SCENE_TYPE::HIDDENSTUDY].insert(make_pair(10, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ �ƴմϴ�. ó������ Ǫ����",ButtonType::FIGHT ,ButtonType::FINAL_END }));
		////////////////////////////////////////////

	}
	
	else if (L"MT Scene" == m_pCurScene->GetName())
	{
		
		m_CurSceneCount = SCENE_TYPE::MT;
		
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n��: (�� �Ѽ��� ���b����)\n���� ������ ��Ƽ��.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"\n��: (ù ��Ƽ�� �׷��� ���� �� ���� ���̾�..)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"\n��: (ȥ�㸻�� �߾�Ÿ���)\n�ϴ� ì�ܾߵ� �� ���� �־�����.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"\n��: (�߾��߾�)\nġ��,ĩ��,�ӿ�,����,���..." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"\n��: (�ڵ��� �� ����ó�� ����)\n�갡 ���ְ�..�갡 �����̰�..�갡 ������.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"\n��: (���� ������)\n�̸��̶� �󱼵� ��Ī�� �ȵǴµ�..\n���Ͼ�ϳ� �� ����..�Ф�" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"\n��: (����� ǥ������)\n�ϴ� ����..���� ���� �Ͼ����!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"\n��: (...)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"\n��: (...zzZ)" ,ButtonType::NONE ,ButtonType::FINAL_END }));





		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"���� ��ٸ��� MT..!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"�ֵ��� �������?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�� �ָ� ������\n���°Ͱ���.." ,ButtonType::NONE ,ButtonType::FINAL_END }));

	
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ Ÿ�� �Ǵ°ǰ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"����!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"������ ����: ���� �ѽ��� �Դ°�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: ��..�� �ȳ��ϼ��� �����!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"ģ��1: (���� ����)\n�ѽž�!! �����!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: �� ���� Ÿ�� �־���!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: (���ڸ��� ������)\n�� �ٵ� ���� �ֱ׷�..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"ģ��1: (��ǰ�� �ϸ�)\n���� ��Ƽ�� ���� ���� ���ƾ�..��" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"��: (�����ϸ�)\n���� ���� �� ����� �ʹ� �����ؼ�..����" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"ģ��1: (������ ��Ҹ���)\n���� ����Ե鵵 ��� ���̳�..?" ,ButtonType::NONE,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"(�����ϴ� ��Ҹ���) �׷���.. ù ��Ƽ�� ����鵵��ôϱ� ������.. " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 2,L"(��� ��Ҹ���) �׷���! ���� ����Ե��̶��� ��������� ���ڴ�!!" ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"			(�̾߱⸦ �ְ� �޴��� ģ�� �Ѹ��� �ڸ��� ���ɰ� �ִ�..)\n��: �����?.." ,ButtonType::YES ,ButtonType::FINAL_END}));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"			(�̾߱⸦ �ְ� �޴��� ģ�� �Ѹ��� �ڸ��� ���ɰ� �ִ�..)\n��: �����?.." ,ButtonType::NO  ,ButtonType::FINAL_END}));



		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�ƽ�: ��.. �� ������ �ɾ�..�̤�" ,ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"(���� ���̸� ���ϵ���) ���� �׳� ���ڸ� �ƹ����̳� ��.." ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,2.f}, 2,L"(�ݷ��ϵ� ģ���� ì���) �츮 ���ڸ��� �÷�??!" ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�ƽ�: ��.. �� �� ȥ�ھ�..." ,ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�ƽ�: ����!! ������ ����!" ,ButtonType::NO  ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�л�ȸ��: (����ũ�� Ų��)\n�� �� ����ũ �׽�Ʈ!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"�л�ȸ��: (�췷�� ��Ҹ���)\n�ȳ��Ͻʴϱ�!!\n���� ��Ƽ ������ �ðԵ� �л�ȸ�� ���߼�!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"���л�ȸ��: (�췷�� ��Ҹ���)\n���л�ȸ�� ��̳��Դϴ�!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"�л�ȸ��: (�췷�� ��Ҹ���)\n���� ��Ƽ�� �������ֽ�\n�����е� �������� ����帳�ϴ�~!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"�л���: (�ڼ��� ġ�� ȯȣ�Ѵ�)\n���ƾƾƾ�~~ ���~~" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"�л�ȸ��: (�췷�� ��Ҹ���)\n����ϱ� ���� �ο�üũ ���� �ѹ� �ҰԿ�~!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"���л�ȸ��: (ȥ�㸻�� �����ϰ�)\n1��,2��,3��,4,5..." ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"���л�ȸ��: (���� �����̸�)\n�ٿԱ���!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"�л�ȸ��: (����� �ǳ� ������)\n���� �ο��� ���� �� ������ �ٷ� ����ҰԿ�!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"�л�ȸ��: (ū ��Ҹ��� �ȳ��ϸ�)\n������Ʈ�� �����ϱ� ������ ���� Ǯ��������~!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"�л���: (��� ��Ҹ���)\n ��~~~~!!" ,ButtonType::NONE,ButtonType::FINAL_END  }));

		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�л�ȸ��: ���� ���� �����ϰ� �����帳�ϴ�~!\n���ҿ� �����ϸ� ���� ���� Ǯ�� ���� �غ��� ��ũ���̼��� ����,\n�������� ��⸦ ���� �����Դϴ�!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"�л���: (�� �� ����ϸ�)\n��~~!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�л�ȸ��: (�Ǹ��� ��Ҹ���)\n��Ҹ��� �۳׿�..??\n�ٽ�.. ��.��.��.�� �Ǽ̳���!?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"�л���: (����� ������ ȯȣ�Ѵ�)\n�� !!!!!!!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"�л�ȸ��: �����մϴ�~! �����ô�!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"							(���ڱ� �ų��� ������ �귯���´�)..." ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�л�ȸ��: (���� ���ٵ�����)\nũ����!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"���л�ȸ��: (�� ������ �ٵ�����)\n����..!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�л�ȸ��: ������!!!! ���ʽÿ�!!\n �� ȭ.��.��.����!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"�л���: �;ƾƾ�~!~!�;ƿ;ƾƾ�~!~!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"�л�ȸ��: (�귯������ �뷡�� ������ Ÿ��)\n�ε�ĩ �ε�ĩ �εε�ĩ!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"�л���: (ȯȣ�ϸ�)\n�;ƾƾƾƾ�!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"���л�ȸ��: (���� ���ٴµ���)\n������!!! �� ��Ÿ�Ƿ��� ����������!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"�л���: �;ƾ�!!!!\n���ƾӾƾƾ�" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"�л�ȸ��,���л�ȸ��: (�����⸦ ����)\n �ɾ� �ִ� ģ���� ���!!!!\n�����ϰ� STAND UP !!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"					(�л�ȸ�� �ų��� ���븦 ���� ���ҷ� ���Ѵ�...)" ,ButtonType::NONE ,ButtonType::FINAL_END}));
	
	}

	else if (L"MainMT Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::MAINMT;

		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"���� �� ���: (����..)\n������~ ���� MT ���� ���������ּ���!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"���� �� ���: (����..)\n���� �Ϸ� ��մ� �ð� �������� �ٵ�~?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"���� �� ���: (����..)\n������ ����� ���� ���������� �Ͱ��Ͻø�˴ϴ�!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"���� �� ���:						��� �����ϼ̽��ϴ�!" ,ButtonType::NONE ,ButtonType::FINAL_END}));



		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"!!!!!!!!!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"���Ⱑ MT ���ұ���!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��û ������..������ �غ���?" ,ButtonType::NONE ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"���� �� ���: (������..)\n�ƾ�~! ����ũ �׽�Ʈ" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"���� �� ���: (������..)\n���� �غ��� ��ũ���̼��� �������̳���~??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�л���: (��������)\n��~!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"���� �� ���: (������..)\n����ְ� ����� ���������?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"���� �� ���: (������..)\n���� ���� �غ�� ���� ��ð�,\n8�ñ��� �ٽ� ���� ���ֽø� �����ϰڽ��ϴ� !!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"								(����� ������ ������� �����ϰ� �����δ�)" ,ButtonType::NONE ,ButtonType::FINAL_END}));





		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"(������ �������ִ� ���̺��� �Ĵٺ���)\n..." ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"(������..)\n������� �� �Ծ��!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�ȳ�.. ���ֵ� ���־�!!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"���� �ѿ��ΰ�?" ,ButtonType::NONE ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"���־�.. ���� ��θ���.." ,ButtonType::NONE ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�ڸ��� ���� ������ �Ǵ°Ͱ�����?" ,ButtonType::NONE ,ButtonType::FINAL_END }));
		



		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"					(������ �԰��ִ��� �𸣴� ģ���� �ڸ��� ã�´�..)" ,ButtonType::NONE ,ButtonType::Choice,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��:" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"(ģ���� �Ĵٺ���) �� �� �Ծ���!! ���� �ɾƼ� �Ծ� ����" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::AMOUNT_SOCIABILIT,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 2,L"(�����ϱ� �� �����.. �ٸ� ������� �����ְ��� ��!..)" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::FRIEND_SETING,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 3,L"(���� �Ű澲���ʴ´�...)" ,ButtonType::Choice }));




		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"ģ��2: (�ջ緡 ġ��) �ƴϾ� !! ��ٷȴٰ� �̵��� ������!\n�������༭ ���� ����" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: �ƴϾ�!! �� �Ʊ���� �԰� �־���! �󸥸Ծ�!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"ģ��2: (Ȱ¦������)\n �� �׷�!? ���� ����" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"ģ��2: �� �̸��� ����?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"��: (�̼Ҹ� ������) �ѽ��̾�..����" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"��: (�ñ����ϴ� ǥ������) �� �̸��� ����?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"ģ��2: �� ������!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"��: (ȭ��¦ ����)\n��! ���� ī�� ��ܿ��� �ô�!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"ģ��2: (���� ����)\n����?? ���� �Ʊ���� ģ������;��µ�!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"��: �����̴�! ������ ������ �λ��Ұ�!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"ģ��2: (������) ��! ���� �λ��Ұ�!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"��: ���ְ� �Ծ�~!" ,ButtonType::YES ,ButtonType::FINAL_END}));




		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"							(ģ��2�� ���ư��� �ʰ� ���� �ڸ��� ��ٸ���)" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: (�Ű�������..) �ڸ� ������ ���⼭ ������?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"ģ��2: (��ٷȴٴ� ǥ������) �� �׷�? ���� !!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: (���� �̼Ҹ� ����) ���� ���ְԸԾ�~" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"ģ��2: ��!! �ٵ� �� �̸��� ����?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"��: �� �ѽ��̾�! ��?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"ģ��2: (���ɽ����� ������..) ����.. �� ������!!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"��: ����ͱ���..?\n��..�׷�! ���ְԸԾ�!" ,ButtonType::NO ,ButtonType::FINAL_END}));



		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"							(ģ��2�� ������ ���Ǵ� ���ư���...)" ,ButtonType::FIGHT ,ButtonType::FINAL_END }));
		






		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�Ϳ���¦ģ��: �ѽž�!!! �����°ž�!!!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: ��..��� �ͼ� ���԰��־���!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�Ϳ���¦ģ��: �ֵ��̶� �ٴٺ�������!!\n�ٵ� �ۿ��־�!!" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: ����!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"�Ϳ���¦ģ��: �� �����~~!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�Ϳ���¦ģ��: ���ݳ����ھ�?" ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::MAINMT].insert(make_pair(8, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"�Ϳ���¦ģ��: �ٴٺ��� ����!!" ,ButtonType::NONE ,ButtonType::FINAL_END  ,ButtonType::NONE,false,true }));
	}


	else if (L"SeaMT Scene" == m_pCurScene->GetName())
	{

		m_CurSceneCount = SCENE_TYPE::SEA_MT;



		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: (������ �ѷ�����)\n���� �����ϰ� ó������ ���� ���������� �����ϴ°Ͱ���" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"ģ��1: (�����ϸ�)\n�¾Ƹ¾�! ���� �̹� ��Ƽ ���� �� �°Ͱ���!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"ģ��1: (���� ����ġ��)\n�ѽ��� �ʴ� ���⼭ ģ���� ģ���־�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: (������ �����غ���..)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"��: (�峭���� ����) ��.. ��..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"ģ��1: ��������������" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"��: ��������������" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"ģ��1: (����) ���� ���������� ����.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"								(�Ʊ� ������� �� ģ���� ���� ���δ�)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"��: ���� ��ģ�� ������ �˾�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"ģ��1: ������� ģ����!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"��: ����.. �Ʊ� ������� ���� �ڸ��� �纸 �����༭.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"ģ��1: �׷�?? �ҷ�����??" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ��..�� �ȳ�..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"����: �ȳ�?? �Ʊ� �ڸ��� ��� ���� ���Ծ���..��" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: ��..�̾�,, ���� �ڸ��� �纸���ٰ� �׷�����" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"����: �ƳľƳ� ���� �ʹ� �ʰ� �����߾�" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"ģ��1: �׷�~~ �̵� ���� �ٸ��� ���������!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"							(ģ��1�� �������� �����Ⱑ ��ȸ�ȴ�..)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"��: ��� �ٵ� ������ �� �߿�Ͱ���" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"ģ��1: �׷��� �ٴ尡 ���̶� �ҽ��ϴ�.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"����: �¾�,, �װ� �׷��� ���� �ʳ� ���δ� �� �Ű�?" ,ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"(����ϸ�) �̷��� ��������, �ǽ����ϸ� ������ �κ��� ������.." ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(����������) ���� �� �𸣰ھ�.." ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"����: ���� ���е����� ������ �ʹ� �� �´� �����̾�.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"ģ��1: (�����ϸ�)\n���� �׷��� ������.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: �츮��� ���Ի��̴ϱ� �׷��� �ִٰ� ������!!!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: (ģ������ �Ĵٺ���)\n���� �����ִ� ���� �����ϰ�\n������ �����ϸ� ������ �� ��̸� ������ ���� ������?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"ģ��1,����: (���� �����̸�)\n�׷�����..?" ,ButtonType::YES ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"����: ���� ���е����� ������ �ʹ� �� �´� �����̾�.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"ģ��1: (�����ϸ�)\n���� �׷��� ������.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: (������ ������)\n�������� �Ҷ� ������ �����ؾ����� �𸣰ھ�.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"ģ��1,����: (���� �����̸�)\n�¾�.." ,ButtonType::NO ,ButtonType::FINAL_END }));




		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"����: (�Ʊ�.. �Ÿ����� �ڸ��纸�� ������ �༮�̱�..)" ,ButtonType::NONE,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"����: �ȳ� �ѽž�!!\n�Ʊ� ������� �����༭ ������" ,ButtonType::NONE,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"���..�ٴٴ�" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"������ �ʹ� �̻ڴ�" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(-1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"���� ģ�����ΰ�?" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: (������ �ѷ�����)\n���� �����ϰ� ó������ ���� ���������� �����ϴ°Ͱ���" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"ģ��1: (�����ϸ�)\n�¾Ƹ¾�! ���� �̹� ��Ƽ ���� �� �°Ͱ���!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"ģ��1: (���� ����ġ��)\n�ѽ��� �ʴ� ���⼭ ģ���� ģ���־�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: (�����̸�)\n�Ʊ� ��Դٰ� ����..." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"��: (������ ������ ����Ų��)\n���� ���� �簥��ģ��!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"��: (ū ��Ҹ���)\n������!" ,ButtonType::NONE ,ButtonType::FINAL_END})); // ������ ���ĸ����� �������� ���ھ���

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ��ħ �� ��� ���ͼ� �ϰ� �־��µ� �� �ƴ�~!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"����: (��� ǥ������)\n�� ����??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"����: (���� ������ ǥ������)\n��! �׸��� �Ʊ� ���п� �� ������ �־���..!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"����: �� �ƴϿ����� �� �ð��� �� �Ա� ��������ž�.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"��: (�̼Ҹ� ������)\n�׷��ٸ� �����̴٤���" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"��: (ģ������ ������ �Ĵٺ���)\n���� �ѵ� �λ��ѹ���!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"ģ��1,����: (���ÿ� ������)\n�ȳ� ������" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"��: �츮 ���� ���� ��å�ұ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"ģ��1,����: �׷� ����!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"								(�� ������ �ڱ� �Ұ��� �̾�����..)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"����: �ʳ� ���� ���δ� �� �Ű�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"ģ��1: (���� ������)\n�� ���� �𸣰ھ�.." ,ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"(����ϸ�) �̷��� ��������, �ǽ����ϸ� ������ �κ��� ������.." ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(����������) ���� �� �𸣰ھ�.." ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"����: ���� ���е����� ������ �ʹ� �� �´� �����̾�.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"ģ��1: (�����ϸ�)\n���� �׷��� ������.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: �츮��� ���Ի��̴ϱ� �׷��� �ִٰ� ������!!!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: (ģ������ �Ĵٺ���)\n���� �����ִ� ���� �����ϰ�\n������ �����ϸ� ������ �� ��̸� ������ ���� ������?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"ģ��1,����: (���� �����̸�)\n�׷�����..?" ,ButtonType::YES ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"����: ���� ���е����� ������ �ʹ� �� �´� �����̾�.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"ģ��1: (�����ϸ�)\n���� �׷��� ������.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: (������ ������)\n�������� �Ҷ� ������ �����ؾ����� �𸣰ھ�.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"ģ��1,����: (���� �����̸�)\n�¾�.." ,ButtonType::NO ,ButtonType::FINAL_END}));
		



		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"����: �׷� ���θ���, �ʳ� �˹ٴ� ��� �ϰ��־�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"ģ��1: �� ���� ���غþ�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: ���� ������!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"����: (����)\n�� ����? ���� ������ �˹� �����ߴµ�\n������ �˹ٴ� �?" ,ButtonType::NONE,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"�׳� ���� �մ� �����ϴ� �� ����� ���� ������~" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�ٸ� �˹� ã�ƺ�..�� �����.." ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"����: ��¥?? ���� �ѽ��� �ڽŰ� ����!!" ,ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"����: �׷�?? �������˹ٳ� �˾ƺ���.." ,ButtonType::NO ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"							  (���� ��⸦ �ϸ� �ð��� �帥��)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"ģ��1: (�ñ����ϴ� ǥ������)\n�ʳ� ���� ����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"����: ���� ���� �� �����ž� ����" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"ģ��1: (����)\n���߿� �����!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"ģ��1: �ѽ��̴�?" ,ButtonType::NONE ,ButtonType::Choice}));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"���� �ڵ��ϴ� �� ����־ IT ������ �����ϰ� �־�!" ,ButtonType::Choice  }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"���� ���� �� �����ϸ鼭 �����غ��� �� �� ����!" ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"ģ��1: �׷�����.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: �ʴ�!?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"ģ��1: ..." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"ģ��1: �۽�..." ,ButtonType::YES ,ButtonType::FINAL_END}));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"ģ��1: �׷�����.." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: �ʴ�!?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"ģ��1: ..." ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"ģ��1: �۽�..." ,ButtonType::NO ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ����..." ,ButtonType::NONE,ButtonType::Choice ,ButtonType::YES}));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ����..." ,ButtonType::NONE ,ButtonType::Choice ,ButtonType::NO}));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice ,ButtonType::NONE,ButtonType::YES}));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"�����ϴ� ������ �־�?" ,ButtonType::Choice,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(����� ����̸�) �ð��� �ذ����ٰž�..!" ,ButtonType::Choice,ButtonType::NONE,ButtonType::YES }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice ,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"�����ϴ� ������ �־�?" ,ButtonType::Choice,ButtonType::NONE,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(����� ����̸�) �ð��� �ذ����ٰž�..!" ,ButtonType::Choice,ButtonType::NONE,ButtonType::NO }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"ģ��1: ��..������������ ...��..��.." ,ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"ģ��1: (�Ǹ��� ǥ������) �׷�����..��½.." ,ButtonType::NO ,ButtonType::FINAL_END ,ButtonType::NO }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"ģ��1: �� ���α׷��� ����??" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: �׷� �� ������ ���� �� ������ �ְڳ�!" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"ģ��1: ������ �Ҹ��ѵ� ���߿� �װ͵� ����� ������..?" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: ���� ���� �ſ� ��̸� ������ ���� ������?" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"ģ��1: (�ǹ̽����� ǥ������)\n���� ���̾�..?" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"��: ���� �����ִ� �о� ������ �����ϴٺ���\n���߿� ����� ���Ͽ찡 �׿��� ��ư� �������������ž�!" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"ģ��1: (������ǥ������)\n�׷� ó������ ����� ����ְھ�!!\n���� �ѽž�!!����" ,ButtonType::YES ,ButtonType::NONE,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"��: (�峭������ ǥ������)\n�׷� ���߿� �� �� ����" ,ButtonType::YES ,ButtonType::FINAL_END ,ButtonType::YES}));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"ģ��1: (�Ǹ��� ǥ������)\n�׷�����..��½.." ,ButtonType::NO ,ButtonType::FINAL_END ,ButtonType::YES }));



		


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: �׷����� ���� ����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"ģ��1,����: �׷�!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"ģ��1: (�Ѿ�����) �ƾ�!!! �Ф�" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: (�����ϴ� ǥ������) ������???" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"ģ��1: ..��.." ,ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 1,L"(����) ��ٷ��� �� ���Ϻ��� �־�" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"(�����ϴ� ǥ������) ���� ���� �ɾ����..����� ������!" ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"							(ģ�� ������ ���带 �ٿ��ش�)" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"ģ��1: �ٵ� ���带 ���ٳ�!?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: �� ���� �ֵ� ������� ì�ܴٴ��� ��������" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"ģ��1: (�������ϸ�) ���� ������ �󸥰���!" ,ButtonType::YES ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::SEA_MT].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"ģ��1: ��½.. �˰ڿ�! �󸥰���!" ,ButtonType::NO ,ButtonType::FINAL_END}));

	}

	else if (L"EndMT Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::END_MT;

		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ���� ���� ����� �ð��̳�.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"								( �ѽ��̰� �޹������� ������ ��Ʈ��������.. )" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: (��½��½)\n�ФФФоֵ�� ���п��ͼ� ����� ģ������ �Ǽ� �ʹ��⻵!!.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: (������ �ٱ�ġ��)\n���� �ް��� MT���� ��������!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"ģ��1: ������~~ ������ �б����� ���ֺ���!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"����: �� �׷��� ���!! ��¥�� �����ǵ�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"��: (��½..)\n�׷�..����..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::END_MT].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"ģ���� ���: �翬����!! �ѽž�!" ,ButtonType::NONE,ButtonType::FINAL_END }));
		
	}


	else if (L"Date Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::DATE;

		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ������ ����� �Ұ��ó�.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: ��ӽð��� �ָ��Ѱ�,," ,ButtonType::NONE,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"����ź��" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::AMOUNT_RESPONSIBILITY,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 2,L"�ý�ź��" ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: �׷�! �� �ʰھ�? ���� �Ƴ��� ����Ÿ����!" ,ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ù�λ��� ���� �߿��ѵ� �����ϸ� �ȵ�!! �ý�Ÿ��!" ,ButtonType::NO ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::DATE].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��Ÿ�� ����!" ,ButtonType::NONE ,ButtonType::FINAL_END }));
	}

	else if (L"MainDate Scene" == m_pCurScene->GetName())
	{
		m_CurSceneCount = SCENE_TYPE::MAINDATE;

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"���Ѻ��� �����: Ŀ��!!" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"���Ѻ��� �����: �ڳ� �׷��� ��ȭ�ϸ� �ȵ� ~~" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: ����..?����?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"���Ѻ��� �����: ����!! ���� ���Ұ� ������ ��굿 ī���� ��� �ұ�!?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"��:	(....����)" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"��: �������� ī���� �󱸿�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"���Ѻ��� �����: �׷�!! �ڳ��� ��ȭ ����� �߸��Ǿ���!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"���Ѻ��� �����: ���ڰ� �����ҹ��� ��ȭ������ ��̸� �������!!\n���� ���׵� ���ְ�!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"��: ���� ����!!!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"���Ѻ��� �����: �� �ϴ±�.. �׷� �ѹ� �� ����� ���ڳ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"��: (�� ������)\n�׷���..��.. �ѹ� �� ���ô�" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"��굿 ī����: �켱!! ù������ �ڱ⸦ �������߸� ����� �Ǳ�������,\n�ڳ�ó�� ��Ÿ����ó�� �ൿ�ϸ� �ȵ�..\n���ڴ� ��ġ�� �������ڴ� ���������� ������ �ʰԵǾ��־�.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"��굿 ī����: �׸��� �ι�°!!\n ó���� ��뿡�� �δ㽺���� ������ ����!!!\n��ȭ�� �ڿ������� ������ �������ؾ���!! �ڳ� �ʹ�����!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 13,L"��굿 ī����: ������ ����°!!\n ���ڰ� ���ɵ������ϴ� ��̸� �ų���\nȥ�� �׷��� ���ϸ� ��ϳ�?!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 14,L"��: .....�׷�����..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 15,L"��: �׷� ����� �˷����!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 16,L"��굿 ī����: ���� ���� �˷����� �ʾ�..\n������ ���� �ѹ� ã�ƺ���!! ���� ������ ������!\n�ϳ� �������̾�!!" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 17,L"��굿 ī����: ������ �Ұ��� ù ��Ҵ� ����� �����ϳ�?" ,ButtonType::NONE ,ButtonType::Choice ,ButtonType::NONE,true,false }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"�ò������� ����" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��ȭ��" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,1.f}, 3,L"������ ���� ī��" ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��굿 ī����: �ò����� ������ ���� ��ȭ�� �ȵǴµ�..\n�ű氣�ٱ�?! ����..��..",ButtonType::YES ,ButtonType::Choice ,ButtonType::NONE, true}));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��굿 ī����: ù ������ ��ȭ�� �����̾�!!\n��ȭ�� ���� 2�ð����� �ƹ����� ���ϰ� ��ȭ�����ٱ�?!",ButtonType::NO ,ButtonType::Choice ,ButtonType::NONE, true }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(99, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��굿 ī����: �׷�!! ù������ ���������� ī��� ����!!\n������ ��븦 �˾ư��°���!\n�ϴ� ��Ҵ� �� �������",ButtonType::FIGHT ,ButtonType::FINAL_END}));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��굿 ī����: �״���! ������ �ڱ�ҷ��� �ϸ鼭 ��������!!!\n�ʹ� ���濡�� ��ġ��ġ ĳ���� ����!\n�� ���ο����� ����Ѵٴ���..�������踦 ���´ٴ���..����Ƚ���� ����ٴ���!!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: �쾾 �׷� �Ҹ��� ���ݾƿ�!!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��굿 ī����: ������ �̻����� ���� ����ٴ���..\n�ƴϸ� ������ ���� ������ ��ȭ�Ҽ��� ����!\n",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: ��.. �׷�����..!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"��굿 ī����: �� �ٸ� ������ ������ڳ�?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"��굿 ī����: ������� �ڱ� �Ұ��� ����..\n������ �ʿ��� �����̾�����..Ȯ���ϴ� ����̶���?\n�ѹ� �� �����غ���!",ButtonType::NONE,ButtonType::Choice ,ButtonType::NONE,true,false }));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"���ڰ� ��� �Ӹ��� �����°�?" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"���ڰ� �ڵ����� ���÷� Ȯ���ϴ°�?" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"���ڰ� �����ϰ� ���� ���ϴ°�?" ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��굿 ī����: �׷�!! �Ӹ��� ��� �����ٴ°���..\n�ڽ��� �ܸ� ��� �����Ѵٴ� ���ǽ��� �ൿ����!!\n�׸�ŭ �ڳ׿��� �� ���̰� ������!!!",ButtonType::YES ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��굿 ī����: �ڳ� �ٺ��ΰ�?! �ڵ����� ���÷κ��°�\n�� ���ڸ��� ��������ʹٴ¶�!!",ButtonType::NO ,ButtonType::Choice ,ButtonType::NONE, true }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(100, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��굿 ī����: ���� ����.. �ڳ׿��� �̼����� �ŷ��� ���������ϴ�..\n�����ϰ� �����°�����! �ŷ��� �ִٸ� �Ƿ� �β������Ұž�!!",ButtonType::FIGHT ,ButtonType::Choice ,ButtonType::NONE, true }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��굿 ī����: �����̸� ������ �� �Ǿ���?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: ���� �ȰͰ��⵵..�ƴѰͰ��⵵..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: ������� �׷� ��� ���ڿ� ���� �� �ƽô°ſ���?",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"���� ������̿��� ����: ����.. �Ѷ� ����ϴ� �׳ฦ ���� �������..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"���� ������̿��� ����: �׳࿡�� ù���� ���� �׳��� ������ �������\n���ֿ� ���� ���θ� �� ��������..����",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"���� ������̿��� ����: ������ �ο��� �����ٴ°���..\n�����..��..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"���� ������̿��� ����: �ƹ�ư!! �ο��� ������������!!\n�׻� �غ� �ؾ��Ѵ� �̸��̾�!!",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"���� ������̿��� ����: �� �ο��� ���� �ٽ� ������..~",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"���� ������̿��� ����: ������ ��ȸ�� ���� �ڳװ� �η�����..",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"��: ����,,,",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"��: �ٵ� �̺� ���̷��� �ȿ�����?\n�������Ű� �ƴϿ���??",ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(101, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"���� ������̿��� ����: ���� ����!! ���غ��� ģ��!!",ButtonType::NONE ,ButtonType::FINAL_END}));






		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�αٵα�.. �����ñ�.." ,ButtonType::NONE ,ButtonType::FINAL_END}));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�Ұ��ó�: ��..�ȳ��ϼ���!\n���� �ɾƵ��ǳ���?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(1, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: (��.. �̻ڴ�)\n�翬����!!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));


		PLAYER_MBTI eMbti = pG_Player->GetMbti();
		wstring strMbti = EnumToString(eMbti);
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ...." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"�Ұ��ó�: ...." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�Ұ��ó�: ����.. ����ϳ׿�,," ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: (�β������ϸ�)\nȤ�� ���̰� ��� �ǽó���!!?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"�Ұ��ó�: �� 20���̿���!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"��: ��!! ���� 20���ε�! ģ���׿�!?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"��: �������ø� �� �������..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"�Ұ��ó�: �׷� ���� �̸��� ����??" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"��: �� ���ѽ�!! �ʴ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"�Ұ��ó�: ��.. �谡��!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"��: ��!! �̸� �̻ڴ�!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"��: MBTI ������� �ɱ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"�Ұ��ó�: �� ISTP��! �ʴ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 13,L"��: �� " + strMbti + L"��!" ,ButtonType::NONE}));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 14,L"�Ұ��ó�: ��~~ ���� �����ϴ� MBTI��!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 15,L"��: ��¥??! �츮 �����ϴ°� ����!!\n�ٵ� �� �α� �ǰ� �����Ͱ�����.. �� ������?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 16,L"�Ұ��ó�: (��Ȳ���ϸ�)\n��..��? ���ڱ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 17,L"��: (�̻����� �����ϸ�)\n��..�̾�! �� �δ㽺����������..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 18,L"��: (��ġ ����)\n�׷� ���� ����غþ�?!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 19,L"�Ұ��ó�: (���׸���)\n��..�װ� ���߿� ���ϰ������ �Ф�" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 20,L"�Ұ��ó�: (�ֽ� ������ô�ϸ�)\n�Ƴ� ����..�� ��̰� ��� ��?" ,ButtonType::NONE ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"�ִ� ����!" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"������� Ÿ��!" ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�Ұ��ó�: (ǥ���� ������)\n��..? �ִ�..? ���..?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: (�ڽŰ� ��������)\n��!! �ʹ� ������!!!\n�����ߵ� ��հ� !! ��Į!! �� ��Į�� �̹� ��������?!\n�п������ְ�..��..�ڵ��!! �̰� ��û ����.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�Ұ��ó�: (������ ������)\n���!! ��..�����? �ϳ��� ���˾� ��ھ�.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: (������ ��ä)\n��..? �� �̾� �ʹ� ���̳��� �ִϸ� ��õ�߳�?\n �ٸ��� ��õ���ٱ�?!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"�Ұ��ó�: �ƴ�.. �� ȭ����� ����.." ,ButtonType::YES ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�Ұ��ó�: (���� ���µ���)\n��~~ ������� ��վ�?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: (�ڽŰ��� ������)\n��������!! ����� ������� Ÿ�°͵���ձ�!!\n�� ���� �̴Ϲ���ũ�� �����̾�!!\n�¾�!! �����͵� ��վ�!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�Ұ��ó�: (�Ѽ� ����)\n...��.. �� �𸣰ھ� ���ɾ��� �о߾� !!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: (������ ��ä)\n�׷�??! �׷� ���� �� �ڼ��� ������ �ٱ�??" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(2, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"�Ұ��ó�: �ƴ�.. �� ȭ����� ����.." ,ButtonType::NO ,ButtonType::FINAL_END }));




		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(3, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n\n						Hidden Quest!\n\n						���콺�� �ֺ��� Ž���غ���..\n						���ѽð� 10��!!!!!!" ,ButtonType::NONE ,ButtonType::FINAL_END }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(4, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n\n						�̼ǿ� �����ϼ̽��ϴ�...\n						���� �Ұ������� �غ� �ȵȰͰ��ƿ�..\n						�ð��� �����帱�Կ�!" ,ButtonType::NONE ,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�Ұ��ó�: ȭ��� ���� �ʹ� �� �ʾ��׿�.. �˼��մϴ�!!.." ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: �� �������ϴ�!! ����\n�Ʊ� ���� �ʹ� �ڱ� �Ҹ��� �ѰͰ��Ƽ� �˼��ؿ�,," ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�Ұ��ó�: ����..�ƴϿ��� ���� ���� ��̸� �� ����,,\n���� ������ ��̰ŵ��!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: �� ������!? �ǰ� �̻����� ��̸� �������ó׿�!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(5, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"�Ұ��ó�: ����..." ,ButtonType::NONE ,ButtonType::FINAL_END}));



		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: (��.. �� ����� ħ��.. �� �����������..?)" ,ButtonType::NONE,ButtonType::Choice ,ButtonType::NONE,true}));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"Ȥ�� ����ģ�̶��� �� ������� �Ȱž�?" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME},true,1.f}, 2,L"�ʴ� �̻����� ��� ��?" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"�θ���� ������ �ϼ�?!" ,ButtonType::Choice }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�Ұ��ó�: (�Ѽ�����)\n�ƴ�..�װ� �� �ñ��ѵ� ����ü?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: �̾�..��..�Ұ��þ�ƴ�.." ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: (��¿�� �����ϸ�)\n��..ȭ�����!!.." ,ButtonType::YES,ButtonType::FINAL_END }));



		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�Ұ��ó�: ����..�����ΰ� �����̻ڰ� Űũ��..\n�����ΰ� ���ϰ� ���� �ٶ���ִ»��?!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: (�峭������)\n���� �̻۰� ���� ���� �� ��û �����ѵ�!!..����" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�Ұ��ó�: Ǳ.. �� �̻����̹���?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: ��..�Ϳ��� �ڰ� �̻ۻ��! ������ ���� ƼŰŸī�� �� �Ǵ»��!!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"�Ұ��ó�: �ڰ� �̻ۻ���̶��.. Ư���ϳ� ����" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"�Ұ��ó�: �Ƕ�!! ��մ� �̾߱�����!!" ,ButtonType::NO,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�Ұ��ó�: (�������ϸ�)\n���ư��̾�..2������.." ,ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: ��?..��..�̾�" ,ButtonType::FIGHT }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(6, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: (��¿�� �����ϸ�)\n��..ȭ�����!!.." ,ButtonType::FIGHT,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(102, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"\n\n										�׳డ ��������\n\n�ȳ��ϼ���, �ѽž�.. ª�Գ��� ��ȭ������ �־ �����߾��\n\n���� ���ʰ� �� �����ʴ� ����ΰͰ��ƿ� �˼��ؿ�..\n\n������ ��������..  -�谡�� �ø�- ",ButtonType::NONE ,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ����..." ,ButtonType::NONE,ButtonType::Choice}));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: " , ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::AMOUNT_PROBLEM_SOLVING,PLAYER_INFO::AMOUNT_ATTRACTIVENESS,PLAYER_INFO::NAME},true,1.f}, 1,L"Ȥ�� �����ϴ� �����־�?" ,ButtonType::Choice }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"Ȥ�� ������??" ,ButtonType::Choice }));

		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�Ұ��ó�: (ǥ���� �������)\n��!! �� �������ФФ� �� ���޶�Ͼ� Ű��ŵ�!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"�Ұ��ó�: (ǥ���� �������)\n�� ���� �ʹ��ʹ� �����!! �ʴ�?? ������??" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: (������ ������)\n��!! ���� ������ ������!! Ű����������\n�������� �ʹ� �Ϳ��ݾ�~~~!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"�Ұ��ó�: �� ����?! ���� �������Ұ� ���Ҵµ� �ǿܳ�!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"��: ��!! �츮 ���߿� ���� ������ī�� ����!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"�Ұ��ó�: �� �ʹ� ����!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"									(�׳డ �Ӹ�ī���� ������ �Ÿ���)" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"��: (�� ��ȣ��..! ������� ����!..)" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"�Ұ��ó�: �츮 ���� �̵��� ���� ������ ����?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"��: ����!! ������ ������?" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"�Ұ��ó�: (���� ȣ���� ���̸�)\n�翬����! �� ������ ���� ������ �ФФ�" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"��: �츮 �� �߸´°� ����!! ���� �����ϰŵ�!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"�Ұ��ó�: �׷���!! ������" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 13,L"��: �׷� ���� �� ������ ������!!!" ,ButtonType::YES }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 14,L"								(�����Ⱑ �����Ͱ� �Ұ����� ���������� �������ȴ�...)" ,ButtonType::YES,ButtonType::FINAL_END }));


		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"�Ұ��ó�: �ƴ� ����.. ��?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: (��ġ�� ä��)\n���� ���Ӿ���!! �츮 �߸´°�..?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"�Ұ��ó�: ��..�׷���..? ����" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: �̵� ���� ���� ������?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"�Ұ��ó�: �׷���? ��������?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 5,L"��: �� ��â ������!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 6,L"�Ұ��ó�: ��! ��¥? ���� �����ϴµ�!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 7,L"��: �׷�?!! �׷� ��â������ ����!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 8,L"�Ұ��ó�: (����ϸ�)\n��..���� �ٷ�??" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 9,L"��: (��Ȳ�ϸ�)\n��..������ �� �δ㽺���?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 10,L"�Ұ��ó�: Ȥ�� �������� �� �����³��� �Դ°� �?" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 11,L"��: �׷� ����!!" ,ButtonType::NO }));
		m_strTex[(UINT)SCENE_TYPE::MAINDATE].insert(make_pair(7, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 12,L"								(������ ����ϸ� �Ұ����� �������ȴ�...)" ,ButtonType::NO ,ButtonType::FINAL_END }));

	}

	else if (L"Endding Scene" == m_pCurScene->GetName())
	{
		// 6���� �ɷ�ġ�� ���� ���� ���� �ɷ�ġ 1���� �̴´�.
		// ���� �ɷ�ġ�� ��δ� ������    Max �ɷ�ġ�� ���� ���, Min �ɷ�ġ�� ��������� ����
		// �� �ΰ��� ����� �������� ���ϸ� ������ (�̻�,����)
		// ���� �ɷ�ġ�� �⺻���� ������ ������ �˸���  3��° ������ ����� �̻����� �������� ������ �� 12���� ������ ����.

		// �뷱������ �ϴ�����

		// TEST ���� 1��°�� ���δɷ�ġ: ��ȸ��, ��մɷ�ġ�� �̻��϶� 
		m_CurSceneCount = SCENE_TYPE::ENDDING;

		m_strTex[(UINT)SCENE_TYPE::ENDDING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 0,L"��: ��..? ���� �� �����ݾ�!!" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::ENDDING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 1,L"��: �� ���� �� ���̿���..?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::ENDDING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 2,L"��: �޼ӿ��� ���л�Ȱ�� �ߴ��Ű���..\n�̷��� ���� ������ ����� �ϰ�;����ɱ�?" ,ButtonType::NONE }));
		m_strTex[(UINT)SCENE_TYPE::ENDDING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 3,L"��: ...." ,ButtonType::NONE  }));
		m_strTex[(UINT)SCENE_TYPE::ENDDING].insert(make_pair(0, Conversation{ {{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f}, 4,L"��:							( �װ� �ߴ����� ����ϸ�.. �ٽ��ѹ� ��������� ������)" ,ButtonType::NONE,ButtonType::FINAL_END }));
		
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
	// �۷ι� Player ó�� ����¼���
	if (nullptr == pG_Player)
	{
		
		pG_Player = new CPlayer(eMbti);		// Player�� �������� ������ ����غ�����
		pG_Player->SetName(L"Player");
		pG_Player->SetScale(Vec2(50.f, 70.f));
		pG_Player->SetMbti(eMbti);
	}

	wstring strMbti = EnumToString(eMbti);

	pG_Player->SetPlayerInfo(PLAYER_INFO::MBTI, L"MBTI: " + strMbti);


	m_strTex[(UINT)m_CurSceneCount].insert(make_pair(0, Conversation{{{PLAYER_INFO::NAME,PLAYER_INFO::NAME,PLAYER_INFO::NAME},true,0.f} ,6,L"\n\n\n\n				���ѽž� ����� MBTI��\n\n				" + strMbti + L"�Դϴ�.",ButtonType::NONE ,ButtonType::FINAL_END }));
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


	if (nullptr != m_TalkingOwner) // ��ȭ UI�� npc�� �������̸� ���� Owner�� ��������
	{
		if (!m_TalkingOwner->IsDead())
		{
			CurConversation_Npc_Count = m_TalkingOwner->GetMyConversationID();
		}
		
	}
	else
		CurConversation_Npc_Count = 0; // npc�� ������ ������ 0



	
	
	
	
	
	m_CurConversation.PreButtonType = g_LastConversationInfo.CurButtonType;

	auto range = m_strTex[(UINT)m_CurSceneCount].equal_range(CurConversation_Npc_Count);
	auto it = range.first;
	for (; it != range.second; ++it)   // ���� �������� ���� ��ȭ�� ���� ( ��ư ���°������� )
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
		// ��ȭ�� ��ã������ ������ �� ������ ù��° ��ȭ�� ����
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
		RenderText(_dc, m_CurConversation); // �ش� ���� ��ȭ �ؽ�Ʈ�� ��� ���͸� ����

	
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
		RenderText(_dc, m_CurConversation); // �ش� ���� ��ȭ �ؽ�Ʈ�� ��� ���͸� ����

	if (ButtonType::Choice == m_CurConversation.CurButtonType)
	{
		
		RenderChoiceText(_dc);
	}
}



void CTalkUI::RenderText(HDC _dc, Conversation _str)
{
	// ���� ��ȭ�� �̸� �� ���ؼ� ��������� ������Ʈ�� �̸� ���� ��ȭ�� ���� ã�Ƴ�����

	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	
	m_fTime += fDT;

	
	// 0.1�� ����
	if (0.05f < m_fTime)
	{
		// ���� ���� ��� // �ѱ��� �� �� ��¥�� ȭ�鿡 �ȶ� ��? ����ۿ� �׸��� �����ϱ�

		m_fTime = 0.f;

		if (m_itextSize != m_CurConversation.text.size())  // ���ڿ� �� ����ϸ� ����
			m_itextSize++;
		else
		{
			if(SCENE_TYPE::LODING_2 == m_CurSceneCount && nullptr != pG_Player)
				m_bStart = true;
		}
			
	}
	


	Vec2 vPosLT = vPos - vScale / 2.2f; // �»��
	Vec2 vPosRB = vPos + vScale / 2.2f; // ���ϴ�

	

	for (int i = 0; i < m_itextSize; ++i)
	{
		if (m_CurConversation.text[i] == L'\n')
		{
			// '\n' ���ڸ� ������ �� x ��ǥ�� �ٽ� �ʱ� ������ �����ϰ� y ��ǥ�� �����ϴ�.
			vPosLT.x = vPos.x - vScale.x / 2.2f;
			vPosLT.y += 30; // �������� �ϴ� y ��ǥ ������ �����մϴ�.
			vPosRB.y += 30; // �������� �ϴ� y ��ǥ ������ �����մϴ�.
			continue; // '\n' ���ڴ� �׸��� �ʰ� ���� ���ڷ� �Ѿ�ϴ�.
		}

		RECT rt = {
		  vPosLT.x
		, vPosLT.y + 40
		, vPosLT.x + 40
		, vPosRB.y + 60 };

		DrawTextW(_dc
			, &m_CurConversation.text[i] // m_Textcount�� ��ư������ �ϳ��� ���� ���� ��ȭ�� ������
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
	//	//pNewSound->SetPosition(1.f); // ��з� , �Ҹ���ġ
	//	pNewSound->SetVolume(70.f); // �Ҹ� ���� ũ��
	//
	//	soundCheck = true;
	//}
	
}



void CTalkUI::RenderChoiceText(HDC _dc)
{
	// ���� ������ ���� ��ȭ�Ҽ��ִ� npc�� �ִ� ���� �˾ƾ��� �� npc�� ���� �� multimap ��ȭ�� Ű����
	// ������ npc���� ��ȭ�� Ű���� ��� ���� �� npc�� ��ȭ�� �ϸ鼭 �ش� Ű������ � npc���� �˼��ְ�
	// �״��� �� npc���� 7777 �̶�� ���ô�ȭ�� ���ü�����

	wstring strChoice[3];
	int k = 0;

	CTalkUI* pMainTalkUI = CSceneMgr::GetInst()->GetCurScene()->GetMainTalkUI();


	UINT CurConversation_Npc_Count;
	if (nullptr != m_TalkingOwner) // ��ȭ UI�� npc�� �������̸� ���� Owner�� ��������
	{
		if (!m_TalkingOwner->IsDead())
		{
			CurConversation_Npc_Count = m_TalkingOwner->GetMyConversationID();
		}

	}
	else
		CurConversation_Npc_Count = 0; // npc�� ������ ������ 0

	auto range = m_strTex[(UINT)m_CurSceneCount].equal_range(CurConversation_Npc_Count);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (ButtonType::Choice == it->second.CurButtonType)
		{
			if (it->second.PreButtonType == m_CurConversation.PreButtonType)
			{
				if (0 != it->second.id) // id�� 0�ΰ��� ��: �̰Ŵϱ� ��� X
				{	
					// ���� ��ȭ
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
			, strChoice[i].c_str() // m_Textcount�� ��ư������ �ϳ��� ���� ���� ��ȭ�� ������
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
				, wstrMbti[i][j].c_str() // m_Textcount�� ��ư������ �ϳ��� ���� ���� ��ȭ�� ������
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


	// Job Scene���� ��ȭ�� ���� NPC (Monster)�� �����Ұ���
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();


	vector<CObject*>& vecObjMon = pCurScene->GetMonsterGroup();


	for (size_t i = 0; i < vecObjMon.size(); ++i)
	{
		CMonster* pMon = dynamic_cast<CMonster*>(vecObjMon[i]);

		if (L"Player" != GetNpcOwner()->GetName() && !pMon->IsDead())  // ���� ���� ��ü�� ó��
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


void CTalkUI::ApplyChoiceToPlayer_InFo(CUI* _pBtnUI) // � �ɷ�ġ�� ��� �ø��� �˰��� �����غ�����
{
	double* pPlayerInfo = pG_Player->GetPlayer_Info();
	

	// ��ư�� ��ȭ�� �˾ƾ���
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

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // ����  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // ����
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // �ο�
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

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // ����  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // ����
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // �ο�
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

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // ����  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // ����
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // �ο�
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

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // ����  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // ����
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // �ο�
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

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // ����  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // ����
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // �ο�
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

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // ����  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // ����
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // �ο�
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

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // ����  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // ����
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // �ο�
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

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // ����  
		{	
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // ����
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // �ο�
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

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // ����  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // ����
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // �ο�
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

		if (L"Choice_Left_Btn_UI" == _pBtnUI->GetName()) // ����  
		{
			m_CurConversation.CurButtonType = ButtonType::YES;
		}
		else if (L"Choice_Right_Btn_UI" == _pBtnUI->GetName()) // ����
		{
			m_CurConversation.CurButtonType = ButtonType::NO;
		}
		else if (L"Choice_Bottom_Btn_UI" == _pBtnUI->GetName()) // �ο�
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
	 //this�� �׻� �Ȱ��� �� �ϰŰ� ��ȭâ�� 1���ϱ� ����� �� �Ȱ�������
	// pUI�� �� �Լ��� �θ� ��ưUI�� ����
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



	m_PreSceneCount = m_CurSceneCount; // �������̶� ����� ����ȭ
	ButtonType eType = m_CurConversation.CurButtonType;

	if (ButtonType::Choice == m_CurConversation.NextButtonType) // ���� ��ȭ�� ���̽����� �̴ϱ�
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
		// ����ؾ�����: �ϴ� �����ȭ�ϴ� ���� � ������, � npc�� � ��ȭ�� �������ִ��� �׿����� Player�� � ������ �ٲܰ����� �����ؾ���
		m_itextSize = 1;
		m_CurConversation.CurButtonType = ButtonType::Choice;

		ApplyChoiceToPlayer_InFo(pUI); // Scene ������ �°� Player ���� �ٲ��ִ� ������ ��� �Լ�
			
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
		// ��ȭ�� �������� �ڿ� ���� �۾��� �Լ�ȭ ��Ű��
		noneId = 0;
		choiceId = 0;
		yesId = 0;
		noId = 0;
		fightId = 0;
		
		
		// ��ȭ�� ��������� �������� ������ � ������ �ߴ��� �˾ƾ���
		// ���� ������� ��ȭ�� ����ϰ��ִ� �װ��� �����ؾ���
		g_LastConversationInfo = m_CurConversation;
		End_of_Conversation();
	}
		

	default:
		break;
	}



	if (m_CurConversation.playerInfo_data.player_Info[0] == PLAYER_INFO::MBTI)
	{
		PLAYER_MBTI mbti = pG_Player->GetMbti();  // 0 ~7 I�λ�� 8~15 E�λ��

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

