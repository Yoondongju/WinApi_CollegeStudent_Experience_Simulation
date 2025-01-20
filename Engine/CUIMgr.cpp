#include "pch.h"
#include "CUIMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CUI.h"

#include "CKeyMgr.h"

CUIMgr::CUIMgr()
	: m_pFocusedUI(nullptr)
{

}

CUIMgr::~CUIMgr()
{

}


void CUIMgr::update()
{
	
	// 1. FocusedUI 확인
	// UI가 클릭되기 전까진 m_pFocusedUI가 nullptr 이여서 MouseOn함수가 안들어옴
	// 이때 m_pFocusedUI 는 포커스된 최상위 부모 UI임
	m_pFocusedUI = GetFocusedUI();

	if (m_pFocusedUI) // 좀비 상태 
	{
		if (m_pFocusedUI->IsDead())
		{
			m_pFocusedUI = nullptr;
		}
	}
	

	if (!m_pFocusedUI)
		return;

	bool bLbtnTap = KEY_CHECK(KEY::LBTN, KEY_STATE::TAP);     // 딱 누른시점 
	bool bLbtnAway = KEY_CHECK(KEY::LBTN, KEY_STATE::AWAY);   // 딱 뗀 시점



	// 2. 현재 포커싱된 FocusedUI 내에서, 부모 UI포함, 자식 UI 들 중 실제 타겟팅된 UI를 가져온다.
	CUI* pTargetUI = GetTargetedUI(m_pFocusedUI);  // target UI 지정


	// 포커싱이 안되있더라도 마우스가 UI위에 올려져 있으면 MouseOn 호출해야함.

	if (nullptr != pTargetUI)
	{
		if (!pTargetUI->IsDead())
		{
			pTargetUI->MouseOn();

			if (bLbtnTap)
			{
				pTargetUI->MouseLbtnDown();
				pTargetUI->m_bLbtnDown = true;
			}

			else if (bLbtnAway)
			{
				pTargetUI->MouseLbtnUp();

				if (pTargetUI->m_bLbtnDown)
				{
					pTargetUI->MouseLbtnClicked();
				}

				pTargetUI->m_bLbtnDown = false;
			}
		}

	}
	
	


}

void CUIMgr::SetFocusedUI(CUI* _pUI)
{
	// 이미 포커싱인경우  또는 포커싱 해제요청인경우
	if (m_pFocusedUI == _pUI || nullptr == _pUI)
	{
		m_pFocusedUI = _pUI;
		return;
	}

	m_pFocusedUI = _pUI;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();  // 현재 Scene에 있는 부모 UI들 가져오기


	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (m_pFocusedUI == *iter)
		{
			break;
		}

	}


	// 백터 내에서 맨뒤로 순번 교체
	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);

}


CUI* CUIMgr::GetFocusedUI()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	vector<CObject*>& vecUI = pCurScene->GetUIGroup();  // 현재 Scene에 있는 부모 UI들 가져오기


	bool bLbtnTap = KEY_CHECK(KEY::LBTN, KEY_STATE::TAP); // 딱 누른시점 
		
	
	if (0 == vecUI.size()) // 해당 씬의 UI가 없다는이야기니까 포커싱 다 없애줘야함
	{
		m_pFocusedUI = nullptr;
	}

	

	vector<CObject*>::iterator targetiter = vecUI.end();
	vector<CObject*>::iterator iter = vecUI.begin();


	// 기존 포커싱 UI를 받아두고 변경되었는지 확인한다.
	CUI* pFocusedUI = m_pFocusedUI;

	list<CUI*> queue;
	CUI* pUI = nullptr;
	CUI* pTargetUI = nullptr;


	if (!bLbtnTap)  // 내가 누른적이 없다면 포커싱은 안주되, MouseOn은 호출해줘야함..
	{
		for (; iter != vecUI.end(); ++iter)
		{
			pUI = ((CUI*)*iter);  // 최상위 부모
			queue.push_back(pUI); // 그 부모를 큐에 넣고

			while (!queue.empty())
			{
				pUI = queue.front();
				queue.pop_front();

				if (!pUI->IsDead())
				{
					if (pUI->IsMouseOn())
					{
						pTargetUI = pUI;

						if (nullptr != pTargetUI)
							pTargetUI->MouseOn();
					}

					else if (!pUI->IsMouseOn())
					{
						pTargetUI = pUI;

						if (nullptr != pTargetUI)
							pTargetUI->MouseOut();
					}

					// 그 부모 pUI의 자식UI벡터를 vecChild한테 줌
					const vector<CUI*>& vecChild = pUI->GetChildUI();

					// queue에 자식 UI추가
					for (size_t i = 0; i < vecChild.size(); ++i)
					{
						queue.push_back(vecChild[i]);
					}
				}
				
			}

		}

		return pFocusedUI;
	}


	// 왼쪽버튼 눌렸다는 전제 하에 
	for (; iter != vecUI.end(); ++iter)
	{	
		if (!(*iter)->IsDead())
		{
			if (((CUI*)*iter)->IsMouseOn())
			{
				targetiter = iter;
			}
		}	

	}


	// 이번에 Focus된 UI가 없다
	if(vecUI.end() == targetiter)
	{
		return nullptr;
	}
	

	pFocusedUI = (CUI*)*targetiter;

	// 백터 내에서 맨뒤로 순번 교체
	vecUI.erase(targetiter);
	vecUI.push_back(pFocusedUI);

	return pFocusedUI;

}


CUI* CUIMgr::GetTargetedUI(CUI* _pParentUI)
{
	bool bLbtnAway = KEY_CHECK(KEY::LBTN, KEY_STATE::AWAY);   // 딱 뗀 시점



	// 1. 부모 UI 포함, 모든 자식들을 검사 한다.
	// 2. 타겟 UI 들 중, 더 우선순위가 높은 기준은 더 낮은 계층의 자식 UI이다.
	// 3. 제일 중요한개념 (Level 순회 방식을 통해서 검사해야함) (재귀함수 방식말고 queue 활용할거임)
	 
	 
	// 4. queue에서 데이터를 확인한다 == 꺼낸다 == 큐에 담긴 해당 인덱스의 UI를 삭제한다
	// 4.1 queue에서 데이터를 queue.front() 로 확인한것은 단순히 맨처음 데이터를 반환해주는것
	// 4.2 데이터를 확인하고 진짜 삭제하려면 pop_front() 써줘야함
	// 5. 만약 꺼낸 UI가 자식이 있다면 자식을 뒤따라 추가해준다.
	// 6. 리스트를 큐 처럼 쓸거임

	CUI* pTargetUI = nullptr;

	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;
	
	queue.clear();
	vecNoneTarget.clear();

	queue.push_back(_pParentUI);

	while(!queue.empty())
	{ 
		// queue안에 있는거 꺼내기
		CUI* pUI = queue.front();
		queue.pop_front();

		if (!pUI->IsDead())
		{
			if (pUI->IsMouseOn())
			{

				if (nullptr != pTargetUI)
				{
					vecNoneTarget.push_back(pTargetUI);
				}

				// 큐에서 꺼내온 UI가 TargetUI 인지 확인
				pTargetUI = pUI;
			}
			else
			{
				vecNoneTarget.push_back(pUI);
			}
		}
		
		// 그 부모 pUI의 자식UI벡터를 vecChild한테 줌
		const vector<CUI*>& vecChild = pUI->GetChildUI();

		// queue에 자식 UI추가
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}


	if (bLbtnAway)
	{
		for (size_t i = 0; i < vecNoneTarget.size(); ++i)
		{
			vecNoneTarget[i]->m_bLbtnDown = false;
		}		
	}

	return pTargetUI;
}













