#pragma once

#include "stdafx.h"
#include "SLP30.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"
#include <dbt.h>
#include "afxwin.h"

// CCircuitNoMove 대화 상자입니다.

class CCircuitNoMoveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCircuitNoMoveDlg)

public:
	CCircuitNoMoveDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCircuitNoMoveDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CIRCUIT_NO_CHANGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
