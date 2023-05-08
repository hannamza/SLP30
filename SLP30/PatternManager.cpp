#include "stdafx.h"
#include "PatternManager.h"


CPatternManager::CPatternManager()
{
#ifndef ENGLISH_MODE
	m_alarmPatternDown.sPatternName = L"경종/비상방송/시각경보 지하";
#else
	m_alarmPatternDown.sPatternName = L"BELL/P.ADDRESS/STROBE BASEMENT";
#endif
}


CPatternManager::~CPatternManager()
{
	ReleasePattern();
}
// 1: m_alarmPatternDown
// 2~ m_alarmPattern
// 3~ m_visualalarmPattern
// 4~ m_alarmPatternUp
// 5~ m_doorPattern
// 6~ m_doorPatternStair
// 7~ m_damperPatternAll
// 8~ m_damperPatternStair
void CPatternManager::ReleasePattern()
{
	ReleaseList(&m_alarmPattern);
	ReleaseList(&m_visualalarmPattern);
	ReleaseList(&m_alarmPatternUp);
	ReleaseList(&m_doorPattern);
	ReleaseList(&m_doorPatternStair);
	ReleaseList(&m_damperPatternAll);
	ReleaseList(&m_damperPatternStair);

	m_alarmPatternDown.arrayBroad.RemoveAll();
	m_alarmPatternDown.arrayPattern.RemoveAll();
}

void CPatternManager::ReleaseList(CList<pPatternInfo, pPatternInfo> * pList)
{
	pPatternInfo pInfo;
	while (!pList->IsEmpty()) {
		pInfo = pList->RemoveHead();
		SAFE_DELETE(pInfo);
	}
}
// 1: m_alarmPatternDown
// 2~ m_alarmPattern
// 3~ m_visualalarmPattern
// 4~ m_alarmPatternUp
// 5~ m_doorPattern
// 6~ m_doorPatternStair
// 7~ m_damperPatternAll
// 8~ m_damperPatternStair
int CPatternManager::GetPatternCount()
{
	return GetFixedPatternCount() + m_alarmPattern.GetCount() + m_visualalarmPattern.GetCount()
		+ m_alarmPatternUp.GetCount() + m_doorPattern.GetCount() + m_doorPatternStair.GetCount()
		+ m_damperPatternAll.GetCount() + m_damperPatternStair.GetCount();
}

bool CPatternManager::CheckCompareName(CStringArray & listName, CString sName)
{
	CString sTemp;
	int nCount = listName.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		sTemp = listName.GetAt(nIndex);
		if (sTemp.CompareNoCase(sName) == 0) {
			return true;
		}
	}
	return false;
}
// 1: m_alarmPatternDown
// 2~ m_alarmPattern
// 3~ m_visualalarmPattern
// 4~ m_alarmPatternUp
// 5~ m_doorPattern
// 6~ m_doorPatternStair
// 7~ m_damperPatternAll
// 8~ m_damperPatternStair
int CPatternManager::GetFixedPatternCount()		// 패턴 번호를 매길 때 지하 패턴이 있으면 지상층 경종 패턴 번호가 2부터 시작이고 그렇지 않으면 1부터 시작
{
	int nCount = 0;
	// 1: m_alarmPattern
	if (m_alarmPatternDown.arrayPattern.GetCount() + m_alarmPatternDown.arrayBroad.GetCount() > 0) {
		++nCount;
	}

	return nCount;
}

void CPatternManager::AddPatternInfo(SYSTEM_INFO_* pInfo)
{
	if (_tcslen(pInfo->szCircuitName) <= 0 && pInfo->nSystem != 99) {
		return;
	}

	/*고정*/
	// 지하 전층 (경종, 시각경보, 비상방송)
	if ((CheckType(0, pInfo) || CheckType(5, pInfo)) && !CheckType(2, pInfo)) {
		AddAlarmPatternDown(pInfo);
	}

	/*변동*/
	// 전층 (경종)
	if (CheckType(0, pInfo)) {
		AddAlarmPattern(pInfo);
	}
	// 전층 (시각경보, 비상방송)
	if (CheckType(5, pInfo)) {
		AddVisualAlarmPattern(pInfo);
	}
	// 전층 (자동 폐쇄, 창문 폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
	if (CheckType(1, pInfo)) {
		AddDoorPattern(pInfo);
	}
	// 전층 급기댐퍼, 계단별 급기댐퍼
	if (CheckType(4, pInfo)) {
		AddDamperPattern(pInfo);
		AddDamperPatternStair(pInfo);
	}
	// 계단 층 지상 (경종, 시각경보, 비상방송)
	if ((CheckType(0, pInfo) || CheckType(5, pInfo)) && CheckType(2, pInfo)) {
		AddAlarmPatternUp(pInfo);
	}
	// 계단별 전층 (자동폐쇄, 창문 폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
	if (CheckType(1, pInfo)) {
		AddDoorPatternStair(pInfo);
	}
}

int CPatternManager::CheckType(int nType, SYSTEM_INFO_* pInfo)
{
	CString sTemp = pInfo->szCircuitName;
	switch (nType) {
	case 0: // 경종, 시각경보, 비상방송
		if (sTemp.Compare(g_lpszCircuitName[설비명_소화전]) == 0 || sTemp.Compare(g_lpszCircuitName[설비명_발신기]) == 0) {
			return true;
		}
		break;
	case 1: // 자동 폐쇄, 창문 폐쇄, 옥상개폐, 로비폰, 자동문, 비상등
		if (sTemp.Compare(g_lpszCircuitName[설비명_자동_폐쇄]) == 0 || sTemp.Compare(g_lpszCircuitName[설비명_창문_폐쇄]) == 0 ||
			sTemp.Compare(g_lpszCircuitName[설비명_옥상문]) == 0) {
			return true;
		}
		break;
	case 2: // 지상
		if (pInfo->nFloor >= 0) {
			return true;
		}
		break;
	case 3: // 패턴 체크
		if (sTemp.Compare(g_lpszCircuitName[설비명_배연창]) == 0) {
			return true;
		}
		break;	
	case 4: // 급기댐퍼
		if (sTemp.Compare(g_lpszCircuitName[설비명_전실_급기댐퍼]) == 0) {
			return true;
		}
		break;
	case 5: // 시각경보
#ifndef ENGLISH_MODE
		if (sTemp.Compare(g_lpszCircuitName[설비명_시각경보]) == 0 || sTemp.Compare(L"비상방송") == 0) {
			return true;
		}
#else
		if (sTemp.Compare(g_lpszCircuitName[설비명_시각경보]) == 0 || sTemp.Compare(L"P.Address") == 0) {
			return true;
		}
#endif
		break;
	default:
		break;
	}
	return false;
}

// 1: m_alarmPatternDown
// 2~ m_alarmPattern
// 3~ m_visualalarmPattern
// 4~ m_alarmPatternUp
// 5~ m_doorPattern
// 6~ m_doorPatternStair
// 7~ m_damperPatternAll
// 8~ m_damperPatternStair
pPatternInfo CPatternManager::GetPatternInfo(int nPatternIndex)
{
	if (nPatternIndex < 0) {
		return NULL;
	}
	if (nPatternIndex == 0 && ChangeFixedIndex() == 0) {
		return &m_alarmPatternDown;
	}

	int nCount = GetFixedPatternCount();
	// 2~ m_alarmPattern
	if (nCount + m_alarmPattern.GetCount() > nPatternIndex) {
		return m_alarmPattern.GetAt(m_alarmPattern.FindIndex(nPatternIndex - nCount));
	}
	nCount += m_alarmPattern.GetCount();
	// 3~ m_visualalarmPattern
	if (nCount + m_visualalarmPattern.GetCount() > nPatternIndex) {
		return m_visualalarmPattern.GetAt(m_visualalarmPattern.FindIndex(nPatternIndex - nCount));
	}
	nCount += m_visualalarmPattern.GetCount();
	// 4~ m_alarmPatternUp
	if (nCount + m_alarmPatternUp.GetCount() > nPatternIndex) {
		return m_alarmPatternUp.GetAt(m_alarmPatternUp.FindIndex(nPatternIndex - nCount));
	}
	nCount += m_alarmPatternUp.GetCount();
	// 5~ m_doorPattern
	if (nCount + m_doorPattern.GetCount() > nPatternIndex) {
		return m_doorPattern.GetAt(m_doorPattern.FindIndex(nPatternIndex - nCount));
	}
	nCount += m_doorPattern.GetCount();
	// 6~ m_doorPatternStair
	if (nCount + m_doorPatternStair.GetCount() > nPatternIndex) {
		return m_doorPatternStair.GetAt(m_doorPatternStair.FindIndex(nPatternIndex - nCount));
	}
	nCount += m_doorPatternStair.GetCount();
	// 7~ m_damperPatternAll
	if (nCount + m_damperPatternAll.GetCount() > nPatternIndex) {
		return m_damperPatternAll.GetAt(m_damperPatternAll.FindIndex(nPatternIndex - nCount));
	}
	nCount += m_damperPatternAll.GetCount();
	// 8~ m_damperPatternStair
	if (nCount + m_damperPatternStair.GetCount() > nPatternIndex) {
		return m_damperPatternStair.GetAt(m_damperPatternStair.FindIndex(nPatternIndex - nCount));
	}

	return NULL;
}

// 전층(경종)
void CPatternManager::AddAlarmPattern(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, 생성_동_전층, L"경종");
#else
	sPatternName = MakePatternName(pInfo, 생성_동_전층, L"BELL");
#endif
	CheckAddPatternSystem(pInfo, 생성_동_전층, m_alarmPattern, sPatternName);
}
// 전층(시각경보, 비상방송)
void CPatternManager::AddVisualAlarmPattern(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, 생성_동_전층, L"시각경보/비상방송");
#else
	sPatternName = MakePatternName(pInfo, 생성_동_전층, L"STROBE/P.ADDRESS");
#endif
	CheckAddPatternSystem(pInfo, 생성_동_전층, m_visualalarmPattern, sPatternName);
}

// 계단별 전층(경종, 시각경보, 비상방송)
void CPatternManager::AddAlarmPatternUp(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, 생성_동_계단_층, L"경종/방송/시각");
#else
	sPatternName = MakePatternName(pInfo, 생성_동_계단_층, L"BELL/P.ADDRESS/STROBE");
#endif
	CheckAddPatternSystem(pInfo, 생성_동_계단_층, m_alarmPatternUp, sPatternName);
}

// 지하 전층(경종, 시각경보, 비상방송)
void CPatternManager::AddAlarmPatternDown(SYSTEM_INFO_* pInfo)
{
	CString sTemp;
#ifndef ENGLISH_MODE
	m_alarmPatternDown.sPatternName = L"경종/비상방송 지하";
#else
	m_alarmPatternDown.sPatternName = L"BELL/P.ADDRESS BASEMENT";
#endif
	bool bBroad = (pInfo->nSystem == 99) ? true : false;
	if (bBroad) {
		sTemp.Format(L"%d", pInfo->nNo);
		m_alarmPatternDown.arrayBroad.Add(sTemp);
	}
	else {
		sTemp.Format(L"%d", pInfo->nNo);
		m_alarmPatternDown.arrayPattern.Add(sTemp);
	}
}

// 전층(자동 폐쇄, 창문 폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
void CPatternManager::AddDoorPattern(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, 생성_동_전층, L"폐쇄/개폐");
#else
	sPatternName = MakePatternName(pInfo, 생성_동_전층, L"CLOSE/OPEN");
#endif
	CheckAddPatternSystem(pInfo, 생성_동_전층, m_doorPattern, sPatternName);
}

// 계단별 지상 전층(자동 폐쇄, 창문 폐쇄, 옥상개폐, 로비폰, 자동문, 비상등)
void CPatternManager::AddDoorPatternStair(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, 생성_동_계단별, L"폐쇄/개폐");
#else
	sPatternName = MakePatternName(pInfo, 생성_동_계단별, L"CLOSE/OPEN");
#endif
	CheckAddPatternSystem(pInfo, 생성_동_계단별, m_doorPatternStair, sPatternName);
}

// 전층 급기댐퍼
void CPatternManager::AddDamperPattern(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, 생성_동_전층, L"급기댐퍼");
#else
	sPatternName = MakePatternName(pInfo, 생성_동_전층, L"SSMD");
#endif
	CheckAddPatternSystem(pInfo, 생성_동_전층, m_damperPatternAll, sPatternName);
}

// 계단별 급기댐퍼
void CPatternManager::AddDamperPatternStair(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, 생성_동_계단별, L"급기댐퍼");
#else
	sPatternName = MakePatternName(pInfo, 생성_동_계단별, L"SSMD");
#endif
	CheckAddPatternSystem(pInfo, 생성_동_계단별, m_damperPatternStair, sPatternName);
}

CString CPatternManager::MakePatternName(SYSTEM_INFO_* pInfo, int nType, CString sName)
{
	CString strRooftop;
	strRooftop = ROOFTOP_NAME;

	CString sPatternName = sName;
	int nStair = 0;
	if (pInfo->nStair == 0) {
		nStair = 1;
	}
	else {
		nStair = pInfo->nStair;
	}
	switch (nType) {
	case 생성_동_전층: // 패턴 동 전층
#ifndef ENGLISH_MODE
		if (wcslen(pInfo->szBlock) > 0) {
			sPatternName.Format(L"%s %s동 전층", sName, pInfo->szBlock);
		}
		else {
			sPatternName.Format(L"%s 전층", sName);
		}
#else
		if (wcslen(pInfo->szBlock) > 0) {
			sPatternName.Format(L"%s %sB.BLCK ALL FLOOR", sName, pInfo->szBlock);
		}
		else {
			sPatternName.Format(L"%s ALL FLOOR", sName);
		}
#endif
		break;
	case 생성_동_계단별: // 패턴 동 계단
#ifndef ENGLISH_MODE
		if (wcslen(pInfo->szBlock) > 0) {
			if (pInfo->bRooftop)		// 옥탑층
			{	
				sPatternName.Format(L"%s %s동 %s", sName, pInfo->szBlock, strRooftop);
			}
			else
			{
				sPatternName.Format(L"%s %s동 %d계단", sName, pInfo->szBlock, nStair);
			}	
		}
		else {
			if (pInfo->bRooftop)		// 옥탑층
			{
				sPatternName.Format(L"%s %s", sName, strRooftop);
			}
			else
			{
				sPatternName.Format(L"%s %d계단", sName, nStair);
			}
		}
#else
		if (wcslen(pInfo->szBlock) > 0) {
			if (pInfo->bRooftop)		// 옥탑층
			{
				sPatternName.Format(L"%s %sB.BLCK %s", sName, pInfo->szBlock, strRooftop);
			}
			else
			{
				sPatternName.Format(L"%s %sB.BLCK %dLINE", sName, pInfo->szBlock, nStair);
			}
		}
		else {
			if (pInfo->bRooftop)		// 옥탑층
			{
				sPatternName.Format(L"%s %s", sName, strRooftop);
			}
			else
			{
				sPatternName.Format(L"%s %dLINE", sName, nStair);
			}
		}
#endif
		break;
	case 생성_동_계단_층: // 패턴 동 계단, 층
#ifndef ENGLISH_MODE
		if (wcslen(pInfo->szBlock) == 0) {
			if (CCircuitBasicInfo::Instance()->m_nStair > 1) {
				if (pInfo->bRooftop)	// 옥탑층
				{
					sPatternName.Format(L"%s %d계단 %s", sName, pInfo->nStair, strRooftop);
				}
				else
				{
					sPatternName.Format(L"%s %d계단 %s%dF", sName, pInfo->nStair, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
				}
			}
			else {
				sPatternName.Format(L"%s %s%dF", sName, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
			}
		}
		else if (wcslen(pInfo->szBlock) > 0) {
			if (CCircuitBasicInfo::Instance()->m_nStair > 1) {
				if (pInfo->bRooftop)	// 옥탑층
				{
					sPatternName.Format(L"%s %s동 %d계단 %s", sName, pInfo->szBlock, pInfo->nStair, strRooftop);
				}
				else
				{
					sPatternName.Format(L"%s %s동 %d계단 %s%dF", sName, pInfo->szBlock, pInfo->nStair, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
				}	
			}
			else {
				if (pInfo->bRooftop)	// 옥탑층
				{
					sPatternName.Format(L"%s %s동 %s", sName, pInfo->szBlock, strRooftop);
				}
				else
				{
					sPatternName.Format(L"%s %s동 %s%dF", sName, pInfo->szBlock, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
				}
			}
		}
		else {
			if (CCircuitBasicInfo::Instance()->m_nStair > 1) {
				sPatternName.Format(L"%s %d계단", sName, pInfo->nStair);
			}
			else {
				sPatternName.Format(L"%s", sName);
			}
		}
#else
		if (wcslen(pInfo->szBlock) == 0) {
			if (CCircuitBasicInfo::Instance()->m_nStair > 1) {
				if (pInfo->bRooftop)	// 옥탑층
				{
					sPatternName.Format(L"%s %dLINE %s", sName, pInfo->nStair, strRooftop);
				}
				else
				{
					sPatternName.Format(L"%s %dLINE %s%dF", sName, pInfo->nStair, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
				}
			}
			else {
				sPatternName.Format(L"%s %s%dF", sName, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
			}
		}
		else if (wcslen(pInfo->szBlock) > 0) {
			if (CCircuitBasicInfo::Instance()->m_nStair > 1) {
				if (pInfo->bRooftop)	// 옥탑층
				{
					sPatternName.Format(L"%s %sB.BLCK %dLINE %s", sName, pInfo->szBlock, pInfo->nStair, strRooftop);
				}
				else
				{
					sPatternName.Format(L"%s %sB.BLCK %dLINE %s%dF", sName, pInfo->szBlock, pInfo->nStair, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
				}
			}
			else {
				if (pInfo->bRooftop)	// 옥탑층
				{
					sPatternName.Format(L"%s %sB.BLCK %s", sName, pInfo->szBlock, strRooftop);
				}
				else
				{
					sPatternName.Format(L"%s %sB.BLCK %s%dF", sName, pInfo->szBlock, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
				}
			}
		}
		else {
			if (CCircuitBasicInfo::Instance()->m_nStair > 1) {
				sPatternName.Format(L"%s %dLINE", sName, pInfo->nStair);
			}
			else {
				sPatternName.Format(L"%s", sName);
			}
		}
#endif
		break;
	default:
		break;
	}
	return sPatternName;
}

void CPatternManager::CheckAddPatternSystem(SYSTEM_INFO_* pInfo, int nType, CList<pPatternInfo, pPatternInfo> & pattern, CString sPatternName)
{
	bool bBroad = (pInfo->nSystem == 99) ? true : false;

	CString sTemp;
	POSITION pos = pattern.GetHeadPosition();
	pPatternInfo pPattern;
	bool bCheck = false;
	while (pos) {
		pPattern = pattern.GetNext(pos);
		bCheck = false;
		switch (nType) {
		case 생성_동_전층: // 동별 전층
			if (pPattern->sBlock.Compare(pInfo->szBlock) == 0) {
				bCheck = true;
			}
			break;
		case 생성_동_계단별: // 계단별 전층
			if (pPattern->sBlock.Compare(pInfo->szBlock) == 0 && pPattern->nStair == pInfo->nStair) {
				bCheck = true;
			}
			break;
		case 생성_동_계단_층: // 동, 계단, 층별
			if (pPattern->sBlock.Compare(pInfo->szBlock) == 0 && pPattern->nStair == pInfo->nStair && pPattern->nFloor == pInfo->nFloor) {
				bCheck = true;
			}
			break;
		default: return;
		}
		if (bCheck) {
			sTemp.Format(L"%d", pInfo->nNo);
			if (bBroad) {
				pPattern->arrayBroad.Add(sTemp);
			}
			else {
				pPattern->arrayPattern.Add(sTemp);
			}
			return;
		}
	}
	pPattern = new patternInfo;
	pPattern->nFloor = pInfo->nFloor;
	pPattern->nStair = pInfo->nStair;
	pPattern->sBlock = pInfo->szBlock;
	pPattern->bBroad = false;
	pPattern->sPatternName = sPatternName;
	sTemp.Format(L"%d", pInfo->nNo);
	if (bBroad) {
		pPattern->arrayBroad.Add(sTemp);
	}
	else {
		pPattern->arrayPattern.Add(sTemp);
	}

	pattern.AddTail(pPattern); // 패턴 추가
}

// 1: m_alarmPatternDown
// 2~ m_alarmPattern
// 3~ m_visualalarmPattern
// 4~ m_alarmPatternUp
// 5~ m_doorPattern
// 6~ m_doorPatternStair
// 7~ m_damperPatternAll
// 8~ m_damperPatternStair

void CPatternManager::CheckAddPatternSystem(CString sValue, CStringArray & list)
{
	if (sValue == L"-1") {
		return;
	}
	CString sTemp;
	for (int nIndex = 0; nIndex < list.GetCount(); nIndex++) {
		sTemp = list.GetAt(nIndex);
		if (sTemp == sValue) {
			return;
		}
	}
	list.Add(sValue);
}

// 실명, 층, 계단
bool CPatternManager::GetSystem(CString sBlock, int nStair, int nFloor, CString sRoom, CString sCircuitName, CStringArray & system)
{
	if (sRoom.GetLength() == 0) {
		return false;
	}
	bool bFind = false;
	CString sTemp;
	SYSTEM_INFO_* pInfo;
	POSITION pos = CSaveManager::Instance()->m_listSystem.GetHeadPosition();
	while (pos) {
		pInfo = CSaveManager::Instance()->m_listSystem.GetNext(pos);
		if (!pInfo) {
			continue;
		}
		if (sBlock.Compare(pInfo->szBlock) == 0 && pInfo->nStair == nStair && pInfo->nFloor == nFloor
			&& sCircuitName.Compare(pInfo->szCircuitName) == 0 && sRoom.Compare(pInfo->szRoomName) == 0) {
			bFind = true;
			sTemp.Format(L"%d", pInfo->nNo);
			CheckAddPatternSystem(sTemp, system);
		}
	}
	return bFind;
}

// -99 : 전계단, 전층  |  nType: (0: 해당층, 1:직상층 포함, 2: 지하층, 3: 지하층+1층)
bool CPatternManager::GetSystem(CString sBlock, int nStair, int nFloor, int nType, CStringArray &  sCircuitName, CStringArray & system)
{
	bool bFind;
	CString sName;
	SYSTEM_INFO_* pInfo;
	POSITION pos = CSaveManager::Instance()->m_listSystem.GetHeadPosition();
	while (pos) {
		pInfo = CSaveManager::Instance()->m_listSystem.GetNext(pos);
		if (!pInfo) {
			continue;
		}
		bFind = false;
		// 설비명 비교
		for (int nIndex = 0; nIndex < sCircuitName.GetCount(); nIndex++) {
			sName = sCircuitName.GetAt(nIndex);
			if (sName.CompareNoCase(pInfo->szCircuitName) == 0) {
				bFind = true;
				break;
			}
		}
		if (!bFind) { // 찾는 설비가 아니면
			continue;
		}
		// 동 비교
		if (sBlock != L"-99" && sBlock.Compare(pInfo->szBlock) != 0 && nType != 2) {
			continue;
		}
		if (nStair != -99 && nStair != pInfo->nStair) {
			continue;
		}
		if (nFloor != -99) {
			if (nType == 0 && nFloor != pInfo->nFloor) {
				continue;
			}
			else if (nType == 1 && nFloor != pInfo->nFloor && nFloor + 1 != pInfo->nFloor) { 
				continue;
			}
			else if (nType == 2 && pInfo->nFloor >= 0) {
				continue;
			}
			else if (nType == 3 && pInfo->nFloor > 1) {
				continue;
			}
		}

		sName.Format(L"%d", pInfo->nNo);
		CheckAddPatternSystem(sName, system);
	}

	return true;
}

void CPatternManager::GetPattern(SYSTEM_INFO_* pInfo, CUIntArray & pattern, CUIntArray & system)
{
	CString sTemp;
	CStringArray sPattern, sSystem;
	GetPattern(pInfo, sPattern, sSystem);
	for (int nIndex = 0; nIndex < sPattern.GetCount(); nIndex++) {
		sTemp = sPattern.GetAt(nIndex);
		sTemp.Replace(L"P", L"");
		pattern.Add(_wtoi(sTemp.GetBuffer(0)));
	}
	for (int nIndex = 0; nIndex < sSystem.GetCount(); nIndex++) {
		sTemp = sSystem.GetAt(nIndex);
		system.Add(_wtoi(sTemp.GetBuffer(0)));
	}
}
// 1: m_alarmPatternDown
// 2~ m_alarmPattern
// 3~ m_visualalarmPattern
// 4~ m_alarmPatternUp
// 5~ m_doorPattern
// 6~ m_doorPatternStair
// 7~ m_damperPatternAll
// 8~ m_damperPatternStair

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 연동 목록 얻기 //////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 경종, 시각경보, 비상방송 패턴
void CPatternManager::GetAlarmBroadPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern)
{
	int nPattern, nTemp;
	int nFloor = pInfo->nFloor;
	CString sTemp;
	pPatternInfo pPattern, pAPattern = NULL, pBPattern = NULL;

	if (CSaveManager::Instance()->m_makeData.data[연동_경종_전층] == 1) { // 동별 경종/비상방송 전층/시각경보 연동
		POSITION pos = m_alarmPattern.GetHeadPosition();
		nPattern = 1;
		while (pos) {
			pAPattern = m_alarmPattern.GetNext(pos);
			if (pAPattern->sBlock.Compare(pInfo->szBlock) == 0) {
				break;
			}
			++nPattern;
			pAPattern = NULL;
		}
		if (pAPattern && (pAPattern->arrayBroad.GetCount() > 0 || pAPattern->arrayPattern.GetCount() > 0)) {
			nTemp = nPattern + GetPatternIndex(패턴_알람);
			if (nTemp > 0) {
				sTemp.Format(L"P%d", nTemp);
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
		pos = m_visualalarmPattern.GetHeadPosition();
		nPattern = 1;
		while (pos) {
			pBPattern = m_visualalarmPattern.GetNext(pos);
			if (pBPattern->sBlock.Compare(pInfo->szBlock) == 0) {
				break;
			}
			++nPattern;
			pBPattern = NULL;
		}
		if (pBPattern && (pBPattern->arrayBroad.GetCount() > 0 || pBPattern->arrayPattern.GetCount() > 0)) {
			nPattern = GetPatternIndex(패턴_시각경보) + nPattern;
			if (nPattern > 0) {
				sTemp.Format(L"P%d", nPattern);
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
		return;
	}
	else if (nFloor < -1) { // 지하 2층 이하
		if (m_alarmPatternDown.arrayBroad.GetCount() > 0 || m_alarmPatternDown.arrayPattern.GetCount() > 0) {
			nPattern = GetVeriatyIndex(패턴_알람지하);
			if (nPattern > 0) {
				sTemp.Format(L"P%d", GetVeriatyIndex(패턴_알람지하));
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
		return;
	}
	else if (nFloor == -1 || nFloor == 1) { // 지하 1층, 1층
		if (m_alarmPatternDown.arrayBroad.GetCount() > 0 || m_alarmPatternDown.arrayPattern.GetCount() > 0) {
			nPattern = GetVeriatyIndex(패턴_알람지하);
			if (nPattern > 0) {
				sTemp.Format(L"P%d", GetVeriatyIndex(패턴_알람지하)); // 지하층 전체 포함
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
		if (nFloor == -1) { // 지하 1층 일 경우 0으로 지정하여 1층 포함 시킴, 1층일 경우 1층, 2층 포함
			nFloor = 0;
		}
	}

	if (CSaveManager::Instance()->m_makeData.data[연동_경종_계단별직상층] == 1) { // 계단별 직상층
		for (int nIndex = 0; nIndex < m_alarmPatternUp.GetCount(); nIndex++) {
			pPattern = m_alarmPatternUp.GetAt(m_alarmPatternUp.FindIndex(nIndex));
			if ((pPattern->sBlock.Compare(pInfo->szBlock) == 0 && pPattern->nStair == pInfo->nStair && pPattern->nFloor == nFloor)
				|| (pPattern->sBlock.Compare(pInfo->szBlock) == 0 && pPattern->nStair == pInfo->nStair && pPattern->nFloor == nFloor + 1) ) {
				sTemp.Format(L"P%d", GetPatternIndex(패턴_알람층별) + nIndex + 1);
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
	}
	if (CSaveManager::Instance()->m_makeData.data[연동_경종_동별직상층] == 1) { // 동별 직상층
		for (int nIndex = 0; nIndex < m_alarmPatternUp.GetCount(); nIndex++) {
			pPattern = m_alarmPatternUp.GetAt(m_alarmPatternUp.FindIndex(nIndex));
			if (pPattern->sBlock.Compare(pInfo->szBlock) == 0 && (pPattern->nFloor == nFloor || pPattern->nFloor == nFloor + 1)) {
				sTemp.Format(L"P%d", GetPatternIndex(패턴_알람층별) + nIndex + 1);
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
	}
}

// 싸이렌 | 교차회로 밸브 값 / 아날로그 감지기 밸브 값
void CPatternManager::GetValve(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	// 밸브 , 박준구 차장 요청으로 SVP 감지기B -> SVP T/S 출력 타입 변경
	if (!GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[설비명_P_T_확인_SVP], system)) {
		return;
	}
	// 싸이렌, 박준구 차장 요청으로 SVP 감지기A -> SVP P/S 출력 타입 변경
	if (CSaveManager::ie()->m_makeData.data[연동_싸이렌_작동연동] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[설비명_P_S_확인_SVP], system);
	}
}

// 싸이렌 동작확인 시 연동(svp ps, av p/s)
void CPatternManager::GetSiren(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	// 싸이렌, 박준구 차장 요청으로 SVP 감지기A -> SVP T/S 출력 타입 변경
	if (CSaveManager::ie()->m_makeData.data[연동_싸이렌_동작확인] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[설비명_P_S_확인_SVP], system);
	}
}

// 자동 폐쇄, 창문 폐쇄, 옥상문, 비상등, 로비폰, 자동문 패턴
void CPatternManager::GetDoorPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern)
{
	int nPattern;
	CString sTemp;
	pPatternInfo pPattern;
	if (CSaveManager::ie()->m_makeData.data[연동_문_전층] == 1) {
		int nCount = m_doorPattern.GetCount();
		nPattern = GetPatternIndex(패턴_문전층);
		sTemp = L"";
		for (int nIndex = 0; nIndex < nCount; nIndex++) {
			pPattern = m_doorPattern.GetAt(m_doorPattern.FindIndex(nIndex));
			if (pPattern->sBlock.Compare(pInfo->szBlock) == 0) {
				sTemp.Format(L"P%d", nPattern + nIndex + 1);
				CheckAddPatternSystem(sTemp, pattern);
				break;
			}
		}
		/*if (nPattern > 0) {
			sTemp.Format(L"P%d", nPattern + 1);
			CheckAddPatternSystem(sTemp, pattern);
		}*/
	}
	else if (CSaveManager::ie()->m_makeData.data[연동_문_계단별전층] == 1) {
		int nCount = m_doorPatternStair.GetCount();
		sTemp = L"";
		nPattern = GetPatternIndex(패턴_문계단별전층);
		for (int nIndex = 0; nIndex < nCount; nIndex++) {
			pPattern = m_doorPatternStair.GetAt(m_doorPatternStair.FindIndex(nIndex));
			if ((pPattern->sBlock.Compare(pInfo->szBlock) == 0 && pInfo->nStair == pPattern->nStair)
				|| (pInfo->nFloor < 0 && pPattern->nFloor < 0)) {
				sTemp.Format(L"P%d", nPattern + nIndex + 1);
				CheckAddPatternSystem(sTemp, pattern);
				break;
			}
		}
	}
}

// 배연창
void CPatternManager::GetSmokeScreen(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	int nPattern;
	CString sTemp;
	CStringArray listCircuit;
	listCircuit.Add(g_lpszCircuitName[설비명_배연창]);

	if (CSaveManager::ie()->m_makeData.data[연동_배연창_건물해당층] == 1) {
		GetSystem(pInfo->szBlock, -99, pInfo->nFloor, 0, listCircuit, system); // 건물 해당층
	}
	else if (CSaveManager::ie()->m_makeData.data[연동_배연창_계단별해당층] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, 0, listCircuit, system); // 계단별 해당층
	}
}

// 전실 급기댐퍼
void CPatternManager::GetAirDamper(SYSTEM_INFO_* pInfo, CStringArray & pattern)
{
	int nPattern;
	CString sTemp;
	if (CSaveManager::ie()->m_makeData.data[연동_급기_건물전층] == 1) {
		pPatternInfo pPattern;
		int nCount = m_damperPatternAll.GetCount();
		nPattern = GetPatternIndex(패턴_댐퍼전층);
		for (int nIndex = 0; nIndex < nCount; nIndex++) {
			pPattern = m_damperPatternAll.GetAt(m_damperPatternAll.FindIndex(nIndex));
			if (pPattern->sBlock.Compare(pInfo->szBlock) == 0) {
				sTemp.Format(L"P%d", nPattern + nIndex + 1);
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
	}
	else if (CSaveManager::ie()->m_makeData.data[연동_급기_계단별전층] == 1) {
		pPatternInfo pPattern;
		int nCount = m_damperPatternStair.GetCount();
		nPattern = GetPatternIndex(패턴_댐퍼계단별);
		for (int nIndex = 0; nIndex < nCount; nIndex++) {
			pPattern = m_damperPatternStair.GetAt(m_damperPatternStair.FindIndex(nIndex));
			//if ((pPattern->sBlock.Compare(pInfo->szBlock) == 0)	|| (pInfo->nFloor < 0 && pPattern->nFloor < 0)) {
			if ((pPattern->sBlock.Compare(pInfo->szBlock) == 0 && pInfo->nStair == pPattern->nStair)
				|| (pInfo->nFloor < 0 && pPattern->nFloor < 0)) {
				sTemp.Format(L"P%d", nPattern + nIndex + 1);
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
	}
}

// 전실배기댐퍼
void CPatternManager::GetExDamper(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	CStringArray listCircuit;
	if (CSaveManager::ie()->m_makeData.data[연동_배기_건물해당층] == 1) {
		listCircuit.Add(g_lpszCircuitName[설비명_전실_배기댐퍼]);
		GetSystem(pInfo->szBlock, -99, pInfo->nFloor, 0, listCircuit, system); // 건물 해당층

	}
	else if (CSaveManager::ie()->m_makeData.data[연동_배기_계단별해당층] == 1) {
		listCircuit.Add(g_lpszCircuitName[설비명_전실_배기댐퍼]);
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, 0, listCircuit, system); // 계단별 해당층
		//if (pInfo->nFloor < 0) { // 해당층이 지하면
		//	GetSystem(pInfo->szBlock, -99, -99, 2, listCircuit, system); // 지하 전층
		//}
	}
	if (pInfo->nFloor < 0) { // 해당층이 지하면
		GetSystem(pInfo->szBlock, -99, -99, 2, listCircuit, system); // 지하 전층
	}
}
// 급기팬
void CPatternManager::GetAirFan(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	CStringArray listCircuit;
	listCircuit.Add(g_lpszCircuitName[설비명_급기팬]);
	if (CSaveManager::ie()->m_makeData.data[연동_급기_건물전층] == 1) {
		GetSystem(pInfo->szBlock, -99, -99, 99, listCircuit, system); // 전층
	}
	else if (CSaveManager::ie()->m_makeData.data[연동_급기_계단별전층] == 1)
	{
		GetSystem(pInfo->szBlock, pInfo->nStair, -99, 99, listCircuit, system); // 계단별 전층
		if (pInfo->nFloor < 0) { // 해당층이 지하면
			GetSystem(pInfo->szBlock, -99, -99, 2, listCircuit, system); // 지하 전층
		}
	}
}

// 배기팬
void CPatternManager::CPatternManager::GetExFan(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	CStringArray listCircuit;
	listCircuit.Add(g_lpszCircuitName[설비명_배기팬]);
	if (CSaveManager::ie()->m_makeData.data[연동_배기_건물해당층] == 1) {
		GetSystem(pInfo->szBlock, -99, -99, 99, listCircuit, system); // 전층
	}
	else if (CSaveManager::ie()->m_makeData.data[연동_배기_계단별해당층] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, -99, 99, listCircuit, system); // 계단별 전층
		if (pInfo->nFloor < 0) { // 해당층이 지하면
			GetSystem(pInfo->szBlock, -99, -99, 2, listCircuit, system); // 지하 전층
		}
	}
}

// 셔터 1차/2차 폐쇄 (연감지기)
void CPatternManager::GetSutterSmoke(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	if (CSaveManager::ie()->m_makeData.data[연동_셔터_광전1_정온2] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[설비명_셔터_1차_폐쇄], system); // 계단, 층, 실
	}
	else if (CSaveManager::ie()->m_makeData.data[연동_셔터_1차만] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[설비명_셔터_1차_폐쇄], system); // 계단, 층, 실
	}
	else if (CSaveManager::ie()->m_makeData.data[연동_셔터_2차만] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[설비명_셔터_2차_폐쇄], system); // 계단, 층, 실
	}
}

// 셔터 1차/2차 폐쇄 (열감지기)
void CPatternManager::GetSutterFire(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	CStringArray listCircuit;
	if (CSaveManager::ie()->m_makeData.data[연동_셔터_광전1_정온2] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[설비명_셔터_2차_폐쇄], system); // 계단, 층, 실
	}
	else if (CSaveManager::ie()->m_makeData.data[연동_셔터_1차만] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[설비명_셔터_1차_폐쇄], system); // 계단, 층, 실
	}
	else if (CSaveManager::ie()->m_makeData.data[연동_셔터_2차만] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[설비명_셔터_2차_폐쇄], system); // 계단, 층, 실
	}
}

// 기동램프(소화전기동) 전층
void CPatternManager::GetLamp(CStringArray & system)
{
	CStringArray listCircuit;
	listCircuit.Add(g_lpszCircuitName[설비명_P_L_점등]);
	GetSystem(L"-99", -99, -99, 99, listCircuit, system); // 전층
}
void CPatternManager::GetLamp(CUIntArray & system)
{
	CString sTemp;
	CStringArray sSystem;
	GetLamp(sSystem);
	for (int nIndex = 0; nIndex < sSystem.GetCount(); nIndex++) {
		sTemp = sSystem.GetAt(nIndex);
		system.Add(_wtoi(sTemp.GetBuffer(0)));
	}
}

// 설비별 연동 목록
void CPatternManager::GetPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern, CStringArray & system)
{
	int nCircuitIndex = CCircuitBasicInfo::Instance()->GetIndexCircuitName(pInfo->szCircuitName);
	if (nCircuitIndex == -1) {
		return;
	}
	switch (nCircuitIndex) {
	case 설비명_아날로그_광전_SVP:
	case 설비명_아날로그_정온_SVP:
	case 설비명_감지기_A_SVP:
	case 설비명_감지기_B_SVP:
	//case 설비명_주소형차동SVP감지기A:
	//case 설비명_주소형차동SVP감지기B:
		GetValve(pInfo, system);
		break;
	/*case 설비명_계단감지기:
	case 설비명_ELEV감지기:*/
	case 설비명_감지기:
		GetValve(pInfo, system);
		break;
	case 설비명_광전_셔터:
	case 설비명_아날로그_광전_셔터:
	case 설비명_주소형_광전_셔터:
		GetValve(pInfo, system);
		GetSutterSmoke(pInfo, system);
		break;
	case 설비명_광온_셔터:
	case 설비명_아날로그_정온_셔터:
	case 설비명_주소형_정온_셔터:
		GetValve(pInfo, system);
		GetSutterFire(pInfo, system);
		break;
	case 설비명_P_S_확인_SVP:
	case 설비명_알람밸브_P_S:
		GetSiren(pInfo, system);
		break;
	case 설비명_전실_급기댐퍼:
		GetAirFan(pInfo, system);
		break;
	case 설비명_전실_배기댐퍼:
		GetExFan(pInfo, system);
		break;
	default:
		break;
	}
	// 입력 타입이 화재면 
	for (int nIndex = 설비명_감지기; nIndex < 설비명_아날로그_광전_SVP; nIndex++) { // 화재 타입인 것들 
		if (CCircuitBasicInfo::Instance()->GetCircuitName(nCircuitIndex) == g_lpszCircuitName[설비명_감지기_계단]
			|| CCircuitBasicInfo::Instance()->GetCircuitName(nCircuitIndex) == g_lpszCircuitName[설비명_감지기_ELEV]) {
			continue;
		}
		if (CCircuitBasicInfo::Instance()->GetCircuitInput(nCircuitIndex) == g_lpszInputType[nIndex]) {
			// 1. 경종/비상방송/시각경보
			GetAlarmBroadPattern(pInfo, pattern);
			// 2. 자동 폐쇄/창문 폐쇄/옥상문/로비폰/자동문/비상등
			GetDoorPattern(pInfo, pattern);
			// 3. 배연창
			GetSmokeScreen(pInfo, system);
			// 4. 전실급기댐퍼
			GetAirDamper(pInfo, pattern);
			// 5. 전실배기댐퍼
			GetExDamper(pInfo, system);
			break;
		}
	}
}

void CPatternManager::GetPatternList(CString sPattern, CStringArray & system, CStringArray & broad)
{
	sPattern.Replace(L"P", L"");
	int nPattern = _wtoi(sPattern.GetBuffer(0));
	GetPatternList(nPattern, system, broad);
}
// 1: m_alarmPatternDown
// 2~ m_alarmPattern
// 3~ m_visualalarmPattern
// 4~ m_alarmPatternUp
// 5~ m_doorPattern
// 6~ m_doorPatternStair
// 7~ m_damperPatternAll
// 8~ m_damperPatternStair

int CPatternManager::GetPatternIndex(int nIndex)	// 모든 패턴의 순번이 정해져 있고 건물 구성에 따라 인덱스가 변동할 수 있는데 이를 계산
{
	if (nIndex <= FIXED_PATTERN) {
		return GetVeriatyIndex(nIndex);
	}
	int nPattern = GetFixedPatternCount();
	switch (nIndex) {
	case 패턴_알람지하:
		if (m_alarmPatternDown.arrayBroad.GetCount() > 0 || m_alarmPatternDown.arrayPattern.GetCount() > 0) {
			return nPattern;
		}
		return 0;
	case 패턴_알람:
		if (m_alarmPattern.GetCount() > 0) {
			return nPattern;
		}
		return 0;
	case 패턴_시각경보:
		if (m_visualalarmPattern.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount();
		}
		return 0;
	case 패턴_알람층별:
		if (m_alarmPatternUp.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount() + m_visualalarmPattern.GetCount();
		}
		return 0;
	case 패턴_문전층:
		if (m_doorPattern.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount() + m_visualalarmPattern.GetCount() + m_alarmPatternUp.GetCount();
		}
		return 0;
	case 패턴_문계단별전층:
		if (m_doorPatternStair.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount() + m_visualalarmPattern.GetCount() + m_alarmPatternUp.GetCount() + m_doorPattern.GetCount();
		}
		return 0;
	case 패턴_댐퍼전층:
		if (m_damperPatternAll.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount() + m_visualalarmPattern.GetCount() + m_alarmPatternUp.GetCount() + m_doorPattern.GetCount() + m_doorPatternStair.GetCount();
		}
		return 0;
	case 패턴_댐퍼계단별:
		if (m_damperPatternStair.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount() + m_visualalarmPattern.GetCount() + m_alarmPatternUp.GetCount() + m_doorPattern.GetCount() + m_doorPatternStair.GetCount() + m_damperPatternAll.GetCount();
		}
		return 0;
	default:
		break;
	}
	return 0;
}

// 고정 인덱스를 변동 인덱스로 변환
// 1: m_alarmPatternDown
int CPatternManager::GetVeriatyIndex(int nIndex)
{
	int nPatternCount;
	int nCount = 0;
	if (m_alarmPatternDown.arrayPattern.GetCount() > 0 && m_alarmPatternDown.arrayBroad.GetCount() > 0) {
		++nCount;
	}
	return nCount;
}

// 변동 인덱스를 고정 인덱스로 변환
int CPatternManager::ChangeFixedIndex()
{
	int nCount = 0;
	// m_alarmPatternDown
	if (m_alarmPatternDown.arrayPattern.GetCount() + m_alarmPatternDown.arrayBroad.GetCount() > 0) {
		return 0;
	}

	return -1;
}

// 패턴의 설비 목록(1부터 시작)
void CPatternManager::GetPatternList(int nPatternIndex, CStringArray & system, CStringArray & broad)
{
	// 고정
	if (nPatternIndex == 1 && ChangeFixedIndex() == 0) {
		system.Copy(m_alarmPatternDown.arrayPattern); broad.Copy(m_alarmPatternDown.arrayBroad);
		return;
	}

	// 변동
	int nCount = GetFixedPatternCount();
	pPatternInfo pInfo;
	// 2~ m_alarmPattern
	if (nCount + m_alarmPattern.GetCount() > nPatternIndex) {
		pInfo = m_alarmPattern.GetAt(m_alarmPattern.FindIndex(nPatternIndex - nCount));
		system.Copy(pInfo->arrayPattern);
		broad.Copy(pInfo->arrayBroad);
		return;
	}
	// 3~ m_visualalarmPattern
	if (nCount + m_visualalarmPattern.GetCount() > nPatternIndex) {
		pInfo = m_visualalarmPattern.GetAt(m_visualalarmPattern.FindIndex(nPatternIndex - nCount));
		system.Copy(pInfo->arrayPattern);
		broad.Copy(pInfo->arrayBroad);
		return;
	}
	// 4~ m_alarmPatternUp
	if (nCount + m_alarmPatternUp.GetCount() > nPatternIndex) {
		pInfo = m_alarmPatternUp.GetAt(m_alarmPatternUp.FindIndex(nPatternIndex - nCount));
		system.Copy(pInfo->arrayPattern);
		broad.Copy(pInfo->arrayBroad);
		return;
	}
	// 5~ m_doorPattern
	if (nCount + m_doorPattern.GetCount() > nPatternIndex) {
		pInfo = m_doorPattern.GetAt(m_doorPattern.FindIndex(nPatternIndex - nCount));
		system.Copy(pInfo->arrayPattern);
		broad.Copy(pInfo->arrayBroad);
		return;
	}
	// 6~ m_doorPatternStair
	if (nCount + m_doorPatternStair.GetCount() > nPatternIndex) {
		pInfo = m_doorPatternStair.GetAt(m_doorPatternStair.FindIndex(nPatternIndex - nCount));
		system.Copy(pInfo->arrayPattern);
		broad.Copy(pInfo->arrayBroad);
		return;
	}
	// 7~ m_damperPatternAll
	if (nCount + m_damperPatternAll.GetCount() > nPatternIndex) {
		pInfo = m_damperPatternAll.GetAt(m_damperPatternAll.FindIndex(nPatternIndex - nCount));
		system.Copy(pInfo->arrayPattern);
		broad.Copy(pInfo->arrayBroad);
		return;
	}
	// 8~ m_damperPatternStair
	if (nCount + m_damperPatternStair.GetCount() > nPatternIndex) {
		pInfo = m_damperPatternStair.GetAt(m_damperPatternStair.FindIndex(nPatternIndex - nCount));
		system.Copy(pInfo->arrayPattern);
		broad.Copy(pInfo->arrayBroad);
		return;
	}
}
