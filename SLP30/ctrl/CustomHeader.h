#pragma once


// CCustomHeader 대화 상자입니다.
#include <afxtempl.h>

#define		RESIZE_NOTIFY	WM_USER+9501
#define		HEADER_RESIZE	0
#define		HEADER_DBCLICK	1
#define		HEADER_CLICK	2

#define		HEADER_HEIGHT_SIZE		50

class CCustomHeader : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomHeader)

public:
	CCustomHeader(CWnd* pParent);   // 표준 생성자입니다.
	virtual ~CCustomHeader();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

private:
	void Redisplay();
	void ReleaseRectList();
	void CheckHeaderWidth();

public:
	void SetHeaderInfo(CStringArray & arrayName, CUIntArray & arraySize);
	void ResizeHeaderRect();
	int  ResizeCheckHeader(CPoint & point, int nType = 0);
	int  GetHeaderWidth();
	void SetCurrentHPosition(int nPosition);
	void GetHeaderSize(CUIntArray & arraySize);
	void GetHeaderSizeEx(CUIntArray & arraySize);
	void SetHeaderColor(COLORREF bkColor, COLORREF lineColor, COLORREF textColor);
	void SetFontSize(int nSize);
	void SetFontBold(UINT nValue);
	void SetFontName(CString sName);
	void FixHeaderSize(bool bFix);
	void AutoHeaderSize(bool bAuto);

private:
	CStringArray m_sHeaderList;
	CUIntArray m_nHeaderSize;
	CList<CRect*, CRect*> m_headerRect;
	bool m_bMouseDown;
	int m_nSelectHeader;
	CWnd* m_pParent;
	int m_nHPosition, m_nOldHPosition;
	CFont m_font;
	CPoint m_point;
	COLORREF m_colorBk, m_colorText, m_colorLine;
	bool m_bFixHeaderSize;
	bool m_bAutoHeaderSize;
	int m_nFontSize;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnMove(int x, int y);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
