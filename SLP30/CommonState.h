#pragma once

#include "TSingleton.h"
#include "CriticalSectionEx.h"

typedef struct UPLOAD_INFO
{
	WCHAR szUserID[32]; // user id
	WCHAR szFolder[64]; // 프로젝트 명
	WCHAR szYear[16]; // 년도
	WCHAR szDate[32]; // 날짜
	WCHAR szFileName[64]; // 파일명
	WCHAR szLocalPath[2048];
}uploadInfo, *pUploadInfo;

typedef struct SELECT_CIRCUIT {
	CString sSystem;
	CString sBlock;
	CString sStair;
	CString sFloor;
	CString sCircuitName;
	int nCount;
}selectCircuit, *pSelectCircuit;

class CCommonState : public TSingleton<CCommonState>
{
public:
	CCommonState();
	~CCommonState();

public:
	void ReleaseCircuit(int nType);
	void SetResult(int nIndex, bool bValue);
	bool GetResult(int nIndex);

public:
	CWnd* m_pWnd;
	CWnd* m_pEditWnd;
	bool m_bWaitCircuit;
	bool m_bLoginResult;
	int  m_nIdx;
	bool m_bLogin;
	bool m_bMax;
	bool m_bMenu;
	bool m_bInitCircuit;
	CString m_sUserID;
	int  m_nReturnValue;

	CList<pSelectCircuit, pSelectCircuit> m_selectCircuit_0;
	CList<pSelectCircuit, pSelectCircuit> m_selectCircuit_1;

private:
	bool m_bResult[ProtocolHeader::DefineEndProtocol];
};

