#pragma once


// CCircuitInfoDlg 대화 상자입니다.
#include "TextTabBtn.h"
#include "CircuitSetupDlg.h"
#include "CustomListCtrl.h"

#define		MAKE_CIRCUIT_INFO		(WM_USER+6601)

class CCircuitInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCircuitInfoDlg)

public:
	CCircuitInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCircuitInfoDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

public:
	void InitCircuitInfo();
	void LoadInfo();

private:
	void Redisplay();
	int  GetCircuitCount(int nChartIndex);
	bool CheckCircuitCount();

private:
	CTextTabBtn* m_pTabButton[2];
	CCircuitSetupDlg* m_pSetupDlg[2];
	CImageBtn m_btnNext, m_btnPrev;
	CCustomListCtrl* m_pListCtrl[6];

	bool m_bInit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnTabButtnClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResetValue(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNextClick();
	afx_msg void OnPrevClick();
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
