#include "stdafx.h"
#include "USBDriveManger.h"
#include <devguid.h>
#include <winioctl.h>
#include <SetupAPI.h>
#include <initguid.h>
#include "KISA_SEED_CBC.h"

#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "Setupapi.lib")

#define		SEED_MAX_LEN	512

BYTE g_pbszUK[] = { 0x26, 0x6d, 0x66, 0x47, 0x35, 0x18, 0x1a, 0x61, 0x6f, 0x0a, 0x39, 0x7a, 0x36, 0x16, 0x25, 0x1a, 0x00 };
BYTE g_pbszIV[] = { 0xa1, 0xd1, 0x23, 0x2a, 0xc1, 0x77, 0xa2, 0x28, 0x89, 0xe1, 0x6c, 0xb5, 0x78, 0x01, 0x41, 0xd1, 0x00 };
BYTE g_pbszIV0[] = { 0x11, 0xd6, 0x33, 0x26, 0xa1, 0x07, 0xb2, 0xdd, 0x54, 0x99, 0x2c, 0xe3, 0x38, 0x22, 0x77, 0xbb, 0x00 };
BYTE g_pbszIV1[] = { 0x11, 0xd6, 0x33, 0x26, 0xa1, 0x07, 0xb2, 0xdd, 0x54, 0x66, 0x21, 0xa3, 0x38, 0x44, 0x12, 0xe0, 0x00 };
BYTE g_pbszIV2[] = { 0xc3, 0x27, 0x32, 0x26, 0x9c, 0x7a, 0x12, 0xcc, 0xa4, 0x11, 0x22, 0x13, 0xb3, 0x2c, 0xe8, 0x0e, 0x00 };
BYTE g_pbszIV3[] = { 0xd2, 0x18, 0x61, 0x26, 0x6a, 0x43, 0xb2, 0xbb, 0x40, 0x22, 0x23, 0x23, 0xbb, 0xa9, 0x8e, 0x78, 0x00 };


CUSBDriveManger::CUSBDriveManger()
{
	BYTE value[64];
	memset(value, 0x00, 64);
	int nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV0, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
	nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV1, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
	nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV2, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
}

CUSBDriveManger::~CUSBDriveManger()
{
	ReleaseManager();
}

int StrReplace(LPCTSTR sStringReplace, LPCTSTR sOldStr, LPCTSTR sNewStr, int nStringLengthMax)
{
	int nNewLength = lstrlen(sNewStr);
	int nOldLength = lstrlen(sOldStr);
	int nStringLength = lstrlen(sStringReplace);
	LPCTSTR pCharRep;
	if ((pCharRep = wcsstr(sStringReplace, sOldStr)) == NULL)
		return -1;
	int nAdd = 0;
	while (pCharRep != NULL)
	{
		int nLength = nStringLength - (pCharRep - sStringReplace) - nOldLength + nAdd + 2;
		nAdd += (nNewLength - nOldLength);
		if (nStringLength + nAdd < nStringLengthMax)
		{
			memmove((LPVOID)(pCharRep + nNewLength), pCharRep + nOldLength, nLength * sizeof(WCHAR));
			memcpy((LPVOID)pCharRep, sNewStr, nNewLength * sizeof(WCHAR));
			pCharRep += nNewLength;
			pCharRep = wcsstr(pCharRep, sOldStr);
		}
		else
			return -1;
	}
	return 0;
}

void GetDriveLettersFromDevInstID(LPCTSTR pwsDeviceInstanceID, __out LPTSTR pwsDriveLetters)
{
	pwsDriveLetters[0] = 0;
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
	{
		// hr = 0x80010119
		hr = CoInitializeSecurity(NULL,
			-1,                          // COM authentication 
			NULL,                        // Authentication services
			NULL,                        // Reserved
			RPC_C_AUTHN_LEVEL_NONE,      // RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
			RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
			NULL,                        // Authentication info
			EOAC_NONE,                   // Additional capabilities
			NULL                         // Reserved
		);
		if (SUCCEEDED(hr))
		{
			IWbemLocator* pLoc = NULL;
			hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
			if (SUCCEEDED(hr))
			{
				IWbemServices* pSvc = NULL;
				hr = pLoc->ConnectServer(bstr_t(L"ROOT\\CIMV2"), NULL, NULL, NULL, 0, NULL, NULL, &pSvc);
				if (SUCCEEDED(hr))
				{
					hr = CoSetProxyBlanket(
						pSvc,                        // Indicates the proxy to set
						RPC_C_AUTHN_DEFAULT,         // RPC_C_AUTHN_WINNT,  // RPC_C_AUTHN_xxx
						RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
						NULL,                        // Server principal name
						RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
						RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
						NULL,                        // client identity
						EOAC_NONE                    // proxy capabilities
					);
					if (SUCCEEDED(hr))
					{
						WCHAR wsQuery[MAX_PATH] = L"";
						lstrcpy(wsQuery, L"SELECT * FROM Win32_DiskDrive where PNPDeviceID='");
						lstrcat(wsQuery, pwsDeviceInstanceID);
						lstrcat(wsQuery, L"'");
						StrReplace(wsQuery, L"\\", L"\\\\", ARRAYSIZE(wsQuery));
						IEnumWbemClassObject* pWbemEnum = NULL;
						hr = pSvc->ExecQuery(bstr_t("WQL"),
							bstr_t(wsQuery),
							WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pWbemEnum);
						if (SUCCEEDED(hr))
						{
							IWbemClassObject* pclsObj;
							ULONG uReturn = 0;
							int nCount = 0;
							while (pWbemEnum)
							{
								hr = pWbemEnum->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
								// 0x80041010  Invalid class WBEM_E_INVALID_CLASS
								// 2147749911 (0x80041017) WBEM_E_INVALID_QUERY
								if (0 == uReturn)
								{
									break;
								}

								VARIANT vtProp;
								VariantInit(&vtProp);
								hr = pclsObj->Get(L"DeviceID", 0, &vtProp, 0, 0);
								WCHAR wsDeviceID[255] = L"";
								wsprintf(wsDeviceID, L"%s", vtProp.bstrVal);
								VariantClear(&vtProp);
								lstrcpy(wsQuery, L"ASSOCIATORS OF {Win32_DiskDrive.DeviceID='");
								lstrcat(wsQuery, wsDeviceID);
								lstrcat(wsQuery, L"'");
								lstrcat(wsQuery, L"} WHERE AssocClass = Win32_DiskDriveToDiskPartition");

								IEnumWbemClassObject* pWbemEnum2 = NULL;
								hr = pSvc->ExecQuery(bstr_t("WQL"),
									bstr_t(wsQuery),
									WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pWbemEnum2);
								StrReplace(wsQuery, L"\\", L"\\\\", ARRAYSIZE(wsQuery));

								if (SUCCEEDED(hr))
								{
									IWbemClassObject* pclsObj2;
									ULONG uReturn2 = 0;
									while (pWbemEnum2)
									{
										hr = pWbemEnum2->Next(WBEM_INFINITE, 1, &pclsObj2, &uReturn2);
										if (0 == uReturn2)
										{
											break;
										}
										VARIANT vtProp;
										VariantInit(&vtProp);
										hr = pclsObj2->Get(L"DeviceID", 0, &vtProp, 0, 0);
										WCHAR wsPartitionDeviceID[255] = L"";
										wsprintf(wsPartitionDeviceID, L"%s", vtProp.bstrVal);
										VariantClear(&vtProp);

										lstrcpy(wsQuery, L"ASSOCIATORS Of {Win32_DiskPartition.DeviceID='");
										lstrcat(wsQuery, wsPartitionDeviceID);
										lstrcat(wsQuery, L"'");
										lstrcat(wsQuery, L"} WHERE AssocClass = Win32_LogicalDiskToPartition");
										IEnumWbemClassObject* pWbemEnum3 = NULL;
										hr = pSvc->ExecQuery(bstr_t("WQL"),
											bstr_t(wsQuery),
											WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pWbemEnum3);
										StrReplace(wsQuery, L"\\", L"\\\\", ARRAYSIZE(wsQuery));
										if (SUCCEEDED(hr))
										{
											IWbemClassObject* pclsObj3;
											ULONG uReturn3 = 0;
											while (pWbemEnum3)
											{
												hr = pWbemEnum3->Next(WBEM_INFINITE, 1, &pclsObj3, &uReturn3);
												if (0 == uReturn3)
												{
													break;
												}
												VARIANT vtProp;
												VariantInit(&vtProp);
												hr = pclsObj3->Get(L"DeviceID", 0, &vtProp, 0, 0);
												WCHAR sLogicalDeviceID[255] = L"";
												wsprintf(sLogicalDeviceID, L"%s", vtProp.bstrVal);
												VariantClear(&vtProp);

												if (lstrcmp(pwsDriveLetters, L"") == 0)
												{
													lstrcat(pwsDriveLetters, sLogicalDeviceID);
												}
												else
												{
													lstrcat(pwsDriveLetters, L", ");
													lstrcat(pwsDriveLetters, sLogicalDeviceID);
												}
												pclsObj3->Release();
											}
											pWbemEnum3->Release();
										}
										pclsObj2->Release();
									}
									pWbemEnum2->Release();
								}

								nCount++;
								pclsObj->Release();
							}
							pWbemEnum->Release();
						}
					}
					pSvc->Release();
				}
				pLoc->Release();
			}
		}
		CoUninitialize();
	}
}

void CUSBDriveManger::InitUSBManager()
{
	GetUSBList();
}

void CUSBDriveManger::ReleaseManager()
{
	pUsbList pUsb;
	while (!m_list.IsEmpty()) {
		pUsb = m_list.RemoveHead();
		SAFE_DELETE(pUsb);
	}
}

int CUSBDriveManger::GetUSBList()
{
	ReleaseManager();

	HANDLE hDevice = NULL;
	HDEVINFO DeviceInfoSet = SetupDiGetClassDevs(&GUID_DEVINTERFACE_DISK, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	SP_DEVINFO_DATA DeviceInfoData;
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (DWORD dwDeviceIndex = 0; SetupDiEnumDeviceInfo(DeviceInfoSet, dwDeviceIndex, &DeviceInfoData); dwDeviceIndex++)
	{
		SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
		DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		for (DWORD dwMemberIndex = 0; SetupDiEnumDeviceInterfaces(DeviceInfoSet, &DeviceInfoData, &GUID_DEVINTERFACE_DISK, dwMemberIndex, &DeviceInterfaceData); dwMemberIndex++)
		{
			DWORD dwDeviceInterfaceDetailDataSize = offsetof(SP_DEVICE_INTERFACE_DETAIL_DATA, DevicePath) + MAX_PATH * sizeof(TCHAR);
			PSP_DEVICE_INTERFACE_DETAIL_DATA pDeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)new BYTE[dwDeviceInterfaceDetailDataSize];
			pDeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			if (SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, pDeviceInterfaceDetailData, dwDeviceInterfaceDetailDataSize, NULL, NULL))
			{
				CONFIGRET cRet;
				DEVINST hDevInst = DeviceInfoData.DevInst;
				DEVINST hParentDevInst;
				cRet = CM_Get_Parent(&hParentDevInst, hDevInst, 0);
				TCHAR wsParentDeviceID[MAX_DEVICE_ID_LEN];
				cRet = CM_Get_Device_ID(hParentDevInst, wsParentDeviceID, sizeof(wsParentDeviceID) / sizeof(TCHAR), 0);
				TCHAR wsDeviceID[MAX_DEVICE_ID_LEN];
				cRet = CM_Get_Device_ID(hDevInst, wsDeviceID, sizeof(wsDeviceID) / sizeof(TCHAR), 0);

				WCHAR wsText[MAX_PATH] = L"";
				WCHAR wsDriveLetters[MAX_PATH] = L"";
				GetDriveLettersFromDevInstID(wsDeviceID, wsDriveLetters);
				SAFE_DELETE(pDeviceInterfaceDetailData);

				UINT nType = GetDriveType(wsDriveLetters);
				if (DRIVE_REMOVABLE != nType) {
					continue;
				}
				pUsbList pUsb = new usbList;
				pUsb->sDeviceID = wsDeviceID;
				pUsb->sDriveLetter = wsDriveLetters;
				pUsb->sParentDeviceID.Format(L"[GFS_HIDDENKEY]%s", wsParentDeviceID);
				m_list.AddTail(pUsb);
				TRACE(L"=====> USB Drive: [%s], Device ID: [%s], Parent Device ID: [%s]\n", wsDriveLetters, wsDeviceID, wsParentDeviceID);
			}
			else {
				SAFE_DELETE(pDeviceInterfaceDetailData);
			}
		}
	}

	return 0;
}

bool CUSBDriveManger::WriteSerialKey(CString sDriveLetter)
{
	InitUSBManager();

	pUsbList pUsb = NULL;
	CString sText;
	bool bFind = false;
	for (int nIndex = 0; nIndex < m_list.GetCount(); nIndex++) {
		pUsb = m_list.GetAt(m_list.FindIndex(nIndex));
		if (pUsb->sDriveLetter == sDriveLetter) {
			bFind = true;
			break;
		}
	}
	if (!bFind) {
		return false;
	}
	BYTE byteDeviceID[SEED_MAX_LEN], byteEncrypt[SEED_MAX_LEN];
	memset(byteDeviceID, 0x00, SEED_MAX_LEN);
	memset(byteEncrypt, 0x00, SEED_MAX_LEN);
	memcpy_s(byteDeviceID, SEED_MAX_LEN, pUsb->sParentDeviceID, sizeof(WCHAR)*pUsb->sParentDeviceID.GetLength());

	BYTE value[64];
	memset(value, 0x00, 64);
	int nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV3, g_pbszIV, 16, value);
	value[4] = 0x00;

	int nResult = SEED_CBC_Encrypt(g_pbszUK, value, byteDeviceID, sizeof(WCHAR)*pUsb->sParentDeviceID.GetLength(), byteEncrypt);
	sText.Format(L"%s\\usersn.bin", sDriveLetter);
	if (::PathFileExists(sText)) {
		if (!::DeleteFile(sText)) {
			return false;
		}
	}
	HANDLE hFile = CreateFile(sText, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if (!hFile) {
		return false;
	}
	DWORD dwWritten = 0;
	BOOL bResult = WriteFile(hFile, byteEncrypt, nResult, &dwWritten, NULL);
	CloseHandle(hFile);
	if (bResult) {
		return true;
	}
	else {
		::DeleteFile(sText);
		return false;
	}
	return false;
}

bool CUSBDriveManger::ConfirmSerialKey()
{
	InitUSBManager();

	pUsbList pUsb = NULL;
	CString sText;
	DWORD dwReadden = 0;
	BYTE byteEncrypt[SEED_MAX_LEN];
	WCHAR szPlainText[SEED_MAX_LEN];
	memset(szPlainText, 0x00, sizeof(WCHAR) * SEED_MAX_LEN);
	memset(byteEncrypt, 0x00, SEED_MAX_LEN);
	BYTE value[64];
	memset(value, 0x00, 64);
	int nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV3, g_pbszIV, 16, value);
	value[4] = 0x00;

	for (int nIndex = 0; nIndex < m_list.GetCount(); nIndex++) {
		pUsb = m_list.GetAt(m_list.FindIndex(nIndex));
		sText.Format(L"%s\\usersn.bin", pUsb->sDriveLetter);
		if (::PathFileExists(sText)) {
			HANDLE hFile = CreateFile(sText, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if (!hFile) {
				continue;
			}
			BOOL bResult = ReadFile(hFile, byteEncrypt, SEED_MAX_LEN, &dwReadden, NULL);
			CloseHandle(hFile);
			if (bResult) {
				nValue = SEED_CBC_Decrypt(g_pbszUK, value, byteEncrypt, dwReadden, (BYTE*)szPlainText);
				if (pUsb->sParentDeviceID.Compare(szPlainText) == 0) {
					return true;
				}
			}
		}
	}
	return false;
}

bool CUSBDriveManger::WriteSerialKeyUserID(CString sDriveLetter, CString sUserID)
{
	InitUSBManager();

	pUsbList pUsb = NULL;
	CString sText;
	bool bFind = false;
	for (int nIndex = 0; nIndex < m_list.GetCount(); nIndex++) {
		pUsb = m_list.GetAt(m_list.FindIndex(nIndex));
		if (pUsb->sDriveLetter == sDriveLetter) {
			bFind = true;
			break;
		}
	}
	if (!bFind) {
		return false;
	}
	BYTE byteDeviceID[SEED_MAX_LEN], byteEncrypt[SEED_MAX_LEN];
	memset(byteDeviceID, 0x00, SEED_MAX_LEN);
	memset(byteEncrypt, 0x00, SEED_MAX_LEN);
	CString sDeviceID;
	sDeviceID.Format(L"%s」%s」GFS_ENCRYPT_VALUE", pUsb->sParentDeviceID, sUserID);
	memcpy_s(byteDeviceID, SEED_MAX_LEN, sDeviceID, sizeof(WCHAR)*sDeviceID.GetLength());

	BYTE value[64];
	memset(value, 0x00, 64);
	int nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV3, g_pbszIV, 16, value);
	value[4] = 0x00;

	int nResult = SEED_CBC_Encrypt(g_pbszUK, value, byteDeviceID, sizeof(WCHAR)*sDeviceID.GetLength(), byteEncrypt);
	sText.Format(L"%s\\usersn.bin", sDriveLetter);
	if (::PathFileExists(sText)) {
		if (!::DeleteFile(sText)) {
			return false;
		}
	}
	HANDLE hFile = CreateFile(sText, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if (!hFile) {
		return false;
	}
	DWORD dwWritten = 0;
	BOOL bResult = WriteFile(hFile, byteEncrypt, nResult, &dwWritten, NULL);
	CloseHandle(hFile);
	if (bResult) {
		return true;
	}
	else {
		::DeleteFile(sText);
		return false;
	}
	return false;
}

bool CUSBDriveManger::ConfirmSerialKeyUserID(CString & sUserID)
{
	InitUSBManager();

	pUsbList pUsb = NULL;
	CString sText;
	DWORD dwReadden = 0;
	BYTE byteEncrypt[SEED_MAX_LEN];
	WCHAR szPlainText[SEED_MAX_LEN];
	memset(szPlainText, 0x00, sizeof(WCHAR) * SEED_MAX_LEN);
	memset(byteEncrypt, 0x00, SEED_MAX_LEN);

	BYTE value[64];
	memset(value, 0x00, 64);
	int nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV3, g_pbszIV, 16, value);
	value[4] = 0x00;
	for (int nIndex = 0; nIndex < m_list.GetCount(); nIndex++) {
		pUsb = m_list.GetAt(m_list.FindIndex(nIndex));
		sText.Format(L"%s\\usersn.bin", pUsb->sDriveLetter);
		if (::PathFileExists(sText)) {
			HANDLE hFile = CreateFile(sText, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if (!hFile) {
				continue;
			}
			BOOL bResult = ReadFile(hFile, byteEncrypt, SEED_MAX_LEN, &dwReadden, NULL);
			CloseHandle(hFile);
			if (bResult) {
				nValue = SEED_CBC_Decrypt(g_pbszUK, value, byteEncrypt, dwReadden, (BYTE*)szPlainText);
				if (nValue == 0) {
					return false;
				}
				CString sDeviceID, sUserID;
				if (AfxExtractSubString(sDeviceID, szPlainText, 0, L'」') && AfxExtractSubString(sUserID, szPlainText, 1, L'」'))
				{
					if (pUsb->sParentDeviceID.Compare(sDeviceID) == 0) {
						return true;
					}
				}
			}
		}
	}
	return false;
}