#pragma once


// CTextTabBtn ��ȭ �����Դϴ�.

#define		TEXTTAB_BUTTON_CLICK		(WM_USER+5503)

class CTextTabBtn : public CDialogEx
{
	DECLARE_DYNAMIC(CTextTabBtn)

public:
	CTextTabBtn(int nIndex, CString sCaption, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTextTabBtn();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

public:
	void SetCheck(BOOL bTrue);
	BOOL GetCheck();
	void EnableWindowEx(BOOL bEnable);
	int  GetWidthExtent(CString sText);
	void SetCaption(CString sCaption);

private:
	void Redisplay();

private:
	CFont m_font, m_font1;
	CString m_sCaption;
	int m_nIndex;
	bool m_bEnable;
	bool m_bMouseDown;
	bool m_bMouseHover;
	bool m_bCheck;

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
	afx_msg void OnRadioClick();
	afx_msg void OnMouseLeave();
};
