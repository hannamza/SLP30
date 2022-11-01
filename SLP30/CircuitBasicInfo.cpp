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
		case 0: // 소화전
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_소화전]);
			break;
		case 1: // 발신기
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_발신기]);
			break;
		case 2: // 알람밸브
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_AVP_S]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_AVT_S]);
			break;
		case 3: // 프리액션밸브
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_SVP감지기A]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_SVP감지기B]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_SVPP_S]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_SVPT_S]);
			break;
		case 4: // 시각경보
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_시각경보]);
			break;
		case 5: // 전실제연급기
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_전실급기댐퍼]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_전실급기수동]);
			break;
		case 6: // 전실제연배기
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_전실배기댐퍼]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_전실배기수동]);
			break;
		case 7: // 상가제연
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_제연수동기동]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_SMD확인]);
			//pInfo->arrayChild.Add(g_lpszCircuitName[설비명_급기팬]);
			//pInfo->arrayChild.Add(g_lpszCircuitName[설비명_배기팬]);
			break;
		case 8: // 방화셔터
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_셔터연감지기]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_셔텨열감지기]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_셔터1차폐쇄]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_셔터2차폐쇄]);
			break;
		case 9: // 배연창
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_배연창]);
			break;
		case 10: // 방화문
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_방화문]);
			break;
		case 11: // 창폐장치
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_창문폐쇄]);
			break;
		case 12: // 자폐장치
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_자동폐쇄]);
			break;
		case 13: // 소화전기동확인
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_P_L점등]);
			break;
		case 14: // 옥상개폐확인
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_옥상문]);
			break;
		case 15: // 저수위감시
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_저수위F_S]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_저수위T_S]);
			break;
		case 16: // 제연급기팬
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_급기팬]);
			break;
		case 17: // 제연배기팬
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_배기팬]);
			break;
		case 18: // 감지기
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_계단감지기]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_ELEV감지기]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_감지기]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_AN연기]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_AN정온]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_주소형연기]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_주소형정온]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_주소형차동]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_AN연기계단감지기]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_AN연기ELEV감지기]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_주소형연기계단감지기]);
			pInfo->arrayChild.Add(g_lpszCircuitName[설비명_주소형연기ELEV감지기]);
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
	for (int i = 설비명_계단감지기; i <= 설비명_주소형연기ELEV감지기; i++)
	{
		if (sCircuitName.Compare(g_lpszCircuitName[i]) == 0)
		{
			return true;
		}
	}

	return false;
}