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

	m_hBit = LoadPng(_strFilePath.c_str());  // bmp 파일 로딩이 안되서 png 파일로 로딩함

	//m_hBit = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP,
	//0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if (m_hBit == nullptr)
	{
		assert(m_hBit);
	}

	//==============================


	// 비트맵과 연결할 DC	 
	m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());


	// 비트맵과 DC 연결
	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hPrevBit);


	// 가로 세로 길이 알아야함
	GetObject(m_hBit, sizeof(BITMAP), &tInfo);
	tInfo.bmWidth;  // 가로
	tInfo.bmHeight; // 세로


}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	HDC maindc = CCore::GetInst()->GetMainDC();
	

	m_hBit = CreateCompatibleBitmap(maindc, _iWidth, _iHeight); // 비트맵 id 할당
	m_dc = CreateCompatibleDC(maindc);							// 비트맵에 그릴 DC id 할당

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_dc, m_hBit);   // 내가 그리고싶은 비트맵과 비트맵에 그릴 DC를 연결해줌


	DeleteObject(hOldBit);

	GetObject(m_hBit, sizeof(BITMAP), &tInfo);

}

