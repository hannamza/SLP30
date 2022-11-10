#pragma once

#include "CustomListCtrl.h"
#include "CheckTextBtn.h"
#include "ImageBtn.h"

// CCircuitBasicInfoDlg ��ȭ �����Դϴ�.

class CCircuitBasicInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCircuitBasicInfoDlg)

public:
	CCircuitBasicInfoDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCircuitBasicInfoDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

public:
	void InfoInit();
	void LoadInfo();
	void GetCircuitInfo(CString & sName, int & nStair, int & nFloor, int & nBasement);
	bool IsBuildingInfoComplete();
	void LoadCheckInfoOnly();

private:
	void Redisplay();

private:
	CCustomListCtrl* m_pListCtrl;
	CFont m_font;
	CCheckTextBtn* m_pCheck[CIRCUIT_PARENT];
	CImageBtn m_btnNext;
	bool m_bFirstWork;
	bool m_bInit;
	bool m_bBuildingInfoComplate;		//20221013 GBM - ���� �ǹ� ������ �ѹ� ���õǸ� ������ �� ������ �����ϱ� ���� �÷���

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
	afx_msg void OnNextClick();
};
