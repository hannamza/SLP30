#pragma once
#include "afxwin.h"
#include "CustomComboBox.h"
#include "ImageBtn.h"

// CSecurityUsbDlg ��ȭ �����Դϴ�.

class CSecurityUsbDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSecurityUsbDlg)

public:
	CSecurityUsbDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSecurityUsbDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SECURITY };
#endif
public:
	CString GetDriveLetter();

private:
	void Redisplay();

private:
	CFont m_font;
	CString m_sDriveLetter;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConfirmClick();
	afx_msg void OnCancelClick();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
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
	CCustomComboBox m_comboUsb;
	CImageBtn m_btnConfirm;
	CImageBtn m_btnCancel;
};
