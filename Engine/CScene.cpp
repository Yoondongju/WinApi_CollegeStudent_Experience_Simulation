#include "pch.h"
#include "CScene.h"
#include "CObject.h"

#include "CTile.h"
#include "CTimeMgr.h"
#include "CTalkUI.h"

#include "CResMgr.h"
#include "CPathMgr.h"
#include "CTexture.h"

#include "CCamera.h"
#include "CCore.h"
#include "CUI.h"
#include "CBtnUI.h"
#include "CEffect.h"

#include <chrono>
#include <thread>


CScene::CScene()
	: m_iTileX(0)
	, m_iTileY(0)
	, m_pPlayer(nullptr)
	, m_pBackground_Texture(nullptr)
	, m_MainTalkUI(nullptr)
{

}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			// i��° �׷��� j��ü�� ����
			delete m_arrObj [i][j];
		}
	}
}

void CScene::start()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			// i��° �׷��� j ������ŭ�� ��ü���� �ʱ�ȭ ������
			m_arrObj[i][j]->start();
		}
	}


}


void CScene::update()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			if (!m_arrObj[i][j]->IsDead())
			{
				m_arrObj[i][j]->update();
			}			
		}
	}
}


void CScene::finalupdate()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			if (!m_arrObj[i][j]->IsDead())
			{
				m_arrObj[i][j]->finalupdate();
			}
		}
	}

	CTalkUI* mainTalkUI = GetMainTalkUI();
	if (nullptr != mainTalkUI)
	{
		if (mainTalkUI->IsDead())
		{
			SetMainTalkUI(nullptr);
		}
	}

}

void CScene::subsequent_update()
{
	// vector<CObject*> vecObj = GetUIGroup();
	// for (size_t i = 0; i < vecObj.size(); ++i)
	// {
	// 	CTalkUI* pTalkUI = dynamic_cast<CTalkUI*>(vecObj[i]);
	// 	if (pTalkUI->IsStart())
	// 	{
	// 		float time = 0.f;
	// 		while (true)
	// 		{
	// 			time += fDT;
	// 			if (time > 1.5f)
	// 			{
	// 				ChangeScene(SCENE_TYPE::STAGE_LOBBY);
	// 				time = 0.f;
	// 				break;
	// 			}
	// 			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(fDT * 1000)));
	// 		}
	// 	}
	// 
	// }
}


void CScene::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		//if ((UINT)GROUP_TYPE::TILE == i)
		//{
		//	render_tile(_dc);
		//	continue;
		//
		//}

		vector<CObject*>::iterator iter = m_arrObj[i].begin();


		for (; iter < m_arrObj[i].end();)
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->render(_dc);

				if (i == (UINT)GROUP_TYPE::UI)
				{
					CUI* pUI = dynamic_cast<CUI*>(*iter);
					if (pUI)
					{
						CBtnUI* pBtnUI = dynamic_cast<CBtnUI*>(pUI);
						if (pBtnUI)
						{
							CEffect* pEffect = pBtnUI->GetEffect();
							if (nullptr != pEffect)
							{
								pEffect->render(_dc);
							}
						}


						vector<CUI*> pChildUI = pUI->GetChildUI();
						
						for (size_t i = 0; i < pChildUI.size(); ++i)
						{
							CBtnUI* pChlidBtnUI = dynamic_cast<CBtnUI*>(pChildUI[i]);
							if (pChlidBtnUI)
							{
								CEffect* pEffect = pChlidBtnUI->GetEffect();
								if (nullptr != pEffect)
								{
									pEffect->render(_dc);
								}
							}
						}
					}

				}
				++iter;
			}
			else
			{
				iter = m_arrObj[i].erase(iter);
			}
			
		}

	}
}

void CScene::render_tile(HDC _dc)
{
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	Vec2 vCamLook = CCamera::GetInst()->GetLookAt(); // ī�޶� �����ִ°�
	Vec2 vResolution = CCore::GetInst()->GetResolution(); // �츮 ���� �ػ�

	Vec2 vLeftTop = vCamLook - vResolution/2.f;

	int iTileSize = TILE_SIZE;

	int iLTCol = (int)vLeftTop.x / iTileSize;
	int iLTRow = (int)vLeftTop.y / iTileSize;
	






	int iClientWidth = ((int)vResolution.x / iTileSize)  +1;
	int iClientHeight = ((int)vResolution.y / iTileSize) + 1;

	for (int iCurRow = iLTRow; iCurRow < (iLTRow + iClientHeight); ++iCurRow)
	{
		for (int iCurCol = iLTCol; iCurCol < (iLTCol + iClientWidth); ++iCurCol)
		{
			if (iCurCol < 0 || m_iTileX <= iCurCol
				|| iCurRow < 0 || m_iTileY <= iCurRow)
			{
				continue;
			}

			int iIdx = (m_iTileX * iCurRow) + iCurCol;

			vecTile[iIdx]->render(_dc);
			vecTile[iIdx]->component_render(_dc);

		}


	}

}


void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	Safe_Delete_Vec<CObject*> (m_arrObj[(UINT)_eTarget]);
}


void CScene::DeleteAll()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		if ((UINT)GROUP_TYPE::PLAYER == i) // ���� Player�� ����� ����
		{
			if (m_arrObj[i].size() >= 1)
			{
				CPlayer* pPlayer = (CPlayer*)m_arrObj[i][0];
				pPlayer->SetstarEffect(nullptr);
				m_arrObj[i].clear();
				continue;
			}
			
		}
			

		
		// ����Ʈ�� ���� �ٲ� ���⼭ �����ϴ°��� �ƴ϶� �׶��׶�
		// ������� ������Ʈ�� ���콺�� ������ ���ų� � Ư����Ȳ�϶� ���� ������Ʈ�� �Բ� �������
		// ���ʰ� ���� ����Ʈ�� ���߿� ����ȯ�� �����Ǵºκ��� ���� ������ �κ��� �ʹ� ���� ���� �ϰ���.

		
		DeleteGroup((GROUP_TYPE)i);
	} 

}


void CScene::CreateTile()
{
	DeleteGroup(GROUP_TYPE::TILE);

	CTexture* pTileTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\map.png");


	m_iTileX = pTileTex->Width() / TILE_SIZE;
	m_iTileY = pTileTex->Height() / TILE_SIZE;



	

	// Ÿ�� ����
	for (UINT i = 0; i < m_iTileY; ++i) // ��
	{
		for (UINT j = 0; j < m_iTileX; ++j) // ��
		{
			CTile* pTile = new CTile();


			pTile->SetPos(Vec2((float)(j * TILE_SIZE), (float)(i * TILE_SIZE)));	

			pTile->SetTexture(pTileTex);
			pTile->SetImgIdx(m_iTileY * i + j);  

			AddObject(pTile, GROUP_TYPE::TILE);

		}
	}

	


}


void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// Ŀ�� ������Ʈ
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	if (nullptr == pFile)
	{
		// ���� ���� ����
		assert(pFile);
	}

	UINT xCount = 0;
	UINT yCount = 0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	//CreateTile();

	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Load(pFile);

	}


	fclose(pFile);
}



