#include "stdafx.h"
#include "PatternManager.h"


CPatternManager::CPatternManager()
{
}


CPatternManager::~CPatternManager()
{
	ReleasePattern();
}

void CPatternManager::ReleasePattern()
{
	ReleaseList(&m_listPattern);
	ReleaseList(&m_listPattern1);
	ReleaseList(&m_listPattern2);
	ReleaseList(&m_listPattern3);
	m_listBasePattern.RemoveAll();
	m_listUpPattern.RemoveAll();
	//ReleaseList(&m_listBasePattern);
	//ReleaseList(&m_listUpPattern);
}

void CPatternManager::ReleaseList(CList<pPatternInfo, pPatternInfo> * pList)
{
	pPatternInfo pInfo;
	while (!pList->IsEmpty()) {
		pInfo = pList->RemoveHead();
		SAFE_DELETE(pInfo);
	}
}

pPatternInfo CPatternManager::GetPatternAlarm(int nSystem, int nStair, int nFloor, int nType)
{
	CList<pPatternInfo, pPatternInfo>* list;
	switch (nType) {
	case pattentype_normal: list = &m_listPattern; break;
	case pattentype_base: list = &m_listBasePattern; break;
	case pattentype_up: list = &m_listUpPattern; break;

	case pattentype_exceptalarm: list = &m_listPattern1; break;
	case pattentype_exceptdoor: list = &m_listPattern2; break;
	case pattentype_exceptalarmdoor: list = &m_listPattern3; break;

	case pattentype_alarmnormal: return &m_alarmPattern;
	case pattentype_alarmup: return &m_alarmPatternUp;
	case pattentype_alarmdown: return &m_alarmPatternDown;

	case pattentype_doornormal: return &m_doorPattern;
	case pattentype_doorup: return &m_doorPatternUp;
	case pattentype_doordown: return &m_doorPatternDown;
	default: return NULL;
	}
	pPatternInfo pInfo;
	int nCount = list->GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pInfo = list->GetAt(list->FindIndex(nIndex));
		if (pInfo->nSystem == nSystem && pInfo->nFloor == nFloor && pInfo->nStair == nStair) {
			return pInfo;
		}
	}
	return NULL;
}

int CPatternManager::GetPatternIndex(int nType, int nSystem, int nStair, int nFloor)
{
	CUIntArray system, broad;
	return GetPatternIndex(nType, nSystem, nStair, nFloor, system, broad);
}

int CPatternManager::GetPatternIndex(int nType, int nSystem, int nStair, int nFloor, CUIntArray & system, CUIntArray & broad)
{
	int n1Count = m_listPattern.GetCount();		// 1. 층별 패턴
	int n2Count = m_listBasePattern.GetCount();	// 2. 지하패턴
	int n3Count = m_listUpPattern.GetCount();	// 3. 지상패턴
	int n4Count = m_listPattern1.GetCount();	// 4. 패턴(경종, 시각경보, 비상방송 제외)
	int n5Count = m_listPattern2.GetCount();	// 5. 패턴(자동폐쇄, 창문폐쇄, 옥상개폐 제외)
	int n6Count = m_listPattern3.GetCount();	// 6. 패턴(경종, 시각경보, 비상방송, 자동폐쇄, 창문폐쇄, 옥상개폐 제외)

	pPatternInfo pInfo = NULL;

	switch (nType) {
	case pattentype_normal: // 층, 계단별 전층
		for (int nIndex = 0; nIndex < n1Count; nIndex++) {
			pInfo = m_listPattern.GetAt(m_listPattern.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return nIndex + 1;
			}
		}
		break;
	case pattentype_base: // 층, 계단별 지하
		for (int nIndex = 0; nIndex < n2Count; nIndex++) {
			pInfo = m_listBasePattern.GetAt(m_listBasePattern.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return n1Count + nIndex + 1;
			}
		}
		break;
	case pattentype_up: // 층, 계단별 지상
		for (int nIndex = 0; nIndex < n3Count; nIndex++) {
			pInfo = m_listUpPattern.GetAt(m_listUpPattern.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return n1Count + n2Count + nIndex + 1;
			}
		}
		break;

	case pattentype_exceptalarm: // 경종, 시각경보, 비상방송 제외 층, 계단별
		for (int nIndex = 0; nIndex < n4Count; nIndex++) {
			pInfo = m_listPattern1.GetAt(m_listPattern1.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return n1Count + n2Count + n3Count + nIndex + 1;
			}
		}
		break;
	case pattentype_exceptdoor: // 자동폐쇄, 창문폐쇄, 옥상개폐 제외 층, 계단별
		for (int nIndex = 0; nIndex < n5Count; nIndex++) {
			pInfo = m_listPattern2.GetAt(m_listPattern2.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return n1Count + n2Count + n3Count + n4Count + nIndex + 1;
			}
		}
		break;
	case pattentype_exceptalarmdoor: // 경종, 시각경보, 비상방송, 자동폐쇄, 창문폐쇄, 옥상개폐 제외 층, 계단별
		for (int nIndex = 0; nIndex < n6Count; nIndex++) {
			pInfo = m_listPattern3.GetAt(m_listPattern3.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return n1Count + n2Count + n3Count + n4Count + n5Count + nIndex + 1;
			}
		}
		break;

	case pattentype_alarmnormal:	// 포함 층, 계단별 전층
		system.Copy(m_alarmPattern.arrayPattern2);
		broad.Copy(m_alarmPattern.arrayPattern1);
		return n1Count + n2Count + n3Count + n4Count + n5Count + n6Count + 1;
	case pattentype_alarmup:		// 포함 층, 계단별 지상
		system.Copy(m_alarmPatternUp.arrayPattern2);
		broad.Copy(m_alarmPatternUp.arrayPattern1);
		return n1Count + n2Count + n3Count + n4Count + n5Count + n6Count + 2;
	case pattentype_alarmdown:		// 포함 층, 계단별 지하
		system.Copy(m_alarmPatternDown.arrayPattern2);
		broad.Copy(m_alarmPatternDown.arrayPattern1);
		return n1Count + n2Count + n3Count + n4Count + n5Count + n6Count + 3;

	case pattentype_doornormal:		// 포함 층, 계단별 전층
		system.Copy(m_doorPattern.arrayPattern2);
		broad.Copy(m_doorPattern.arrayPattern1);
		return n1Count + n2Count + n3Count + n4Count + n5Count + n6Count + 4;
	case pattentype_doorup:			// 층, 계단별 지상
		system.Copy(m_doorPatternUp.arrayPattern2);
		broad.Copy(m_doorPatternUp.arrayPattern1);
		return n1Count + n2Count + n3Count + n4Count + n5Count + n6Count + 5;
	case pattentype_doordown:		// 층, 계단별 지하
		system.Copy(m_doorPatternDown.arrayPattern2);
		broad.Copy(m_doorPatternDown.arrayPattern1);
		return n1Count + n2Count + n3Count + n4Count + n5Count + n6Count + 6;

	default: return -2;
	}
	return -1;
}

int CPatternManager::GetPatternList(int nType, int nSystem, int nStair, int nFloor, CUIntArray & system, CUIntArray & broad)
{
	return GetPatternIndex(nType, nSystem, nStair, nFloor, system, broad);
}

int CPatternManager::GetPattern(int nStair, int nFloor, int nType, bool bAll, CUIntArray & pattern)
{
	CString sTemp;
	int nPatternIndex;
	CUIntArray system, broad;
	if (bAll) {
		for (int nIndex = 0; nIndex < CCircuitBasicInfo::Instance()->m_nStair; nIndex++) {
			nPatternIndex = GetPatternIndex(nType, 0, nIndex, nFloor, system, broad);
			if (nPatternIndex > 0) {
				//sTemp.Format(L"P%d", nPatternIndex);
				pattern.Add(nPatternIndex);
			}
			nPatternIndex = GetPatternIndex(nType, 1, nIndex, nFloor, system, broad);
			if (nPatternIndex > 0) {
				//sTemp.Format(L"P%d", nPatternIndex);
				pattern.Add(nPatternIndex);
			}
			nPatternIndex = GetPatternIndex(nType, 99, nIndex, nFloor, system, broad);
			if (nPatternIndex > 0) {
				//sTemp.Format(L"A%d", nPatternIndex);
				pattern.Add(1000000 + nPatternIndex);
			}
		}
	}
	else {
		nPatternIndex = GetPatternIndex(nType, 0, nStair, nFloor, system, broad);
		if (nPatternIndex > 0) {
			//sTemp.Format(L"P%d", nPatternIndex);
			pattern.Add(nPatternIndex);
		}
		nPatternIndex = GetPatternIndex(nType, 1, nStair, nFloor, system, broad);
		if (nPatternIndex > 0) {
			//sTemp.Format(L"P%d", nPatternIndex);
			pattern.Add(nPatternIndex);
		}
		nPatternIndex = GetPatternIndex(nType, 99, nStair, nFloor, system, broad);
		if (nPatternIndex > 0) {
			//sTemp.Format(L"A%d", nPatternIndex);
			pattern.Add(1000000 + nPatternIndex);
		}
	}
	return 0;
}

int CPatternManager::CommonPattern(int nStair, int nFloor, CUIntArray & pattern)
{
	if (CSaveManager::Instance()->m_makeData.data[0] == 1) { // 경보 전층 연동
		GetPattern(nStair, nFloor, pattentype_alarmnormal, true, pattern);
	}

	if (CSaveManager::Instance()->m_makeData.data[0] == 1) { // 전층 연동
		if (nFloor < -1) {
			GetPattern(nStair, nFloor, pattentype_base, true, pattern);
		}
		else if(nFloor < 1) {
			GetPattern(nStair, nFloor, pattentype_base, true, pattern);
			GetPattern(nStair, 1, pattentype_normal, true, pattern);
			GetPattern(nStair, 2, pattentype_normal, true, pattern);
		}
		else if (nFloor == 1) {
			GetPattern(nStair, nFloor, pattentype_base, true, pattern);
			GetPattern(nStair, 1, pattentype_normal, true, pattern);
			GetPattern(nStair, 2, pattentype_normal, true, pattern);
		}
		else {
			GetPattern(nStair, nFloor, pattentype_normal, true, pattern);
			GetPattern(nStair, nFloor + 1, pattentype_normal, true, pattern);
		}
	}
	else if (CSaveManager::Instance()->m_makeData.data[1] == 1) { // 계단별 직상층
		if (nFloor < -1) {
			GetPattern(nStair, nFloor, pattentype_base, true, pattern);
		}
		else if (nFloor < 1) {
			GetPattern(nStair, nFloor, pattentype_base, true, pattern);
			GetPattern(nStair, 1, pattentype_normal, false, pattern);
		}
		else if (nFloor == 1) {
			GetPattern(nStair, nFloor, pattentype_base, true, pattern);
			GetPattern(nStair, 1, pattentype_normal, true, pattern);
			GetPattern(nStair, 2, pattentype_normal, true, pattern);
		}
		else {
			GetPattern(nStair, nFloor, pattentype_normal, false, pattern);
			GetPattern(nStair, nFloor + 1, pattentype_normal, false, pattern);
		}
	}
	else if (CSaveManager::Instance()->m_makeData.data[2] == 1) { // 동별 직상층
		if (nFloor < -1) {
			GetPattern(nStair, nFloor, pattentype_base, true, pattern);
		}
		else if (nFloor < 1) {
			GetPattern(nStair, nFloor, pattentype_base, true, pattern);
			GetPattern(nStair, 1, pattentype_normal, true, pattern);
		}
		else if (nFloor == 1) {
			GetPattern(nStair, nFloor, pattentype_base, true, pattern);
			GetPattern(nStair, 1, pattentype_normal, true, pattern);
			GetPattern(nStair, 2, pattentype_normal, true, pattern);
		}
		else {
			GetPattern(nStair, nFloor, pattentype_normal, true, pattern);
			GetPattern(nStair, nFloor + 1, pattentype_normal, true, pattern);
		}
	}

	if (CSaveManager::Instance()->m_makeData.data[7] == 1) { // 자동폐쇄, 창문폐쇄, 옥상개폐
		GetPattern(nStair, nFloor, pattentype_doornormal, true, pattern);
	}
	return 0;
}

int CPatternManager::GetSystemList(int nStair, int nFloor, CString sSystem, CUIntArray & system)
{
	CString sSystemName;
	int nCircuitIndex;
	SYSTEM_INFO_ * pInfo = NULL;
	int nCount = CSaveManager::Instance()->m_listSystem.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pInfo = CSaveManager::Instance()->m_listSystem.GetAt(CSaveManager::Instance()->m_listSystem.FindIndex(nIndex));
		if (pInfo->nFloor != nFloor) {
			continue;
		}
		if (CSaveManager::Instance()->m_makeData.data[1] == 1 && pInfo->nStair != nStair) {
			continue;
		}
		nCircuitIndex = CCircuitBasicInfo::Instance()->GetIndexCircuitName(pInfo->szCircuitName);
		sSystemName = CCircuitBasicInfo::Instance()->GetCircuitOutputName(nCircuitIndex);
		if (sSystemName == sSystem) {
			system.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		}
	}
	return 0;
}

int CPatternManager::GetPatternList(int nStair, int nFloor, CString sPattern, CUIntArray & pattern, CUIntArray & system)
{
	int nCircuitIndex = CCircuitBasicInfo::Instance()->GetIndexCircuitName(sPattern);
	if (nCircuitIndex == -1) {
		return -1;
	}
	switch (nCircuitIndex) {
	// 일반 경우
	case 설비명_소화전:			// 0
	case 설비명_발신기:			// 1
	case 설비명_감지기:			// 4
	case 설비명_AN연기:			// 5
	case 설비명_AN정온:			// 6
	case 설비명_전실급기수동:	// 21
	case 설비명_전실배기수동:	// 23
	case 설비명_가스감지기A:	// 30
	case 설비명_가스감지기B:	// 31
	case 설비명_가스방출확인:	// 32
	case 설비명_가스수동기동:	// 33
	case 설비명_제연수동기동:	// 37
		CommonPattern(nStair, nFloor, pattern); // 일반
		break;
	// 일반 + 밸브, 싸이렌
	case 설비명_SVP감지기A:		// 10
	case 설비명_SVP감지기B:		// 11
	case 설비명_AVP_S:			// 14
		CommonPattern(nStair, nFloor, pattern);
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_밸브], system);
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_싸이렌], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_밸브], system);
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_싸이렌], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[출력타입_밸브], system);
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_밸브], system);
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_싸이렌], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[출력타입_밸브], system);
			GetSystemList(nStair, 1, g_lpszOutputCircuit[출력타입_싸이렌], system); 
			GetSystemList(nStair, 2, g_lpszOutputCircuit[출력타입_밸브], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[출력타입_싸이렌], system);
		}

		break;
	// 일반 + 싸이렌 
	case 설비명_SVPP_S:			// 12
		CommonPattern(nStair, nFloor, pattern);
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_싸이렌], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_싸이렌], system);
			}
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_싸이렌], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[출력타입_싸이렌], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[출력타입_싸이렌], system);
		}
		break;
	// 일반 + 셔터 1차 폐쇄
	case 설비명_주소형연기:		// 7
		CommonPattern(nStair, nFloor, pattern);
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_1차폐쇄], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_1차폐쇄], system);
			}
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_1차폐쇄], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[출력타입_1차폐쇄], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[출력타입_1차폐쇄], system);
		}
		break;
	// 일반 + 셔터 2차 폐쇄
	case 설비명_주소형정온:		// 8
	case 설비명_주소형차동:		// 9
		CommonPattern(nStair, nFloor, pattern);
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_2차폐쇄], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_2차폐쇄], system);
			}
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_2차폐쇄], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[출력타입_2차폐쇄], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[출력타입_2차폐쇄], system);
		}
		break;
	// 급기FAN
	case 설비명_전실급기댐퍼:	// 20
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_전실제연1], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_전실제연1], system);
			}
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_전실제연1], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[출력타입_전실제연1], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[출력타입_전실제연1], system);
		}
		break;
	// 배기FAN
	case 설비명_전실배기댐퍼:	// 22
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_배기FAN], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_배기FAN], system);
			}
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[출력타입_배기FAN], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[출력타입_배기FAN], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[출력타입_배기FAN], system);
		}
		break;
	case 설비명_배연창:			// 26
		break;

	case 설비명_계단감지기:		// 2
	case 설비명_ELEV감지기:		// 3
	case 설비명_SVPT_S:			// 13
	case 설비명_AVT_S:			// 15
	case 설비명_셔터연감지기:	// 16
	case 설비명_셔텨열감지기:	// 17
	case 설비명_셔터1차폐쇄:	// 18
	case 설비명_셔터2차폐쇄:	// 19
	case 설비명_자동폐쇄:		// 24
	case 설비명_창문폐쇄:		// 25
	case 설비명_방화문:			// 27
	case 설비명_P_L점등:		// 28
	case 설비명_유도등점등:		// 29
	case 설비명_저수위F_S:		// 34
	case 설비명_PUMPP_S:		// 35
	case 설비명_PUMPT_S:		// 36
	case 설비명_SMD확인:		// 38
	case 설비명_MVD확인:		// 39
	case 설비명_급기FAN:		// 40
	case 설비명_배기FAN:		// 41
	case 설비명_저수위T_S:		// 42
	case 설비명_옥상문:			// 43
		break;
	default: return -2;
	}
	return 0;
}

void CPatternManager::AddPatternList(CList<pPatternInfo, pPatternInfo>* pList)
{
	pPatternInfo pInfo = NULL;
	for (int nIndex = 0; nIndex < pList->GetCount(); nIndex++) {
		pInfo = pList->GetAt(pList->FindIndex(nIndex));
		m_patternTotal.AddTail(pInfo);
	}
}

void CPatternManager::MakePatternList()
{
	m_patternTotal.RemoveAll();

	AddPatternList(&m_listPattern);
	AddPatternList(&m_listBasePattern);
	AddPatternList(&m_listUpPattern);
	AddPatternList(&m_listPattern1);
	AddPatternList(&m_listPattern2);
	AddPatternList(&m_listPattern3);

	m_patternTotal.AddTail(&m_alarmPattern);
	m_patternTotal.AddTail(&m_alarmPatternUp);
	m_patternTotal.AddTail(&m_alarmPatternDown);
	m_patternTotal.AddTail(&m_doorPattern);
	m_patternTotal.AddTail(&m_doorPatternUp);
	m_patternTotal.AddTail(&m_doorPatternDown);
}

int CPatternManager::GetPatternCount()
{
	return m_patternTotal.GetCount();
}

pPatternInfo CPatternManager::GetPattern(int nIndex)
{
	if (nIndex < 0 || m_patternTotal.GetCount() <= nIndex - 1) {
		return NULL;
	}
	return m_patternTotal.GetAt(m_patternTotal.FindIndex(nIndex - 1));
}

void CPatternManager::AddPatternInfo(SYSTEM_INFO_* pInfo)
{
	bool bFind = false;
	// 층별
	POSITION pos = m_listPattern.GetHeadPosition();
	pPatternInfo pPattern;
	while (pos) {
		pPattern  = m_listPattern.GetNext(pos);
		if (pPattern->nFloor == pInfo->nFloor && pPattern->nStair == pInfo->nStair && pPattern->nSystem == pInfo->nSystem) {
			pPattern->arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
			bFind = true;
			break;
		}
	}
	if (!bFind) {
		pPattern = new patternInfo;
		pPattern->nFloor = pInfo->nFloor;
		pPattern->nStair = pInfo->nStair;
		pPattern->nSystem = pInfo->nSystem;
		pPattern->arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);

		m_listPattern.AddTail(pPattern); // 층별 패턴
	}

	// 지하패턴
	if (pInfo->nFloor < 0) {
		m_listBasePattern.AddTail(pPattern); // 지하패턴
	}
	else {
		m_listUpPattern.AddTail(pPattern); // 지상 패턴
	}

	// 전층(경종, 시각경보)
	if (wcscmp(pInfo->szCircuitName, g_lpszOutputCircuit[출력타입_지구경종]) == 0)
	{
		m_alarmPattern.arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		// 지상 전층(경종, 시각경보, 비상방송)
		if (pPattern->nFloor < 0) {
			m_alarmPatternDown.arrayPattern1.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		}
		// 지하 전층(경종, 시각경보, 비상방송)
		else {
			m_alarmPatternUp.arrayPattern1.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		}
	}
	// 전층(자동폐쇄, 창문폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
	if (//wcscmp(pInfo->szCircuitName, g_lpszOutputCircuit[출력타입_자동폐쇄]) == 0
		//|| wcscmp(pInfo->szCircuitName, g_lpszOutputCircuit[출력타입_창문폐쇄]) == 0
		wcscmp(pInfo->szCircuitName, g_lpszOutputCircuit[출력타입_옥상문]) == 0) {
		m_doorPattern.arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		// 지상 전층(자동폐쇄, 창문폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
		if (pPattern->nFloor < 0) {
			m_doorPatternDown.arrayPattern1.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		}
		// 지하 전층(자동폐쇄, 창문폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
		else {
			m_doorPatternUp.arrayPattern1.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		}
	}
}
// 전층 패턴(경종, 시각경보, 비상방송 제외)
void CPatternManager::AddPatternInfoExceptAlarm(SYSTEM_INFO_* pInfo)
{
	bool bFind = false;
	// 층별
	POSITION pos = m_listPattern1.GetHeadPosition();
	pPatternInfo pPattern;
	while (pos) {
		pPattern = m_listPattern1.GetNext(pos);
		if (pPattern->nFloor == pInfo->nFloor && pPattern->nStair == pInfo->nStair && pPattern->nSystem == pInfo->nSystem) {
			pPattern->arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
			bFind = true;
			break;
		}
	}
	if (!bFind) {
		pPattern = new patternInfo;
		pPattern->nFloor = pInfo->nFloor;
		pPattern->nStair = pInfo->nStair;
		pPattern->nSystem = pInfo->nSystem;
		pPattern->arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);

		m_listPattern1.AddTail(pPattern); // 층별 패턴
	}
}
// 전층 패턴(자동폐쇄, 창문폐쇄, 옥상개폐 제외)
void CPatternManager::AddPatternInfoExceptDoor(SYSTEM_INFO_* pInfo)
{
	bool bFind = false;
	// 층별
	POSITION pos = m_listPattern2.GetHeadPosition();
	pPatternInfo pPattern;
	while (pos) {
		pPattern = m_listPattern2.GetNext(pos);
		if (pPattern->nFloor == pInfo->nFloor && pPattern->nStair == pInfo->nStair && pPattern->nSystem == pInfo->nSystem) {
			pPattern->arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
			bFind = true;
			break;
		}
	}
	if (!bFind) {
		pPattern = new patternInfo;
		pPattern->nFloor = pInfo->nFloor;
		pPattern->nStair = pInfo->nStair;
		pPattern->nSystem = pInfo->nSystem;
		pPattern->arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);

		m_listPattern2.AddTail(pPattern); // 층별 패턴
	}
}
// 전층 패턴(경종, 시각경보, 비상방송, 자동폐쇄, 창문폐쇄, 옥상개폐 제외)
void CPatternManager::AddPatternInfoExceptAlarmDoor(SYSTEM_INFO_* pInfo)
{
	bool bFind = false;
	// 층별
	POSITION pos = m_listPattern3.GetHeadPosition();
	pPatternInfo pPattern;
	while (pos) {
		pPattern = m_listPattern3.GetNext(pos);
		if (pPattern->nFloor == pInfo->nFloor && pPattern->nStair == pInfo->nStair && pPattern->nSystem == pInfo->nSystem) {
			pPattern->arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
			bFind = true;
			break;
		}
	}
	if (!bFind) {
		pPattern = new patternInfo;
		pPattern->nFloor = pInfo->nFloor;
		pPattern->nStair = pInfo->nStair;
		pPattern->nSystem = pInfo->nSystem;
		pPattern->arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);

		m_listPattern3.AddTail(pPattern); // 층별 패턴
	}
}

void CPatternManager::AddPatternInfo(BC_INFO* pInfo, int nCircuitIndex)
{
	POSITION pos = m_listPattern.GetHeadPosition();
	pPatternInfo pPattern;
	while (pos) {
		pPattern = m_listPattern.GetNext(pos);
		if (pPattern->nFloor == pInfo->nFloor && pPattern->nStair == pInfo->nStair && pPattern->nSystem == 99) {
			pPattern->arrayPattern1.Add(nCircuitIndex);
			return;
		}
	}
	pPattern = new patternInfo;
	pPattern->nFloor = pInfo->nFloor;
	pPattern->nStair = pInfo->nStair;
	pPattern->nSystem = 99;
	pPattern->arrayPattern1.Add(nCircuitIndex);

	m_listPattern.AddTail(pPattern);

	// 전층(비상방송)
	m_alarmPattern.arrayPattern1.Add(nCircuitIndex);
	// 지상 전층(경종, 시각경보, 비상방송)
	if (pPattern->nFloor < 0) {
		m_alarmPatternDown.arrayPattern1.Add(nCircuitIndex);
	}
	// 지하 전층(경종, 시각경보, 비상방송)
	else {
		m_alarmPatternUp.arrayPattern1.Add(nCircuitIndex);
	}
}