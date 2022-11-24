#pragma once

#include <afxtempl.h>
#include "ItemManager.h"
// CListItem 대화 상자입니다.

#define		WM_LBUTTON_BUTTONLIST_CLICK		(WM_USER+1004)
#define		WM_RBUTTON_BUTTONLIST_CLICK		(WM_USER+1005)

class CListItem : public CDialogEx
{
	DECLARE_DYNAMIC(CListItem)

public:
	CListItem(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CListItem();

public:
	void SetListItem(pListItem pSite);
	void SetItemIndex(int nIndex);
	int  GetItemIndex();
	void SetSelectedState(bool bValue);
	bool GetSelectedState();
	bool FindSearchWord(CString sValue);

private:
	void Redisplay();

private:
	CFont	m_font, m_font2, m_font3;
	int		m_nIndex;
	bool	m_bMouseRbtn;
	bool	m_bMouseLbtn;

	bool	m_bMouseOn;
	bool	m_bSelected;

	CWnd*	m_pWnd;

	pListItem m_pSite;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};
