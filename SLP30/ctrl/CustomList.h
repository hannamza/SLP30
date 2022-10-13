#pragma once


#include <afxtempl.h>
#include "CustomItem.h"
// CCustomList 뷰입니다.

#define		SENDER_COUNT	100
#define		SCROLL_NOTIFY	WM_USER+9502
#define		LIST_SELECTION	WM_USER+9503
#define		LIST_CONTROL	WM_USER+9504
#define		LIST_HORIZON	WM_USER+9505
#define		LIST_POPUP_SEL	WM_USER+9506

typedef struct ITEM_DATA
{
	ITEM_DATA() {
		lpData = NULL;
	}
	LPVOID lpData;
	CList<pItemValue, pItemValue> listText;
}itemData, *pItemData;

class CCustomList : public CScrollView
{
	DECLARE_DYNCREATE(CCustomList)

protected:
	CCustomList();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CCustomList();

public:
	int  AddItem();
	int  InsertItem(int nIndex, int nType=0);
	bool DeleteItem(int nIndex);
	bool DeleteAllItems();
	void SetSizeList(CUIntArray & headerSize);
	void SetColumnInfo(CUIntArray & headerSize, CUIntArray & headerType, CUIntArray & alignType);
	bool SetItemText(int nItem, int nSelColumn, CString sText);
	bool GetItemText(int nItem, int nSelColumn, CString & sText);
	int  GetItemCount();
	void SetScrollPosition(int nPos);
	int  GetListHeight();
	void SetParentWnd(CWnd* pWnd);
	bool GetCheckShowTopItem(int nItemIndex);
	bool GetCheckShowBottomItem(int nItemIndex);
	void SetRowHeight(int nValue);
	int  GetRowHeight();
	void SetRowColor(COLORREF bkColor, COLORREF bkSelColor, COLORREF textColor, COLORREF textSelColor, COLORREF lineColor);
	bool SetItemData(int nItemIndex, LPVOID lpData);
	LPVOID GetItemData(int nItemIndex);
	bool GetSelectItemRect(CRect & rect);
	int  FindPosition(int nType=0);
	bool UseTabKey(bool bUse);
	void SetFontSize(int nSize);
	void SetFontBold(UINT nValue);
	void SetFontName(CString sName);
	void SetColumnLimit(CUIntArray & arrayList);
	void AddListItemState(int nColumnIndex, CString sText);
	bool GetSelectEditText(CString & sText);
	void ReleaseListItem();
	void SetDisableMenu(bool bDisable);
	void EnableLastInsertRow(bool bLast, bool bFirst = false);
	void SetReadOnly(bool bReadOnly);	//20221013 GBM - ReadOnly 주기 위한 추가

private:
	CWnd* m_pParent;
	INT		m_nSelectItem, m_nSelCol;
	INT		m_nShowIndex;
	INT		m_nPos, m_nScrollPos;
	bool m_bUseTabKey;
	bool m_bLastInsertRow;
	bool m_bFirstInsertRow;
	CUIntArray m_arraySize;
	CUIntArray m_arrayType;
	CUIntArray m_arrayAlign;
	CUIntArray m_arrayLimit;
	COLORREF m_bkColor, m_bkSelColor, m_textColor, m_textSelColor, m_lineColor;

	CCriticalSectionEx m_listSync, m_sizeSync;
	CList<pItemData, pItemData> m_listData;

private:
	CList<CCustomItem*, CCustomItem*> m_listItem;
	CList<pStateString, pStateString> m_stateString;

	CUIntArray m_arraySizeSend[SENDER_COUNT];
	int m_sizeIndex;

	int m_nFontSize;
	UINT m_nFontBold;
	CString m_sFontName;
	bool m_bDisable;
	bool m_bLButtonDown, m_bRButtonDown;

protected:
	void Redisplay();

	void CheckTextLimit(int nSelColumn, CString & sText);
	bool InsertItemText(int nItem, int nSelColumn, CString sText);
	bool ModItemText(int nItem, int nSelColumn, CString sText);
	void SendSizeTypeInfo(CCustomItem* pItem);
	void SelectItem(int nItemIndex, int nSelCol);
	void CopyTextToClipboard(WCHAR *ap_string);
	bool GetTextClipboard(CString & sText);
	bool GetSelectedItemPos(int & nPos1, int & nPos2);

	bool GetStateArrayString(int nIndex, CStringArray & arrayList);
	CString GetStateString(CString sText, CStringArray & arrayList);
	void CheckInputEditText();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnListLButtonClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListRButtonClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListPageKey(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseClick(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnListUp();
	afx_msg void OnListDown();
	afx_msg void OnListDel();
	afx_msg void OnListUpadd();
	afx_msg void OnListDownadd();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


