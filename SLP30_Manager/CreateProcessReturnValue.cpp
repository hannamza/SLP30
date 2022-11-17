#include "StdAfx.h"
#include "CreateProcessReturnValue.h"
#include "SharedMemoryManager.h"

//BYTE g_pbszUK[] = { 0x27, 0x6d, 0x66, 0x47, 0x35, 0xe8, 0x1d, 0x6a, 0x6f, 0x1a, 0x39, 0x7a, 0x36, 0x16, 0x21, 0x1a, 0x00 };
//BYTE g_pbszIV[] = { 0xa1, 0xd1, 0x23, 0x2a, 0xc1, 0xc7, 0xa2, 0x28, 0x89, 0xe1, 0x6c, 0xb5, 0x78, 0x01, 0x41, 0xd1, 0x00 };
//BYTE g_pbszIV0[] = { 0x11, 0xa6, 0x33, 0xa6, 0xa1, 0x07, 0x12, 0xdd, 0x54, 0x9a, 0x2c, 0xe3, 0x38, 0x2a, 0x77, 0xbb, 0x00 };
//BYTE g_pbszIV1[] = { 0x11, 0xd6, 0x33, 0xb6, 0xa1, 0x07, 0xb2, 0xdd, 0x54, 0x6b, 0x21, 0xa3, 0x38, 0x4a, 0x32, 0xe0, 0x00 };
//BYTE g_pbszIV2[] = { 0xc3, 0x27, 0x32, 0x16, 0x9c, 0x7a, 0x12, 0xcc, 0xa4, 0x1c, 0x22, 0x13, 0xb3, 0x2c, 0xe8, 0x0e, 0x00 };
//BYTE g_pbszIV3[] = { 0xd2, 0x18, 0x61, 0x36, 0x6a, 0x43, 0xa2, 0xbb, 0x40, 0x2d, 0x23, 0x23, 0xbb, 0xaa, 0x8e, 0x78, 0x00 };

CCreateProcessReturnValue::CCreateProcessReturnValue(void)
{
	mEventHandle = NULL;
	m_hThreadHandle = NULL;
	m_bThread = FALSE;
	m_bTerminate = TRUE;

	m_nReturnValue = -1;

	m_pWnd = NULL;
}

CCreateProcessReturnValue::~CCreateProcessReturnValue(void)
{
	CreateProcessReturnValueStop();
}

void CCreateProcessReturnValue::StartCreateProcess(INT nIndex, CString sPath, CString sArgs, CWnd* pWnd)
{
	m_nIndex = nIndex;
	m_sPath = sPath;
	m_sArgs = sArgs;
	m_pWnd = pWnd;

	CreateProcessReturnValueStart();
}

bool CCreateProcessReturnValue::SendDataToWebdav(CString sLocalPath, CString sRemoteFileName, CString sID)
{
	CString sDate, sYear;
	CTime time = CTime::GetCurrentTime();

	uploadInfo info;
	memset(&info, 0x00, sizeof(uploadInfo));
	wcscpy_s(info.szLocalPath, sLocalPath.GetBuffer(0));
	sDate.Format(L"");
	wcscpy_s(info.szDate, sDate.GetBuffer(0));
	wcscpy_s(info.szFileName, sRemoteFileName.GetBuffer(0));
	wcscpy_s(info.szUserID, CCommonState::ie()->m_sUserID.GetBuffer(0));
	sYear.Format(L"%d", time.GetYear());
	wcscpy_s(info.szYear, sYear.GetBuffer(0));

	/*BYTE value[64];
	memset(value, 0x00, 64);
	int nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV0, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
	nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV1, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
	nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV2, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
	g_pbszIV[4] = '0x45';

	BYTE szUploadInfo[8192];
	memset(szUploadInfo, 0x00, 8192);
	DWORD dwReadden = 0;
	nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV, (BYTE*)&info, sizeof(uploadInfo), szUploadInfo);*/

	CCommonState::ie()->m_nReturnValue = -99;

	if (!CSharedMemoryManager::ie()->WriteSharedMemory((BYTE*)&info, sizeof(uploadInfo))) {
		return false;
	}

	WCHAR szPath[2048], szArg[16];
	GetModuleFileName(NULL, szPath, 2048);
	PathRemoveFileSpec(szPath);
	wcscat(szPath, L"\\WebDavClient.exe");
	swprintf_s(szArg, L"%d", sizeof(uploadInfo));

	StartCreateProcess(0, szPath, szArg, NULL);

	return true;
}

//////////// THREAD 1 ///////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI CreateProcessReturnValueThread(LPVOID parameter)
{
	CCreateProcessReturnValue *Owner = (CCreateProcessReturnValue*) parameter;
	Owner->m_bTerminate = FALSE;
	Owner->CreateProcessReturnValue();
	Owner->m_bTerminate = TRUE;

	return 0;
}

void CCreateProcessReturnValue::CreateProcessReturnValueStart()
{
	if(m_hThreadHandle) return;

	m_bThread = TRUE;

	mEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mEventHandle == NULL) return;

	m_hThreadHandle = CreateThread(NULL, 0, ::CreateProcessReturnValueThread, this, 0, NULL);
	WaitForSingleObject(mEventHandle, 2000);
}

void CCreateProcessReturnValue::CreateProcessReturnValueStop()
{
	m_bThread = FALSE;

	if(m_hThreadHandle)
	{
		DWORD dwLastTime = GetTickCount();
		while(!m_bTerminate)
		{
			Sleep(1);
			if(GetTickCount() - dwLastTime >= 3000) break;
		}
		if(!m_bTerminate)
		{
			::SuspendThread(m_hThreadHandle);
			::TerminateThread(m_hThreadHandle, 0);
		}
		else if(m_hThreadHandle)
			CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
		m_bTerminate = TRUE;
	}
}

void CCreateProcessReturnValue::CreateProcessReturnValue()
{
	SetEvent(mEventHandle);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	WCHAR szCommand[2048];
	wsprintf(szCommand, L"\"%s\" %s", m_sPath.GetBuffer(0), m_sArgs.GetBuffer(0));

	try{
		if(!CreateProcess(NULL, szCommand, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi))
		{
			/*m_nReturnValue = -98;
			m_pWnd->PostMessage(RECEIVE_RETURN_VALUE, m_nIndex, m_nReturnValue);*/
			CCommonState::ie()->m_nReturnValue = m_nReturnValue = -98;
			return;
		}

		DWORD dwReturn = WaitForSingleObject( pi.hProcess, 5000);
		if(pi.hProcess)
		{
			DWORD dwExitCode = STILL_ACTIVE;
			while(dwExitCode == STILL_ACTIVE)
			{
				GetExitCodeProcess(pi.hProcess, &dwExitCode);
				Sleep(1);
			} 
			m_nReturnValue = dwExitCode;
		}
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
	} catch(...)
	{
		m_hThreadHandle = NULL;
		return;
	}

	CCommonState::ie()->m_nReturnValue = m_nReturnValue;

	/*if (m_pWnd) {
		m_pWnd->PostMessage(RECEIVE_RETURN_VALUE, m_nIndex, m_nReturnValue);
	}*/
}
