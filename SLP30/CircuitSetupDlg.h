#pragma once


// CCircuitSetupDlg 대화 상자입니다.
#include "TextTabBtn.h"
#include "CustomListCtrl.h"

#define		STAIR_TAB_COUNT		9

class CCircuitSetupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCircuitSetupDlg)

public:
	CCircuitSetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCircuitSetupDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMON_CHILD_DIALOG };
#endif

public:
	bool LoadInfo(int nSystem);
	void InitCircuitInfo();
	bool SaveCircuitInfo(int nIndex);
	bool CompareNewCircuitInfo(int nSystem);

private:
	CCustomListCtrl* CreateListCtrl();
	void MakeHeader(CStringArray & arrayList);
	void MakeHeaderWidth(CUIntArray & arrayList);
	void MakeHeaderType(CUIntArray & arrayList);
	void MakeHeaderAlign(CUIntArray & arrayList);
	void MakeHeaderLimit(CUIntArray & arrayList);
	CCustomListCtrl* NewCircuitListCtrl(CCustomListCtrl* pListCtrl);
	bool CompareCircuit(pSelectCircuit pCircuit1, pSelectCircuit pCircuit2);
	void Redisplay();

	CCustomListCtrl* GetStairValue(CString sBlock, CString sStair);
	int GetFloorValue(CCustomListCtrl* p_List, CString sFloor);
	int GetCircuitValue(CString sCircuit);

private:
	CTextTabBtn* m_pTabButton[STAIR_TAB_COUNT];
	bool m_bInit;
	CCustomListCtrl* m_p_List[STAIR_TAB_COUNT];

	CStringArray m_arrayHeaderName;
	CUIntArray m_arrayHeaderWidth;
	CUIntArray m_arrayHeaderType;
	CUIntArray m_arrayHeaderAlign;
	CUIntArray m_arrayHeaderLimit;

	CCriticalSectionEx m_sync;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnTabButtnClick(WPARAM wParam, LPARAM lParam);
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
