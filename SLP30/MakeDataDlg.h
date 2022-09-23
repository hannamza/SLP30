#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CreateProcessReturnValue.h"


// CMakeDataDlg ��ȭ �����Դϴ�.

class CMakeDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMakeDataDlg)

public:
	CMakeDataDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMakeDataDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DATA };
#endif

private:
	void Redisplay();

private:
	CFont m_font;
	CString m_sPath;
	bool m_bInit;
	CImageBtn m_btnFolder, m_btnMakeData;
	CImageBtn m_btnNext, m_btnPrev, m_btnExcel;
	thread* m_pThread;
	CCreateProcessReturnValue m_createProcess;

public:
	CString m_sExcelPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnExcelSaveMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRomSaveMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFolderClick();
	afx_msg void OnMakeDataClick();
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
	//afx_msg void OnNextClick();
	afx_msg void OnPrevClick();
	afx_msg void OnExcelClick();
	CEdit m_editPath;
	CProgressCtrl m_prgCreate;
};
