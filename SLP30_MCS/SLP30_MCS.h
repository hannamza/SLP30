
// GFSM_MCS.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSLP30_MCSApp:
// �� Ŭ������ ������ ���ؼ��� GFSM_MCS.cpp�� �����Ͻʽÿ�.
//

class CSLP30_MCSApp : public CWinApp
{
public:
	CSLP30_MCSApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CSLP30_MCSApp theApp;