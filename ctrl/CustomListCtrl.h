#pragma once

#include "CustomHeader.h"
#include "CustomList.h"
#include "PopupTextDlg.h"
#include "ListPopupDlg.h"

// CCustomListCtrl 뷰입니다.
/*
제작: royd choi
email: csy19c@naver.com
*/

class CCustomListCtrl : public CScrollView
{
	DECLARE_DYNCREATE(CCustomListCtrl)

protected:
	CCustomListCtrl();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CCustomListCtrl();

public:
	bool AddItem(int nCount);
	bool ReleaseListItem();
	int  AddItem(); // -1: fail
	int  InsertItem(int nIndex); // -1: fail
	void SetListIndex(int nIndex); // 한 화면에 리스트를 여러개 사용할 때 구분하기 위한 인덱스
	void SetHeaderInfo(TCHAR** szHeaderList, int* nSize, int *nType, int* nAlign=NULL);
	void SetHeaderInfo(CStringArray& header, CUIntArray& size, CUIntArray& type, CUIntArray& align);
	void SetColumnLimit(int* colLimit);
	void SetColumnLimit(CUIntArray& limit);
	bool SetItemText(int nItem, int nSelColumn, CString sText);
	bool GetItemText(int nItem, int nCol, CString & sText);
	bool SetItemData(int nItemIndex, LPVOID lpData);
	LPVOID GetItemData(int nItemIndex);
	void UseTabKey(bool bUse);
	void FixHeaderSize(bool bFix);
	int  GetItemCount();
	void ShowScrollBar(bool bShow);
	void DeleteAllItems();
	void EnableLastInsertRow(bool bLast, bool bFirst = false);

	void SetHeaderHeight(int nHeight);
	int  GetHeaderHeight();
	void SetRowHeight(int nHeight);
	int  GetRowHeight();
	void SetHeaderBkColor(COLORREF bkColor, COLORREF lineColor);
	void SetHeaderTextColor(COLORREF color);
	void SetRowBkColor(COLORREF normal, COLORREF select, COLORREF line);
	void SetRowTextColor(COLORREF normal, COLORREF select);
	void SetTextPopupSize(int nWidth, int nHeight);
	void SetListPopupSize(int nWidth, int nHeight);
	void SetAutoHeaderSize(bool bAuto);
	void SetDisableMenu(bool bDisable);

	void AddPopupListItem(int nColumnIndex, CString sText);
	void RemovePopupListItem(int nColumIndex);
	void AddListItemState(int nColumnIndex, CString sText);
	void SetHeaderFontSize(int nSize); // default 16
	void SetRowFontSize(int nSize); // default 15
	void SetRowFontBold(UINT bBold); // FW_NORMAL, FW_BOLD, ....
	void SetHeaderFontBold(UINT bBold); // FW_NORMAL, FW_BOLD, ....
	void SetRowFontName(CString sName); // default 굴림
	void SetHeaderFontName(CString sName); // default 돋움
	void DelListItem(int nRowIndex);
	void InsertListItemUp(int nRowIndex);
	void InsertListItemDown(int nRowIndex);
	void SetReadOnly(bool bReadOnly);	//20221013 GBM - ReadOnly 속성이 되도록 플래그 추가
	void SetBackgroundColorAfterMaxCircuit(int nChartIndex);
private:
	void Redisplay();

private:
	CCustomHeader* m_pHeader;
	CCustomList* m_pList;
	CStringArray m_sHeaderList;
	CUIntArray m_nHeaderSize;
	CUIntArray m_nHeaderType;
	CUIntArray m_nAlign;
	CUIntArray m_nLimit;
	CList<pPopupList, pPopupList> m_popupList;
	CList<pStateString, pStateString> m_stateString;
	int m_nListIndex;
	int m_nListWidth;
	int m_nHeaderHeight;
	int m_nRowHeight;
	bool m_bUseTabkey;
	bool m_bFixHeaderSize;
	bool m_bHeaderAutoSize;
	bool m_bDisable;
	bool m_bLastInsertRow;
	bool m_bFirstInsertRow;
	int m_nListPopupWidth, m_nListPopupHeight;
	int m_nTextPopupWidth, m_nTextPopupHeight;
	COLORREF m_headerBk, m_headerText, m_headerLine;
	COLORREF m_rowBkNormal, m_rowBkSelect, m_rowLine;
	COLORREF m_rowTextNormal, m_rowTextSelect;
	CPopupTextDlg* m_pPopupTextDlg;
	CListPopupDlg* m_pPopupListDlg;
	int m_nFontSize, m_nHeaderFontSize;
	UINT m_nFontBold, m_nHeaderFontBold;
	CString m_sRowFontName, m_sHeaderFontName;
	bool m_bShowScrollbar;

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
	afx_msg LRESULT OnHeaderNotify(WPARAM, LPARAM);
	afx_msg LRESULT OnListNotify(WPARAM, LPARAM);
	afx_msg LRESULT OnListControl(WPARAM, LPARAM);
	afx_msg LRESULT OnListHorizon(WPARAM, LPARAM);
	afx_msg LRESULT OnSelectionNotify(WPARAM, LPARAM);
	afx_msg LRESULT OnListPageKey(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInputTextInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInputListInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
};


