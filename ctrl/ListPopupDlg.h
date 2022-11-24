#pragma once
#include "afxwin.h"
#include "PopupListCtrl.h"

#define		INPUT_LIST_INFO		WM_USER+9516
// CListPopupDlg 대화 상자입니다.

typedef struct SEND_POINT
{
	int nFlag;
	CPoint point;
}sendPoint;

typedef struct POPUP_LIST
{
	int nColumn;
	CStringArray arrayList;
}popupList, *pPopupList;

class CListPopupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListPopupDlg)

public:
	CListPopupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CListPopupDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIST_POPUP_DIALOG };
#endif

public:
	void SetItemIndexInfo(int nItemIndex, int nSelCol);
	CString GetInputText();
	CString GetSelectText();
	void AddPopupItem(CString sText);
	void SetTextLimit(int nSize);

private:
	void Redisplay();
	void SetHanMode(HWND hWnd);
	void SetInputEditText(CString sText);

private:
	CString m_sText, m_sSelText;
	CWnd* m_pParent;
	int m_nItemIndex, m_nSelCol;
	CPopupListCtrl* m_pListCtrl;
	sendPoint m_sendPoint[100];
	CStringArray m_arrayList;
	int m_nPointIndex;
	int m_nLimit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_editText;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEnChangeEditText();
	afx_msg LRESULT OnSelectItem(WPARAM wParam, LPARAM lParam);
};
