#pragma once

#include <afxtempl.h>
#include "ListItem.h"
#include "PosVSliderDlg.h"
#include "ItemManager.h"

// CPopupListCtrl 뷰입니다.

class CPopupListCtrl : public CScrollView
{
	DECLARE_DYNCREATE(CPopupListCtrl)

protected:
	CPopupListCtrl();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CPopupListCtrl();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	INT		m_nScrollY, m_nOldScrollY;
	INT		m_nScrollX, m_nOldScrollX;
	INT		m_Range;

	CFont	m_font;
	CString	m_sSearch;

	INT		m_nShowIndex;
	INT		m_nPos, m_nScrollPos;

	CPosVSliderDlg* m_pSlider;

	CItemManager* m_pItemManager;

	CCriticalSectionEx m_listSync, m_displaySync;

	CList<CDialogEx*, CDialogEx*> m_listObject;
	CUIntArray m_displayList;

protected:
	void Redisplay();
	void SetScrollSize();
	void RedisplayObject(INT nIndex);
	void ControlSlider(int nSign);
	void ReleaseWindow();

public:
	void SetSearchWord(CString sSearch);
	void SetSelectedItem(int nIndex);
	int  GetSelectedItemSeq();
	int  GetSelectedItemIndex();

	void AddItem(int nSeq, CString sText, LPVOID lpData = NULL);
	bool InsertItem(int nIndex, int nSeq, CString sText, LPVOID lpData = NULL);
	bool RemoveItem(int nIndex);
	int  GetCount();
	pListItem GetItemInfo(int nIndex);

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnLObjectClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRObjectClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSliderInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


