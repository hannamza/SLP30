
// SLP30Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "SLP30.h"
#include "SLP30Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSLP30Dlg 대화 상자

CSLP30Dlg::CSLP30Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SLP30_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pTopBarDlg = NULL;
	m_pListCtrl = NULL;
	m_pMainFrameDlg = NULL;

	m_nDisplay = 0;
}

void CSLP30Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSLP30Dlg, CDialogEx)
	ON_MESSAGE(SOCKET_RECV, OnSocketRecvMsg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CSLP30Dlg 메시지 처리기

BOOL CSLP30Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//CBaseInfoManager::New();
	CCommonState::New();
	//CUSBDriveManger::New();
	CPositionManager::New();
	CSaveManager::New();
	//CUSBDriveManger::ie()->WriteSerialKeyUserID(L"E:", L"userkey");
	/*CString sUserKey;
	BOOL bValue = CUSBDriveManger::ie()->ConfirmSerialKeyUserID(sUserKey);*/

	CClientInterface::New();
	CPatternManager::New();

	CClientInterface::ie()->TryConnection("160.202.162.3", 10345);
	CCommonState::ie()->m_pWnd = this;

	CSharedMemoryManager::New();
	if (!CSharedMemoryManager::ie()->CreateSharedMemory()) {
		CMessagePopup popup(L"생성 실패", L"\n\nSMC설정에 실패하였습니다.", MB_OK, this);
		popup.DoModal();
		EndDialog(0);
		return FALSE;
	}
	if (!CSharedMemoryManager::ie()->OpenSharedMemory()) {
		CMessagePopup popup(L"생성 실패", L"\n\nSMC설정에 실패하였습니다.", MB_OK, this);
		popup.DoModal();
		EndDialog(0);
		return FALSE;
	}

	DWORD dwLastTime = GetTickCount();
	SetWindowText(L"SLP30");

	CRect rect, rt;
	GetClientRect(&rect);

	bool bMax = false, bResult = false;

	//20221026 GBM start - 싱글<->듀얼모니터 번갈아 가면서 사용시 좌표계 꼬이면 화면에서 안보이는 경우도 있어서 굳이 위치저장 기능 사용하지 않도록 함
// 	if (bResult = CPositionManager::ie()->LoadPosition(this, rt, bMax)) {
// 		CCommonState::ie()->m_bMax = bMax;
// 	}
	//20221026 GBM end

	m_pTopBarDlg = new CTopBarDlg(this);
	m_pTopBarDlg->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTopBarDlg->MoveWindow(0, 0, rect.Width(), TOPBAR_HEIGHT);
	m_pTopBarDlg->ShowWindow(SW_SHOW);

	m_pMainFrameDlg = new CMainFrameDlg(this);
	m_pMainFrameDlg->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pMainFrameDlg->MoveWindow(0, TOPBAR_HEIGHT, rect.Width(), rect.Height() - TOPBAR_HEIGHT);
	m_pMainFrameDlg->ShowWindow(SW_SHOW);

	if (bResult) {
		this->SetWindowPos(NULL, rt.left, rt.top, rt.Width(), rt.Height(), SWP_SHOWWINDOW);
		if (CCommonState::ie()->m_bMax) {
			ShowWindow(SW_MAXIMIZE);
		}
	}
	else {
		this->SetWindowPos(NULL, 0, 0, 1024, 768, SWP_NOMOVE);
		CenterWindow();
	}

	CString sDumpPath = CCommonFunc::GetBaseDumpPathW();
	CCommonFunc::CreateDirectoryW(sDumpPath.GetBuffer(0));

	SetTimer(1, 10, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSLP30Dlg::OnPaint()
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

		if (m_pTopBarDlg) {
			m_pTopBarDlg->Invalidate();
		}
		if (m_pListCtrl) {
			m_pListCtrl->Invalidate();
		}
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSLP30Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CSLP30Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CSLP30Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	SAFE_DELETE(m_pTopBarDlg);
	SAFE_DELETE(m_pMainFrameDlg);

	if (!CCommonState::ie()->m_bMax) {
		//CPositionManager::ie()->SavePosition(this, CCommonState::ie()->m_bMax);	//20221026 GBM - 싱글<->듀얼모니터 번갈아 가면서 사용시 좌표계 꼬이면 화면에서 안보이는 경우도 있어서 굳이 위치저장 기능 사용하지 않도록 함
	}

	CString sLog;
	sLog.Format(L"[%s] program ended.", CCommonState::ie()->m_sUserID);
	CClientInterface::ie()->ProcessRequestHistoryAdd(CCommonState::ie()->m_sUserID.GetBuffer(0), sLog.GetBuffer(0));

	Sleep(100);

	CClientInterface::ie()->TryShutdown();
	CClientInterface::ie()->Destroy();
	CClientInterface::Delete();

	CCommonState::Delete();
	//CBaseInfoManager::Delete();
	//CUSBDriveManger::Delete();
	CSharedMemoryManager::Delete();
	CPositionManager::Delete();
	CSaveManager::Delete();
	CPatternManager::Delete();
	CDataLinkManager::Delete();
}

BOOL CSLP30Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CSLP30Dlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CSLP30Dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y = 768;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CSLP30Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!IsIconic()) {
		CRect rect;
		GetClientRect(&rect);
		if (m_pTopBarDlg) {
			m_pTopBarDlg->MoveWindow(0, 0, rect.Width(), TOPBAR_HEIGHT);
		}
		if (m_pMainFrameDlg) {
			m_pMainFrameDlg->MoveWindow(0, TOPBAR_HEIGHT, rect.Width(), rect.Height() - TOPBAR_HEIGHT);
		}
	}
}

void CSLP30Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1) {
		KillTimer(1);

		CLoginDlg dlg;
		UINT nResult = dlg.DoModal();
		// test
		if (nResult == IDOK) {
			return;
		}

		if (nResult != IDOK) {
			OnOK();
			return;
		}
		CCommonState::ie()->m_bLoginResult = false;
		CCommonState::ie()->m_sUserID = dlg.m_sID;
		CCommonState::ie()->m_nIdx = 0;
		CClientInterface::ie()->ProcessRequestLogin(dlg.m_sID.GetBuffer(0), dlg.m_sPW.GetBuffer(0));
		DWORD dwLastTime = GetTickCount();
		bool bTimeout = true;
		while (GetTickCount() - dwLastTime < 2000)
		{
			if (CCommonState::ie()->m_bLoginResult) {
				bTimeout = false;
				break;
			}
			Sleep(1);
		}
		if (CCommonState::ie()->m_nIdx <= 0) {
			CString sLog;
			sLog.Format(L"[%s] login failed.", dlg.m_sID);
			CClientInterface::ie()->ProcessRequestHistoryAdd(dlg.m_sID.GetBuffer(0), sLog.GetBuffer(0));

			if (bTimeout) {
				CMessagePopup dlg(L"로그인", L"\n\n서버에 접속할 수 없습니다.\n\n서버 또는 네트워크 문제일 수 있습니다.", MB_OK);
				dlg.DoModal();
			}
			else {
				CMessagePopup dlg(L"로그인", L"\n\n로그인 아이디 또는 비밀번호가 맞지 않습니다.\n\n다시 시도하여 주십시오.", MB_OK);
				dlg.DoModal();
			}
			SetTimer(1, 10, NULL);
		}
		else {
			CString sLog;
			sLog.Format(L"[%s] logined.", dlg.m_sID);
			CClientInterface::ie()->ProcessRequestHistoryAdd(dlg.m_sID.GetBuffer(0), sLog.GetBuffer(0));
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CSLP30Dlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	//lpwndpos->flags &= ~SWP_SHOWWINDOW;

	CDialogEx::OnWindowPosChanging(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CSLP30Dlg::Redisplay()
{
	/*CRect rect, rt;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	rt.CopyRect(&rect);
	rt.top -= 2;
	CCommonDisplay::DrawRect(&memDC, true , RGB(120, 120, 120), RGB(238, 238, 238), rt);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);*/
}

void CSLP30Dlg::DialogResizing(UINT nFlags, CPoint point)
{
	SetCursor(LoadCursor(0, IDC_SIZENWSE));
	SHORT value = GetAsyncKeyState(VK_LBUTTON);		// 현재 버튼 상태 리턴
	if (value == -32768)							// 1000 0000 0000 0000 : 0x8000 이전에 누른 적이 없고 호출 시점에는 눌려있는 상태 
	{
		SendMessage(WM_NCLBUTTONDOWN, HTBOTTOMRIGHT, 0);
	}
}

void CSLP30Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	DialogResizing(nFlags, point);

	CDialogEx::OnMouseMove(nFlags, point);
}

LRESULT CSLP30Dlg::OnSocketRecvMsg(WPARAM wParam, LPARAM lParam)
{
	int nProtocol = wParam - 100;
	int nValue = lParam;
	switch (nProtocol)
	{
	case ProtocolHeader::ResponseUserLogin:
		break;
	default:
		break;
	}
	return 0;
}