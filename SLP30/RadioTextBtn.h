#pragma once


// CRadioTextBtn 대화 상자입니다.

#define		RADIO_BUTTON_CLICK		(WM_USER+5501)

class CRadioTextBtn : public CDialogEx
{
	DECLARE_DYNAMIC(CRadioTextBtn)

public:
	CRadioTextBtn(int nIndex, CString sCaption, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRadioTextBtn();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

public:
	void SetCheck(BOOL bTrue);
	BOOL GetCheck();
	void EnableWindowEx(BOOL bEnable);
	int  GetWidthExtent(CString sText);

private:
	void Redisplay();

private:
	CFont m_font;
	CImageCheckBtn m_button;
	CString m_sCaption;
	int m_nIndex;
	bool m_bEnable;
	bool m_bMouseDown;

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
	afx_msg void OnRadioClick();
};
