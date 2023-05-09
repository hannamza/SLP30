// CircuitInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CircuitInfoDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CCircuitInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCircuitInfoDlg, CDialogEx)

CCircuitInfoDlg::CCircuitInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_pTabButton[0] = NULL;
	m_pTabButton[1] = NULL;

	m_pSetupDlg[0] = NULL;
	m_pSetupDlg[1] = NULL;

	m_bInit = false;
}

CCircuitInfoDlg::~CCircuitInfoDlg()
{
	SAFE_DELETE(m_pTabButton[0]);
	SAFE_DELETE(m_pTabButton[1]);

	SAFE_DELETE(m_pSetupDlg[0]);
	SAFE_DELETE(m_pSetupDlg[1]);
}

void CCircuitInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircuitInfoDlg, CDialogEx)
	ON_MESSAGE(TEXTTAB_BUTTON_CLICK, OnTabButtnClick)
	ON_MESSAGE(MAKE_CIRCUIT_INFO, OnResetValue)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnNextClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnPrevClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(IDC_COMMON_BUTTON3, OnCircuitListInit)
END_MESSAGE_MAP()

// CCircuitInfoDlg 메시지 처리기입니다.

BOOL CCircuitInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
#ifndef ENGLISH_MODE
	m_pTabButton[0] = new CTextTabBtn(0, L"0 계통", this);
#else
	m_pTabButton[0] = new CTextTabBtn(0, L"LOOP 0", this);
#endif
	m_pTabButton[0]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[0]->MoveWindow(20, 14, 89, 37);

	m_pTabButton[0]->SetCheck(true);

#ifndef ENGLISH_MODE
	m_pTabButton[1] = new CTextTabBtn(1, L"1 계통", this);
#else
	m_pTabButton[1] = new CTextTabBtn(1, L"LOOP 1", this);
#endif
	m_pTabButton[1]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[1]->MoveWindow(115, 14, 89, 37);

	m_pSetupDlg[0] = new CCircuitSetupDlg(this);
	m_pSetupDlg[0]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pSetupDlg[0]->ShowWindow(SW_SHOW);

	m_pSetupDlg[1] = new CCircuitSetupDlg(this);
	m_pSetupDlg[1]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pSetupDlg[1]->ShowWindow(SW_HIDE);

#ifndef ENGLISH_MODE
	m_btnNext.Create(IDB_BMP_NEXT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnPrev.Create(IDB_BMP_PREV, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnCircuitListInit.Create(IDB_BMP_CIRCUIT_LIST_INIT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
#else
	m_btnNext.Create(IDB_BMP_NEXT_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnPrev.Create(IDB_BMP_PREV_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnCircuitListInit.Create(IDB_BMP_CIRCUIT_LIST_INIT_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
#endif

	m_btnCircuitListInit.ShowWindow(FALSE);

	m_bInit = true;

	//for (int nIndex = 0; nIndex < 6; nIndex++) {
	//	CRuntimeClass	*pRuntimeClass;
	//	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomListCtrl))) {
	//		if (NULL != (m_pListCtrl[nIndex] = (CCustomListCtrl *)(pRuntimeClass->CreateObject()))) {
	//			m_pListCtrl[nIndex]->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
	//		}
	//	}
	//	m_pListCtrl[nIndex]->SetListIndex(0);
	//	m_pListCtrl[nIndex]->SetHeaderInfo((TCHAR**)lpszHeaderBasic, (int*)colWidthListBasic, (int*)colTypeBasic, (int*)colAlignBasic); // Align: option, row align text, default: DT_LEFT
	//	m_pListCtrl[nIndex]->SetColumnLimit((int*)colLimitBasic);
	//	// 높이 변경 (option)
	//	m_pListCtrl[nIndex]->SetHeaderHeight(40);
	//	m_pListCtrl[nIndex]->SetRowHeight(30);
	//	//m_pListCtrl[nIndex]->SetListPopupSize(500, 500);
	//	// 색변경 (option)
	//	m_pListCtrl[nIndex]->SetHeaderBkColor(RGB(80, 80, 80), RGB(255, 255, 255));
	//	m_pListCtrl[nIndex]->SetHeaderTextColor(RGB(220, 220, 220));
	//	m_pListCtrl[nIndex]->SetRowBkColor(RGB(160, 160, 160), RGB(255, 255, 0), RGB(255, 255, 255));
	//	m_pListCtrl[nIndex]->SetRowTextColor(RGB(255, 255, 255), RGB(80, 80, 80));
	//	m_pListCtrl[nIndex]->SetHeaderFontSize(15);
	//	m_pListCtrl[nIndex]->FixHeaderSize(false);

	//	m_pListCtrl[nIndex]->SendMessage(WM_INITIALUPDATE);
	//}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCircuitInfoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CCircuitInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CCircuitInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCircuitInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircuitInfoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircuitInfoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCircuitInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	m_pTabButton[0]->Invalidate();
	m_pTabButton[1]->Invalidate();

	m_pSetupDlg[0]->Invalidate();
	m_pSetupDlg[1]->Invalidate();
}

void CCircuitInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Redisplay();
}

void CCircuitInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CCircuitInfoDlg::OnResetValue(WPARAM wParam, LPARAM lParam)
{
	Sleep(100);
	CCommonState::ie()->m_bWaitCircuit = false;
	return 0;
}

void CCircuitInfoDlg::InitCircuitInfo()
{
	//20221013 GBM start - 계통 정보를 넘겨서 기존 설비 개수 정보는 가져오고 새로 추가된 종류의 설비는 0으로 초기화
#if 1
	m_pSetupDlg[0]->InitCircuitInfo(0);
	m_pSetupDlg[1]->InitCircuitInfo(1);
#else
	m_pSetupDlg[0]->InitCircuitInfo();
	m_pSetupDlg[1]->InitCircuitInfo();
#endif
	//20221013 GBM end
	m_pSetupDlg[0]->ShowWindow(SW_SHOW);
	m_pSetupDlg[1]->ShowWindow(SW_HIDE);
	PostMessage(TEXTTAB_BUTTON_CLICK, 0, 0);
	PostMessage(MAKE_CIRCUIT_INFO, 0, 0);
}

int CCircuitInfoDlg::GetCircuitCount(int nChartIndex)
{
	pSelectCircuit pCircuit = NULL;
	int nTotal = 0;
	int nCount = 0;
	if (nChartIndex == 0) {
		nCount = CCommonState::ie()->m_selectCircuit_0.GetCount();
	}
	else if(nChartIndex == 1) {
		nCount = CCommonState::ie()->m_selectCircuit_1.GetCount();
	}
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		if (nChartIndex == 0) {
			pCircuit = CCommonState::ie()->m_selectCircuit_0.GetAt(CCommonState::ie()->m_selectCircuit_0.FindIndex(nIndex));
		}
		else if (nChartIndex == 1) {
			pCircuit = CCommonState::ie()->m_selectCircuit_1.GetAt(CCommonState::ie()->m_selectCircuit_1.FindIndex(nIndex));
		}
		nTotal += pCircuit->nCount;
	}
	return nTotal;
}

bool CCircuitInfoDlg::CheckCircuitCount()
{
	if (!m_pSetupDlg[0] || !m_pSetupDlg[1]) {
		return false;
	}
	bool bResult1 = m_pSetupDlg[0]->SaveCircuitInfo(0);
	bool bResult2 = m_pSetupDlg[1]->SaveCircuitInfo(1);
	/*if (!bResult1 || !bResult2) {
		CString sText;
		sText.Format(L"\n\n%d계통 최대 입력 개수(250개)를 초과하였습니다.\n\n다시 입력하여 주시기 바랍니다.", (!bResult1) ? 0 : 1);
		CMessagePopup popup(L"중계기 일람표", sText, MB_OK, this);
		UINT nResult = popup.DoModal();
		CCommonState::ie()->ReleaseCircuit(0);
		CCommonState::ie()->ReleaseCircuit(1);
		return false;
	}*/

	/*int nCount1 = GetCircuitCount(0);
	int nCount2 = GetCircuitCount(1);
	if (nCount1 > MAX_CIRCUIT || nCount2 > MAX_CIRCUIT) {
		CString sText;
		sText.Format(L"\n\n%d계통 최대 입력 개수(250개)를 초과하였습니다.\n\n다시 입력하여 주시기 바랍니다.", (nCount1 > 250) ? 0 : 1);
		CMessagePopup popup(L"중계기 일람표", sText, MB_OK, this);
		UINT nResult = popup.DoModal();
		CCommonState::ie()->ReleaseCircuit(0);
		CCommonState::ie()->ReleaseCircuit(1);
		return false;
	}*/

	return true;
}

void CCircuitInfoDlg::LoadInfo()
{
	m_pSetupDlg[0]->InitCircuitInfo(0);
	m_pSetupDlg[1]->InitCircuitInfo(1);

	m_pSetupDlg[0]->LoadInfo(0);
	m_pSetupDlg[1]->LoadInfo(1);

	//20221122 GBM -  설비 수정 후 취소의 경우에 롤백 대비로 설비 정보를 따로 저장해둔다.
	m_pSetupDlg[0]->BackupCircuitInfo(0);
	m_pSetupDlg[1]->BackupCircuitInfo(1);

	//롤백 대비 회로 개수 정보
	m_pSetupDlg[0]->BackupCircuitCountInfo(0);
	m_pSetupDlg[1]->BackupCircuitCountInfo(1);

	//20221024 GBM start - 회로 개수 정보가 로드되었으므로 추후 비교를 위해 현재 비교 데이터에 넣는다.
	m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0, true);
	m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1, true);

	//파일 로드 시 중계기 일람표 기준대로 중계기 일람표 회로정보를 넣는다.
	CCommonState::ie()->InitSelectCircuitRepeaterListOnLoadFile(0);
	CCommonState::ie()->InitSelectCircuitRepeaterListOnLoadFile(1);

	//중계기 일람표가 일단 확정되는 순간 전체 회로 갯수를 구하고 추가분에 대해서는 따로 추가해서 현재 최종 회로 갯수를 항상 알고 있도록 함
	CCommonState::ie()->m_nTotalCountCircuit_0 = CCommonState::ie()->CalculateTotalCircuitCount(0);
	CCommonState::ie()->m_nTotalCountCircuit_1 = CCommonState::ie()->CalculateTotalCircuitCount(1);
	//20221024 GBM end

	CCommonState::ie()->m_bInitCircuit = false;
	m_btnCircuitListInit.ShowWindow(TRUE);
}

void CCircuitInfoDlg::OnNextClick()
{
	bool bCheckMaxCircuit1 = false;
	bool bCheckMaxCircuit2 = false;

	//20221018 GBM start - 중계기 일람표가 회로정보가 확정되어 처음 작성될 때 사용자에게 이후의 설비 변경 사항은 추가일 경우 현재 중계기 일람표 아래에 추가되고 삭제일 경우 기존 요소가 공란으로 대체됨을 알려야 함
	if (CCommonState::ie()->m_bInitCircuit)
	{
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"중계기 일람표 확정", L"\n설비 설정을 적용하여 중계기 일람표 확정됨\n\n중계기 일람표 확정 이후\n설비 추가인 경우 기존 중계기 일람표 하단에 추가,\n설비 삭제인 경우 중계기 일람표에서 설비가 제거됨\n\n계속하시겠습니까?", MB_YESNO, this);
#else
		CMessagePopup popup(L"Confirmation of\nModule Table", L"\n\nModule Table is finalized\napplying the current Equipment Settings.\nAfter confirmation, If Equipment is added,\nit will be added to the bottom of Module Table,\nIf Equipment is deleted,\nthe Equipment will be deleted from Module Table.", MB_YESNO, this);
#endif
		UINT nResult = popup.DoModal();
		if (nResult != IDOK) {
			return;
		}
	}
	//20221018 GBM end

// 	bool bCircuitChanged = false;
// 	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
// 		bool bCheck = CCircuitBasicInfo::Instance()->m_bCheck[nIndex];
// 		if (bCheck != CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex])
// 		{
// 			bCircuitChanged = true;
// 			break;
// 		}
// 	}

	//20221124 GBM start - test
#if 1
	//설정된 회로 개수의 변화를 체크
	bool bResult1 = m_pSetupDlg[0]->CompareNewCircuitInfoNeo(0);
	bool bResult2 = m_pSetupDlg[1]->CompareNewCircuitInfoNeo(1);
#else
	//설정된 회로 개수의 변화를 체크
	bool bResult1 = m_pSetupDlg[0]->CompareNewCircuitInfo(0);
	bool bResult2 = m_pSetupDlg[1]->CompareNewCircuitInfo(1);
#endif
	//20221124 GBM end

	//설정된 설비 종류의 변화를 체크
	//20221123 GBM start - test
	bool bResult3 = true;
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++)
	{
		bool bCheck = CCircuitBasicInfo::Instance()->m_bCheck[nIndex];
		if (CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex] != bCheck)
		{
			bResult3 = false;
			break;
		}
	}
	//20221123 GBM end

	//20221021 GBM start - 중계기 일람표 확정 전에 회로 기본 화면으로 돌아갔다가 아무 편집도 하지 않은 경우 중계기 일람표가 작성되어 있지 않음에도 불구하고 회로 개수의 변화가 없어 중계기 일람표를 작성하지 않는 오류 수정
	//if (!bResult1 || !bResult2) 
	if (!bResult1 || !bResult2 || !bResult3 || CCommonState::ie()->m_bInitCircuit)
	//20221021 GBM end
	{

		//20221018 GBM start - 중계기 일람표가 미작성 / 기작성 되었을 때 분기될 내용 변경
#if 1
		if (!CCommonState::ie()->m_bInitCircuit) 
		{
#ifndef ENGLISH_MODE
			CMessagePopup popup(L"중계기 일람표 변경", L"\n설비 설정 변경이 중계기 일람표에 적용됨\n\n설비 추가 시: 중계기 일람표 하단 추가,\n설비 삭제 시: 설비 설정에서 삭제된 갯수만큼\n중계기 일람표 상 역순으로 설비 삭제", MB_YESNO, this);
#else
			CMessagePopup popup(L"Change of Module Table", L"\nChange to Equipment Settings\nareapplied to Module Table.\nIn case of Equipment Addition:\nAt the bottom of Module Table\nIn case of Equipment Deletion:\nEquipment is deleted\nin reverse order on Module Table.", MB_YESNO, this);
#endif
			UINT nResult = popup.DoModal();
			if (nResult == IDOK)
			{
				// 1. 현재 설비 리스트의 개수를 m_selectciruit에 적용
				if (!CheckCircuitCount()) {
					return;
				}

				int nTotalCircuit0 = 0;
				int nTotalCircuit1 = 0;

				//최대 회로 개수 초과여부 확인
				bCheckMaxCircuit1 = m_pSetupDlg[0]->CheckMaxCircuitCount(0, &nTotalCircuit0);
				bCheckMaxCircuit2 = m_pSetupDlg[1]->CheckMaxCircuitCount(1, &nTotalCircuit1);

				if (!bCheckMaxCircuit1 || !bCheckMaxCircuit2)
				{
					// 현재/과거 설비 비교 데이터 , 현재 회로 정보 롤백
// 					m_pSetupDlg[0]->RollbackCircuitCount(0);
// 					m_pSetupDlg[1]->RollbackCircuitCount(1);

// 					m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
// 					m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

					CString strMsg = L"";

					if (!bCheckMaxCircuit1 && bCheckMaxCircuit2)
					{
#ifndef ENGLISH_MODE
						strMsg.Format(L"\n\n0 계통 설비의 설정수(%d)가\n최대 회로수(%d)를 초과함\n\n기존 설비수로 원복\n(현재 중계기 일람표 기준)", nTotalCircuit0, MAX_CIRCUIT);
#else
						strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 0\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, MAX_CIRCUIT);
#endif
					}
					else if (bCheckMaxCircuit1 && !bCheckMaxCircuit2)
					{
#ifndef ENGLISH_MODE
						strMsg.Format(L"\n\n1 계통 설비의 설정수(%d)가\n최대 회로수(%d)를 초과함\n\n기존 설비수로 원복\n(현재 중계기 일람표 기준)", nTotalCircuit1, MAX_CIRCUIT);
#else
						strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 1\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit1, MAX_CIRCUIT);
#endif
					}
					else
					{
#ifndef ENGLISH_MODE
						strMsg.Format(L"\n\n0 계통, 1 계통 설비의 설정수(%d, %d)가\n최대 회로수(%d)를 초과함\n\n기존 설비수로 원복\n(현재 중계기 일람표 기준)", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#else
						strMsg.Format(L"\n\nThe number of Equipment Settings(%d, %d)\nof Loop 0 and Loop 1 exceeded\nthe maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#endif
					}

#ifndef ENGLISH_MODE
					CMessagePopup popup(L"최대 회로 개수 초과", strMsg, MB_OK, this);
#else
					CMessagePopup popup(L"Exceed the maximum\nnumber of Circuit", strMsg, MB_OK, this);
#endif
					popup.DoModal();

					// 설비 정보를 확인 후 이전 값을 롤백함
					for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++)
					{
						bool bCheck = CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex];
						if (bCheck != CCircuitBasicInfo::Instance()->m_bCheck[nIndex])
						{
							CCircuitBasicInfo::Instance()->m_bCheck[nIndex] = bCheck;
						}
					}

					m_pSetupDlg[0]->CopyPreviousCircuitInfo(0);
					m_pSetupDlg[1]->CopyPreviousCircuitInfo(1);

					m_pSetupDlg[0]->InitCircuitInfo(0);
					m_pSetupDlg[1]->InitCircuitInfo(1);

					m_pSetupDlg[0]->Invalidate();
					m_pSetupDlg[1]->Invalidate();

					m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
					m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

					m_pSetupDlg[0]->LoadInfo(0);
					m_pSetupDlg[1]->LoadInfo(1);

					return;
				}

				// 수정을 확정지었으므로 설비 종류 상황을 old값에 저장함
				for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
					bool bCheck = CCircuitBasicInfo::Instance()->m_bCheck[nIndex];
					CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex] = bCheck;
				}

				// 2. 설비 비교 구조체 내 기존 설비 개수에 현재 설비 개수를 대입

				//20221014 GBM start - 기존 설비 개수를 백업해둔다, 

				//중계기 일람표 확정 전에 앞서 설비정보만 수정한 상태로 설비/회로 화면만 왔다갔다하다가 
				//다시 중계기 일람표 확정 시 기존 회로 개수를 오버라이트한 상황이라서 
				//중계기 일람표에 수정된 내용이 적용되지 않는 오류 수정을 위해 
				//중계기 일람표가 확정되는 시점에 별도의 메모리에 백업해 둠

				m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0);
				m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1);

				m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
				m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

				//20221122 GBM - 설비 수정 후 취소의 경우에 롤백 대비로 설비 정보를 따로 저장해둔다.
				m_pSetupDlg[0]->BackupCircuitInfo(0);
				m_pSetupDlg[1]->BackupCircuitInfo(1);

				//롤백 대비 회로 개수 정보
				m_pSetupDlg[0]->BackupCircuitCountInfo(0);
				m_pSetupDlg[1]->BackupCircuitCountInfo(1);

				//20221014 GBM end

				// 3. 현재 설비 개수와 기존 설비 개수를 비교해서 추가/삭제된 경우의 리스트를 얻는다.
				CCommonState::ie()->m_vecAddCircuit.clear();
				CCommonState::ie()->m_vecDeleteCircuit.clear();

				m_pSetupDlg[0]->MakeAddCircuitInfoAndDeleteCircuitInfo(0);
				m_pSetupDlg[1]->MakeAddCircuitInfoAndDeleteCircuitInfo(1);

				GetParent()->PostMessage(SELECTION_PROJECT, 12, 0);
			}
			else
			{
				// 수정을 취소했으므로 설비 정보를 확인 후 이전 값을 롤백함
				for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) 
				{
					bool bCheck = CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex];
					if (bCheck != CCircuitBasicInfo::Instance()->m_bCheck[nIndex])
					{
						CCircuitBasicInfo::Instance()->m_bCheck[nIndex] = bCheck;
					}
				}

				//설비 종류 수정 사항이 있을 때는 중계기 일람표 확정 시 백업했던 회로 정보를 가져와서 사용자가 [취소]를 선택할 경우 원복

				m_pSetupDlg[0]->CopyPreviousCircuitInfo(0);
				m_pSetupDlg[1]->CopyPreviousCircuitInfo(1);

				//수정 사항을 반영하지 않을 것이므로 이전 값으로 돌아감
				m_pSetupDlg[0]->InitCircuitInfo(0);
				m_pSetupDlg[1]->InitCircuitInfo(1);

				m_pSetupDlg[0]->Invalidate();
				m_pSetupDlg[1]->Invalidate();

				m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
				m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

				m_pSetupDlg[0]->LoadInfo(0);
				m_pSetupDlg[1]->LoadInfo(1);

				return;
				//GetParent()->PostMessage(SELECTION_PROJECT, 13, 0);
			}
		}
		else
		{

			// 1. 현재 설비 리스트의 개수를 m_selectciruit에 적용
			if (!CheckCircuitCount()) {
				return;
			}

			int nTotalCircuit0 = 0;
			int nTotalCircuit1 = 0;

			//최대 회로 개수 초과여부 확인
			bCheckMaxCircuit1 = m_pSetupDlg[0]->CheckMaxCircuitCount(0, &nTotalCircuit0);
			bCheckMaxCircuit2 = m_pSetupDlg[1]->CheckMaxCircuitCount(1, &nTotalCircuit1);

			if (!bCheckMaxCircuit1 || !bCheckMaxCircuit2)
			{
				// 현재/과거 설비 비교 데이터 , 현재 회로 정보 롤백
// 				m_pSetupDlg[0]->RollbackCircuitCount(0);
// 				m_pSetupDlg[1]->RollbackCircuitCount(1);

// 				m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
// 				m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

				CString strMsg = L"";

				if (!bCheckMaxCircuit1 && bCheckMaxCircuit2)
				{
#ifndef ENGLISH_MODE
					strMsg.Format(L"\n\n0 계통 설비의 설정수(%d)가\n최대 회로수(%d)를 초과함\n\n기존 설비수로 원복", nTotalCircuit0, MAX_CIRCUIT);
#else
					strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 0\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, MAX_CIRCUIT);
#endif
				}
				else if (bCheckMaxCircuit1 && !bCheckMaxCircuit2)
				{
#ifndef ENGLISH_MODE
					strMsg.Format(L"\n\n1 계통 설비의 설정수(%d)가\n최대 회로수(%d)를 초과함\n\n기존 설비수로 원복", nTotalCircuit1, MAX_CIRCUIT);
#else
					strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 1\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit1, MAX_CIRCUIT);
#endif
				}
				else
				{
#ifndef ENGLISH_MODE
					strMsg.Format(L"\n\n0 계통, 1 계통 설비의 설정수(%d, %d)가\n최대 회로수(%d)를 초과함\n\n기존 설비수로 원복", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#else
					strMsg.Format(L"\n\nThe number of Equipment Settings(%d, %d)\nof Loop 0 and Loop 1 exceeded\nthe maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#endif
				}

#ifndef ENGLISH_MODE
				CMessagePopup popup(L"최대 회로 개수 초과", strMsg, MB_OK, this);
#else
				CMessagePopup popup(L"Exceed the maximum\nnumber of Circuit", strMsg, MB_OK, this);
#endif
				popup.DoModal();

				// 설비 정보를 확인 후 이전 값을 롤백함
				for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++)
				{
					bool bCheck = CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex];
					if (bCheck != CCircuitBasicInfo::Instance()->m_bCheck[nIndex])
					{
						CCircuitBasicInfo::Instance()->m_bCheck[nIndex] = bCheck;
					}
				}

				m_pSetupDlg[0]->CopyPreviousCircuitInfo(0);
				m_pSetupDlg[1]->CopyPreviousCircuitInfo(1);

				m_pSetupDlg[0]->InitCircuitInfo(0);
				m_pSetupDlg[1]->InitCircuitInfo(1);

				m_pSetupDlg[0]->Invalidate();
				m_pSetupDlg[1]->Invalidate();

				m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
				m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

				m_pSetupDlg[0]->LoadInfo(0);
				m_pSetupDlg[1]->LoadInfo(1);

				return;
			}

			// 수정을 확정지었으므로 설비 종류 상황을 old값에 저장함
			for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
				bool bCheck = CCircuitBasicInfo::Instance()->m_bCheck[nIndex];
				CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex] = bCheck;
			}

			//20221014 GBM start - 기존 설비 개수를 백업해둔다.
			m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0);
			m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1);
			//20221014 GBM end

			m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
			m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

			//20221122 GBM - 설비 수정 후 취소의 경우에 롤백 대비로 설비 정보를 따로 저장해둔다.
			m_pSetupDlg[0]->BackupCircuitInfo(0);
			m_pSetupDlg[1]->BackupCircuitInfo(1);

			//롤백 대비 회로 개수 정보
			m_pSetupDlg[0]->BackupCircuitCountInfo(0);
			m_pSetupDlg[1]->BackupCircuitCountInfo(1);

			//중계기 일람표가 일단 확정되는 순간 전체 회로 갯수를 구하고 추가분에 대해서는 따로 추가해서 현재 최종 회로 갯수를 항상 알고 있도록 함
			CCommonState::ie()->m_nTotalCountCircuit_0 = CCommonState::ie()->CalculateTotalCircuitCount(0);
			CCommonState::ie()->m_nTotalCountCircuit_1 = CCommonState::ie()->CalculateTotalCircuitCount(1);

			//중계기 일람표 확정 취소 버튼 활성화
			m_btnCircuitListInit.ShowWindow(TRUE);
			m_btnCircuitListInit.Invalidate();

			GetParent()->PostMessage(SELECTION_PROJECT, 12, 0);
		}
#else
		//20221011 GBM start
		if (!CCommonState::ie()->m_bInitCircuit) {
			CMessagePopup popup(L"중계기 일람표", L"\n변경사항으로 인하여\n\n중계기 일람표가 초기화 됩니다.\n\n계속하시겠습니까?\n\n(확인: 새작업 / 취소: 기존작업)", MB_YESNO, this);

			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {

				if (!CheckCircuitCount()) {
					return;
				}

				//20221014 GBM start - 기존 설비 개수를 백업해둔다.
				m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0);
				m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1);
				//20221014 GBM end

				GetParent()->PostMessage(SELECTION_PROJECT, 12, 0);
			}
			else {
				GetParent()->PostMessage(SELECTION_PROJECT, 13, 0);
			}
			//CMessagePopup popup(L"중계기 일람표", L"\n변경사항이 존재합니다.\n새로 구성하시겠습니까?\n\n(확인: 새구성, 취소: 기존구성 변경)", MB_YESNO, this);
			//UINT nResult = popup.DoModal();
			//if (nResult == IDOK) {
			//	if (!CheckCircuitCount()) {
			//		return;
			//	}
			//	GetParent()->PostMessage(SELECTION_PROJECT, 21, 0);
			//}
			//else {
			//	GetParent()->PostMessage(SELECTION_PROJECT, 22, 0);
			//}
			//20221011 GBM end
		}
		else {
			if (!CheckCircuitCount()) {
				return;
			}

			//20221014 GBM start - 기존 설비 개수를 백업해둔다.
			m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0);
			m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1);
			//20221014 GBM end

			GetParent()->PostMessage(SELECTION_PROJECT, 12, 0);
		}
#endif
		//20221018 GBM end

	}
	else {
		GetParent()->PostMessage(SELECTION_PROJECT, 13, 0);
	}
}

void CCircuitInfoDlg::OnPrevClick()
{
	bool bCheckMaxCircuit1 = false;
	bool bCheckMaxCircuit2 = false;

	//20221014 GBM start - 현재 회로 개수 작업 내용 메모리에 저장
	m_pSetupDlg[0]->SaveCircuitInfo(0);
	m_pSetupDlg[1]->SaveCircuitInfo(1);
	//20221014 GBM end

	int nTotalCircuit0 = 0;
	int nTotalCircuit1 = 0;

	//최대 회로 개수 초과여부 확인
	bCheckMaxCircuit1 = m_pSetupDlg[0]->CheckMaxCircuitCount(0, &nTotalCircuit0);
	bCheckMaxCircuit2 = m_pSetupDlg[1]->CheckMaxCircuitCount(1, &nTotalCircuit1);

	if (!bCheckMaxCircuit1 || !bCheckMaxCircuit2)
	{
		// 현재/과거 설비 비교 데이터 , 현재 회로 정보 롤백, 중계기 일람표로 넘어갈 때와는 달리 바로 이전 시점의 데이터로 되돌아 감
		m_pSetupDlg[0]->RollbackCircuitCount(0);
		m_pSetupDlg[1]->RollbackCircuitCount(1);

		CString strMsg = L"";

		if (!bCheckMaxCircuit1 && bCheckMaxCircuit2)
		{
#ifndef ENGLISH_MODE
			strMsg.Format(L"\n\n0 계통 설비의 설정수(%d)가\n최대 회로수(%d)를 초과함\n\n기존 설비수로 원복", nTotalCircuit0, MAX_CIRCUIT);
#else
			strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 0\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, MAX_CIRCUIT);
#endif
		}
		else if (bCheckMaxCircuit1 && !bCheckMaxCircuit2)
		{
#ifndef ENGLISH_MODE
			strMsg.Format(L"\n\n1 계통 설비의 설정수(%d)가\n최대 회로수(%d)를 초과함\n\n기존 설비수로 원복", nTotalCircuit1, MAX_CIRCUIT);
#else
			strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 1\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit1, MAX_CIRCUIT);
#endif
		}
		else
		{
#ifndef ENGLISH_MODE
			strMsg.Format(L"\n\n0 계통, 1 계통 설비의 설정수(%d, %d)가\n최대 회로수(%d)를 초과함\n\n기존 설비수로 원복", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#else
			strMsg.Format(L"\n\nThe number of Equipment Settings(%d, %d)\nof Loop 0 and Loop 1 exceeded\nthe maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#endif
		}

#ifndef ENGLISH_MODE
		CMessagePopup popup(L"최대 회로 개수 초과", strMsg, MB_OK, this);
#else
		CMessagePopup popup(L"Exceed the maximum\nnumber of Circuit", strMsg, MB_OK, this);
#endif
		popup.DoModal();

		m_pSetupDlg[0]->InitCircuitInfo(0);
		m_pSetupDlg[1]->InitCircuitInfo(1);

		m_pSetupDlg[0]->LoadInfo(0);
		m_pSetupDlg[1]->LoadInfo(1);

		return;
	}

	//20221014 GBM start - 기존 설비 개수를 백업해둔다.
	m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0);
	m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1);
	//20221014 GBM end

	GetParent()->PostMessage(SELECTION_PROJECT, 0, 2);
}

LRESULT CCircuitInfoDlg::OnTabButtnClick(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)wParam;
	switch (nIndex) {
	case 0:
		m_pTabButton[0]->SetCheck(true);
		m_pTabButton[1]->SetCheck(false);
		m_pSetupDlg[0]->ShowWindow(SW_SHOW);
		m_pSetupDlg[1]->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pTabButton[1]->SetCheck(true);
		m_pTabButton[0]->SetCheck(false);
		m_pSetupDlg[1]->ShowWindow(SW_SHOW);
		m_pSetupDlg[0]->ShowWindow(SW_HIDE);
		break;
	default: break;
	}
	return 0;
}

void CCircuitInfoDlg::Redisplay()
{
	CRect rect, rt;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawRect(&memDC, true, RGB(255, 255, 255), RGB(255, 255, 255), rect);
	rt.CopyRect(&rect);
	rt.left += 4;
	rt.right -= 4;
	rt.top += 50;
	rt.bottom -= 7;
	CCommonDisplay::DrawRect(&memDC, false, RGB(220, 220, 220), 0, rt);

	if (m_pSetupDlg[0] && m_pSetupDlg[1]) {
		rt.left += 5;
		rt.right -= 5;
		rt.bottom -= 5;
		rt.top += 5;
		m_pSetupDlg[0]->MoveWindow(rt);
		m_pSetupDlg[1]->MoveWindow(rt);
	}

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInit) {
		m_btnNext.MoveWindow(rect.right - 87, 0, 83, 48);
		m_btnPrev.MoveWindow(rect.right - 180, 0, 83, 48);
		m_btnCircuitListInit.MoveWindow(rect.right - 350, 0, 155, 48);
		m_btnNext.Invalidate();
		m_btnPrev.Invalidate();
		m_btnCircuitListInit.Invalidate();
	}
}

void CCircuitInfoDlg::OnCircuitListInit()
{
#ifndef ENGLISH_MODE
	CMessagePopup popup(L"중계기 일람표 초기화", L"\n중계기 일람표가 초기화됨\n\n※ 경고 : [확인] 선택 시\n기존에 생성된 중계기 일람표가 모두 삭제되어\n다시 중계기 일람표를 확정해도\n[새 회로번호]가 적용됩니다.\n신중히 선택하세요.", MB_YESNO, this);
#else
	CMessagePopup popup(L"Initialization\nof Module Table", L"\n\nThe Module Table will be initialized\n\n※ Warning : When [OK] is selected,\nAll of created Module Table is deleted,\nso even if Module Table is confirmed again,\n[New Circuit Number] will be applied.", MB_YESNO, this);
#endif

	UINT nResult = popup.DoModal();
	if (nResult != IDOK)
	{
		return;
	}

	//중계기 일람표 확정 플래그 false
	CCommonState::ie()->m_bInitCircuit = true;

	//회로 리스트 삭제
	CSaveManager::ie()->ReleaseInfo();

	//중계기 일람표 확정 취소 버튼 비활성화
	m_btnCircuitListInit.ShowWindow(FALSE);
	m_btnCircuitListInit.Invalidate();

	//중계기 일람표 확정 문구 삭제, 중계기 일람표 Excel 저장 플래그 false
	GetParent()->PostMessage(CIRCUIT_LIST_INIT_MSG, 0, 0);

}