#pragma once

#include "TSingleton.h"
#include <afxtempl.h>

typedef struct USER_INFO
{
	USER_INFO() {
		memset(this, 0x00, sizeof(*this));
	}
	int nSeq;
	WCHAR szName[20];
	WCHAR szPhone[20];
	WCHAR szID[20];
	WCHAR szPW[20];
	WCHAR szEtc[201];
}userInfo, *pUserInfo;

class CUserManager : public TSingleton<CUserManager>
{
public:
	CUserManager();
	~CUserManager();

public:
	bool InsertUser(WCHAR* pUser);
	bool DeleteUser(int nSeq);
	bool ModifyUser(pUserInfo pInfo);

	void ReleaseList();
	int  GetCount();
	pUserInfo GetUserInfo(int nIndex);


private:
	CList<pUserInfo, pUserInfo> m_list;
	CCriticalSectionEx mSync;
};

