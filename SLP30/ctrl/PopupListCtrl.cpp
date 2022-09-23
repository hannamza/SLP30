// PopupListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PopupListCtrl.h"
#include "SLP30.h"


int g_nObjectItemTerm = 1;
int g_nObjectItemHeight = 25;

// CPopupListCtrl

IMPLEMENT_DYNCREATE(CPopupListCtrl, CScrollView)

CPopupListCtrl::CPopupListCtrl()
{
	m_nMapMode = MM_TEXT;

	m_nScrollX = 0;
	m_nScrollY = 0;

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
		_T("돋움")); // lpszFacename 

	m_nShowIndex = 0;
	m_nPos = -1;
	m_nScrollPos = -1;
	m_Range = 100;

	m_pSlider = NULL;
	m_pItemManager = NULL;
}

CPopupListCtrl::~CPopupListCtrl()
{
	ReleaseWindow();
}


BEGIN_MESSAGE_MAP(CPopupListCtrl, CScrollView)
	ON_MESSAGE(WM_LBUTTON_BUTTONLIST_CLICK, OnLObjectClick)
	ON_MESSAGE(WM_RBUTTON_BUTTONLIST_CLICK, OnRObjectClick)
	ON_MESSAGE(SLIDER_INFO, OnSliderInfo)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEHWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPopupListCtrl 그리기입니다.

void CPopupListCtrl::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	ShowScrollBar(SB_BOTH, FALSE);

	m_pSlider = new CPosVSliderDlg(this);
	m_pSlider->Create(IDD_COMMON_CHILD_DIALOG, this);

	m_pItemManager = new CItemManager();

	SetTimer(1, 10, NULL);
}

void CPopupListCtrl::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CPopupListCtrl 진단입니다.

#ifdef _DEBUG
void CPopupListCtrl::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPopupListCtrl::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPopupListCtrl 메시지 처리기입니다.


BOOL CPopupListCtrl::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ReleaseWindow();

	return CScrollView::DestroyWindow();
}


void CPopupListCtrl::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ReleaseWindow();
}

void CPopupListCtrl::ReleaseWindow()
{
	while (!m_listObject.IsEmpty())
	{
		CListItem * pItem = (CListItem*)m_listObject.RemoveHead();
		SAFE_DELETE(pItem);
	}

	SAFE_DELETE(m_pSlider);

	SAFE_DELETE(m_pItemManager);
}


HBRUSH CPopupListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CScrollView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CPopupListCtrl::ControlSlider(int nSign)
{
	CRect rect;
	GetClientRect(&rect);

	if (m_displayList.GetCount() * (g_nObjectItemTerm + g_nObjectItemHeight) < rect.Height()) {
		return;
	}

	int nItemCount = m_pItemManager->GetCount();
	int nTotalHeight = nItemCount * (g_nObjectItemTerm + g_nObjectItemHeight) - g_nObjectItemTerm;

	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	int nPos = si.nTrackPos + (-nSign * (g_nObjectItemHeight + g_nObjectItemTerm));
	if (nTotalHeight < rect.Height() + nPos)
	{
		nPos = nTotalHeight - rect.Height();
	}
	if (nPos < 0)
	{
		nPos = 0;
	}

	SetScrollPos(SB_VERT, nPos, TRUE);

	if (m_nPos != nPos)
	{
		m_nPos = nPos;
		m_nScrollPos = nPos;
		Redisplay();

		m_pSlider->SetValue(m_Range, rect.Height(), m_nPos);
	}
}

BOOL CPopupListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_UP) {
			int nSelIndex = GetSelectedItemSeq();
			for (int nIndex = 0; nIndex < m_displayList.GetCount(); nIndex++) {
				if (m_displayList.GetAt(nIndex) == nSelIndex && nIndex != 0)
				{
					if (nIndex <= 3) {
						int nSelValue = m_displayList.GetAt(nIndex);
						ControlSlider(1);
						for (int i = 0; i < m_displayList.GetCount(); i++) {
							if (m_displayList.GetAt(i) == nSelValue && i > 0) {
								PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, m_displayList.GetAt(i - 1));
								break;
							}
						}
					}
					else {
						PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, m_displayList.GetAt(nIndex - 1));
					}
					break;
				}
			}
		}
		else if (pMsg->wParam == VK_DOWN) {
			int nSelIndex = GetSelectedItemSeq();
			int nItemCount = m_pItemManager->GetCount();
			if (nSelIndex == -1) {
				nSelIndex = m_displayList.GetAt(0);
				PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, nSelIndex);
			}
			else {
				for (int nIndex = 0; nIndex < m_displayList.GetCount(); nIndex++) {
					if (m_displayList.GetAt(nIndex) == nSelIndex && nIndex < m_displayList.GetCount()) {
						if (m_displayList.GetCount() - 3 <= nIndex) {
							int nSelValue = m_displayList.GetAt(nIndex);
							ControlSlider(-1);
							for (int i = 0; i < m_displayList.GetCount(); i++) {
								if (m_displayList.GetAt(i) == nSelValue && i != m_displayList.GetCount() - 1) {
									PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, m_displayList.GetAt(i + 1));
									break;
								}
							}
							break;
						}
						PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, m_displayList.GetAt(nIndex + 1));
						break;
					}
				}
			}
		}
	}

	return CScrollView::PreTranslateMessage(pMsg);
}

void CPopupListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rect;
	GetClientRect(&rect);

	int nHeight = rect.Height();
	int nItemCount = ((nHeight / g_nObjectItemHeight) * 2) + 2;

	for (int nIndex = nItemCount; nIndex > m_listObject.GetCount(); nIndex)
	{
		CListItem* pItem = new CListItem(this);
		pItem->Create(IDD_COMMON_CHILD_DIALOG, this);
		pItem->ShowWindow(SW_HIDE);
		m_listObject.AddTail(pItem);
	}
	if (m_pSlider)
	{
		m_pSlider->MoveWindow(rect.Width() - 13, 0, 13, rect.Height());

		if (rect.Height() >= m_Range)
		{
			m_pSlider->ShowWindow(SW_HIDE);
			if (m_nPos != 0)
			{
				SetScrollPos(SB_VERT, 0, TRUE);
				m_nPos = 0;
				m_nScrollPos = 0;
				Redisplay();
			}
		}
		else
		{
			m_pSlider->ShowWindow(SW_SHOW);
		}

		m_pSlider->SetValue(m_Range, rect.Height(), m_nPos);

		SetScrollSize();
	}

	ShowScrollBar(SB_BOTH, FALSE);
}

void CPopupListCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1) {
		KillTimer(1);

		m_displaySync.Enter();
		int nItemCount = m_listObject.GetCount();//m_pItemManager->GetCount();
		if (nItemCount <= 0) {
			m_displaySync.Leave();
			return;
		}
		CListItem* pItem;
		bool bFind = false;
		for (int nIndex = 0; nIndex < nItemCount; nIndex++)
		{
			pItem = (CListItem*)m_listObject.GetAt(m_listObject.FindIndex(nIndex));
			if (pItem->GetSelectedState()) {
				bFind = true;
			}
		}
		if (!bFind) {
			/*pItem = (CListItem*)m_listObject.GetHead();
			pItem->SetSelectedState(true);
			PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, pItem->GetItemIndex());*/

			POSITION pos = m_listObject.GetHeadPosition();
			pItem = (CListItem*)m_listObject.GetNext(pos);
			pItem->SetSelectedState(true);
			PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, pItem->GetItemIndex());
			while (pos) {
				pItem = (CListItem*)m_listObject.GetNext(pos);
				pItem->SetSelectedState(false);
			}
		}
		m_displaySync.Leave();
	}

	CScrollView::OnTimer(nIDEvent);
}


void CPopupListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	LockWindowUpdate();
	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	nPos = si.nTrackPos;
	UnlockWindowUpdate();

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CPopupListCtrl::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CPopupListCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/*CRect rect;
	GetClientRect(&rect);
	CCommonDisplay::DrawRect(pDC, true, RGB(140, 140, 140), RGB(140, 140, 140), rect);*/

	return  CScrollView::OnEraseBkgnd(pDC);
}


void CPopupListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CScrollView::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}


BOOL CPopupListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	LockWindowUpdate();

	CRect rect;
	GetClientRect(&rect);

	if (m_displayList.GetCount() * (g_nObjectItemTerm + g_nObjectItemHeight) < rect.Height()) {
		return CScrollView::OnMouseWheel(nFlags, 0, pt);
	}

	int nItemCount = m_pItemManager->GetCount();
	int nTotalHeight = nItemCount * (g_nObjectItemTerm + g_nObjectItemHeight) - g_nObjectItemTerm;
	int nSign = zDelta / abs(zDelta);

	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	int nPos = si.nTrackPos + (-nSign * 50);// (g_nObjectItemHeight + g_nObjectItemTerm));
	if (nTotalHeight < rect.Height() + nPos)
	{
		nPos = nTotalHeight - rect.Height();
	}
	if (nPos < 0)
	{
		nPos = 0;
	}

	SetScrollPos(SB_VERT, nPos, TRUE);

	if (m_nPos != nPos)
	{
		m_nPos = nPos;
		m_nScrollPos = nPos;
		Redisplay();

		m_pSlider->SetValue(m_Range, rect.Height(), m_nPos);
	}
	UnlockWindowUpdate();

	return CScrollView::OnMouseWheel(nFlags, 0, pt);
}

LRESULT CPopupListCtrl::OnLObjectClick(WPARAM wParam, LPARAM lParam)
{
	int nItemIndex = (int)lParam;
	GetParent()->PostMessageW(WM_LBUTTON_BUTTONLIST_CLICK, wParam, lParam);

	m_displaySync.Enter();
	int nItemCount = m_listObject.GetCount();//m_pItemManager->GetCount();
	CListItem* pItem;
	for (int nIndex = 0; nIndex < nItemCount; nIndex++)
	{
		pItem = (CListItem*)m_listObject.GetAt(m_listObject.FindIndex(nIndex));
		if (pItem->GetItemIndex() == nItemIndex && !pItem->GetSelectedState()) {
			pItem->SetSelectedState(true);
		}
		else if (pItem->GetItemIndex() != nItemIndex && pItem->GetSelectedState()) {
			pItem->SetSelectedState(false);
		}
	}
	m_displaySync.Leave();

	return 0;
}

LRESULT CPopupListCtrl::OnRObjectClick(WPARAM wParam, LPARAM lParam)
{
	int nItemIndex = (int)wParam;
	//GetParent()->PostMessageW(WM_RBUTTON_BUTTONLIST_CLICK, wParam, lParam);

	return 0;
}

LRESULT CPopupListCtrl::OnSliderInfo(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;
	int nValue = (int)lParam;

	CRect rect;
	GetClientRect(&rect);

	int nItemCount = m_pItemManager->GetCount();
	int nLeafCount = 0;

	int nTotalHeight = nItemCount * (g_nObjectItemTerm + g_nObjectItemHeight) - g_nObjectItemTerm;

	int nPos = m_nScrollPos + nValue;
	if (nTotalHeight < rect.Height() + nPos)
	{
		nPos = nTotalHeight - rect.Height();
	}
	if (nPos < 0)
	{
		nPos = 0;
	}

	SetScrollPos(SB_VERT, nPos, TRUE);

	if (m_nPos != nPos)
	{
		m_nPos = nPos;
		Redisplay();

		m_pSlider->SetValue(m_Range, rect.Height(), m_nPos);
	}
	if (nType == 1)
	{
		m_nScrollPos = m_nPos;
	}

	return 0;
}

//---------------------------------------------------------------------------------------------------------

void CPopupListCtrl::SetScrollSize()
{
	int nItemCount = m_pItemManager->GetCount();

	int nTotalHeight = nItemCount * (g_nObjectItemTerm + g_nObjectItemHeight) - g_nObjectItemTerm;
	if (nTotalHeight < 0) {
		nTotalHeight = 0;
	}
	if (m_Range == nTotalHeight) {
		return;
	}

	CSize sizeTotal;
	sizeTotal.cx = 0, sizeTotal.cy = nTotalHeight;
	SetScrollSizes(MM_TEXT, sizeTotal);
	m_Range = nTotalHeight;

	ShowScrollBar(SB_BOTH, FALSE);

	CRect rect;
	GetClientRect(&rect);

	if (rect.Height() >= m_Range)
	{
		m_pSlider->ShowWindow(SW_HIDE);

		if (m_nPos != 0)
		{
			SetScrollPos(SB_VERT, 0, TRUE);
			m_nPos = 0;
			m_nScrollPos = 0;
			Redisplay();
		}
	}
	else
	{
		m_pSlider->ShowWindow(SW_SHOW);
	}
	m_pSlider->SetValue(m_Range, rect.Height(), m_nPos);
}

void CPopupListCtrl::RedisplayObject(INT nIndex)
{
	m_displaySync.Enter();

	if (nIndex < 0 || nIndex >= m_pItemManager->GetCount())
	{
		m_displaySync.Leave();
		return;
	}

	CListItem* pItem = (CListItem*)m_listObject.GetAt(m_listObject.FindIndex(nIndex));
	if (pItem)
	{
		pItem->Invalidate();
	}

	m_displaySync.Leave();
}

void CPopupListCtrl::SetSearchWord(CString sSearch)
{
	m_sSearch = sSearch;
	Redisplay();
	if (m_displayList.GetCount() > 0) {
		int nSelIndex = m_displayList.GetAt(0);
		PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, nSelIndex);
	}
	else {
		PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, -1);
	}
}

void CPopupListCtrl::SetSelectedItem(int nValue)
{
	m_displaySync.Enter();
	int nItemCount = m_pItemManager->GetCount();
	CListItem* pItem;
	for (int nIndex = 0; nIndex < nItemCount; nIndex++)
	{
		pItem = (CListItem*)m_listObject.GetAt(m_listObject.FindIndex(nIndex));
		if (!pItem) {
			continue;
		}
		if (nValue == pItem->GetItemIndex()) {
			pItem->SetSelectedState(true);
		}
		else if (pItem->GetSelectedState()) {
			pItem->SetSelectedState(false);
		}
	}
	m_displaySync.Leave();
}

int CPopupListCtrl::GetSelectedItemSeq()
{
	m_displaySync.Enter();
	int nItemCount = m_listObject.GetCount();//m_pItemManager->GetCount();
	CListItem* pItem;
	for (int nIndex = 0; nIndex < nItemCount; nIndex++)
	{
		pItem = (CListItem*)m_listObject.GetAt(m_listObject.FindIndex(nIndex));
		if (pItem->GetSelectedState()) {
			m_displaySync.Leave();
			return pItem->GetItemIndex();
		}
	}
	m_displaySync.Leave();
	return -1;
}

int CPopupListCtrl::GetSelectedItemIndex()
{
	m_displaySync.Enter();
	int nItemCount = m_listObject.GetCount();// m_pItemManager->GetCount();
	CListItem* pItem;
	for (int nIndex = 0; nIndex < nItemCount; nIndex++)
	{
		pItem = (CListItem*)m_listObject.GetAt(m_listObject.FindIndex(nIndex));
		if (pItem && pItem->GetSelectedState()) {
			m_displaySync.Leave();
			return nIndex;
		}
	}
	m_displaySync.Leave();
	return -1;
}

void CPopupListCtrl::AddItem(int nSeq, CString sText, LPVOID lpData/* = NULL*/)
{
	if (m_pItemManager) {
		m_pItemManager->AddItem(nSeq, sText, lpData);
	}
}

bool CPopupListCtrl::InsertItem(int nIndex, int nSeq, CString sText, LPVOID lpData/* = NULL*/)
{
	if (m_pItemManager) {
		return m_pItemManager->InsertItem(nIndex, nSeq, sText, lpData);
	}
	return false;
}

bool CPopupListCtrl::RemoveItem(int nIndex)
{
	if (m_pItemManager) {
		return m_pItemManager->RemoveItem(nIndex);
	}
	return false;
}

int  CPopupListCtrl::GetCount()
{
	if (m_pItemManager) {
		return m_pItemManager->GetCount();
	}
	return 0;
}

pListItem CPopupListCtrl::GetItemInfo(int nIndex)
{
	if (m_pItemManager) {
		return m_pItemManager->GetItemInfo(nIndex);
	}
	return NULL;
}

void CPopupListCtrl::Redisplay()
{
	CString sSearch = m_sSearch;
	sSearch.Trim(L" ");
	if (sSearch.GetLength() == 0) {
		SetScrollSize();
	}

	m_displaySync.Enter();

	m_displayList.RemoveAll();

	CRect rect;
	GetClientRect(&rect);

	int nItemCount = m_pItemManager->GetCount();
	int nPos = 0, nLineH = 0;
	int nItemIndex = 0;
	CListItem* pItem;
	pListItem pValue;
	int nCount = 0;

	if (sSearch.GetLength() > 0) {
		for (int nIndex = 0; nIndex < nItemCount; nIndex++)
		{
			pValue = m_pItemManager->GetItemInfo(nIndex);
			CString sText = pValue->sText;
			if (sText.Find(sSearch, 0) < 0) {
				continue;
			}
			else {
				++nCount;
			}
		}
		int nTotalHeight = nCount * (g_nObjectItemTerm + g_nObjectItemHeight) - g_nObjectItemTerm;
		CSize sizeTotal;
		if (nTotalHeight < 0) {
			nTotalHeight = 0;
		}
		if (m_Range != nTotalHeight)
		{
			sizeTotal.cx = 0, sizeTotal.cy = nTotalHeight;
			SetScrollSizes(MM_TEXT, sizeTotal);
			m_Range = nTotalHeight;

			if (rect.Height() >= m_Range)
			{
				m_pSlider->ShowWindow(SW_HIDE);
				if (m_nPos != 0)
				{
					SetScrollPos(SB_VERT, 0, TRUE);
					m_nPos = 0;
					m_nScrollPos = 0;
					Redisplay();
				}
			}
			else
			{
				m_pSlider->ShowWindow(SW_SHOW);
			}
		}
	}
	ShowScrollBar(SB_BOTH, FALSE);

	nCount = 0;

	CDC* _pDC = GetDC();

	int nStartIndex = m_nPos / (g_nObjectItemHeight + g_nObjectItemTerm);
	nStartIndex -= (m_nPos % (g_nObjectItemHeight + g_nObjectItemTerm) > 0) ? 1 : 0;

	for (int nIndex = 0; nIndex < nItemCount; nIndex++)
	{
		if (sSearch.GetLength() > 0) {
			pValue = m_pItemManager->GetItemInfo(nIndex);
			CString sText = pValue->sText;
			if (sText.Find(sSearch, 0) < 0) {
				continue;
			}
		}
		++nCount;
		if (nPos > m_nPos + rect.Height()) {
			nPos += g_nObjectItemHeight + g_nObjectItemTerm;
			continue;
		}
		if (nIndex < nStartIndex) {
			nPos += g_nObjectItemHeight + g_nObjectItemTerm;
			continue;
		}

		pValue = m_pItemManager->GetItemInfo(nIndex);
		pItem = (CListItem*)m_listObject.GetAt(m_listObject.FindIndex(nItemIndex));
		pItem->SetItemIndex(pValue->nSequenceNo);
		pItem->SetListItem(pValue);

		m_displayList.Add(pValue->nSequenceNo);

		int nTerm = 13;
		if (rect.Height() >= m_Range) {
			nTerm = 0;
		}
		nLineH = nPos - m_nPos + g_nObjectItemHeight;
		CCommonDisplay::DrawLine(_pDC, 0, nLineH, rect.Width() - nTerm, nLineH,	PS_SOLID, 1, RGB(140, 140, 140));

		pItem->MoveWindow(0, nPos - m_nPos, rect.Width() - nTerm, g_nObjectItemHeight);
		pItem->ShowWindow(SW_SHOW);
		pItem->Invalidate();
		++nItemIndex;

		nPos += g_nObjectItemHeight + g_nObjectItemTerm;
	}
	rect.top = nPos;
	CCommonDisplay::DrawRect(_pDC, true, RGB(255, 255, 255), RGB(255, 255, 255), rect);

	for (int nIndex = nItemIndex; nIndex < m_listObject.GetCount(); nIndex++)
	{
		pItem = (CListItem*)m_listObject.GetAt(m_listObject.FindIndex(nIndex));
		pItem->SetItemIndex(-1);
		pItem->SetListItem(NULL);
		pItem->ShowWindow(SW_HIDE);
	}

	ReleaseDC(_pDC);

	m_pSlider->Invalidate();

	m_displaySync.Leave();
}

