#pragma once

// CStartProjectSelectionDlg 대화 상자입니다.

class CStartProjectSelectionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartProjectSelectionDlg)

public:
	CStartProjectSelectionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStartProjectSelectionDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

private:
	void Redisplay();

private:
	CFont m_font, m_font1;
	CImageBtn m_btnNew, m_btnLoad;
	bool m_bInit;
	bool m_bFirst;	// 기존 작업 존재시 초기화 안내 문구를 위한 플래그

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
	afx_msg void OnNewProjectClick();
	afx_msg void OnLoadProjectClick();
};
