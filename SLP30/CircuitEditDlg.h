#pragma once

#include "TextTabBtn.h"
#include "CustomListCtrl.h"
#include "CircuitChartDlg.h"
#include "PSPumpSetupDlg.h"
#include "BroadcastDlg.h"

#include <thread>

// CCircuitEditDlg ��ȭ �����Դϴ�.

class CCircuitEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCircuitEditDlg)

public:
	CCircuitEditDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCircuitEditDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

public:
	int  DisplayListItem();
	bool LoadInfo();
	void SetExcelSaveFlag(bool bFlag);

private:
	void Redisplay();
	int  FileSave();
	int  ExcelSave();
	void MakePattern();

private:
	bool m_bInit;
	CImageBtn m_btnNext, m_btnPrev, m_btnSave, m_btnExcel;
	CFont m_font;
	CTextTabBtn* m_pTabButton[4];
	CCircuitChartDlg* m_pCircuitChartDlg[2];
	CPSPumpSetupDlg* m_pPSPumpDlg;
	CBroadcastDlg* m_pBroadcastDlg;
	thread* m_pThread;
	bool m_bExcelSaved;		// �߰�� �϶�ǥ�� �ѹ��� �����ؾ߸� ���������� ������ �Ѿ���� �ϴ� �÷���

public:
	CString m_sExcelPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnTabButtnClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExcelSaveMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnNextClick();
	afx_msg void OnPrevClick();
	afx_msg void OnSaveClick();
	afx_msg void OnExcelClick();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
