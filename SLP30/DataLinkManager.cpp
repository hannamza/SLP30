#include "stdafx.h"
#include "SLP30.h"
#include "DataLinkManager.h"
#include "MessagePopup.h"
#include "XLEzAutomation.h"
#include <AFXPRIV.H>
#include "CreateProcessReturnValue.h"

CMessagePopup* g_pWaitDataLink = NULL;
CDataLinkManager::CDataLinkManager()
{
	g_pWaitDataLink = NULL;
	m_bLcdW = new BYTE[ONE_MEGA_SIZE];
	m_nLcdLen = 30;
}


CDataLinkManager::~CDataLinkManager()
{
	SAFE_DELETE(g_pWaitDataLink);
	SAFE_DELETE(m_bLcdW);
}

void CDataLinkManager::MakeDataLink(SYSTEM_INFO_* pInfo)
{
}

bool CDataLinkManager::MakeDataLinkTotal()
{
	int nCount = CSaveManager::Instance()->m_listSystem.GetCount();
	SYSTEM_INFO_* pInfo = NULL;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pInfo = CSaveManager::Instance()->m_listSystem.GetAt(CSaveManager::Instance()->m_listSystem.FindIndex(nIndex));

		MakeDataLink(pInfo);
	}
	return true;
}

void CDataLinkManager::SetDirPath(CString sPath)
{
	if (sPath.Right(1) == L"\\") {
		m_sPath = sPath;
	}
	else {
		m_sPath.Format(L"%s\\", sPath);
	}
}

int CDataLinkManager::fileWrite(FILE* f, int dwSize, int nElement, BYTE* pBuf)
{
	if (!f) {
		return -1;
	}
	DWORD dwWrite = 0, dwWritten = 0, dwTime = GetTickCount();
	while (dwWritten != dwSize) {
		dwWrite = fwrite(&pBuf[dwWritten], nElement, dwSize - dwWritten, f);
		dwWritten += dwWrite;
		if (dwWrite <= 0) {
			break;
		}
		/*if (GetTickCount() - dwTime >= 2500) {
			return -2;
		}*/
	}
	return 0;
}

void CDataLinkManager::ReleaseExcelWaitPopup()
{
	if (g_pWaitDataLink) {
		g_pWaitDataLink->ShowWindow(SW_HIDE);
		SAFE_DELETE(g_pWaitDataLink);
	}
}

bool CDataLinkManager::MakeEmerData(FILE* f)
{
	CString sPath, strTxt, sFloor, sText, sBDName;
	sPath.Format(L"%sEMER00.ROM", m_sPath);

	BYTE empty[6000];
	memset(empty, 0x00, 6000);
	if (CSaveManager::Instance()->m_nBroadType == 2) {
		if (fileWrite(f, 6000, 1, empty) != 0) {
			fclose(f);
			::DeleteFile(sPath);
			return false;
		}
		fclose(f);
		return true;
	}
	BC_INFO* pInfo = NULL;
	UINT nPt = 6000;
	int nLen = 0;
	CStringA sContain;
	BYTE pBuffer[256 * 1024];
	memset(pBuffer, 0x00, 256 * 1024);
	INT nCount = CSaveManager::Instance()->m_listBC.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pInfo = CSaveManager::Instance()->m_listBC.GetAt(CSaveManager::Instance()->m_listBC.FindIndex(nIndex));
		if (wcslen(pInfo->BC_CONTAIN) == 0) {
			continue;
		}
		sContain = CCommonFunc::WCharToChar(pInfo->BC_CONTAIN);
		//nLen = (wcslen(pInfo->BC_CONTAIN) * sizeof(WCHAR)) + 1;
		//memcpy_s(&pBuffer[nPt], nLen, pInfo->BC_CONTAIN, nLen);
		nLen = sContain.GetLength() + 1;
		memcpy_s(&pBuffer[nPt], nLen, sContain.GetBuffer(0), nLen);

		pBuffer[nIndex * 2 + 0] = nPt / 0x100;
		pBuffer[nIndex * 2 + 1] = nPt % 0x100;

		nPt += nLen;
	}
	int nTemp = 256 - (nPt % 256);
	if (fileWrite(f, nPt + nTemp, 1, pBuffer) != 0) {
		fclose(f);
		return false;
	}
	return true;
}

bool CDataLinkManager::LcdContentsWriteCommon(FILE* f, CString sContent, long & lSize)
{
	fseek(f, 0L, SEEK_END);
	lSize = ftell(f);

	int nSize = (sContent.GetLength() + 1) * sizeof(WCHAR);
	memcpy_s(&m_bLcdW[m_nLcdLen], nSize, sContent.GetBuffer(0), nSize);
	m_nLcdLen += nSize;

	CStringA sValue = CCommonFunc::WCharToChar(sContent.GetBuffer(0));
	//fwrite(sContent.GetBuffer(0), sizeof(WCHAR), sContent.GetLength(), f);
	fwrite(sValue.GetBuffer(0), sizeof(CHAR), sValue.GetLength() + 1, f);

	return true;
}

bool CDataLinkManager::LcdContentsWrite(SYSTEM_INFO_* pInfo, FILE* f, long & lSize)
{
	fseek(f, 0L, SEEK_END);
	lSize = ftell(f);
	CString sFloor, strTxt, sText;
	if (pInfo->nFloor < 0) {
		sFloor.Format(L"B%dF", abs(pInfo->nFloor));
	}
	else {
		sFloor.Format(L"%dF", abs(pInfo->nFloor));
	}
	(wcslen(pInfo->szRoomName) > 0) ? strTxt.Format(L" %s ", pInfo->szRoomName) : strTxt = L" ";
	sText.Format(L"%s동 %d계단 %s%s%s", pInfo->szBlock, pInfo->nStair, sFloor, strTxt, pInfo->szCircuitName);
	if (pInfo->nCircuitNo > 0) {
		strTxt.Format(L"%s-%d", sText, pInfo->nCircuitNo);
	}
	else {
		strTxt.Format(L"%s ", sText);
	}
	int nSize = (strTxt.GetLength() + 1) * sizeof(WCHAR);
	memcpy_s(&m_bLcdW[m_nLcdLen], nSize, strTxt.GetBuffer(0), nSize);
	m_nLcdLen += nSize;

	CStringA sValue = CCommonFunc::WCharToChar(strTxt.GetBuffer(0));
	//fwrite(strTxt.GetBuffer(0), sizeof(WCHAR), strTxt.GetLength() + 1, f);
	fwrite(sValue.GetBuffer(0), sizeof(CHAR), sValue.GetLength() + 1, f);

	return true;
}

bool CDataLinkManager::MakeRedboxOPData(CWnd* pWnd)
{
	long lMainPt = 0x030000;
	long lLcdPt = 30;
	long lEmerPt = 0;
	ST_YEONDONG addr;
	long uTemp;
	FILE* OPf = NULL, *lcdf = NULL, *emerf = NULL, *mainf = NULL;
	CString sVersion, sMainPath, sOpPath, sLcdPath, sEmerPath, strTxt, strLcdTxt, sOpNewPath;
	BYTE empty[512], *buffer;
	CCreateProcessReturnValue createProcess;
	memset(empty, 0x00, 512);

	WCHAR szDocPath[2048];
	SHGetSpecialFolderPath(NULL, szDocPath, CSIDL_MYDOCUMENTS, FALSE);
	wcscat(szDocPath, L"\\SLP30");

	sEmerPath.Format(L"%s\\EMER00.ROM", szDocPath);
	emerf = _wfopen(sEmerPath.GetBuffer(0), L"w+b");
	if (!emerf) {
		return false;
	}
	// emergency
	if (!MakeEmerData(emerf)) {
		//fclose(emerf);
		return false;
	}
	lEmerPt = ftell(emerf);

	//sOpPath.Format(L"%sRedboX_OPData.rom", m_sPath);
	sMainPath.Format(L"%s\\Main.rom", szDocPath);
	mainf = _wfopen(sMainPath.GetBuffer(0), L"w+b");
	if (!mainf) {
		fclose(emerf);
		return false;
	}
	sLcdPath.Format(L"%s\\lcd.rom", szDocPath);
	lcdf = _wfopen(sLcdPath.GetBuffer(0), L"w+b");
	if (!lcdf) {
		fclose(mainf);
		fclose(emerf);
		return false;
	}
	fseek(mainf, 0L, SEEK_SET);
	fseek(lcdf, 0L, SEEK_SET);

	ST_MAINROM main;
	if (fileWrite(mainf, sizeof(ST_MAINROM), 1, (BYTE*)&main) != 0) {
		fclose(mainf);
		fclose(lcdf);
		fclose(emerf);
		return false;
	}

	buffer = new BYTE[ONE_MEGA_SIZE];
	fwrite(empty, 1, 30, lcdf);

	m_nLcdLen = 30;
	memset(m_bLcdW, 0x00, ONE_MEGA_SIZE);

	// circuit
	if (!WriteCircuitInfo(mainf, lcdf, lMainPt, lLcdPt)) {
		goto _ERROR_EXIT;
	}
	pWnd->PostMessage(ROM_SAVE_MSG, 0, 35);
	
	// p/s
	if (!WritePsInfo(mainf, lcdf, lMainPt, lLcdPt)) {
		goto _ERROR_EXIT;
	}
	pWnd->PostMessage(ROM_SAVE_MSG, 0, 40);

	// pump
	if (!WritePumpInfo(mainf, lcdf, lMainPt, lLcdPt)) {
		goto _ERROR_EXIT;
	}
	pWnd->PostMessage(ROM_SAVE_MSG, 0, 45);

	// pattern data
	if (!WritePatternInfo(mainf, lcdf, lMainPt, lLcdPt)) {
		goto _ERROR_EXIT;
	}
	pWnd->PostMessage(ROM_SAVE_MSG, 0, 50);

	fseek(mainf, 0L, SEEK_END);
	lMainPt = ftell(mainf);
	int nTemp = 256 - (lMainPt % 256);
	fwrite(empty, 1, nTemp, mainf);
	fseek(mainf, 0L, SEEK_END);
	lMainPt = ftell(mainf);

	// lcd save length
	fseek(lcdf, 0L, SEEK_END);
	lLcdPt = ftell(lcdf);

	nTemp = 256 - (m_nLcdLen % 256);
	m_nLcdLen += nTemp;

	addr.bt65536Divid = (m_nLcdLen / 0x10000) % 0x100;
	uTemp = (m_nLcdLen % 0x10000);
	addr.bt256DMod = uTemp / 0x100;
	addr.bt256Mod = uTemp % 0x100;
	fseek(lcdf, 16L, SEEK_SET);
	fwrite(&addr, 1, sizeof(ST_YEONDONG), lcdf);
	memcpy_s(&m_bLcdW[16], sizeof(ST_YEONDONG), &addr, sizeof(ST_YEONDONG));

	fseek(lcdf, 0L, SEEK_END);
	lLcdPt = ftell(lcdf);

	nTemp = 256 - (lLcdPt % 256);
	fwrite(empty, 1, nTemp, lcdf);
	lLcdPt = ftell(lcdf);

	sOpPath.Format(L"%s\\RedboX_OPData.rom", m_sPath);
	OPf = _wfopen(sOpPath.GetBuffer(0), L"w+b");
	if (!OPf) {
		goto _ERROR_EXIT;
	}

	fseek(OPf, 0L, SEEK_SET);
	fwrite(buffer, 1, 16, OPf);

	// 1. main start point
	nTemp = 16;
	fseek(OPf, 0L, SEEK_SET);
	fwrite(&nTemp, sizeof(int), 1, OPf);

	// 2. write main
	fseek(mainf, 0L, SEEK_SET);
	fseek(OPf, 0L, SEEK_END);
	DWORD dwRead = 0, dwReadden = 0;
	while (dwReadden != lMainPt) {
		dwRead = fread(&buffer[dwReadden], 1, lMainPt - dwReadden, mainf);
		fileWrite(OPf, dwRead, 1, &buffer[dwReadden]);
		dwReadden += dwRead;
	}

	// 3. lcd start point
	nTemp = 16 + lMainPt;
	fseek(OPf, 4L, SEEK_SET);
	fwrite(&nTemp, sizeof(int), 1, OPf);

	// 4. write lcd
	fseek(OPf, 0L, SEEK_END);

	/*fseek(lcdf, 0L, SEEK_SET);
	fseek(OPf, 0L, SEEK_END);
	dwRead = 0, dwReadden = 0;
	memset(buffer, 0x00, ONE_MEGA_SIZE);
	while (dwReadden != lLcdPt) {
		dwRead = fread(&buffer[dwReadden], 1, lLcdPt - dwReadden, lcdf);
		dwReadden += dwRead;
	}*/

	fileWrite(OPf, m_nLcdLen, 1, (BYTE*)m_bLcdW);
	lLcdPt = m_nLcdLen;

	// 5. emer start point
	nTemp = 16 + lMainPt + lLcdPt;
	fseek(OPf, 8L, SEEK_SET);
	fwrite(&nTemp, sizeof(int), 1, OPf);

	// 6. version start point
	fseek(emerf, 0L, SEEK_END);
	lEmerPt = ftell(emerf);
	nTemp = 16 + lMainPt + lLcdPt + lEmerPt;
	fseek(OPf, 12L, SEEK_SET);
	fwrite(&nTemp, sizeof(int), 1, OPf);

	// 7. write emer
	fseek(emerf, 0L, SEEK_SET);
	fseek(OPf, 0L, SEEK_END);
	dwRead = 0, dwReadden = 0;
	while (dwReadden != lEmerPt) {
		dwRead = fread(&buffer[dwReadden], 1, lEmerPt - dwReadden, emerf);
		fileWrite(OPf, dwRead, 1, &buffer[dwReadden]);
		dwReadden += dwRead;
	}

	// 8. version
	CTime time = CTime::GetCurrentTime();
	sVersion.Format(L"Ver %02d.%02d %04d/%02d/%02d %02d:%02d:%02d %s", 
		CSaveManager::ie()->m_versionDate.cMajorVer, CSaveManager::ie()->m_versionDate.cMinorVer, 
		time.GetYear(), time.GetMonth(), time.GetDay(),
		time.GetHour(), time.GetMinute(), time.GetSecond(), CCommonState::Instance()->m_sUserID.GetBuffer(0));
	fwrite(sVersion.GetBuffer(0), sizeof(WCHAR), sVersion.GetLength() +1, OPf);

	fclose(mainf);
	fclose(lcdf);
	fclose(emerf);
	fclose(OPf);

	// 서버로 파일을 전송한다.
	CCommonState::ie()->m_nReturnValue = -99;
	if (!createProcess.SendDataToWebdav(sOpPath, L"RedboX_OPData.rom", CCommonState::ie()->m_sUserID)) {
		::DeleteFile(sOpPath);
		pWnd->PostMessage(ROM_SAVE_MSG, 2, 0);
		goto _ERROR_EXIT;
	}
	while (CCommonState::ie()->m_nReturnValue == -99) {
		Sleep(50);
	}
	if (CCommonState::ie()->m_nReturnValue != 0) {
		::DeleteFile(sOpPath);
		pWnd->PostMessage(ROM_SAVE_MSG, 2, 0);
		goto _ERROR_EXIT;
	}
	//int ddd = CCommonState::ie()->m_nReturnValue;
	pWnd->PostMessage(ROM_SAVE_MSG, 0, 95);

	::DeleteFile(sMainPath);
	::DeleteFile(sLcdPath);
	::DeleteFile(sEmerPath);

	/*sOpNewPath.Format(L"%sRedboX_OPData.rom", m_sPath);
	CFile::Rename(sOpPath.GetBuffer(0), sOpNewPath.GetBuffer(0));
	::DeleteFile(sOpPath);*/

	pWnd->PostMessage(ROM_SAVE_MSG, 0, 100);

	SAFE_DELETE(buffer);
	return true;

_ERROR_EXIT:

	fclose(mainf);
	fclose(lcdf);
	fclose(emerf);

	::DeleteFile(sMainPath);
	::DeleteFile(sLcdPath);
	::DeleteFile(sEmerPath);

	SAFE_DELETE(buffer);
	return false;
}

bool CDataLinkManager::WriteCircuitInfo(FILE* mainf, FILE* lcdf, long & lMainPt, long & lLcdPt)
{
	SYSTEM_INFO_* pInfo = NULL;
	int nCount = CSaveManager::Instance()->m_listSystem.GetCount();
	int nCircuitIndex = 0;
	int nInput, nOutput, nTemp, nLcdAddr;
	ST_CIRCUIT info;
	CUIntArray system, pattern;
	BYTE btCmd = 0, btDiv, btRest;
	const int nLinkAdderSize = 193536;
	BYTE buffer[nLinkAdderSize];
	BYTE linkAddr[nLinkAdderSize];
	ST_YEONDONG addr;
	bool bUse = true;
	memset(linkAddr, 0x00, nLinkAdderSize);
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		if (nIndex >= 511) {
			continue;
		}
		// get equip item
		bUse = true;
		pInfo = CSaveManager::Instance()->m_listSystem.GetAt(CSaveManager::Instance()->m_listSystem.FindIndex(nIndex));
		if (wcslen(pInfo->szCircuitName) == 0) {
			continue;
			//bUse = false;
		}
		// get equip index
		nCircuitIndex = CCircuitBasicInfo::Instance()->GetIndexCircuitName(pInfo->szCircuitName);
		if (nCircuitIndex == -1) {
			continue;
		}
		system.RemoveAll();
		pattern.RemoveAll();
		// get link data
		if (bUse) {
			CPatternManager::Instance()->GetPattern(pInfo, pattern, system);
		}
		/*if (pattern.GetCount() == 0 && system.GetCount() == 0) {
			continue;
		}*/
		// write lcd data
		fseek(lcdf, lLcdPt, SEEK_SET);
		if (_tcslen(pInfo->szCircuitName) > 0) {
			LcdContentsWrite(pInfo, lcdf, lLcdPt); // and get lcd addr
			nLcdAddr = lLcdPt;
			lLcdPt = ftell(lcdf);
		} else {
			nLcdAddr = 0;
		}

		memset(&info, 0x00, sizeof(ST_CIRCUIT));
		// get input type
		nInput = CCircuitBasicInfo::Instance()->GetCircuitInputNo(pInfo->szCircuitName);
		if (nInput > 0) {
			info.inputType = nInput;
		}
		// get output type
		nOutput = CCircuitBasicInfo::Instance()->GetCircuitOutputNo(pInfo->szCircuitName);
		if (nOutput > 0) {
			info.outputType = nOutput;
		}
		if (!bUse) {
			info.inputType = 0;
			info.outputType = 0;
		}

		// circuit info
		info.count = pattern.GetCount();
		info.count += system.GetCount();
		// lcd addr
		info.msgAddr.bt65536Divid = nLcdAddr / 0x10000;
		info.msgAddr.bt256DMod = ((nLcdAddr) / 0x100) % 0x100;
		info.msgAddr.bt256Mod = (nLcdAddr) % 0x100;

		// circuit write
		fseek(mainf, lMainPt, SEEK_SET);
		fwrite(&info, 1, sizeof(ST_CIRCUIT), mainf);

		int nPCnt = 0;
		memset(buffer, 0x00, nLinkAdderSize);

		// equip
		btCmd = 0x00;
		for (int i = 0; i < system.GetCount(); i++) {
			nTemp = system.GetAt(i);

			btDiv = nTemp / 256;
			btRest = nTemp % 256;
			buffer[(nPCnt * 3)] = btCmd;
			buffer[(nPCnt * 3) + 1] = btDiv;
			buffer[(nPCnt * 3) + 2] = btRest;
			++nPCnt;
		}
		btCmd = 0x80;
		// link data
		for (int i = 0; i < pattern.GetCount(); i++) {
			nTemp = pattern.GetAt(i);

			btDiv = nTemp / 256;
			btRest = nTemp % 256;
			buffer[nPCnt * 3] = btCmd;
			buffer[nPCnt * 3 + 1] = btDiv;
			buffer[nPCnt * 3 + 2] = btRest;
			++nPCnt;
		}
		// write link data
		if (nPCnt > 0) {
			fwrite(&buffer, 1, nPCnt * 3 + 1, mainf);
		}

		// save link data addr
		addr.bt65536Divid = (lMainPt) / 0x10000;
		addr.bt256DMod = ((lMainPt) / 0x100) % 0x100;
		addr.bt256Mod = (lMainPt) % 0x100;
		memcpy_s(&linkAddr[3 + (nIndex * 3)/* + (pInfo->nSystem * 256 * 3)*/], sizeof(ST_YEONDONG), &addr, sizeof(ST_YEONDONG));

		lMainPt = ftell(mainf);
	}
	fseek(mainf, 0L, SEEK_SET);
	// write link data addr
	if (fileWrite(mainf, nLinkAdderSize, 1, linkAddr) < 0) {
		return false;
	}

	fseek(mainf, 0L, SEEK_END);
	lMainPt = ftell(mainf);

	return true;
}

bool CDataLinkManager::WritePsInfo(FILE* mainf, FILE* lcdf, long & lMainPt, long & lLcdPt)
{
	const int nLinkAdderSize = 1024;
	const int nPsPt = 0x02F400;
	BYTE linkAddr[nLinkAdderSize];
	CString strLcdTxt;
	int nCount = 3;
	ST_YEONDONG addr;
	ST_CIRCUIT info;
	memset(linkAddr, 0x00, nLinkAdderSize);
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		if (CSaveManager::Instance()->m_PSInfo.use[nIndex] == 0) {
			continue;
		}
		switch (nIndex) {
		case 0: strLcdTxt = L"옥내소화전 주펌프 입력 P/S 확인"; break;
		case 1: strLcdTxt = L"옥내소화전 예비펌프 입력 P/S 확인"; break;
		case 2: strLcdTxt = L"옥내소화전 보조펌프 입력 P/S 확인"; break;
		default: continue;
		}
		LcdContentsWriteCommon(lcdf, strLcdTxt, lLcdPt);
		/*if (CSaveManager::Instance()->m_PSInfo.use[nIndex] == 0) {
			info.count = 0;
			info.inputType = 0;
			info.outputType = 0;
			info.msgAddr.bt65536Divid = (lLcdPt) / 0x10000;
			info.msgAddr.bt256DMod = ((lLcdPt) / 0x100) % 0x100;
			info.msgAddr.bt256Mod = (lLcdPt) % 0x100;
		}
		else {*/
			info.count = 0;
			info.inputType = 1;
			info.outputType = 0;
			info.msgAddr.bt65536Divid = (lLcdPt) / 0x10000;
			info.msgAddr.bt256DMod = ((lLcdPt) / 0x100) % 0x100;
			info.msgAddr.bt256Mod = (lLcdPt) % 0x100;
		//}
		addr.bt65536Divid = (lMainPt) / 0x10000;
		addr.bt256DMod = ((lMainPt) / 0x100) % 0x100;
		addr.bt256Mod = (lMainPt) % 0x100;
		memcpy_s(&linkAddr[3 + (nIndex * 3)], sizeof(ST_YEONDONG), &addr, sizeof(ST_YEONDONG));

		fseek(mainf, lMainPt, SEEK_SET);
		fwrite(&info, 1, sizeof(ST_CIRCUIT), mainf);

		lLcdPt = ftell(lcdf);
		lMainPt = ftell(mainf);
	}
	fseek(mainf, nPsPt, SEEK_SET);
	// write link data addr
	if (fileWrite(mainf, 768, 1, linkAddr) < 0) {
		return false;
	}

	fseek(mainf, 0L, SEEK_END);
	lMainPt = ftell(mainf);

	return true;
}

bool CDataLinkManager::WritePumpInfo(FILE* mainf, FILE* lcdf, long & lMainPt, long & lLcdPt)
{
	const int nPump = 0x02F700;
	const int nLinkAdderSize = 1024;
	BYTE linkAddr[nLinkAdderSize];
	CString strLcdTxt;
	int nCount = 4;
	ST_YEONDONG addr;
	ST_CIRCUIT info;
	int nPCnt = 0, nTemp;
	BYTE btCmd = 0, btDiv, btRest;
	CUIntArray system;
	BYTE buffer[1024 * 10];
	memset(buffer, 0x00, 1024 * 10);
	memset(linkAddr, 0x00, nLinkAdderSize);
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		if (CSaveManager::Instance()->m_pumpInfo.use[nIndex] == 0) {
			continue;
		}
		switch (nIndex) {
		case 0: strLcdTxt = L"옥내소화전 주펌프 기동확인"; break;
		case 1: strLcdTxt = L"옥내소화전 예비펌프 기동확인"; break;
		case 2: strLcdTxt = L"옥내소화전 보조펌프 기동확인"; break;
		case 3: strLcdTxt = L"비상발전기 기동확인"; break;
		default: continue;
		}
		CPatternManager::ie()->GetLamp(system);
		LcdContentsWriteCommon(lcdf, strLcdTxt, lLcdPt);
		/*if (CSaveManager::Instance()->m_pumpInfo.use[nIndex] == 0) {
			info.count = 0;
			info.inputType = 0;
			info.outputType = 0;
			info.msgAddr.bt65536Divid = (lLcdPt) / 0x10000;
			info.msgAddr.bt256DMod = ((lLcdPt) / 0x100) % 0x100;
			info.msgAddr.bt256Mod = (lLcdPt) % 0x100;
		}
		else {*/
			info.count = system.GetCount();;
			info.inputType = 1;
			info.outputType = 0;
			info.msgAddr.bt65536Divid = (lLcdPt) / 0x10000;
			info.msgAddr.bt256DMod = ((lLcdPt) / 0x100) % 0x100;
			info.msgAddr.bt256Mod = (lLcdPt) % 0x100;
		//}
		addr.bt65536Divid = (lMainPt) / 0x10000;
		addr.bt256DMod = ((lMainPt) / 0x100) % 0x100;
		addr.bt256Mod = (lMainPt) % 0x100;
		memcpy_s(&linkAddr[3 + (nIndex * 3)], sizeof(ST_YEONDONG), &addr, sizeof(ST_YEONDONG));

		fseek(mainf, lMainPt, SEEK_SET);
		fwrite(&info, 1, sizeof(ST_CIRCUIT), mainf);

		if (CSaveManager::Instance()->m_pumpInfo.use[nIndex] == 1) {
			// equip
			btCmd = 0x00;
			for (int i = 0; i < system.GetCount(); i++) {
				nTemp = system.GetAt(i);

				btDiv = nTemp / 256;
				btRest = nTemp % 256;
				buffer[(nPCnt * 3)] = btCmd;
				buffer[(nPCnt * 3) + 1] = btDiv;
				buffer[(nPCnt * 3) + 2] = btRest;
				++nPCnt;
			}
			// write link data
			if (nPCnt > 0) {
				fwrite(&buffer, 1, nPCnt * 3 + 1, mainf);
			}
		}
		lLcdPt = ftell(lcdf);
		lMainPt = ftell(mainf);
	}
	fseek(mainf, nPump, SEEK_SET);
	// write link data addr
	if (fileWrite(mainf, 768, 1, linkAddr) < 0) {
		return false;
	}

	fseek(mainf, 0L, SEEK_END);
	lMainPt = ftell(mainf);

	return true;
}

bool CDataLinkManager::WritePatternInfo(FILE* mainf, FILE* lcdf, long & lMainPt, long & lLcdPt)
{
	const int nLinkAdderSize = 2048;
	BYTE buffer[nLinkAdderSize];
	BYTE linkAddr[nLinkAdderSize];

	BYTE btCmd = 0, btDiv, btRest;
	int nCount = CPatternManager::Instance()->GetPatternCount();
	memset(linkAddr, 0x00, nLinkAdderSize);
	long lPtnPt = 0x02FA00;
	const int nPtnSize = 1536;
	int nPCnt = 0, nTemp = 0;
	SYSTEM_INFO_* pEquip;
	ST_YEONDONG addr;
	CString sTemp;
	pPatternInfo pPtn = NULL;
	// loop pattern count
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		// get pattern info
		pPtn = NULL;
		pPtn = CPatternManager::Instance()->GetPatternInfo(nIndex);
		//CPatternManager::Instance()->GetPatternList(nIndex + 1, system, broad);

		nPCnt = 0;
		memset(buffer, 0x00, nLinkAdderSize);
		buffer[0] = pPtn->arrayPattern.GetCount() + pPtn->arrayBroad.GetCount();
		//TRACE(L"PATTERN %d ====> count: %d\n", nIndex + 1, pPtn->arrayPattern.GetCount() + pPtn->arrayBroad.GetCount());
		btCmd = 0x00;
		for (int i = 0; i < pPtn->arrayPattern.GetCount(); i++) {
			sTemp = pPtn->arrayPattern.GetAt(i);
			nTemp = _wtoi(sTemp.GetBuffer(0));

			btDiv = nTemp / 256;
			btRest = nTemp % 256;

			buffer[1 + (nPCnt * 3)] = btCmd;
			buffer[1 + (nPCnt * 3) + 1] = btDiv;
			buffer[1 + (nPCnt * 3) + 2] = btRest;

			++nPCnt;
		}
		btCmd = 0xC0;
		for (int i = 0; i < pPtn->arrayBroad.GetCount(); i++) {
			sTemp = pPtn->arrayBroad.GetAt(i);
			nTemp = _wtoi(sTemp.GetBuffer(0));

			btDiv = nTemp / 256;
			btRest = nTemp % 256;

			buffer[1 + (nPCnt * 3)] = btCmd;
			buffer[1 + (nPCnt * 3) + 1] = btDiv;
			buffer[1 + (nPCnt * 3) + 2] = btRest;

			++nPCnt;
		}
		// save pattern addr
		addr.bt65536Divid = (lMainPt) / 0x10000;
		addr.bt256DMod = ((lMainPt) / 0x100) % 0x100;
		addr.bt256Mod = (lMainPt) % 0x100;

		memcpy_s(&linkAddr[3 + (nIndex * 3)], sizeof(ST_YEONDONG), &addr, sizeof(ST_YEONDONG));

		fileWrite(mainf, 1 + (nPCnt * 3), 1, buffer);
		//fwrite(buffer, 1, 1 + (nPCnt * 3), mainf); // write 
		lMainPt = ftell(mainf);
	}
	fseek(mainf, lPtnPt, SEEK_SET);
	//fwrite(linkAddr, 1, nPtnSize, mainf);
	fileWrite(mainf, nPtnSize, 1, linkAddr);

	fseek(mainf, 0L, SEEK_END);
	lMainPt = ftell(mainf);

	return true;
}

bool CDataLinkManager::MakeCRTData()
{
	FILE* f = NULL;
	CString sPath, strTxt, sFloor, sText, sBDName;
	sPath.Format(L"%sCRTDATA.txt", m_sPath);
	f = _wfopen(sPath.GetBuffer(0), L"wb");
	if (!f) {
		return false;
	}
	SYSTEM_INFO_* pInfo = NULL;
	int nCount = CSaveManager::Instance()->m_listSystem.GetCount();
	int nCircuitIndex = 0;
	CStringA sTemp, sName;
	int nInput, nOutput;

	fseek(f, 30, SEEK_SET);
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pInfo = CSaveManager::Instance()->m_listSystem.GetAt(CSaveManager::Instance()->m_listSystem.FindIndex(nIndex));
		if (wcslen(pInfo->szCircuitName) > 0) {
			if (pInfo->nFloor < 0) {
				sFloor.Format(L"B%dF", abs(pInfo->nFloor));
			}
			else {
				sFloor.Format(L"%dF", abs(pInfo->nFloor));
			}
			nInput = CCircuitBasicInfo::Instance()->GetCircuitInputNo(pInfo->szCircuitName);
			nOutput = CCircuitBasicInfo::Instance()->GetCircuitOutputNo(pInfo->szCircuitName);

			sText.Format(L"%02d%02d%d%03d,%02d,%02d,%s %s %s\n", 
				0, 0, pInfo->nSystem, pInfo->nSystemNo, nInput, nOutput, 
				pInfo->szBlock, sFloor, pInfo->szCircuitName);

			sTemp = CCommonFunc::WCharToChar(strTxt.GetBuffer(0));
			fwrite(sTemp.GetBuffer(0),sizeof(CHAR),  sTemp.GetLength(), f);
		}
	}
	// ps 유닛 : 63 계통 : 펌프 1, ps 0 회로번호 : 번호
	for (int nIndex = 0; nIndex < 3; nIndex++) {
		if (CSaveManager::Instance()->m_PSInfo.use[nIndex] == 0) {
			continue;
		}
		switch (nIndex) {
		case 0: sName = L"주펌프 입력"; break; //압력스위치 01
		case 1: sName = L"예비펌프 입력"; break; //압력스위치 02
		case 2: sName = L"보조펌프 입력"; break; //압력스위치 03
		default: break;
		}
		sText.Format(L"%02d%02d%d%03d,%02d,%02d,%s\n", 0, 63, 0, nIndex+1, 1, 0, sName);

		sTemp = CCommonFunc::WCharToChar(strTxt.GetBuffer(0));
		fwrite(sTemp.GetBuffer(0), sizeof(CHAR), sTemp.GetLength(), f);
	}
	// pump 유닛 : 63 계통 : 펌프 1, ps 0 회로번호 : 번호
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		if (CSaveManager::Instance()->m_pumpInfo.use[nIndex] == 0) {
			continue;
		}
		nInput = 2;
		switch (nIndex) {
		case 0: sName = L"주펌프"; break;
		case 1: sName = L"예비펌프"; break;
		case 2: sName = L"보조펌프"; break;
		case 3: sName = L"비상발전기"; nInput = 3; break;
		default: break;
		}
		sText.Format(L"%02d%02d%d%03d,%02d,%02d,%s\n", 0, 63, 1, nIndex + 1, nInput, 0, sName);

		sTemp = CCommonFunc::WCharToChar(strTxt.GetBuffer(0));
		fwrite(sTemp.GetBuffer(0), sizeof(CHAR), sTemp.GetLength(), f);
	}

	fclose(f);

	return true;
}

bool CDataLinkManager::MakeFloor()
{
	FILE* f = NULL;
	CString sPath;
	sPath.Format(L"%sFLOOR.txt", m_sPath);
	f = _wfopen(sPath.GetBuffer(0), L"wb");
	if (!f) {
		return false;
	}
	fclose(f);

	return true;
}

CString CDataLinkManager::GetCircuitNo(int nNum)
{
	CString sTemp;
	if (nNum <= 256) {
		sTemp.Format(L"%d", nNum);
	}
	else {
		int nValue = nNum % 256;
		sTemp.Format(L"1%03d", nValue);
	}
	return sTemp;
}

int CDataLinkManager::MakeExcelData(CString sPath)
{
	WCHAR wszPath[2048];
	GetModuleFileName(NULL, wszPath, 2048);
	PathRemoveFileSpec(wszPath);
	CString sSamplePath;
	sSamplePath.Format(L"%s\\sample2.xlsx", wszPath);

	if (sSamplePath.CompareNoCase(sPath) == 0) {
		CMessagePopup popup(L"Excel 정보 생성", L"\n\n해당 파일은 프로그램 파일입니다.\n\n다른 경로를 선택하여 주십시오", MB_OK, CCommonState::Instance()->m_pWnd);
		UINT nResult = popup.DoModal();
		return -2;
	}

	if (::PathFileExists(sPath)) {
		CMessagePopup popup(L"Excel 정보 생성", L"\n\n이미 같은 이름의 파일이 존재합니다.\n\n덮어쓰기 하시겠습니까?", MB_YESNO, CCommonState::Instance()->m_pWnd);
		UINT nResult = popup.DoModal();
		if (nResult != IDOK) {
			return -3;
		}
		if (::PathFileExists(sPath + L".tmp")) {
			::DeleteFile(sPath + L".tmp");
		}
		HANDLE hHandle = CreateFile(sPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hHandle == INVALID_HANDLE_VALUE) {
			CMessagePopup popup(L"Excel 정보 생성", L"\n\n파일 저장에 실패하였습니다.\n\n이미 해당 파일이 사용중입니다.", MB_OK, CCommonState::Instance()->m_pWnd);
			UINT nResult = popup.DoModal();
			return -4;
		}
		CloseHandle(hHandle);

		CFile::Rename(sPath, sPath + L".tmp");
		if (::PathFileExists(sPath)) {
			CMessagePopup popup(L"Excel 정보 생성", L"\n\n파일 저장에 실패하였습니다.\n\n파일을 사용중이거나 권한 문제일 수 있습니다.", MB_OK, CCommonState::Instance()->m_pWnd);
			UINT nResult = popup.DoModal();
			CFile::Rename(sPath + L".tmp", sPath);
			return -5;
		}
	}
	::DeleteFile(sPath + L".tmp2");
	::CopyFile(sSamplePath, sPath + L".tmp2", true);

	CRect rect, rt, rect1;
	CCommonState::Instance()->m_pWnd->GetClientRect(&rect);
	CCommonState::Instance()->m_pWnd->ClientToScreen(&rect);
	if (g_pWaitDataLink) {
		SAFE_DELETE(g_pWaitDataLink);
	}

	if (g_pWaitDataLink) {
		SAFE_DELETE(g_pWaitDataLink);
	}
	g_pWaitDataLink = new CMessagePopup(L"Excel 정보 생성", L"\n\n\nExcel 정보를 생성중입니다. \n\n잠시만 기다려 주세요.", -1, CCommonState::Instance()->m_pWnd);
	g_pWaitDataLink->Create(IDD_COMMON_POPUP_DIALOG);
	g_pWaitDataLink->ShowWindow(SW_HIDE);

	g_pWaitDataLink->GetClientRect(&rt);
	g_pWaitDataLink->ClientToScreen(&rt);

	rect1.CopyRect(&rect);
	rect1.top += rect.Height() / 2 - rt.Height() / 2;
	rect1.left += rect.Width() / 2 - rt.Width() / 2;
	rect1.bottom = rect1.top + rt.Height();
	rect1.right = rect1.left + rt.Width();
	g_pWaitDataLink->MoveWindow(&rect1);
	g_pWaitDataLink->ShowWindow(SW_SHOW);

	g_pWaitDataLink->SetForegroundWindow();
	::SetWindowPos(g_pWaitDataLink->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	g_pWaitDataLink->SetCaptionAddBottom(L"                파일 생성중입니다.               ");

	int nTotalCount = CSaveManager::Instance()->m_listSystem.GetCount() + CPatternManager::Instance()->GetPatternCount() + CSaveManager::Instance()->m_listBC.GetCount() + 7;
	int nCurrentCount = 1;

	SYSTEM_INFO_* pInfo = NULL;
	CString sInput, sOutput, sTemp;
	int nCount = CSaveManager::Instance()->m_listSystem.GetCount();
	int nCircuitIndex = 0;
	CStringArray system, pattern;

	CXLEzAutomation XL(FALSE);
	XL.OpenExcelFile(sPath + L".tmp2");

	bool bBlock = false;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		// get equip item
		pInfo = CSaveManager::Instance()->m_listSystem.GetAt(CSaveManager::Instance()->m_listSystem.FindIndex(nIndex));
		if (wcslen(pInfo->szCircuitName) == 0) {
			sTemp.Format(L"%d / %d (전체/완료)", nTotalCount, nCurrentCount);
			++nCurrentCount;
			g_pWaitDataLink->SetCaptionAddBottom(sTemp);
			continue;
		}
		// get equip index
		nCircuitIndex = CCircuitBasicInfo::Instance()->GetIndexCircuitName(pInfo->szCircuitName);
		if (nCircuitIndex == -1) {
			sTemp.Format(L"%d / %d (전체/완료)", nTotalCount, nCurrentCount);
			++nCurrentCount;
			g_pWaitDataLink->SetCaptionAddBottom(sTemp);
			continue;
		}
		bBlock = false;
		if (wcslen(pInfo->szBlock) > 0) {
			bBlock = true;
		}
		// get input type
		sInput = CCircuitBasicInfo::Instance()->GetCircuitInput(nCircuitIndex);
		XL.SetCellValue(3, 2 + (nIndex * 2), sInput);
		// get output type
		sOutput = CCircuitBasicInfo::Instance()->GetCircuitOutput(nCircuitIndex);
		XL.SetCellValue(4, 2 + (nIndex * 2), sOutput);
		if (bBlock) {
			sTemp.Format(L"%s동%d계단 %s", pInfo->szBlock, pInfo->nStair, pInfo->szCircuitName);
			XL.SetCellValue(5, 2 + (nIndex * 2), sTemp);
			sTemp.Format(L"%s동 %d계단", pInfo->szBlock, pInfo->nStair);
			XL.SetCellValue(27, 2 + (nIndex * 2), sTemp);
		}
		else if (CCircuitBasicInfo::Instance()->m_nStair <= 1) {
			sTemp.Format(L"%s", pInfo->szCircuitName);
			XL.SetCellValue(5, 2 + (nIndex * 2), sTemp);
		}
		else {
			sTemp.Format(L"%d계단 %s", pInfo->nStair, pInfo->szCircuitName);
			XL.SetCellValue(5, 2 + (nIndex * 2), sTemp);
			sTemp.Format(L"%d계단", pInfo->nStair);
			XL.SetCellValue(27, 2 + (nIndex * 2), sTemp);
		}
		sTemp.Format(L"%s%dF", (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor));
		XL.SetCellValue(28, 2 + (nIndex * 2), sTemp);
		XL.SetCellValue(29, 2 + (nIndex * 2), pInfo->szRoomName);
		XL.SetCellValue(30, 2 + (nIndex * 2), pInfo->szCircuitName);

		(wcslen(pInfo->szRoomName) > 0) ? sInput.Format(L" %s ", pInfo->szRoomName) : sInput = L" ";
		if (bBlock) {
			sTemp.Format(L"%s동 %d계단 %s%dF%s%s", pInfo->szBlock, pInfo->nStair, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor), sInput, pInfo->szCircuitName);
		}
		else if (CCircuitBasicInfo::Instance()->m_nStair <= 1) {
			sTemp.Format(L"%s%dF%s%s", (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor), sInput, pInfo->szCircuitName);
		}
		else {
			sTemp.Format(L"%d계단 %s%dF%s%s", pInfo->nStair, (pInfo->nFloor < 0) ? L"B" : L"", abs(pInfo->nFloor), sInput, pInfo->szCircuitName);
		}
		XL.SetCellValue(32, 2 + (nIndex * 2), sTemp);

		// get pattern
		pattern.RemoveAll();
		system.RemoveAll();
		CPatternManager::Instance()->GetPattern(pInfo, pattern, system);
		for (int i = 0; i < pattern.GetCount(); i++) {
			sTemp = pattern.GetAt(i);
			XL.SetCellValue(7 + i, 2 + (nIndex * 2), sTemp);
		}
		for (int i = 0; i < system.GetCount(); i++) {
			sInput = system.GetAt(i);
			sTemp = GetCircuitNo(_wtoi(sInput.GetBuffer(0)));//system.GetAt(i);
			XL.SetCellValue(7 + i, 3 + (nIndex * 2), sTemp);
		}

		sTemp.Format(L"%d / %d (전체/완료)", nTotalCount, nCurrentCount);
		++nCurrentCount;
		g_pWaitDataLink->SetCaptionAddBottom(sTemp);
	}
	XL.ChangeWorksheet(2);
	nCount = CPatternManager::Instance()->GetPatternCount();
	pPatternInfo pPtn;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pPtn = CPatternManager::Instance()->GetPatternInfo(nIndex);
		if (!pPtn) {
			sTemp.Format(L"%d / %d (전체/완료)", nTotalCount, nCurrentCount);
			++nCurrentCount;
			g_pWaitDataLink->SetCaptionAddBottom(sTemp);
			continue;
		}
		XL.SetCellValue(2, 2 + (nIndex * 2), pPtn->sPatternName);
		for (int i = 0; i < pPtn->arrayPattern.GetCount(); i++) {
			sInput = pPtn->arrayPattern.GetAt(i);
			sTemp = GetCircuitNo(_wtoi(sInput.GetBuffer(0)));
			XL.SetCellValue(4 + i, 3 + (nIndex * 2), sTemp);
		}
		for (int i = 0; i < pPtn->arrayBroad.GetCount(); i++) {
			XL.SetCellValue(4 + i, 2 + (nIndex * 2), L"A" + pPtn->arrayBroad.GetAt(i));
		}

		sTemp.Format(L"%d / %d (전체/완료)", nTotalCount, nCurrentCount);
		++nCurrentCount;
		g_pWaitDataLink->SetCaptionAddBottom(sTemp);
	}

	XL.ChangeWorksheet(3);
	BC_INFO* pBc;
	nCount = CSaveManager::Instance()->m_listBC.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pBc = CSaveManager::Instance()->m_listBC.GetAt(CSaveManager::Instance()->m_listBC.FindIndex(nIndex));
		sTemp.Format(L"%s동 %s%dF", pBc->szBlock, (pBc->nFloor < 0) ? L"B" : L"", abs(pBc->nFloor));
		XL.SetCellValue(2, 3 + nIndex, sTemp);
		XL.SetCellValue(3, 3 + nIndex, pBc->BC_CONTAIN);

		sTemp.Format(L"%d / %d (전체/완료)", nTotalCount, nCurrentCount);
		++nCurrentCount;
		g_pWaitDataLink->SetCaptionAddBottom(sTemp);
	}
	XL.ChangeWorksheet(5);
	int nTempIndex = 0;

	for (int nIndex = 0; nIndex < 3; nIndex++) {
		if (CSaveManager::Instance()->m_PSInfo.use[nIndex] == 1) {
			switch (nIndex) {
			case 0:
			case 1:
				XL.SetCellValue(3, 2 + (nTempIndex * 2), L"감시지속");
				break;
			case 2:
				XL.SetCellValue(3, 2 + (nTempIndex * 2), L"감시비지속");
				break;
			default: continue;
			}
			XL.SetCellValue(26, 2 + (nTempIndex * 2), L"옥내소화전");
			//sTemp.Format(L"압력스위치 %02d", nIndex + 1);
			switch (nIndex) {
			case 0: sOutput = L"주펌프 입력"; break;
			case 1: sOutput = L"예비펌프 입력"; break;
			case 2: sOutput = L"보조펌프 입력"; break;
			default: continue;
			}
			XL.SetCellValue(27, 2 + (nTempIndex * 2), sOutput);
			XL.SetCellValue(28, 2 + (nTempIndex * 2), L"P/S");
			XL.SetCellValue(29, 2 + (nTempIndex * 2), L"확인");
			sTemp.Format(L"옥내소화전 %s P/S 확인", sOutput);
			XL.SetCellValue(31, 2 + (nTempIndex * 2), sTemp);
			++nTempIndex;
		}
		sTemp.Format(L"%d / %d (전체/완료)", nTotalCount, nCurrentCount);
		++nCurrentCount;
		g_pWaitDataLink->SetCaptionAddBottom(sTemp);
	}
	XL.ChangeWorksheet(6);
	nTempIndex = 0;
	system.RemoveAll();
	CPatternManager::ie()->GetLamp(system);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		if (CSaveManager::Instance()->m_pumpInfo.use[nIndex] == 1) {
			switch (nIndex) {
			case 0: XL.SetCellValue(3, 2 + (nTempIndex * 2), L"주펌프 입력"); break;
			case 1: XL.SetCellValue(3, 2 + (nTempIndex * 2), L"예비펌프 입력"); break;
			case 2: XL.SetCellValue(3, 2 + (nTempIndex * 2), L"보조펌프 입력"); break;
			case 3: XL.SetCellValue(3, 2 + (nTempIndex * 2), L"비상발전기 전원"); break;
			default: break;
			}
			XL.SetCellValue(26, 2 + (nTempIndex * 2), L"옥내소화전");
			for (int i = 0; i < system.GetCount(); i++) {
				sOutput = system.GetAt(i);
				XL.SetCellValue(6 + i, 2 + (nTempIndex * 2), sOutput);
			}
			switch (nIndex) {
			case 0:	sInput = L"주펌프 "; break;
			case 1:	sInput = L"예비펌프 "; break;
			case 2:	sInput = L"보조펌프 "; break;
			default: sInput = L""; break;
			}

			XL.SetCellValue(27, 2 + (nTempIndex * 2), sInput);
			XL.SetCellValue(28, 2 + (nTempIndex * 2), L"기동");
			XL.SetCellValue(29, 2 + (nTempIndex * 2), L"확인");
			if (nIndex == 3) {
				XL.SetCellValue(26, 2 + (nTempIndex * 2), L"비상발전기");
				sTemp = L"비상발전기 기동확인";
			}
			else {
				sTemp.Format(L"옥내소화전 %s 기동확인", sInput);
			}
			XL.SetCellValue(31, 2 + (nTempIndex * 2), sTemp);
			++nTempIndex;
		}
		sTemp.Format(L"%d / %d (전체/완료)", nTotalCount, nCurrentCount);
		++nCurrentCount;
		g_pWaitDataLink->SetCaptionAddBottom(sTemp);
	}
	Sleep(300); // 메시지 출력 대기 시간

	g_pWaitDataLink->SetCaptionAddBottom(L"                저장 중입니다.               ");

	XL.SaveFileAs(sPath);
	XL.ReleaseExcel();

	::DeleteFile(sPath + L".tmp2");
	::DeleteFile(sPath + L".tmp");

	return 0;
}
