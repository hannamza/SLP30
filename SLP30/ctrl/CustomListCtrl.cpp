// CustomListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CustomListCtrl.h"


// CCustomListCtrl

IMPLEMENT_DYNCREATE(CCustomListCtrl, CScrollView)

CCustomListCtrl::CCustomListCtrl()
{
	m_nMapMode = MM_TEXT;
	m_pHeader = NULL;
	m_nListIndex = -1;
	m_pList = NULL;
	m_nListWidth = 0;
	m_nHeaderHeight = HEADER_HEIGHT_SIZE;
	m_nRowHeight = LIST_ITEM_HEIGHT;
	m_bUseTabkey = true;

	m_headerBk = RGB(0, 162, 232);
	m_headerText = RGB(255, 255, 255);
	m_headerLine = RGB(255, 255, 255);

	m_rowBkNormal = RGB(207, 238, 245);
	m_rowBkSelect = RGB(112, 146, 190);
	m_rowLine = RGB(255, 255, 255);
	
	m_rowTextNormal = RGB(30, 30, 30);
	m_rowTextSelect = RGB(255, 255, 255);

	m_pPopupTextDlg = NULL;
	m_pPopupListDlg = NULL;

	m_nListPopupWidth = 180;
	m_nListPopupHeight = 250;

	m_nTextPopupWidth = 350;
	m_nTextPopupHeight = 33;

	m_nFontSize = 15;
	m_nHeaderFontSize = 16;
	m_nFontBold = FW_NORMAL;
	m_nHeaderFontBold = FW_BOLD;
	m_sRowFontName = L"굴림";
	m_sHeaderFontName = L"굴림";

	m_bFixHeaderSize = false;
	m_bShowScrollbar = true;
	m_bHeaderAutoSize = false;

	m_bDisable = false;
	m_bLastInsertRow = false;
	m_bFirstInsertRow = false;
}

CCustomListCtrl::~CCustomListCtrl()
{
	SAFE_DELETE(m_pHeader);
	SAFE_DELETE(m_pPopupTextDlg);
	SAFE_DELETE(m_pPopupListDlg);

	pPopupList pList = NULL;
	while (!m_popupList.IsEmpty()) {
		pList = m_popupList.RemoveHead();
		SAFE_DELETE(pList);
	}

	POSITION pos = m_stateString.GetHeadPosition();
	pStateString pState;
	while (pos) {
		pState = m_stateString.GetNext(pos);
		pState->value.RemoveAll();
		SAFE_DELETE(pState);
	}
}


BEGIN_MESSAGE_MAP(CCustomListCtrl, CScrollView)
	ON_MESSAGE(RESIZE_NOTIFY, OnHeaderNotify)
	ON_MESSAGE(SCROLL_NOTIFY, OnListNotify)
	ON_MESSAGE(LIST_SELECTION, OnSelectionNotify)
	ON_MESSAGE(WM_PAGE_MOVE_KEY, OnListPageKey)
	ON_MESSAGE(INPUT_TEXT_INFO, OnInputTextInfo)
	ON_MESSAGE(INPUT_LIST_INFO, OnInputListInfo)
	ON_MESSAGE(LIST_CONTROL, OnListControl)
	ON_MESSAGE(LIST_HORIZON, OnListHorizon)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEHWHEEL()
END_MESSAGE_MAP()


// CCustomListCtrl 그리기입니다.

void CCustomListCtrl::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);

	CRect rect, mRect;
	GetClientRect(&rect);

	CCommonFunc::GetMonitorInfoEx(mRect);

	rect.right = mRect.Width();
	rect.bottom = rect.top + m_nHeaderHeight;

	m_pHeader = new CCustomHeader(this);
	m_pHeader->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pHeader->MoveWindow(rect);
	m_pHeader->SetHeaderInfo(m_sHeaderList, m_nHeaderSize);
	m_pHeader->SetHeaderColor(m_headerBk, m_headerLine, m_headerText);
	if (m_bHeaderAutoSize) {
		m_pHeader->AutoHeaderSize(m_bHeaderAutoSize);
		m_pHeader->GetHeaderSizeEx(m_nHeaderSize);
	}
	m_pHeader->SetFontBold(m_nHeaderFontBold);
	m_pHeader->SetFontSize(m_nHeaderFontSize);
	m_pHeader->SetFontName(m_sHeaderFontName);
	m_pHeader->FixHeaderSize(m_bFixHeaderSize);
	m_pHeader->ShowWindow(SW_SHOW);

	CRuntimeClass	*pRuntimeClass;
	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomList))) {
		if (NULL != (m_pList = (CCustomList *)(pRuntimeClass->CreateObject()))) {
			m_pList->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}
	m_pList->SetColumnInfo(m_nHeaderSize, m_nHeaderType, m_nAlign);
	m_pList->SetRowHeight(m_nRowHeight);
	m_pList->SetRowColor(m_rowBkNormal, m_rowBkSelect, m_rowTextNormal, m_rowTextSelect, m_rowLine);
	m_pList->SetParentWnd(this);
	m_pList->UseTabKey(m_bUseTabkey);
	m_pList->SetFontBold(m_nFontBold);
	m_pList->SetFontSize(m_nFontSize);
	m_pList->SetFontName(m_sRowFontName);
	m_pList->SetColumnLimit(m_nLimit);
	m_pList->SetDisableMenu(m_bDisable);
	m_pList->EnableLastInsertRow(m_bLastInsertRow, m_bFirstInsertRow);

	POSITION pos = m_stateString.GetHeadPosition();
	pStateString pState;
	while (pos) {
		pState = m_stateString.GetNext(pos);
		for (int nIndex = 0; nIndex < pState->value.GetCount(); nIndex++) {
			m_pList->AddListItemState(pState->nColumn, pState->value.GetAt(nIndex));
		}
	}
	m_pList->SendMessage(WM_INITIALUPDATE);

	GetClientRect(&rect);
	rect.top += m_nHeaderHeight;
	m_pList->MoveWindow(rect);
	m_pList->ShowWindow(SW_SHOW);
}

void CCustomListCtrl::SetListIndex(int nIndex)
{
	m_nListIndex = nIndex;
}

bool CCustomListCtrl::AddItem(int nCount)
{
	bool bResult = true;
	if (m_pList) {
		for (int nIndex = 0; nIndex < nCount; nIndex++) {
			if (m_pList->AddItem() == -1) {
				bResult = false;
				break;
			}
		}
	}
	CSize sizeTotal;
	int nSize = 0;
	if (m_pHeader) {
		nSize = m_pHeader->GetHeaderWidth();
	}
	sizeTotal.cy = 0;
	if (m_pList) {
		sizeTotal.cy = m_pList->GetListHeight() + m_nHeaderHeight;
	}
	sizeTotal.cx = nSize;
	m_nListWidth = nSize;

	if (!m_bShowScrollbar) {
		::ShowScrollBar(m_hWnd, SB_BOTH, FALSE);
	}
	else {
		SetScrollSizes(MM_TEXT, sizeTotal);
	}


	m_pList->Invalidate();

	return bResult;
}

bool CCustomListCtrl::ReleaseListItem()
{
	m_pList->ReleaseListItem();

	return true;
}

int CCustomListCtrl::AddItem()
{
	bool bResult = false;
	if (m_pList) {
		if (m_pList->AddItem() != -1) {
			bResult = true;
		}
	}
	CSize sizeTotal;
	int nSize = 0;
	if (m_pHeader) {
		nSize = m_pHeader->GetHeaderWidth();
	}
	sizeTotal.cy = 0;
	if (m_pList) {
		sizeTotal.cy = m_pList->GetListHeight() + m_nHeaderHeight;
	}
	sizeTotal.cx = nSize;
	m_nListWidth = nSize;

	if (!m_bShowScrollbar) {
		::ShowScrollBar(m_hWnd, SB_BOTH, FALSE);
	}
	else {
		SetScrollSizes(MM_TEXT, sizeTotal);
	}


	m_pList->Invalidate();

	return bResult;
}

int CCustomListCtrl::InsertItem(int nIndex)
{
	if (m_pList) {
		return m_pList->InsertItem(nIndex);
	}
	m_pList->Invalidate();
	return -1;
}

void CCustomListCtrl::SetHeaderInfo(TCHAR** szHeaderList, int* nSize, int *nType, int* nAlign/*=NULL*/)
{
	m_sHeaderList.RemoveAll();
	m_nHeaderSize.RemoveAll();
	m_nHeaderType.RemoveAll();
	m_nAlign.RemoveAll();

	int nIndex = 0;
	bool bContinue = true;
	while (TRUE)
	{
		if (!szHeaderList[nIndex])
			break;
		m_sHeaderList.Add(szHeaderList[nIndex]);
		m_nHeaderSize.Add(nSize[nIndex]);
		m_nHeaderType.Add(nType[nIndex]);
		if (bContinue && nAlign && nAlign[nIndex] != -1) {
			m_nAlign.Add(nAlign[nIndex]);
		}
		else {
			bContinue = false;
		}
		++nIndex;
	}
}
void CCustomListCtrl::SetHeaderInfo(CStringArray& header, CUIntArray& size, CUIntArray& type, CUIntArray& align)
{
	m_sHeaderList.RemoveAll();
	m_nHeaderSize.RemoveAll();
	m_nHeaderType.RemoveAll();
	m_nAlign.RemoveAll();

	m_sHeaderList.Copy(header);
	m_nHeaderSize.Copy(size);
	m_nHeaderType.Copy(type);
	m_nAlign.Copy(align);
}

bool CCustomListCtrl::SetItemText(int nItem, int nSelColumn, CString sText)
{
	if (m_pList) {
		return m_pList->SetItemText(nItem, nSelColumn, sText);
	}
	return false;
}

bool CCustomListCtrl::GetItemText(int nItem, int nCol, CString & sText)
{
	if (m_pList) {
		return m_pList->GetItemText(nItem, nCol, sText);
	}
	return false;
}

void CCustomListCtrl::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	
}


// CCustomListCtrl 진단입니다.

#ifdef _DEBUG
void CCustomListCtrl::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CCustomListCtrl::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

// CCustomListCtrl 메시지 처리기입니다.

void CCustomListCtrl::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	/*if (m_pPopupListDlg) {
		m_pPopupListDlg->ClosePopup();
	}*/
}

BOOL CCustomListCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return 0;// CScrollView::OnEraseBkgnd(pDC);
}

void CCustomListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CScrollView::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	if (m_pList) {
		m_pList->Invalidate();
	}
	if (m_pHeader) {
		m_pHeader->Invalidate();
	}
}

void CCustomListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if (m_pList) {
		CRect rect;
		GetClientRect(&rect);
		rect.top += m_nHeaderHeight;
		m_pList->MoveWindow(rect);
		m_pList->ShowWindow(SW_SHOW);
	}
}

void CCustomListCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnTimer(nIDEvent);
}

LRESULT CCustomListCtrl::OnHeaderNotify(WPARAM wParam, LPARAM lParam)
{
	if (!m_pHeader) {
		return 0;
	}
	int nWParam = (int)wParam;
	int nLParam = (int)lParam;
	switch (nWParam)
	{
	case HEADER_RESIZE:
		Redisplay();
		break;
	case HEADER_DBCLICK:
		break;
	case HEADER_CLICK:
		break;
	}

	return 0;
}

LRESULT CCustomListCtrl::OnListNotify(WPARAM wParam, LPARAM lParam)
{
	if (!m_pList) {
		return 0;
	}
	int nWParam = (int)wParam;
	int nLParam = (int)lParam;

	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_HORZ, &si);
	GetScrollInfo(SB_VERT, &si);
	int nPos = si.nTrackPos;

	CRect rect;
	m_pList->GetClientRect(&rect);

	switch (nWParam)
	{
	case 0: // up
		nPos -= m_pList->GetRowHeight();
		if (nPos < m_pList->GetRowHeight()) {
			nPos = 0;
		}
		SetScrollPos(SB_VERT, nPos);

		if (m_pList) {
			m_pList->SetScrollPosition(nPos);
		}
		break;
	case 1: // down
		nPos += m_pList->GetRowHeight();
		if (nPos > m_pList->GetListHeight() + m_nHeaderHeight) {
			nPos = m_pList->GetListHeight() + m_nHeaderHeight;
		}
		SetScrollPos(SB_VERT, nPos);

		if (m_pList) {
			m_pList->SetScrollPosition(nPos);
		}
		break;
	case 2: // scroll
		nPos = nLParam + m_nHeaderHeight;
		if (nPos > m_pList->GetListHeight() + m_nHeaderHeight + m_pList->GetRowHeight() -rect.Height()) {
			nPos = m_pList->GetListHeight() + m_nHeaderHeight + m_pList->GetRowHeight() - rect.Height();
		}
		SetScrollPos(SB_VERT, nPos - m_nHeaderHeight);
		if (m_pList) {
			m_pList->SetScrollPosition(nPos - m_nHeaderHeight);
		}
		break;
	default:
		break;
	}

	return 0;
}

LRESULT CCustomListCtrl::OnListControl(WPARAM wParam, LPARAM lParam)
{
	GetParent()->PostMessage(LIST_CONTROL, wParam, lParam);

	//wParam: 1: delete, 2: insert up, 3: insert down, 4: circuit no move

	return 0;
}

LRESULT CCustomListCtrl::OnListHorizon(WPARAM wParam, LPARAM lParam)
{
	int nListPos1 = (int)wParam;
	int nListPos2 = (int)lParam;
	CRect rect;
	GetClientRect(&rect);

	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_HORZ, &si);
	int nPos = si.nTrackPos;
	int nWidth = rect.Width();
	if (nPos > nListPos1){
		SetScrollPos(SB_HORZ, nListPos1);
	}
	else if (nPos + nWidth < nListPos2) {
		SetScrollPos(SB_HORZ, nListPos1);
	}
	Invalidate();
	return 0;
}

LRESULT CCustomListCtrl::OnSelectionNotify(WPARAM wParam, LPARAM lParam)
{
	int nSelectItem = (int)wParam;
	int nSelCol = (int)lParam;
	CString sText;

	int nValue = m_pList->FindPosition(1);
	if (nValue != 0) {
		OnListNotify(2, (nValue == -1) ? 0 : nValue);
	}
	CRect rect;
	bool bResult = m_pList->GetSelectItemRect(rect);
	if (rect.Width() == 0 || rect.Height() == 0) {
		return 0;
	}

	int nType = m_nHeaderType.GetAt(nSelCol);
	switch (nType)
	{
	case POPUP_TYPE_NONE:
		break;
	case POPUP_TYPE_SEQ:
		break;
	case POPUP_TYPE_TEXT:
		SAFE_DELETE(m_pPopupTextDlg);
		m_pPopupTextDlg = new CPopupTextDlg(this);
		m_pPopupTextDlg->Create(IDD_TEXT_POPUP_DIALOG, this);
		m_pList->GetItemText(nSelectItem, nSelCol, sText);
		if (m_nLimit.GetCount() > nSelCol) {
			m_pPopupTextDlg->SetTextLimit(m_nLimit.GetAt(nSelCol));
		}
		m_pPopupTextDlg->SetItemIndexInfo(nSelectItem, nSelCol);
		m_pPopupTextDlg->SetInputText(sText);
		m_pPopupTextDlg->MoveWindow(rect.left, rect.bottom, m_nTextPopupWidth, m_nTextPopupHeight);
		{
			CRect rt;
			CCommonFunc::GetMonitorInfoEx(rt);
			if (rect.bottom + m_nTextPopupHeight > rt.bottom) {
				m_pPopupTextDlg->MoveWindow(rect.left, rect.top - m_nTextPopupHeight, m_nTextPopupWidth, m_nTextPopupHeight);
				if (rect.right + m_nTextPopupWidth > rt.right) {
					m_pPopupTextDlg->MoveWindow(rect.left - m_nTextPopupWidth, rect.top - m_nTextPopupHeight, m_nTextPopupWidth, m_nTextPopupHeight);
				}
			}
			else if (rect.right + m_nTextPopupWidth > rt.right) {
				m_pPopupTextDlg->MoveWindow(rect.left - m_nTextPopupWidth, rect.top, m_nTextPopupWidth, m_nTextPopupHeight);
			}
			else {
				m_pPopupTextDlg->MoveWindow(rect.left, rect.bottom, m_nTextPopupWidth, m_nTextPopupHeight);
			}
		}
		m_pPopupTextDlg->ShowWindow(SW_SHOW);
		break;
	case POPUP_TYPE_LIST:
	case POPUP_TYPE_LIST_ADD:
	case POPUP_TYPE_LIST_NOINPUT:
		SAFE_DELETE(m_pPopupListDlg);
		m_pPopupListDlg = new CListPopupDlg(this);
		m_pPopupListDlg->Create(IDD_LIST_POPUP_DIALOG, this);
		m_pList->GetItemText(nSelectItem, nSelCol, sText);
		if (m_nLimit.GetCount() > nSelCol) {
			m_pPopupListDlg->SetTextLimit(m_nLimit.GetAt(nSelCol));
		}
		m_pPopupListDlg->SetItemIndexInfo(nSelectItem, nSelCol);
		//m_pPopupListDlg->SetInputEditText(sText);
		{
			POSITION pos = m_popupList.GetHeadPosition();
			while (pos) {
				pPopupList pList = m_popupList.GetNext(pos);
				if (pList->nColumn == nSelCol) {
					for (int nIndex = 0; nIndex < pList->arrayList.GetCount(); nIndex++) {
						m_pPopupListDlg->AddPopupItem(pList->arrayList.GetAt(nIndex));
					}
					break;
				}
			}
			CRect rt;
			CCommonFunc::GetMonitorInfoEx(rt);
			if (rect.bottom + m_nListPopupHeight > rt.bottom) {
				m_pPopupListDlg->MoveWindow(rect.left, rect.top - m_nListPopupHeight, m_nListPopupWidth, m_nListPopupHeight);
				if (rect.right + m_nListPopupWidth > rt.right) {
					m_pPopupListDlg->MoveWindow(rect.left - m_nListPopupWidth, rect.top - m_nListPopupHeight, m_nListPopupWidth, m_nListPopupHeight);
				}
			}
			else if (rect.right + m_nListPopupWidth > rt.right) {
				m_pPopupListDlg->MoveWindow(rect.left - m_nListPopupWidth, rect.top, m_nListPopupWidth, m_nListPopupHeight);
			}
			else {
				m_pPopupListDlg->MoveWindow(rect.left, rect.bottom, m_nListPopupWidth, m_nListPopupHeight);
			}
		}
		
		m_pPopupListDlg->ShowWindow(SW_SHOW);
		break;
	default: break;
	}

	return 0;
}

LRESULT CCustomListCtrl::OnListPageKey(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;
	CRect rect;
	m_pList->GetClientRect(&rect);

	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	int nPos = si.nTrackPos;

	switch (nType) {
	case 0: // page up
		nPos -= rect.Height();
		if (nPos < 0) {
			nPos = 0;
		}
		SetScrollPos(SB_VERT, nPos);
		if (m_pList) {
			m_pList->SetScrollPosition(nPos);
		}
		break;
	case 1: // page down
		nPos += rect.Height();
		if (nPos > m_pList->GetListHeight() + m_nHeaderHeight + m_pList->GetRowHeight() - rect.Height()) {
			nPos = m_pList->GetListHeight() + m_nHeaderHeight + m_pList->GetRowHeight() - rect.Height();
			SetScrollPos(SB_VERT, nPos - m_nHeaderHeight);
			if (m_pList) {
				m_pList->SetScrollPosition(nPos - m_nHeaderHeight);
			}
		}
		else {
			SetScrollPos(SB_VERT, nPos);
			if (m_pList) {
				m_pList->SetScrollPosition(nPos);
			}
		}
		break;
	case 2: // top
		nPos = 0;
		SetScrollPos(SB_VERT, nPos);
		if (m_pList) {
			m_pList->SetScrollPosition(nPos);
		}
		m_pList->Invalidate();
		break;
	case 3: // bottom
		nPos = m_pList->GetListHeight() + m_nHeaderHeight + m_pList->GetRowHeight() - rect.Height();
		SetScrollPos(SB_VERT, nPos - m_nHeaderHeight);
		if (m_pList) {
			m_pList->SetScrollPosition(nPos - m_nHeaderHeight);
		}
		m_pList->Invalidate();
		break;
	default:
		break;
	}

	return 0;
}

LRESULT CCustomListCtrl::OnInputTextInfo(WPARAM wParam, LPARAM lParam)
{
	int nItemIndex = (int)wParam;
	int nSelCol = (int)lParam;

	CString sText;
	if (m_pPopupTextDlg) {
		sText = m_pPopupTextDlg->GetInputText();
	}
	if (m_pList) {
		m_pList->SetItemText(nItemIndex, nSelCol, sText);
	}
	return 0;
}

LRESULT CCustomListCtrl::OnInputListInfo(WPARAM wParam, LPARAM lParam)
{
	int nItemIndex = (int)wParam;
	int nSelCol = (int)lParam;

	int nType = m_nHeaderType.GetAt(nSelCol);
	CString sText;
	if (m_pPopupListDlg) {
		sText = m_pPopupListDlg->GetInputText();
		if (nType == POPUP_TYPE_LIST_ADD) {
			POSITION pos = m_popupList.GetHeadPosition();
			bool bIsPopup = false;
			while (pos) {
				pPopupList pList = m_popupList.GetNext(pos);
				if (pList->nColumn == nSelCol) {
					bool bFind = false;
					bIsPopup = true;
					for (int i = 0; i < pList->arrayList.GetCount(); i++) {
						if (pList->arrayList.GetAt(i) == sText) {
							bFind = true;
							break;
						}
					}
					if (!bFind) {
						pList->arrayList.InsertAt(0, sText);
					}
					break;
				}
			}
			if (!bIsPopup) {
				pPopupList pList = new popupList;
				pList->nColumn = nSelCol;
				pList->arrayList.Add(sText);
				m_popupList.AddHead(pList);
			}
		}
		else if (nType == POPUP_TYPE_LIST_NOINPUT) {
			sText = m_pPopupListDlg->GetSelectText();
		}
	}
	if (m_pList) {
		m_pList->SetItemText(nItemIndex, nSelCol, sText);
		GetParent()->PostMessage(LIST_POPUP_SEL, nItemIndex, nSelCol);
	}
	return 0;
}

void CCustomListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ((nSBCode == 4 || nSBCode == 5) && m_pHeader) {
		m_pHeader->SetCurrentHPosition(nPos);
	}

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCustomListCtrl::Redisplay()
{
	if (!m_pHeader || !m_pList) {
		return;
	}
	CSize sizeTotal;
	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);

	int nSize = m_pHeader->GetHeaderWidth();
	sizeTotal.cy = 0;
	if (m_pList) {
		sizeTotal.cy = m_pList->GetListHeight() + m_nHeaderHeight;
	}
	sizeTotal.cx = nSize;
	m_nListWidth = nSize;
	if (!m_bShowScrollbar) {
		::ShowScrollBar(m_hWnd, SB_BOTH, FALSE);
	}
	else {
		SetScrollSizes(MM_TEXT, sizeTotal);
	}

	int nPos = GetScrollPos(SB_HORZ);
	m_pHeader->SetCurrentHPosition(nPos);

	CRect rect;
	GetClientRect(&rect);
	int nWidth = rect.Width();
	m_pHeader->GetClientRect(&rect);
	rect.left -= nPos;
	rect.right = (nWidth > nSize) ? nWidth : nSize;
	rect.right += nPos;
	m_pHeader->MoveWindow(rect);

	GetClientRect(&rect);
	rect.left -= nPos;
	rect.top = m_nHeaderHeight;
	rect.right = rect.left + (nWidth > nSize) ? nWidth : nSize + nWidth + nPos;
	m_pList->MoveWindow(rect);

	CUIntArray arrayList;
	m_pHeader->GetHeaderSize(arrayList);
	m_pList->SetSizeList(arrayList);
}

void CCustomListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	LockWindowUpdate();

	switch (nSBCode) {
	case SB_LINEUP:
	case SB_LINEDOWN:
	case SB_PAGEUP:
	case SB_PAGEDOWN:
	{
		SCROLLINFO si = { 0, };
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL;
		GetScrollInfo(SB_VERT, &si);
		if (m_pList) {
			m_pList->SetScrollPosition(si.nPos);
		}
	}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		if (m_pList) {
			CRect rect;
			m_pList->GetClientRect(&rect);
			if (nPos == m_pList->GetListHeight() - rect.Height()) {
				nPos = m_pList->GetListHeight() + m_pList->GetRowHeight() - rect.Height();
			}
			m_pList->SetScrollPosition(nPos);
		}
		break;
	}
	UnlockWindowUpdate();

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CCustomListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SAFE_DELETE(m_pPopupTextDlg);
	SAFE_DELETE(m_pPopupListDlg);

	LockWindowUpdate();

	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);

	CRect rect;
	m_pList->GetClientRect(&rect);

	int nSign = zDelta / abs(zDelta);
	int nPos = si.nTrackPos + (-nSign * 120);
	zDelta = nSign * 120;

	if (nPos > m_pList->GetListHeight() + m_nHeaderHeight - rect.Height()) {
		nPos = m_pList->GetListHeight() + m_pList->GetRowHeight() - rect.Height();
	}

	SetScrollPos(SB_VERT, nPos, false);

	if (m_pList) {
		m_pList->SetScrollPosition(nPos);
	}
	UnlockWindowUpdate();

	return false;
	//return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CCustomListCtrl::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseHWheel(nFlags, zDelta, pt);
}

void CCustomListCtrl::SetHeaderHeight(int nHeight)
{
	m_nHeaderHeight = nHeight;
}

int CCustomListCtrl::GetHeaderHeight()
{
	return m_nHeaderHeight;
}

void CCustomListCtrl::SetRowHeight(int nHeight)
{
	m_nRowHeight = nHeight;
	if (m_pList) {
		m_pList->SetRowHeight(nHeight);
	}
}

int CCustomListCtrl::GetRowHeight()
{
	return m_nRowHeight;
}

void CCustomListCtrl::SetHeaderBkColor(COLORREF color, COLORREF lineColor)
{
	m_headerBk = color;
	m_headerLine = lineColor;
	if (m_pHeader) {
		m_pHeader->SetHeaderColor(m_headerBk, m_headerLine, m_headerText);
	}
}

void CCustomListCtrl::SetHeaderTextColor(COLORREF color)
{
	m_headerText = color;
	if (m_pHeader) {
		m_pHeader->SetHeaderColor(m_headerBk, m_headerLine, m_headerText);
	}
}

void CCustomListCtrl::SetRowBkColor(COLORREF normal, COLORREF select, COLORREF line)
{
	m_rowBkNormal = normal;
	m_rowBkSelect = select;
	m_rowLine = line;

	if (m_pList) {
		m_pList->SetRowColor(m_rowBkNormal, m_rowBkSelect, m_rowTextNormal, m_rowTextSelect, m_rowLine);
	}
}

void CCustomListCtrl::SetRowTextColor(COLORREF normal, COLORREF select)
{
	m_rowTextNormal = normal;
	m_rowTextSelect = select;

	if (m_pList) {
		m_pList->SetRowColor(m_rowBkNormal, m_rowBkSelect, m_rowTextNormal, m_rowTextSelect, m_rowLine);
	}
}

void CCustomListCtrl::SetTextPopupSize(int nWidth, int nHeight)
{
	m_nTextPopupWidth = nWidth;
	m_nTextPopupHeight = nHeight;
}

void CCustomListCtrl::SetListPopupSize(int nWidth, int nHeight)
{
	m_nListPopupWidth = nWidth;
	m_nListPopupHeight = nHeight;
}

void CCustomListCtrl::SetAutoHeaderSize(bool bAuto)
{
	m_bHeaderAutoSize = bAuto;
}

void CCustomListCtrl::SetDisableMenu(bool bDisable)
{
	m_bDisable = bDisable;
	if (m_pList) {
		m_pList->SetDisableMenu(bDisable);
	}
}

void CCustomListCtrl::SetColumnLimit(int* colLimit)
{
	m_nLimit.RemoveAll();

	int nIndex = 0;
	while (colLimit[nIndex] != -1) {
		m_nLimit.Add(colLimit[nIndex++]);
	}
}

void CCustomListCtrl::SetColumnLimit(CUIntArray& limit)
{
	m_nLimit.RemoveAll();
	m_nLimit.Copy(limit);
}

void CCustomListCtrl::AddPopupListItem(int nColumnIndex, CString sText)
{
	pPopupList pList = NULL;
	POSITION pos = m_popupList.GetHeadPosition();
	while (pos) {
		pList = m_popupList.GetNext(pos);
		if (pList->nColumn == nColumnIndex) {
			break;
		}
		pList = NULL;
	}
	if (pList) {
		pList->arrayList.Add(sText);
	}
	else {
		pList = new popupList;
		pList->nColumn = nColumnIndex;
		pList->arrayList.Add(sText);
		m_popupList.AddHead(pList);
	}
}

void CCustomListCtrl::RemovePopupListItem(int nColumnIndex)
{
	pPopupList pList = NULL;
	POSITION pos = m_popupList.GetHeadPosition();
	while (pos) {
		pList = m_popupList.GetNext(pos);
		if (pList->nColumn == nColumnIndex) {
			break;
		}
		pList = NULL;
	}
	if (pList) {
		pList->arrayList.RemoveAll();
	}
}

void CCustomListCtrl::AddListItemState(int nColumnIndex, CString sText)
{
	POSITION pos = m_stateString.GetHeadPosition();
	pStateString pState;
	while (pos) {
		pState = m_stateString.GetNext(pos);
		if (nColumnIndex == pState->nColumn) {
			pState->value.Add(sText);
			return;
		}
	}
	if (pState = new stateString) {
		pState->nColumn = nColumnIndex;
		pState->value.Add(sText);

		m_stateString.AddTail(pState);
	}
}

bool CCustomListCtrl::SetItemData(int nItemIndex, LPVOID lpData)
{
	if (m_pList) {
		return m_pList->SetItemData(nItemIndex, lpData);
	}
	return false;
}

LPVOID CCustomListCtrl::GetItemData(int nItemIndex)
{
	if (m_pList) {
		return m_pList->GetItemData(nItemIndex);
	}
	return NULL;
}

int CCustomListCtrl::GetItemCount()
{
	int nCount = 0;
	if (m_pList) {
		nCount = m_pList->GetItemCount();
	}
	return nCount;
}

void CCustomListCtrl::ShowScrollBar(bool bShow)
{
	m_bShowScrollbar = bShow;
}

void CCustomListCtrl::EnableLastInsertRow(bool bLast, bool bFirst/* = false*/)
{
	m_bLastInsertRow = bLast;
	m_bFirstInsertRow = bFirst;
	if (m_pList) {
		m_pList->EnableLastInsertRow(m_bLastInsertRow);
	}
}

void CCustomListCtrl::UseTabKey(bool bUse)
{
	if (m_bUseTabkey == bUse) {
		return;
	}
	m_bUseTabkey = bUse;
	if (m_pList) {
		m_pList->UseTabKey(m_bUseTabkey);
	}
}

void CCustomListCtrl::FixHeaderSize(bool bFix)
{
	m_bFixHeaderSize = bFix;
	if (m_pHeader) {
		m_pHeader->FixHeaderSize(m_bFixHeaderSize);
	}
}

void CCustomListCtrl::SetHeaderFontSize(int nSize)
{
	if (m_nHeaderFontSize == nSize) {
		return;
	}
	m_nHeaderFontSize = nSize;
	if (m_pHeader) {
		m_pHeader->SetFontSize(nSize);
	}
}

void CCustomListCtrl::SetRowFontSize(int nSize)
{
	if (m_nFontSize == nSize) {
		return;
	}
	m_nFontSize = nSize;
	if (m_pList) {
		return m_pList->SetFontSize(nSize);
	}
}

void CCustomListCtrl::SetRowFontBold(UINT nBold)
{
	if (m_nFontBold == nBold) {
		return;
	}
	m_nFontBold = nBold;
	if (m_pList) {
		return m_pList->SetFontBold(nBold);
	}
}

void CCustomListCtrl::SetHeaderFontBold(UINT nBold)
{
	if (m_nHeaderFontBold == nBold) {
		return;
	}
	m_nHeaderFontBold = nBold;
	if (m_pHeader) {
		m_pHeader->SetFontBold(nBold);
	}
}

void CCustomListCtrl::SetRowFontName(CString sName)
{
	if (m_sRowFontName.Compare(sName) == 0) {
		return;
	}
	m_sRowFontName = sName;
	if (m_pList) {
		return m_pList->SetFontName(sName);
	}
}

void CCustomListCtrl::SetHeaderFontName(CString sName)
{
	if (m_sHeaderFontName.Compare(sName) == 0) {
		return;
	}
	m_sHeaderFontName = sName;
	if (m_pHeader) {
		m_pHeader->SetFontName(sName);
	}
}

void CCustomListCtrl::DelListItem(int nIndex)
{
	if (m_pList) {
		m_pList->DeleteItem(nIndex);
	}
}

void CCustomListCtrl::DeleteAllItems()
{
	if (m_pList) {
		m_pList->DeleteAllItems();
	}
}

void CCustomListCtrl::InsertListItemUp(int nRowIndex)
{
	if (m_pList) {
		m_pList->InsertItem(nRowIndex, 0);
	}
}

void CCustomListCtrl::InsertListItemDown(int nRowIndex)
{
	if (m_pList) {
		m_pList->InsertItem(nRowIndex, 1);
	}
}

//20221013 GBM start - ReadOnly 주기 위한 추가
void CCustomListCtrl::SetReadOnly(bool bReadOnly)
{
	m_pList->SetReadOnly(bReadOnly);
}
//20221013 GBM end
