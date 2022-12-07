
// SLP30_MCSDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "SLP30_MCS.h"
#include "SLP30_MCSDlg.h"
#include "afxdialogex.h"
#include "Server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSLP30_MCSDlg ��ȭ ����



CSLP30_MCSDlg::CSLP30_MCSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SLP30_MCS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSLP30_MCSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_richEdit);
	DDX_Control(pDX, IDC_CHECK_SCROLL, m_checkScroll);
}

BEGIN_MESSAGE_MAP(CSLP30_MCSDlg, CDialogEx)
	ON_MESSAGE(LOG_MESSAGE, OnLogMessage)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CSLP30_MCSDlg::OnBnClickedButtonStart)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSLP30_MCSDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CSLP30_MCSDlg �޽��� ó����

BOOL CSLP30_MCSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	SetWindowText(_T("SLP30 ���� ��Ʈ�� ����"));

	m_nServerPort = 10345;
	m_nLimitClient = 1000;
	m_bStartServer = false;
	m_checkScroll.SetCheck(true);

	::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

	CString sPort;
	sPort.Format(_T("%d"), m_nServerPort);
	SetDlgItemText(IDC_EDIT_PORT, sPort);

	Log::Setup();
	Log::SetMainPointer(this);

	Log::EnableTrace(true);

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);

	SetTimer(1, 1000, NULL);

	MoveWindow(0, 0, 1200, 800);
	CenterWindow();

	Log::WriteLog(0, "Program started....\n");

	SetTimer(2, 500, NULL);

	dbInformation info;
	info.nPort = 3306;
	strcpy_s(info.szDBInfoName, "gfsm");
	strcpy_s(info.szDBName, "slp30");
	strcpy_s(info.szID, "slp30_manager");

	//20221121 GBM start - slp30_manager test
#if 1
	strcpy_s(info.szIP, "160.202.162.3");
#else
	strcpy_s(info.szIP, "127.0.0.1");
#endif
	//20221121 GBM end

	strcpy_s(info.szPW, "gfs1020!@");

	CDBPool::New();
	CDBPool::Instance()->SetDBInformation(info);
	CDBPool::Instance()->StartDBPool(3);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CSLP30_MCSDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	OnBnClickedButtonStop();

	CDBPool::Delete();

	Log::WriteLog(0, "Program ended....\n");

	Log::Cleanup();
}

BOOL CSLP30_MCSDlg::WriteLog(INT nMode, LPTSTR data, ...)
{
	SYSTEMTIME	SystemTime;
	TCHAR		CurrentDate[32] = { 0, };
	TCHAR		CurrentFileName[MAX_PATH] = { 0, };
	FILE*		FilePtr = NULL;
	TCHAR		DebugLog[2048] = { 0, };

	GetLocalTime(&SystemTime);

	TCHAR szValue[64];
	switch (nMode)
	{
	case 0:
		_tcscpy_s(szValue, 64, _T("LOG"));
		break;
	case 1:
		_tcscpy_s(szValue, 64, _T("DEBUG"));
		break;
	case 2:
		_tcscpy_s(szValue, 64, _T("WARNING"));
		break;
	case 3:
		_tcscpy_s(szValue, 64, _T("ERROR"));
		break;
	default:
		return TRUE;
	}

	_sntprintf(CurrentDate, 32, _T("%02d/%02d %02d:%02d:%02d %03d"),
		/*SystemTime.wYear,*/
		SystemTime.wMonth,
		SystemTime.wDay, 
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond,
		SystemTime.wMilliseconds);

	_sntprintf(DebugLog, 2048, _T("%s  [ %s ]  %s"), CurrentDate, szValue, data);

	OutputDebugString(DebugLog);
	//_tprintf(_T("%s"), DebugLog);

	WriteFunc(DebugLog);

	return TRUE;
}

void CSLP30_MCSDlg::WriteFunc(CString strText)
{
	long lLength = m_richEdit.GetTextLengthEx(GTL_NUMCHARS);
	if (lLength > LIMIT_RICH_LOG)
	{
		m_richEdit.SetSel(0, LIMIT_RICH_LOG / 10);
		m_richEdit.ReplaceSel(L"");
	}

	CHARRANGE cr = { 0, 0 };
	m_richEdit.GetSel(cr);
	m_richEdit.SetSel(lLength, lLength);
	m_richEdit.ReplaceSel(strText);

	INT nMax = m_richEdit.GetScrollLimit(SB_VERT);
	if (m_checkScroll.GetCheck())
	{
		m_richEdit.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, nMax), NULL);
		m_richEdit.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_ENDSCROLL, nMax), NULL);
	}
}

void CSLP30_MCSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSLP30_MCSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSLP30_MCSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSLP30_MCSDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSLP30_MCSDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_richEdit.m_hWnd)
	{
		CRect rect1, rect2;
		GetClientRect(&rect1);
		rect2.SetRect(3, 70, rect1.Width() - 3, rect1.bottom - 3);
		m_richEdit.MoveWindow(rect2);
	}
}

void CSLP30_MCSDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	/*if (m_richEdit.m_hWnd)
	{
		CRect rect1, rect2;
		GetClientRect(&rect1);
		rect2.SetRect(3, 70, rect1.Width() - 3, rect1.bottom - 3);
		m_richEdit.MoveWindow(rect2);
	}*/
}

void CSLP30_MCSDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == 1)
	{
		if (m_bStartServer)
		{
			int nAccept = Server::Instance()->GetNumClients();
			int nLimit = Server::Instance()->GetNumPostAccepts();

			CString sTemp;
			sTemp.Format(_T("%d"), nLimit);
			SetDlgItemText(IDC_EDIT_LIMIT, sTemp);
			sTemp.Format(_T("%d"), nAccept);
			SetDlgItemText(IDC_EDIT_CONNECT, sTemp);
		}
	}
	else if (nIDEvent == 2)
	{
		KillTimer(2);
		OnBnClickedButtonStart();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CSLP30_MCSDlg::OnBnClickedButtonStart()
{
	Log::WriteLog(0, "Server starting....\n");
	CString sPort;
	GetDlgItemText(IDC_EDIT_PORT, sPort);
	m_nServerPort = _ttoi(sPort.GetBuffer(0));
	sPort.ReleaseBuffer();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (Network::Initialize() == false)
	{
		ERROR_MSG("Network::Initialize() failed");
		return;
	}

	Server::New();

	if (Server::Instance()->Create(m_nServerPort, m_nLimitClient) == false)
	{
		ERROR_MSG("Server::Create() failed");
		Network::Deinitialize();
		return;
	}

	Sleep(100);
	m_bStartServer = true;

	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(false);
	
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
	Log::WriteLog(0, "Server started....\n");
}

void CSLP30_MCSDlg::OnBnClickedButtonStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Log::WriteLog(0, "Server stopping....\n");

	m_bStartServer = false;
	Sleep(100);

	Server::Delete();

	Network::Deinitialize();

	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(true);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);

	Log::WriteLog(0, "Server stopped....\n");
}

LRESULT CSLP30_MCSDlg::OnLogMessage(WPARAM wParam, LPARAM lParam)
{
	CHAR* pText = (CHAR*)wParam;
	int nMode = (int)lParam;

	WriteLog(nMode, CCommonFunc::CharToTCHAR(pText));

	return 0;
}
