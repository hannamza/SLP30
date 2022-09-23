// CheckTextBtn.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CheckTextBtn.h"
#include "afxdialogex.h"


// CCheckTextBtn 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCheckTextBtn, CDialogEx)

CCheckTextBtn::CCheckTextBtn(int nIndex, CString sCaption, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_sCaption = sCaption;
	m_nIndex = nIndex;
	m_bEnable = true;
	m_bMouseDown = false;
	m_bSendEvent = false;

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
		_T("굴림")); // lpszFacename 
}

CCheckTextBtn::~CCheckTextBtn()
{
}

void CCheckTextBtn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCheckTextBtn, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnRadioClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCheckTextBtn 메시지 처리기입니다.

BOOL CCheckTextBtn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_button.Create(IDB_BMP_CHECKBTN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_button.MoveWindow(0, 0, 16, 16);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCheckTextBtn::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CCheckTextBtn::PreTranslateMessage(MSG* pMsg)
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

BOOL CCheckTextBtn::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCheckTextBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
	SetCursor(hHandCursor);
	m_bMouseDown = true;
	SetCapture();

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCheckTextBtn::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (m_bMouseDown && rect.PtInRect(point)) {
		PostMessage(WM_COMMAND, IDC_COMMON_BUTTON1);
	}
	HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
	SetCursor(hHandCursor);
	m_bMouseDown = false;
	ReleaseCapture();

	CDialogEx::OnLButtonUp(nFlags, point);
}

int CCheckTextBtn::GetWidthExtent(CString sText)
{
	CDC* _pDC = GetDC();
	CSize size = CCommonFunc::GetTextExtent(_pDC, m_font, sText);
	ReleaseDC(_pDC);
	return size.cx + 21;
}

int CCheckTextBtn::GetWidthExtent()
{
	CDC* _pDC = GetDC();
	CSize size = CCommonFunc::GetTextExtent(_pDC, m_font, m_sCaption);
	ReleaseDC(_pDC);
	return size.cx + 21;
}

void CCheckTextBtn::SetFontSize(int nSize, UINT nBold)
{
	LOGFONT lf;
	m_font.GetLogFont(&lf);
	m_font.DeleteObject();
	lf.lfHeight = nSize;
	lf.lfWeight = nBold;
	m_font.CreateFontIndirect(&lf);
}

void CCheckTextBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
	SetCursor(hHandCursor);

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCheckTextBtn::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	CRect rect;
	GetClientRect(&rect);

	int nValue = 0;
	if (rect.Height() != 16) {
		nValue = (rect.Height() - 16) / 2;
	}
	m_button.MoveWindow(0, nValue, 16, 16);
	m_button.Invalidate();
}

void CCheckTextBtn::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CCheckTextBtn::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CCheckTextBtn::OnRadioClick()
{
	m_button.SetCheckEx(!m_button.GetCheckEx());
	if (m_bSendEvent) {
		GetParent()->PostMessage(CHECK_BUTTON_CLICK, m_nIndex, 0);
	}

	Invalidate();
}

void CCheckTextBtn::ChangeEventInfo(bool bEvent)
{
	m_bSendEvent = bEvent;
}

void CCheckTextBtn::SetCheck(BOOL bTrue)
{
	m_button.SetCheckEx(bTrue);
	Invalidate();
}

BOOL CCheckTextBtn::GetCheck()
{
	return m_button.GetCheckEx();
}

void CCheckTextBtn::EnableWindowEx(BOOL bEnable)
{
	m_button.EnableWindow(bEnable);
	m_bEnable = bEnable;
	EnableWindow(bEnable);

	Invalidate();
}

void CCheckTextBtn::Redisplay()
{
	CRect rect, rt;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawRect(&memDC, true, RGB(255, 255, 255), RGB(255, 255, 255), rect);
	rt.CopyRect(&rect);
	rt.left += 21;
	if (m_bEnable) {
		if (GetCheck()) {
			CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(241, 114, 17), m_font, false, 0, rt, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
		}
		else {
			CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(110, 110, 110), m_font, false, 0, rt, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
		}
	}
	else {
		CCommonDisplay::DrawCaption(&memDC, m_sCaption, RGB(160, 160, 160), m_font, false, 0, rt, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
	}

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);
}