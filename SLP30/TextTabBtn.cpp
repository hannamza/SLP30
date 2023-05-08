// TextTabBtn.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "TextTabBtn.h"
#include "afxdialogex.h"


// CTextTabBtn ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTextTabBtn, CDialogEx)

CTextTabBtn::CTextTabBtn(int nIndex, CString sCaption, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_sCaption = sCaption;
	m_nIndex = nIndex;
	m_bEnable = true;
	m_bMouseDown = false;
	m_bMouseHover = false;

	m_font.CreateFont(15, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("����")); // lpszFacename 
	m_font1.CreateFont(15, // nHeight 
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

	m_bCheck = false;
}

CTextTabBtn::~CTextTabBtn()
{
}

void CTextTabBtn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTextTabBtn, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnRadioClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

// CTextTabBtn �޽��� ó�����Դϴ�.

BOOL CTextTabBtn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CTextTabBtn::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CTextTabBtn::PreTranslateMessage(MSG* pMsg)
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

BOOL CTextTabBtn::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CTextTabBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
	SetCursor(hHandCursor);
	m_bMouseDown = true;
	SetCapture();
	Redisplay();

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CTextTabBtn::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect;
	GetClientRect(&rect);
	if (m_bMouseDown && rect.PtInRect(point)) {
		GetParent()->PostMessage(TEXTTAB_BUTTON_CLICK, m_nIndex, 0);
	}
	HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
	SetCursor(hHandCursor);
	m_bMouseDown = false;
	ReleaseCapture();
	Redisplay();

	CDialogEx::OnLButtonUp(nFlags, point);
}

int CTextTabBtn::GetWidthExtent(CString sText)
{
	CDC* _pDC = GetDC();
	CSize size = CCommonFunc::GetTextExtent(_pDC, m_font, sText);
	ReleaseDC(_pDC);
	return size.cx + 21;
}

void CTextTabBtn::SetCaption(CString sCaption)
{
	m_sCaption = sCaption;
	Invalidate();
}

void CTextTabBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
	SetCursor(hHandCursor);

	if (!m_bMouseHover)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		if (TrackMouseEvent(&tme))
		{
			m_bMouseHover = true;
		}
	}
	Redisplay();

	CDialogEx::OnMouseMove(nFlags, point);
}

void CTextTabBtn::OnMouseLeave()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_bMouseHover = false;
	Redisplay();

	CDialogEx::OnMouseLeave();
}

void CTextTabBtn::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();
}

void CTextTabBtn::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CTextTabBtn::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

void CTextTabBtn::OnRadioClick()
{
	if (m_bEnable) {
		GetParent()->PostMessage(TEXTTAB_BUTTON_CLICK, m_nIndex, 0);
	}
}

void CTextTabBtn::SetCheck(BOOL bTrue)
{
	m_bCheck = bTrue;
	Redisplay();
}

BOOL CTextTabBtn::GetCheck()
{
	return m_bCheck;
}

void CTextTabBtn::EnableWindowEx(BOOL bEnable)
{
	m_bEnable = bEnable;
	EnableWindow(bEnable);

	Invalidate();
}

void CTextTabBtn::Redisplay()
{
	CRect rect, rt;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	if (GetCheck()) {
		CRect rt1;
		GetClientRect(&rt1);
		if (rt1.Width() > 89) {
#ifndef ENGLISH_MODE
			CCommonDisplay::DrawImageGdiplusBMP(&memDC, IDB_BMP_TEXTTAB1, 0, 0, rect.Width(), rect.Height(), 0, rect.Height(), rect.Width(), rect.Height());
#else
			CCommonDisplay::DrawImageGdiplusBMP(&memDC, IDB_BMP_TEXTTAB1_EN, 0, 0, rect.Width(), rect.Height(), 0, rect.Height(), rect.Width(), rect.Height());
#endif
		}
		else {
			CCommonDisplay::DrawImageGdiplusBMP(&memDC, IDB_BMP_TEXTTAB, 0, 0, rect.Width(), rect.Height(), 0, rect.Height(), rect.Width(), rect.Height());
		}
	}
	else {
		CRect rt1;
		GetClientRect(&rt1);
		if (rt1.Width() > 89) {
#ifndef ENGLISH_MODE
			CCommonDisplay::DrawImageGdiplusBMP(&memDC, IDB_BMP_TEXTTAB1, 0, 0, rect.Width(), rect.Height(), 0, 0, rect.Width(), rect.Height());
#else
			CCommonDisplay::DrawImageGdiplusBMP(&memDC, IDB_BMP_TEXTTAB1_EN, 0, 0, rect.Width(), rect.Height(), 0, 0, rect.Width(), rect.Height());
#endif
		}
		else {
			CCommonDisplay::DrawImageGdiplusBMP(&memDC, IDB_BMP_TEXTTAB, 0, 0, rect.Width(), rect.Height(), 0, 0, rect.Width(), rect.Height());
		}
	}

	if (m_bEnable) {
		rt.CopyRect(&rect);
		if (GetCheck()) {
			if (m_bMouseDown) {
				rt.top += 2;
				rt.bottom += 2;
				CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(241, 112, 17), m_font1, false, 0, rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			}
			else if (m_bMouseHover) {
				rt.top += 1;
				rt.bottom += 1;
				CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(241, 112, 17), m_font1, false, 0, rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			}
			else {
				CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(241, 112, 17), m_font1, false, 0, rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			}
		}
		else {
			if (m_bMouseDown) {
				rt.top += 2;
				rt.bottom += 2;
				CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(60, 60, 60), m_font, false, 0, rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			}
			else if (m_bMouseHover) {
				rt.top += 1;
				rt.bottom += 1;
				CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(60, 60, 60), m_font, false, 0, rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			}
			else {
				CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(60, 60, 60), m_font, false, 0, rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			}
		}
	}
	else {
		CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(140, 140, 140), m_font1, false, 0, rt, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
	}

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);
}
