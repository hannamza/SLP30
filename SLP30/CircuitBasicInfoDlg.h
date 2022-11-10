#pragma once

#include "CustomListCtrl.h"
#include "CheckTextBtn.h"
#include "ImageBtn.h"

// CCircuitBasicInfoDlg 대화 상자입니다.

class CCircuitBasicInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCircuitBasicInfoDlg)

public:
	CCircuitBasicInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCircuitBasicInfoDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

public:
	void InfoInit();
	void LoadInfo();
	void GetCircuitInfo(CString & sName, int & nStair, int & nFloor, int & nBasement);
	bool IsBuildingInfoComplete();
	void LoadCheckInfoOnly();

private:
	void Redisplay();

private:
	CCustomListCtrl* m_pListCtrl;
	CFont m_font;
	CCheckTextBtn* m_pCheck[CIRCUIT_PARENT];
	CImageBtn m_btnNext;
	bool m_bFirstWork;
	bool m_bInit;
	bool m_bBuildingInfoComplate;		//20221013 GBM - 최초 건물 정보는 한번 세팅되면 변경할 수 없도록 수정하기 위한 플래그

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
	afx_msg void OnNextClick();
};
