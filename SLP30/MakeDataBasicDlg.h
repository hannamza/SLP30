#pragma once

#include "MakeDataDlg.h"
#include "SetupMakeDataDlg.h"

// CMakeDataBasicDlg ��ȭ �����Դϴ�.

class CMakeDataBasicDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMakeDataBasicDlg)

public:
	CMakeDataBasicDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMakeDataBasicDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

public:
	void CallFirstDisplay();

private:
	void Redisplay();

private:
	CMakeDataDlg* m_pMakeData;
	CSetupMakeDataDlg* m_pSetupData;
	CImageBtn m_btnNext, m_btnPrev, m_btnSave;
	int m_nDisplay;

	bool m_bInit;
	CFont m_font;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnChangePage(WPARAM wParam, LPARAM lParam);
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
	afx_msg void OnNextClick();
	afx_msg void OnPrevClick();
	afx_msg void OnSaveClick();
};
