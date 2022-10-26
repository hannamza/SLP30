
// SLP30Dlg.cpp : ���� ����
//

#include "stdafx.h"
#include "SLP30.h"
#include "SLP30Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSLP30Dlg ��ȭ ����

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


// CSLP30Dlg �޽��� ó����

BOOL CSLP30Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

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
		CMessagePopup popup(L"���� ����", L"\n\nSMC������ �����Ͽ����ϴ�.", MB_OK, this);
		popup.DoModal();
		EndDialog(0);
		return FALSE;
	}
	if (!CSharedMemoryManager::ie()->OpenSharedMemory()) {
		CMessagePopup popup(L"���� ����", L"\n\nSMC������ �����Ͽ����ϴ�.", MB_OK, this);
		popup.DoModal();
		EndDialog(0);
		return FALSE;
	}

	DWORD dwLastTime = GetTickCount();
	SetWindowText(L"SLP30");

	CRect rect, rt;
	GetClientRect(&rect);

	bool bMax = false, bResult = false;

	//20221026 GBM start - �̱�<->������� ������ ���鼭 ���� ��ǥ�� ���̸� ȭ�鿡�� �Ⱥ��̴� ��쵵 �־ ���� ��ġ���� ��� ������� �ʵ��� ��
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSLP30Dlg::OnPaint()
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

		if (m_pTopBarDlg) {
			m_pTopBarDlg->Invalidate();
		}
		if (m_pListCtrl) {
			m_pListCtrl->Invalidate();
		}
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSLP30Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CSLP30Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

void CSLP30Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	SAFE_DELETE(m_pTopBarDlg);
	SAFE_DELETE(m_pMainFrameDlg);

	if (!CCommonState::ie()->m_bMax) {
		//CPositionManager::ie()->SavePosition(this, CCommonState::ie()->m_bMax);	//20221026 GBM - �̱�<->������� ������ ���鼭 ���� ��ǥ�� ���̸� ȭ�鿡�� �Ⱥ��̴� ��쵵 �־ ���� ��ġ���� ��� ������� �ʵ��� ��
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CSLP30Dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y = 768;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CSLP30Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
				CMessagePopup dlg(L"�α���", L"\n\n������ ������ �� �����ϴ�.\n\n���� �Ǵ� ��Ʈ��ũ ������ �� �ֽ��ϴ�.", MB_OK);
				dlg.DoModal();
			}
			else {
				CMessagePopup dlg(L"�α���", L"\n\n�α��� ���̵� �Ǵ� ��й�ȣ�� ���� �ʽ��ϴ�.\n\n�ٽ� �õ��Ͽ� �ֽʽÿ�.", MB_OK);
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
	SHORT value = GetAsyncKeyState(VK_LBUTTON);		// ���� ��ư ���� ����
	if (value == -32768)							// 1000 0000 0000 0000 : 0x8000 ������ ���� ���� ���� ȣ�� �������� �����ִ� ���� 
	{
		SendMessage(WM_NCLBUTTONDOWN, HTBOTTOMRIGHT, 0);
	}
}

void CSLP30Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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