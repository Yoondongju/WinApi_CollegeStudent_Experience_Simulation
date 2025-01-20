#include "pch.h"
#include "CCore.h"
#include "CTile.h"
#include "CTexture.h"

#include "CCollider.h"
#include "CCollisionMgr.h"



CTile::CTile()
	: m_pTileTex(nullptr)
	, m_iImgIdx(0)
{
	SetScale(Vec2(TILE_SIZE, TILE_SIZE));
	CreateCollider();

	GetCollider()->SetOffsetPos(Vec2(TILE_SIZE / 2.f, TILE_SIZE / 2.f));
	GetCollider()->SetScale(Vec2(TILE_SIZE, TILE_SIZE));

	BLENDFUNCTION bf = GetBf();
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;
	SetBf(bf);

}

CTile::~CTile()
{


}
 

void CTile::update()
{
	
}

void CTile::render(HDC _dc)
{
	if (nullptr == m_pTileTex || -1 == m_iImgIdx)
		return;

	UINT iWidth = m_pTileTex->Width();
	UINT iHeight = m_pTileTex->Height();

	UINT iMaxCol = iWidth  / TILE_SIZE; // 행
	UINT iMaxRow = iHeight / TILE_SIZE; // 열

	UINT iCurRow = (UINT)m_iImgIdx / iMaxCol;
	UINT iCurCol = (UINT)m_iImgIdx % iMaxCol;

	// 이미지 범위를 벗어난 인덱스
	if (iMaxRow <= iCurRow)
		assert(nullptr);


	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();


	AlphaBlend(_dc
		, int(vRenderPos.x)
		, int(vRenderPos.y)
		, int(vScale.x	  )
		, int(vScale.y	  )
		, m_pTileTex->GetDC()
		, iCurCol * TILE_SIZE
		, iCurRow * TILE_SIZE
		, TILE_SIZE
		, TILE_SIZE
		, GetBf());


	

	

}


void CTile::Save(FILE* _pFile)
{
	fwrite(&m_iImgIdx, sizeof(int), 1, _pFile);
}

void CTile::Load(FILE* _pFile)
{
	fread(&m_iImgIdx, sizeof(int), 1, _pFile);
}
