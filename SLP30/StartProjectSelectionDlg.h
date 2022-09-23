#pragma once

// CStartProjectSelectionDlg ��ȭ �����Դϴ�.

class CStartProjectSelectionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartProjectSelectionDlg)

public:
	CStartProjectSelectionDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStartProjectSelectionDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

private:
	void Redisplay();

private:
	CFont m_font, m_font1;
	CImageBtn m_btnNew, m_btnLoad;
	bool m_bInit;
	bool m_bFirst;	// ���� �۾� ����� �ʱ�ȭ �ȳ� ������ ���� �÷���

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
	afx_msg void OnNewProjectClick();
	afx_msg void OnLoadProjectClick();
};
