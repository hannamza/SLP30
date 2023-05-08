// MessagePopup.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "MessagePopup.h"
#include "afxdialogex.h"


// CMessagePopup ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMessagePopup, CDialogEx)

CMessagePopup::CMessagePopup(CString sTitle, CString sCaption, int nType, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MESSAGE, pParent)
{
	m_sTitle = sTitle;
	m_sCaption = sCaption;
	m_nType = nType;
	m_nStyle = 0;

	m_font.CreateFont(44, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_HEAVY, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("HY�߰��")); // lpszFacename 

	m_font2.CreateFont(16, // nHeight 
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
}

CMessagePopup::CMessagePopup(CString sTitle, CString sCaption1, CString sCaption2, int nType, UINT nStyle, CWnd* pParent)
	: CDialogEx(IDD_DIALOG_MESSAGE, pParent)
{
	m_sTitle = sTitle;
	m_nStyle = nStyle;
	m_sCaption1 = sCaption1;
	m_sCaption2 = sCaption2;
	m_nType = nType;

	m_font.CreateFont(44, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_HEAVY, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("HY�߰��")); // lpszFacename 

	m_font2.CreateFont(16, // nHeight 
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
}

CMessagePopup::~CMessagePopup()
{
}

void CMessagePopup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessagePopup, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnOkClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnCancelClick)

	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CMessagePopup �޽��� ó�����Դϴ�.


BOOL CMessagePopup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	const int nBkWidth = 490;
	const int nBkHeight = 348;

	CRect rect;
	int nX = -1, nY = -1;
	if (GetParent()) {
		GetParent()->GetClientRect(&rect);
		GetParent()->ClientToScreen(&rect);
		nX = rect.left + (rect.Width() / 2) - (nBkWidth / 2);
		nY = rect.top + (rect.Height() / 2) - (nBkHeight / 2);
	}

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

#ifndef ENGLISH_MODE
	m_btnOk.Create(IDB_BMP_CONFIRM, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnCancel.Create(IDB_BMP_CANCEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
#else
	m_btnOk.Create(IDB_BMP_CONFIRM_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnCancel.Create(IDB_BMP_CANCEL_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
#endif

	if (nX == -1 || nY == -1) {
		this->SetWindowPos(NULL, 0, 0, nBkWidth, nBkHeight, SWP_NOMOVE);
	}
	else {
		this->SetWindowPos(NULL, nX, nY, nBkWidth, nBkHeight, SWP_SHOWWINDOW);
	}

	switch (m_nType)
	{
	case MB_OK:
		m_btnOk.MoveWindow(nBkWidth / 2 - 132 / 2, 251, 132, 44);
		m_btnCancel.ShowWindow(SW_HIDE);
		break;
	case MB_OKCANCEL:
	case MB_YESNO:
		m_btnOk.MoveWindow(102, 251, 132, 44);
		m_btnCancel.MoveWindow(253, 251, 132, 44);
		break;
	default:
		m_btnOk.ShowWindow(SW_HIDE);
		m_btnCancel.ShowWindow(SW_HIDE);
		break;
	}
	/*if (m_nType == -1) {
		CRect rect;
		CRgn rgn;
		GetClientRect(rect);
		rgn.CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom, 30, 30);
		::SetWindowRgn(this->m_hWnd, (HRGN)rgn, TRUE);
	}*/

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CMessagePopup::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CMessagePopup::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) {
			OnOkClick();
		}
		else if (m_nType == -1 && pMsg->wParam == VK_ESCAPE) {
			ShowWindow(SW_HIDE);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CMessagePopup::OnEraseBkgnd(CDC* pDC)
{
	//// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//LONG ExtendedStyle = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	//::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, ExtendedStyle | WS_EX_LAYERED);
	//BYTE byAlphaValue = 0; // 0 ~ 255 ���� ����[0 - ���� ����, 255 - ������]
	//::SetLayeredWindowAttributes(GetSafeHwnd(), RGB(254, 255, 255), byAlphaValue, LWA_COLORKEY);

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CMessagePopup::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	Redisplay();

	m_btnOk.Invalidate();
	m_btnCancel.Invalidate();
}

void CMessagePopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

HBRUSH CMessagePopup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
// 	if (nCtlColor == CTLCOLOR_DLG)
// 	{
// 		CBrush B;
// 		B.CreateStockObject(NULL_BRUSH);
// 		pDC->SetBkMode(TRANSPARENT); // ���� ���� 
// 
// 		return B;
// 	}
	return hbr;
}

void CMessagePopup::OnOkClick()
{
	if (m_nType != -1) {
		OnOK();
	}
	else {
		ShowWindow(SW_HIDE);
	}
}

void CMessagePopup::OnCancelClick()
{
	if (m_nType != -1) {
		OnCancel();
	}
	else {
		ShowWindow(SW_HIDE);
	}
}

void CMessagePopup::SetCaption(CString sTitle, CString sCaption)
{
	m_sTitle = sTitle;
	m_sCaption = sCaption;
}

void CMessagePopup::SetCaptionAddBottom(CString sBottom)
{
	m_sBottom = sBottom;

	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CCommonDisplay::DrawCaption(_pDC, m_sBottom, RGB(255, 111, 0), m_font2, true, RGB(255, 255, 255), CRect(30, 230, 455, 260), DT_CENTER | DT_VCENTER);
	ReleaseDC(_pDC);
}

void CMessagePopup::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();

	/*if (m_nType == -1) {
		CCommonDisplay::DrawImage(_pDC, IDB_BMP_WAITBG);
	}
	else*/ {
		CCommonDisplay::DrawImageGdiplusPNG(_pDC, IDB_PNG_MSGBG, 0, 0);
	}

	// title
	CCommonDisplay::DrawCaption(_pDC, m_sTitle, RGB(255, 111, 0), m_font, 0, 0, CRect(0, 40, rect.Width(), 200), DT_CENTER | DT_TOP);	// message
	if (m_nStyle == 0) {
		CCommonDisplay::DrawCaption(_pDC, m_sCaption, RGB(80, 80, 80), m_font2, 0, 0, CRect(30, 89, 455, 228), DT_CENTER | DT_VCENTER);
	}
	else {
		CCommonDisplay::DrawCaption(_pDC, m_sCaption1, RGB(80, 80, 80), m_font2, 0, 0, CRect(100, 89, 455, 200), m_nStyle);
		CCommonDisplay::DrawCaption(_pDC, m_sCaption2, RGB(80, 80, 80), m_font2, 0, 0, CRect(30, 200, 455, 228), DT_CENTER | DT_VCENTER);
	}
	if (m_sBottom.GetLength() > 0) {
		CCommonDisplay::DrawCaption(_pDC, m_sBottom, RGB(255, 111, 0), m_font2, 0, 0, CRect(30, 230, 455, 260), DT_CENTER | DT_VCENTER);
	}

	ReleaseDC(_pDC);
}

LRESULT CMessagePopup::OnNcHitTest(CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CDialogEx::OnNcHitTest(point);
}
