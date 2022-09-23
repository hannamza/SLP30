#pragma once

#include <math.h>
#include <imm.h>

#pragma comment(lib, "imm32.lib")

#define		CHAR_LEN_SIZE		(512*1024)

static double  g_dOSVersion = -1;
static CString g_sOsVersion;

static UINT g_nTimerCount = 0;
static UINT g_nCaptureCount = 0;
static  INT g_nPassCount = 0;
static  INT g_nTrashCount = 0;
static double g_dDivValue = 1.0;
static double g_dRealDivValue = 1.0;
static double g_dReverseDivValue = 1.0;

static WCHAR g_wszFilePath[2048];
static CHAR g_szFilePath[2048];
static WCHAR g_wszDocumentPath[2048];
static CHAR g_szDocumentPath[2048];
static WCHAR g_wszBasePath[2048];
static CHAR g_szBasePath[2048];
static WCHAR g_wszBaseRecordPath[2048];
static CHAR g_szBaseRecordPath[2048];
static WCHAR g_wszBaseImagePath[2048];
static CHAR g_szBaseImagePath[2048];
static WCHAR g_wszBaseLogPath[2048];
static CHAR g_szBaseLogPath[2048];
static WCHAR g_wszBaseDumpPath[2048];
static CHAR g_szBaseDumpPath[2048];
static WCHAR g_wszBaseTempPath[2048];
static CHAR g_szBaseTempPath[2048];
static WCHAR g_wszBaseConfigPath[2048];
static CHAR g_szBaseConfigPath[2048];
static WCHAR g_wszBaseDocPath[2048];
static CHAR g_szBaseDocPath[2048];
static WCHAR g_wszTemp[2048];
static CHAR g_szTemp[2048];

static INT g_nFilePath = 0;
static CHAR g_szChar[CHAR_LEN_SIZE];
static TCHAR g_tszChar[CHAR_LEN_SIZE];
static WCHAR g_wszChar[CHAR_LEN_SIZE];
static BOOL g_bFaultRatio = FALSE;

static INT g_nProgramType = 0;

class CCommonFunc
{
public:
	CCommonFunc(void);
	~CCommonFunc(void);

public:
	static UINT GetTimerCount();
	static void IncreaseTimerCount();
	static UINT GetCaptureCount();
	static void IncreaseCaptureCount();
	static void DecreaseCaptureCount();
	static  INT	GetPassCount();
	static void IncreasePassCount();
	static void DecreasePassCount();
	static  INT GetTrashCount();
	static void IncreaseTrashCount();
	static void DecreaseTrashCount();
	static void SetFaultRatio(BOOL bValue);
	static BOOL GetFaultRatio();
	static void SetProgramType(INT nType);	// 0: ����ũž, 1: ķ
	static INT  GetProgramType();
	static void SetDivValue(double dValue);
	static double GetDivValue();
	static void SetRealDivValue(double dValue);
	static double GetRealDivValue();
	static void SetReverseDivValue(double dValue);
	static double GetReverseDivValue();
	static INT  GetRealSize(INT nValue);

	static CHAR* WCharToChar(WCHAR* wszText);
	static CHAR* WcharToUtf8(WCHAR* wszText);
	static CHAR* Utf8ToChar(char* str);
	static WCHAR* Utf8ToWCHAR(char* str);
	static TCHAR* CharToTCHAR(char *str);
	static WCHAR* CharToWCHAR(char *str);

	static BOOL	FindProcess(CString sExeName, HANDLE & hHandle);
	static BOOL FindProcessID(CString sExeName, DWORD & pID);
	static BOOL	ExecuteCreateWindow(CString strPath, HWND & hWnd);

	static BOOL GetTrayRect(CRect & rect);

public:
	static void		GetMonitorInfoEx(CRect & rect); // ��ü ������� ������ǥ
	static void		GetMonitorInfo(CRect & rect, INT nType); // 0: �ָ����, 1: ��ü �����
	static CString	GetCurrentOsVersionString();
	static double	GetCurrentOsVersion(CString & sVerstion);
	static double	GetCPUUsages();
	static  INT		GetInternetState();
	static BOOL		MixVoiceData(LPWORD lpRecvData, LPWORD lpRecordData, LPWORD lpDstData, INT nSize, INT bitsPerSample);
	static BOOL		VoiceVolumeControl(LPWORD lpRecordData, INT nSize, INT bitsPerSample, float gain);

	static BOOL		MakeResource2File(CString strFilename, UINT uIDResource, LPCTSTR lpResourceType);
	static BOOL		GetCurrentPath(CString & strPath);
	static CString	GetCurrentPath();
	static BOOL		GetRealFilePath(CString sFileName, CString & sPath);
	static CString	GetMakePath(CString sName);
	static void		GetMakeFolderPath(CString sFolderName, CString & sFolderPath);
	static BOOL		CreateDirectory(LPCTSTR szDir);
	static void		DeleteDirectory(LPCTSTR szDir);
	static void		DeleteDataInDirectory(LPCTSTR szDir);
	static CString	GetCurrentModuleFileName();

	static BOOL		SaveBMPFile(BYTE* buffer, INT nWidth, INT nHeight, CHAR* szPath, INT nBitmapDepth);

	static WCHAR*	GetDocumentPathW();		// ������ ���
	static CHAR*	GetDocumentPathA();

	static WCHAR*	GetBaseDumpPathW();

	static CString	FormatVersion(CString cs);
	static CString	GetVersionInfo(HMODULE hLib, CString csEntry);

	static HWND		GetFullScreenWindow();
	static BOOL		ExecuteProgram(WCHAR* szPath);
	static BOOL		ShellExecuteProgram(WCHAR* szPath, WCHAR* args, UINT nShow=SW_SHOW);
	static BOOL		KillProcess(CString sExeName);
	static BOOL		KillProcessAll(CString sExeName);
	static BOOL		ProgramCreateProcess(const WCHAR *pszTitle, const WCHAR *pszCommand, BOOL bShow, WCHAR* pszArg);
	static BOOL		ProgramCreateProcessA(const CHAR *pszTitle, const CHAR *pszCommand, BOOL bShow, CHAR* pszArg);

	static void		GetMyLocalIP(UINT & nIP);
	static void		GetMyLocalIP(CString & sIP);
	static UINT		ChangeIPAddressToINT(CHAR* szIP);
	static VOID		ChangeIPAddressToString(UINT in, CHAR* szIP);

	static void		SetHanMode(HWND hWnd);
	static void		SetEngMode(HWND hWnd);

	static BOOL		GetIPFromUrl(char* szUrl, INT & nCount, UINT * nIP);

	static ULONG	ProcIDFromWnd(HWND hwnd);	// ������ �ڵ�� ���μ��� ���̵� ���
	static HWND		GetWinHandle(ULONG pid);	// ���μ��� ���̵�� ������ �ڵ� ���

	static time_t	MakeTimeT(int YYYY, int MM, int DD, int hh, int mi, int ss);

	static UINT		GetAtoUI(char* pValue);
	static CSize	GetTextExtent(CDC* _pDC, CString sText);
	static CSize	GetTextExtent(CDC* _pDC, CFont & font, CString sText);

	static BOOL		SaveDCtoImage(CBitmap & bitmap, INT nWidth, INT nHeight, CString sPath);

	static BOOL		GetFileVersion(CString sFilePath, CString & sVersion);
	static BOOL		RegistryDisableHighDpiAware(WCHAR* szPath=NULL);

	static BOOL		CheckDigit(WCHAR* szValue);
};
