#pragma once

#include "TSingleton.h"
#include <afxtempl.h>

// 1. CircuitBasicInfo
typedef struct CIRCUIT_BASIC_INFO
{
	CIRCUIT_BASIC_INFO() {
		memset(this, 0x00, sizeof(CIRCUIT_BASIC_INFO));
	}
	WCHAR szBDName[128];
	WCHAR szBlockList[128];
	short nBlock;
	short nStair;
	short nFloor;
	short nBasement;
	short nRooftop;
	bool bCheck[CIRCUIT_PARENT];
}circuitBasicInfo, *pCircuitBasicInfo;

// 2. CircuitCountInfo
struct CIRUIT_COUNT
{
	CIRUIT_COUNT() {
		memset(this, 0x00, sizeof(CIRUIT_COUNT));
	}
	WCHAR szName[32];
	WCHAR szSystem[8];
	WCHAR szBlock[32];
	WCHAR szStair[8];
	WCHAR szFloor[8];
	int nCount;
};
typedef struct CIRCUIT_COUNT_INFO
{
	CIRCUIT_COUNT_INFO() {
		memset(this, 0x00, sizeof(CIRCUIT_COUNT_INFO));
	}
	int nCount;
	CIRUIT_COUNT info[0];
}circuitCountInfo, *pCircuitCountInfo;

// 3. CircuitSystemInfo
struct SYSTEM_INFO_
{
	WCHAR szBDName[64];		// 건물명
	WCHAR szCircuitName[32];// 회로명
	WCHAR szRoomName[32];	// 실
	WCHAR szBlock[32];		// 동/계단별 구분(1101동, 1101동...)
	short nNo;
	short nSystemNo;	// 중계기 No
	short nSystem;		// 계통
	short nStair;		// 계단
	short nFloor;		// 층
	short nCircuitNo;	// 설비번호
	bool bRooftop;		// 옥탑
};
typedef struct CIRCUIT_SYSTEM_INFO
{
	CIRCUIT_SYSTEM_INFO() {
		memset(this, 0x00, sizeof(CIRCUIT_SYSTEM_INFO));
	}
	int nCount;
	SYSTEM_INFO_ info[0];
}circuitSystemInfo, * pCircuitSystemInfo;

// 4. PSInfo
typedef struct CIRCUIT_PS_INFO
{
	CIRCUIT_PS_INFO() {
		memset(this, 0x00, sizeof(CIRCUIT_PS_INFO));
	}
	char use[3];
}circuitPSInfo, *pCircuitPSInfo;

// 5. PumpInfo
typedef struct CIRCUIT_PUMP_INFO
{
	CIRCUIT_PUMP_INFO() {
		memset(this, 0x00, sizeof(CIRCUIT_PUMP_INFO));
		memset(use, '1', 4);
		memset(lamp, '1', 4);
	}
	char use[4];
	char lamp[4];
}circuitPumpInfo, *pCircuitPumpInfo;

// 6. BroadcastInfo
struct BC_INFO
{
	BC_INFO() {
		memset(this, 0x00, sizeof(BC_INFO));
	}
	short nStair;
	short nFloor;
	WCHAR szBlock[32];
	WCHAR BC_CONTAIN[16];
};
typedef struct BROADCAST_INFO
{
	BROADCAST_INFO() {
		memset(this, 0x00, sizeof(BROADCAST_INFO));
	}
	int nCount;
	int nBroadType;
	BC_INFO info[0];
}BroadcastInfo, *pBroadcastInfo;

// 7. version and date
typedef struct VERSION_DATE
{
	VERSION_DATE() {
		memset(this, 0x00, sizeof(VERSION_DATE));
		cMajorVer = 1;
		cMinorVer = 1;
	}
	char cMajorVer;
	char cMinorVer;
	short nYear;
	short nMonth;
	short nDay;
	short nHour;
	short nMin;
	short nSec;
	WCHAR szUserId[20];
}versionDate, *pVersionDate;

// 8. MakeData
typedef struct MAKE_DATA
{
	MAKE_DATA() {
		memset(this, 0x00, sizeof(MAKE_DATA));
	}
	char data[CHECK_COUNT];
}makeData, *pMakeData;

class CSaveManager : public TSingleton<CSaveManager>
{
public:
	CSaveManager();
	~CSaveManager();

public:
	bool FileSave(CString sPath);
	int  ExcelFileSave(CString sPath);
	bool FileLoad(CString sPath);
	void ReleaseExcelWaitPopup();
	void ReleaseInfo();

	void SetSystemInfo(WCHAR* szBDName, WCHAR* szCircuitName, WCHAR* szRoomName, WCHAR* szBlock, short nStair, short nSystemNo, short nSystem, short nFloor, bool bRooftop, short nCircuitNo);
	void SetBroadcast(CString sBlock, short nStair, short nFloor, WCHAR* szBroadcast);

	void DeleteSystemInfo();	//20221020 GBM - 중계기 일람표 확정 후 다시 회로 설정에서 특정 회로가 삭제되었을 경우
	void AddSystemInfo();		//20221020 GBM - 중계기 일람표 확정 후 다시 회로 설정에서 특정 회로가 추가되었을 경우

	bool IsVacantCircuit(int nCircuitNum);
	bool MoveCircuitInfo(int nCurrentCircuitNum, int nNewCircuitNum);

	CList <SYSTEM_INFO_*, SYSTEM_INFO_*> m_listSystem;
	CList <BC_INFO*, BC_INFO*> m_listBC;
	circuitPumpInfo m_pumpInfo;
	circuitPSInfo m_PSInfo;
	versionDate m_versionDate;
	makeData m_makeData;
	int m_nBroadType;
};

