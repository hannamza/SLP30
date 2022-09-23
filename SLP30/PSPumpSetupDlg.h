#pragma once


// CPSPumpSetupDlg ��ȭ �����Դϴ�.

#include "CustomListCtrl.h"

class CPSPumpSetupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPSPumpSetupDlg)

public:
	CPSPumpSetupDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPSPumpSetupDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

public:
	CCustomListCtrl* m_pPSCtrl;
	CCustomListCtrl* m_pPumpCtrl;
	CFont m_font;

public:
	void DisplayListItem();
	void DisplayLoadFile();
	bool SaveInformation();

private:
	void Redisplay();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
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
