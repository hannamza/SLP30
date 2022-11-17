#include "stdafx.h"
#include "UserManager.h"

CUserManager::CUserManager()
{
}

CUserManager::~CUserManager()
{
	ReleaseList();
}

bool CUserManager::InsertUser(WCHAR* pValue)
{
	mSync.Enter();
	pUserInfo pUser = new userInfo;
	CString sUser = pValue;
	CString sTemp;
	if (AfxExtractSubString(sTemp, sUser, 0, L'」')) {
		pUser->nSeq = _wtoi(sTemp.GetBuffer(0));
	}
	if (AfxExtractSubString(sTemp, sUser, 1, L'」')) {
		wcscpy_s(pUser->szName, sTemp.GetBuffer(0));
	}
	if (AfxExtractSubString(sTemp, sUser, 2, L'」')) {
		wcscpy_s(pUser->szPhone, sTemp.GetBuffer(0));
	}
	if (AfxExtractSubString(sTemp, sUser, 3, L'」')) {
		wcscpy_s(pUser->szID, sTemp.GetBuffer(0));
	}
	if (AfxExtractSubString(sTemp, sUser, 4, L'」')) {
		wcscpy_s(pUser->szEtc, sTemp.GetBuffer(0));
	}

	pUserInfo pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSeq == pUser->nSeq)
		{
			mSync.Leave();
			SAFE_DELETE(pUser);
			return false;
		}
	}

	m_list.AddTail(pUser);
	mSync.Leave();
	return true;
}

bool CUserManager::DeleteUser(int nSeq)
{
	mSync.Enter();
	pUserInfo pInfo = NULL;
	POSITION oldPos;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		oldPos = pos;
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSeq == nSeq)
		{
			m_list.RemoveAt(oldPos);
			mSync.Leave();
			return true;;
		}
	}
	mSync.Leave();
	return false;
}

bool CUserManager::ModifyUser(pUserInfo pUser)
{
	mSync.Enter();
	pUserInfo pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		if (pInfo->nSeq == pUser->nSeq)
		{
			memcpy_s(pInfo, sizeof(userInfo), pUser, sizeof(userInfo));
			mSync.Leave();
			return true;;
		}
	}
	mSync.Leave();
	return false;
}

int CUserManager::GetCount()
{
	mSync.Enter();
	int nCount = m_list.GetCount();
	mSync.Leave();
	return nCount;
}

pUserInfo CUserManager::GetUserInfo(int nIndex)
{
	mSync.Enter();
	if (m_list.GetCount() <= nIndex) {
		mSync.Leave();
		return NULL;
	}
	pUserInfo pInfo = m_list.GetAt(m_list.FindIndex(nIndex));
	mSync.Leave();

	return pInfo;
}

void CUserManager::ReleaseList()
{
	mSync.Enter();
	pUserInfo pInfo = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		pInfo = m_list.GetNext(pos);
		SAFE_DELETE(pInfo);
	}
	m_list.RemoveAll();
	mSync.Leave();
}
