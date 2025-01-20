#include "pch.h"
#include "CPathMgr.h"

#include "CCore.h"

CPathMgr::CPathMgr()
	:m_szContentPath{}
{

}

CPathMgr::~CPathMgr()
{

}


void CPathMgr::init()
{
	GetCurrentDirectory(255, m_szContentPath); // 현재 실행된 프로그램의 경로를 알려줌  

	int iLen = wcslen(m_szContentPath);

	// 상위폴더로 나간후에  + bin // content // textrue 해줘야함

	for (int i = iLen-1; i >=0; i--)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\");

	//SetWindowText(CCore::GetInst()->GetMainHwnd(), m_szContentPath);

	
}
wstring CPathMgr::GetRelativePath(const wchar_t *_filepath)
{
	wstring strFilePath = _filepath;

	// C:\Users\User\Desktop\Engine\Output\bin\content
	// C:\Users\User\Desktop\Engine\Output\bin\content\Tile
	
	size_t iAbsLen = wcslen(m_szContentPath);  // 48 개
	size_t iFulllen = strFilePath.length();    // 62 개


	// iFulllen- iAbsLen == 14개
	wstring strRelativePath = strFilePath.substr(iAbsLen, iFulllen- iAbsLen); // iAbsLen 위치부터 iFulllen- iAbsLen개의 글자까지

	return strRelativePath;
}
