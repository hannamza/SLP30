// TopBarDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "TopBarDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CTopBarDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTopBarDlg, CDialogEx)

CTopBarDlg::CTopBarDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_bInit = false;
}

CTopBarDlg::~CTopBarDlg()
{
}

void CTopBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTopBarDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnCloseClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnMaximizeClick)
	ON_COMMAND(IDC_COMMON_BUTTON3, OnMinimizeClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CTopBarDlg �޽��� ó�����Դϴ�.

BOOL CTopBarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_btnClose.Create(IDB_BMP_CLOSE, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	if (CCommonState::ie()->m_bMax) {
		m_btnMaximize.Create(IDB_BMP_RESTORE, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	}
	else {
		m_btnMaximize.Create(IDB_BMP_MAXIMIZE, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	}
	m_btnMinimize.Create(IDB_BMP_MINIMIZE, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CTopBarDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CTopBarDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CTopBarDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CTopBarDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	GetParent()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKEWPARAM(point.x, point.y));

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CTopBarDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CTopBarDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	/*CRect rectES;
	CRect moveableRect;
	GetClientRect(&moveableRect);

	rectES.SetRect(moveableRect.left, moveableRect.top, moveableRect.left + 20, moveableRect.top + 20);
	if (rectES.PtInRect(point)) {
		GetParent()->SendMessage(WM_MOUSEMOVE, 0, 0);
	}*/

	CDialogEx::OnMouseMove(nFlags, point);
}

void CTopBarDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (GetParent()->IsZoomed()) {
		CCommonState::ie()->m_bMax = false;
		::ShowWindow(GetParent()->m_hWnd, SW_RESTORE);
		m_btnMaximize.ChangeBitmap(IDB_BMP_MAXIMIZE);
	}
	else {
		CCommonState::ie()->m_bMax = true;
		//CPositionManager::ie()->SavePosition(GetParent(), CCommonState::ie()->m_bMax);	//20221026 GBM - �̱�<->������� ������ ���鼭 ���� ��ǥ�� ���̸� ȭ�鿡�� �Ⱥ��̴� ��쵵 �־ ���� ��ġ���� ��� ������� �ʵ��� ��
		::ShowWindow(GetParent()->m_hWnd, SW_MAXIMIZE);
		m_btnMaximize.ChangeBitmap(IDB_BMP_RESTORE);
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CTopBarDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	Redisplay();

	m_btnClose.Invalidate();
	m_btnMaximize.Invalidate();
	m_btnMinimize.Invalidate();
}

void CTopBarDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Redisplay();
}

void CTopBarDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

void CTopBarDlg::OnCloseClick()
{
#ifndef ENGLISH_MODE
	CMessagePopup popup(L"���α׷� ����", L"\n\n���α׷��� �����մϱ�?", MB_YESNO, CCommonState::ie()->m_pWnd);
#else
	CMessagePopup popup(L"Program Exit", L"\n\n\nDo you want to exit the program?", MB_YESNO, CCommonState::ie()->m_pWnd);
#endif
	UINT nResult = popup.DoModal();
	if (nResult != IDOK) {
		return;
	}
	::SendMessage(GetParent()->m_hWnd, WM_CLOSE, NULL, NULL);
}

void CTopBarDlg::OnMaximizeClick()
{
	if (GetParent()->IsZoomed()) {
		CCommonState::ie()->m_bMax = false;
		::ShowWindow(GetParent()->m_hWnd, SW_RESTORE);
		m_btnMaximize.ChangeBitmap(IDB_BMP_MAXIMIZE);
	}
	else {
		CCommonState::ie()->m_bMax = true;
		//CPositionManager::ie()->SavePosition(GetParent(), CCommonState::ie()->m_bMax);	//20221026 GBM - �̱�<->������� ������ ���鼭 ���� ��ǥ�� ���̸� ȭ�鿡�� �Ⱥ��̴� ��쵵 �־ ���� ��ġ���� ��� ������� �ʵ��� ��
		::ShowWindow(GetParent()->m_hWnd, SW_MAXIMIZE);
		m_btnMaximize.ChangeBitmap(IDB_BMP_RESTORE);
	}
}

void CTopBarDlg::OnMinimizeClick()
{
	::ShowWindow(GetParent()->m_hWnd, SW_MINIMIZE);
}

void CTopBarDlg::Redisplay()
{
	CRect rect, rt;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawImage(&memDC, IDB_BMP_TOPMIDDLE, 0, 0, rect.Width(), rect.Height());
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_TOPLOGO, 0, 0);
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_TOPRIGHT, rect.Width() - 149, rect.Height() - TOPBAR_HEIGHT);

	rt.CopyRect(&rect);
	rt.bottom += 2;
	CCommonDisplay::DrawRect(&memDC, false, RGB(120, 120, 120), 0, rt);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInit) {
		m_btnMinimize.MoveWindow(rect.right - 144, 12, 28, 28);
		m_btnMaximize.MoveWindow(rect.right - 100, 12, 28, 28);
		m_btnClose.MoveWindow(rect.right - 56, 12, 28, 28);
	}
}
