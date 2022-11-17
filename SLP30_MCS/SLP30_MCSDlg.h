
// SLP30_MCSDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define		LIMIT_RICH_LOG		(1024*1024*2)

// CSLP30_MCSDlg ��ȭ ����
class CSLP30_MCSDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CSLP30_MCSDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GFSM_MCS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

protected:
	void WriteFunc(CString strText);
	BOOL WriteLog(INT nMode, LPTSTR data, ...);

private:
	int m_nServerPort;
	int m_nLimitClient;
	BOOL m_bStartServer;


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnLogMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonStart();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStop();
	CRichEditCtrl m_richEdit;
	CButton m_checkScroll;
};
