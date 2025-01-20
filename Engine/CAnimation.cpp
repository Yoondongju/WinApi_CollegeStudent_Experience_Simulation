#include "pch.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CTexture.h"
#include "CObject.h"

#include "CTimeMgr.h"
#include "CCamera.h"

#include "CPathMgr.h"
#include "CResMgr.h"

#include "CUI.h"


CAnimation::CAnimation()
	: m_pAnimator(nullptr)
	, m_pTex(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{


}

CAnimation::~CAnimation()
{


}


void CAnimation::update()
{
	if (m_bFinish)
		return;

	m_fAccTime += fDT;

	// ���� �����ӿ��� �����ϴ� �ð��� ����� ������ �������������� �̵�
	if (m_fAccTime > m_vecFrm[m_iCurFrm].fDuration)
	{
		m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;
		++m_iCurFrm;
		if (m_vecFrm.size() <= m_iCurFrm)
		{

			m_iCurFrm = -1;
			m_bFinish = true;
		}
	}
}


void CAnimation::render(HDC _dc)
{
	if (m_bFinish) // �ִϸ��̼� ��� 1�� �������� ������ ����
		return;

	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();

	if (L"MBTI_UI" == pObj->GetName())
	{
		CUI* pUI = (CUI*)pObj;
		vPos = pUI->GetFinalPos();
	}

	Vec2 vScale = pObj->GetScale();
	
	vPos = vPos + m_vecFrm[m_iCurFrm].vOffset;




	// ������ ��ǥ�� ��ȯ
	//vPos = CCamera::GetInst()->GetRenderPos(vPos);


	//if (!pObj->GetCamAffectedObj())
	//{
	//	vPos = CCamera::GetInst()->GetRealPos(vPos);
	//}
	

	 // Rectangle(_dc,
	 // 	  (int)(vPos.x - vScale.x / 2.f)
	 // 	, (int)(vPos.y - vScale.y / 2.f)
	 // 	, (int)(vPos.x + vScale.x / 2.f)
	 // 	, (int)(vPos.y + vScale.y / 2.f));


	float widthRatio = vScale.x / m_vecFrm[m_iCurFrm].vSlice.x;
	float heightRatio = vScale.y / m_vecFrm[m_iCurFrm].vSlice.y;

	 //vScale.x /= 100.f;   // Player�� Scale�� �°� ũ�� ����
	 //vScale.y /= 100.f;	  // Player�� Scale�� �°� ũ�� ����

	 

	AlphaBlend(_dc
		 , (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f *  widthRatio)
		 , (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.f *  heightRatio)
		 , (int)(m_vecFrm[m_iCurFrm].vSlice.x  * widthRatio)
		 , (int)(m_vecFrm[m_iCurFrm].vSlice.y  * heightRatio)
		 , m_pTex->GetDC()
		 , (int)(m_vecFrm[m_iCurFrm].vLT.x)
		 , (int)(m_vecFrm[m_iCurFrm].vLT.y)
		 , (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		 , (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		 , pObj->GetBf());


	 


	//AlphaBlend(_dc
	//	 , (int)(vPos.x - vScale.x / 2.f)
	//	 , (int)(vPos.y - vScale.y / 2.f)
	//	 , (int)(vPos.x + vScale.x / 2.f)
	//	 , (int)(vPos.y + vScale.y / 2.f)
	//	 , m_pTex->GetDC()
	//	 , (int)(m_vecFrm[m_iCurFrm].vLT.x)
	//	 , (int)(m_vecFrm[m_iCurFrm].vLT.y)
	//	 , (int)(m_vecFrm[m_iCurFrm].vSlice.x)
	//	 , (int)(m_vecFrm[m_iCurFrm].vSlice.y)
	//	 , pObj->GetBf());


	//����Ÿ �����϶� �׷��ִ� �Լ���
	//TransparentBlt(_dc
	//	, (int) (vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f)
	//	, (int) (vPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.f)
	//	, (int) (m_vecFrm[m_iCurFrm].vSlice.x				)
	//	, (int) (m_vecFrm[m_iCurFrm].vSlice.y				)
	//	, m_pTex->GetDC()							
	//	, (int) (m_vecFrm[m_iCurFrm].vLT.x					)
	//	, (int) (m_vecFrm[m_iCurFrm].vLT.y					)
	//	, (int) (m_vecFrm[m_iCurFrm].vSlice.x				)
	//	, (int) (m_vecFrm[m_iCurFrm].vSlice.y				)
	//	, RGB(255,0,255)								
	//);
}

void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vScliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pTex = _pTex;

	tAnimFrm frm = {};
	for (int i = 0; i < _iFrameCount; ++i)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vScliceSize;
		frm.vLT = _vLT + _vStep * i;


		m_vecFrm.push_back(frm);
	}

}


void CAnimation::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	assert(pFile);

	
	// Animation�� �̸��� ���� 1����Ʈ ������
	fprintf(pFile, "[Animation_Name]\n");
	string strName = string(m_strName.begin(), m_strName.end());

	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");


	// �ؽ��� Ű�� ����
	fprintf(pFile, "[Texture_Key]\n");
	string strKey = string(m_pTex->GetKey().begin(), m_pTex->GetKey().end());

	fprintf(pFile, strKey.c_str());
	fprintf(pFile, "\n");

	// �ؽ��� ��� ����
	fprintf(pFile, "[Texture_RelativePath]\n");
	string strPath = string(m_pTex->GetRelativePath().begin(), m_pTex->GetRelativePath().end());

	fprintf(pFile, strPath.c_str());
	fprintf(pFile, "\n");

	// ������ ����
	fprintf(pFile, "[Frame_Count]\n");
	fprintf(pFile, "%d\n", (int)m_vecFrm.size());

	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		fprintf(pFile, "===============%d_��°_�ε���_����==============\n\n", i);
		fprintf(pFile, "[Frame_Index]\n");
		fprintf(pFile, "%d\n" , (int)i);

		fprintf(pFile, "[Left_Top]\n");
		fprintf(pFile, "%d, %d\n", (int)m_vecFrm[i].vLT.x, (int)m_vecFrm[i].vLT.y);

		fprintf(pFile, "[Slice_Size]\n");
		fprintf(pFile, "%d, %d\n", (int)m_vecFrm[i].vSlice.x, (int)m_vecFrm[i].vSlice.y);

		fprintf(pFile, "[Offset]\n");
		fprintf(pFile, "%d, %d\n", (int)m_vecFrm[i].vOffset.x, (int)m_vecFrm[i].vOffset.y);

		fprintf(pFile, "[Duration]\n");
		fprintf(pFile, "%f\n", (float)m_vecFrm[i].fDuration);

	}
	

	
	// Animation�� �̸��� �����Ұ��� ( ������ ����ȭ ) 
	//const wchar_t* pStrName = m_strName.c_str();  // m_strName ���ڿ��� �ּ�
	//size_t iLen = m_strName.length();
	// 
	// ���� ���� ����
	// fwrite(&iLen, sizeof(size_t), 1, pFile);
	// ���ڿ� ����
	// fwrite(pStrName, sizeof(wchar_t), iLen, pFile);


	// Animation�� ����ϴ� �ؽ��� Ű�� 
	// const wstring& texKey = m_pTex->GetKey();
	// const wchar_t* pTexKey = texKey.c_str();
	// size_t iTexKeyLen = texKey.length();


	// Animation�� ����ϴ� �ؽ��� ���
	// const wstring& texRelativePath = m_pTex->GetRelativePath();
	// const wchar_t* pTexPath = texRelativePath.c_str();
	// size_t iTexPathLen = texRelativePath.length();

	// �ؽ��� Ű�� ���� ����
	// fwrite(&iTexKeyLen, sizeof(size_t), 1, pFile);
	// �ؽ��� Ű�� ���ڿ� ����
	// fwrite(pTexKey, sizeof(wchar_t), iTexKeyLen, pFile);

	// ������ ����
	// size_t iFrameCount = m_vecFrm.size();
	// fwrite(&iFrameCount, sizeof(size_t), 1, pFile);

	// ������ ����
	// fwrite(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);
	

	fclose(pFile);
}


void CAnimation::Load(const wstring& _strRelativePath)
{
	m_vecFrm;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	assert(pFile);


	// Animation�� �̸��� �о�´�.
	string str;
	char szBuff[256] = {};

	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);
	str = szBuff;

	m_strName = wstring(str.begin(), str.end());
	
	// �����ϴ� �ؽ��� Ű��, �� ���
	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);
	str = szBuff;
	wstring strTextKey = wstring(str.begin(), str.end());

	FScanf(szBuff, pFile);
	FScanf(szBuff, pFile);
	str = szBuff;
	wstring strTexPath = wstring(str.begin(), str.end());

	m_pTex = CResMgr::GetInst()->LoadTexture(strTextKey, strTexPath);

	FScanf(szBuff, pFile);

	int iFrameCount = 0;
	fscanf_s(pFile, "%d", &iFrameCount);
	 
	tAnimFrm frm = {};

	for (int i = 0; i < iFrameCount; ++i)
	{
		POINT pt = {};
		float f = 0.f;

		while (true)
		{
			FScanf(szBuff, pFile);
			

			if (!strcmp("[Frame_Index]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
			}
			else if (!strcmp("[Left_Top]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);
				frm.vLT = Vec2(pt);
			}
			else if (!strcmp("[Slice_Size]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);
				frm.vSlice = Vec2(pt);
			}
			else if (!strcmp("[Offset]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);
				frm.vOffset = Vec2(pt);
			}
			else if (!strcmp("[Duration]", szBuff))
			{
				fscanf_s(pFile, "%f", &f);
				frm.fDuration = f;
				break;
			}
			 
		}

		m_vecFrm.push_back(frm);
	}

	 

	fclose(pFile);
}



