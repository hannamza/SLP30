// CustomHeader.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CustomHeader.h"
#include "afxdialogex.h"


// CCustomHeader 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCustomHeader, CDialogEx)

CCustomHeader::CCustomHeader(CWnd* pParent)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_bMouseDown = false;
	m_nSelectHeader = -1;
	m_pParent = pParent;
	m_nHPosition = 0;
	m_nOldHPosition = 0;
	m_colorBk = RGB(0, 162, 232);
	m_colorText = RGB(255, 255, 255);
	m_colorLine = RGB(255, 255, 255);

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
		_T("돋움")); // lpszFacename 

	m_nFontSize = 16;

	m_bFixHeaderSize = false;
	m_bAutoHeaderSize = false;
}

CCustomHeader::~CCustomHeader()
{
	ReleaseRectList();
}

void CCustomHeader::ReleaseRectList()
{
	CRect* pRect;
	while (!m_headerRect.IsEmpty())
	{
		pRect = m_headerRect.RemoveHead();
		SAFE_DELETE(pRect);
	}
}

void CCustomHeader::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCustomHeader, CDialogEx)
	ON_WM_DESTROY()
//	ON_WM_MOVE()
ON_WM_PAINT()
ON_WM_TIMER()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDBLCLK()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CCustomHeader 메시지 처리기입니다.


BOOL CCustomHeader::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCustomHeader::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CCustomHeader::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CCustomHeader::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

BOOL CCustomHeader::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCustomHeader::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CCustomHeader::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bFixHeaderSize) {
		ResizeCheckHeader(point, 2);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCustomHeader::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bFixHeaderSize) {
		if (m_bMouseDown) {
			m_bMouseDown = false;
			m_nSelectHeader = -1;
			ReleaseCapture();
		}
		else {
			CRect* pRect = NULL;
			int nCount = m_nHeaderSize.GetCount();
			for (int nIndex = 0; nIndex < nCount; nIndex++)
			{
				pRect = m_headerRect.GetAt(m_headerRect.FindIndex(nIndex));
				if (pRect->PtInRect(point))
				{
					m_pParent->PostMessage(RESIZE_NOTIFY, HEADER_CLICK, nIndex);
					return;
				}
			}
		}
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCustomHeader::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nIndex = ResizeCheckHeader(point, 3);
	if (nIndex >= 0) {
		TRACE("header line(%d) double click!\n", nIndex);
		m_pParent->PostMessage(RESIZE_NOTIFY, HEADER_DBCLICK, nIndex);
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CCustomHeader::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (!m_bFixHeaderSize) {
		if (m_bMouseDown) {
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));

			CPoint pt;
			pt = point;
			ClientToScreen(&pt);
			if (pt.x == m_point.x)
			{
				return;
			}
			int nX = m_point.x - pt.x;
			if (m_nSelectHeader < 0) {
				m_bMouseDown = false;
				m_nSelectHeader = -1;
				ReleaseCapture();
				return;
			}

			CRect* pRect = m_headerRect.GetAt(m_headerRect.FindIndex(m_nSelectHeader));
			if (pRect->right - nX < pRect->left + 10) {
				nX = pRect->right - (pRect->left + 10);
				pRect->right = pRect->left + 10;
			}
			else {
				pRect->right -= nX;
			}
			point.x = pRect->right;
			pt = point;
			ClientToScreen(&pt);
			m_point = pt;
			m_nOldHPosition = m_nHPosition;

			for (int nIndex = m_nSelectHeader + 1; nIndex < m_headerRect.GetCount(); nIndex++)
			{
				pRect = m_headerRect.GetAt(m_headerRect.FindIndex(nIndex));
				pRect->left -= nX;
				pRect->right -= nX;
			}
			CheckHeaderWidth();
			m_pParent->PostMessage(RESIZE_NOTIFY, HEADER_RESIZE, m_nSelectHeader);
			Redisplay();
		}
		else {
			ResizeCheckHeader(point);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCustomHeader::CheckHeaderWidth()
{
	CRect* pRect = NULL;
	CUIntArray arrayList;
	int nCount = m_nHeaderSize.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pRect = m_headerRect.GetAt(m_headerRect.FindIndex(nIndex));
		arrayList.Add(pRect->Width());
	}
}

int CCustomHeader::ResizeCheckHeader(CPoint & point, int nType/* = 0*/)
{
	CRect* pRect = NULL, rect, checkRect;
	GetClientRect(&rect);
	int nCount = m_nHeaderSize.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pRect = m_headerRect.GetAt(m_headerRect.FindIndex(nIndex));
		checkRect.SetRect(pRect->right - 5, 0, pRect->right + 5, rect.bottom);
		if (checkRect.PtInRect(point))
		{
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			switch (nType)
			{
			case 0:
			default:
				break;
			case 2:
				m_point = point;
				ClientToScreen(&m_point);
				m_nOldHPosition = m_nHPosition;
				m_bMouseDown = true;
				SetCapture();
				break;
			case 3:
				return nIndex;
			}
			m_nSelectHeader = nIndex;
			break;
		}
	}
	return -1;
}

void CCustomHeader::SetHeaderInfo(CStringArray & arrayName, CUIntArray & arraySize)
{
	ReleaseRectList();

	m_sHeaderList.RemoveAll();
	m_nHeaderSize.RemoveAll();

	m_sHeaderList.Copy(arrayName);
	m_nHeaderSize.Copy(arraySize);

	ResizeHeaderRect();
}

void CCustomHeader::FixHeaderSize(bool bFix)
{
	m_bFixHeaderSize = bFix;
}

void CCustomHeader::AutoHeaderSize(bool bAuto)
{
	m_bAutoHeaderSize = bAuto;
	if (bAuto) {
		CDC* _pDC = GetDC();
		CSize size;
		m_nHeaderSize.RemoveAll();
		int nCount = m_sHeaderList.GetCount();
		for (int nIndex = 0; nIndex < nCount; nIndex++) {
			size = CCommonFunc::GetTextExtent(_pDC, m_font, m_sHeaderList.GetAt(nIndex));
			m_nHeaderSize.Add(size.cx + 8);
		}
		ReleaseDC(_pDC);
		ResizeHeaderRect();
	}
	Invalidate();
}

void CCustomHeader::GetHeaderSizeEx(CUIntArray & arrayList)
{
	arrayList.RemoveAll();
	arrayList.Copy(m_nHeaderSize);
}

void CCustomHeader::SetCurrentHPosition(int nPosition)
{
	m_nHPosition = nPosition;
}

int CCustomHeader::GetHeaderWidth()
{
	CRect* pRect = NULL;
	int nWidth = 0;
	if (m_headerRect.GetCount() > 0) {
		pRect = m_headerRect.GetAt(m_headerRect.FindIndex(m_headerRect.GetCount() - 1));
		nWidth = pRect->right;
	}
	/*int nCount = m_nHeaderSize.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pRect = m_headerRect.GetAt(m_headerRect.FindIndex(nIndex));
		nWidth += pRect->Width();
	}*/
	return nWidth;
}

void CCustomHeader::GetHeaderSize(CUIntArray & arraySize)
{
	arraySize.RemoveAll();
	CRect* pRect = NULL;
	int nCount = m_nHeaderSize.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pRect = m_headerRect.GetAt(m_headerRect.FindIndex(nIndex));
		arraySize.Add(pRect->Width());
	}
}

void CCustomHeader::SetHeaderColor(COLORREF bkColor, COLORREF lineColor, COLORREF textColor)
{
	m_colorBk = bkColor;
	m_colorText = textColor;
	m_colorLine = lineColor;

	Invalidate();
}

void CCustomHeader::ResizeHeaderRect()
{
	CRect* pRect = NULL, rect;
	GetClientRect(&rect);
	int nStartPosition = 0, nWidth = 0;
	int nCount = m_nHeaderSize.GetCount();
	for (int nIndex = m_headerRect.GetCount(); nIndex < nCount; nIndex++)
	{
		pRect = new CRect();
		pRect->SetRect(0, 0, 0, 0);
		m_headerRect.AddTail(pRect);
	}
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pRect = m_headerRect.GetAt(m_headerRect.FindIndex(nIndex));
		nWidth = m_nHeaderSize.GetAt(nIndex);
		pRect->SetRect(nStartPosition, 0, nStartPosition + nWidth, rect.bottom);
		nStartPosition += nWidth;
	}
}

void CCustomHeader::SetFontSize(int nSize)
{
	LOGFONT lf;
	m_font.GetLogFont(&lf);
	m_font.DeleteObject();
	lf.lfHeight = nSize;
	m_font.CreateFontIndirect(&lf);
	m_nFontSize = nSize;

	AutoHeaderSize(m_bAutoHeaderSize);
}

void CCustomHeader::SetFontBold(UINT nValue)
{
	LOGFONT lf;
	m_font.GetLogFont(&lf);
	m_font.DeleteObject();
	lf.lfWeight = nValue;
	m_font.CreateFontIndirect(&lf);
	Invalidate();
}

void CCustomHeader::SetFontName(CString sName)
{
	LOGFONT lf;
	m_font.GetLogFont(&lf);
	m_font.DeleteObject();
	_tcscpy_s(lf.lfFaceName, sName.GetBuffer(0));
	m_font.CreateFontIndirect(&lf);
	Invalidate();
}

void CCustomHeader::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawRect(&memDC, true, m_colorBk, m_colorBk, rect);

	CRect* pRect = NULL, textRect;
	CString sText;
	int nCount = m_nHeaderSize.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pRect = m_headerRect.GetAt(m_headerRect.FindIndex(nIndex));
		sText = m_sHeaderList.GetAt(nIndex);
		textRect.CopyRect(pRect);
		CCommonDisplay::DrawLine(&memDC, pRect->right, pRect->top, pRect->right, pRect->bottom, PS_SOLID, 1, m_colorLine);
		if (sText.Find(L"\n", 0) >= 0) {
			textRect.top += (pRect->Height() / 2) - m_nFontSize;
		}
		else {
			textRect.top += (pRect->Height() / 2) - (m_nFontSize / 2);
		}

		CCommonDisplay::DrawCaption(&memDC, sText, m_colorText, m_font, false, 0, textRect, DT_CENTER | DT_TOP);
	}

	// message
	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	::DeleteObject(memDC);
	ReleaseDC(_pDC);
}

