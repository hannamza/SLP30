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
	//20221013 GBM start - 계통 정보를 넘겨서 기존 설비 개수 정보는 가져오고 새로 추가된 종류의 설비는 0으로 초기화
#if 1
	void InitCircuitInfo(int nSystem);
#else
	void InitCircuitInfo();
#endif
	//20221013 GBM end
	bool SaveCircuitInfo(int nIndex);
	bool CompareNewCircuitInfo(int nSystem);		// 설비 표 내용과 현재 설비 메모리 비교 -> 이전 방식
	bool CompareNewCircuitInfoNeo(int nSystem);		// 설비 표 내용과 현재 확정된 중계기 일람표 기준 메모리 비교 -> 새 방식

	void CopyNewCircuitInfoToOldCircuitInfo(int nSystem, bool bFileLoad = false);	//20221014 GBM - 중계기 일람표 화면으로 넘어가기 전에 기존 설비 정보를 복사해둬 중계기 일람표 상에서 설비 정보의 변경에 따라 추가/삭제를 위한 기반 정보를 만든다.
	void MakeAddCircuitInfoAndDeleteCircuitInfo(int nSystem);
	bool CheckMaxCircuitCount(int nSystem, int* nTotalCount = 0);
	void RollbackCircuitCount(int nSystem);
	void BackupCircuitInfo(int nSystem);
	void BackupCircuitCountInfo(int nSystem);
	void CopyPreviousCircuitInfo(int nSystem);
	void CopyPreviousCircuitCompInfo(int nSystem);

private:
	CCustomListCtrl* CreateListCtrl();
	void MakeHeader(CStringArray & arrayList);
	void MakeHeaderWidth(CUIntArray & arrayList);
	void MakeHeaderType(CUIntArray & arrayList);
	void MakeHeaderAlign(CUIntArray & arrayList);
	void MakeHeaderLimit(CUIntArray & arrayList);
	CCustomListCtrl* NewCircuitListCtrl(CCustomListCtrl* pListCtrl);
	void FillDataInCircuitListCtrl(int nSystem);		
	bool CompareCircuit(pSelectCircuit pCircuit1, pSelectCircuit pCircuit2);
	bool CompareCircuit(selectCircuit sc1, selectCircuit sc2);
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
