// CustomList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CustomList.h"
#include "MessagePopup.h"
#include "CircuitNoMoveDlg.h"
#include "MessagePopup.h"

int g_nItemTerm = 1;
int g_nItemHeight = LIST_ITEM_HEIGHT;
int g_nItemWidth = 150;

// CCustomList
IMPLEMENT_DYNCREATE(CCustomList, CScrollView)

CCustomList::CCustomList()
{
	m_nMapMode = MM_TEXT;
	m_sizeIndex = 0;
	m_nSelectItem = -1;
	m_nSelCol = -1;
	m_nScrollPos = 0;
	m_pParent = NULL;

	m_bkColor = RGB(207, 238, 245);
	m_bkSelColor = RGB(112, 146, 190);
	m_textColor = RGB(30, 30, 30);
	m_textSelColor = RGB(255, 255, 255);
	m_lineColor = RGB(255, 255, 255);

	m_bUseTabKey = true;
	m_bDisable = false;
	m_bLastInsertRow = false;
	m_bFirstInsertRow = false;
	m_bLButtonDown = false;
	m_bRButtonDown = false;

	m_nFontSize = 15;
	m_nFontBold = FW_NORMAL;
	m_sFontName = _T("굴림");

	m_nCircuitMoveNum[0] = -1;
	m_nCircuitMoveNum[1] = -1;

	m_nChartIndex = -1;
}

CCustomList::~CCustomList()
{
	POSITION pos = m_stateString.GetHeadPosition();
	pStateString pState;
	while (pos) {
		pState = m_stateString.GetNext(pos);
		pState->value.RemoveAll();
		SAFE_DELETE(pState);
	}

	m_listSync.Enter();

	CCustomItem* pItem = NULL;
	while (!m_listItem.IsEmpty())
	{
		pItem = m_listItem.RemoveHead();
		SAFE_DELETE(pItem);
	}
	pItemData pData = NULL;
	while (!m_listData.IsEmpty()) {
		pData = m_listData.RemoveHead();
		for (int i = 0; i < pData->listText.GetCount(); i++) {
			pItemValue pValue = pData->listText.GetAt(pData->listText.FindIndex(i));
			SAFE_DELETE(pValue);
		}
		SAFE_DELETE(pData);
	}
	m_listSync.Leave();
}

BEGIN_MESSAGE_MAP(CCustomList, CScrollView)
	ON_MESSAGE(WM_LBUTTON_BUTTONLIST_CLICK, OnListLButtonClick)
	ON_MESSAGE(WM_RBUTTON_BUTTONLIST_CLICK, OnListRButtonClick)
	ON_MESSAGE(WM_MOUSE_CLICK, OnMouseClick)
	ON_MESSAGE(WM_PAGE_MOVE_KEY, OnListPageKey)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_LIST_UP, &CCustomList::OnListUp)
	ON_COMMAND(ID_LIST_DOWN, &CCustomList::OnListDown)
	ON_COMMAND(ID_LIST_DEL, &CCustomList::OnListDel)
	ON_COMMAND(ID_LIST_UPADD, &CCustomList::OnListUpadd)
	ON_COMMAND(ID_LIST_DOWNADD, &CCustomList::OnListDownadd)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_LIST_CIRCUIT_NO_CHANGE, &CCustomList::OnListCircuitNoChange)
END_MESSAGE_MAP()


// CCustomList 그리기입니다.

void CCustomList::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CCustomList::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}

// CCustomList 진단입니다.

#ifdef _DEBUG
void CCustomList::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CCustomList::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

// CCustomList 메시지 처리기입니다.

BOOL CCustomList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_SHIFT) && pMsg->wParam == VK_UP) {
			OnListUp();
			SelectItem(m_nSelectItem, m_nSelCol);
		}
		else if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_SHIFT) && pMsg->wParam == VK_DOWN) {
			OnListDown();
			SelectItem(m_nSelectItem, m_nSelCol);
		}
		else if (pMsg->wParam == VK_UP) {
			if (m_nSelectItem > 0) {
				CheckInputEditText();
				--m_nSelectItem;
				SelectItem(m_nSelectItem, m_nSelCol);
			}
			return CScrollView::PreTranslateMessage(pMsg);
		}
		else if (pMsg->wParam == VK_DOWN) {
			if (m_nSelectItem >= 0 && m_nSelectItem < m_listData.GetCount() - 1) {
				CheckInputEditText();
				++m_nSelectItem;
				SelectItem(m_nSelectItem, m_nSelCol);
			}
			else if (m_nSelectItem >= 0 && m_nSelectItem == m_listData.GetCount() - 1 && m_arrayType.GetAt(m_nSelCol) == POPUP_TYPE_EDIT) {
				CheckInputEditText();
				/*m_nSelectItem = -1;
				m_nSelCol = -1;
				SelectItem(m_nSelectItem, m_nSelCol);*/
			}
			return CScrollView::PreTranslateMessage(pMsg);
		}
		else if (pMsg->wParam == 0x99) {
			if (m_nSelectItem >= 0 && m_nSelectItem < m_listData.GetCount() - 1) {
				CheckInputEditText();
				++m_nSelectItem;
				SelectItem(m_nSelectItem, m_nSelCol);
			}
			else if (m_nSelectItem >= 0 && m_nSelectItem == m_listData.GetCount() - 1 && m_arrayType.GetAt(m_nSelCol) == POPUP_TYPE_EDIT) {
				CheckInputEditText();
				m_nSelectItem = -1;
				m_nSelCol = -1;
				SelectItem(m_nSelectItem, m_nSelCol);
			}
			return false;
		}
		else if (pMsg->wParam == VK_RIGHT) {
			if (m_nSelectItem >= 0 && m_nSelCol < m_arraySize.GetCount() - 1) {
				CheckInputEditText();
				++m_nSelCol;
				SelectItem(m_nSelectItem, m_nSelCol);
				int nPos1, nPos2;
				if (GetSelectedItemPos(nPos1, nPos2)) {
					GetParent()->PostMessage(LIST_HORIZON, nPos1, nPos2);
				}
			}
		}
		else if (pMsg->wParam == VK_LEFT) {
			if (m_nSelectItem >= 0 && m_nSelCol > 0) {
				CheckInputEditText();
				--m_nSelCol;
				SelectItem(m_nSelectItem, m_nSelCol);
				int nPos1, nPos2;
				if (GetSelectedItemPos(nPos1, nPos2)) {
					GetParent()->PostMessage(LIST_HORIZON, nPos1, nPos2);
				}
			}
		}
		else if (m_bUseTabKey && GetAsyncKeyState(VK_SHIFT) && pMsg->wParam == VK_TAB) {
			if (m_nSelectItem >= 0 && m_nSelCol > 0) {
				CheckInputEditText();
				--m_nSelCol;
				SelectItem(m_nSelectItem, m_nSelCol);
				int nPos1, nPos2;
				if (GetSelectedItemPos(nPos1, nPos2)) {
					GetParent()->PostMessage(LIST_HORIZON, nPos1, nPos2);
				}
			}
			else if (m_nSelectItem > 0) {
				CheckInputEditText();
				m_nSelCol = m_arraySize.GetCount() - 1;
				--m_nSelectItem;
				SelectItem(m_nSelectItem, m_nSelCol);
				int nPos1, nPos2;
				if (GetSelectedItemPos(nPos1, nPos2)) {
					GetParent()->PostMessage(LIST_HORIZON, nPos1, nPos2);
				}
			}
		}
		else if (m_bUseTabKey && pMsg->wParam == VK_TAB) {
			if (m_nSelCol < m_arraySize.GetCount() - 1) {
				CheckInputEditText();
				++m_nSelCol;	
				SelectItem(m_nSelectItem, m_nSelCol);
				int nPos1, nPos2;
				if (GetSelectedItemPos(nPos1, nPos2)) {
					GetParent()->PostMessage(LIST_HORIZON, nPos1, nPos2);
				}
			}
			else if (m_nSelectItem < m_listData.GetCount() - 1) {
				CheckInputEditText();
				m_nSelCol = 0;
				++m_nSelectItem;
				SelectItem(m_nSelectItem, m_nSelCol);
				int nPos1, nPos2;
				if (GetSelectedItemPos(nPos1, nPos2)) {
					GetParent()->PostMessage(LIST_HORIZON, nPos1, nPos2);
				}
			}
		}
		else if (pMsg->wParam == VK_DELETE) {
			SetItemText(m_nSelectItem, m_nSelCol, L"");
		}
		else if (pMsg->wParam == VK_PRIOR) { // page up
			CheckInputEditText();
			PostMessage(WM_PAGE_MOVE_KEY, 0, 0);
		}
		else if (pMsg->wParam == VK_NEXT) { // page down
			CheckInputEditText();
			PostMessage(WM_PAGE_MOVE_KEY, 1, 0);
		}
		else if (GetAsyncKeyState(VK_CONTROL) && pMsg->wParam == VK_HOME) {
			CheckInputEditText();
			PostMessage(WM_PAGE_MOVE_KEY, 2, 0);
		}
		else if (GetAsyncKeyState(VK_CONTROL) && pMsg->wParam == VK_END) {
			CheckInputEditText();
			PostMessage(WM_PAGE_MOVE_KEY, 3, 0);
		}
		else if (GetAsyncKeyState(VK_CONTROL) && pMsg->wParam == 0x43) { // ctrl + c
			CString sText;
			if (GetItemText(m_nSelectItem, m_nSelCol, sText)) {
				CopyTextToClipboard(sText.GetBuffer(0));
			}
		}
		else if (GetAsyncKeyState(VK_CONTROL) && pMsg->wParam == 0x56) { // ctrl + v
			CString sText;
			if (m_nSelectItem >= 0 && m_nSelCol >= 0 && GetTextClipboard(sText)) {
				if (m_arrayType.GetAt(m_nSelCol) == POPUP_TYPE_LIST_ADD || m_arrayType.GetAt(m_nSelCol) == POPUP_TYPE_TEXT) {
					SetItemText(m_nSelectItem, m_nSelCol, sText);
				}
			}
		}
		else if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE /*|| pMsg->wParam == 229*/) {
			if (m_nSelectItem >= 0 && m_nSelectItem < m_listData.GetCount() && m_nSelCol >= 0 && m_arrayType.GetAt(m_nSelCol) == POPUP_TYPE_SPACE) {
				CStringArray arrayList;
				CString sText, sValue;
				if (GetItemText(m_nSelectItem, m_nSelCol, sText) && GetStateArrayString(m_nSelCol, arrayList)) {
					sValue = GetStateString(sText, arrayList);
					SetItemText(m_nSelectItem, m_nSelCol, sValue);
				}
			}
			else if (m_nSelectItem >= 0 &&  m_nSelectItem < m_listData.GetCount() && m_nSelCol >= 0 && m_nSelCol < m_arrayType.GetCount()){
				if (m_arrayType.GetAt(m_nSelCol) != POPUP_TYPE_EDIT) {
					for (int i = 0; i < m_arraySize.GetCount(); i++) {
						if (i == m_nSelCol) {
							break;
						}
					}
					m_pParent->PostMessage(LIST_SELECTION, m_nSelectItem, m_nSelCol);
				}
			}
			return CScrollView::PreTranslateMessage(pMsg);
		}
	}

	return CScrollView::PreTranslateMessage(pMsg);
}

void CCustomList::CheckInputEditText()
{
	if (m_nSelCol <= -1 || m_nSelCol >= m_arraySize.GetCount()) {
		return;
	}
	if (m_arrayType.GetAt(m_nSelCol) == POPUP_TYPE_EDIT) {
		CString sText;
		if (GetSelectEditText(sText)) {
			SetItemText(m_nSelectItem, m_nSelCol, sText);
		}
	}
}

BOOL CCustomList::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CScrollView::OnEraseBkgnd(pDC);
}

void CCustomList::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CScrollView::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CCustomList::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CCustomList::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnTimer(nIDEvent);
}

void CCustomList::ReleaseListItem()
{
	m_listSync.Enter();
	pItemData pData = NULL;
	POSITION pos = m_listData.GetHeadPosition();
	while (pos) {
		pData = m_listData.GetNext(pos);
		for (int i = 0; i < pData->listText.GetCount(); i++) {
			pItemValue pValue = pData->listText.GetAt(pData->listText.FindIndex(i));
			SAFE_DELETE(pValue);
		}
		pData->listText.RemoveAll();
	}
	m_listSync.Leave();
	Invalidate();
}

bool CCustomList::GetStateArrayString(int nIndex, CStringArray & arrayList)
{
	pStateString pString;
	POSITION pos = m_stateString.GetHeadPosition();
	while (pos) {
		pString = m_stateString.GetNext(pos);
		if (pString->nColumn == nIndex) {
			arrayList.Copy(pString->value);
			return true;
		}
	}
	return false;
}

CString CCustomList::GetStateString(CString sText, CStringArray & arrayList)
{
	for (int nIndex = 0; nIndex < arrayList.GetCount(); nIndex++) {
		if (sText.Compare(arrayList.GetAt(nIndex)) == 0) {
			++nIndex;
			if (nIndex == arrayList.GetCount()) {
				nIndex = 0;
			}
			return arrayList.GetAt(nIndex);
		}
	}
	return L"";
}


bool CCustomList::GetTextClipboard(CString & sText)
{
	if (::IsClipboardFormatAvailable(CF_TEXT)) // 믄자형이 클립보드에 있으면
	{
		if (::OpenClipboard(NULL))
		{
			HANDLE hData = ::GetClipboardData(CF_TEXT); // 데이터 가져오기
			LPCTSTR _data = (LPCTSTR)::GlobalLock(hData);
			if (!hData)
			{
				::GlobalUnlock(hData);
				::CloseClipboard();
				return false;
			}
			sText = CCommonFunc::CharToWCHAR((char*)_data);
			::CloseClipboard();
			return true;
		}
	}
	return false;
}

void CCustomList::CopyTextToClipboard(WCHAR *ap_string)
{
	int string_length = _tcslen(ap_string) + 1;
	char* pTemp = new char[string_length * sizeof(WCHAR)];
	strcpy_s(pTemp, string_length * sizeof(WCHAR), CCommonFunc::WCharToChar(ap_string));
	string_length = strlen(pTemp) + 1;
	// 저장할 문자열의 길이를 구한다. ('\0'까지 포함한 크기)
	// 클립보드로 문자열을 복사하기 위하여 메모리를 할당한다. 
	// 클립보드에는 핸들을 넣는 형식이라서 HeapAlloc 함수 사용이 블가능하다. 
	HANDLE h_data = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, string_length);
	// 할당된 메모리에 문자열을 복사하기 위해서 사용 가능한 주소를 얻는다. 
	CHAR *p_data = (CHAR *)::GlobalLock(h_data);
	if (NULL != p_data)
	{
		// 할당된 메모리 영역에 삽입할 문자열을 복사한다. 
		memcpy(p_data, pTemp, string_length);
		// 문자열을 복사하기 위해서 Lock 했던 메모리를 해제한다.
		::GlobalUnlock(h_data);
		if (::OpenClipboard(m_hWnd))
		{
			::EmptyClipboard(); // 클립보드를 연다.
			::SetClipboardData(CF_TEXT, h_data);  // 클립보드에 저장된 기존 문자열을 삭제한다.
												  // 클립보드로 문자열을 복사한다.
			::CloseClipboard(); // 클립보드를 닫는다.
		}
	}
	else {
		::GlobalUnlock(h_data);
	}
	SAFE_DELETE(pTemp);
}

void CCustomList::SetRowHeight(int nValue)
{
	g_nItemHeight = nValue;
}

int CCustomList::GetRowHeight()
{
	return g_nItemHeight;
}

void CCustomList::SetFontSize(int nSize)
{
	m_nFontSize = nSize;
	CCustomItem* pItem = NULL;
	m_listSync.Enter();
	int nCount = m_listItem.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pItem = m_listItem.GetAt(m_listItem.FindIndex(nIndex));
		pItem->SetFontSize(nSize);
	}
	m_listSync.Leave();
}

void CCustomList::SetFontBold(UINT nValue)
{
	m_nFontBold = nValue;
	CCustomItem* pItem = NULL;
	m_listSync.Enter();
	int nCount = m_listItem.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pItem = m_listItem.GetAt(m_listItem.FindIndex(nIndex));
		pItem->SetFontBold(nValue);
	}
	m_listSync.Leave();
}

void CCustomList::SetFontName(CString sName)
{
	m_sFontName = sName;
	CCustomItem* pItem = NULL;
	m_listSync.Enter();
	int nCount = m_listItem.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pItem = m_listItem.GetAt(m_listItem.FindIndex(nIndex));
		pItem->SetFontName(sName);
	}
	m_listSync.Leave();
}

void CCustomList::SetColumnLimit(CUIntArray & arrayList)
{
	m_arrayLimit.Copy(arrayList);
}

void CCustomList::AddListItemState(int nColumnIndex, CString sText)
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

	if (m_stateString.GetCount() == 1) {
		m_listSync.Enter();
		int nCount = m_listData.GetCount();
		for (int nIndex = 0; nIndex < nCount; nIndex++) {
			ModItemText(nIndex, nColumnIndex, sText);
		}
		m_sizeSync.Leave();
	}
}

int CCustomList::AddItem()
{
	int nIndex = -1;
	m_listSync.Enter();

	nIndex = m_listData.GetCount();
	pItemData pData = new itemData;
	m_listData.AddTail(pData);

	m_sizeSync.Leave();

	for (int i = 0; i < m_arrayType.GetCount(); i++) {
		if (m_arrayType.GetAt(i) == POPUP_TYPE_SPACE) {
			CStringArray arrayList;
			if (GetStateArrayString(i, arrayList)) {
				if (arrayList.GetCount() > 0) {
					ModItemText(nIndex, i, arrayList.GetAt(0));
				}
			}
		}
	}


	return nIndex;
}

int CCustomList::InsertItem(int nIndex, int nType/*=0*/)
{
	if (nIndex < 0 || nIndex >= m_listData.GetCount()) {
		return -1;
	}

	m_listSync.Enter();

	pItemData pData = new itemData;

	if (nType == 0) {
		m_listData.InsertBefore(m_listData.FindIndex(nIndex), pData); // up add
	}
	else if (nType == 1) {
		m_listData.InsertAfter(m_listData.FindIndex(nIndex), pData); // down add
	}

	m_listSync.Leave();

	for (int i = 0; i < m_arrayType.GetCount(); i++) {
		if (m_arrayType.GetAt(i) == POPUP_TYPE_SPACE) {
			CStringArray arrayList;
			if (GetStateArrayString(i, arrayList)) {
				if (arrayList.GetCount() > 0) {
					if (nType == 0) {
						ModItemText(nIndex, i, arrayList.GetAt(0));
					}
					else if (nType == 1) {
						ModItemText(nIndex + 1, i, arrayList.GetAt(0));
					}
				}
			}
		}
	}

	Redisplay();

	return nIndex;
}

bool CCustomList::DeleteItem(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_listData.GetCount()) {
		return false;
	}
	m_listSync.Enter();

	pItemData pData = m_listData.GetAt(m_listData.FindIndex(nIndex));
	m_listData.RemoveAt(m_listData.FindIndex(nIndex));
	SAFE_DELETE(pData);
	m_nSelectItem = -1;
	m_nSelCol = -1;

	m_listSync.Leave();
	return true;
}

bool CCustomList::DeleteAllItems()
{
	m_listSync.Enter();

	pItemData pData = NULL;
	while (!m_listData.IsEmpty()) {
		pData = m_listData.RemoveHead();
		for (int i = 0; i < pData->listText.GetCount(); i++) {
			pItemValue pValue = pData->listText.GetAt(pData->listText.FindIndex(i));
			SAFE_DELETE(pValue);
		}
		SAFE_DELETE(pData);
	}
	m_nSelectItem = -1;
	m_nSelCol = -1;

	m_listSync.Leave();
	return true;
}

void CCustomList::CheckTextLimit(int nSelColumn, CString & sText)
{
	if (m_arrayLimit.GetCount() > nSelColumn && m_arrayLimit.GetAt(nSelColumn) > 0) {
		int nLimit = m_arrayLimit.GetAt(nSelColumn);
		CStringA sTemp = CCommonFunc::WCharToChar(sText.GetBuffer(0));
		int len = sTemp.GetLength();
		if (len > nLimit) {
			sText = CCommonFunc::CharToWCHAR(sTemp.Left(nLimit).GetBuffer(0));
		}
		sTemp.ReleaseBuffer();
	}
}

bool CCustomList::InsertItemText(int nItem, int nSelColumn, CString sText)
{
	pItemData pData = m_listData.GetAt(m_listData.FindIndex(nItem));

	CheckTextLimit(nSelColumn, sText);

	pItemValue pValue = new itemValue;
	pValue->nColIndex = nSelColumn;
	pValue->sText = sText;

	pData->listText.AddTail(pValue);

	return true;
}
bool CCustomList::ModItemText(int nItem, int nSelColumn, CString sText)
{
	if (nItem >= m_listData.GetCount() || nItem < 0) {
		return false;
	}
	bool bFind = false;
	pItemData pData = m_listData.GetAt(m_listData.FindIndex(nItem));
	for (int i = 0; i < pData->listText.GetCount(); i++)
	{
		pItemValue pValue = pData->listText.GetAt(pData->listText.FindIndex(i));
		if (pValue->nColIndex == nSelColumn) {

			CheckTextLimit(nSelColumn, sText);

			pValue->sText = sText;
			bFind = true;
			break;
		}
	}
	if (bFind) {
		return true;
	}

	return InsertItemText(nItem, nSelColumn, sText);
}

bool CCustomList::SetItemText(int nItem, int nSelColumn, CString sText)
{
	if (nItem >= m_listData.GetCount()) {
		return false;
	}

	m_listSync.Enter();
	bool bResult = ModItemText(nItem, nSelColumn, sText);
	m_listSync.Leave();

	Invalidate();

	return bResult;
}

bool CCustomList::GetItemText(int nItem, int nSelColumn, CString & sText)
{
	if (nItem >= m_listData.GetCount()) {
		return false;
	}
	bool bFind = false;
	sText = L"";
	pItemData pData = m_listData.GetAt(m_listData.FindIndex(nItem));
	for (int i = 0; i < pData->listText.GetCount(); i++)
	{
		pItemValue pValue = pData->listText.GetAt(pData->listText.FindIndex(i));
		if (pValue->nColIndex == nSelColumn) {
			sText = pValue->sText;
			return true;
		}
	}
	return false;
}

bool CCustomList::GetSelectEditText(CString & sText) {
	if (m_nSelectItem < 0 || m_nSelCol < 0 || m_nSelectItem >= m_listData.GetCount()) {
		return false;
	}
	bool bFind = false;
	CCustomItem* pItem = NULL;
	for (int i = 0; i < m_listItem.GetCount(); i++)
	{
		pItem = m_listItem.GetAt(m_listItem.FindIndex(i));
		if (pItem && pItem->GetItemIndex() == m_nSelectItem) {
			sText = pItem->GetEditText();
			return true;
		}
	}
	return false;
}

int  CCustomList::GetItemCount()
{
	m_listSync.Enter();
	int nCount = m_listData.GetCount();
	m_listSync.Leave();

	return nCount;
}

void CCustomList::SetScrollPosition(int nPos)
{
	m_nScrollPos = nPos;

	Redisplay();
}

int CCustomList::GetListHeight()
{
	int nCount = m_listData.GetCount();
	return nCount * g_nItemHeight;
}

void CCustomList::SetRowColor(COLORREF bkColor, COLORREF bkSelColor, COLORREF textColor, COLORREF textSelColor, COLORREF lineColor)
{
	m_bkColor = bkColor;
	m_bkSelColor = bkSelColor;
	m_textColor = textColor;
	m_textSelColor = textSelColor;
	m_lineColor = lineColor;
}

bool CCustomList::SetItemData(int nItemIndex, LPVOID lpData)
{
	m_listSync.Enter();
	if (m_listData.GetCount() <= nItemIndex) {
		m_listSync.Leave();
		return false;
	}
	pItemData pData = m_listData.GetAt(m_listData.FindIndex(nItemIndex));
	if (!pData) {
		m_listSync.Leave();
		return false;
	}
	pData->lpData = lpData;
	m_listSync.Leave();
	return true;
}

LPVOID CCustomList::GetItemData(int nItemIndex)
{
	LPVOID lpData = NULL;
	m_listSync.Enter();
	if (m_listData.GetCount() <= nItemIndex) {
		m_listSync.Leave();
		return NULL;
	}
	pItemData pData = m_listData.GetAt(m_listData.FindIndex(nItemIndex));
	if (!pData) {
		m_listSync.Leave();
		return NULL;
	}
	lpData = pData->lpData;
	m_listSync.Leave();

	return lpData;
}

bool CCustomList::GetSelectItemRect(CRect & rect)
{
	int nCount = m_listItem.GetCount();
	CCustomItem* pItem;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pItem = m_listItem.GetAt(m_listItem.FindIndex(nIndex));
		if (pItem->GetItemIndex() == m_nSelectItem) {
			pItem->GetSelectItemRect(rect);
			return true;
		}
	}
	return false;
}

void CCustomList::SendSizeTypeInfo(CCustomItem* pItem)
{
	m_sizeSync.Enter();
	pItem->SetRowInfo(m_arraySize, m_arrayType, m_arrayAlign);
	m_sizeSync.Leave();
	return;

	m_sizeSync.Enter();
	m_arraySizeSend[m_sizeIndex].RemoveAll();
	m_arraySizeSend[m_sizeIndex].Copy(m_arraySize);
	pItem->PostMessage(WM_RESIZE_COLUMN, 0, (LPARAM)&m_arraySizeSend[m_sizeIndex]);
	if (++m_sizeIndex >= SENDER_COUNT) {
		m_sizeIndex = 0;
	}

	m_arraySizeSend[m_sizeIndex].RemoveAll();
	m_arraySizeSend[m_sizeIndex].Copy(m_arrayType);
	pItem->PostMessage(WM_RESIZE_COLUMN, 1, (LPARAM)&m_arraySizeSend[m_sizeIndex]);
	if (++m_sizeIndex >= SENDER_COUNT) {
		m_sizeIndex = 0;
	}

	m_arraySizeSend[m_sizeIndex].RemoveAll();
	m_arraySizeSend[m_sizeIndex].Copy(m_arrayAlign);
	pItem->PostMessage(WM_RESIZE_COLUMN, 2, (LPARAM)&m_arraySizeSend[m_sizeIndex]);
	if (++m_sizeIndex >= SENDER_COUNT) {
		m_sizeIndex = 0;
	}
	m_sizeSync.Leave();
}

bool CCustomList::GetCheckShowTopItem(int nItemIndex)
{
	CRect rect;
	GetClientRect(&rect);

	int nListCount = rect.Height() / g_nItemHeight + 2;

	int nTop = 0;
	int nStartPosition = 0;
	if (m_nScrollPos > 0) {
		nStartPosition = m_nScrollPos / g_nItemHeight - 1;
		if (nStartPosition < 0) {
			nStartPosition = 0;
		}
		nTop -= m_nScrollPos % g_nItemHeight;
	}
	if (nStartPosition > nItemIndex || (nStartPosition == nItemIndex && m_nScrollPos % g_nItemHeight > 0)) {
		return true;
	}
	return false;
}

bool CCustomList::GetCheckShowBottomItem(int nItemIndex)
{
	CRect rect;
	GetClientRect(&rect);

	int nListCount = rect.Height() / g_nItemHeight;
	int nLeft = m_nScrollPos % g_nItemHeight;
	if (nLeft > 0) {
		nListCount = 1;
		nListCount += (rect.Height() - nLeft) / g_nItemHeight;
		nListCount += ((rect.Height() - nLeft) % g_nItemHeight > 0) ? 1 : 0;
	}
	int nTop = 0;
	int nStartPosition = 0;
	if (m_nScrollPos > 0) {
		nStartPosition = m_nScrollPos / g_nItemHeight;
		if (m_nScrollPos % g_nItemHeight > 0) {
			nStartPosition -= 1;
		}
		if (nStartPosition < 0) {
			nStartPosition = 0;
		}
		nTop -= m_nScrollPos % g_nItemHeight;
	}
	if (nStartPosition + nListCount - 1 <= nItemIndex) {
		return true;
	}
	return false;
}

void CCustomList::SetColumnInfo(CUIntArray & headerSize, CUIntArray & headerType, CUIntArray & alignType)
{
	m_arraySize.Copy(headerSize);
	m_arrayType.Copy(headerType);
	m_arrayAlign.Copy(alignType);
}

BOOL CCustomList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/*CRect rect;
	GetClientRect(&rect);

	int nItemCount = m_listItem.GetCount();
	int nTotalHeight = nItemCount * (g_nItemTerm + g_nItemHeight) - g_nItemTerm;
	int nSign = zDelta / abs(zDelta);

	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	int nPos = si.nTrackPos + (-nSign * 50);
	if (nTotalHeight < rect.Height() + nPos) {
		nPos = nTotalHeight - rect.Height();
	}
	if (nPos < 0) {
		nPos = 0;
	}

	SetScrollPos(SB_VERT, nPos, TRUE);*/

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CCustomList::SetSizeList(CUIntArray & headerSize)
{
	bool bValue = false;
	if (m_arraySize.GetCount() == headerSize.GetCount()) {
		for (int nIndex = 0; nIndex < headerSize.GetCount(); nIndex++) {
			if (m_arraySize.GetAt(nIndex) != headerSize.GetAt(nIndex)) {
				bValue = true;
				break;
			}
		}
	}
	if (!bValue) {
		return;
	}
	m_sizeSync.Enter();

	m_arraySizeSend[m_sizeIndex].RemoveAll();
	m_arraySize.RemoveAll();

	m_arraySize.Copy(headerSize);

	m_sizeSync.Leave();

	Redisplay();
}

void CCustomList::EnableLastInsertRow(bool bLast, bool bFirst/*=false*/)
{
	m_bLastInsertRow = bLast;
	m_bFirstInsertRow = bFirst;
}

LRESULT CCustomList::OnListPageKey(WPARAM wParam, LPARAM lParam)
{
	m_pParent->PostMessage(WM_PAGE_MOVE_KEY, wParam, lParam);
	return 0;
}

LRESULT CCustomList::OnMouseClick(WPARAM wParam, LPARAM lParam)
{
	int nItemIndex = (int)wParam;
	int nSelCol = (int)lParam;

	if (m_nSelectItem != nItemIndex || m_nSelCol != nSelCol) {
		CheckInputEditText();
		m_nSelectItem = nItemIndex;
		m_nSelCol = nSelCol;
		SelectItem(m_nSelectItem, m_nSelCol);
	}

	/*if (nItemIndex >= 0 && nItemIndex < m_listData.GetCount()) {
		CheckInputEditText();
		m_nSelectItem = nItemIndex;
		m_nSelCol = nSelCol;
		SelectItem(m_nSelectItem, m_nSelCol);
	}*/

	return 0;
}

LRESULT CCustomList::OnListLButtonClick(WPARAM wParam, LPARAM lParam)
{
	int nItemIndex = (int)wParam;
	int nSelCol = (int)lParam;

	OnMouseClick(nItemIndex, nSelCol);

	m_nSelectItem = nItemIndex;
	m_nSelCol = nSelCol;
	if (nItemIndex == -1 || nSelCol == -1)
	{
		return 0;
	}
	//MouseClick(nItemIndex, nSelCol);

	if (m_nSelectItem >= 0 && m_nSelectItem < m_listData.GetCount() && m_nSelCol >= 0 && m_nSelCol < m_arrayType.GetCount()) {
		m_pParent->PostMessage(LIST_SELECTION, m_nSelectItem, m_nSelCol);
	}

	return 0;
}

LRESULT CCustomList::OnListRButtonClick(WPARAM wParam, LPARAM lParam)
{
	int nItemIndex = (int)wParam;
	int nSelCol = (int)lParam;

	PostMessage(WM_MOUSE_CLICK, nItemIndex, nSelCol);
	//MouseClick(nItemIndex, nSelCol);

	POINT pt;
	GetCursorPos(&pt);

	CMenu muTemp, *pContextMenu;

	//20221026 GBM start - 회로번호 이동 메뉴로 대체
#if 1
	muTemp.LoadMenu(IDR_MENU_MOVE);				// 메뉴는 정의해 놓은 것 중 원하는 것
	pContextMenu = muTemp.GetSubMenu(0);
	pContextMenu->EnableMenuItem(ID_LIST_CIRCUIT_NO_CHANGE, MF_BYCOMMAND | MF_ENABLED);
	if (!m_bDisable) {
		CCommonState::ie()->m_bMenu = true;
		pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
		CCommonState::ie()->m_bMenu = false;
	}
#else
	muTemp.LoadMenu(IDR_MENU_LIST);				// 메뉴는 정의해 놓은 것 중 원하는 것
	pContextMenu = muTemp.GetSubMenu(0);

	if (m_listData.GetCount() <= m_nSelectItem + 1) {
		if (!m_bLastInsertRow) {
			pContextMenu->EnableMenuItem(ID_LIST_DOWNADD, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		pContextMenu->EnableMenuItem(ID_LIST_DOWN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	else if (0 >= m_nSelectItem) {
		pContextMenu->EnableMenuItem(ID_LIST_UP, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		/*if (m_bFirstInsertRow) {
		pContextMenu->EnableMenuItem(ID_LIST_UPADD, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}*/
	}

	/*MENUINFO MenuInfo = { 0 };
	MenuInfo.cbSize = sizeof(MENUINFO);
	pContextMenu->GetMenuInfo(&MenuInfo);
	MenuInfo.hbrBack = ::CreateSolidBrush(RGB(255, 255, 255));
	MenuInfo.fMask = MIM_BACKGROUND | MIM_STYLE;
	MenuInfo.dwStyle = MIM_APPLYTOSUBMENUS;
	pContextMenu->SetMenuInfo(&MenuInfo);*/
	if (!m_bDisable) {
		CCommonState::ie()->m_bMenu = true;
		pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
		//UINT nResult = pContextMenu->GetMenuState(IDR_MENU_LIST, 0);
		CCommonState::ie()->m_bMenu = false;
	}

	/*if (m_nSelectItem >= 0 && m_nSelectItem < m_listData.GetCount() && m_nSelCol >= 0 && m_nSelCol < m_arrayType.GetCount()) {
	for (int i = 0; i < m_arraySize.GetCount(); i++) {
	if (i == m_nSelCol) {
	break;
	}
	}
	m_pParent->PostMessage(LIST_SELECTION, m_nSelectItem, m_nSelCol);
	}*/
#endif
	//20221026 GBM end

	return 0;
}

bool CCustomList::GetSelectedItemPos(int & nPos1, int & nPos2)
{
	m_listSync.Enter();

	CCustomItem* pItem = NULL;
	int nCount = m_listItem.GetCount();
	if (m_listData.GetCount() <= m_nSelectItem) {
		m_listSync.Leave();
		return false;
	}
	pItem = NULL;
	bool bFind = false;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pItem = m_listItem.GetAt(m_listItem.FindIndex(nIndex));
		if (pItem && pItem->GetItemIndex() == m_nSelectItem) {
			bFind = true;
			break;
		}
	}
	if (!bFind) {
		m_listSync.Leave();
		return false;
	}
	//pItem = m_listItem.GetAt(m_listItem.FindIndex(m_nSelectItem));
	CRect rect;
	if (pItem && pItem->GetSelectItemRect(rect)) {
		pItem->ScreenToClient(rect);
		nPos1 = rect.left;
		nPos2 = rect.right;
		m_listSync.Leave();
		return true;
	}

	m_listSync.Leave();
	return false;
}

void CCustomList::SelectItem(int nItemIndex, int nSelCol)
{
	m_listSync.Enter();
	CCustomItem* pItem = NULL;
	int nCount = m_listItem.GetCount();
	if (m_listData.GetCount() <= nItemIndex) {
		m_listSync.Leave();
		return;
	}
	int nSelIndex = 0;
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pItem = m_listItem.GetAt(m_listItem.FindIndex(nIndex));
		if (!pItem) {
			continue;
		}
		if (pItem->GetItemIndex() != nItemIndex) {
			pItem->SetSelectedState(false, 0);
		}
		else {
			pItem->SetSelectedState(true, nSelCol);
			nSelIndex = nIndex;
		}
	}
	pItem = m_listItem.GetAt(m_listItem.FindIndex(nSelIndex));
	if (pItem) {
		if (m_nSelCol >= 0 && m_nSelCol < m_arrayType.GetCount() && m_arrayType.GetAt(m_nSelCol) == POPUP_TYPE_EDIT) {
			pItem->SetFocusItem(1);
		}
		else {
			pItem->SetFocusItem(0);
		}
	}
	m_nSelectItem = nItemIndex;
	m_nSelCol = nSelCol;

	m_listSync.Leave();

	if (m_nSelectItem != -1 && m_nSelCol != -1) {
		if (FindPosition()) {
		}
		else if (GetCheckShowTopItem(m_nSelectItem)) {
			m_pParent->PostMessage(SCROLL_NOTIFY, 0, 0);
		}
		else if (GetCheckShowBottomItem(m_nSelectItem)) {
			m_pParent->PostMessage(SCROLL_NOTIFY, 1, 0);
		}
	}
}

void CCustomList::SetDisableMenu(bool bDisable)
{
	m_bDisable = bDisable;
}

int CCustomList::FindPosition(int nType /*= 0*/)
{
	CRect rect;
	GetClientRect(&rect);

	int nListCount = rect.Height() / g_nItemHeight;
	int nLeft = m_nScrollPos % g_nItemHeight;
	if (nLeft > 0) {
		nListCount = 1;
		nListCount += (rect.Height() - nLeft) / g_nItemHeight;
		nListCount += ((rect.Height() - nLeft) % g_nItemHeight > 0) ? 1 : 0;
	}
	int nTop = 0;
	int nStartPosition = 0;
	if (m_nScrollPos > 0) {
		nStartPosition = m_nScrollPos / g_nItemHeight;
		if (m_nScrollPos % g_nItemHeight > 0) {
			nStartPosition -= 1;
		}
		if (nStartPosition < 0) {
			nStartPosition = 0;
		}
		nTop -= m_nScrollPos % g_nItemHeight;
	}
	if (m_nSelectItem < nStartPosition || m_nSelectItem >= nStartPosition + nListCount) {
		if (nType == 0) {
			m_pParent->PostMessage(SCROLL_NOTIFY, 2, m_nSelectItem * g_nItemHeight);
			return 1;
		}
		return (m_nSelectItem * g_nItemHeight == 0) ? -1 : m_nSelectItem * g_nItemHeight;
	}
	return 0;
}

void CCustomList::SetParentWnd(CWnd* pWnd)
{
	m_pParent = pWnd;
}

bool CCustomList::UseTabKey(bool bUse)
{
	m_bUseTabKey = bUse;
	return true;
}

void CCustomList::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	int nListCount = rect.Height() / g_nItemHeight + 2;

	if (nListCount + 2 > m_listItem.GetCount()) {
		for (int nIndex = m_listItem.GetCount(); nIndex < nListCount + 2; nIndex++) {
			CCustomItem* pItem = new CCustomItem(this);
			pItem->Create(IDD_CUSTOM_ITEM_DIALOG, this);
			pItem->SetFontBold(m_nFontBold);
			pItem->SetFontSize(m_nFontSize);
			pItem->SetFontName(m_sFontName);
			pItem->SetItemLimit(m_arrayLimit);
			m_listItem.AddTail(pItem);
			SendSizeTypeInfo(pItem);
		}
	}

	int nTop = 0;
	int nStartPosition = 0;
	if (m_nScrollPos > 0) {
		nStartPosition = m_nScrollPos / g_nItemHeight - 1;
		if (nStartPosition < 0) {
			nStartPosition = 0;
		}
		nTop -= m_nScrollPos % g_nItemHeight;
	}
	CCustomItem* pItem = NULL;
	pItemData pData = NULL;
	int nShowCount = 0;
	int nItemIndex = 0;
	m_listSync.Enter();
	int nCount = m_listData.GetCount();
	for (int nIndex = nStartPosition; nIndex < nCount; nIndex++)
	{
		pData = m_listData.GetAt(m_listData.FindIndex(nIndex));

		//20220919 GBM start - Release 빌드 시 컴파일러 오류 방지를 위한 코드 수정
#if 1
		pItem = m_listItem.GetAt(m_listItem.FindIndex(nItemIndex));
		nItemIndex++;
#else
		pItem = m_listItem.GetAt(m_listItem.FindIndex(nItemIndex++));
#endif
		//20220919 GBM end

		if (!pItem) {
			continue;
		}
		SendSizeTypeInfo(pItem);

		//20221028 GBM start - 최대 회로 번호 이후 리스트 배경 색을 다른 색으로 표시
#if 1
		if (m_nChartIndex == 0)
		{
			if (CCommonState::ie()->m_nTotalCountCircuit_0 > nIndex)
			{
				if (nIndex % 2 == 0) {
					pItem->SetRowColor(m_bkColor, m_bkSelColor, m_textColor, m_textSelColor, m_lineColor);
				}
				else {
					pItem->SetRowColor(m_bkColor - RGB(15, 15, 15), m_bkSelColor, m_textColor, m_textSelColor, m_lineColor);
				}
			}
			else
			{
				COLORREF bkColor = RGB(0, 0, 0);
				pItem->SetRowColor(bkColor, m_bkSelColor, m_textColor, m_textSelColor, m_lineColor);
			}
		}
		else if (m_nChartIndex == 1)
		{
			if (CCommonState::ie()->m_nTotalCountCircuit_1 > nIndex)
			{
				if (nIndex % 2 == 0) {
					pItem->SetRowColor(m_bkColor, m_bkSelColor, m_textColor, m_textSelColor, m_lineColor);
				}
				else {
					pItem->SetRowColor(m_bkColor - RGB(15, 15, 15), m_bkSelColor, m_textColor, m_textSelColor, m_lineColor);
				}
			}
			else
			{
				COLORREF bkColor = RGB(0, 0, 0);
				pItem->SetRowColor(bkColor, m_bkSelColor, m_textColor, m_textSelColor, m_lineColor);
			}
		}
		else
		{
			if (nIndex % 2 == 0) {
				pItem->SetRowColor(m_bkColor, m_bkSelColor, m_textColor, m_textSelColor, m_lineColor);
			}
			else {
				pItem->SetRowColor(m_bkColor - RGB(15, 15, 15), m_bkSelColor, m_textColor, m_textSelColor, m_lineColor);
			}
		}
		
#else
		if (nIndex % 2 == 0) {
			pItem->SetRowColor(m_bkColor, m_bkSelColor, m_textColor, m_textSelColor, m_lineColor);
	}
		else {
			pItem->SetRowColor(m_bkColor - RGB(15, 15, 15), m_bkSelColor, m_textColor, m_textSelColor, m_lineColor);
		}
#endif
		//20221028 GBM end

		pItem->SetItemText(pData->listText);
		pItem->SetItemIndex(nIndex);
		pItem->MoveWindow(0, nTop, rect.Width(), g_nItemHeight);
		if (nIndex == m_nSelectItem) {
			pItem->SetSelectedState(true, m_nSelCol);
			if (m_arrayType.GetAt(m_nSelCol) == POPUP_TYPE_EDIT) {
				pItem->SetFocusItem(1);
			}
			else {
				pItem->SetFocusItem(0);
			}
		}
		else {
			pItem->SetSelectedState(false, -1);
		}
		pItem->ShowWindow(SW_SHOW);
		nTop += g_nItemHeight;

		if (++nShowCount >= nListCount) {
			break;
		}
	}
	for (int nIndex = nShowCount; nIndex < m_listItem.GetCount(); nIndex++) {
		pItem = m_listItem.GetAt(m_listItem.FindIndex(nIndex));
		if (!pItem) {
			continue;
		}
		pItem->SetItemIndex(-1);
		pItem->ShowWindow(SW_HIDE);
	}
	if (rect.Height() > nTop) {
		rect.top = nTop;
		CDC* _pDC = GetDC();
		CCommonDisplay::DrawRect(_pDC, true, RGB(255, 255, 255), RGB(255, 255, 255), rect, 1);
		ReleaseDC(_pDC);
	}

	m_listSync.Leave();
}

void CCustomList::OnListUp()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nSelectItem <= 0 || m_listData.GetCount() <= m_nSelectItem) {
		return;
	}
	m_listSync.Enter();
	pItemData pData = m_listData.GetAt(m_listData.FindIndex(m_nSelectItem));
	m_listData.RemoveAt(m_listData.FindIndex(m_nSelectItem));
	--m_nSelectItem;
	m_listData.InsertBefore(m_listData.FindIndex(m_nSelectItem), pData);
	m_listSync.Leave();

	Invalidate();
}

void CCustomList::OnListDown()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nSelectItem < 0 || m_listData.GetCount() <= m_nSelectItem + 1) {
		return;
	}
	m_listSync.Enter();
	pItemData pData = m_listData.GetAt(m_listData.FindIndex(m_nSelectItem));
	m_listData.RemoveAt(m_listData.FindIndex(m_nSelectItem));
	m_listData.InsertAfter(m_listData.FindIndex(m_nSelectItem), pData);
	++m_nSelectItem;
	m_listSync.Leave();

	Invalidate();
}

void CCustomList::OnListDel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nSelectItem < 0 || m_listData.GetCount() <= m_nSelectItem) {
		return;
	}
	GetParent()->PostMessage(LIST_CONTROL, 1, m_nSelectItem);
}

void CCustomList::OnListUpadd()
{
	if (m_nSelectItem < 0 || m_listData.GetCount() <= m_nSelectItem) {
		return;
	}
	GetParent()->PostMessage(LIST_CONTROL, 2, m_nSelectItem);
}

void CCustomList::OnListDownadd()
{
	GetParent()->PostMessage(LIST_CONTROL, 3, m_nSelectItem);
}

void CCustomList::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bLButtonDown) {
		m_bLButtonDown = true;
		SetCapture();
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CCustomList::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bLButtonDown) {
		PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, -1, -1);
		m_bLButtonDown = false;
		ReleaseCapture();
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void CCustomList::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bRButtonDown) {
		m_bRButtonDown = true;
		SetCapture();
	}

	CScrollView::OnRButtonDown(nFlags, point);
}

void CCustomList::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bRButtonDown) {
		PostMessage(WM_LBUTTON_BUTTONLIST_CLICK, -1, -1);
		m_bRButtonDown = false;
		ReleaseCapture();
	}

	CScrollView::OnRButtonUp(nFlags, point);
}

//20221013 GBM start - ReadOnly 주기 위한 추가
void CCustomList::SetReadOnly(bool bReadOnly)
{
	Redisplay();
	POSITION pos = m_listItem.GetHeadPosition();
	CCustomItem* pItem;
	while (pos) {
		pItem = m_listItem.GetNext(pos);
		pItem->SetReadOnly(bReadOnly);
	}
}
//20221013 GBM end

void CCustomList::OnListCircuitNoChange()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CCircuitNoMoveDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_nCircuitMoveNum[0] = -1;
		m_nCircuitMoveNum[1] = -1;

		int nNewCircuitNum = -1;
		wchar_t szNewCircuitNum[32];

		if (dlg.m_strNewCircuitNo.Compare(L"") == 0)
		{
			CMessagePopup popup(L"회로 번호 오류", L"\n\n회로 번호가 입력되지 않았습니다.", MB_OK, this);
			popup.DoModal();
			return;
		}

		wcscpy_s(szNewCircuitNum, dlg.m_strNewCircuitNo.GetBuffer(0));
		nNewCircuitNum = CCommonFunc::utoi(szNewCircuitNum);
		m_nCircuitMoveNum[0] = m_nSelectItem;
		m_nCircuitMoveNum[1] = nNewCircuitNum - 1;		//위치는 제로베이스
		GetParent()->PostMessage(LIST_CONTROL, 4, (LPARAM)m_nCircuitMoveNum);
	}
	else
	{
		return;
	}		
}

void CCustomList::SetBackgroundColorAfterMaxCircuit(int nChartIndex)
{
	m_nChartIndex = nChartIndex;
}