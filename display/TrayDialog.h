#if !defined(AFX_TRAYDIALOG_H__24C2AEA9_B40D_11D4_BFA7_00C0DF034AED__INCLUDED_)
#define AFX_TRAYDIALOG_H__24C2AEA9_B40D_11D4_BFA7_00C0DF034AED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrayDialog.h : header file
//

// Ʈ���̷� ����ϱ� ���ϰ� ���� CDialogExEx ��� ���� Ŭ����(CDialogEx�� ���� ����)
// CDialogEx, CDialogExEx�� ��� ���� ���̾�α� Ŭ���� ��ſ� ����Ͽ� Ʈ���̸� ���� ��� ����

/////////////////////////////////////////////////////////////////////////////
// CTrayDialog dialog


class CTrayDialog : public CDialogEx
{

private:
	BOOL m_bMinimizeToTray;

	BOOL			m_bTrayIconVisible; // Ʈ���� ���¸� ����
	NOTIFYICONDATA	m_nidIconData;		// ������ ������
	CMenu			m_mnuTrayMenu;		// Ʈ���� �޴�
protected:
	BOOL			m_bVisible;
	// Construction
public:
	// �޴� ���ҽ� ����
	BOOL TraySetMenu(UINT nResourceID, UINT nDefaultPos = 0);
	BOOL TraySetMenu(HMENU hMenu,UINT nDefaultPos=0);
	BOOL TraySetMenu(LPCTSTR lpszMenuName,UINT nDefaultPos=0);
	// Ʈ���� ������, ���� ���� ������Ʈ
	BOOL TrayUpdate();
	// Ʈ���� ���·�
	BOOL TrayShow();
	// Ʈ���̿��� ����
	BOOL TrayHide();
	// Ʈ���� ���·� ���̾�α׸� �˾�
	BOOL ShowDialog();
	// ���̾�α� �����
	BOOL HIdeDialog();
	// Ʈ���� ���� ����
	void TraySetToolTip(LPCTSTR lpszToolTip);
	// Ʈ���� ������ ���ҽ� ����
	void TraySetIcon(HICON hIcon);
	void TraySetIcon(UINT nResourceID);
	void TraySetIcon(LPCTSTR lpszResourceName);
	// Ʈ���� ���� ���� ��������
	BOOL TrayIsVisible();
	CTrayDialog(UINT uIDD,CWnd* pParent = NULL);   // standard constructor
	
	virtual void OnTrayLButtonDown(CPoint pt);
	virtual void OnTrayLButtonDblClk(CPoint pt);
	
	virtual void OnTrayRButtonDown(CPoint pt);
	virtual void OnTrayRButtonDblClk(CPoint pt);

	virtual void OnTrayMouseMove(CPoint pt);
	

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CTrayDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);	
	private:
//	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnTrayIconNotifyMessage(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAYDIALOG_H__24C2AEA9_B40D_11D4_BFA7_00C0DF034AED__INCLUDED_)