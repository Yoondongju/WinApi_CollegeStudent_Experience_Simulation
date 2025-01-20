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
	// 윈도우 포커싱 알아내기
	//HWND hMainWnd = CCore::GetInst()->GetMainHwnd();
	HWND hWnd = GetFocus();


	
	// 윈도우 포커싱 중일때
	if (hWnd != nullptr )
	{
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			if (GetAsyncKeyState(g_arrKey[i]) & 0x8000)   // 해당 키가 눌렸다면?
			{
				if (m_vecKey[i].bPrev) // 이전에도 눌림
				{
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else // 이전엔 안눌렸다
				{
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrev = true;
			}

			else // 해당키가 안눌림
			{
				if (m_vecKey[i].bPrev) // 이전엔 눌림
				{
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else // 이전에도 안눌렸다
				{
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
				m_vecKey[i].bPrev = false;
			}
		}

		// Mouse 위치 계산
		POINT ptPos = { };
		GetCursorPos(&ptPos); // 전체 윈도우의 좌표
		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptPos); // 내 클라이언트의 좌표

		m_vCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y); // 내가 누른곳의 x,y 좌표가담김

	}
	
	else // 포커싱 중 아닐때
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

