
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#define		MAX_CIRCUIT		250

//#define		CIRCUIT_PARENT	16
#define		CIRCUIT_PARENT	21
#define		CHECK_COUNT		20
#define		CIRCUIT_CNT		55
#define		MAX_CIRCUIT_ADDRESS		MAX_CIRCUIT + 6		//계통 당 최대 회로 주소 크기, 실제 할당 최대 크기는 MAX_CIRCUIT (250)
#define		ROOFTOP_NAME	"RF"

#include <gdiPlus.h> //header 포함
using namespace Gdiplus; //네임스페이스 설정
#pragma comment (lib,"Gdiplus.lib")

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
#define		CIRCUIT_LIST_INIT_MSG	(WM_USER+1502)		// 중계기 일람표 확정 취소

#define		SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#define		TIME_SLEEP(t, b) { DWORD cT=GetTickCount(); while(b && GetTickCount() - cT < t) Sleep(1); }

#define		TOPBAR_HEIGHT		50

#define		SOCKET_RECV		(WM_USER+9001)
#define		EDIT_SAVE_MSG	(WM_USER+9821)
#define		EXCEL_SAVE_MSG	(WM_USER+9822)
#define		ROM_SAVE_MSG	(WM_USER+9823)
#define		RECEIVE_RETURN_VALUE		(WM_USER+1234)


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원


#include <afxsock.h>            // MFC 소켓 확장




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


