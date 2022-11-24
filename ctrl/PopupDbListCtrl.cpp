// PopupDbListCtrl.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PopupDbListCtrl.h"
#include "SLP30.h"


int g_nObjectDbItemTerm = 1;
int g_nObjectDbItemHeight = 25;

// CPopupDbListCtrl

IMPLEMENT_DYNCREATE(CPopupDbListCtrl, CScrollView)

CPopupDbListCtrl::CPopupDbListCtrl()
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
		_T("����")); // lpszFacename 

	m_nShowIndex = 0;
	m_nPos = -1;
	m_nScrollPos = -1;
	m_Range = 100;

	m_pSlider = NULL;
	m_pItemManager = NULL;
}

CPopupDbListCtrl::~CPopupDbListCtrl()
{
	ReleaseWindow();
}


BEGIN_MESSAGE_MAP(CPopupDbListCtrl, CScrollView)
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


// CPopupDbListCtrl �׸����Դϴ�.

void CPopupDbListCtrl::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	ShowScrollBar(SB_BOTH, FALSE);

	m_pSlider = new CPosVSliderDlg(this);
	m_pSlider->Create(IDD_COMMON_CHILD_DIALOG, this);

	m_pItemManager = new CItemManager();

	SetTimer(1, 10, NULL);
}

void CPopupDbListCtrl::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CPopupDbListCtrl �����Դϴ�.

#ifdef _DEBUG
void CPopupDbListCtrl::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPopupDbListCtrl::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPopupDbListCtrl �޽��� ó�����Դϴ�.


BOOL CPopupDbListCtrl::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	ReleaseWindow();

	return CScrollView::DestroyWindow();
}


void CPopupDbListCtrl::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	ReleaseWindow();
}

void CPopupDbListCtrl::ReleaseWindow()
{
	while (!m_listObject.IsEmpty())
	{
		CListItem * pItem = (CListItem*)m_listObject.RemoveHead();
		SAFE_DELETE(pItem);
	}

	SAFE_DELETE(m_pSlider);

	SAFE_DELETE(m_pItemManager);
}


HBRUSH CPopupDbListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CScrollView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

void CPopupDbListCtrl::ControlSlider(int nSign)
{
	CRect rect;
	GetClientRect(&rect);

	if (m_displayList.GetCount() * (g_nObjectDbItemTerm + g_nObjectDbItemHeight) < rect.Height()) {
		return;
	}

	int nTotalHeight = GetScrollHeight();

	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	int nPos = si.nTrackPos + (-nSign * (g_nObjectDbItemHeight + g_nObjectDbItemTerm));
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

		m_pSlider->SetValue(m_Range, rect.Height(), m_nPos);
		Invalidate();
	}
}

BOOL CPopupDbListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_UP) {
			int nSelIndex = GetSelectedItemSeq();
			for (int nIndex = 0; nIndex < m_displayList.GetCount(); nIndex++) {
				if (m_displayList.GetAt(nIndex) == nSelIndex && nIndex != 0)
				{
					if (nIndex <= 4) {
						int nSelValue = m_displayList.GetAt(nIndex);
						ControlSlider(1);
						for (int i = 0; i < m_displayList.GetCount(); i++) {
							if (m_displayList.GetAt(i) == nSelValue && i > 1) {
								PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, m_displayList.GetAt(i - 2));
								break;
							}
						}
					}
					else {
						PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, m_displayList.GetAt(nIndex - 2));
					}
					break;
				}
			}
		}
		else if (pMsg->wParam == VK_LEFT) {
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
						if (m_displayList.GetCount() - 4 <= nIndex) {
							int nSelValue = m_displayList.GetAt(nIndex);
							ControlSlider(-1);
							for (int i = 0; i < m_displayList.GetCount(); i++) {
								if (m_displayList.GetAt(i) == nSelValue && i < m_displayList.GetCount() - 2) {
									PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, m_displayList.GetAt(i + 2));
									break;
								}
							}
							break;
						}
						PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, 0, m_displayList.GetAt(nIndex + 2));
						break;
					}
				}
			}
		}
		else if (pMsg->wParam == VK_RIGHT) {
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

void CPopupDbListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CRect rect;
	GetClientRect(&rect);

	int nHeight = rect.Height();
	int nItemCount = ((nHeight / g_nObjectDbItemHeight) * 3) + 2;

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

void CPopupDbListCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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


void CPopupDbListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	LockWindowUpdate();
	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	nPos = si.nTrackPos;
	UnlockWindowUpdate();

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CPopupDbListCtrl::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CPopupDbListCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	/*CRect rect;
	GetClientRect(&rect);
	CCommonDisplay::DrawRect(pDC, true, RGB(140, 140, 140), RGB(140, 140, 140), rect);*/

	return  CScrollView::OnEraseBkgnd(pDC);
}


void CPopupDbListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CScrollView::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();
}


BOOL CPopupDbListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	LockWindowUpdate();

	CRect rect;
	GetClientRect(&rect);

	if (m_displayList.GetCount() * (g_nObjectDbItemTerm + g_nObjectDbItemHeight) < rect.Height()) {
		return CScrollView::OnMouseWheel(nFlags, 0, pt);
	}

	int nTotalHeight = GetScrollHeight();
	int nSign = zDelta / abs(zDelta);

	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	int nPos = si.nTrackPos + (-nSign * 50);// (g_nObjectDbItemHeight + g_nObjectDbItemTerm));
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

LRESULT CPopupDbListCtrl::OnLObjectClick(WPARAM wParam, LPARAM lParam)
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

LRESULT CPopupDbListCtrl::OnRObjectClick(WPARAM wParam, LPARAM lParam)
{
	int nItemIndex = (int)wParam;
	//GetParent()->PostMessageW(WM_RBUTTON_BUTTONLIST_CLICK, wParam, lParam);

	return 0;
}

LRESULT CPopupDbListCtrl::OnSliderInfo(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;
	int nValue = (int)lParam;

	CRect rect;
	GetClientRect(&rect);

	int nLeafCount = 0;

	int nTotalHeight = GetScrollHeight();

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

void CPopupDbListCtrl::SetScrollSize()
{
	int nTotalHeight = GetScrollHeight();
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

void CPopupDbListCtrl::RedisplayObject(INT nIndex)
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

void CPopupDbListCtrl::SetSearchWord(CString sSearch)
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

void CPopupDbListCtrl::SetSelectedItem(int nValue)
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

int CPopupDbListCtrl::GetSelectedItemSeq()
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

int CPopupDbListCtrl::GetSelectedItemIndex()
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

void CPopupDbListCtrl::AddItem(int nSeq, CString sText, LPVOID lpData/* = NULL*/)
{
	if (m_pItemManager) {
		m_pItemManager->AddItem(nSeq, sText, lpData);
	}
}

bool CPopupDbListCtrl::InsertItem(int nIndex, int nSeq, CString sText, LPVOID lpData/* = NULL*/)
{
	if (m_pItemManager) {
		return m_pItemManager->InsertItem(nIndex, nSeq, sText, lpData);
	}
	return false;
}

bool CPopupDbListCtrl::RemoveItem(int nIndex)
{
	if (m_pItemManager) {
		return m_pItemManager->RemoveItem(nIndex);
	}
	return false;
}

int  CPopupDbListCtrl::GetCount()
{
	if (m_pItemManager) {
		return m_pItemManager->GetCount();
	}
	return 0;
}

pListItem CPopupDbListCtrl::GetItemInfo(int nIndex)
{
	if (m_pItemManager) {
		return m_pItemManager->GetItemInfo(nIndex);
	}
	return NULL;
}

int CPopupDbListCtrl::GetScrollHeight()
{
	CString sSearch = m_sSearch;
	int nTotalHeight = 0;
	sSearch.Trim(L" ");

	int nCount = 0;
	int nItemCount = m_pItemManager->GetCount();
	if (sSearch.GetLength() == 0) {
		nTotalHeight = (nItemCount / 2 + nItemCount % 2) * (g_nObjectDbItemTerm + g_nObjectDbItemHeight) - g_nObjectDbItemTerm;
	}
	else {
		m_displaySync.Enter();
		pListItem pValue;
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
		m_displaySync.Leave();
		nTotalHeight = (nCount / 2 + nCount % 2) * (g_nObjectDbItemTerm + g_nObjectDbItemHeight) - g_nObjectDbItemTerm;
	}
	return nTotalHeight;
}

void CPopupDbListCtrl::Redisplay()
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
		int nTotalHeight = GetScrollHeight();
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

	int nStartIndex = (m_nPos / (g_nObjectDbItemHeight + g_nObjectDbItemTerm)) / 2;
	nStartIndex -= (m_nPos % (g_nObjectDbItemHeight + g_nObjectDbItemTerm) > 0) ? 1 : 0;

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
			nPos += g_nObjectDbItemHeight + g_nObjectDbItemTerm;
			continue;
		}
		if (nIndex < nStartIndex) {
			nPos += g_nObjectDbItemHeight + g_nObjectDbItemTerm;
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
		nLineH = nPos - m_nPos + g_nObjectDbItemHeight;
		CCommonDisplay::DrawLine(_pDC, 0, nLineH, rect.Width() - nTerm, nLineH,	PS_SOLID, 1, RGB(140, 140, 140));

		int nX = (((rect.Width() - nTerm) / 2) * (nIndex % 2) > 0) ? ((rect.Width() - nTerm) / 2) * (nIndex % 2) + 1 : 0;
		pItem->MoveWindow(nX, nPos - m_nPos, (rect.Width() - nTerm) / 2, g_nObjectDbItemHeight);
		pItem->ShowWindow(SW_SHOW);
		pItem->Invalidate();
		++nItemIndex;

		nPos += (g_nObjectDbItemHeight + g_nObjectDbItemTerm) * (nIndex % 2);
		if (nIndex % 2 == 0 && nIndex + 1 == nItemCount) {
			nPos += g_nObjectDbItemHeight + g_nObjectDbItemTerm;
		}
	}
	int nTerm = 13;
	if (rect.Height() >= m_Range) {
		nTerm = 0;
	}
	CCommonDisplay::DrawLine(_pDC, ((rect.Width() - nTerm) / 2), 0, ((rect.Width() - nTerm) / 2), rect.Height(), PS_SOLID, 1, RGB(140, 140, 140));
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

