#pragma once

#include "TSingleton.h"
#include "CriticalSectionEx.h"
#include "afxtempl.h"
#include <atlcoll.h>

typedef struct CIRCUIT_INFO {
	int nIndex;
	CString sCircuitName;
	CStringArray arrayChild;
}circuitInfo, *pCircuitInfo;

class CCircuitBasicInfo
{
public:
	CCircuitBasicInfo();
	~CCircuitBasicInfo();

	void SetInfoInit();
	int GetIndexCircuitName(CString sText);		// 입력 설비명으로 찾기
	CString GetCircuitName(int nIndex);			// 입력 설비명
	CString GetCircuitInput(int nIndex);		// 입력 타입명
	CString GetCircuitOutput(int nIndex);		// 출력 타입명
	CString GetCircuitOutputName(int nIndex);	// 출력 설비명

	CString GetCircuitParent(int nIndex);

	void GetCircuitName(CStringArray & arrayList);			// 입력 설비명
	void GetCircuitInput(CStringArray & arrayList);			// 입력 타입명
	void GetCircuitOutputType(CStringArray & arrayList);	// 출력 타입명
	void GetCircuitOutputName(CStringArray & arrayList);	// 출력 설비명

	int GetCircuitInputNo(CString sText);		// 입력 타입 번호(설비 적용 번호)
	int GetCircuitOutputNo(CString sText);		// 출력 타입 번호(설비 적용 번호)

	bool GetCircuitChild(CString sName, CStringArray & arrayList);
	int  GetCircuitParentIndex(CString sName);
	bool IsDetector(CString sCircuitName);		//20221021 GBM - 해당 회로가 감지기인지 여부

	static CCircuitBasicInfo* Instance();

public:
	CString m_sBuildingName;
	int m_nStair;
	int m_nFloor;
	int m_nBasement;
	int m_nBlock;
	CString m_sBlockName;
	CStringArray m_arrayBlockName;

	bool m_bCheck[CIRCUIT_PARENT];

private:
	CAtlMap<CAtlString, int> m_map;
	CList<pCircuitInfo, pCircuitInfo> m_listCircuit;

protected:
	static CCircuitBasicInfo g_circuitBasicInfo;
};

