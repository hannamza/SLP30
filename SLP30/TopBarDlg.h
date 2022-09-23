#pragma once


// CTopBarDlg 대화 상자입니다.

class CTopBarDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTopBarDlg)

public:
	CTopBarDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTopBarDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

private:
	bool m_bInit;

private:
	void Redisplay();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	afx_msg void OnCloseClick();
	afx_msg void OnMaximizeClick();
	afx_msg void OnMinimizeClick();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	CImageBtn m_btnClose;
	CImageBtn m_btnMaximize;
	CImageBtn m_btnMinimize;
};
