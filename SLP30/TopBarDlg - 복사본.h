#pragma once


// CTopBarDlg ��ȭ �����Դϴ�.

class CTopBarDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTopBarDlg)

public:
	CTopBarDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTopBarDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOM_ITEM_DIALOG };
#endif

private:
	void Redisplay();

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
};
