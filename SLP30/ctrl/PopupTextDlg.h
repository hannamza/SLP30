#pragma once
#include "afxwin.h"


// CPopupTextDlg 대화 상자입니다.

#define		INPUT_TEXT_INFO		WM_USER+9510

class CPopupTextDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPopupTextDlg)

public:
	CPopupTextDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPopupTextDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXT_POPUP_DIALOG };
#endif

public:
	void SetItemIndexInfo(int nItemIndex, int nSelCol);
	CString GetInputText();
	void SetInputText(CString sText);
	void SetTextLimit(int nSize);

private:
	void Redisplay();
	void SetHanMode(HWND hWnd);

private:
	CWnd* m_pParent;
	CString m_sText;
	int m_nItemIndex, m_nSelCol;
	int m_nLimit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_editText;
};
