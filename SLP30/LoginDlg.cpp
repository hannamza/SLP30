// LoginDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"
#include <dbt.h>

// CLoginDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_EDIT_PW, m_editPW);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnConfirmClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnCancelClick)
	ON_COMMAND(IDC_COMMON_BUTTON3, OnFindClick)
	ON_WM_DEVICECHANGE()

	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOVING()
END_MESSAGE_MAP()

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_brush.CreateSolidBrush(RGB(255, 255, 255));

	this->SetWindowPos(NULL, 0, 0, 420, 380, SWP_NOMOVE);

	m_editID.MoveWindow(146, 118, 194, 28);
	m_editPW.MoveWindow(146, 179, 194, 28);

#ifndef ENGLISH_MODE
	m_btnConfirm.Create(IDB_BMP_CONFIRM, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnCancel.Create(IDB_BMP_CANCEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnFind.Create(IDB_BMP_FIND, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
#else
	m_btnConfirm.Create(IDB_BMP_CONFIRM_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnCancel.Create(IDB_BMP_CANCEL_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnFind.Create(IDB_BMP_FIND_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
#endif

	m_btnConfirm.MoveWindow(67, 236, 132, 44);
	m_btnCancel.MoveWindow(218, 236, 132, 44);
	m_btnFind.MoveWindow(67, 292, 283, 34);

	SetTimer(1, 10, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CLoginDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) {
			OnConfirmClick();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
	}
	else if (nCtlColor == CTLCOLOR_DLG)
	{
		CBrush B;
		B.CreateStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT); // ���� ���� 

		return B;
	}
	return m_brush;

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

BOOL CLoginDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	LONG ExtendedStyle = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, ExtendedStyle | WS_EX_LAYERED);
	BYTE byAlphaValue = 0; // 0 ~ 255 ���� ����[0 - ���� ����, 255 - ������]
	::SetLayeredWindowAttributes(GetSafeHwnd(), RGB(252, 252, 252), byAlphaValue, LWA_COLORKEY);

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();

	m_btnConfirm.Invalidate();
	m_btnCancel.Invalidate();
	m_btnFind.Invalidate();
}

void CLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == 1) {
		KillTimer(1);
		/*CString sUserKey;
		BOOL bValue = CUSBDriveManger::ie()->ConfirmSerialKeyUserID(sUserKey);
		if (bValue) {
			CCommonState::ie()->m_sUserID = sUserKey;
			CCommonState::ie()->m_bLogin = true;
			OnOK();
		}*/
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CLoginDlg::OnConfirmClick()
{
	GetDlgItemText(IDC_EDIT_ID, m_sID);
	GetDlgItemText(IDC_EDIT_PW, m_sPW);

	// if(login success)\
	CCommonState::ie()->m_sUserID = m_sID;\
	CCommonState::ie()->m_bLogin = true;

	OnOK();
}

void CLoginDlg::OnCancelClick()
{
	OnCancel();
}

void CLoginDlg::OnFindClick()
{
	CString sName, sPhone, sMsg;
	CReadWriteState state;
	state.SetFileName(L"cs.ini");
#ifndef ENGLISH_MODE
	state.ReadText(L"����㿬��ó", L"NAME", sName);
	state.ReadText(L"����㿬��ó", L"PHONE", sPhone);
	sMsg.Format(L"\n\n�Ʒ� ����ó�� �����ٶ��ϴ�.\n\n%s: %s", sName, sPhone);
	CMessagePopup dlg(L"���̵�/�н����� ã��", sMsg, MB_OK);
#else
	state.ReadText(L"C/S Center", L"NAME", sName);
	state.ReadText(L"C/S Center", L"PHONE", sPhone);
	sMsg.Format(L"\n\nPlease contact to information below\n\n%s: %s", sName, sPhone);
	CMessagePopup dlg(L"FIND ID / PASSWORD", sMsg, MB_OK);
#endif
	UINT nResult = dlg.DoModal();
}

void CLoginDlg::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	//CBitmap bitmap;
	//CDC memDC;
	//bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	//memDC.CreateCompatibleDC(_pDC);
	//memDC.SelectObject(bitmap);

	////CCommonDisplay::DrawImageGdiplusPNG(&memDC, IDB_PNG_LOGINBG);
	//CCommonDisplay::DrawColorBg(&memDC, RGB(252, 252, 252), rect);
	//CCommonDisplay::DrawImageGdiplusPNG(&memDC, IDB_PNG_LOGIN, 0, 0);

#ifndef ENGLISH_MODE
	CCommonDisplay::DrawImageGdiplusPNG(_pDC, IDB_PNG_LOGIN, 0, 0);
#else
	CCommonDisplay::DrawImageGdiplusPNG(_pDC, IDB_PNG_LOGIN_EN, 0, 0);
#endif

	//bitmap.DeleteObject();
	//::DeleteObject(memDC);

	/*_pDC->SetBkMode(TRANSPARENT);
	CCommonDisplay::DrawImageGdiplusPNG(_pDC, IDB_PNG_LOGIN, 0, 0);*/

	ReleaseDC(_pDC);
}

LRESULT CLoginDlg::OnNcHitTest(CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CDialogEx::OnNcHitTest(point);
	/*int nHit = CDialogEx::OnNcHitTest(point);
	if (nHit == HTCLIENT) {
	nHit = HTCAPTION;
	}

	return nHit;*/
}

void CLoginDlg::n_SetDlgAlpha(int chAlpha)     // ���̾�α� ���� ����
{
	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	SetLayer pSetLayer = (SetLayer)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
	if (pSetLayer == NULL)
	{
		return;
	}

	SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE) | 0x80000);
	pSetLayer(this->m_hWnd, 0, chAlpha, LWA_ALPHA);
}



void CLoginDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CLoginDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Redisplay();
}

BOOL CLoginDlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
	switch (nEventType)
	{
	case DBT_DEVICEARRIVAL:
		TRACE("DBT_DEVICEARRIVAL\n");
		//��ġ ���� ���� �� ó��
		break;
	case DBT_DEVICEREMOVECOMPLETE:
		TRACE("DBT_DEVICEREMOVECOMPLETE\n");
		//��ġ ���� ���� ���� �� ó��
		break;
	case DBT_DEVNODES_CHANGED:
		TRACE("DBT_DEVNODES_CHANGED\n");
		{
			/*CString sUserKey;
			BOOL bValue = CUSBDriveManger::ie()->ConfirmSerialKeyUserID(sUserKey);
			if (bValue) {
				CCommonState::ie()->m_sUserID = sUserKey;
				CCommonState::ie()->m_bLogin = true;
				OnOK();
			}*/
		}
		break;
	default:
		break;
	}

	return 0;
}