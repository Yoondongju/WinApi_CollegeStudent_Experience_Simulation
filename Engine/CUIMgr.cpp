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
	
	// 1. FocusedUI Ȯ��
	// UI�� Ŭ���Ǳ� ������ m_pFocusedUI�� nullptr �̿��� MouseOn�Լ��� �ȵ���
	// �̶� m_pFocusedUI �� ��Ŀ���� �ֻ��� �θ� UI��
	m_pFocusedUI = GetFocusedUI();

	if (m_pFocusedUI) // ���� ���� 
	{
		if (m_pFocusedUI->IsDead())
		{
			m_pFocusedUI = nullptr;
		}
	}
	

	if (!m_pFocusedUI)
		return;

	bool bLbtnTap = KEY_CHECK(KEY::LBTN, KEY_STATE::TAP);     // �� �������� 
	bool bLbtnAway = KEY_CHECK(KEY::LBTN, KEY_STATE::AWAY);   // �� �� ����



	// 2. ���� ��Ŀ�̵� FocusedUI ������, �θ� UI����, �ڽ� UI �� �� ���� Ÿ���õ� UI�� �����´�.
	CUI* pTargetUI = GetTargetedUI(m_pFocusedUI);  // target UI ����


	// ��Ŀ���� �ȵ��ִ��� ���콺�� UI���� �÷��� ������ MouseOn ȣ���ؾ���.

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
	// �̹� ��Ŀ���ΰ��  �Ǵ� ��Ŀ�� ������û�ΰ��
	if (m_pFocusedUI == _pUI || nullptr == _pUI)
	{
		m_pFocusedUI = _pUI;
		return;
	}

	m_pFocusedUI = _pUI;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();  // ���� Scene�� �ִ� �θ� UI�� ��������


	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (m_pFocusedUI == *iter)
		{
			break;
		}

	}


	// ���� ������ �ǵڷ� ���� ��ü
	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);

}


CUI* CUIMgr::GetFocusedUI()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	vector<CObject*>& vecUI = pCurScene->GetUIGroup();  // ���� Scene�� �ִ� �θ� UI�� ��������


	bool bLbtnTap = KEY_CHECK(KEY::LBTN, KEY_STATE::TAP); // �� �������� 
		
	
	if (0 == vecUI.size()) // �ش� ���� UI�� ���ٴ��̾߱�ϱ� ��Ŀ�� �� ���������
	{
		m_pFocusedUI = nullptr;
	}

	

	vector<CObject*>::iterator targetiter = vecUI.end();
	vector<CObject*>::iterator iter = vecUI.begin();


	// ���� ��Ŀ�� UI�� �޾Ƶΰ� ����Ǿ����� Ȯ���Ѵ�.
	CUI* pFocusedUI = m_pFocusedUI;

	list<CUI*> queue;
	CUI* pUI = nullptr;
	CUI* pTargetUI = nullptr;


	if (!bLbtnTap)  // ���� �������� ���ٸ� ��Ŀ���� ���ֵ�, MouseOn�� ȣ���������..
	{
		for (; iter != vecUI.end(); ++iter)
		{
			pUI = ((CUI*)*iter);  // �ֻ��� �θ�
			queue.push_back(pUI); // �� �θ� ť�� �ְ�

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

					// �� �θ� pUI�� �ڽ�UI���͸� vecChild���� ��
					const vector<CUI*>& vecChild = pUI->GetChildUI();

					// queue�� �ڽ� UI�߰�
					for (size_t i = 0; i < vecChild.size(); ++i)
					{
						queue.push_back(vecChild[i]);
					}
				}
				
			}

		}

		return pFocusedUI;
	}


	// ���ʹ�ư ���ȴٴ� ���� �Ͽ� 
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


	// �̹��� Focus�� UI�� ����
	if(vecUI.end() == targetiter)
	{
		return nullptr;
	}
	

	pFocusedUI = (CUI*)*targetiter;

	// ���� ������ �ǵڷ� ���� ��ü
	vecUI.erase(targetiter);
	vecUI.push_back(pFocusedUI);

	return pFocusedUI;

}


CUI* CUIMgr::GetTargetedUI(CUI* _pParentUI)
{
	bool bLbtnAway = KEY_CHECK(KEY::LBTN, KEY_STATE::AWAY);   // �� �� ����



	// 1. �θ� UI ����, ��� �ڽĵ��� �˻� �Ѵ�.
	// 2. Ÿ�� UI �� ��, �� �켱������ ���� ������ �� ���� ������ �ڽ� UI�̴�.
	// 3. ���� �߿��Ѱ��� (Level ��ȸ ����� ���ؼ� �˻��ؾ���) (����Լ� ��ĸ��� queue Ȱ���Ұ���)
	 
	 
	// 4. queue���� �����͸� Ȯ���Ѵ� == ������ == ť�� ��� �ش� �ε����� UI�� �����Ѵ�
	// 4.1 queue���� �����͸� queue.front() �� Ȯ���Ѱ��� �ܼ��� ��ó�� �����͸� ��ȯ���ִ°�
	// 4.2 �����͸� Ȯ���ϰ� ��¥ �����Ϸ��� pop_front() �������
	// 5. ���� ���� UI�� �ڽ��� �ִٸ� �ڽ��� �ڵ��� �߰����ش�.
	// 6. ����Ʈ�� ť ó�� ������

	CUI* pTargetUI = nullptr;

	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;
	
	queue.clear();
	vecNoneTarget.clear();

	queue.push_back(_pParentUI);

	while(!queue.empty())
	{ 
		// queue�ȿ� �ִ°� ������
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

				// ť���� ������ UI�� TargetUI ���� Ȯ��
				pTargetUI = pUI;
			}
			else
			{
				vecNoneTarget.push_back(pUI);
			}
		}
		
		// �� �θ� pUI�� �ڽ�UI���͸� vecChild���� ��
		const vector<CUI*>& vecChild = pUI->GetChildUI();

		// queue�� �ڽ� UI�߰�
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













