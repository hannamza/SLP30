
// SLP30_Manager.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSLP30_ManagerApp:
// �� Ŭ������ ������ ���ؼ��� SLP30_Manager.cpp�� �����Ͻʽÿ�.
//

class CSLP30_ManagerApp : public CWinApp
{
public:
	CSLP30_ManagerApp();

	ULONG_PTR m_gdiplusToken;

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CSLP30_ManagerApp theApp;