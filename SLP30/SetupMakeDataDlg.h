#pragma once

#include "CheckTextBtn.h"

// CSetupMakeDataDlg 대화 상자입니다.

class CSetupMakeDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMakeDataDlg)

public:
	CSetupMakeDataDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMakeDataDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

public:
	void SaveData();
	void LoadData();
	void InitData();

private:
	void Redisplay();

private:
	CFont m_font;
	bool m_bInit;
	CCheckTextBtn* m_pCheck[CHECK_COUNT];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT ChangeCheckButtonState(WPARAM wParam, LPARAM lParam);
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
