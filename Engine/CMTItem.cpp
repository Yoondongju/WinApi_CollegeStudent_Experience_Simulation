#include "pch.h"
#include "CMTItem.h"
#include "CTexture.h"


CMTItem::CMTItem()
{


}

CMTItem::~CMTItem()
{


}



void CMTItem::update()
{



}

void CMTItem::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();


	

	AlphaBlend(_dc
		, int(vPos.x - vScale.x / 2.f)
		, int(vPos.y - vScale.y / 2.f)
		, vScale.x    // 출력할 가로크기
		, vScale.y   // 출력할 세로크기
		, pTex->GetDC()
		, 0
		, 0
		, (int)pTex->Width()
		, (int)pTex->Height()
		, pTex->GetBf());

}

void CMTItem::ItemClickSpeech()
{
	UINT n = pG_Player->GetMyConversationID();

	if (6 == n)
		return;

	pG_Player->SetMyConversationID(++n);
	pG_Player->SetspeechBubble();

}


