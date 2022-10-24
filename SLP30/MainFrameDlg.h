#pragma once


// CMainFrameDlg ��ȭ �����Դϴ�.

#include "CircuitBasicInfoDlg.h"
#include "CircuitInfoDlg.h"
#include "StartProjectSelectionDlg.h"
#include "ImageCheckBtn.h"
#include "AutoCreateSetupDlg.h"
#include "MessagePopup.h"
#include "CircuitEditDlg.h"
#include "MakeDataBasicDlg.h"

class CMainFrameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainFrameDlg)

public:
	CMainFrameDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMainFrameDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

private:
	void Redisplay();
	void SetTabButton(int nIndex);

private:
	CCircuitBasicInfoDlg* m_pCircuitBasicDlg;
	CCircuitInfoDlg* m_pCircuitDlg;
	CStartProjectSelectionDlg* m_pSelectionDlg;
	CCircuitEditDlg* m_pCircuitEditDlg;
	//CAutoCreateSetupDlg* m_pSetupDlg;
	CMessagePopup* m_pWaitPopup;
	CMakeDataBasicDlg* m_pMakeDataDlg;
	CImageCheckBtn m_btnStart, m_btnCircuit, m_btnData;

	CString m_sBuildName;
	int m_nStair, m_nFloor, m_nBasement;

	bool m_bInitialize;

	CFont m_font;	//20221024 GBM - ���� �ǹ� / �߰�� ������ ��Ÿ���� ���� ���ڿ� ��Ʈ
	CString m_strCurrentConfigMsg;	//20221024 GBM - ���� �ǹ� / �߰�� ������ ��Ÿ���� ���� ���ڿ�

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnSelectionProject(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelectClick();
	afx_msg void OnCircuitClick();
	afx_msg void OnDataClick();
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
