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
	int GetIndexCircuitName(CString sText);		// �Է� ��������� ã��
	CString GetCircuitName(int nIndex);			// �Է� �����
	CString GetCircuitInput(int nIndex);		// �Է� Ÿ�Ը�
	CString GetCircuitOutput(int nIndex);		// ��� Ÿ�Ը�
	CString GetCircuitOutputName(int nIndex);	// ��� �����

	CString GetCircuitParent(int nIndex);

	void GetCircuitName(CStringArray & arrayList);			// �Է� �����
	void GetCircuitInput(CStringArray & arrayList);			// �Է� Ÿ�Ը�
	void GetCircuitOutputType(CStringArray & arrayList);	// ��� Ÿ�Ը�
	void GetCircuitOutputName(CStringArray & arrayList);	// ��� �����

	int GetCircuitInputNo(CString sText);		// �Է� Ÿ�� ��ȣ(���� ���� ��ȣ)
	int GetCircuitOutputNo(CString sText);		// ��� Ÿ�� ��ȣ(���� ���� ��ȣ)

	bool GetCircuitChild(CString sName, CStringArray & arrayList);
	int  GetCircuitParentIndex(CString sName);
	bool IsDetector(CString sCircuitName);		//20221021 GBM - �ش� ȸ�ΰ� ���������� ����

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

