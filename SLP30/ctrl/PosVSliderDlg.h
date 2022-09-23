#pragma once

#define		SLIDER_INFO		(WM_USER+1003)
// CPosVSliderDlg ��ȭ �����Դϴ�.

class CPosVSliderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPosVSliderDlg)

public:
	CPosVSliderDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPosVSliderDlg();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHILD_DIALOG };
#endif

public:
	void SetValue(INT nRange, INT nHeight, INT nPos);

protected:
	INT		m_nRange;	// �� ��ũ�� ������
	INT		m_nHeight;	// �� ȭ�� ����
	INT		m_nPos;		// ���� ��ũ��

	INT		m_nMiddle;	// �̵� �� ������

	INT		m_nTop;
	double	m_dRatio;
	INT		m_nDownPos;

	BOOL	m_bMouseDown;
	CCriticalSectionEx m_sync;

protected:
	void CalculateValue(INT nDownPos, INT nCurrentPos, INT nType=0);
	void Redisplay();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
