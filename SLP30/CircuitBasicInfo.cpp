#include "stdafx.h"
#include "CircuitBasicInfo.h"

CCircuitBasicInfo CCircuitBasicInfo::g_circuitBasicInfo;

CCircuitBasicInfo::CCircuitBasicInfo()
{
	m_nStair = 1;
	m_nFloor = 2;
	m_nBasement = 0;
	m_nBlock = 1;

	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_bCheck[nIndex] = false;
	}

	for (int nIndex = 0; nIndex < CIRCUIT_CNT; nIndex++) {
		m_map.SetAt(CAtlString(g_lpszCircuitName[nIndex]), nIndex);
	}

	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		pCircuitInfo pInfo = new circuitInfo;
		pInfo->sCircuitName = g_lpszCircuitParent[nIndex];
		pInfo->nIndex = nIndex;
		m_listCircuit.AddTail(pInfo);
		switch (nIndex) {
		case 0: // ��ȭ��
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_��ȭ��]);
			break;
		case 1: // �߽ű�
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_�߽ű�]);
			break;
		case 2: // �˶����
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_AVP_S]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_AVT_S]);
			break;
		case 3: // �����׼ǹ��
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_SVP������A]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_SVP������B]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_SVPP_S]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_SVPT_S]);
			break;
		case 4: // �ð��溸
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_�ð��溸]);
			break;
		case 5: // ���������ޱ�
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_���Ǳޱ����]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_���Ǳޱ����]);
			break;
		case 6: // �����������
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_���ǹ�����]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_���ǹ�����]);
			break;
		case 7: // ������
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_���������⵿]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_SMDȮ��]);
			//pInfo->arrayChild.Add(g_lpszCircuitName[�����_�ޱ���]);
			//pInfo->arrayChild.Add(g_lpszCircuitName[�����_�����]);
			break;
		case 8: // ��ȭ����
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_���Ϳ�������]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_���߿�������]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_����1�����]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_����2�����]);
			break;
		case 9: // �迬â
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_�迬â]);
			break;
		case 10: // ��ȭ��
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_��ȭ��]);
			break;
		case 11: // â����ġ
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_â�����]);
			break;
		case 12: // ������ġ
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_�ڵ����]);
			break;
		case 13: // ��ȭ���⵿Ȯ��
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_P_L����]);
			break;
		case 14: // ������Ȯ��
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_����]);
			break;
		case 15: // ����������
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_������F_S]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_������T_S]);
			break;
		case 16: // �����ޱ���
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_�ޱ���]);
			break;
		case 17: // ���������
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_�����]);
			break;
		case 18: // ������
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_��ܰ�����]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_ELEV������]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_������]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_AN����]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_AN����]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_�ּ�������]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_�ּ�������]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_�ּ�������]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_AN�����ܰ�����]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_AN����ELEV������]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_�ּ��������ܰ�����]);
			pInfo->arrayChild.Add(g_lpszCircuitName[�����_�ּ�������ELEV������]);
			break;
		default:
			break;
		}
	}
}

CCircuitBasicInfo::~CCircuitBasicInfo()
{
	pCircuitInfo pInfo = NULL;
	//m_map.RemoveAll();
	while (!m_listCircuit.IsEmpty()) {
		pInfo = m_listCircuit.RemoveHead();
		SAFE_DELETE(pInfo);
	}
}

CCircuitBasicInfo* CCircuitBasicInfo::Instance()
{
	return &g_circuitBasicInfo;
}

void CCircuitBasicInfo::SetInfoInit()
{
	m_nStair = 1;
	m_nFloor = 2;
	m_nBasement = 0;

	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_bCheck[nIndex] = false;
	}
}

int CCircuitBasicInfo::GetIndexCircuitName(CString sText)
{
	//int nIndex = -1;
	for (int nIndex = 0; nIndex < CIRCUIT_CNT; nIndex++) {
		if (sText.Compare(g_lpszCircuitName[nIndex]) == 0) {
			return nIndex;
		}
	}
	/*if (m_map.Lookup(CAtlString(sText), nIndex)) {
		return nIndex;
	}*/
	return -1;
}

CString CCircuitBasicInfo::GetCircuitName(int nIndex)
{
	if (nIndex >= CIRCUIT_CNT) {
		return L"";
	}
	return g_lpszCircuitName[nIndex];
}
CString CCircuitBasicInfo::GetCircuitInput(int nIndex)
{
	if (nIndex >= CIRCUIT_CNT) {
		return L"";
	}
	return g_lpszInputType[nIndex];
}

CString CCircuitBasicInfo::GetCircuitOutput(int nIndex)
{
	if (nIndex >= CIRCUIT_CNT) {
		return L"";
	}
	return g_lpszOutputType[nIndex];
}

CString CCircuitBasicInfo::GetCircuitOutputName(int nIndex)
{
	if (nIndex >= CIRCUIT_CNT) {
		return L"";
	}
	return g_lpszOutputCircuit[nIndex];
}

CString CCircuitBasicInfo::GetCircuitParent(int nIndex)
{
	if (nIndex >= CIRCUIT_PARENT) {
		return L"";
	}
	return g_lpszCircuitParent[nIndex];
}

void CCircuitBasicInfo::GetCircuitName(CStringArray & arrayList)
{
	for (int nIndex = 0; nIndex < CIRCUIT_CNT; nIndex++) {
		arrayList.Add(g_lpszCircuitName[nIndex]);
	}
}

void CCircuitBasicInfo::GetCircuitInput(CStringArray & arrayList)
{
	for (int nIndex = 0; nIndex < CIRCUIT_CNT; nIndex++) {
		arrayList.Add(g_lpszInputType[nIndex]);
	}
}

void CCircuitBasicInfo::GetCircuitOutputType(CStringArray & arrayList)
{
	for (int nIndex = 0; nIndex < CIRCUIT_CNT; nIndex++) {
		arrayList.Add(g_lpszOutputType[nIndex]);
	}
}

void CCircuitBasicInfo::GetCircuitOutputName(CStringArray & arrayList)
{
	for (int nIndex = 0; nIndex < CIRCUIT_CNT; nIndex++) {
		arrayList.Add(g_lpszOutputCircuit[nIndex]);
	}
}

int CCircuitBasicInfo::GetCircuitInputNo(CString sText)
{
	int nIndex = GetIndexCircuitName(sText);
	if (nIndex < 0 || nIndex >= CIRCUIT_CNT) {
		return -2;
	}
	CString sTemp = g_lpszInputNo[nIndex];
	if (sTemp.GetLength() == 0) {
		return -1;
	}
	return _wtoi(sTemp.GetBuffer(0));
}

int CCircuitBasicInfo::GetCircuitOutputNo(CString sText)
{
	int nIndex = GetIndexCircuitName(sText);
	if (nIndex < 0 || nIndex >= CIRCUIT_CNT) {
		return -2;
	}
	CString sTemp = g_lpszOutputNo[nIndex];
	if (sTemp.GetLength() == 0) {
		return -1;
	}
	return _wtoi(sTemp.GetBuffer(0));
}

bool CCircuitBasicInfo::GetCircuitChild(CString sName, CStringArray & arrayList)
{
	arrayList.RemoveAll();
	pCircuitInfo pInfo = NULL;
	int nCount = m_listCircuit.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pInfo = m_listCircuit.GetAt(m_listCircuit.FindIndex(nIndex));
		if (pInfo && pInfo->sCircuitName.Compare(sName) == 0) {
			arrayList.Copy(pInfo->arrayChild);
			return true;
		}
	}

	return false;
}

int CCircuitBasicInfo::GetCircuitParentIndex(CString sName)
{
	pCircuitInfo pInfo = NULL;
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		if (sName.Compare(g_lpszCircuitParent[nIndex]) == 0) {
			return nIndex;
		}
	}

	return -1; 
}

bool CCircuitBasicInfo::IsDetector(CString sCircuitName)
{
	for (int i = �����_��ܰ�����; i <= �����_�ּ�������ELEV������; i++)
	{
		if (sCircuitName.Compare(g_lpszCircuitName[i]) == 0)
		{
			return true;
		}
	}

	return false;
}