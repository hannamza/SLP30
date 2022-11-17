// WebDavClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WebDavClient.h"
#include "KISA_SEED_CBC.h"
#include "WebDavInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define		SEED_MAX_LEN	8192

BYTE g_pbszUK[] = { 0x27, 0x6d, 0x66, 0x47, 0x35, 0xe8, 0x1d, 0x6a, 0x6f, 0x1a, 0x39, 0x7a, 0x36, 0x16, 0x21, 0x1a, 0x00 };
BYTE g_pbszIV[] = { 0xa1, 0xd1, 0x23, 0x2a, 0xc1, 0xc7, 0xa2, 0x28, 0x89, 0xe1, 0x6c, 0xb5, 0x78, 0x01, 0x41, 0xd1, 0x00 };
BYTE g_pbszIV0[] = { 0x11, 0xa6, 0x33, 0xa6, 0xa1, 0x07, 0x12, 0xdd, 0x54, 0x9a, 0x2c, 0xe3, 0x38, 0x2a, 0x77, 0xbb, 0x00 };
BYTE g_pbszIV1[] = { 0x11, 0xd6, 0x33, 0xb6, 0xa1, 0x07, 0xb2, 0xdd, 0x54, 0x6b, 0x21, 0xa3, 0x38, 0x4a, 0x32, 0xe0, 0x00 };
BYTE g_pbszIV2[] = { 0xc3, 0x27, 0x32, 0x16, 0x9c, 0x7a, 0x12, 0xcc, 0xa4, 0x1c, 0x22, 0x13, 0xb3, 0x2c, 0xe8, 0x0e, 0x00 };
BYTE g_pbszIV3[] = { 0xd2, 0x18, 0x61, 0x36, 0x6a, 0x43, 0xa2, 0xbb, 0x40, 0x2d, 0x23, 0x23, 0xbb, 0xaa, 0x8e, 0x78, 0x00 };

// 유일한 응용 프로그램 개체입니다.

CWinApp theApp;

using namespace std;


char g_szChar[SEED_MAX_LEN];
CHAR* WCharToChar(WCHAR* wszText)
{
	memset(g_szChar, 0x00, SEED_MAX_LEN);
	int l = WideCharToMultiByte(CP_ACP, 0, wszText, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, wszText, -1, g_szChar, /*CHAR_LEN_SIZE*/l, 0, 0);
	return g_szChar;
}

std::string TrimAll(std::string & str)
{
	CStringA value = str.c_str();
	value.Replace(" ", "");
	value.Replace("\\", "");
	value.Replace("/", "");
	value.Replace(":", "");
	value.Replace("*", "");
	value.Replace("?", "");
	value.Replace("\"", "");
	value.Replace("<", "");
	value.Replace(">", "");
	value.Replace("|", "");
	str = value.GetBuffer(0);
	value.ReleaseBuffer();
	return str;
}

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 오류 코드를 필요에 따라 수정합니다.
            wprintf(L"심각한 오류: MFC를 초기화하지 못했습니다.\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 응용 프로그램의 동작은 여기에서 코딩합니다.
        }
    }
    else
    {
        // TODO: 오류 코드를 필요에 따라 수정합니다.
        wprintf(L"심각한 오류: GetModuleHandle 실패\n");
        nRetCode = 1;
    }
	nRetCode = 0;

	std::wstring arglist;
	LPWSTR* szArglist;
	int nArgs;
	BYTE szPlainInfo[SEED_MAX_LEN];
	BYTE szUploadInfo[SEED_MAX_LEN];
	memset(szPlainInfo, 0x00, SEED_MAX_LEN);
	memset(szUploadInfo, 0x00, SEED_MAX_LEN);
	uploadInfo info;
	BYTE value[64];
	memset(value, 0x00, 64);
	DWORD dwReadden = 0;
	std::string temp, temp1, put;

	CSharedMemoryManager::New();
	if (!CSharedMemoryManager::Instance()->OpenSharedMemory()) {
		nRetCode = -10;
		goto _EXIT_POINT;
	}

	//20221117 GBM start - test
#if 1
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	if (NULL == szArglist) {
		nRetCode = -1;
		goto _EXIT_POINT;
}
	else {
		for (int i = 0; i < nArgs; ++i) {
			switch (i) {
			case 0: break;
			case 1: memcpy_s(szUploadInfo, SEED_MAX_LEN, szArglist[i], wcslen(szArglist[i]) * sizeof(WCHAR)); break;
			default: break;
			}
		}
	}
	int nRead = _wtoi((WCHAR*)szUploadInfo);
#else
	int nRead = 4512;
#endif
	//20221117 GBM end

	//AfxMessageBox(L"dkkdkd");

	if (!CSharedMemoryManager::Instance()->ReadSharedMemory((BYTE*)&info, nRead)) {
		nRetCode = -11;
		goto _EXIT_POINT;
	}

	/*int nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV0, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
	nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV1, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
	nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV2, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
	g_pbszIV[4] = '0x45';

	nValue = SEED_CBC_Decrypt(g_pbszUK, g_pbszIV, szUploadInfo, nRead, (BYTE*)&info);
	if (nValue == 0) {
		nRetCode = -2;
		goto _EXIT_POINT;

	}
	if (nValue != sizeof(uploadInfo)) {
		nRetCode = -3;
		goto _EXIT_POINT;
	}*/

	CWebDavInterface::New();
	CWebDavInterface::Instance()->LoginInfo("160.202.162.3", "GFS_SLP30_WebDav", "Gfsrnd@1020!", "/DATA/", 8090);

	temp1 = WCharToChar(info.szUserID);
	temp = TrimAll(temp1);
	if (!CWebDavInterface::Instance()->MakeDirectory(temp)) {
		nRetCode = -4;
		goto _EXIT_POINT;
	}
	if (wcslen(info.szLocalPath) == 0) {
		nRetCode = 0;
		goto _EXIT_POINT;
	}
	temp += "/";
	temp1 = WCharToChar(info.szYear);
	temp += TrimAll(temp1);
	if (!CWebDavInterface::Instance()->MakeDirectory(temp)) {
		nRetCode = -5;
		goto _EXIT_POINT;
	}
	temp += "/";
	temp1 = WCharToChar(info.szFolder);
	temp += TrimAll(temp1);
	if (!CWebDavInterface::Instance()->MakeDirectory(temp)) {
		nRetCode = -6;
		goto _EXIT_POINT;
	}
	temp += "/";
	temp1 = WCharToChar(info.szDate);
	temp += TrimAll(temp1);
	if (!CWebDavInterface::Instance()->MakeDirectory(temp)) {
		nRetCode = -7;
		goto _EXIT_POINT;
	}
	temp += "/";
	temp1 = WCharToChar(info.szFileName);
	temp += TrimAll(temp1);
	if (!CWebDavInterface::Instance()->UploadFile(temp, WCharToChar(info.szLocalPath))) {
		nRetCode = -8;
		goto _EXIT_POINT;
	}

_EXIT_POINT:
	CSharedMemoryManager::Delete();
	CWebDavInterface::Delete();
	return nRetCode;
}
