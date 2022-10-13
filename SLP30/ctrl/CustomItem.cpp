// CustomItem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CustomItem.h"
#include "afxdialogex.h"

#include "MessagePopup.h"	//20221013 GBM - 건물 정보 확정 시 ReadOnly주기 위한 추가

// CCustomItem 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCustomItem, CDialogEx)

CCustomItem::CCustomItem(CWnd* pParent)
	: CDialogEx(IDD_CUSTOM_ITEM_DIALOG, pParent)
{
	m_font3.CreateFont(15, // nHeight 
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

	m_font2.CreateFont(15, // nHeight 
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

	m_bMouseRbtn = false;
	m_bMouseLbtn = false;

	m_nIndex = -1;
	m_nSelectIndex = -1;

	m_bMouseOn = false;
	m_bSelected = false;

	m_pWnd = pParent;

	m_bkColor = RGB(207, 238, 245);
	m_bkSelColor = RGB(112, 146, 190);
	m_textColor = RGB(30, 30, 30);
	m_textSelColor = RGB(255, 255, 255);
	m_lineColor = RGB(255, 255, 255);

	m_bReadOnlyState = false;	//20221013 GBM - ReadOnly 주기 위한 추가
}

CCustomItem::~CCustomItem()
{
}

void CCustomItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITEM_EDIT, m_editText);
}


BEGIN_MESSAGE_MAP(CCustomItem, CDialogEx)
	ON_MESSAGE(WM_RESIZE_COLUMN, OnResizeColumn)
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
	ON_EN_KILLFOCUS(IDC_ITEM_EDIT, &CCustomItem::OnEnKillfocusItemEdit)
END_MESSAGE_MAP()


// CCustomItem 메시지 처리기입니다.


BOOL CCustomItem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_editText.ShowWindow(SW_HIDE);

	CCommonFunc::SetHanMode(m_editText.GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCustomItem::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CCustomItem::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		//20221013 GBM start - ReadOnly 주기 위한 추가
		if (m_bReadOnlyState)
		{	
			CMessagePopup popup(L"회로 기본 입력", L"\n\n\n건물정보는 이미 결정되었으므로 변경할 수 없습니다.", MB_OK, this);
			UINT nResult = popup.DoModal();
			return TRUE;
		}
		//20221013 GBM end

		if (pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
		else if (m_bSelected
			&& m_nSelectIndex >= 0
			&& ::GetFocus() == m_editText.m_hWnd
			&& m_arrayType.GetCount() > m_nSelectIndex
			&& m_arrayType.GetAt(m_nSelectIndex) == POPUP_TYPE_EDIT
			&& (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT)) {
			return CDialogEx::PreTranslateMessage(pMsg);
		}
		else
		{
			if (m_bSelected
				&& m_nSelectIndex >= 0 
				&& ::GetFocus() == m_editText.m_hWnd
				&& m_arrayType.GetCount() > m_nSelectIndex
				&& m_arrayType.GetAt(m_nSelectIndex) == POPUP_TYPE_EDIT
				&& pMsg->wParam == VK_RETURN) {
				pMsg->wParam = 0x99;
			}
			GetParent()->PreTranslateMessage(pMsg);
		}

		if (m_bSelected 
			&& m_nSelectIndex >= 0 
			&& ::GetFocus() == m_editText.m_hWnd 
			&& m_arrayType.GetCount() > m_nSelectIndex
			&& m_arrayType.GetAt(m_nSelectIndex) == POPUP_TYPE_EDIT) {
			SetTimer(98, 10, NULL);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CCustomItem::CheckLimitEditText()
{
	if (m_bSelected
		&& m_nSelectIndex >= 0
		&& ::GetFocus() == m_editText.m_hWnd
		&& m_arrayType.GetCount() > m_nSelectIndex
		&& m_arrayLimit.GetCount() > m_nSelectIndex
		&& m_arrayType.GetAt(m_nSelectIndex) == POPUP_TYPE_EDIT) {
		CString sText;
		m_editText.GetWindowText(sText);

		int nLimit = m_arrayLimit.GetAt(m_nSelectIndex);
		CStringA sTemp = CCommonFunc::WCharToChar(sText.GetBuffer(0));
		int len = sTemp.GetLength();
		if (nLimit > 0 && len > nLimit) {
			sText = CCommonFunc::CharToWCHAR(sTemp.Left(nLimit).GetBuffer(0));
			m_editText.SetSel(0, -1);
			//m_editText.ReplaceSel(sText);
			m_editText.SetFocus();
		}
		sTemp.ReleaseBuffer();
	}
}

HBRUSH CCustomItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

BOOL CCustomItem::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return 0;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCustomItem::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CCustomItem::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CCustomItem::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CCustomItem::OnTimer(UINT_PTR nIDEvent)
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
	else if (nIDEvent == 97) {
		if (!m_bSelected) {
			//ReleaseCapture();
			KillTimer(97);
		}
		else {
			SHORT value = GetAsyncKeyState(VK_LBUTTON);
			SHORT value1 = GetAsyncKeyState(VK_RBUTTON);
			CRect rect;
			m_pWnd->GetClientRect(&rect);
			m_pWnd->ClientToScreen(&rect);
			CPoint point;
			GetCursorPos(&point);
			bool bRect = rect.PtInRect(point);
			if (!bRect && (value == -32767 || value == -32768 || value1 == -32767 || value1 == -32768)
				&& !CCommonState::ie()->m_bMenu) {
				KillTimer(97);
				TypeTextRefresh();
				m_pWnd->PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, -1, -1);
			}
			//TRACE("=====> %d / %d\n", value, value1);
			//SetCapture();
		}
	}
	else if (nIDEvent == 98) {
		KillTimer(98);
		CheckLimitEditText();
	}
	else if (nIDEvent == 99) {
		SHORT value = GetKeyState(VK_LBUTTON);
		if (value == -128 || value == -127 || !m_bSelected || m_arrayType.GetAt(m_nSelectIndex) != POPUP_TYPE_EDIT) {
			KillTimer(99);
		}
		if (value != -128 && value != -127 && ::GetFocus() != m_editText.m_hWnd) {
			CString sText;
			m_editText.GetWindowText(sText);
			m_editText.SetSel(0, -1);
			//m_editText.ReplaceSel(sText);
			m_editText.SetFocus();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------------------------------------------

void CCustomItem::SetItemIndex(int nIndex)
{
	m_nIndex = nIndex;
	if (m_nIndex == -1) {
		if (m_bSelected) {
			ReleaseCapture();
		}
		m_bSelected = false;
		m_nSelectIndex = -1;
		m_editText.ShowWindow(SW_HIDE);
	}
}

void CCustomItem::SetFocusItem(int nType)
{
	if (nType == 0) {
		this->SetFocus();
	}
	else if (nType == 1) {
		SetTimer(99, 10, NULL);
	}
}

int CCustomItem::GetItemIndex()
{
	return m_nIndex;
}

CString CCustomItem::GetEditText()
{
	CString sText;
	m_editText.GetWindowText(sText);
	m_editText.SetWindowText(L"");
	return sText;
}

void CCustomItem::SetSelectedState(bool bState, int nCol)
{
	/*if (m_bSelected == bState && m_nSelectIndex == nCol) {
		return;
	}*/
	/*if (m_bSelected && !bState) {
		ReleaseCapture();
	}*/
	m_bSelected = bState;
	if (!bState) {
		m_nSelectIndex = -1;
	}
	else {
		m_nSelectIndex = nCol;
		CheckEditItem(nCol);
		SetTimer(97, 60, NULL);
	}
	Redisplay();
}

void CCustomItem::CheckEditItem(int nCol)
{
	if (nCol >= 0 && nCol < m_arrayType.GetCount() && m_arrayType.GetAt(nCol) == POPUP_TYPE_EDIT) {
		pItemValue pValue = NULL;
		bool bFind = false;
		for (int i = 0; i < m_listItemValue.GetCount(); i++) {
			pValue = m_listItemValue.GetAt(m_listItemValue.FindIndex(i));
			if (pValue && pValue->nColIndex == m_nSelectIndex) {
				m_editText.SetWindowText(pValue->sText);
				bFind = true;
				break;
			}
			pValue = NULL;
		}
		CString sText;
		if (!bFind) {
			m_editText.SetSel(0, -1);
			//m_editText.ReplaceSel(sText);
		}
		CRect rect, rt;
		GetSelectItemRect(rect);
		ScreenToClient(&rect);
		rt.SetRect(rect.left, rect.bottom - 23, rect.right, rect.bottom);
		m_editText.MoveWindow(rt);
		m_editText.SetFont(&m_font);
		m_editText.ShowWindow(SW_SHOW);
		m_editText.GetWindowText(sText);
		m_editText.SetSel(0, -1);
		//m_editText.ReplaceSel(sText);
		SetTimer(99, 10, NULL);
	}
	else {
		m_editText.ShowWindow(SW_HIDE);
	}
}

bool CCustomItem::GetSelectedState()
{
	return m_bSelected;
}

bool CCustomItem::SetItemText(CList<pItemValue, pItemValue> & listText)
{
	pItemValue pValue = NULL;
	m_textSync.Enter();
	m_listItemValue.RemoveAll();
	POSITION pos = listText.GetHeadPosition();
	while (pos) {
		pValue = listText.GetNext(pos);
		m_listItemValue.AddTail(pValue);
	}
	m_textSync.Leave();

	return true;
}

void CCustomItem::SetRowColor(COLORREF bkColor, COLORREF bkSelColor, COLORREF textColor, COLORREF textSelColor, COLORREF lineColor)
{
	m_bkColor = bkColor;
	m_bkSelColor = bkSelColor;
	m_textColor = textColor;
	m_textSelColor = textSelColor;
	m_lineColor = lineColor;
}

bool CCustomItem::GetSelectItemRect(CRect & rect)
{
	if (!m_bSelected || m_nSelectIndex < 0) {
		return false;
	}
	CRect rt, rtemp;
	GetClientRect(&rt);
	rt.right = 0;
	for (int nIndex = 0; nIndex < m_arraySize.GetCount(); nIndex++)
	{
		pItemValue pValue = NULL;
		rt.right += m_arraySize.GetAt(nIndex);
		if (m_bSelected && m_nSelectIndex == nIndex) {
			ClientToScreen(&rt);
			rect.CopyRect(&rt);
			return true;
		}
		rt.left = rt.right + 1;
	}
	return false;
}

void CCustomItem::SetFontSize(int nSize)
{
	LOGFONT lf;
	m_font.GetLogFont(&lf);
	m_font.DeleteObject();
	lf.lfHeight = nSize;
	m_font.CreateFontIndirect(&lf);
	Invalidate();
}

void CCustomItem::SetFontBold(UINT nValue)
{
	LOGFONT lf;
	m_font.GetLogFont(&lf);
	m_font.DeleteObject();
	lf.lfWeight = nValue;
	m_font.CreateFontIndirect(&lf);
	Invalidate();
}

void CCustomItem::SetFontName(CString sName)
{
	LOGFONT lf;
	m_font.GetLogFont(&lf);
	m_font.DeleteObject();
	_tcscpy_s(lf.lfFaceName, sName.GetBuffer(0));
	m_font.CreateFontIndirect(&lf);
	Invalidate();
}

void CCustomItem::SetItemLimit(CUIntArray & arrayList)
{
	m_arrayLimit.Copy(arrayList);
}

void CCustomItem::Redisplay()
{
	CString sTemp;
	CRect rect, rt, rtemp;
	UINT nDTAlign = DT_CENTER;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawRect(&memDC, true, m_bkColor, m_bkColor, rect, 1);

	rt.CopyRect(&rect);
	rt.right = 0;
	int nColType = -1;
	for (int nIndex = 0; nIndex < m_arraySize.GetCount(); nIndex++)
	{
		pItemValue pValue = NULL;
		rt.right += m_arraySize.GetAt(nIndex);
		CCommonDisplay::DrawLine(&memDC, rt.right, 0, rt.right, rt.bottom, PS_SOLID, 1, m_lineColor);

		for (int i = 0; i < m_listItemValue.GetCount(); i++) {
			pValue = m_listItemValue.GetAt(m_listItemValue.FindIndex(i));
			if (pValue && pValue->nColIndex == nIndex) {
				break;
			}
			pValue = NULL;
		}
		if (m_arrayType.GetCount() > nIndex) {
			nColType = m_arrayType.GetAt(nIndex);
		}
		if (m_arrayAlign.GetCount() > nIndex) {
			nDTAlign = m_arrayAlign.GetAt(nIndex);
		}
		else {
			nDTAlign = DT_CENTER;
		}
		rtemp.CopyRect(&rt);
		rtemp.bottom -= 1;
		if (m_bSelected && m_nSelectIndex == nIndex) {
			if (nColType == POPUP_TYPE_EDIT) {
				CCommonDisplay::DrawRect(&memDC, true, RGB(255, 255, 255), RGB(255, 255, 255), rtemp, 1);
			}
			else {
				CCommonDisplay::DrawRect(&memDC, true, m_bkSelColor, m_bkSelColor, rtemp, 1);
			}
			rtemp.left += 5;
			rtemp.right -= 5;
			if (nColType == POPUP_TYPE_SEQ) {
				sTemp.Format(L"%d", GetItemIndex() + 1);
				CCommonDisplay::DrawCaption(&memDC, sTemp, m_textSelColor, m_font, false, 0, rtemp, nDTAlign | DT_VCENTER | DT_SINGLELINE);
			}
			else if (pValue && pValue->sText.GetLength() > 0) {
				CCommonDisplay::DrawCaption(&memDC, pValue->sText, m_textSelColor, m_font, false, 0, rtemp, nDTAlign | DT_VCENTER | DT_SINGLELINE);
			}
		}
		else {
			rtemp.left += 5;
			rtemp.right -= 5;
			if (nColType == POPUP_TYPE_SEQ) {
				sTemp.Format(L"%d", GetItemIndex() + 1);
				CCommonDisplay::DrawCaption(&memDC, sTemp, m_textColor, m_font, false, 0, rtemp, nDTAlign | DT_VCENTER | DT_SINGLELINE);
			}
			else if (pValue && pValue->sText.GetLength() > 0) {
				CCommonDisplay::DrawCaption(&memDC, pValue->sText, m_textColor, m_font, false, 0, rtemp, nDTAlign | DT_VCENTER | DT_SINGLELINE);
			}
			if (!m_bSelected) {
				m_editText.ShowWindow(SW_HIDE);
			}
		}
		rt.left = rt.right + 1;
	}
	CCommonDisplay::DrawLine(&memDC, 0, rect.bottom-1, rect.right, rect.bottom-1, PS_SOLID, 1, m_lineColor);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);
}

void CCustomItem::CheckSelectItemRefresh(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && /*m_bMouseLbtn &&*/ m_nIndex >= 0)
	{
		rect.right = 0;
		for (int i = 0; i < m_arraySize.GetCount(); i++) {
			rect.right += m_arraySize.GetAt(i);
			if (rect.PtInRect(point)) {
				if (m_nSelectIndex != i)
				{
					TypeTextRefresh();
					if (nFlags == 0) {
						m_bSelected = true;
						m_nSelectIndex = i;
						m_pWnd->PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, m_nIndex, i);
					}
				}
				break;
			}
			rect.left = rect.right;
		}
	}
}

void CCustomItem::TypeTextRefresh()
{
	if (m_bSelected && m_nSelectIndex >= 0 && m_nSelectIndex < m_arrayType.GetCount()
		&& m_arrayType.GetAt(m_nSelectIndex) == POPUP_TYPE_EDIT)
	{
		keybd_event(VK_ESCAPE, 0, 0, 0);
		Sleep(50);
		keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
	}
}

void CCustomItem::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && !m_bMouseLbtn && m_nIndex >= 0)
	{
		CheckSelectItemRefresh(1, point);

		m_bMouseLbtn = true;
		m_bMouseOn = false;
		SetCapture();
		//Redisplay();
	}
	else if (m_bSelected) {
		TypeTextRefresh();
		m_pWnd->PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, -1, -1);

		/*if (m_nSelectIndex < m_arrayType.GetCount() && m_arrayType.GetAt(m_nSelectIndex) == POPUP_TYPE_EDIT) {
			CString sText = GetEditText();
			ModItemText
		}
		m_bSelected = false;
		m_nSelectIndex = -1;
		Redisplay();*/
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCustomItem::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CRect rect;
	//GetClientRect(&rect);
	//if (rect.PtInRect(point) && /*m_bMouseLbtn &&*/ m_nIndex >= 0)
	//{
	//	rect.right = 0;
	//	for (int i = 0; i < m_arraySize.GetCount(); i++) {
	//		rect.right += m_arraySize.GetAt(i);
	//		if (rect.PtInRect(point)) {
	//			/*if (m_bSelected && m_nSelectIndex == i) {
	//				break;
	//			}*/
	//			/*m_bSelected = true;
	//			m_nSelectIndex = i;*/

	//			TypeTextRefresh();

	//			m_pWnd->PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, m_nIndex, i);
	//			//CheckEditItem(m_nSelectIndex);
	//			break;
	//		}
	//		rect.left = rect.right;
	//	}
	//	
	//}
	CheckSelectItemRefresh(0, point);

	if (m_bMouseLbtn)
	{
		m_bMouseLbtn = false;
		ReleaseCapture();
		//Redisplay();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCustomItem::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && !m_bMouseRbtn && m_nIndex >= 0)
	{
		CheckSelectItemRefresh(1, point);

		m_bMouseRbtn = true;
		m_bMouseOn = false;
		SetCapture();
		//Redisplay();
	}
	else if (m_bSelected) {
		TypeTextRefresh();
		m_pWnd->PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, -1, -1);
		/*ReleaseCapture();
		m_bSelected = false;
		m_nSelectIndex = -1;
		Redisplay();*/
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}

void CCustomItem::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/*CRect rect;
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
	}*/

	CheckSelectItemRefresh(0, point);

	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && /*m_bMouseRbtn &&*/ m_nIndex >= 0)
	{
		rect.right = 0;
		for (int i = 0; i < m_arraySize.GetCount(); i++) {
			rect.right += m_arraySize.GetAt(i);
			if (rect.PtInRect(point)) {
				//m_bSelected = true;
				//m_nSelectIndex = i;
				m_pWnd->PostMessage(WM_RBUTTON_BUTTONLIST_CLICK, m_nIndex, i);
				break;
			}
			rect.left = rect.right;
		}
	}
	if (m_bMouseRbtn)
	{
		m_bMouseRbtn = false;
		ReleaseCapture();
		//Redisplay();
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}

void CCustomItem::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/*if (!m_bMouseRbtn && !m_bMouseLbtn)
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
	}*/

	CDialogEx::OnMouseMove(nFlags, point);
}

LRESULT CCustomItem::OnResizeColumn(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;
	if (nType == 0) {
		CUIntArray* pSize = (CUIntArray*)lParam;
		if (!pSize) {
			return 0;
		}
		m_sizeSync.Enter();

		m_arraySize.RemoveAll();
		m_arraySize.Copy(*pSize);

		m_sizeSync.Leave();

		Redisplay();
	}
	else if (nType == 1)
	{
		CUIntArray* pType = (CUIntArray*)lParam;
		if (!pType) {
			return 0;
		}
		m_arrayType.RemoveAll();
		m_arrayType.Copy(*pType);
	}
	else if (nType == 2)
	{
		CUIntArray* pAlign = (CUIntArray*)lParam;
		if (!pAlign) {
			return 0;
		}
		m_arrayAlign.RemoveAll();
		m_arrayAlign.Copy(*pAlign);
		
		//Redisplay();
	}

	return 0;
}

void CCustomItem::SetRowInfo(CUIntArray & arraySize, CUIntArray & arrayType, CUIntArray & arrayAlign)
{
	bool bChange = false;

	m_sizeSync.Enter();
	if (m_arraySize.GetCount() == arraySize.GetCount()) {
		for (int i = 0; i < m_arraySize.GetCount(); i++) {
			if (m_arraySize.GetAt(i) != arraySize.GetAt(i)) {
				bChange = true;
				break;
			}
		}
	}
	else {
		bChange = true;
	}
	m_arraySize.Copy(arraySize);
	m_sizeSync.Leave();

	if (!bChange && m_arrayType.GetCount() == arrayType.GetCount()) {
		for (int i = 0; i < m_arrayType.GetCount(); i++) {
			if (m_arrayType.GetAt(i) != arrayType.GetAt(i)) {
				bChange = true;
				break;
			}
		}
	}
	else {
		bChange = true;
	}
	m_arrayType.Copy(arrayType);

	if (!bChange && m_arrayAlign.GetCount() == arrayAlign.GetCount()) {
		for (int i = 0; i < m_arrayAlign.GetCount(); i++) {
			if (m_arrayAlign.GetAt(i) != arrayAlign.GetAt(i)) {
				bChange = true;
				break;
			}
		}
	}
	else {
		bChange = true;
	}
	m_arrayAlign.Copy(arrayAlign);

	if (bChange) {
		Redisplay();
	}
}

void CCustomItem::OnEnKillfocusItemEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CCustomItem::SetReadOnly(bool bReadOnly)
{
	m_bReadOnlyState = bReadOnly;
}
