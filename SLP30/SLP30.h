
// SLP30.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSLP30App:
// �� Ŭ������ ������ ���ؼ��� SLP30.cpp�� �����Ͻʽÿ�.
//

class CSLP30App : public CWinApp
{
public:
	CSLP30App();

	ULONG_PTR m_gdiplusToken;

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CSLP30App theApp;