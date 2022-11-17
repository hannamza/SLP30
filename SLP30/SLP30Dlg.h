
// SLP30Dlg.h : 헤더 파일
//

#pragma once

#include "CustomListCtrl.h"
#include "TopBarDlg.h"
#include "LoginDlg.h"
#include "StartProjectSelectionDlg.h"
#include "AutoCreateSetupDlg.h"
#include "MainFrameDlg.h"

#define SLP30DLG_WIDTH	1024
#define SLP30DLG_HEIGHT	810

// CSLP30Dlg 대화 상자
class CSLP30Dlg : public CDialogEx
{
// 생성입니다.
public:
	CSLP30Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SLP30_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	CCustomListCtrl* m_pListCtrl;
	CTopBarDlg* m_pTopBarDlg;
	CMainFrameDlg* m_pMainFrameDlg;

private:
	void Redisplay();
	void DialogResizing(UINT nFlags, CPoint point);

protected:
	int m_nDisplay;

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnSocketRecvMsg(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
