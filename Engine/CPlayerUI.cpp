#include "pch.h"
#include "CPlayerUI.h"


CPlayerUI::CPlayerUI()
	: CUI(false)
{
	
	BLENDFUNCTION bf = GetBf();
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;

	SetBf(bf);
}

CPlayerUI::~CPlayerUI()
{


}


void CPlayerUI::render_CamZoomIn(HDC _dc)
{
	CUI::render_CamZoomIn(_dc);
}

void CPlayerUI::MouseOn()
{
}

void CPlayerUI::MouseLbtnDown()
{
}

void CPlayerUI::MouseLbtnUp()
{
}
