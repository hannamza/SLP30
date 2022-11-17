// MessagePopup.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30_Manager.h"
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_btnOk.Create(IDB_BMP_CONFIRM, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnCancel.Create(IDB_BMP_CANCEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);

	this->SetWindowPos(NULL, 0, 0, 490, 348, SWP_NOMOVE);

	switch (m_nType)
	{
	case MB_OK:
		m_btnOk.MoveWindow(490/2 - 132/2, 251, 132, 44);
		m_btnCancel.ShowWindow(SW_HIDE);
		break;
	case MB_YESNO:
		m_btnOk.MoveWindow(102, 251, 132, 44);
		m_btnCancel.MoveWindow(253, 251, 132, 44);
		break;
	}

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
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CMessagePopup::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	LONG ExtendedStyle = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, ExtendedStyle | WS_EX_LAYERED);
	BYTE byAlphaValue = 0; // 0 ~ 255 ���� ����[0 - ���� ����, 255 - ������]
	::SetLayeredWindowAttributes(GetSafeHwnd(), RGB(252, 252, 252), byAlphaValue, LWA_COLORKEY);

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
	if (nCtlColor == CTLCOLOR_DLG)
	{
		CBrush B;
		B.CreateStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT); // ���� ���� 

		return B;
	}
	return hbr;
}

void CMessagePopup::OnOkClick()
{
	OnOK();
}

void CMessagePopup::OnCancelClick()
{
	OnCancel();
}

void CMessagePopup::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	//CBitmap bitmap;
	//CDC memDC;
	//bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	//memDC.CreateCompatibleDC(_pDC);
	//memDC.SelectObject(bitmap);

	//CCommonDisplay::DrawColorBg(&memDC, RGB(252, 252, 252), rect);
	//CCommonDisplay::DrawImageGdiplusPNG(&memDC, IDB_PNG_MSGBG, 0, 0);

	//// title
	//CCommonDisplay::DrawCaption(&memDC, m_sTitle, RGB(255, 111, 0), m_font, 0, 0, CRect(0, 40, rect.Width(), 200), DT_CENTER | DT_TOP);	// message
	//if (m_nStyle == 0) {
	//	CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(80, 80, 80), m_font2, 0, 0, CRect(30, 89, 455, 228), DT_CENTER | DT_VCENTER);
	//}
	//else {
	//	CCommonDisplay::DrawCaption(&memDC, m_sCaption1, RGB(80, 80, 80), m_font2, 0, 0, CRect(100, 89, 455, 200), m_nStyle);
	//	CCommonDisplay::DrawCaption(&memDC, m_sCaption2, RGB(80, 80, 80), m_font2, 0, 0, CRect(30, 200, 455, 228), DT_CENTER | DT_VCENTER);
	//}

	//_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	CCommonDisplay::DrawImageGdiplusPNG(_pDC, IDB_PNG_MSGBG, 0, 0);

	// title
	CCommonDisplay::DrawCaption(_pDC, m_sTitle, RGB(255, 111, 0), m_font, 0, 0, CRect(0, 40, rect.Width(), 200), DT_CENTER | DT_TOP);	// message
	if (m_nStyle == 0) {
		CCommonDisplay::DrawCaption(_pDC, m_sCaption, RGB(80, 80, 80), m_font2, 0, 0, CRect(30, 89, 455, 228), DT_CENTER | DT_VCENTER);
	}
	else {
		CCommonDisplay::DrawCaption(_pDC, m_sCaption1, RGB(80, 80, 80), m_font2, 0, 0, CRect(100, 89, 455, 200), m_nStyle);
		CCommonDisplay::DrawCaption(_pDC, m_sCaption2, RGB(80, 80, 80), m_font2, 0, 0, CRect(30, 200, 455, 228), DT_CENTER | DT_VCENTER);
	}

	//bitmap.DeleteObject();
	//::DeleteObject(memDC);
	ReleaseDC(_pDC);
}

LRESULT CMessagePopup::OnNcHitTest(CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CDialogEx::OnNcHitTest(point);
}
