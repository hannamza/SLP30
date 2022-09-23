// PopupTextDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "PopupTextDlg.h"
#include "afxdialogex.h"


// CPopupTextDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPopupTextDlg, CDialogEx)

CPopupTextDlg::CPopupTextDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEXT_POPUP_DIALOG, pParent)
{
	m_pParent = pParent;
	m_nLimit = 0;
}

CPopupTextDlg::~CPopupTextDlg()
{
	ReleaseCapture();
}

void CPopupTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
}


BEGIN_MESSAGE_MAP(CPopupTextDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPopupTextDlg 메시지 처리기입니다.


BOOL CPopupTextDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetTimer(1, 10, NULL);

	SetHanMode(m_editText.m_hWnd);

	if (m_nLimit > 0) {
		m_editText.SetLimitText(m_nLimit);
	}

	SetCapture();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPopupTextDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}

BOOL CPopupTextDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) {
			ReleaseCapture();
			m_editText.GetWindowText(m_sText);
			m_pParent->PostMessage(INPUT_TEXT_INFO, m_nItemIndex, m_nSelCol);
		}
		else if (pMsg->wParam == VK_ESCAPE) {
			ReleaseCapture();
		}
		else if (m_nLimit > 0) {
			SetTimer(9, 10, NULL);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CPopupTextDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return 0;// CDialogEx::OnEraseBkgnd(pDC);
}

void CPopupTextDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CPopupTextDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (!rect.PtInRect(point)) {
		ReleaseCapture();
		OnCancel();
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}

void CPopupTextDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (!rect.PtInRect(point)) {
		ReleaseCapture();
		OnCancel();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CPopupTextDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1) {
		KillTimer(1);
		CString sText;
		CRect rect;
		GetClientRect(&rect);
		m_editText.MoveWindow(5, 5, rect.right - 10, rect.bottom - 10);
		m_editText.GetWindowText(sText);
		m_editText.SetSel(0, -1);
		m_editText.ReplaceSel(sText);
		m_editText.SetFocus();
	}
	else if (nIDEvent == 9) {
		KillTimer(9);
		CString sText;
		m_editText.GetWindowText(sText);
		CStringA sTemp = CCommonFunc::WCharToChar(sText.GetBuffer(0));
		int len = sTemp.GetLength();
		if (len > m_nLimit) {
			sText = CCommonFunc::CharToWCHAR(sTemp.Left(m_nLimit).GetBuffer(0));
			m_editText.SetWindowText(sText);
			m_editText.SetSel(0, -1);
			m_editText.ReplaceSel(sText);
			m_editText.SetFocus();
			sTemp.ReleaseBuffer();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CPopupTextDlg::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawRect(&memDC, true, RGB(60, 60, 60), RGB(255, 255, 255), rect, 1);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);
}

void CPopupTextDlg::SetHanMode(HWND hWnd)
{
	HIMC hIMC = ImmGetContext(hWnd);
	DWORD dwConv, dwSent;
	DWORD dwTemp;

	ImmGetConversionStatus(hIMC, &dwConv, &dwSent);

	dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
	dwTemp |= IME_CMODE_NATIVE;
	dwConv = dwTemp;
	ImmSetConversionStatus(hIMC, dwConv, dwSent);
	ImmReleaseContext(hWnd, hIMC);
}

void CPopupTextDlg::SetItemIndexInfo(int nItemIndex, int nSelCol)
{
	m_nItemIndex = nItemIndex;
	m_nSelCol = nSelCol;
}

CString CPopupTextDlg::GetInputText()
{
	return m_sText;
}

void CPopupTextDlg::SetInputText(CString sText)
{
	m_editText.SetWindowText(sText);
}

void CPopupTextDlg::SetTextLimit(int nSize)
{
	m_nLimit = nSize;
	if (m_nLimit > 0) {
		m_editText.SetLimitText(m_nLimit);
	}
}