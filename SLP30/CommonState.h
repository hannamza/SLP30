#pragma once

#include "TSingleton.h"
#include "CriticalSectionEx.h"
#include <vector>

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
	void InitSelectCircuitCompInfo(int nSystem);			//20221017 GBM - 회로 설정 비교 정보 초기화
	int CalculateCircuitAddressCount(CString strCircuitName);
	int CalculateTotalCircuitCount(int nSystem);	// 최초 중계기 일람표가 확정되면 현재 회로 갯수를 저장

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

	//20221014 GBM start - 현재 설비 구성과 기존 설비 구성 비교를 위한 추가
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

