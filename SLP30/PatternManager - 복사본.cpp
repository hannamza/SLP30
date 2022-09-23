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
	int n1Count = m_listPattern.GetCount();		// 1. ���� ����
	int n2Count = m_listBasePattern.GetCount();	// 2. ��������
	int n3Count = m_listUpPattern.GetCount();	// 3. ��������
	int n4Count = m_listPattern1.GetCount();	// 4. ����(����, �ð��溸, ����� ����)
	int n5Count = m_listPattern2.GetCount();	// 5. ����(�ڵ����, â�����, ������ ����)
	int n6Count = m_listPattern3.GetCount();	// 6. ����(����, �ð��溸, �����, �ڵ����, â�����, ������ ����)

	pPatternInfo pInfo = NULL;

	switch (nType) {
	case pattentype_normal: // ��, ��ܺ� ����
		for (int nIndex = 0; nIndex < n1Count; nIndex++) {
			pInfo = m_listPattern.GetAt(m_listPattern.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return nIndex + 1;
			}
		}
		break;
	case pattentype_base: // ��, ��ܺ� ����
		for (int nIndex = 0; nIndex < n2Count; nIndex++) {
			pInfo = m_listBasePattern.GetAt(m_listBasePattern.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return n1Count + nIndex + 1;
			}
		}
		break;
	case pattentype_up: // ��, ��ܺ� ����
		for (int nIndex = 0; nIndex < n3Count; nIndex++) {
			pInfo = m_listUpPattern.GetAt(m_listUpPattern.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return n1Count + n2Count + nIndex + 1;
			}
		}
		break;

	case pattentype_exceptalarm: // ����, �ð��溸, ����� ���� ��, ��ܺ�
		for (int nIndex = 0; nIndex < n4Count; nIndex++) {
			pInfo = m_listPattern1.GetAt(m_listPattern1.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return n1Count + n2Count + n3Count + nIndex + 1;
			}
		}
		break;
	case pattentype_exceptdoor: // �ڵ����, â�����, ������ ���� ��, ��ܺ�
		for (int nIndex = 0; nIndex < n5Count; nIndex++) {
			pInfo = m_listPattern2.GetAt(m_listPattern2.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return n1Count + n2Count + n3Count + n4Count + nIndex + 1;
			}
		}
		break;
	case pattentype_exceptalarmdoor: // ����, �ð��溸, �����, �ڵ����, â�����, ������ ���� ��, ��ܺ�
		for (int nIndex = 0; nIndex < n6Count; nIndex++) {
			pInfo = m_listPattern3.GetAt(m_listPattern3.FindIndex(nIndex));
			if (pInfo->nSystem == nSystem && pInfo->nStair == nStair && pInfo->nFloor == nFloor) {
				system.Copy(pInfo->arrayPattern2);
				broad.Copy(pInfo->arrayPattern1);
				return n1Count + n2Count + n3Count + n4Count + n5Count + nIndex + 1;
			}
		}
		break;

	case pattentype_alarmnormal:	// ���� ��, ��ܺ� ����
		system.Copy(m_alarmPattern.arrayPattern2);
		broad.Copy(m_alarmPattern.arrayPattern1);
		return n1Count + n2Count + n3Count + n4Count + n5Count + n6Count + 1;
	case pattentype_alarmup:		// ���� ��, ��ܺ� ����
		system.Copy(m_alarmPatternUp.arrayPattern2);
		broad.Copy(m_alarmPatternUp.arrayPattern1);
		return n1Count + n2Count + n3Count + n4Count + n5Count + n6Count + 2;
	case pattentype_alarmdown:		// ���� ��, ��ܺ� ����
		system.Copy(m_alarmPatternDown.arrayPattern2);
		broad.Copy(m_alarmPatternDown.arrayPattern1);
		return n1Count + n2Count + n3Count + n4Count + n5Count + n6Count + 3;

	case pattentype_doornormal:		// ���� ��, ��ܺ� ����
		system.Copy(m_doorPattern.arrayPattern2);
		broad.Copy(m_doorPattern.arrayPattern1);
		return n1Count + n2Count + n3Count + n4Count + n5Count + n6Count + 4;
	case pattentype_doorup:			// ��, ��ܺ� ����
		system.Copy(m_doorPatternUp.arrayPattern2);
		broad.Copy(m_doorPatternUp.arrayPattern1);
		return n1Count + n2Count + n3Count + n4Count + n5Count + n6Count + 5;
	case pattentype_doordown:		// ��, ��ܺ� ����
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
	if (CSaveManager::Instance()->m_makeData.data[0] == 1) { // �溸 ���� ����
		GetPattern(nStair, nFloor, pattentype_alarmnormal, true, pattern);
	}

	if (CSaveManager::Instance()->m_makeData.data[0] == 1) { // ���� ����
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
	else if (CSaveManager::Instance()->m_makeData.data[1] == 1) { // ��ܺ� ������
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
	else if (CSaveManager::Instance()->m_makeData.data[2] == 1) { // ���� ������
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

	if (CSaveManager::Instance()->m_makeData.data[7] == 1) { // �ڵ����, â�����, ������
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
	// �Ϲ� ���
	case �����_��ȭ��:			// 0
	case �����_�߽ű�:			// 1
	case �����_������:			// 4
	case �����_AN����:			// 5
	case �����_AN����:			// 6
	case �����_���Ǳޱ����:	// 21
	case �����_���ǹ�����:	// 23
	case �����_����������A:	// 30
	case �����_����������B:	// 31
	case �����_��������Ȯ��:	// 32
	case �����_���������⵿:	// 33
	case �����_���������⵿:	// 37
		CommonPattern(nStair, nFloor, pattern); // �Ϲ�
		break;
	// �Ϲ� + ���, ���̷�
	case �����_SVP������A:		// 10
	case �����_SVP������B:		// 11
	case �����_AVP_S:			// 14
		CommonPattern(nStair, nFloor, pattern);
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���], system);
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���̷�], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���], system);
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���̷�], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[���Ÿ��_���], system);
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���], system);
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���̷�], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[���Ÿ��_���], system);
			GetSystemList(nStair, 1, g_lpszOutputCircuit[���Ÿ��_���̷�], system); 
			GetSystemList(nStair, 2, g_lpszOutputCircuit[���Ÿ��_���], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[���Ÿ��_���̷�], system);
		}

		break;
	// �Ϲ� + ���̷� 
	case �����_SVPP_S:			// 12
		CommonPattern(nStair, nFloor, pattern);
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���̷�], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���̷�], system);
			}
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���̷�], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[���Ÿ��_���̷�], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[���Ÿ��_���̷�], system);
		}
		break;
	// �Ϲ� + ���� 1�� ���
	case �����_�ּ�������:		// 7
		CommonPattern(nStair, nFloor, pattern);
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_1�����], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_1�����], system);
			}
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_1�����], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[���Ÿ��_1�����], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[���Ÿ��_1�����], system);
		}
		break;
	// �Ϲ� + ���� 2�� ���
	case �����_�ּ�������:		// 8
	case �����_�ּ�������:		// 9
		CommonPattern(nStair, nFloor, pattern);
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_2�����], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_2�����], system);
			}
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_2�����], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[���Ÿ��_2�����], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[���Ÿ��_2�����], system);
		}
		break;
	// �ޱ�FAN
	case �����_���Ǳޱ����:	// 20
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_��������1], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_��������1], system);
			}
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_��������1], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[���Ÿ��_��������1], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[���Ÿ��_��������1], system);
		}
		break;
	// ���FAN
	case �����_���ǹ�����:	// 22
		if (nFloor < -1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���FAN], system);
			}
		}
		else if (nFloor < 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���FAN], system);
			}
		}
		else if (nFloor == 1) {
			for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex < 0; nIndex++) {
				GetSystemList(nStair, nIndex, g_lpszOutputCircuit[���Ÿ��_���FAN], system);
			}
			GetSystemList(nStair, 1, g_lpszOutputCircuit[���Ÿ��_���FAN], system);
			GetSystemList(nStair, 2, g_lpszOutputCircuit[���Ÿ��_���FAN], system);
		}
		break;
	case �����_�迬â:			// 26
		break;

	case �����_��ܰ�����:		// 2
	case �����_ELEV������:		// 3
	case �����_SVPT_S:			// 13
	case �����_AVT_S:			// 15
	case �����_���Ϳ�������:	// 16
	case �����_���߿�������:	// 17
	case �����_����1�����:	// 18
	case �����_����2�����:	// 19
	case �����_�ڵ����:		// 24
	case �����_â�����:		// 25
	case �����_��ȭ��:			// 27
	case �����_P_L����:		// 28
	case �����_����������:		// 29
	case �����_������F_S:		// 34
	case �����_PUMPP_S:		// 35
	case �����_PUMPT_S:		// 36
	case �����_SMDȮ��:		// 38
	case �����_MVDȮ��:		// 39
	case �����_�ޱ�FAN:		// 40
	case �����_���FAN:		// 41
	case �����_������T_S:		// 42
	case �����_����:			// 43
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
	// ����
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

		m_listPattern.AddTail(pPattern); // ���� ����
	}

	// ��������
	if (pInfo->nFloor < 0) {
		m_listBasePattern.AddTail(pPattern); // ��������
	}
	else {
		m_listUpPattern.AddTail(pPattern); // ���� ����
	}

	// ����(����, �ð��溸)
	if (wcscmp(pInfo->szCircuitName, g_lpszOutputCircuit[���Ÿ��_��������]) == 0)
	{
		m_alarmPattern.arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		// ���� ����(����, �ð��溸, �����)
		if (pPattern->nFloor < 0) {
			m_alarmPatternDown.arrayPattern1.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		}
		// ���� ����(����, �ð��溸, �����)
		else {
			m_alarmPatternUp.arrayPattern1.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		}
	}
	// ����(�ڵ����, â�����, ������, �κ���, �ڵ���, ����)
	if (//wcscmp(pInfo->szCircuitName, g_lpszOutputCircuit[���Ÿ��_�ڵ����]) == 0
		//|| wcscmp(pInfo->szCircuitName, g_lpszOutputCircuit[���Ÿ��_â�����]) == 0
		wcscmp(pInfo->szCircuitName, g_lpszOutputCircuit[���Ÿ��_����]) == 0) {
		m_doorPattern.arrayPattern2.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		// ���� ����(�ڵ����, â�����, ������, �κ���, �ڵ���, ����)
		if (pPattern->nFloor < 0) {
			m_doorPatternDown.arrayPattern1.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		}
		// ���� ����(�ڵ����, â�����, ������, �κ���, �ڵ���, ����)
		else {
			m_doorPatternUp.arrayPattern1.Add(pInfo->nSystemNo + pInfo->nSystem * 256);
		}
	}
}
// ���� ����(����, �ð��溸, ����� ����)
void CPatternManager::AddPatternInfoExceptAlarm(SYSTEM_INFO_* pInfo)
{
	bool bFind = false;
	// ����
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

		m_listPattern1.AddTail(pPattern); // ���� ����
	}
}
// ���� ����(�ڵ����, â�����, ������ ����)
void CPatternManager::AddPatternInfoExceptDoor(SYSTEM_INFO_* pInfo)
{
	bool bFind = false;
	// ����
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

		m_listPattern2.AddTail(pPattern); // ���� ����
	}
}
// ���� ����(����, �ð��溸, �����, �ڵ����, â�����, ������ ����)
void CPatternManager::AddPatternInfoExceptAlarmDoor(SYSTEM_INFO_* pInfo)
{
	bool bFind = false;
	// ����
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

		m_listPattern3.AddTail(pPattern); // ���� ����
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

	// ����(�����)
	m_alarmPattern.arrayPattern1.Add(nCircuitIndex);
	// ���� ����(����, �ð��溸, �����)
	if (pPattern->nFloor < 0) {
		m_alarmPatternDown.arrayPattern1.Add(nCircuitIndex);
	}
	// ���� ����(����, �ð��溸, �����)
	else {
		m_alarmPatternUp.arrayPattern1.Add(nCircuitIndex);
	}
}