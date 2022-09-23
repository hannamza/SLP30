#pragma once
#include "afxwin.h"

typedef BOOL(WINAPI *SetLayer)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
#define LWA_COLORKEY            0x01
#define LWA_ALPHA               0x02

// CLoginDlg ��ȭ �����Դϴ�.

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLoginDlg();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

public:
	CString m_sID, m_sPW;
	CBrush m_brush;

private:
	void Redisplay();

	void n_SetDlgAlpha(int chAlpha);     // ���̾�α� ���� ����
	void RegisterForDeviceNotifications();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConfirmClick();
	afx_msg void OnCancelClick();
	afx_msg void OnFindClick();

	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_editID;
	CEdit m_editPW;
	CImageBtn m_btnConfirm;
	CImageBtn m_btnCancel;
	CImageBtn m_btnFind;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
};
