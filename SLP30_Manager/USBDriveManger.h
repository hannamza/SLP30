#pragma once

#include <cfgmgr32.h>
#include <afxtempl.h>
typedef struct USB_LIST
{
	CString sDeviceID;
	CString sParentDeviceID;
	CString sDriveLetter;
}usbList, *pUsbList;

class CUSBDriveManger : public TSingleton<CUSBDriveManger>
{
public:
	CUSBDriveManger();
	~CUSBDriveManger();

public:
	bool WriteSerialKey(CString sDriveLetter);
	bool ConfirmSerialKey();
	bool WriteSerialKeyUserID(CString sDriveLetter, CString sUserID);
	bool ConfirmSerialKeyUserID(CString & sUserID);
	int  GetUSBList();

private:
	void InitUSBManager();
	void ReleaseManager();
	bool matchDevInstToUsbDevice(DEVINST devinst, DWORD vid, DWORD pid);
	CString GetUSBVolumn(DEVINST devinst, unsigned vid, unsigned pid);

public:
	CList<pUsbList, pUsbList> m_list;
};

