#include "pch.h"
#include "CTimeMgr.h"
#include "CCore.h"

CTimeMgr::CTimeMgr()
	: m_llPrevCount{}
	, m_llCurCount{}
	, m_llFrequency{}
	, m_dDT(0.)
	, m_dAcc(0.)
	, m_iCallCount()
	, m_iFPS()
{

}


CTimeMgr::~CTimeMgr()
{


}

void CTimeMgr::init()
{
	// 초기화 할때 현재 카운트 (update 시점에서는 이전 카운트임)
	QueryPerformanceCounter(&m_llPrevCount);

	// 초당 카운트 횟수
	QueryPerformanceFrequency(&m_llFrequency);


}

void CTimeMgr::update()
{
	// 1 FPS ? ( update 시점에서는 현재 카운트 )
	QueryPerformanceCounter(&m_llCurCount);

	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;


	// 현재 카운트를 이전카운트에 넣음 (다음번 갱신을 위해)
	m_llPrevCount = m_llCurCount;


#ifdef _DEBUG
	if (m_dDT > (1. / 60.))
	{
		m_dDT = (1. / 60.);
	}


#endif


}

void CTimeMgr::render()
{

	++m_iCallCount;
	m_dAcc += m_dDT;


	if (m_dAcc >= 1.)
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;

		wchar_t szBufffer[255] = {};

		swprintf_s(szBufffer, L"FPS: %d , DT: %f", m_iFPS, m_dDT);
		SetWindowText(CCore::GetInst()->GetMainHwnd(), szBufffer);
	}
}
