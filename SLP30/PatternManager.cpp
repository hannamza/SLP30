#include "stdafx.h"
#include "PatternManager.h"


CPatternManager::CPatternManager()
{
#ifndef ENGLISH_MODE
	m_alarmPatternDown.sPatternName = L"����/�����/�ð��溸 ����";
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
int CPatternManager::GetFixedPatternCount()		// ���� ��ȣ�� �ű� �� ���� ������ ������ ������ ���� ���� ��ȣ�� 2���� �����̰� �׷��� ������ 1���� ����
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

	/*����*/
	// ���� ���� (����, �ð��溸, �����)
	if ((CheckType(0, pInfo) || CheckType(5, pInfo)) && !CheckType(2, pInfo)) {
		AddAlarmPatternDown(pInfo);
	}

	/*����*/
	// ���� (����)
	if (CheckType(0, pInfo)) {
		AddAlarmPattern(pInfo);
	}
	// ���� (�ð��溸, �����)
	if (CheckType(5, pInfo)) {
		AddVisualAlarmPattern(pInfo);
	}
	// ���� (�ڵ� ���, â�� ���, ������, �κ���, �ڵ���, ����)
	if (CheckType(1, pInfo)) {
		AddDoorPattern(pInfo);
	}
	// ���� �ޱ����, ��ܺ� �ޱ����
	if (CheckType(4, pInfo)) {
		AddDamperPattern(pInfo);
		AddDamperPatternStair(pInfo);
	}
	// ��� �� ���� (����, �ð��溸, �����)
	if ((CheckType(0, pInfo) || CheckType(5, pInfo)) && CheckType(2, pInfo)) {
		AddAlarmPatternUp(pInfo);
	}
	// ��ܺ� ���� (�ڵ����, â�� ���, ������, �κ���, �ڵ���, ����)
	if (CheckType(1, pInfo)) {
		AddDoorPatternStair(pInfo);
	}
}

int CPatternManager::CheckType(int nType, SYSTEM_INFO_* pInfo)
{
	CString sTemp = pInfo->szCircuitName;
	switch (nType) {
	case 0: // ����, �ð��溸, �����
		if (sTemp.Compare(g_lpszCircuitName[�����_��ȭ��]) == 0 || sTemp.Compare(g_lpszCircuitName[�����_�߽ű�]) == 0) {
			return true;
		}
		break;
	case 1: // �ڵ� ���, â�� ���, ������, �κ���, �ڵ���, ����
		if (sTemp.Compare(g_lpszCircuitName[�����_�ڵ�_���]) == 0 || sTemp.Compare(g_lpszCircuitName[�����_â��_���]) == 0 ||
			sTemp.Compare(g_lpszCircuitName[�����_����]) == 0) {
			return true;
		}
		break;
	case 2: // ����
		if (pInfo->nFloor >= 0) {
			return true;
		}
		break;
	case 3: // ���� üũ
		if (sTemp.Compare(g_lpszCircuitName[�����_�迬â]) == 0) {
			return true;
		}
		break;	
	case 4: // �ޱ����
		if (sTemp.Compare(g_lpszCircuitName[�����_����_�ޱ����]) == 0) {
			return true;
		}
		break;
	case 5: // �ð��溸
#ifndef ENGLISH_MODE
		if (sTemp.Compare(g_lpszCircuitName[�����_�ð��溸]) == 0 || sTemp.Compare(L"�����") == 0) {
			return true;
		}
#else
		if (sTemp.Compare(g_lpszCircuitName[�����_�ð��溸]) == 0 || sTemp.Compare(L"P.Address") == 0) {
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

// ����(����)
void CPatternManager::AddAlarmPattern(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, ����_��_����, L"����");
#else
	sPatternName = MakePatternName(pInfo, ����_��_����, L"BELL");
#endif
	CheckAddPatternSystem(pInfo, ����_��_����, m_alarmPattern, sPatternName);
}
// ����(�ð��溸, �����)
void CPatternManager::AddVisualAlarmPattern(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, ����_��_����, L"�ð��溸/�����");
#else
	sPatternName = MakePatternName(pInfo, ����_��_����, L"STROBE/P.ADDRESS");
#endif
	CheckAddPatternSystem(pInfo, ����_��_����, m_visualalarmPattern, sPatternName);
}

// ��ܺ� ����(����, �ð��溸, �����)
void CPatternManager::AddAlarmPatternUp(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, ����_��_���_��, L"����/���/�ð�");
#else
	sPatternName = MakePatternName(pInfo, ����_��_���_��, L"BELL/P.ADDRESS/STROBE");
#endif
	CheckAddPatternSystem(pInfo, ����_��_���_��, m_alarmPatternUp, sPatternName);
}

// ���� ����(����, �ð��溸, �����)
void CPatternManager::AddAlarmPatternDown(SYSTEM_INFO_* pInfo)
{
	CString sTemp;
#ifndef ENGLISH_MODE
	m_alarmPatternDown.sPatternName = L"����/����� ����";
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

// ����(�ڵ� ���, â�� ���, ������, �κ���, �ڵ���, ����)
void CPatternManager::AddDoorPattern(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, ����_��_����, L"���/����");
#else
	sPatternName = MakePatternName(pInfo, ����_��_����, L"CLOSE/OPEN");
#endif
	CheckAddPatternSystem(pInfo, ����_��_����, m_doorPattern, sPatternName);
}

// ��ܺ� ���� ����(�ڵ� ���, â�� ���, ������, �κ���, �ڵ���, ����)
void CPatternManager::AddDoorPatternStair(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, ����_��_��ܺ�, L"���/����");
#else
	sPatternName = MakePatternName(pInfo, ����_��_��ܺ�, L"CLOSE/OPEN");
#endif
	CheckAddPatternSystem(pInfo, ����_��_��ܺ�, m_doorPatternStair, sPatternName);
}

// ���� �ޱ����
void CPatternManager::AddDamperPattern(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, ����_��_����, L"�ޱ����");
#else
	sPatternName = MakePatternName(pInfo, ����_��_����, L"SSMD");
#endif
	CheckAddPatternSystem(pInfo, ����_��_����, m_damperPatternAll, sPatternName);
}

// ��ܺ� �ޱ����
void CPatternManager::AddDamperPatternStair(SYSTEM_INFO_* pInfo)
{
	CString sPatternName;
#ifndef ENGLISH_MODE
	sPatternName = MakePatternName(pInfo, ����_��_��ܺ�, L"�ޱ����");
#else
	sPatternName = MakePatternName(pInfo, ����_��_��ܺ�, L"SSMD");
#endif
	CheckAddPatternSystem(pInfo, ����_��_��ܺ�, m_damperPatternStair, sPatternName);
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
	case ����_��_����: // ���� �� ����
#ifndef ENGLISH_MODE
		if (wcslen(pInfo->szBlock) > 0) {
			sPatternName.Format(L"%s %s�� ����", sName, pInfo->szBlock);
		}
		else {
			sPatternName.Format(L"%s ����", sName);
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
	case ����_��_��ܺ�: // ���� �� ���
#ifndef ENGLISH_MODE
		if (wcslen(pInfo->szBlock) > 0) {
			if (pInfo->bRooftop)		// ��ž��
			{	
				sPatternName.Format(L"%s %s�� %s", sName, pInfo->szBlock, strRooftop);
			}
			else
			{
				sPatternName.Format(L"%s %s�� %d���", sName, pInfo->szBlock, nStair);
			}	
		}
		else {
			if (pInfo->bRooftop)		// ��ž��
			{
				sPatternName.Format(L"%s %s", sName, strRooftop);
			}
			else
			{
				sPatternName.Format(L"%s %d���", sName, nStair);
			}
		}
#else
		if (wcslen(pInfo->szBlock) > 0) {
			if (pInfo->bRooftop)		// ��ž��
			{
				sPatternName.Format(L"%s %sB.BLCK %s", sName, pInfo->szBlock, strRooftop);
			}
			else
			{
				sPatternName.Format(L"%s %sB.BLCK %dLINE", sName, pInfo->szBlock, nStair);
			}
		}
		else {
			if (pInfo->bRooftop)		// ��ž��
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
	case ����_��_���_��: // ���� �� ���, ��
#ifndef ENGLISH_MODE
		if (wcslen(pInfo->szBlock) == 0) {
			if (CCircuitBasicInfo::Instance()->m_nStair > 1) {
				if (pInfo->bRooftop)	// ��ž��
				{
					sPatternName.Format(L"%s %d��� %s", sName, pInfo->nStair, strRooftop);
				}
				else
				{
					sPatternName.Format(L"%s %d��� %s%dF", sName, pInfo->nStair, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
				}
			}
			else {
				sPatternName.Format(L"%s %s%dF", sName, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
			}
		}
		else if (wcslen(pInfo->szBlock) > 0) {
			if (CCircuitBasicInfo::Instance()->m_nStair > 1) {
				if (pInfo->bRooftop)	// ��ž��
				{
					sPatternName.Format(L"%s %s�� %d��� %s", sName, pInfo->szBlock, pInfo->nStair, strRooftop);
				}
				else
				{
					sPatternName.Format(L"%s %s�� %d��� %s%dF", sName, pInfo->szBlock, pInfo->nStair, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
				}	
			}
			else {
				if (pInfo->bRooftop)	// ��ž��
				{
					sPatternName.Format(L"%s %s�� %s", sName, pInfo->szBlock, strRooftop);
				}
				else
				{
					sPatternName.Format(L"%s %s�� %s%dF", sName, pInfo->szBlock, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
				}
			}
		}
		else {
			if (CCircuitBasicInfo::Instance()->m_nStair > 1) {
				sPatternName.Format(L"%s %d���", sName, pInfo->nStair);
			}
			else {
				sPatternName.Format(L"%s", sName);
			}
		}
#else
		if (wcslen(pInfo->szBlock) == 0) {
			if (CCircuitBasicInfo::Instance()->m_nStair > 1) {
				if (pInfo->bRooftop)	// ��ž��
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
				if (pInfo->bRooftop)	// ��ž��
				{
					sPatternName.Format(L"%s %sB.BLCK %dLINE %s", sName, pInfo->szBlock, pInfo->nStair, strRooftop);
				}
				else
				{
					sPatternName.Format(L"%s %sB.BLCK %dLINE %s%dF", sName, pInfo->szBlock, pInfo->nStair, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
				}
			}
			else {
				if (pInfo->bRooftop)	// ��ž��
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
		case ����_��_����: // ���� ����
			if (pPattern->sBlock.Compare(pInfo->szBlock) == 0) {
				bCheck = true;
			}
			break;
		case ����_��_��ܺ�: // ��ܺ� ����
			if (pPattern->sBlock.Compare(pInfo->szBlock) == 0 && pPattern->nStair == pInfo->nStair) {
				bCheck = true;
			}
			break;
		case ����_��_���_��: // ��, ���, ����
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

	pattern.AddTail(pPattern); // ���� �߰�
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

// �Ǹ�, ��, ���
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

// -99 : �����, ����  |  nType: (0: �ش���, 1:������ ����, 2: ������, 3: ������+1��)
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
		// ����� ��
		for (int nIndex = 0; nIndex < sCircuitName.GetCount(); nIndex++) {
			sName = sCircuitName.GetAt(nIndex);
			if (sName.CompareNoCase(pInfo->szCircuitName) == 0) {
				bFind = true;
				break;
			}
		}
		if (!bFind) { // ã�� ���� �ƴϸ�
			continue;
		}
		// �� ��
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
//// ���� ��� ��� //////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ����, �ð��溸, ����� ����
void CPatternManager::GetAlarmBroadPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern)
{
	int nPattern, nTemp;
	int nFloor = pInfo->nFloor;
	CString sTemp;
	pPatternInfo pPattern, pAPattern = NULL, pBPattern = NULL;

	if (CSaveManager::Instance()->m_makeData.data[����_����_����] == 1) { // ���� ����/����� ����/�ð��溸 ����
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
			nTemp = nPattern + GetPatternIndex(����_�˶�);
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
			nPattern = GetPatternIndex(����_�ð��溸) + nPattern;
			if (nPattern > 0) {
				sTemp.Format(L"P%d", nPattern);
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
		return;
	}
	else if (nFloor < -1) { // ���� 2�� ����
		if (m_alarmPatternDown.arrayBroad.GetCount() > 0 || m_alarmPatternDown.arrayPattern.GetCount() > 0) {
			nPattern = GetVeriatyIndex(����_�˶�����);
			if (nPattern > 0) {
				sTemp.Format(L"P%d", GetVeriatyIndex(����_�˶�����));
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
		return;
	}
	else if (nFloor == -1 || nFloor == 1) { // ���� 1��, 1��
		if (m_alarmPatternDown.arrayBroad.GetCount() > 0 || m_alarmPatternDown.arrayPattern.GetCount() > 0) {
			nPattern = GetVeriatyIndex(����_�˶�����);
			if (nPattern > 0) {
				sTemp.Format(L"P%d", GetVeriatyIndex(����_�˶�����)); // ������ ��ü ����
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
		if (nFloor == -1) { // ���� 1�� �� ��� 0���� �����Ͽ� 1�� ���� ��Ŵ, 1���� ��� 1��, 2�� ����
			nFloor = 0;
		}
	}

	if (CSaveManager::Instance()->m_makeData.data[����_����_��ܺ�������] == 1) { // ��ܺ� ������
		for (int nIndex = 0; nIndex < m_alarmPatternUp.GetCount(); nIndex++) {
			pPattern = m_alarmPatternUp.GetAt(m_alarmPatternUp.FindIndex(nIndex));
			if ((pPattern->sBlock.Compare(pInfo->szBlock) == 0 && pPattern->nStair == pInfo->nStair && pPattern->nFloor == nFloor)
				|| (pPattern->sBlock.Compare(pInfo->szBlock) == 0 && pPattern->nStair == pInfo->nStair && pPattern->nFloor == nFloor + 1) ) {
				sTemp.Format(L"P%d", GetPatternIndex(����_�˶�����) + nIndex + 1);
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
	}
	if (CSaveManager::Instance()->m_makeData.data[����_����_����������] == 1) { // ���� ������
		for (int nIndex = 0; nIndex < m_alarmPatternUp.GetCount(); nIndex++) {
			pPattern = m_alarmPatternUp.GetAt(m_alarmPatternUp.FindIndex(nIndex));
			if (pPattern->sBlock.Compare(pInfo->szBlock) == 0 && (pPattern->nFloor == nFloor || pPattern->nFloor == nFloor + 1)) {
				sTemp.Format(L"P%d", GetPatternIndex(����_�˶�����) + nIndex + 1);
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
	}
}

// ���̷� | ����ȸ�� ��� �� / �Ƴ��α� ������ ��� ��
void CPatternManager::GetValve(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	// ��� , ���ر� ���� ��û���� SVP ������B -> SVP T/S ��� Ÿ�� ����
	if (!GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[�����_P_T_Ȯ��_SVP], system)) {
		return;
	}
	// ���̷�, ���ر� ���� ��û���� SVP ������A -> SVP P/S ��� Ÿ�� ����
	if (CSaveManager::ie()->m_makeData.data[����_���̷�_�۵�����] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[�����_P_S_Ȯ��_SVP], system);
	}
}

// ���̷� ����Ȯ�� �� ����(svp ps, av p/s)
void CPatternManager::GetSiren(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	// ���̷�, ���ر� ���� ��û���� SVP ������A -> SVP T/S ��� Ÿ�� ����
	if (CSaveManager::ie()->m_makeData.data[����_���̷�_����Ȯ��] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[�����_P_S_Ȯ��_SVP], system);
	}
}

// �ڵ� ���, â�� ���, ����, ����, �κ���, �ڵ��� ����
void CPatternManager::GetDoorPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern)
{
	int nPattern;
	CString sTemp;
	pPatternInfo pPattern;
	if (CSaveManager::ie()->m_makeData.data[����_��_����] == 1) {
		int nCount = m_doorPattern.GetCount();
		nPattern = GetPatternIndex(����_������);
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
	else if (CSaveManager::ie()->m_makeData.data[����_��_��ܺ�����] == 1) {
		int nCount = m_doorPatternStair.GetCount();
		sTemp = L"";
		nPattern = GetPatternIndex(����_����ܺ�����);
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

// �迬â
void CPatternManager::GetSmokeScreen(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	int nPattern;
	CString sTemp;
	CStringArray listCircuit;
	listCircuit.Add(g_lpszCircuitName[�����_�迬â]);

	if (CSaveManager::ie()->m_makeData.data[����_�迬â_�ǹ��ش���] == 1) {
		GetSystem(pInfo->szBlock, -99, pInfo->nFloor, 0, listCircuit, system); // �ǹ� �ش���
	}
	else if (CSaveManager::ie()->m_makeData.data[����_�迬â_��ܺ��ش���] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, 0, listCircuit, system); // ��ܺ� �ش���
	}
}

// ���� �ޱ����
void CPatternManager::GetAirDamper(SYSTEM_INFO_* pInfo, CStringArray & pattern)
{
	int nPattern;
	CString sTemp;
	if (CSaveManager::ie()->m_makeData.data[����_�ޱ�_�ǹ�����] == 1) {
		pPatternInfo pPattern;
		int nCount = m_damperPatternAll.GetCount();
		nPattern = GetPatternIndex(����_��������);
		for (int nIndex = 0; nIndex < nCount; nIndex++) {
			pPattern = m_damperPatternAll.GetAt(m_damperPatternAll.FindIndex(nIndex));
			if (pPattern->sBlock.Compare(pInfo->szBlock) == 0) {
				sTemp.Format(L"P%d", nPattern + nIndex + 1);
				CheckAddPatternSystem(sTemp, pattern);
			}
		}
	}
	else if (CSaveManager::ie()->m_makeData.data[����_�ޱ�_��ܺ�����] == 1) {
		pPatternInfo pPattern;
		int nCount = m_damperPatternStair.GetCount();
		nPattern = GetPatternIndex(����_���۰�ܺ�);
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

// ���ǹ�����
void CPatternManager::GetExDamper(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	CStringArray listCircuit;
	if (CSaveManager::ie()->m_makeData.data[����_���_�ǹ��ش���] == 1) {
		listCircuit.Add(g_lpszCircuitName[�����_����_������]);
		GetSystem(pInfo->szBlock, -99, pInfo->nFloor, 0, listCircuit, system); // �ǹ� �ش���

	}
	else if (CSaveManager::ie()->m_makeData.data[����_���_��ܺ��ش���] == 1) {
		listCircuit.Add(g_lpszCircuitName[�����_����_������]);
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, 0, listCircuit, system); // ��ܺ� �ش���
		//if (pInfo->nFloor < 0) { // �ش����� ���ϸ�
		//	GetSystem(pInfo->szBlock, -99, -99, 2, listCircuit, system); // ���� ����
		//}
	}
	if (pInfo->nFloor < 0) { // �ش����� ���ϸ�
		GetSystem(pInfo->szBlock, -99, -99, 2, listCircuit, system); // ���� ����
	}
}
// �ޱ���
void CPatternManager::GetAirFan(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	CStringArray listCircuit;
	listCircuit.Add(g_lpszCircuitName[�����_�ޱ���]);
	if (CSaveManager::ie()->m_makeData.data[����_�ޱ�_�ǹ�����] == 1) {
		GetSystem(pInfo->szBlock, -99, -99, 99, listCircuit, system); // ����
	}
	else if (CSaveManager::ie()->m_makeData.data[����_�ޱ�_��ܺ�����] == 1)
	{
		GetSystem(pInfo->szBlock, pInfo->nStair, -99, 99, listCircuit, system); // ��ܺ� ����
		if (pInfo->nFloor < 0) { // �ش����� ���ϸ�
			GetSystem(pInfo->szBlock, -99, -99, 2, listCircuit, system); // ���� ����
		}
	}
}

// �����
void CPatternManager::CPatternManager::GetExFan(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	CStringArray listCircuit;
	listCircuit.Add(g_lpszCircuitName[�����_�����]);
	if (CSaveManager::ie()->m_makeData.data[����_���_�ǹ��ش���] == 1) {
		GetSystem(pInfo->szBlock, -99, -99, 99, listCircuit, system); // ����
	}
	else if (CSaveManager::ie()->m_makeData.data[����_���_��ܺ��ش���] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, -99, 99, listCircuit, system); // ��ܺ� ����
		if (pInfo->nFloor < 0) { // �ش����� ���ϸ�
			GetSystem(pInfo->szBlock, -99, -99, 2, listCircuit, system); // ���� ����
		}
	}
}

// ���� 1��/2�� ��� (��������)
void CPatternManager::GetSutterSmoke(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	if (CSaveManager::ie()->m_makeData.data[����_����_����1_����2] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[�����_����_1��_���], system); // ���, ��, ��
	}
	else if (CSaveManager::ie()->m_makeData.data[����_����_1����] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[�����_����_1��_���], system); // ���, ��, ��
	}
	else if (CSaveManager::ie()->m_makeData.data[����_����_2����] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[�����_����_2��_���], system); // ���, ��, ��
	}
}

// ���� 1��/2�� ��� (��������)
void CPatternManager::GetSutterFire(SYSTEM_INFO_* pInfo, CStringArray & system)
{
	CStringArray listCircuit;
	if (CSaveManager::ie()->m_makeData.data[����_����_����1_����2] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[�����_����_2��_���], system); // ���, ��, ��
	}
	else if (CSaveManager::ie()->m_makeData.data[����_����_1����] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[�����_����_1��_���], system); // ���, ��, ��
	}
	else if (CSaveManager::ie()->m_makeData.data[����_����_2����] == 1) {
		GetSystem(pInfo->szBlock, pInfo->nStair, pInfo->nFloor, pInfo->szRoomName, g_lpszCircuitName[�����_����_2��_���], system); // ���, ��, ��
	}
}

// �⵿����(��ȭ���⵿) ����
void CPatternManager::GetLamp(CStringArray & system)
{
	CStringArray listCircuit;
	listCircuit.Add(g_lpszCircuitName[�����_P_L_����]);
	GetSystem(L"-99", -99, -99, 99, listCircuit, system); // ����
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

// ���� ���� ���
void CPatternManager::GetPattern(SYSTEM_INFO_* pInfo, CStringArray & pattern, CStringArray & system)
{
	int nCircuitIndex = CCircuitBasicInfo::Instance()->GetIndexCircuitName(pInfo->szCircuitName);
	if (nCircuitIndex == -1) {
		return;
	}
	switch (nCircuitIndex) {
	case �����_�Ƴ��α�_����_SVP:
	case �����_�Ƴ��α�_����_SVP:
	case �����_������_A_SVP:
	case �����_������_B_SVP:
	//case �����_�ּ�������SVP������A:
	//case �����_�ּ�������SVP������B:
		GetValve(pInfo, system);
		break;
	/*case �����_��ܰ�����:
	case �����_ELEV������:*/
	case �����_������:
		GetValve(pInfo, system);
		break;
	case �����_����_����:
	case �����_�Ƴ��α�_����_����:
	case �����_�ּ���_����_����:
		GetValve(pInfo, system);
		GetSutterSmoke(pInfo, system);
		break;
	case �����_����_����:
	case �����_�Ƴ��α�_����_����:
	case �����_�ּ���_����_����:
		GetValve(pInfo, system);
		GetSutterFire(pInfo, system);
		break;
	case �����_P_S_Ȯ��_SVP:
	case �����_�˶����_P_S:
		GetSiren(pInfo, system);
		break;
	case �����_����_�ޱ����:
		GetAirFan(pInfo, system);
		break;
	case �����_����_������:
		GetExFan(pInfo, system);
		break;
	default:
		break;
	}
	// �Է� Ÿ���� ȭ��� 
	for (int nIndex = �����_������; nIndex < �����_�Ƴ��α�_����_SVP; nIndex++) { // ȭ�� Ÿ���� �͵� 
		if (CCircuitBasicInfo::Instance()->GetCircuitName(nCircuitIndex) == g_lpszCircuitName[�����_������_���]
			|| CCircuitBasicInfo::Instance()->GetCircuitName(nCircuitIndex) == g_lpszCircuitName[�����_������_ELEV]) {
			continue;
		}
		if (CCircuitBasicInfo::Instance()->GetCircuitInput(nCircuitIndex) == g_lpszInputType[nIndex]) {
			// 1. ����/�����/�ð��溸
			GetAlarmBroadPattern(pInfo, pattern);
			// 2. �ڵ� ���/â�� ���/����/�κ���/�ڵ���/����
			GetDoorPattern(pInfo, pattern);
			// 3. �迬â
			GetSmokeScreen(pInfo, system);
			// 4. ���Ǳޱ����
			GetAirDamper(pInfo, pattern);
			// 5. ���ǹ�����
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

int CPatternManager::GetPatternIndex(int nIndex)	// ��� ������ ������ ������ �ְ� �ǹ� ������ ���� �ε����� ������ �� �ִµ� �̸� ���
{
	if (nIndex <= FIXED_PATTERN) {
		return GetVeriatyIndex(nIndex);
	}
	int nPattern = GetFixedPatternCount();
	switch (nIndex) {
	case ����_�˶�����:
		if (m_alarmPatternDown.arrayBroad.GetCount() > 0 || m_alarmPatternDown.arrayPattern.GetCount() > 0) {
			return nPattern;
		}
		return 0;
	case ����_�˶�:
		if (m_alarmPattern.GetCount() > 0) {
			return nPattern;
		}
		return 0;
	case ����_�ð��溸:
		if (m_visualalarmPattern.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount();
		}
		return 0;
	case ����_�˶�����:
		if (m_alarmPatternUp.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount() + m_visualalarmPattern.GetCount();
		}
		return 0;
	case ����_������:
		if (m_doorPattern.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount() + m_visualalarmPattern.GetCount() + m_alarmPatternUp.GetCount();
		}
		return 0;
	case ����_����ܺ�����:
		if (m_doorPatternStair.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount() + m_visualalarmPattern.GetCount() + m_alarmPatternUp.GetCount() + m_doorPattern.GetCount();
		}
		return 0;
	case ����_��������:
		if (m_damperPatternAll.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount() + m_visualalarmPattern.GetCount() + m_alarmPatternUp.GetCount() + m_doorPattern.GetCount() + m_doorPatternStair.GetCount();
		}
		return 0;
	case ����_���۰�ܺ�:
		if (m_damperPatternStair.GetCount() > 0) {
			return nPattern + m_alarmPattern.GetCount() + m_visualalarmPattern.GetCount() + m_alarmPatternUp.GetCount() + m_doorPattern.GetCount() + m_doorPatternStair.GetCount() + m_damperPatternAll.GetCount();
		}
		return 0;
	default:
		break;
	}
	return 0;
}

// ���� �ε����� ���� �ε����� ��ȯ
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

// ���� �ε����� ���� �ε����� ��ȯ
int CPatternManager::ChangeFixedIndex()
{
	int nCount = 0;
	// m_alarmPatternDown
	if (m_alarmPatternDown.arrayPattern.GetCount() + m_alarmPatternDown.arrayBroad.GetCount() > 0) {
		return 0;
	}

	return -1;
}

// ������ ���� ���(1���� ����)
void CPatternManager::GetPatternList(int nPatternIndex, CStringArray & system, CStringArray & broad)
{
	// ����
	if (nPatternIndex == 1 && ChangeFixedIndex() == 0) {
		system.Copy(m_alarmPatternDown.arrayPattern); broad.Copy(m_alarmPatternDown.arrayBroad);
		return;
	}

	// ����
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
