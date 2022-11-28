#include "stdafx.h"
#include "SaveManager.h"
#include "MessagePopup.h"
#include "XLEzAutomation.h"
#include "Resource.h"

CMessagePopup* g_pWaitPopup = NULL;
CSaveManager::CSaveManager()
{
	g_pWaitPopup = NULL;
}


CSaveManager::~CSaveManager()
{
	SAFE_DELETE(g_pWaitPopup);
	ReleaseInfo();
}

void CSaveManager::ReleaseInfo()
{
	SYSTEM_INFO_* pInfo = NULL;
	while (!m_listSystem.IsEmpty()) {
		pInfo = m_listSystem.RemoveHead();
		SAFE_DELETE(pInfo);
	}
	BC_INFO* pInfo1 = NULL;
	while (!m_listBC.IsEmpty()) {
		pInfo1 = m_listBC.RemoveHead();
		SAFE_DELETE(pInfo1);
	}
}

void CSaveManager::SetSystemInfo(WCHAR* szBDName, WCHAR* szCircuitName, WCHAR* szRoomName, WCHAR* szBlock, short nStair, short nSystemNo, short nSystem, short nFloor, bool bRooftop, short nCircuitNo)
{
	SYSTEM_INFO_* pInfo = new SYSTEM_INFO_;
	wcscpy_s(pInfo->szBDName, szBDName);
	wcscpy_s(pInfo->szCircuitName, szCircuitName);
	wcscpy_s(pInfo->szRoomName, szRoomName);
	wcscpy_s(pInfo->szBlock, szBlock);
	pInfo->nSystemNo = nSystemNo;
	pInfo->nSystem = nSystem;
	pInfo->nStair = nStair;
	pInfo->nFloor = nFloor;
	pInfo->nCircuitNo = nCircuitNo;
	pInfo->bRooftop = bRooftop;

	m_listSystem.AddTail(pInfo);
	pInfo->nNo = m_listSystem.GetCount();
}

void CSaveManager::SetBroadcast(CString sBlock, short nStair, short nFloor, WCHAR* szBroadcast)
{
	BC_INFO* pInfo = new BC_INFO;
	wcscpy_s(pInfo->szBlock, sBlock.GetBuffer(0));
	pInfo->nFloor = nFloor;
	pInfo->nStair = nStair;
	wcscpy_s(pInfo->BC_CONTAIN, szBroadcast);

	m_listBC.AddTail(pInfo);
}

bool CSaveManager::FileSave(CString sPath)
{
	if (::PathFileExists(sPath)) {
		CMessagePopup popup(L"정보 저장", L"\n\n이미 같은 이름의 파일이 존재합니다.\n\n덮어쓰기 하시겠습니까?", MB_YESNO, NULL);
		UINT nResult = popup.DoModal();
		if (nResult != IDOK) {
			return false;
		}
		if (::PathFileExists(sPath + L".tmp")) {
			::DeleteFile(sPath + L".tmp");
		}
		CFile::Rename(sPath, sPath + L".tmp");
		if (::PathFileExists(sPath)) {
			CMessagePopup popup(L"정보 저장", L"\n\n파일 저장에 실패하였습니다.\n\n파일을 사용중이거나 권한 문제일 수 있습니다.", MB_OK, NULL);
			UINT nResult = popup.DoModal();
			CFile::Rename(sPath + L".tmp", sPath);
			return false;
		}
	}
	FILE* f;
	f = _wfopen(sPath.GetBuffer(0), L"wb");
	if (!f) {
		CMessagePopup popup(L"정보 저장", L"\n\n파일 저장에 실패하였습니다.\n\n파일을 사용중이거나 권한 문제일 수 있습니다.", MB_OK, NULL);
		UINT nResult = popup.DoModal();
		::DeleteFile(sPath + L".tmp");
		return false;
	}
	fseek(f, 0L, SEEK_SET);
	// 1. CircuitBasicInfo
	circuitBasicInfo basic;
	memcpy_s(basic.bCheck, CIRCUIT_PARENT * sizeof(bool), CCircuitBasicInfo::Instance()->m_bCheck, CIRCUIT_PARENT * sizeof(bool));
	basic.nBasement = CCircuitBasicInfo::Instance()->m_nBasement;
	basic.nFloor = CCircuitBasicInfo::Instance()->m_nFloor;
	basic.nStair = CCircuitBasicInfo::Instance()->m_nStair;
	basic.nBlock = CCircuitBasicInfo::Instance()->m_nBlock;
	basic.nRooftop = CCircuitBasicInfo::Instance()->m_nRooftop;
	CString sBlockList;
	for (int nIndex = 0; nIndex < CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount(); nIndex++) {
		sBlockList += CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nIndex);
		if (nIndex + 1 < CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount()) {
			sBlockList += L",";
		}
	}
	wcscpy_s(basic.szBlockList, sBlockList);

	wcscpy_s(basic.szBDName, CCircuitBasicInfo::Instance()->m_sBuildingName.GetBuffer(0));
	DWORD dwWritten = 0, dwWrite = 0;
	dwWritten = fwrite((BYTE*)&basic, 1, sizeof(circuitBasicInfo), f);

	// 2. CircuitCountInfo
	int nCount = 0;
	circuitCountInfo count;
	CIRUIT_COUNT circuit;
	pSelectCircuit sc;
	CString sTemp;
	count.nCount = CCommonState::ie()->m_selectCircuit_0.GetCount();
	count.nCount += CCommonState::ie()->m_selectCircuit_1.GetCount();

	dwWritten = 0;
	dwWritten = fwrite((BYTE*)&count, 1, sizeof(circuitCountInfo), f);

	for (int i = 0; i < 2; i++) {
		switch (i) {
		case 0: nCount = CCommonState::ie()->m_selectCircuit_0.GetCount(); break;
		case 1: nCount = CCommonState::ie()->m_selectCircuit_1.GetCount(); break;
		default: break;
		}
		for (int nIndex = 0; nIndex < nCount; nIndex++) {
			switch (i) {
			case 0:
				sc = CCommonState::ie()->m_selectCircuit_0.GetAt(CCommonState::ie()->m_selectCircuit_0.FindIndex(nIndex));
				break;
			case 1:
				sc = CCommonState::ie()->m_selectCircuit_1.GetAt(CCommonState::ie()->m_selectCircuit_1.FindIndex(nIndex));
				break;
			default:
				continue;
			}
			circuit.nCount = sc->nCount;
			wcscpy_s(circuit.szName, sc->sCircuitName.GetBuffer(0));
			wcscpy_s(circuit.szFloor, sc->sFloor.GetBuffer(0));
			wcscpy_s(circuit.szSystem, sc->sSystem.GetBuffer(0));
			wcscpy_s(circuit.szBlock, sc->sBlock.GetBuffer(0));
			wcscpy_s(circuit.szStair, sc->sStair.GetBuffer(0));

			dwWritten = 0;
			dwWritten = fwrite((BYTE*)&circuit, 1, sizeof(CIRUIT_COUNT), f);
		}
	}

	// 3. CircuitSystemInfo
	circuitSystemInfo system;
	system.nCount = m_listSystem.GetCount();

	dwWritten = 0;
	dwWritten = fwrite((BYTE*)&system, sizeof(circuitSystemInfo), 1, f);

	SYSTEM_INFO_* pInfo = NULL;
	for (int nIndex = 0; nIndex < m_listSystem.GetCount(); nIndex++) {
		pInfo = m_listSystem.GetAt(m_listSystem.FindIndex(nIndex));
		dwWritten = 0;
		dwWritten = fwrite((BYTE*)pInfo, sizeof(SYSTEM_INFO_), 1, f);
	}
	// 4. PSInfo
	dwWritten = 0;
	dwWritten = fwrite((BYTE*)&m_PSInfo, 1, sizeof(circuitPSInfo), f);

	// 5. PumpInfo
	dwWritten = 0;
	dwWritten = fwrite((BYTE*)&m_pumpInfo, 1, sizeof(circuitPumpInfo), f);

	// 6. BroadcastInfo
	BroadcastInfo bci;
	bci.nCount = m_listBC.GetCount();
	bci.nBroadType = m_nBroadType;

	dwWritten = 0;
	dwWritten = fwrite((BYTE*)&bci, 1, sizeof(BroadcastInfo), f);
	BC_INFO* bc = NULL;
	for (int nIndex = 0; nIndex < bci.nCount; nIndex++) {
		bc = m_listBC.GetAt(m_listBC.FindIndex(nIndex));

		dwWritten = 0;
		dwWritten = fwrite((BYTE*)bc, 1, sizeof(BC_INFO), f);
	}
	// 7. link setup data
	dwWritten = 0;
	dwWritten = fwrite((BYTE*)&m_makeData, 1, sizeof(MAKE_DATA), f);
	// 8. version
	dwWritten == 0;
	dwWritten = fwrite((BYTE*)&m_versionDate, 1, sizeof(versionDate), f);

	fclose(f);

	::DeleteFile(sPath + L".tmp");
	return true;
}

bool CSaveManager::FileLoad(CString sPath)
{
	if (!::PathFileExists(sPath)) {
		CMessagePopup popup(L"불러오기", L"\n\n파일이 존재하지 않습니다.\n\n확인하시고 다시 시도하여 주십시오.", MB_OK, NULL);
		UINT nResult = popup.DoModal();
		return false;
	}
	FILE* f;
	f = _wfopen(sPath.GetBuffer(0), L"rb");
	if (!f) {
		CMessagePopup popup(L"불러오기", L"\n\n파일 읽기에 실패하였습니다.\n\n파일을 사용중이거나 권한 문제일 수 있습니다.", MB_OK, NULL);
		UINT nResult = popup.DoModal();
		return false;
	}
	fseek(f, 0L, SEEK_SET);
	DWORD dwRead = 0, dwReadden = 0;
	// 1. CircuitBasicInfo
	circuitBasicInfo basic;
	long lTest = 0;
	while (dwReadden != sizeof(circuitBasicInfo)) {
		dwRead = fread((BYTE*)&basic, 1, sizeof(circuitBasicInfo), f);
		if(dwRead == 0){
			fclose(f);
			return false;
		}
		dwReadden += dwRead;
	}
	lTest += sizeof(circuitBasicInfo);

	memcpy_s(CCircuitBasicInfo::Instance()->m_bCheck, CIRCUIT_PARENT * sizeof(bool), basic.bCheck, CIRCUIT_PARENT * sizeof(bool));
	//프로젝트 로드 후 최초 설비 수정 후 다시 사용자가 취소를 선택하면 old check 정보가 존재하지 않으므로 프로젝트가 로드되는 이 시점에서 한번 현재의 설비 check정보를 쓴다.
	memcpy_s(CCircuitBasicInfo::Instance()->m_bOldCheck, CIRCUIT_PARENT * sizeof(bool), basic.bCheck, CIRCUIT_PARENT * sizeof(bool));

	CString sBlockList, sBlock;
	CCircuitBasicInfo::Instance()->m_nBasement = basic.nBasement;
	CCircuitBasicInfo::Instance()->m_nFloor = basic.nFloor;
	CCircuitBasicInfo::Instance()->m_nRooftop = basic.nRooftop;
	CCircuitBasicInfo::Instance()->m_nStair = basic.nStair;
	CCircuitBasicInfo::Instance()->m_nBlock = basic.nBlock;
	CCircuitBasicInfo::Instance()->m_sBuildingName = basic.szBDName;
	sBlockList = basic.szBlockList;
	CCircuitBasicInfo::Instance()->m_sBlockName = sBlockList;

	CCircuitBasicInfo::Instance()->m_arrayBlockName.RemoveAll();
	for (int nIndex = 0; nIndex < basic.nBlock; nIndex++) {
		if (!AfxExtractSubString(sBlock, sBlockList, nIndex, L',')) {
			fclose(f);
			return false;
		}
		CCircuitBasicInfo::Instance()->m_arrayBlockName.Add(sBlock);
	}

	// 2. CircuitCountInfo
	CCommonState::ie()->ReleaseCircuit(0);
	CCommonState::ie()->ReleaseCircuit(1);

	int nSize = 0;
	long lPoint = ftell(f);
	circuitCountInfo count;
	BYTE* pCount = NULL;
	dwRead = 0;
	fread((BYTE*)&count, 1, sizeof(circuitCountInfo), f);
	if (count.nCount > 0) {
		fseek(f, lPoint, SEEK_SET);
		nSize = sizeof(circuitCountInfo) + sizeof(CIRUIT_COUNT) * count.nCount;
		pCount = new BYTE[nSize];
		dwRead = 0;
		dwReadden = 0;
		while (dwReadden != nSize) {
			dwRead = fread(pCount, 1, nSize, f);
			if (dwRead == 0) {
				fclose(f);
				SAFE_DELETE(pCount);
				return false;
			}
			dwReadden += dwRead;
		}
		pSelectCircuit pCircuit;
		circuitCountInfo* pInfo = (circuitCountInfo*)pCount;
		for (int nIndex = 0; nIndex < pInfo->nCount; nIndex++) {
			pCircuit = new selectCircuit;
			pCircuit->nCount = pInfo->info[nIndex].nCount;
			pCircuit->sCircuitName = pInfo->info[nIndex].szName;
			pCircuit->sBlock = pInfo->info[nIndex].szBlock;
			pCircuit->sFloor = pInfo->info[nIndex].szFloor;
			pCircuit->sStair = pInfo->info[nIndex].szStair;
			pCircuit->sSystem = pInfo->info[nIndex].szSystem;

			if (wcscmp(pInfo->info[nIndex].szSystem, L"0 계통") == 0) {
				CCommonState::ie()->m_selectCircuit_0.AddTail(pCircuit);
			}
			else if (wcscmp(pInfo->info[nIndex].szSystem, L"1 계통") == 0) {
				CCommonState::ie()->m_selectCircuit_1.AddTail(pCircuit);
			}
			else {
				SAFE_DELETE(pCircuit);
				SAFE_DELETE(pCount);
				fclose(f);
				return false;
			}
		}
	}
	SAFE_DELETE(pCount);

	lTest += nSize;

	// 3. CircuitSystemInfo
	ReleaseInfo();

	circuitSystemInfo system, *pSystem = NULL;
	BYTE* pBuff = NULL;
	dwRead = 0;
	lPoint = ftell(f);
	if (!fread((BYTE*)&system, 1, sizeof(circuitSystemInfo), f) < 0) {
		fclose(f);
		return false;
	}
	if (system.nCount > 0) {
		fseek(f, lPoint, SEEK_SET);
		nSize = sizeof(circuitSystemInfo) + (system.nCount * sizeof(SYSTEM_INFO_));
		pBuff = new BYTE[nSize];
		dwRead = 0;
		dwReadden = 0;
		while (dwReadden != nSize) {
			dwRead = fread(pBuff, 1, nSize - dwReadden, f);
			if (dwRead == 0) {
				fclose(f);
				SAFE_DELETE(pBuff);
				return false;
			}
			dwReadden += dwRead;
		}
		pSystem = (circuitSystemInfo*)pBuff;
		for (int nIndex = 0; nIndex < pSystem->nCount; nIndex++) {
			SYSTEM_INFO_* pInfo = new SYSTEM_INFO_;
			memcpy_s(pInfo, sizeof(SYSTEM_INFO_), &pSystem->info[nIndex], sizeof(SYSTEM_INFO_));
			m_listSystem.AddTail(pInfo);
		}
	}
	SAFE_DELETE(pBuff);

	lTest += nSize;

	// 4. PSInfo
	dwRead = 0;
	if (!fread((BYTE*)&m_PSInfo, 1, sizeof(circuitPSInfo), f) < 0) {
		fclose(f);
		return false;
	}
	lTest += sizeof(circuitPSInfo);

	// 5. PumpInfo
	dwRead = 0;
	if (!fread((BYTE*)&m_pumpInfo, 1, sizeof(circuitPumpInfo), f) < 0) {
		fclose(f);
		return false;
	}
	lTest += sizeof(circuitPumpInfo);

	// 6. BroadcastInfo
	BroadcastInfo bci, *pBroad = NULL;
	dwRead = 0;
	lPoint = ftell(f);
	if (!fread((BYTE*)&bci, 1, sizeof(BroadcastInfo), f) < 0) {
		fclose(f);
		return false;
	}
	m_nBroadType = bci.nBroadType;
	if (bci.nCount > 0) {
		fseek(f, lPoint, SEEK_SET);

		int n1 = sizeof(BroadcastInfo);
		int n2 = sizeof(BC_INFO);
		nSize = sizeof(BroadcastInfo) + (bci.nCount * sizeof(BC_INFO));
		pBuff = new BYTE[nSize];
		dwRead = 0;
		dwReadden = 0;
		while (dwReadden != nSize) {
			dwRead = fread(pBuff, 1, nSize - dwReadden, f);
			if (dwRead == 0) {
				fclose(f);
				SAFE_DELETE(pBuff);
				return false;
			}
			dwReadden += dwRead;
		}
		pBroad = (BroadcastInfo*)pBuff;
		for (int nIndex = 0; nIndex < pBroad->nCount; nIndex++) {
			BC_INFO* pInfo = new BC_INFO;
			memcpy_s(pInfo, sizeof(BC_INFO), &pBroad->info[nIndex], sizeof(BC_INFO));
			m_listBC.AddTail(pInfo);
		}
	}
	SAFE_DELETE(pBuff);

	// 7. link setup data
	dwRead = 0;
	dwRead = fread((BYTE*)&m_makeData, 1, sizeof(MAKE_DATA), f);

	// 8. version and date
	dwRead = 0;
	if (!fread((BYTE*)&m_versionDate, 1, sizeof(versionDate), f) < 0) {
		fclose(f);
		return false;
	}

	fclose(f);
	return true;
}

void CSaveManager::ReleaseExcelWaitPopup()
{
	if (g_pWaitPopup) {
		g_pWaitPopup->ShowWindow(SW_HIDE);
		SAFE_DELETE(g_pWaitPopup);
	}
}

int CSaveManager::ExcelFileSave(CString sPath)
{
	CString strRooftop;
	strRooftop = ROOFTOP_NAME;

	WCHAR wszPath[2048];
	GetModuleFileName(NULL, wszPath, 2048);
	PathRemoveFileSpec(wszPath);
	CString sSamplePath;
	sSamplePath.Format(L"%s\\sample1.xlsx", wszPath);
	if (sSamplePath.CompareNoCase(sPath) == 0) {
		CMessagePopup popup(L"Excel 정보 생성", L"\n\n해당 파일은 프로그램 파일입니다.\n\n다른 경로를 선택하여 주십시오", MB_OK, CCommonState::ie()->m_pWnd);
		UINT nResult = popup.DoModal();
		return -1;
	}

	if (::PathFileExists(sPath)) {
		CMessagePopup popup(L"Excel 정보 생성", L"\n\n이미 같은 이름의 파일이 존재합니다.\n\n덮어쓰기 하시겠습니까?", MB_YESNO, CCommonState::ie()->m_pWnd);
		UINT nResult = popup.DoModal();
		if (nResult != IDOK) {
			return -1;
		}
		if (::PathFileExists(sPath + L".tmp")) {
			::DeleteFile(sPath + L".tmp");
		}
		HANDLE hHandle = CreateFile(sPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hHandle == INVALID_HANDLE_VALUE) {
			CMessagePopup popup(L"Excel 정보 생성", L"\n\n파일 저장에 실패하였습니다.\n\n이미 해당 파일이 사용중입니다.", MB_OK, CCommonState::ie()->m_pWnd);
			UINT nResult = popup.DoModal();
			return -4;
		}
		CloseHandle(hHandle);

		CFile::Rename(sPath, sPath + L".tmp");
		if (::PathFileExists(sPath)) {
			CMessagePopup popup(L"Excel 정보 생성", L"\n\n파일 저장에 실패하였습니다.\n\n파일을 사용중이거나 권한 문제일 수 있습니다.", MB_OK, CCommonState::ie()->m_pWnd);
			UINT nResult = popup.DoModal();
			CFile::Rename(sPath + L".tmp", sPath);
			return -4;
		}
	}
	::DeleteFile(sPath + L".tmp2");

	//sample1.xlsx (중계기 일람표 탬플릿) 파일이 없으면 엑셀 저장 실패
	if (!::CopyFile(sSamplePath, sPath + L".tmp2", true)) {
		CMessagePopup popup(L"Excel 정보 생성", L"\n\n파일 저장에 실패하였습니다.\n\n파일을 사용중이거나 권한 문제일 수 있습니다.", MB_OK, CCommonState::ie()->m_pWnd);
		UINT nResult = popup.DoModal();
		return -5;
	}

	CRect rect, rt, rect1;
	CCommonState::ie()->m_pWnd->GetClientRect(&rect);
	CCommonState::ie()->m_pWnd->ClientToScreen(&rect);
	if (g_pWaitPopup) {
		SAFE_DELETE(g_pWaitPopup);
	}
	g_pWaitPopup = new CMessagePopup(L"Excel 정보 생성", L"\n\n\nExcel 정보를 생성중입니다. \n\n잠시만 기다려 주세요.", -1, CCommonState::ie()->m_pWnd);
	g_pWaitPopup->Create(IDD_COMMON_POPUP_DIALOG);
	g_pWaitPopup->ShowWindow(SW_HIDE);

	g_pWaitPopup->GetClientRect(&rt);
	g_pWaitPopup->ClientToScreen(&rt);

	rect1.CopyRect(&rect);
	rect1.top += rect.Height() / 2 - rt.Height() / 2;
	rect1.left += rect.Width() / 2 - rt.Width() / 2;
	rect1.bottom = rect1.top + rt.Height();
	rect1.right = rect1.left + rt.Width();
	g_pWaitPopup->MoveWindow(&rect1);
	g_pWaitPopup->ShowWindow(SW_SHOW);

	g_pWaitPopup->SetForegroundWindow();
	::SetWindowPos(g_pWaitPopup->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	g_pWaitPopup->SetCaptionAddBottom(L"                파일 생성중입니다.               ");

	CString sTemp;
	CXLEzAutomation XL(FALSE);
	XL.OpenExcelFile(sPath + L".tmp2");
	XL.SetCellValue(3, 2, CCircuitBasicInfo::Instance()->m_sBuildingName);
	XL.SetCellValue(16, 2, CCircuitBasicInfo::Instance()->m_sBuildingName);

	// CircuitSystemInfo
	SYSTEM_INFO_* pInfo = NULL;
	int nCircuitNo0 = 0, nCircuitNo1 = 0, nColumn = 0, nRow = 0, nCircuitNo = 0;
	int nTotalCount = m_listSystem.GetCount() + m_listBC.GetCount() + 7;
	int nCurrentCount = 0;
	for (int nIndex = 0; nIndex < m_listSystem.GetCount(); nIndex++) {
		pInfo = m_listSystem.GetAt(m_listSystem.FindIndex(nIndex));
		if (pInfo->nSystem == 1 && nColumn == 0) {
			nCircuitNo1 = 0;
			nColumn = 13;
			nRow = 0;
			nCircuitNo = MAX_CIRCUIT;
		}
		sTemp = L"";
		if (pInfo->nSystemNo > 0) {
			sTemp.Format(L"%d", (nCircuitNo0 + 1) % 256);// (pInfo->nSystemNo + nCircuitNo) % 256);
			XL.SetCellValue(nColumn + 1, 4 + nRow, sTemp.GetBuffer(0));// 중계기 No
		}
		sTemp.Format(L"%d", (nCircuitNo0 + 1) % 256);
		++nCircuitNo0;
		XL.SetCellValue(nColumn + 2, 4 + nRow, sTemp.GetBuffer(0)); // 회로 No
		XL.SetCellValue(nColumn + 10, 4 + nRow, pInfo->szRoomName); // 실명
		XL.SetCellValue(nColumn + 11, 4 + nRow, pInfo->szCircuitName); // 설비명
		if (pInfo->nCircuitNo > 0) {
			sTemp.Format(L"%d", pInfo->nCircuitNo);
			XL.SetCellValue(nColumn + 12, 4 + nRow, sTemp.GetBuffer(0)); // 설비번호
		}
		if (wcslen(pInfo->szBlock/*szCircuitName*/) > 0) {
			sTemp = pInfo->szBlock;
			sTemp += L"동";
			XL.SetCellValue(nColumn + 6, 4 + nRow, sTemp); // 동(건물명)
		}
		sTemp = CCircuitBasicInfo::Instance()->GetCircuitInput(CCircuitBasicInfo::Instance()->GetIndexCircuitName(pInfo->szCircuitName));
		XL.SetCellValue(nColumn + 3, 4 + nRow, sTemp); // 입력타입

		sTemp = CCircuitBasicInfo::Instance()->GetCircuitOutput(CCircuitBasicInfo::Instance()->GetIndexCircuitName(pInfo->szCircuitName));
		XL.SetCellValue(nColumn + 4, 4 + nRow, sTemp); // 출력타입

		sTemp = CCircuitBasicInfo::Instance()->GetCircuitOutputName(CCircuitBasicInfo::Instance()->GetIndexCircuitName(pInfo->szCircuitName));
		XL.SetCellValue(nColumn + 13, 4 + nRow, sTemp); // 출력내용

		if (pInfo->nStair > 0) {
			sTemp.Format(L"%d계단", pInfo->nStair);
		}
		XL.SetCellValue(nColumn + 8, 4 + nRow, sTemp.GetBuffer(0)); // 계단
		sTemp = L"";
		if (pInfo->nFloor < 0) {
			sTemp.Format(L"B%dF", abs(pInfo->nFloor));
		}
		else if (pInfo->nFloor > 0) {
			if (pInfo->bRooftop)		//옥탑층
			{
				sTemp = strRooftop;
			}	
			else
			{
				sTemp.Format(L"%dF", pInfo->nFloor);
			}
		}
		XL.SetCellValue(nColumn + 9, 4 + nRow, sTemp.GetBuffer(0)); // 층

		++nRow;

		sTemp.Format(L"%d / %d (전체/완료)", nTotalCount, nCurrentCount);
		++nCurrentCount;
		g_pWaitPopup->SetCaptionAddBottom(sTemp);
	}
	XL.ChangeWorksheet(2);

	BC_INFO* pBc = NULL;
	for (int nIndex = 0; nIndex < m_listBC.GetCount(); nIndex++) {
		pBc = m_listBC.GetAt(m_listBC.FindIndex(nIndex));
		if (wcslen(pInfo->szBlock) > 0) {
			if (pBc->nFloor == CCircuitBasicInfo::Instance()->m_nFloor + 1)	//옥탑
			{
				sTemp.Format(L"%s동 %d계단 %s", pBc->szBlock, pBc->nStair, strRooftop);
			}
			else
			{
				sTemp.Format(L"%s동 %d계단 %s%dF", pBc->szBlock, pBc->nStair, (pBc->nFloor < 0) ? L"B" : L"", abs(pBc->nFloor));
			}		
		}
		else {
			if (pBc->nFloor == CCircuitBasicInfo::Instance()->m_nFloor + 1)	//옥탑
			{
				sTemp.Format(L"%d계단 %s", pBc->nStair, strRooftop);
			}
			else
			{
				sTemp.Format(L"%d계단 %s%dF", pBc->nStair, (pBc->nFloor < 0) ? L"B" : L"", abs(pBc->nFloor));
			}
		}
		XL.SetCellValue(2, 3 + nIndex, sTemp.GetBuffer(0));
		XL.SetCellValue(3, 3 + nIndex, pBc->BC_CONTAIN);
		sTemp.Format(L"%d / %d (전체/완료)", nTotalCount, nCurrentCount);
		++nCurrentCount;
		g_pWaitPopup->SetCaptionAddBottom(sTemp);
	}
	XL.ChangeWorksheet(3);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		if (m_pumpInfo.use[nIndex] == 1) {
			sTemp = L"사용";
		}
		else {
			sTemp = L"사용안함";
		}
		XL.SetCellValue(3, 2 + nIndex, sTemp.GetBuffer(0));
		if (m_pumpInfo.lamp[nIndex] == 1) {
			sTemp = L"사용";
		}
		else {
			sTemp = L"사용안함";
		}
		XL.SetCellValue(4, 2 + nIndex, sTemp.GetBuffer(0));
		++nCurrentCount;
		g_pWaitPopup->SetCaptionAddBottom(sTemp);
	}
	XL.ChangeWorksheet(4);
	for (int nIndex = 0; nIndex < 3; nIndex++) {
		if (m_PSInfo.use[nIndex] == 1) {
			sTemp = L"사용";
		}
		else {
			sTemp = L"사용안함";
		}
		XL.SetCellValue(3, 2 + nIndex, sTemp.GetBuffer(0));
		++nCurrentCount;
		g_pWaitPopup->SetCaptionAddBottom(sTemp);
	}

	g_pWaitPopup->SetCaptionAddBottom(L"                저장 중입니다.               ");

	XL.SaveFileAs(sPath);
	XL.ReleaseExcel();

	::DeleteFile(sPath + L".tmp2");
	::DeleteFile(sPath + L".tmp");
	return true;
}

void CSaveManager::DeleteSystemInfo()
{
	std::vector<selectCircuitCompRet>::iterator iterSCCR;
	std::vector<CString>::iterator iterCircuitName;
	std::vector<selectCircuitComp*>::iterator iterSCC;

	POSITION pos;
	SYSTEM_INFO_* pSI;

	CString sSystem, sBDName, sBlock, sStair, sFloor, sCircuitName, sRoomName;
	CString strRooftop;
	strRooftop = ROOFTOP_NAME;
	int nNo, nSystemNo, nSystem, nStair, nFloor, nCircuitNo, nFindIndex;
	bool bDetector;

	if (CCommonState::ie()->m_vecDeleteCircuit.size() > 0)
	{
		iterSCCR = CCommonState::ie()->m_vecDeleteCircuit.begin();
		for (; iterSCCR != CCommonState::ie()->m_vecDeleteCircuit.end(); iterSCCR++)
		{
			bDetector = false;

			int nCircuitCnt = abs(iterSCCR->nDiffCount);
			for (int i = 0; i < nCircuitCnt; i++)
			{
				//감지기류인지 판단
				bDetector = CCircuitBasicInfo::Instance()->IsDetector(iterSCCR->sCircuitName);
				if (!bDetector)
				{
					iterCircuitName = iterSCCR->vecSystemName.begin();
					for (; iterCircuitName != iterSCCR->vecSystemName.end(); iterCircuitName++)
					{
						pos = CSaveManager::ie()->m_listSystem.GetTailPosition();
						while (pos)
						{
							pSI = CSaveManager::ie()->m_listSystem.GetPrev(pos);

							//회로명
							sCircuitName = pSI->szCircuitName;

							//회로명이 없으면 아래 과정이 필요없음
							if (sCircuitName.IsEmpty())
								continue;

							//계통
							sSystem.Format(L"%d 계통", pSI->nSystem);

							//동
							sBlock.Format(L"%s동", pSI->szBlock);

							//계단
							sStair.Format(L"%d계단", pSI->nStair);

							//층
							if (pSI->nFloor < 0)
							{
								sFloor.Format(L"B%dF", abs(pSI->nFloor));
							}
							else
							{
								if (pSI->bRooftop)	//옥탑층
								{
									sFloor = strRooftop;
								}
								else
								{
									sFloor.Format(L"%dF", pSI->nFloor);
								}				
							}

							//비교
							if ((sSystem.Compare(iterSCCR->sSystem) == 0)
								&& (sBlock.Compare(iterSCCR->sBlock) == 0)
								&& (sStair.Compare(iterSCCR->sStair) == 0)
								&& (sFloor.Compare(iterSCCR->sFloor) == 0)
								&& (sCircuitName.Compare(*iterCircuitName) == 0)
								)
							{
								//일치하는 회로정보 초기화
								wcscpy_s(pSI->szBDName, L"");
								wcscpy_s(pSI->szCircuitName, L"");
								wcscpy_s(pSI->szRoomName, L"");
								wcscpy_s(pSI->szBlock, L"");
								pSI->nSystemNo = 0;
								//회로No는 일단 남겨둠
								//pSI->nNo = 0;
								pSI->nStair = 0;
								pSI->nFloor = 0;
								pSI->nCircuitNo = 0;

								//찾았으니 break;
								break;
							}
						}
					}
				}
				else
				{
					pos = CSaveManager::ie()->m_listSystem.GetTailPosition();
					while (pos)
					{
						pSI = CSaveManager::ie()->m_listSystem.GetPrev(pos);

						//회로명
						sCircuitName = pSI->szCircuitName;

						//회로명이 없으면 아래 과정이 필요없음
						if (sCircuitName.IsEmpty())
							continue;

						//계통
						sSystem.Format(L"%d 계통", pSI->nSystem);

						//동
						sBlock.Format(L"%s동", pSI->szBlock);

						//계단
						sStair.Format(L"%d계단", pSI->nStair);

						//층
						if (pSI->nFloor < 0)
						{
							sFloor.Format(L"B%dF", abs(pSI->nFloor));
						}
						else
						{
							if (pSI->bRooftop)	//옥탑층
							{
								sFloor = strRooftop;
							}
							else
							{
								sFloor.Format(L"%dF", pSI->nFloor);
							}
						}

						//비교
						if ((sSystem.Compare(iterSCCR->sSystem) == 0)
							&& (sBlock.Compare(iterSCCR->sBlock) == 0)
							&& (sStair.Compare(iterSCCR->sStair) == 0)
							&& (sFloor.Compare(iterSCCR->sFloor) == 0)
							&& (sCircuitName.Compare(iterSCCR->sCircuitName) == 0)
							)
						{
							//일치하는 회로정보 초기화
							wcscpy_s(pSI->szBDName, L"");
							wcscpy_s(pSI->szCircuitName, L"");
							wcscpy_s(pSI->szRoomName, L"");
							wcscpy_s(pSI->szBlock, L"");
							pSI->nSystemNo = 0;
							//회로No는 일단 남겨둠
							//pSI->nNo = 0;
							pSI->nStair = 0;
							pSI->nFloor = 0;
							pSI->nCircuitNo = 0;

							//찾았으니 break;
							break;
						}
					}
				}
			}
		}
	}
}

void CSaveManager::AddSystemInfo()
{
	std::vector<selectCircuitCompRet>::iterator iterSCCR;
	std::vector<CString>::iterator iterCircuitName;
	std::vector<selectCircuitComp*>::iterator iterSCC;

	POSITION pos;
	SYSTEM_INFO_* pSI;
	CString strRooftop;
	strRooftop = ROOFTOP_NAME;

	CString sSystem, sBDName, sBlock, sStair, sFloor, sCircuitName, sRoomName;
	int nNo, nSystemNo, nSystem, nStair, nFloor, nCircuitNo, nFindIndex, nCircuitNoAcc;
	bool bDetector;

	if (CCommonState::ie()->m_vecAddCircuit.size() > 0)
	{
		bDetector = false;
		iterSCCR = CCommonState::ie()->m_vecAddCircuit.begin();
		for (; iterSCCR != CCommonState::ie()->m_vecAddCircuit.end(); iterSCCR++)
		{
			nCircuitNoAcc = 0;
			int nCircuitCount = iterSCCR->nDiffCount;
			for (int i = 0; i < nCircuitCount; i++)
			{
				nCircuitNoAcc++;
	
				//감지기류인지 판단
				bDetector = CCircuitBasicInfo::Instance()->IsDetector(iterSCCR->sCircuitName);
				if (!bDetector)
				{
					iterCircuitName = iterSCCR->vecSystemName.begin();
					for (; iterCircuitName != iterSCCR->vecSystemName.end(); iterCircuitName++)
					{
						sBDName = CCircuitBasicInfo::Instance()->m_sBuildingName;
						sCircuitName = *iterCircuitName;
						sRoomName = L"";
						sBlock = iterSCCR->sBlock;

						if (iterSCCR->sSystem.Compare(L"0 계통") == 0)
						{
							nSystem = 0;
						}
						else
						{
							nSystem = 1;
						}

						if (nSystem == 0)
						{
							nNo = CCommonState::ie()->m_nTotalCountCircuit_0 + 1;
							nSystemNo = nNo;
							nFindIndex = CCommonState::ie()->m_nTotalCountCircuit_0;

							int nCircuitSize = CCommonState::ie()->CalculateCircuitAddressCount(sCircuitName);
							CCommonState::ie()->m_nTotalCountCircuit_0 += nCircuitSize;
						}
						else
						{
							nNo = CCommonState::ie()->m_nTotalCountCircuit_1 + 1;
							nSystemNo = nNo;
							int nMaxCircuitAddress = MAX_CIRCUIT_ADDRESS;
							nFindIndex = CCommonState::ie()->m_nTotalCountCircuit_1 + nMaxCircuitAddress;		// 1회로는 앞에 0회로 다음에 순차적으로 붙기 때문에 0회로분의 256개 이후에 회로 개수만큼 후에 붙인다.

							int nCircuitSize = CCommonState::ie()->CalculateCircuitAddressCount(sCircuitName);
							CCommonState::ie()->m_nTotalCountCircuit_1 += nCircuitSize;
						}

						sStair = iterSCCR->sStair;
						sStair.Replace(L"계단", L"");
						nStair = _wtoi(sStair.GetBuffer(0));
						sBlock.Replace(L"동", L"");

						sFloor = iterSCCR->sFloor;
						if (sFloor.Find(L"B") >= 0) {
							sFloor.Replace(L"B", L"");
							sFloor.Replace(L"F", L"");
							nFloor = -_wtoi(sFloor.GetBuffer(0));
						}
						else {
							if (iterSCCR->sFloor.Compare(strRooftop) == 0)	//옥탑층
							{
								nFloor = CCircuitBasicInfo::Instance()->m_nFloor + 1;
							}
							else
							{
								sFloor.Replace(L"F", L"");
								nFloor = _wtoi(sFloor.GetBuffer(0));
							}					
						}

						nCircuitNo = iterSCCR->nLastCircuitNo + nCircuitNoAcc;

						//회로인덱스로 위치를 찾는다.
						pSI = CSaveManager::ie()->m_listSystem.GetAt(CSaveManager::ie()->m_listSystem.FindIndex(nFindIndex));

						//회로 정보를 찾은 위치에 채워 넣는다.
						wcscpy_s(pSI->szBDName, sBDName);
						wcscpy_s(pSI->szCircuitName, sCircuitName);
						wcscpy_s(pSI->szRoomName, sRoomName);
						wcscpy_s(pSI->szBlock, sBlock);
						pSI->nNo = nNo;
						if (iterCircuitName == iterSCCR->vecSystemName.begin())		//감지기 류 아니면 첫번째 회로 들어갈 때만 중계기 번호 넣음
							pSI->nSystemNo = nSystemNo;
						pSI->nSystem = nSystem;
						pSI->nStair = nStair;
						pSI->nFloor = nFloor;
						pSI->nCircuitNo = nCircuitNo;

						//옥탑방 처리
						if (nFloor == CCircuitBasicInfo::Instance()->m_nFloor + 1)
							pSI->bRooftop = true;
						else
							pSI->bRooftop = false;
					}
				}
				else
				{
					sBDName = CCircuitBasicInfo::Instance()->m_sBuildingName;
					sCircuitName = iterSCCR->sCircuitName;
					sRoomName = L"";
					sBlock = iterSCCR->sBlock;

					if (iterSCCR->sSystem.Compare(L"0 계통") == 0)
					{
						nSystem = 0;
					}
					else
					{
						nSystem = 1;
					}

					if (nSystem == 0)
					{
						nNo = CCommonState::ie()->m_nTotalCountCircuit_0 + 1;
						nSystemNo = nNo;
						nFindIndex = CCommonState::ie()->m_nTotalCountCircuit_0;

						int nCircuitSize = CCommonState::ie()->CalculateCircuitAddressCount(sCircuitName);
						CCommonState::ie()->m_nTotalCountCircuit_0 += nCircuitSize;
					}
					else
					{
						nNo = CCommonState::ie()->m_nTotalCountCircuit_1 + 1;
						nSystemNo = nNo;
						int nMaxCircuitAddress = MAX_CIRCUIT_ADDRESS;
						nFindIndex = CCommonState::ie()->m_nTotalCountCircuit_1 + nMaxCircuitAddress;		// 1회로는 앞에 0회로 다음에 순차적으로 붙기 때문에 0회로분의 256개 이후에 회로 개수만큼 후에 붙인다.

						int nCircuitSize = CCommonState::ie()->CalculateCircuitAddressCount(sCircuitName);
						CCommonState::ie()->m_nTotalCountCircuit_1 += nCircuitSize;
					}

					sStair = iterSCCR->sStair;
					sStair.Replace(L"계단", L"");
					nStair = _wtoi(sStair.GetBuffer(0));
					sBlock.Replace(L"동", L"");

					sFloor = iterSCCR->sFloor;
					if (sFloor.Find(L"B") >= 0) {
						sFloor.Replace(L"B", L"");
						sFloor.Replace(L"F", L"");
						nFloor = -_wtoi(sFloor.GetBuffer(0));
					}
					else {
						if (iterSCCR->sFloor.Compare(strRooftop) == 0)	//옥탑층
						{
							nFloor = CCircuitBasicInfo::Instance()->m_nFloor + 1;
						}
						else
						{
							sFloor.Replace(L"F", L"");
							nFloor = _wtoi(sFloor.GetBuffer(0));
						}
					}

					nCircuitNo = iterSCCR->nLastCircuitNo + nCircuitNoAcc;

					//회로인덱스로 위치를 찾는다.
					pSI = CSaveManager::ie()->m_listSystem.GetAt(CSaveManager::ie()->m_listSystem.FindIndex(nFindIndex));

					//회로 정보를 찾은 위치에 채워 넣는다.
					wcscpy_s(pSI->szBDName, sBDName);
					wcscpy_s(pSI->szCircuitName, sCircuitName);
					wcscpy_s(pSI->szRoomName, sRoomName);
					wcscpy_s(pSI->szBlock, sBlock);
					pSI->nNo = nNo;
					pSI->nSystemNo = nSystemNo;
					pSI->nSystem = nSystem;
					pSI->nStair = nStair;
					pSI->nFloor = nFloor;
					pSI->nCircuitNo = nCircuitNo;

					//옥탑방 처리
					if (nFloor == CCircuitBasicInfo::Instance()->m_nFloor + 1)
						pSI->bRooftop = true;
					else
						pSI->bRooftop = false;
				}

			}
		}
	}
}

bool CSaveManager::IsVacantCircuit(int nCircuitNum)
{
	SYSTEM_INFO_* pInfo;
	pInfo = CSaveManager::ie()->m_listSystem.GetAt(CSaveManager::ie()->m_listSystem.FindIndex(nCircuitNum));
	CString strCircuitName = L"";
	strCircuitName = pInfo->szCircuitName;

	if (strCircuitName.Compare(L"") == 0)
		return true;
	else
		return false;
	
	return false;
}

bool CSaveManager::MoveCircuitInfo(int nCurrentCircuitNum, int nNewCircuitNum)
{
	SYSTEM_INFO_ *pInfoNew, *pInfoOld;
	int nSystem, nNewCircuit, nOldCircuit;

	if (!IsVacantCircuit(nNewCircuitNum))
		return false;

	pInfoOld = m_listSystem.GetAt(m_listSystem.FindIndex(nCurrentCircuitNum));
	nSystem = pInfoOld->nSystem;	//계통은 같음
	nOldCircuit = pInfoOld->nNo;

	pInfoNew = m_listSystem.GetAt(m_listSystem.FindIndex(nNewCircuitNum));
	nNewCircuit = pInfoNew->nNo;

	memcpy(pInfoNew, pInfoOld, sizeof(SYSTEM_INFO_));

	//중계기 번호는 없앰
	pInfoNew->nSystemNo = 0;
	
	memset(pInfoOld, 0, sizeof(SYSTEM_INFO_));

	//나중에 디버깅 위해 계통과 회로번호 넣음
	pInfoNew->nNo = nNewCircuit;
	pInfoNew->nSystem = nSystem;
	pInfoOld->nNo = nOldCircuit;
	pInfoOld->nSystem = nSystem;

	return true;
}