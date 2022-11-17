
// SLP30_ManagerDlg.cpp : 구현 파일
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


// CSLP30_ManagerDlg 대화 상자



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
		_T("돋움")); // lpszFacename 
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

// CSLP30_ManagerDlg 메시지 처리기
const TCHAR* lpszHeaderManager[] = { _T("NO"), _T("이름"), _T("전화번호"), _T("아이디"), _T("비고"), NULL };
const int colWidthListManager[] = { 5, 16, 23, 16, 35, 0 };

BOOL CSLP30_ManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	Log::Setup();
	Log::SetMainPointer(this);

	CPositionManager::New();
	//CUSBDriveManger::New();
	CClientInterface::New();
	CClientInterface::Instance()->TryConnection("160.202.162.3", 10345);

	CUserManager::New();
	CCommonState::New();
	CCommonState::Instance()->m_pUserWnd = this;

	SetWindowText(L"SLP30 사용자 관리 프로그램");

	Global.SetFont(IDX_FONT_SMALL, _T("돋움"), 9);
	Global.SetFont(IDX_FONT_MEDIUM, _T("돋움"), 10);
	Global.SetFont(IDX_FONT_LARGE, _T("돋움"), 11, TRUE);
	Global.SetFont(IDX_FONT_HEADER, _T("돋움"), 10, TRUE);

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

	m_pCheckUsb = new CCheckTextBtn(0, L"보안키(USB)를 발급하실 경우 체크하여 주십시오.", this);
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
		CMessagePopup popup(L"생성 실패", L"\n\nSMC설정에 실패하였습니다.", MB_OK, this);
		popup.DoModal();
		EndDialog(0);
		return FALSE;
	}
	if (!CSharedMemoryManager::Instance()->OpenSharedMemory()) {
		CMessagePopup popup(L"생성 실패", L"\n\nSMC설정에 실패하였습니다.", MB_OK, this);
		popup.DoModal();
		EndDialog(0);
		return FALSE;
	}

	SetTimer(1, 100, NULL);

	m_bInit = true;
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSLP30_ManagerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	CPositionManager::Instance()->SavePosition(this);

	m_bDestroy = true;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSLP30_ManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSLP30_ManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSLP30_ManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bInit) {
		return;
	}
}

void CSLP30_ManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
			CMessagePopup dlg(L"로그인", L"\n\n로그인 아이디 또는 비밀번호가 맞지 않습니다.\n\n다시 시도하여 주십시오.", MB_OK);
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CSLP30_ManagerDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bInit) {
		return;
	}
}

void CSLP30_ManagerDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bInit) {
		return;
	}
}

HBRUSH CSLP30_ManagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CSLP30_ManagerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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
	//_T("No"), _T("이름"), _T("전화번호"), _T("비고")
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

			CMessagePopup dlg(L"사용자 추가", L"\n\n사용자 추가가 완료되었습니다.", MB_OK, this);
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
			CMessagePopup dlg(L"사용자 추가", L"\n\n이미 존재하는 전화번호입니다.\n\n다른 전화번호로 시도하여 주십시오.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == -2) {
			CMessagePopup dlg(L"사용자 추가", L"\n\n사용자 추가에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseUserMod:
		if (nValue == 1 && CCommonState::Instance()->m_nIdx > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestUserList, CCommonState::Instance()->m_nIdx);
			CMessagePopup dlg(L"사용자 수정", L"\n\n사용자 수정이 완료되었습니다.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"사용자 수정", L"\n\n사용자 수정에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
			dlg.DoModal();
		}
		break;
	case ProtocolHeader::ResponseUserDel:
		if (nValue == 1 && CCommonState::Instance()->m_nIdx > 0) {
			CClientInterface::Instance()->ProcessCommonRequest(ProtocolHeader::RequestUserList, CCommonState::Instance()->m_nIdx);
			CMessagePopup dlg(L"사용자 삭제", L"\n\n사용자 삭제가 완료되었습니다.", MB_OK, this);
			dlg.DoModal();
		}
		else if (nValue == 0) {
			CMessagePopup dlg(L"사용자 삭제", L"\n\n사용자 삭제에 실패하였습니다.\n\n재시작 후 다시 시도해 주시기 바랍니다.", MB_OK, this);
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
			CMessagePopup popup(L"보안키 발급", L"\n\n보안키 발급을 취소하셨습니다.\n\n발급을 원할 경우\n\n수정 기능을 통하여 발급하실 수 있습니다.", MB_OK, this);
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
		CMessagePopup dlg(L"사용자 추가", L"\n\n이름은 필수 입력 사항입니다.", MB_OK, this);
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
		CMessagePopup dlg(L"사용자 추가", L"\n\n아이디는 필수 입력사항입니다.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	if (sPw.GetLength() < 3) {
		CMessagePopup dlg(L"사용자 추가", L"\n\n패스워드는 필수 입력사항입니다.", MB_OK, this);
		dlg.DoModal();
		return;
	}
	CString sText;
	sText.Format(L"[이름: %s] [아이디: %s]\n\n전화번호: %s\n\n비고: %s", sName, sId, sPhone, sEtc);
	CMessagePopup dlg(L"사용자 추가", sText, L"이 정보로 사용자를 등록하시겠습니까?", MB_YESNO, DT_LEFT | DT_VCENTER, this);
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
		CMessagePopup dlg(L"사용자 수정", L"\n\n먼저 수정하실 사용자 계정을 선택하여 주십시오.", MB_OK, this);
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
		CMessagePopup dlg(L"사용자 수정", L"\n\n이름은 필수 입력 사항입니다.", MB_OK, this);
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
	sText.Format(L"[이름: %s] [아이디: %s]\n\n전화번호: %s\n\n담당업무: %s", sName, sId, sPhone, sEtc);
	CMessagePopup dlg(L"사용자 수정", sText, L"이 정보로 사용자를 수정하시겠습니까?", MB_YESNO, DT_LEFT | DT_VCENTER, this);
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
		CMessagePopup dlg(L"사용자 삭제", L"\n\n먼저 삭제하실 사용자 계정을 선택하여 주십시오.", MB_OK, this);
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
	sText.Format(L"\n\n[%s : %s]\n\n선택한 사용자를 삭제하시겠습니까?", sName, pInfo->szID);
	CMessagePopup dlg(L"사용자 삭제", sText, MB_YESNO, this);
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	
}

void CSLP30_ManagerDlg::OnDblclkListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	int nSelect = m_listUser.GetCurSel();
	pUserInfo pInfo = CUserManager::Instance()->GetUserInfo(nSelect);
	if (pInfo) {
		CString sUrl = L"http://160.202.162.3:8090/DATA/";
		sUrl += pInfo->szID;
		HINSTANCE hInstance = ShellExecute(NULL, L"open", sUrl, NULL, NULL, SW_SHOWNORMAL);
	}
}
