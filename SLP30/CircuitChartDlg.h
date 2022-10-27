#pragma once

#include "CustomListCtrl.h"

// CCircuitChartDlg 대화 상자입니다.

class CCircuitChartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCircuitChartDlg)

public:
	CCircuitChartDlg(CWnd* pParent = NULL, int nCircuit = 0);   // 표준 생성자입니다.
	virtual ~CCircuitChartDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

private:
	int m_nCircuit;

public:
	bool DisplayListItem(int nChartIndex);
	void SettingChartNumber();
	void DisplayLoadFile(int nChartIndex);
	bool SaveInformation(int nChartIndex);

private:
	void Redisplay();
	void SetupPopupList();

private:
	CCustomListCtrl* m_pListCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnListControl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPopupSelect(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
