#pragma once

#include "TSingleton.h"
typedef struct UPLOAD_INFO
{
	WCHAR szUserID[32]; // user id
	WCHAR szFolder[64]; // 프로젝트 명
	WCHAR szYear[16]; // 년도
	WCHAR szDate[32]; // 날짜
	WCHAR szFileName[64]; // 파일명
	WCHAR szLocalPath[2048];
}uploadInfo, *pUploadInfo;
class CCommonState : public TSingleton<CCommonState>
{
public:
	CCommonState();
	~CCommonState();

public:
	void SetResult(int nIndex, bool bValue);
	bool GetResult(int nIndex);

public:
	int m_nIdx;
	int m_nWorksiteSeq;
	int m_nLimitCount;
	char m_szID[32];
	bool m_bLoginResult;
	CString m_sID;
	CWnd* m_pWnd;
	CWnd* m_pManagerWnd;
	CWnd* m_pUserWnd;
	CString m_sUserID;
	int m_nReturnValue;

private:
	bool m_bResult[ProtocolHeader::DefineEndProtocol];
};

