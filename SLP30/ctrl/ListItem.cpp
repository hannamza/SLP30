// ListItem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ListItem.h"
#include "SLP30.h"
#include "afxdialogex.h"

// CListItem 대화 상자입니다.

IMPLEMENT_DYNAMIC(CListItem, CDialogEx)

CListItem::CListItem(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
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
		_T("굴림")); // lpszFacename 

	m_font2.CreateFont(13, // nHeight 
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
		_T("굴림")); // lpszFacename 

	m_font3.CreateFont(15, // nHeight 
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
		_T("굴림")); // lpszFacename 

	m_bMouseRbtn = false;
	m_bMouseLbtn = false;

	m_nIndex = -1;

	m_pSite = NULL;

	m_bMouseOn = false;
	m_bSelected = false;

	m_pWnd = pParent;
}

CListItem::~CListItem()
{
}

void CListItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CListItem, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


// CListItem 메시지 처리기입니다.


BOOL CListItem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CListItem::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CListItem::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else
		{
			GetParent()->PreTranslateMessage(pMsg);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CListItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

BOOL CListItem::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CListItem::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CListItem::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CListItem::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CListItem::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		POINT point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		CRect rect;
		GetClientRect(&rect);
		if (!rect.PtInRect(point))
		{
			if (m_bMouseOn)
			{
				KillTimer(1);
				m_bMouseOn = false;
				Redisplay();
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------------------------------------------

void CListItem::SetListItem(pListItem pSite)
{
	m_pSite = pSite;
}

void CListItem::SetItemIndex(int nIndex)
{
	m_nIndex = nIndex;
}

int CListItem::GetItemIndex()
{
	return m_nIndex;
}

void CListItem::SetSelectedState(bool bValue)
{
	m_bSelected = bValue;
	Redisplay();
}

bool CListItem::GetSelectedState()
{
	return m_bSelected;
}

bool CListItem::FindSearchWord(CString sValue)
{
	if (m_pSite)
	{
		CString sText = m_pSite->sText;
		if (sText.Find(sValue, 0) >= 0)
		{
			return true;
		}
	}
	return false;
}

void CListItem::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	if (!m_pSite) {
		return;
	}

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	if (m_bSelected) {
		CCommonDisplay::DrawRect(&memDC, true, RGB(207, 238, 245), RGB(207, 238, 245), rect, 2);
	}
	else if (m_bMouseOn) {
		CCommonDisplay::DrawRect(&memDC, true, RGB(231, 247, 250), RGB(239, 250, 252), rect, 2);
	}
	else {
		CCommonDisplay::DrawRect(&memDC, true, RGB(255, 255, 255), RGB(255, 255, 255), rect, 2);
	}
	CCommonDisplay::DrawCaption(&memDC, m_pSite->sText, RGB(120, 120, 120), m_font2, 0, 0, CRect(10, 0, rect.Width() - 10, rect.bottom), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	//CCommonDisplay::DrawCaption(&memDC, m_pSite->sText, RGB(80, 80, 80), m_font, 0, 0, CRect(10, 24, rect.Width() - 10, 49), DT_LEFT | DT_TOP | DT_SINGLELINE);
	//CCommonDisplay::DrawCaption(&memDC, m_pSite->sText, RGB(120, 120, 120), m_font3, 0, 0, CRect(10, 46, rect.Width() - 15, 62), DT_LEFT | DT_TOP | DT_SINGLELINE);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);
}

void CListItem::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && !m_bMouseLbtn && m_nIndex >= 0)
	{
		m_bMouseLbtn = true;
		m_bMouseOn = false;
		SetCapture();
		Redisplay();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CListItem::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && m_bMouseLbtn && m_nIndex >= 0)
	{
		m_pWnd->PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 1, m_nIndex);
	}
	if (m_bMouseLbtn)
	{
		m_bMouseLbtn = false;
		ReleaseCapture();
		Redisplay();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CListItem::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && !m_bMouseRbtn && m_nIndex >= 0)
	{
		m_bMouseRbtn = true;
		m_bMouseOn = false;
		SetCapture();
		Redisplay();
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}

void CListItem::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && m_bMouseRbtn && m_nIndex >= 0)
	{
		m_pWnd->PostMessageW(WM_RBUTTON_BUTTONLIST_CLICK, m_nIndex, 0);
	}
	if (m_bMouseRbtn)
	{
		m_bMouseRbtn = false;
		ReleaseCapture();
		Redisplay();
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}

void CListItem::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bMouseRbtn && !m_bMouseLbtn)
	{
		CRect rect;
		GetClientRect(&rect);
		if (rect.PtInRect(point))
		{
			if (!m_bMouseOn)
			{
				m_bMouseOn = true;
				Redisplay();
			}
			SetTimer(1, 50, NULL);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CListItem::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseLeave();
}
