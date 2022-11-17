////////////////////////////////////////////////////////////////////////////////
/// \file MiniDump.cpp
/// \author excel96
/// \date 2003.11.18
///
/// ���� ���� �ɼ�... from MSDN
///
/// - MiniDumpNormal
///     Include just the information necessary to capture stack traces for 
///     all existing threads in a process. 
///
/// - MiniDumpWithDataSegs 
///     Include the data sections from all loaded modules. This results in 
///     the inclusion of global variables, which can make the minidump file 
///     significantly larger. 
///
/// - MiniDumpWithFullMemory 
///     Include all accessible memory in the process. The raw memory data is 
///     included at the end, so that the Initial structures can be mapped 
///     directly without the raw memory information. This option can result 
///     in a very large file. 
///
/// - MiniDumpWithHandleData
///     Include high-level information about the operating system handles 
///     that are active when the minidump is made. 
///     \n Windows Me/98/95: This value is not supported.
///
/// - MiniDumpFilterMemory
///     Stack and backing store memory written to the minidump file should be 
///     filtered to remove all but the pointer values necessary to reconstruct 
///     a stack trace. Typically, this removes any private information. 
///
/// - MiniDumpScanMemory 
///     Stack and backing store memory should be scanned for pointer 
///     references to modules in the module list. If a module is referenced by 
///     stack or backing store memory, the ModuleWriteFlags member of the 
///     MINIDUMP_CALLBACK_OUTPUT structure is set to ModuleReferencedByMemory. 
///
/// - MiniDumpWithUnloadedModules
///     Include information from the list of modules that were recently 
///     unloaded, if this information is maintained by the operating system.
///     \n DbgHelp 5.1 and earlier: This value is not supported.
///
/// - MiniDumpWithIndirectlyReferencedMemory 
///     Include pages with data referenced by locals or other stack memory. 
///     This option can increase the size of the minidump file significantly.
///     \n DbgHelp 5.1 and earlier:  This value is not supported.
///
/// - MiniDumpFilterModulePaths
///     Filter module paths for information such as user names or important 
///     directories. This option may prevent the system from locating the 
///     image file and should be used only in special situations.
///     \n DbgHelp 5.1 and earlier:  This value is not supported.
///
/// - MiniDumpWithProcessThreadData
///     Include complete per-process and per-thread information from the 
///     operating system.
///     \n DbgHelp 5.1 and earlier:  This value is not supported.
///
/// - MiniDumpWithPrivateReadWriteMemory 
///     Scan the virtual address space for other types of memory to be 
///     included.
///     \n DbgHelp 5.1 and earlier:  This value is not supported.
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MiniDump.h"
#include <dbghelp.h>
#include <stdio.h>
//#include <tchar.h>

//#define HAS_STACK_WALKER 1

#ifdef HAS_STACK_WALKER
    #include "StackWalker.h"
#endif

#ifndef Assert
    #include <assert.h>
    #define Assert assert
    #define LogToFile (void)(0);
#endif

cMiniDump::DumpLevel cMiniDump::s_DumpLevel          = cMiniDump::DUMP_LEVEL_LIGHT;
bool                 cMiniDump::s_AddTimeStamp       = true;
TCHAR                cMiniDump::s_AppName[_MAX_PATH] = {0,};
TCHAR                cMiniDump::s_CallStack[8192]    = {0,};
TCHAR                cMiniDump::s_Modules[8192]      = {0,};
LPCTSTR              cMiniDump::s_DialogTemplate     = NULL;
DLGPROC              cMiniDump::s_DialogProc         = NULL;

namespace
{
    /// \brief ������ ���ο� ���� ���ڿ��� ��ȯ�Ѵ�.
    LPCTSTR GetFaultReason(PEXCEPTION_POINTERS exPtrs);

    /// \brief ����� ������ ��ȯ�Ѵ�.
    LPCTSTR GetUserInfo();

    /// \brief �������� ������ ��ȯ�Ѵ�.
    LPCTSTR GetOSInfo();

    /// \brief CPU ������ ��ȯ�Ѵ�.
    LPCTSTR GetCpuInfo();

    /// \brief �޸� ������ ��ȯ�Ѵ�.
    LPCTSTR GetMemoryInfo();

    /// \brief �������� ������ �˾Ƴ���.
    //bool GetWinVersion(LPTSTR pszVersion, int *nVersion, LPTSTR pszMajorMinorBuild);

    /// \brief strrchr TCHAR ����
    TCHAR* lstrrchr(TCHAR* str, TCHAR ch);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief �̴� ���� ����� �ʱ�ȭ�Ѵ�.
/// \param dumpLevel ���� ����
/// \param addTimeStamp ���� ���� �̸����ٰ� ���� ������ ������ ��¥��
/// ����ִ°��� ����.
/// \param dialogTemplate ��ȭâ ���ø�
/// \param dialogProc ��ȭâ ���ν���
////////////////////////////////////////////////////////////////////////////////
void cMiniDump::Install(
    DumpLevel dumpLevel, bool addTimeStamp, LPCTSTR dialogTemplate, DLGPROC dialogProc)
{
    Assert(s_AppName[0] == 0);
    Assert(dumpLevel >= DUMP_LEVEL_LIGHT);
    Assert(dumpLevel <= DUMP_LEVEL_HEAVY);

    s_DumpLevel      = dumpLevel;
    s_AddTimeStamp   = addTimeStamp;
    s_DialogTemplate = dialogTemplate;
    s_DialogProc     = dialogProc;

    // ��� ��θ� �˾Ƴ���.
    // C:\somewhere\something.exe
    TCHAR szFileName[_MAX_PATH];
    ::GetModuleFileName(NULL, szFileName, _MAX_PATH);

    // Ȯ���ڸ� ������ ��� ��θ� �غ��صд�.
    // C:\somewhere\something.exe -> C:\somewhere\something
    TCHAR* dot = lstrrchr(szFileName, '.');
    ::lstrcpyn(s_AppName, szFileName, (int)(dot - szFileName + 1));

    // ���� ó�� �ڵ鷯�� �����Ѵ�.
    ::SetUnhandledExceptionFilter(WriteDump);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ���ܿ� ���� ������ �޾Ƽ�, �̴� ���� ������ �����Ѵ�. 
/// 
/// SetUnhandledExceptionFilter() API�� ���ؼ� �����ǰ�, ���μ��� ���ο��� 
/// Unhandled Exception�� �߻��� ���, ȣ��ǰ� �ȴ�. �� ����Ű� �پ��ִ� 
/// ���, Unhandled Exception Filter�� ȣ����� �ʴ´�. �� ���� �� �Լ� 
/// ���θ� ������� ���� ���ٴ� ���̴�. �� �Լ� ���θ� ������ϱ� ���ؼ��� 
/// �޽��� �ڽ� �Ǵ� ������ �̿��ؾ��Ѵ�.
/// 
/// \param exPtrs ���� ����
/// \return LONG �� �Լ��� �����ϰ� �� ����, ���� �ൿ��. �ڼ��� ���� SEH
/// ������ �����ϵ���.
////////////////////////////////////////////////////////////////////////////////
LONG WINAPI cMiniDump::WriteDump(PEXCEPTION_POINTERS exPtrs)
{
    // based on dbghelp.h
    typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(
        HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
        CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
        CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
        CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
        );

#ifdef UNICODE
    fwprintf_s(stderr, 
        L"=============================================\n"
        L"unhandled excetpion triggerd! writing dump...\n"
        L"=============================================\n"
        );
#else
    fprintf_s(stderr, 
        "=============================================\n"
        "unhandled excetpion triggerd! writing dump...\n"
        "=============================================\n"
        );
#endif

#ifdef HAS_STACK_WALKER
    // ���� �ݽ��ð� ������ ���ڿ��� �����.
    cStackWalker msw;
    msw.ShowCallStack(GetCurrentThread(), exPtrs->ContextRecord);
    ::lstrcpyn(s_CallStack, msw.GetStackString(), _ARRAYSIZE(s_CallStack)-1);
    ::lstrcpyn(s_Modules, msw.GetModuleString(), _ARRAYSIZE(s_Modules)-1);
#endif

    // ��ȭâ�� �����Ǿ� �ִٸ� �����ش�.
    if (s_DialogTemplate != NULL && s_DialogProc != NULL)
    {
        if (DialogBox(NULL, s_DialogTemplate, HWND_DESKTOP, s_DialogProc) != IDOK)
            return EXCEPTION_EXECUTE_HANDLER;
    }

    HMODULE hDLL = NULL;
    TCHAR szDbgHelpPath[_MAX_PATH] = {0, };
    TCHAR szDumpPath[MAX_PATH * 2] = {0,};
    TCHAR szLogPath[MAX_PATH * 2] = {0,};

    // ���� ���� �̸� += �ð� ���ڿ�
    ::lstrcat(szDumpPath, s_AppName);
    ::lstrcat(szLogPath, s_AppName);
    if (s_AddTimeStamp)
    {
        SYSTEMTIME t;
        ::GetLocalTime(&t);

        TCHAR szTail[_MAX_PATH];
#ifdef UNICODE
        swprintf(szTail, _ARRAYSIZE(szTail)-1, 
            L" %04d-%02d-%02d %02d-%02d-%02d",
            t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
#else
        _snprintf_s(szTail, _ARRAYSIZE(szTail)-1, _TRUNCATE,
            " %04d-%02d-%02d %02d-%02d-%02d",
            t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
#endif

        ::lstrcat(szDumpPath, szTail);
        ::lstrcat(szLogPath, szTail);
    }
    ::lstrcat(szDumpPath, _T(".dmp"));
    ::lstrcat(szLogPath, _T(".log"));

    // ���� ���� ������ �ִ� ���丮���� DBGHELP.DLL�� �ε��� ����.
    // Windows 2000 �� System32 ���丮�� �ִ� DBGHELP.DLL ������ ������ 
    // ������ ���� �� �ֱ� �����̴�. (�ּ� 5.1.2600.0 �̻��̾�� �Ѵ�.)
    if (::GetModuleFileName(NULL, szDbgHelpPath, _MAX_PATH))
    {
        if (LPTSTR slash = ::lstrrchr(szDbgHelpPath, '\\'))
        {
            ::lstrcpy(slash + 1, _T("DBGHELP.DLL"));
            hDLL = ::LoadLibrary(szDbgHelpPath);
        }
    }

    // ���� ���丮�� ���ٸ�, �ƹ� �����̳� �ε��Ѵ�.
    if (hDLL == NULL) hDLL = ::LoadLibrary(_T("dbghelp.dll"));

    // DBGHELP.DLL�� ã�� �� ���ٸ� �� �̻� ������ �� ����.
    if (hDLL == NULL)
    {
        LogToFile(szLogPath, _T("dbghelp.dll not found"));
        return EXCEPTION_CONTINUE_SEARCH;
    }

    // DLL ���ο��� MiniDumpWriteDump API�� ã�´�.
    MINIDUMPWRITEDUMP pfnMiniDumpWriteDump = 
        (MINIDUMPWRITEDUMP)::GetProcAddress(hDLL, "MiniDumpWriteDump");
    if (pfnMiniDumpWriteDump == NULL)
    {
        LogToFile(szLogPath, _T("dbghelp.dll too old"));
        return EXCEPTION_CONTINUE_SEARCH;
    }

    // ������ �����Ѵ�.
    HANDLE hFile = ::CreateFile(
        szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, 
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        LogToFile(szLogPath, _T("failed to create dump file '%s' (error %d)"), 
            szDumpPath, ::GetLastError());
        return EXCEPTION_CONTINUE_SEARCH;
    }

    MINIDUMP_EXCEPTION_INFORMATION ExParam;
    ExParam.ThreadId = ::GetCurrentThreadId();
    ExParam.ExceptionPointers = exPtrs;
    ExParam.ClientPointers = FALSE;

    MINIDUMP_TYPE dumptype = MiniDumpNormal;
    switch (s_DumpLevel)
    {
    case DUMP_LEVEL_LIGHT:  dumptype = MiniDumpNormal;         break;
    case DUMP_LEVEL_MEDIUM: dumptype = MiniDumpWithDataSegs;   break;
    case DUMP_LEVEL_HEAVY:  dumptype = MiniDumpWithFullMemory; break;
    }

    // ���� ���� ���� ����� �α� ���Ͽ��� ����Ѵ�.
    if (pfnMiniDumpWriteDump(
        ::GetCurrentProcess(), ::GetCurrentProcessId(), 
        hFile, dumptype, &ExParam, NULL, NULL))
    {
#ifdef UNICODE
        FILE* log = NULL;
        if (_wfopen_s(&log, szLogPath, _T("a")) == 0)
#else
        FILE* log = NULL;
        if (fopen_s(&log, szLogPath, "a") == 0)
#endif
        {
            fprintf(log,
                "saved dump file to '%s'\n"
                "\n<fault reason>\n%s"
                "\n\n<user>\n%s"
                "\n\n<cpu>\n%s"
                "\n\n<memory>\n%s",
                szDumpPath,
                GetFaultReason(exPtrs),
                GetUserInfo(),
                GetCpuInfo(),
                GetMemoryInfo()
                );

#ifdef HAS_STACK_WALKER
            fprintf(log, "\n\n<stack>\n%s\n", GetCallStack());
            fprintf(log, "\n<modules>\n%s\n", GetModules());
#endif
            fclose(log);
        }

    }
    else
    {
        LogToFile(szLogPath, _T("failed to save dump file to '%s' (error %d)"), szDumpPath, ::GetLastError());
        Assert(false);
    }

    ::CloseHandle(hFile);

    return EXCEPTION_EXECUTE_HANDLER;
}

namespace
{
    /// \brief ������ ���ο� ���� ���ڿ��� ��ȯ�Ѵ�.
    /// \param exPtrs ���� ����ü ������
    /// \return LPCTSTR ���� ���ڿ�
    LPCTSTR GetFaultReason(PEXCEPTION_POINTERS exPtrs)
    {
        if (::IsBadReadPtr(exPtrs, sizeof(EXCEPTION_POINTERS))) 
            return _T("bad exception pointers");

        // ������ ���� �ڵ��� �׳� ��ȯ�� �� �ִ�.
        switch (exPtrs->ExceptionRecord->ExceptionCode)
        {
        case EXCEPTION_ACCESS_VIOLATION:         return _T("EXCEPTION_ACCESS_VIOLATION");
        case EXCEPTION_DATATYPE_MISALIGNMENT:    return _T("EXCEPTION_DATATYPE_MISALIGNMENT");
        case EXCEPTION_BREAKPOINT:               return _T("EXCEPTION_BREAKPOINT");
        case EXCEPTION_SINGLE_STEP:              return _T("EXCEPTION_SINGLE_STEP");
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return _T("EXCEPTION_ARRAY_BOUNDS_EXCEEDED");
        case EXCEPTION_FLT_DENORMAL_OPERAND:     return _T("EXCEPTION_FLT_DENORMAL_OPERAND");
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return _T("EXCEPTION_FLT_DIVIDE_BY_ZERO");
        case EXCEPTION_FLT_INEXACT_RESULT:       return _T("EXCEPTION_FLT_INEXACT_RESULT");
        case EXCEPTION_FLT_INVALID_OPERATION:    return _T("EXCEPTION_FLT_INVALID_OPERATION");
        case EXCEPTION_FLT_OVERFLOW:             return _T("EXCEPTION_FLT_OVERFLOW");
        case EXCEPTION_FLT_STACK_CHECK:          return _T("EXCEPTION_FLT_STACK_CHECK");
        case EXCEPTION_FLT_UNDERFLOW:            return _T("EXCEPTION_FLT_UNDERFLOW");
        case EXCEPTION_INT_DIVIDE_BY_ZERO:       return _T("EXCEPTION_INT_DIVIDE_BY_ZERO");
        case EXCEPTION_INT_OVERFLOW:             return _T("EXCEPTION_INT_OVERFLOW");
        case EXCEPTION_PRIV_INSTRUCTION:         return _T("EXCEPTION_PRIV_INSTRUCTION");
        case EXCEPTION_IN_PAGE_ERROR:            return _T("EXCEPTION_IN_PAGE_ERROR");
        case EXCEPTION_ILLEGAL_INSTRUCTION:      return _T("EXCEPTION_ILLEGAL_INSTRUCTION");
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: return _T("EXCEPTION_NONCONTINUABLE_EXCEPTION");
        case EXCEPTION_STACK_OVERFLOW:           return _T("EXCEPTION_STACK_OVERFLOW");
        case EXCEPTION_INVALID_DISPOSITION:      return _T("EXCEPTION_INVALID_DISPOSITION");
        case EXCEPTION_GUARD_PAGE:               return _T("EXCEPTION_GUARD_PAGE");
        case EXCEPTION_INVALID_HANDLE:           return _T("EXCEPTION_INVALID_HANDLE");
            //case EXCEPTION_POSSIBLE_DEADLOCK:        return _T("EXCEPTION_POSSIBLE_DEADLOCK");
        case CONTROL_C_EXIT:                     return _T("CONTROL_C_EXIT");
        case 0xE06D7363:                         return _T("Microsoft C++ Exception");
        default:
            break;
        }

        // ���� �� �� ������ �������...
        static TCHAR szFaultReason[2048];
        ::lstrcpy(szFaultReason, _T("Unknown")); 
        ::FormatMessage(
            FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
            ::GetModuleHandle(_T("ntdll.dll")),
            exPtrs->ExceptionRecord->ExceptionCode, 
            0,
            szFaultReason,
            0,
            NULL);

        return szFaultReason;
    }

    /// \brief ����� ������ ��ȯ�Ѵ�.
    /// \return LPCTSTR ����� �̸�
    LPCTSTR GetUserInfo()
    {
        static TCHAR szUserName[200] = {0,};

        ZeroMemory(szUserName, sizeof(szUserName));
        DWORD UserNameSize = _ARRAYSIZE(szUserName) - 1;

        if (!::GetUserName(szUserName, &UserNameSize))
            ::lstrcpy(szUserName, _T("Unknown"));

        return szUserName;
    }

    /// \brief �������� ������ ��ȯ�Ѵ�.
    /// \return LPCTSTR �������� ���� ���ڿ�
    LPCTSTR GetOSInfo()
    {
       /* TCHAR szWinVer[50] = {0,};
        TCHAR szMajorMinorBuild[50] = {0,};
        int nWinVer = 0;
        ::GetWinVersion(szWinVer, &nWinVer, szMajorMinorBuild);*/

        static TCHAR szOSInfo[512] = {0,};
//#ifdef UNICODE
//        swprintf(szOSInfo, _ARRAYSIZE(szOSInfo)-1, L"%s (%s)",
//            szWinVer, szMajorMinorBuild);
//#else
//        _snprintf_s(szOSInfo, _ARRAYSIZE(szOSInfo)-1, _TRUNCATE, "%s (%s)",
//            szWinVer, szMajorMinorBuild);
//#endif
//        szOSInfo[_ARRAYSIZE(szOSInfo)-1] = 0;
        return szOSInfo;
    }

    /// \brief CPU ������ ��ȯ�Ѵ�.
    /// \return LPCTSTR CPU ���� ���ڿ�
    LPCTSTR GetCpuInfo()
    {
        // CPU ���� ���
        SYSTEM_INFO    SystemInfo;
        GetSystemInfo(&SystemInfo);

        static TCHAR szCpuInfo[512] = {0,};
#ifdef UNICODE
        swprintf(szCpuInfo, _ARRAYSIZE(szCpuInfo)-1, 
            L"%d processor(s), type %d",
            SystemInfo.dwNumberOfProcessors, SystemInfo.dwProcessorType);
#else
        _snprintf_s(szCpuInfo, _ARRAYSIZE(szCpuInfo)-1, _TRUNCATE, 
            "%d processor(s), type %d",
            SystemInfo.dwNumberOfProcessors, SystemInfo.dwProcessorType);
#endif
        return szCpuInfo;
    }

    /// \brief �޸� ������ ��ȯ�Ѵ�.
    /// \return LPCTSTR �޸� ���� ���ڿ�
    LPCTSTR GetMemoryInfo()
    {
        static const int ONE_K = 1024;
        static const int ONE_M = ONE_K * ONE_K;
        static const int ONE_G = ONE_K * ONE_K * ONE_K;

        MEMORYSTATUS MemInfo;
        MemInfo.dwLength = sizeof(MemInfo);
        GlobalMemoryStatus(&MemInfo);

        static TCHAR szMemoryInfo[2048] = {0,};
#ifdef UNICODE
        swprintf(szMemoryInfo, _ARRAYSIZE(szMemoryInfo)-1, 
            L"%d%% of memory in use.\n"
            L"%d MB physical memory.\n"
            L"%d MB physical memory free.\n"
            L"%d MB paging file.\n"
            L"%d MB paging file free.\n"
            L"%d MB user address space.\n"
            L"%d MB user address space free.",
            MemInfo.dwMemoryLoad, 
            (MemInfo.dwTotalPhys + ONE_M - 1) / ONE_M, 
            (MemInfo.dwAvailPhys + ONE_M - 1) / ONE_M, 
            (MemInfo.dwTotalPageFile + ONE_M - 1) / ONE_M, 
            (MemInfo.dwAvailPageFile + ONE_M - 1) / ONE_M, 
            (MemInfo.dwTotalVirtual + ONE_M - 1) / ONE_M, 
            (MemInfo.dwAvailVirtual + ONE_M - 1) / ONE_M);
#else
        _snprintf_s(szMemoryInfo, _ARRAYSIZE(szMemoryInfo)-1, _TRUNCATE,
            "%d%% of memory in use.\n"
            "%d MB physical memory.\n"
            "%d MB physical memory free.\n"
            "%d MB paging file.\n"
            "%d MB paging file free.\n"
            "%d MB user address space.\n"
            "%d MB user address space free.",
            MemInfo.dwMemoryLoad, 
            (MemInfo.dwTotalPhys + ONE_M - 1) / ONE_M, 
            (MemInfo.dwAvailPhys + ONE_M - 1) / ONE_M, 
            (MemInfo.dwTotalPageFile + ONE_M - 1) / ONE_M, 
            (MemInfo.dwAvailPageFile + ONE_M - 1) / ONE_M, 
            (MemInfo.dwTotalVirtual + ONE_M - 1) / ONE_M, 
            (MemInfo.dwAvailVirtual + ONE_M - 1) / ONE_M);
#endif

        return szMemoryInfo;
    }

    /// \brief �������� ������ �˾Ƴ���.
    ///
    /// This table has been assembled from Usenet postings, personal observations, 
    /// and reading other people's code.  Please feel free to add to it or correct 
    /// it.
    ///
    /// <pre>
    /// dwPlatFormID  dwMajorVersion  dwMinorVersion  dwBuildNumber
    /// 95            1               4                 0            950
    /// 95 SP1        1               4                 0            >950 && <=1080
    /// 95 OSR2       1               4               <10            >1080
    /// 98            1               4                10            1998
    /// 98 SP1        1               4                10            >1998 && <2183
    /// 98 SE         1               4                10            >=2183
    /// ME            1               4                90            3000
    ///
    /// NT 3.51       2               3                51
    /// NT 4          2               4                 0            1381
    /// 2000          2               5                 0            2195
    /// XP            2               5                 1            2600
    /// 2003 Server   2               5                 2            3790
    ///
    /// CE            3
    /// </pre>
    ///
    /// \param pszVersion ���� ���ڿ��� ������� ������
    /// \param nVersion ���� ���ڰ��� ������� ������
    /// \param pszMajorMinorBuild ���� ���ڿ��� ������� ������
    /// \return bool ������ ������ ��쿡�� true, ���� ������ ���� ��쿡�� false
//    bool GetWinVersion(LPTSTR pszVersion, int *nVersion, LPTSTR pszMajorMinorBuild)
//    {
//        // from winbase.h
//#ifndef VER_PLATFORM_WIN32s
//    #define VER_PLATFORM_WIN32s 0
//#endif
//
//#ifndef VER_PLATFORM_WIN32_WINDOWS
//    #define VER_PLATFORM_WIN32_WINDOWS 1
//#endif
//
//#ifndef VER_PLATFORM_WIN32_NT
//    #define VER_PLATFORM_WIN32_NT 2
//#endif
//
//#ifndef VER_PLATFORM_WIN32_CE
//    #define VER_PLATFORM_WIN32_CE 3
//#endif
//
//        static LPCTSTR WUNKNOWNSTR     = _T("Unknown Windows Version");
//        static LPCTSTR W95STR          = _T("Windows 95");
//        static LPCTSTR W95SP1STR       = _T("Windows 95 SP1");
//        static LPCTSTR W95OSR2STR      = _T("Windows 95 OSR2");
//        static LPCTSTR W98STR          = _T("Windows 98");
//        static LPCTSTR W98SP1STR       = _T("Windows 98 SP1");
//        static LPCTSTR W98SESTR        = _T("Windows 98 SE");
//        static LPCTSTR WMESTR          = _T("Windows ME");
//        static LPCTSTR WNT351STR       = _T("Windows NT 3.51");
//        static LPCTSTR WNT4STR         = _T("Windows NT 4");
//        static LPCTSTR W2KSTR          = _T("Windows 2000");
//        static LPCTSTR WXPSTR          = _T("Windows XP");
//        static LPCTSTR W2003SERVERSTR  = _T("Windows 2003 Server");
//        static LPCTSTR WCESTR          = _T("Windows CE");
//
//        static const int WUNKNOWN      = 0;
//        static const int W9XFIRST      = 1;
//        static const int W95           = 1;
//        static const int W95SP1        = 2;
//        static const int W95OSR2       = 3;
//        static const int W98           = 4;
//        static const int W98SP1        = 5;
//        static const int W98SE         = 6;
//        static const int WME           = 7;
//        static const int W9XLAST       = 99;
//        static const int WNTFIRST      = 101;
//        static const int WNT351        = 101;
//        static const int WNT4          = 102;
//        static const int W2K           = 103;
//        static const int WXP           = 104;
//        static const int W2003SERVER   = 105;
//        static const int WNTLAST       = 199;
//        static const int WCEFIRST      = 201;
//        static const int WCE           = 201;
//        static const int WCELAST       = 299;
//
//        if (!pszVersion || !nVersion || !pszMajorMinorBuild) return false;
//
//        ::lstrcpy(pszVersion, WUNKNOWNSTR);
//        *nVersion = WUNKNOWN;
//
//        OSVERSIONINFO osinfo;
//        osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
//
//        if (!GetVersionEx(&osinfo)) return false;
//
//        DWORD dwPlatformId   = osinfo.dwPlatformId;
//        DWORD dwMinorVersion = osinfo.dwMinorVersion;
//        DWORD dwMajorVersion = osinfo.dwMajorVersion;
//        DWORD dwBuildNumber  = osinfo.dwBuildNumber & 0xFFFF; // Win 95 needs this
//
//        TCHAR buf[50] = {0, };
//#ifdef UNICODE
//        swprintf(buf, _ARRAYSIZE(buf), L"%u.%u.%u", 
//            dwMajorVersion, dwMinorVersion, dwBuildNumber);
//#else
//        _snprintf_s(buf, _ARRAYSIZE(buf), _TRUNCATE, "%u.%u.%u", 
//            dwMajorVersion, dwMinorVersion, dwBuildNumber);
//#endif
//        ::lstrcpy(pszMajorMinorBuild, buf);
//
//        if ((dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (dwMajorVersion == 4))
//        {
//            if ((dwMinorVersion < 10) && (dwBuildNumber == 950))
//            {
//                ::lstrcpy(pszVersion, W95STR);
//                *nVersion = W95;
//            }
//            else if ((dwMinorVersion < 10) && 
//                ((dwBuildNumber > 950) && (dwBuildNumber <= 1080)))
//            {
//                ::lstrcpy(pszVersion, W95SP1STR);
//                *nVersion = W95SP1;
//            }
//            else if ((dwMinorVersion < 10) && (dwBuildNumber > 1080))
//            {
//                ::lstrcpy(pszVersion, W95OSR2STR);
//                *nVersion = W95OSR2;
//            }
//            else if ((dwMinorVersion == 10) && (dwBuildNumber == 1998))
//            {
//                ::lstrcpy(pszVersion, W98STR);
//                *nVersion = W98;
//            }
//            else if ((dwMinorVersion == 10) && 
//                ((dwBuildNumber > 1998) && (dwBuildNumber < 2183)))
//            {
//                ::lstrcpy(pszVersion, W98SP1STR);
//                *nVersion = W98SP1;
//            }
//            else if ((dwMinorVersion == 10) && (dwBuildNumber >= 2183))
//            {
//                ::lstrcpy(pszVersion, W98SESTR);
//                *nVersion = W98SE;
//            }
//            else if (dwMinorVersion == 90)
//            {
//                ::lstrcpy(pszVersion, WMESTR);
//                *nVersion = WME;
//            }
//        }
//        else if (dwPlatformId == VER_PLATFORM_WIN32_NT)
//        {
//            if ((dwMajorVersion == 3) && (dwMinorVersion == 51))
//            {
//                ::lstrcpy(pszVersion, WNT351STR);
//                *nVersion = WNT351;
//            }
//            else if ((dwMajorVersion == 4) && (dwMinorVersion == 0))
//            {
//                ::lstrcpy(pszVersion, WNT4STR);
//                *nVersion = WNT4;
//            }
//            else if ((dwMajorVersion == 5) && (dwMinorVersion == 0))
//            {
//                ::lstrcpy(pszVersion, W2KSTR);
//                *nVersion = W2K;
//            }
//            else if ((dwMajorVersion == 5) && (dwMinorVersion == 1))
//            {
//                ::lstrcpy(pszVersion, WXPSTR);
//                *nVersion = WXP;
//            }
//            else if ((dwMajorVersion == 5) && (dwMinorVersion == 2))
//            {
//                ::lstrcpy(pszVersion, W2003SERVERSTR);
//                *nVersion = W2003SERVER;
//            }
//        }
//        else if (dwPlatformId == VER_PLATFORM_WIN32_CE)
//        {
//            ::lstrcpy(pszVersion, WCESTR);
//            *nVersion = WCE;
//        }
//
//        return true;
//
//#undef VER_PLATFORM_WIN32s
//#undef VER_PLATFORM_WIN32_WINDOWS
//#undef VER_PLATFORM_WIN32_NT
//#undef VER_PLATFORM_WIN32_CE
//    }
    
    /// \brief strrchr TCHAR ����
    /// \param str �˻��� ���ڿ�
    /// \param ch ã���� �ϴ� ����
    /// \return TCHAR* �־��� ���ڸ� ã�� ��� �ش� ��ġ �����͸� ��ȯ�ϰ�, 
    /// ã�� ���� ��쿡�� NULL�� ��ȯ�Ѵ�.
    TCHAR* lstrrchr(TCHAR* str, TCHAR ch)
    {
        TCHAR* start = str;
        while (*str++) ;
        while (--str != start && *str != ch) ;
        return *str == ch ? str : NULL;
    }
}