#include "pch.h"
#include "CTexture.h"
#include "CCore.h"

#include <gdiplus.h>
#pragma comment(lib,"gdiplus")
using namespace Gdiplus;


CTexture::CTexture()
	: m_dc(0)
	, m_hBit(0)
	, tInfo{}
{
	BLENDFUNCTION bf = {};

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;
	SetBf(bf);

}

CTexture::~CTexture()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}

HBITMAP LoadPng(const wchar_t* filename)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	HBITMAP hBitmap = nullptr;
	Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile(filename);

	if (pBitmap != nullptr)
	{
		pBitmap->GetHBITMAP(Color::White, &hBitmap);
		delete pBitmap;
	}
	return hBitmap;
}


void CTexture::Load(const wstring& _strFilePath) 
{

	m_hBit = LoadPng(_strFilePath.c_str());  // bmp ���� �ε��� �ȵǼ� png ���Ϸ� �ε���

	//m_hBit = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP,
	//0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if (m_hBit == nullptr)
	{
		assert(m_hBit);
	}

	//==============================


	// ��Ʈ�ʰ� ������ DC	 
	m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());


	// ��Ʈ�ʰ� DC ����
	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hPrevBit);


	// ���� ���� ���� �˾ƾ���
	GetObject(m_hBit, sizeof(BITMAP), &tInfo);
	tInfo.bmWidth;  // ����
	tInfo.bmHeight; // ����


}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	HDC maindc = CCore::GetInst()->GetMainDC();
	

	m_hBit = CreateCompatibleBitmap(maindc, _iWidth, _iHeight); // ��Ʈ�� id �Ҵ�
	m_dc = CreateCompatibleDC(maindc);							// ��Ʈ�ʿ� �׸� DC id �Ҵ�

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_dc, m_hBit);   // ���� �׸������ ��Ʈ�ʰ� ��Ʈ�ʿ� �׸� DC�� ��������


	DeleteObject(hOldBit);

	GetObject(m_hBit, sizeof(BITMAP), &tInfo);

}

