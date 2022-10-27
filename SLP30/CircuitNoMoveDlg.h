#pragma once

#include "stdafx.h"
#include "SLP30.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"
#include <dbt.h>
#include "afxwin.h"

// CCircuitNoMove ��ȭ �����Դϴ�.

class CCircuitNoMoveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCircuitNoMoveDlg)

public:
	CCircuitNoMoveDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCircuitNoMoveDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CIRCUIT_NO_CHANGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctrlEditNewCircuitNo;
	CBrush m_brush;
	CString m_strNewCircuitNo;
	CImageBtn m_btnConfirm;
	CImageBtn m_btnCancel;
	int m_nNewCircuitNum;
	CFont m_font;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnConfirmClick();
	afx_msg void OnCancelClick();
	void Redisplay();
};
