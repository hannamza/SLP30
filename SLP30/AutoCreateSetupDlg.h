#pragma once

#include "CustomListCtrl.h"
#include "RadioTextBtn.h"

// CAutoCreateSetupDlg 대화 상자입니다.

class CAutoCreateSetupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoCreateSetupDlg)

public:
	CAutoCreateSetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoCreateSetupDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

private:
	void Redisplay();

private:
	CCustomListCtrl* m_pListCtrl;
	CRadioTextBtn* m_pRadioBtn1, *m_pRadioBtn2, *m_pRadioBtn3, *m_pRadioBtn4;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnRadioButtnClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListControl(WPARAM, LPARAM);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
