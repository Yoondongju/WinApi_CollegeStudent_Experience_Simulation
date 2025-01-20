#include "pch.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

CCollisionMgr::CCollisionMgr()
	:m_arrCheck{}
{

}

CCollisionMgr::~CCollisionMgr()
{

}



void CCollisionMgr::init()
{
}



void CCollisionMgr::update()  
{
	// UINT 는 unsigned int로 양수만 표현하는 4바이트 자료형임
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; iRow++)
	{	
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; iCol++)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionUpdateGroup((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);

			}

		}

	}

}


void CCollisionMgr::CollisionUpdateGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();


	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

	map<ULONGLONG, bool> ::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); i++)
	{
		// 왼쪽Obj의 충돌체가 없을때
		if (nullptr == vecLeft[i]->GetCollider())
		{
			continue;
		}
			

		for (size_t j = 0; j < vecRight.size(); j++)
		{
			// 오른쪽Obj의 충돌체가 없을때거나 자기 자신이랑 충돌이면
			if (nullptr == vecRight[j]->GetCollider() || vecLeft[i] == vecRight[j])
			{
				continue;
			}
			

			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();

			// 두 충돌체 조합 아이디 생성 
			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCol->GetID();
			ID.iRight_id = pRightCol->GetID();
			iter = m_mapColInfo.find(ID.ID);
			


			// 충돌 정보가 미 등록 상태인 경우 (충돌하지 않았다 로) 
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID,false));
				iter = m_mapColInfo.find(ID.ID);
			}


			
			if (IsCollision(pLeftCol, pRightCol))
			{
				// 현재 충돌 중이다.

				if (iter->second) // 이전에도 충돌했네?
				{
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// 근데 둘중 하나가 삭제예정일때 충돌해제
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;

					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
					
				}

				else
				{
					// 이전에는 충돌하지 않았다.
					// 근데 둘중 하나가 삭제예정일때 충돌 안한것으로 취급함
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{				
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
					
				}
			}

			else // 현재 충돌 하고 있지 않다.
			{
				if (iter->second) // 이전엔 충돌했네?
				{
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}


			}

		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{

	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if (abs(vRightPos.x - vLeftPos.x) < (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.y - vLeftPos.y) < (vLeftScale.y + vRightScale.y) / 2.f)
	{
		return true;
	}


	return false;
}



void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// 더 작은 값의 그룹 타입을 행으로,
	// 큰 값을 열(비트) 로 사용

	UINT iRow =  (UINT)_eLeft;   // 행
	UINT iCol =  (UINT)_eRight;  // 열

	if (iRow > iCol)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}


	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}
	
}


void CCollisionMgr::Reset()
{
	memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END);
	
}





