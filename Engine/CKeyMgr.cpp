#include "pch.h"
#include "CKeyMgr.h"

#include "CCore.h"
int g_arrKey[(int)KEY::LAST] =
{
	VK_LEFT, //LEFT,
	VK_RIGHT,//RIGHT,
	VK_UP,//UP,
	VK_DOWN,//DOWN,
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',

	'A',
	'S',
	'D',
	'F',
	'G',
	'Z',
	'X',
	'C',
	'V',
	'B',
	
	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,

	VK_LBUTTON,
	VK_RBUTTON,

	//LAST,

};








CKeyMgr::CKeyMgr()
{
	 

}

CKeyMgr::~CKeyMgr()
{


}


void CKeyMgr::init()
{
	for (int i = 0; i < (int)KEY::LAST; i++)
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE,false });
	}
	
	
}


void CKeyMgr::update()
{
	// ������ ��Ŀ�� �˾Ƴ���
	//HWND hMainWnd = CCore::GetInst()->GetMainHwnd();
	HWND hWnd = GetFocus();


	
	// ������ ��Ŀ�� ���϶�
	if (hWnd != nullptr )
	{
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			if (GetAsyncKeyState(g_arrKey[i]) & 0x8000)   // �ش� Ű�� ���ȴٸ�?
			{
				if (m_vecKey[i].bPrev) // �������� ����
				{
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else // ������ �ȴ��ȴ�
				{
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrev = true;
			}

			else // �ش�Ű�� �ȴ���
			{
				if (m_vecKey[i].bPrev) // ������ ����
				{
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else // �������� �ȴ��ȴ�
				{
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
				m_vecKey[i].bPrev = false;
			}
		}

		// Mouse ��ġ ���
		POINT ptPos = { };
		GetCursorPos(&ptPos); // ��ü �������� ��ǥ
		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptPos); // �� Ŭ���̾�Ʈ�� ��ǥ

		m_vCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y); // ���� �������� x,y ��ǥ�����

	}
	
	else // ��Ŀ�� �� �ƴҶ�
	{
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			m_vecKey[i].bPrev = false;


			if (m_vecKey[i].eState == KEY_STATE::TAP || m_vecKey[i].eState == KEY_STATE::HOLD)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}

			else if (m_vecKey[i].eState == KEY_STATE::AWAY)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}

		}
	}
}

