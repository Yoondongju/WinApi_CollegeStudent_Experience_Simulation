#pragma once

class CCollider; 

union COLLIDER_ID
{
	struct 
	{
		UINT iLeft_id;
		UINT iRight_id;
	};
	ULONGLONG ID;
};



class CCollisionMgr
{
	SINGLE(CCollisionMgr)
		
private:
	map<ULONGLONG, bool> m_mapColInfo; // �浹ü ���� ���� ������ �浹 ����
	UINT m_arrCheck[(UINT)GROUP_TYPE::END]; // �׷찣�� �浹 üũ ��Ʈ����


public:
	void init();
	void update();
	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void Reset();


private:
	void CollisionUpdateGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);

};

