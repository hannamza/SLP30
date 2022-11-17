
// SLP30_ManagerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "SLP30_Manager.h"
#include "SLP30_ManagerDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"
#include "LoginDlg.h"
#include "SecurityUsbDlg.h"
#include "KISA_SEED_CBC.h"
#include "SharedMemoryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSLP30_ManagerDlg ��ȭ ����



CSLP30_ManagerDlg::CSLP30_ManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SLP30_MANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInit = false;

	m_font.CreateFont(16, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("����")); // lpszFacename 
	m_nUserSeq = 0;
	m_bDestroy = false;
	m_pCheckUsb = NULL;
}

void CSLP30_ManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ETC, m_editEtc);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_PHONE, m_editPhone);
	DDX_Control(pDX, IDC_LIST_USER, m_listUser);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_EDIT_PW, m_editPW);
}

BEGIN_MESSAGE_MAP(CSLP30_ManagerDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnAddClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnModClick)
	ON_COMMAND(IDC_COMMON_BUTTON3, OnDelClick)

	ON_COMMAND(IDC_COMMON_BUTTON4, OnMiniClick)
	ON_COMMAND(IDC_COMMON_BUTTON5, OnCloseClick)

	ON_MESSAGE(REDISPLAY_MSG, OnRedisplayMsg)
	ON_MESSAGE(SOCKET_RECV, OnSocketRecvMsg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_MOVE()
	ON_WM_CTLCOLOR()
	ON_WM_MOVING()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCHITTEST()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_USER, &CSLP30_ManagerDlg::OnItemchangedListUser)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CSLP30_ManagerDlg::OnItemdblclickListUser)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USER, &CSLP30_ManagerDlg::OnDblclkListUser)
END_MESSAGE_MAP()

// CSLP30_ManagerDlg �޽��� ó����
const TCHAR* lpszHeaderManager[] = { _T("NO"), _T("�̸�"), _T("��ȭ��ȣ"), _T("���̵�"), _T("���"), NULL };
const int colWidthListManager[] = { 5, 16, 23, 16, 35, 0 };

BOOL CSLP30_ManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	Log::Setup();
	Log::SetMainPointer(this);

	CPositionManager::New();
	//CUSBDriveManger::New();
	CClientInterface::New();
	CClientInterface::Instance()->TryConnection("160.202.162.3", 10345);

	CUserManager::New();
	CCommonState::New();
	CCommonState::Instance()->m_pUserWnd = this;

	SetWindowText(L"SLP30 ����� ���� ���α׷�");

	Global.SetFont(IDX_FONT_SMALL, _T("����"), 9);
	Global.SetFont(IDX_FONT_MEDIUM, _T("����"), 10);
	Global.SetFont(IDX_FONT_LARGE, _T("����"), 11, TRUE);
	Global.SetFont(IDX_FONT_HEADER, _T("����"), 10, TRUE);

	CCommonState::Instance()->m_pWnd = this;

	CRect rect;
	if (CPositionManager::Instance()->LoadPosition(this, rect)) {
		this->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	}
	else {
		this->SetWindowPos(NULL, 0, 0, 1024, 768, SWP_NOMOVE);
	}
	//CenterWindow();

	SetTimer(99, 5 * 1000, NULL);

	m_editName.MoveWindow(160, 182, 230, 20);
	m_editPhone.MoveWindow(492, 182, 230, 20);
	m_editID.MoveWindow(160, 237, 230, 20);
	m_editPW.MoveWindow(492, 237, 230, 20);
	m_editEtc.MoveWindow(158, 283, 565, 58);
	m_listUser.MoveWindow(22, 398, 980, 348);

	m_btnAdd.Create(IDB_BMP_ADD, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnMod.Create(IDB_BMP_MOD, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnDel.Create(IDB_BMP_DEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
	m_btnMini.Create(IDB_BMP_MAX, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON4);
	m_btnClose.Create(IDB_BMP_CLOSE, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON5);

	m_pCheckUsb = new CCheckTextBtn(0, L"����Ű(USB)�� �߱��Ͻ� ��� üũ�Ͽ� �ֽʽÿ�.", this);
	m_pCheckUsb->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pCheckUsb->MoveWindow(135, 342, m_pCheckUsb->GetWidthExtent() + 22, 20);
	m_pCheckUsb->ShowWindow(SW_HIDE);

	m_btnMini.MoveWindow(925, 20, 28, 28);
	m_btnClose.MoveWindow(969, 20, 28, 28);

	m_btnAdd.MoveWindow(804, 185, 156, 44);
	m_btnMod.MoveWindow(804, 237, 156, 44);
	m_btnDel.MoveWindow(804, 289, 156, 44);

	CXListCtrl::InitList(m_listUser, lpszHeaderManager);
	CXListCtrl::ResizeListColumn(m_listUser, colWidthListManager);

	CSharedMemoryManager::New();
	if (!CSharedMemoryManager::Instance()->CreateSharedMemory()) {
		CMessagePopup popup(L"���� ����", L"\n\nSMC������ �����Ͽ����ϴ�.", MB_OK, this);
		popup.DoModal();
		EndDialog(0);
		return FALSE;
	}
	if (!CSharedMemoryManager::Instance()->OpenSharedMemory()) {
		CMessagePopup popup(L"���� ����", L"\n\nSMC������ �����Ͽ����ϴ�.", MB_OK, this);
		popup.DoModal();
		EndDialog(0);
		return FALSE;
	}

	SetTimer(1, 100, NULL);

	m_bInit = true;
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CSLP30_ManagerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	CPositionManager::Instance()->SavePosition(this);

	m_bDestroy = true;

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CClientInterface::Instance()->TryShutdown();
	CClientInterface::Instance()->Destroy();
	CClientInterface::Delete();

	CCommonState::Delete();
	CUserManager::Delete();
	//CUSBDriveManger::Delete();
	CPositionManager::Delete();

	SAFE_DELETE(m_pCheckUsb);

	Log::Cleanup();
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSLP30_ManagerDlg::OnPaint()
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

		Redisplay();

		if (m_bInit) {
			m_editName.Invalidate();
			m_editPhone.Invalidate();
			m_editEtc.Invalidate();
			m_listUser.Invalidate();

			m_btnAdd.Invalidate();
			m_btnMod.Invalidate();
			m_btnDel.Invalidate();
			m_btnMini.Invalidate();
			m_btnClose.Invalidate();
		}
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSLP30_ManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSLP30_ManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			if (pMsg->wParam == VK_RETURN)
			{
				CWnd* pWnd = GetFocus();
				if (pWnd && pWnd->m_hWnd == m_editEtc.m_hWnd)
				{
					m_editEtc.SetSel(-1, 0);
					m_editEtc.ReplaceSel(L"\r\n");
				}

				return TRUE;
			}
			else if (pMsg->wParam == VK_ESCAPE)
			{
				return TRUE;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSLP30_ManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (!m_bInit) {
		return;
	}
}

void CSLP30_ManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == 1) {
		KillTimer(1);

		CLoginDlg dlg;
		UINT nResult = dlg.DoModal();
		if (nResult != IDOK) {
			OnOK();
			return;
		}
		CCommonState::Instance()->m_bLoginResult = false;
		CCommonState::Instance()->m_sID = dlg.m_sID;
		CCommonState::Instance()->m_nIdx = 0;
		CClientInterface::Instance()->ProcessRequestLogin(dlg.m_sID.GetBuffer(0), dlg.m_sPW.GetBuffer(0));
		DWORD dwLastTime = GetTickCount();
		while (GetTickCount() - dwLastTime < 3000)
		{
			if (CCommonState::Instance()->m_bLoginResult) {
				break;
			}
			Sleep(1);
		}
		if (CCommonState::Instance()->m_nIdx > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestUserList, CCommonState::Instance()->m_nIdx);
		}
		else {
			CMessagePopup dlg(L"�α���", L"\n\n�α��� ���̵� �Ǵ� ��й�ȣ�� ���� �ʽ��ϴ�.\n\n�ٽ� �õ��Ͽ� �ֽʽÿ�.", MB_OK);
			dlg.DoModal();
			SetTimer(1, 10, NULL);
		}
	}
	else if (nIDEvent == 99) {
		CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::Keep_Alive, 0);
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CSLP30_ManagerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CSLP30_ManagerDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (!m_bInit) {
		return;
	}
}

void CSLP30_ManagerDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (!m_bInit) {
		return;
	}
}

HBRUSH CSLP30_ManagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


void CSLP30_ManagerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y = 768;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

LRESULT CSLP30_ManagerDlg::OnRedisplayMsg(WPARAM wParam, LPARAM lParam)
{
	int nType = wParam;
	int nValue = lParam;
	switch (nType)
	{
	case 0: ReListup(); break;
	case 1: m_listUser.DeleteAllItems(); break;
	}

	return 0;
}

void CSLP30_ManagerDlg::ReListup()
{
	m_listUser.DeleteAllItems();

	CString sTemp;
	pUserInfo pInfo;
	int nCount = CUserManager::Instance()->GetCount();
	//_T("No"), _T("�̸�"), _T("��ȭ��ȣ"), _T("���")
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pInfo = CUserManager::Instance()->GetUserInfo(nIndex);
		if (pInfo) {
			sTemp.Format(L"%d", nIndex + 1);
			m_listUser.InsertItem(nIndex, sTemp, RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemText(nIndex, 1, pInfo->szName, RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemText(nIndex, 2, pInfo->szPhone, RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemText(nIndex, 3, pInfo->szID, RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemText(nIndex, 4, pInfo->szEtc, RGB(80, 80, 80), RGB(255, 255, 255));
			m_listUser.SetItemData(nIndex, (DWORD)pInfo);
		}
	}
}

void CSLP30_ManagerDlg::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawImage(&memDC, IDB_BMP_BG);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	::DeleteObject(memDC);
	ReleaseDC(_pDC);
}

LRESULT CSLP30_ManagerDlg::OnSocketRecvMsg(WPARAM wParam, LPARAM lParam)
{
	int nProtocol = wParam - 100;
	int nValue = lParam;
	switch (nProtocol)
	{
	case ProtocolHeader::ResponseLogin:
		break;
	case ProtocolHeader::ResponseUserAdd:
		if (nValue == 0 && CCommonState::Instance()->m_nIdx > 0) {
			CCreateProcessReturnValue createProcess;

			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestUserList, CCommonState::Instance()->m_nIdx);

			/*BYTE g_pbszUK[] = { 0x27, 0x6d, 0x66, 0x47, 0x35, 0xe8, 0x1d, 0x6a, 0x6f, 0x1a, 0x39, 0x7a, 0x36, 0x16, 0x21, 0x1a, 0x00 };
			BYTE g_pbszIV[] = { 0xa1, 0xd1, 0x23, 0x2a, 0xc1, 0xc7, 0xa2, 0x28, 0x89, 0xe1, 0x6c, 0xb5, 0x78, 0x01, 0x41, 0xd1, 0x00 };
			BYTE g_pbszIV0[] = { 0x11, 0xa6, 0x33, 0xa6, 0xa1, 0x07, 0x12, 0xdd, 0x54, 0x9a, 0x2c, 0xe3, 0x38, 0x2a, 0x77, 0xbb, 0x00 };
			BYTE g_pbszIV1[] = { 0x11, 0xd6, 0x33, 0xb6, 0xa1, 0x07, 0xb2, 0xdd, 0x54, 0x6b, 0x21, 0xa3, 0x38, 0x4a, 0x32, 0xe0, 0x00 };
			BYTE g_pbszIV2[] = { 0xc3, 0x27, 0x32, 0x16, 0x9c, 0x7a, 0x12, 0xcc, 0xa4, 0x1c, 0x22, 0x13, 0xb3, 0x2c, 0xe8, 0x0e, 0x00 };
			BYTE g_pbszIV3[] = { 0xd2, 0x18, 0x61, 0x36, 0x6a, 0x43, 0xa2, 0xbb, 0x40, 0x2d, 0x23, 0x23, 0xbb, 0xaa, 0x8e, 0x78, 0x00 };*/

			uploadInfo info;
			wcscpy_s(info.szLocalPath, L"");
			wcscpy_s(info.szDate, L"");
			wcscpy_s(info.szFileName, L"");
			wcscpy_s(info.szFolder, L"");
			wcscpy_s(info.szUserID, CCommonState::Instance()->m_sUserID.GetBuffer(0));
			wcscpy_s(info.szYear, L"");

			CCommonState::Instance()->m_nReturnValue = -99;
			createProcess.SendDataToWebdav(L"", L"", CCommonState::Instance()->m_sUserID);
			while (CCommonState::Instance()->m_nReturnValue == -99) {
				Sleep(50);
			}

			CMessagePopup dlg(L"����� �߰�", L"\n\n����� �߰��� �Ϸ�Ǿ����ϴ�.", MB_OK, this);
			dlg.DoModal();
			int ddd = CCommonState::Instance()->m_nReturnValue;
			ddd += 1;

			/*BYTE value[64];
			memset(value, 0x00, 64);
			int nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV0, g_pbszIV, 16, value);
			memcpy_s(g_pbszIV, 16, value, 16);
			nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV1, g_pbszIV, 16, value);
			memcpy_s(g_pbszIV, 16, value, 16);
			nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV2, g_pbszIV, 16, value);
			memcpy_s(g_pbszIV, 16, value, 16);
			g_pbszIV[4] = '0x45';

			BYTE szUploadInfo[8192];
			memset(szUploadInfo, 0x00, 8192);
			DWORD dwReadden = 0;
			nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV, (BYTE*)&info, sizeof(uploadInfo), szUploadInfo);

			CSharedMemoryManager::Instance()->WriteSharedMemory((BYTE*)&info, nValue);

			WCHAR szPath[2048], szArg[16];
			GetModuleFileName(NULL, szPath, 2048);
			PathRemoveFileSpec(szPath);
			wcscat(szPath, L"\\WebDavClient.exe");
			swprintf_s(szArg, L"%d", nValue);

			CCommonFunc::ShellExecuteProgram(szPath, szArg, SW_HIDE);*/
		}
		else if (nValue == -1) {
			CMessagePopup dlg(L"����� �߰�", L"\n\n�̹� �����ϴ� ��ȭ��ȣ�Դϴ�.\n\n�ٸ� ��ȭ��ȣ�� �õ��Ͽ� �ֽʽÿ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"����� �߰�", L"\n\n����� �߰��� �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseUserMod:
		if (nValue == 1 && CCommonState::Instance()->m_nIdx > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestUserList, CCommonState::Instance()->m_nIdx);
			CMessagePopup dlg(L"����� ����", L"\n\n����� ������ �Ϸ�Ǿ����ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"����� ����", L"\n\n����� ������ �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseUserDel:
		if (nValue == 1 && CCommonState::Instance()->m_nIdx > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestUserList, CCommonState::Instance()->m_nIdx);
			CMessagePopup dlg(L"����� ����", L"\n\n����� ������ �Ϸ�Ǿ����ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"����� ����", L"\n\n����� ������ �����Ͽ����ϴ�.\n\n����� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CSLP30_ManagerDlg::OnNcHitTest(CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int nHit = CDialogEx::OnNcHitTest(point);
	if (nHit == HTCLIENT) {
		nHit = HTCAPTION;
	}

	return nHit;
}

void CSLP30_ManagerDlg::OnAddClick()
{
	/*CString sDriveLetter;
	if (m_pCheckUsb->GetCheck()) {
		CSecurityUsbDlg dlg;
		UINT nValue = dlg.DoModal();
		if (nValue == IDOK) {
			sDriveLetter = dlg.GetDriveLetter();
			TRACE(L"Select driveletter. (%s)\n", sDriveLetter);
		}
		else {
			sDriveLetter = L"";
			CMessagePopup popup(L"����Ű �߱�", L"\n\n����Ű �߱��� ����ϼ̽��ϴ�.\n\n�߱��� ���� ���\n\n���� ����� ���Ͽ� �߱��Ͻ� �� �ֽ��ϴ�.", MB_OK, this);
			popup.DoModal();
		}
	}*/
	CString sName, sPhone, sEtc, sId, sPw;
	GetDlgItemText(IDC_EDIT_NAME, sName);
	GetDlgItemText(IDC_EDIT_PHONE, sPhone);
	GetDlgItemText(IDC_EDIT_ETC, sEtc);
	GetDlgItemText(IDC_EDIT_ID, sId);
	GetDlgItemText(IDC_EDIT_PW, sPw);
	if (sName.GetLength() < 2)
	{
		CMessagePopup dlg(L"����� �߰�", L"\n\n�̸��� �ʼ� �Է� �����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	sPhone.Trim();
	sPhone.Replace(L"-", L"");
	sPhone.Replace(L"/", L"");
	sPhone.Replace(L"_", L"");
	sPhone.Replace(L".", L"");
	sPhone.Replace(L"*", L"");
	sPhone.Replace(L"+", L"");
	sPhone.Replace(L"+82", L"");

	if (sId.GetLength() < 3) {
		CMessagePopup dlg(L"����� �߰�", L"\n\n���̵�� �ʼ� �Է»����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (sPw.GetLength() < 3) {
		CMessagePopup dlg(L"����� �߰�", L"\n\n�н������ �ʼ� �Է»����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CString sText;
	sText.Format(L"[�̸�: %s] [���̵�: %s]\n\n��ȭ��ȣ: %s\n\n���: %s", sName, sId, sPhone, sEtc);
	CMessagePopup dlg(L"����� �߰�", sText, L"�� ������ ����ڸ� ����Ͻðڽ��ϱ�?", MB_YESNO, DT_LEFT | DT_VCENTER, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CCommonState::Instance()->m_sUserID = sId;
	CClientInterface::Instance()->ProcessRequestAddUser(sName, sPhone, sId, sPw, sEtc);
}

void CSLP30_ManagerDlg::OnModClick()
{
	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"����� ����", L"\n\n���� �����Ͻ� ����� ������ �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (CUserManager::Instance()->GetCount() <= nSelect)
	{
		return;
	}
	pUserInfo pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	if (!pInfo) {
		return;
	}
	CString sName, sPhone, sEtc, sId, sPw;
	GetDlgItemText(IDC_EDIT_NAME, sName);
	GetDlgItemText(IDC_EDIT_PHONE, sPhone);
	GetDlgItemText(IDC_EDIT_ETC, sEtc);
	GetDlgItemText(IDC_EDIT_ID, sId);
	GetDlgItemText(IDC_EDIT_PW, sPw);
	if (sName.GetLength() < 2)
	{
		CMessagePopup dlg(L"����� ����", L"\n\n�̸��� �ʼ� �Է� �����Դϴ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	sPhone.Trim();
	sPhone.Replace(L"-", L"");
	sPhone.Replace(L"/", L"");
	sPhone.Replace(L"_", L"");
	sPhone.Replace(L".", L"");
	sPhone.Replace(L"*", L"");
	sPhone.Replace(L"+", L"");
	sPhone.Replace(L"+82", L"");

	CString sText;
	sText.Format(L"[�̸�: %s] [���̵�: %s]\n\n��ȭ��ȣ: %s\n\n������: %s", sName, sId, sPhone, sEtc);
	CMessagePopup dlg(L"����� ����", sText, L"�� ������ ����ڸ� �����Ͻðڽ��ϱ�?", MB_YESNO, DT_LEFT | DT_VCENTER, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CClientInterface::Instance()->ProcessRequestModUser(pInfo->nSeq, sName, sPhone, sPw, sEtc);
}

void CSLP30_ManagerDlg::OnDelClick()
{
	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
	{
		CMessagePopup dlg(L"����� ����", L"\n\n���� �����Ͻ� ����� ������ �����Ͽ� �ֽʽÿ�.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (CUserManager::Instance()->GetCount() <= nSelect)
		return;

	pUserInfo pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	if (!pInfo) {
		return;
	}
	CString sText, sName = pInfo->szName;
	sText.Format(L"\n\n[%s : %s]\n\n������ ����ڸ� �����Ͻðڽ��ϱ�?", sName, pInfo->szID);
	CMessagePopup dlg(L"����� ����", sText, MB_YESNO, this);
	UINT nResult = dlg.DoModal();
	if (nResult == IDCANCEL) {
		return;
	}
	CClientInterface::Instance()->ProcessRequestDelUser(pInfo->nSeq);
}

void CSLP30_ManagerDlg::OnMiniClick()
{
	ShowWindow(SW_MINIMIZE);
}

void CSLP30_ManagerDlg::OnCloseClick()
{
	OnOK();
}


void CSLP30_ManagerDlg::OnItemchangedListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	if (m_bDestroy) {
		return;
	}

	int nSelect = m_listUser.GetCurSel();
	if (nSelect < 0)
	{
		m_nUserSeq = 0;

		SetDlgItemText(IDC_EDIT_NAME, L"");
		SetDlgItemText(IDC_EDIT_PHONE, L"");
		SetDlgItemText(IDC_EDIT_ETC, L"");
		SetDlgItemText(IDC_EDIT_ID, L"");
		SetDlgItemText(IDC_EDIT_PW, L"");
		return;
	}
	if (CUserManager::Instance()->GetCount() <= nSelect)
		return;

	pUserInfo pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	m_nUserSeq = pInfo->nSeq;

	SetDlgItemText(IDC_EDIT_NAME, pInfo->szName);
	SetDlgItemText(IDC_EDIT_PHONE, pInfo->szPhone);
	SetDlgItemText(IDC_EDIT_ETC, pInfo->szEtc);
	SetDlgItemText(IDC_EDIT_ID, pInfo->szID);
	SetDlgItemText(IDC_EDIT_PW, pInfo->szPW);
}

void CSLP30_ManagerDlg::OnItemdblclickListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	
}

void CSLP30_ManagerDlg::OnDblclkListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	int nSelect = m_listUser.GetCurSel();
	pUserInfo pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	if (pInfo) {
		CString sUrl = L"http://160.202.162.3:8090/DATA/";
		sUrl += pInfo->szID;
		HINSTANCE hInstance = ShellExecute(NULL, L"open", sUrl, NULL, NULL, SW_SHOWNORMAL);
	}
}
