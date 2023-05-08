// TopBarDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "TopBarDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CTopBarDlg 대화 상자입니다.

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

// CTopBarDlg 메시지 처리기입니다.

BOOL CTopBarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTopBarDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CTopBarDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CTopBarDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	GetParent()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKEWPARAM(point.x, point.y));

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CTopBarDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CTopBarDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (GetParent()->IsZoomed()) {
		CCommonState::ie()->m_bMax = false;
		::ShowWindow(GetParent()->m_hWnd, SW_RESTORE);
		m_btnMaximize.ChangeBitmap(IDB_BMP_MAXIMIZE);
	}
	else {
		CCommonState::ie()->m_bMax = true;
		//CPositionManager::ie()->SavePosition(GetParent(), CCommonState::ie()->m_bMax);	//20221026 GBM - 싱글<->듀얼모니터 번갈아 가면서 사용시 좌표계 꼬이면 화면에서 안보이는 경우도 있어서 굳이 위치저장 기능 사용하지 않도록 함
		::ShowWindow(GetParent()->m_hWnd, SW_MAXIMIZE);
		m_btnMaximize.ChangeBitmap(IDB_BMP_RESTORE);
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CTopBarDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	Redisplay();

	m_btnClose.Invalidate();
	m_btnMaximize.Invalidate();
	m_btnMinimize.Invalidate();
}

void CTopBarDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Redisplay();
}

void CTopBarDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CTopBarDlg::OnCloseClick()
{
#ifndef ENGLISH_MODE
	CMessagePopup popup(L"프로그램 종료", L"\n\n프로그램을 종료합니까?", MB_YESNO, CCommonState::ie()->m_pWnd);
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
		//CPositionManager::ie()->SavePosition(GetParent(), CCommonState::ie()->m_bMax);	//20221026 GBM - 싱글<->듀얼모니터 번갈아 가면서 사용시 좌표계 꼬이면 화면에서 안보이는 경우도 있어서 굳이 위치저장 기능 사용하지 않도록 함
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
