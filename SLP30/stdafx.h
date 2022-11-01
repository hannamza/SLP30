
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.



#include <gdiPlus.h> //header ����
using namespace Gdiplus; //���ӽ����̽� ����
#pragma comment (lib,"Gdiplus.lib")

#define		CIRCUIT_PARENT	16
#define		CHECK_COUNT		20

#include "CommonDisplay.h"
#include "CommonProtocol.h"
#include "CommonState.h"
#include "TSingleton.h"
#include "CommonFunc.h"
#include "ImageBtn.h"
#include "ImageCheckBtn.h"
#include "ReadWriteState.h"
#include "CircuitBasicInfo.h"
#include "BaseInfoManager.h"
#include "SharedMemoryManager.h"
#include "KISA_SEED_CBC.h"
#include "PositionManager.h"
#include "CommonCircuitInfo.h"
#include "SaveManager.h"
#include "EncDec.h"
#include "ClientInterface.h"
#include "Network.h"
#include "Log.h"
#include "PatternManager.h"
#include "DataLinkManager.h"
#include "CreateProcessReturnValue.h"

#include <string>

using namespace std;

#define		SELECTION_PROJECT		(WM_USER+1501)
#define		CIRCUIT_LIST_INIT_MSG	(WM_USER+1502)		// �߰�� �϶�ǥ Ȯ�� ���

#define		SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#define		TIME_SLEEP(t, b) { DWORD cT=GetTickCount(); while(b && GetTickCount() - cT < t) Sleep(1); }

#define		TOPBAR_HEIGHT		50

#define		SOCKET_RECV		(WM_USER+9001)
#define		EDIT_SAVE_MSG	(WM_USER+9821)
#define		EXCEL_SAVE_MSG	(WM_USER+9822)
#define		ROM_SAVE_MSG	(WM_USER+9823)
#define		RECEIVE_RETURN_VALUE		(WM_USER+1234)

#define		MAX_CIRCUIT		250


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����


#include <afxsock.h>            // MFC ���� Ȯ��




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


