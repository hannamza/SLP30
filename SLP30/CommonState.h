#pragma once

#include "TSingleton.h"
#include "CriticalSectionEx.h"
#include <vector>

typedef struct UPLOAD_INFO
{
	WCHAR szUserID[32]; // user id
	WCHAR szFolder[64]; // ������Ʈ ��
	WCHAR szYear[16]; // �⵵
	WCHAR szDate[32]; // ��¥
	WCHAR szFileName[64]; // ���ϸ�
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

typedef struct SELECT_CIRCUIT_COMP {
	CString sSystem;
	CString sBlock;
	CString sStair;
	CString sFloor;
	CString sCircuitName;
	int nPreviousCount;
	int nCurrentCount;
}selectCircuitComp;

typedef struct SELECT_CIRCUIT_COMP_RET {
	CString sSystem;
	CString sBlock;
	CString sStair;
	CString sFloor;
	CString sCircuitName;
	int nDiffCount;
	int nLastCircuitNo;
	//CStringArray strArrSystem;
	std::vector<CString> vecSystemName;
}selectCircuitCompRet;

class CCommonState : public TSingleton<CCommonState>
{
public:
	CCommonState();
	~CCommonState();

public:
	void ReleaseCircuit(int nType);
	void SetResult(int nIndex, bool bValue);
	bool GetResult(int nIndex);
	void InitSelectCircuitCompInfo(int nSystem);			//20221017 GBM - ȸ�� ���� �� ���� �ʱ�ȭ
	int CalculateCircuitAddressCount(CString strCircuitName);
	int CalculateTotalCircuitCount(int nSystem);	// ���� �߰�� �϶�ǥ�� Ȯ���Ǹ� ���� ȸ�� ������ ����
	void ReleaseCircuitCompInfo(int nSystem);

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

	//20221014 GBM start - ���� ���� ������ ���� ���� ���� �񱳸� ���� �߰�
	std::vector<selectCircuitComp*> m_vecCalSelectCircuit_0;
	std::vector<selectCircuitComp*> m_vecCalSelectCircuit_1;

	std::vector<selectCircuitCompRet> m_vecAddCircuit;
	std::vector<selectCircuitCompRet> m_vecDeleteCircuit;

	int m_nTotalCountCircuit_0;
	int m_nTotalCountCircuit_1;
	//20221014 GBM end

private:
	bool m_bResult[ProtocolHeader::DefineEndProtocol];
};

