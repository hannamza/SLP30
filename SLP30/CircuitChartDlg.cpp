// CircuitChartDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CircuitChartDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CCircuitChartDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCircuitChartDlg, CDialogEx)

CCircuitChartDlg::CCircuitChartDlg(CWnd* pParent /*=NULL*/, int nCircuit)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_pListCtrl = NULL;
	m_nCircuit = nCircuit;
}

CCircuitChartDlg::~CCircuitChartDlg()
{
}

void CCircuitChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircuitChartDlg, CDialogEx)
	ON_MESSAGE(LIST_CONTROL, OnListControl)
	ON_MESSAGE(LIST_POPUP_SEL, OnPopupSelect)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCircuitChartDlg 메시지 처리기입니다.
#ifndef ENGLISH_MODE
const TCHAR* g_editHeader[] = { _T("중계기No"), _T("회로No"), _T("회로명"), _T("건물명"), _T("동"), _T("계단"), _T("층"), _T("실명"), _T("설비번호"), NULL };
#else
const TCHAR* g_editHeader[] = { _T("MOD No"), _T("CCT No"), _T("CCT Name"), _T("B.Name"), _T("B.BLCK"), _T("LINE"), _T("FLOOR"), _T("RM NAME"), _T("EQP No."), NULL };
#endif
const int g_editSize[] = { 80, 70, 200, 80, 120, 90, 90, 100, 75, 0 }; // pixel

//20221026 GBM start - 중계기No, 실명, 설비번호만 수정가능하도록 함
#if 1
	const int g_editType[] = { POPUP_TYPE_EDIT, POPUP_TYPE_SEQ , POPUP_TYPE_NONE, POPUP_TYPE_NONE, POPUP_TYPE_NONE, POPUP_TYPE_NONE, POPUP_TYPE_NONE, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, 0 };
#else
	const int g_editType[] = { POPUP_TYPE_EDIT, POPUP_TYPE_SEQ , POPUP_TYPE_LIST_NOINPUT, POPUP_TYPE_EDIT, POPUP_TYPE_LIST_NOINPUT, POPUP_TYPE_LIST_NOINPUT, POPUP_TYPE_LIST_NOINPUT, POPUP_TYPE_LIST_ADD, POPUP_TYPE_EDIT, 0 };
#endif
//20221026 GBM end

const int g_editAlign[] = { DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, -1 }; // option, row align text, default: DT_LEFT
const int g_editLimit[] = { 10, 10, 0, 0, 0, 20, 20, 10, -1 }; // 0: 무제한, > 0 : 텍스트 사이즈 제한

BOOL CCircuitChartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CRuntimeClass	*pRuntimeClass;
	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomListCtrl))) {
		if (NULL != (m_pListCtrl = (CCustomListCtrl *)(pRuntimeClass->CreateObject()))) {
			m_pListCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}

	m_pListCtrl->SetListIndex(0);
	m_pListCtrl->SetHeaderInfo((TCHAR**)g_editHeader, (int*)g_editSize, (int*)g_editType, (int*)g_editAlign); // Align: option, row align text, default: DT_LEFT
	m_pListCtrl->SetColumnLimit((int*)g_editLimit);
	// 높이 변경 (option)
	m_pListCtrl->SetHeaderHeight(40);
	m_pListCtrl->SetRowHeight(30);
	//m_pListCtrl->SetListPopupSize(500, 500);
	// 색변경 (option)
	m_pListCtrl->SetHeaderBkColor(RGB(80, 80, 80), RGB(255, 255, 255));
	m_pListCtrl->SetHeaderTextColor(RGB(220, 220, 220));
	m_pListCtrl->SetRowBkColor(RGB(160, 160, 160), RGB(255, 255, 0), RGB(255, 255, 255));
	m_pListCtrl->SetRowTextColor(RGB(255, 255, 255), RGB(80, 80, 80));
	m_pListCtrl->SetHeaderFontSize(15);
	m_pListCtrl->FixHeaderSize(false);

	CStringArray arrayName;
	CCircuitBasicInfo::Instance()->GetCircuitName(arrayName);
	for (int nIndex = 0; nIndex < CIRCUIT_CNT; nIndex++) {
		m_pListCtrl->AddPopupListItem(2, arrayName.GetAt(nIndex));
	}

	m_pListCtrl->SendMessage(WM_INITIALUPDATE);

	m_pListCtrl->AddItem(MAX_CIRCUIT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCircuitChartDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CCircuitChartDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CCircuitChartDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCircuitChartDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircuitChartDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircuitChartDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCircuitChartDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CCircuitChartDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CCircuitChartDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CCircuitChartDlg::OnPopupSelect(WPARAM wParam, LPARAM lParam)
{
	int nItemIndex = (int)wParam;
	int nSelCol = (int)lParam;
	if (nSelCol == 2) {
		m_pListCtrl->SetItemText(nItemIndex, 3, CCircuitBasicInfo::Instance()->m_sBuildingName);
	}
	return 0;
}

LRESULT CCircuitChartDlg::OnListControl(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;
	int nSelectItem = (int)lParam;
	switch (nType)
	{
	case 1: // del
		if (m_pListCtrl) {
			CMessagePopup popup(L"목록 삭제", L"\n\n선택한 목록을 삭제하면\n\n마지막 목록에 신규 목록이 1개 추가됨\n\n선택한 목록을 삭제하시겠습니까?", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				m_pListCtrl->DelListItem(nSelectItem);
				m_pListCtrl->AddItem();

				SettingChartNumber();
			}
		}
		break;
	case 2: // up add
		if (m_pListCtrl) {
			CMessagePopup popup(L"목록 추가", L"\n\n신규 목록을 추가하면\n\n마지막 목록 1개가 삭제됨\n\n신규 목록을 추가하시겠습니까?", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				m_pListCtrl->InsertListItemUp(nSelectItem);
				m_pListCtrl->DelListItem(m_pListCtrl->GetItemCount() - 1);
				m_pListCtrl->Invalidate();

				SettingChartNumber();
			}
		}
		break;
	case 3: // down add
		if (m_pListCtrl) {
			CMessagePopup popup(L"목록 추가", L"\n\n신규 목록을 추가하면\n\n마지막 목록 1개가 삭제됨\n\n신규 목록을 추가하시겠습니까?", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				m_pListCtrl->InsertListItemDown(nSelectItem);
				m_pListCtrl->DelListItem(m_pListCtrl->GetItemCount() - 1);
				m_pListCtrl->Invalidate();

				SettingChartNumber();
			}
		}
		break;
	//20221027 GBM start - 회로번호 이동 케이스 추가
	case 4:
	{
		int* nCircuitMoveNum = (int*)lParam;
		int nCurrentIndex = nCircuitMoveNum[0];
		int nNewIndex = nCircuitMoveNum[1];
		int nMaxCircuitAddress = MAX_CIRCUIT_ADDRESS;

		bool bIsVacantCircuit = false;
		CString strMsg = L"";

		//새 회로번호 혹은 기존 회로번호가 현재 설정된 전체 회로 갯수의 마지막 회로 번호를 초과하는지 검사
		if (m_nCircuit == 0)
		{
			if (nNewIndex + 1 > CCommonState::ie()->m_nTotalCountCircuit_0)
			{
#ifndef ENGLISH_MODE
				strMsg.Format(L"\n\n신규 회로번호(%d)가\n현재 구성된 총 회로번호 개수(%d)보다 클 수 없음\n\n총 회로번호 개수(%d) 이상의 설비를 추가하는 경우\n[이전]으로 돌아가 설비 개수를 추가하세요.",
					nNewIndex + 1, CCommonState::ie()->m_nTotalCountCircuit_0, CCommonState::ie()->m_nTotalCountCircuit_0);
				CMessagePopup popup(L"총 회로번호 개수 초과", strMsg, MB_OK, this);
#else
				strMsg.Format(L"\n\n\nNew Circuit No.(%d) cannot be greater than\ntotal number of configured Circuit(%d).\n\nReturn to [Previous]\nand increase the number of Equipment.",
					nNewIndex + 1, CCommonState::ie()->m_nTotalCountCircuit_0, CCommonState::ie()->m_nTotalCountCircuit_0);
				CMessagePopup popup(L"Exceeding\nthe total number of circuit", strMsg, MB_OK, this);
#endif
				popup.DoModal();
				break;
			}

			if (nCurrentIndex + 1 > CCommonState::ie()->m_nTotalCountCircuit_0)
			{
#ifndef ENGLISH_MODE
				strMsg.Format(L"\n\n이동할 회로번호(%d)는\n구성된 총 회로번호 개수(%d)보다 큰 수이므로\n이동할 수 없음",
					nCurrentIndex + 1, CCommonState::ie()->m_nTotalCountCircuit_0, CCommonState::ie()->m_nTotalCountCircuit_0);
				CMessagePopup popup(L"총 회로번호 개수 초과", strMsg, MB_OK, this);
#else
				strMsg.Format(L"\n\n\nThe number of Circuit to move(%d)\ncan NOT be moved\nbecause it is greater than\ntotal number of configured Circuit(%d).",
					nCurrentIndex + 1, CCommonState::ie()->m_nTotalCountCircuit_0, CCommonState::ie()->m_nTotalCountCircuit_0);
				CMessagePopup popup(L"Exceeding\nthe total number of circuit", strMsg, MB_OK, this);
#endif
				
				popup.DoModal();
				break;
			}
		}
		else
		{
			if (nNewIndex + 1 > CCommonState::ie()->m_nTotalCountCircuit_1)
			{
#ifndef ENGLISH_MODE
				strMsg.Format(L"\n\n신규 회로번호(%d)가\n현재 구성된 총 회로번호 개수(%d)보다 클 수 없음\n\n총 회로번호 개수(%d) 이상의 설비를 추가하는 경우\n[이전]으로 돌아가 설비 개수를 추가하세요.",
					nNewIndex + 1, CCommonState::ie()->m_nTotalCountCircuit_1, CCommonState::ie()->m_nTotalCountCircuit_1);
				CMessagePopup popup(L"총 회로번호 개수 초과", strMsg, MB_OK, this);
#else
				strMsg.Format(L"\n\n\nNew Circuit No.(%d) cannot be greater than\ntotal number of configured Circuit(%d).\n\nReturn to [Previous]\nand increase the number of Equipment.",
					nNewIndex + 1, CCommonState::ie()->m_nTotalCountCircuit_1, CCommonState::ie()->m_nTotalCountCircuit_1);
				CMessagePopup popup(L"Exceeding\nthe total number of circuit", strMsg, MB_OK, this);
#endif
				popup.DoModal();
				break;
			}

			if (nCurrentIndex + 1 > CCommonState::ie()->m_nTotalCountCircuit_1)
			{
#ifndef ENGLISH_MODE
				strMsg.Format(L"\n\n이동할 회로번호(%d)는\n구성된 총 회로번호 개수(%d)보다 큰 수이므로\n이동할 수 없음",
					nCurrentIndex + 1, CCommonState::ie()->m_nTotalCountCircuit_1, CCommonState::ie()->m_nTotalCountCircuit_1);
				CMessagePopup popup(L"총 회로번호 개수 초과", strMsg, MB_OK, this);
#else
				strMsg.Format(L"\n\n\nThe number of Circuit to move(%d)\ncan NOT be moved\nbecause it is greater than\ntotal number of configured Circuit(%d).",
					nCurrentIndex + 1, CCommonState::ie()->m_nTotalCountCircuit_1, CCommonState::ie()->m_nTotalCountCircuit_1);
				CMessagePopup popup(L"Exceeding\nthe total number of circuit", strMsg, MB_OK, this);
#endif
				popup.DoModal();
				break;
			}
		}

		// 이 시점에 한번도 중계기 일람표 회로 정보가 저장된 적이 없다면 저장
// 		int nSystemInfo = 0;
// 		nSystemInfo = CSaveManager::ie()->m_listSystem.GetSize();
// 		if (nSystemInfo <= 0)
// 		{
// 			SaveInformation(0);
// 			SaveInformation(1);
// 		}
		SaveInformation(0);
		SaveInformation(1);

		if (m_nCircuit == 1)
		{
			nCurrentIndex += nMaxCircuitAddress;
			nNewIndex += nMaxCircuitAddress;
		}

		bIsVacantCircuit = CSaveManager::ie()->MoveCircuitInfo(nCurrentIndex, nNewIndex);

		if (!bIsVacantCircuit)
		{
			int nNewCircuitNum = 0;
			if (m_nCircuit == 0)
				nNewCircuitNum = nNewIndex + 1;
			else
				nNewCircuitNum = nNewIndex + 1 - nMaxCircuitAddress;

#ifndef ENGLISH_MODE
			strMsg.Format(L"\n\n새로운 회로번호(%d)에는 회로가 이미 존재함", nNewCircuitNum);
			CMessagePopup popup(L"회로 번호 중복", strMsg, MB_OK, this);
#else
			strMsg.Format(L"\n\n\nNew Circuit Number(%d) already exists", nNewCircuitNum);
			CMessagePopup popup(L"Circuit already exists", strMsg, MB_OK, this);
#endif

			popup.DoModal();
			break;
		}

		if (m_nCircuit == 0)
		{
			DisplayLoadFile(0);
		}
		else
		{
			DisplayLoadFile(1);
		}
	}
		break;
	//20221027 GBM end
	default:
		break;

	}
	return 0;
}

void CCircuitChartDlg::SettingChartNumber()
{
	CString sText;
	int nCount = m_pListCtrl->GetItemCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		m_pListCtrl->GetItemText(nIndex, 0, sText);
		if (sText.GetLength() > 0) {
			sText.Format(L"%d", nIndex + 1);
			m_pListCtrl->SetItemText(nIndex, 0, sText);
		}
		sText = L"";
	}
}

bool CCircuitChartDlg::SaveInformation(int nChartIndex)
{
	if (!m_pListCtrl) {
		return false;
	}

	CString strRooftop;
	strRooftop = ROOFTOP_NAME;

	int nCount = m_pListCtrl->GetItemCount();
	CString sBlock, sStair, sFloor, sSystemNo, sCircuitNo, sCircuitName, sBDName, sRoomName;
	int nStair, nFloor, nSystemNo, nCircuitNo;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		m_pListCtrl->GetItemText(nIndex, 0, sSystemNo);
		m_pListCtrl->GetItemText(nIndex, 2, sCircuitName);
		m_pListCtrl->GetItemText(nIndex, 3, sBDName);
		m_pListCtrl->GetItemText(nIndex, 4, sBlock);
		m_pListCtrl->GetItemText(nIndex, 5, sStair);
		m_pListCtrl->GetItemText(nIndex, 6, sFloor);
		m_pListCtrl->GetItemText(nIndex, 7, sRoomName);
		m_pListCtrl->GetItemText(nIndex, 8, sCircuitNo);

#ifndef ENGLISH_MODE
		sStair.Replace(L"계단", L"");
#else
		sStair.Replace(L"LINE ", L"");
#endif

		nStair = _wtoi(sStair.GetBuffer(0));

		bool bRooftop = false;
		if (sFloor.Find(L"B") >= 0) {
			sFloor.Replace(L"B", L"");
			sFloor.Replace(L"F", L"");
			nFloor = -_wtoi(sFloor.GetBuffer(0));
		}
		else {
			//옥탑 
			if (sFloor.Compare(strRooftop) == 0)
			{
				nFloor = CCircuitBasicInfo::Instance()->m_nFloor + 1;	//일반층 + 1 == 옥탑층
				bRooftop = true;
			}
			else
			{
				sFloor.Replace(L"F", L"");
				nFloor = _wtoi(sFloor.GetBuffer(0));
			}
		}
		nSystemNo = _wtoi(sSystemNo.GetBuffer(0));
		nCircuitNo = _wtoi(sCircuitNo.GetBuffer(0));

#ifndef ENGLISH_MODE
		sBlock.Replace(L"동", L"");
#else
		sBlock.Replace(L"B.BLCK ", L"");
#endif

		CSaveManager::ie()->SetSystemInfo(sBDName.GetBuffer(0), sCircuitName.GetBuffer(0), sRoomName.GetBuffer(0), sBlock.GetBuffer(0),
			(short)nStair, (short)nSystemNo, (short)nChartIndex, (short)nFloor, (bool)bRooftop, (short)nCircuitNo);
	}
	for (int nIndex = 0; nIndex < 6; nIndex++) {
		CSaveManager::ie()->SetSystemInfo(L"", L"", L"", L"", 0, 0, 0, 0, false, 250 + nIndex);
	}
	return true;
}

void CCircuitChartDlg::SetupPopupList()
{
	CString sTemp;
	int nStairCount = CCircuitBasicInfo::Instance()->m_nStair;
	m_pListCtrl->RemovePopupListItem(4);
	m_pListCtrl->RemovePopupListItem(5);
	m_pListCtrl->RemovePopupListItem(6);

	for (int nIndex = 0; nIndex < CCircuitBasicInfo::Instance()->m_nBlock; nIndex++) {
		if (CCircuitBasicInfo::Instance()->m_nBlock > 0) {
			sTemp = CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nIndex);
#ifndef ENGLISH_MODE
			sTemp += L"동";
#else
			sTemp = L"B.BLCK " + sTemp;
#endif
			m_pListCtrl->AddPopupListItem(4, sTemp);
		}
	}
	for (int nIndex = 0; nIndex < nStairCount; nIndex++) {
#ifndef ENGLISH_MODE
		sTemp.Format(L"%d계단", nIndex + 1);
#else
		sTemp.Format(L"LINE %d", nIndex + 1);
#endif
		m_pListCtrl->AddPopupListItem(5, sTemp);
	}
	int nBasement = CCircuitBasicInfo::Instance()->m_nBasement;
	for (int nIndex = nBasement; nIndex > 0; nIndex--) {
		sTemp.Format(L"B%dF", nIndex);
		m_pListCtrl->AddPopupListItem(6, sTemp);
	}
	int nFloor = CCircuitBasicInfo::Instance()->m_nFloor;
	for (int nIndex = 0; nIndex < nFloor; nIndex++) {
		sTemp.Format(L"%dF", nIndex + 1);
		m_pListCtrl->AddPopupListItem(6, sTemp);
	}
}

void CCircuitChartDlg::DisplayLoadFile(int nChartIndex)
{
	m_pListCtrl->ReleaseListItem();

	SetBackgroundColorAfterMaxCircuit(m_nCircuit);		//20221028 GBM - 리스트가 자신의 계통 정보를 알 수 있도록 추가

	SetupPopupList();

	CString strRooftop;
	strRooftop = ROOFTOP_NAME;
	CString sTemp;
	int nCount = CSaveManager::ie()->m_listSystem.GetCount();
	int nValue = 0;
	SYSTEM_INFO_* pInfo = NULL;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pInfo = CSaveManager::ie()->m_listSystem.GetAt(CSaveManager::ie()->m_listSystem.FindIndex(nIndex));
		if (pInfo->nSystem != nChartIndex) {
			continue;
		}
		sTemp = L"";
		if (pInfo->nSystemNo > 0) {
			sTemp.Format(L"%d", (int)pInfo->nSystemNo);
		}
		m_pListCtrl->SetItemText(nValue, 0, sTemp);
		m_pListCtrl->SetItemText(nValue, 2, pInfo->szCircuitName);
		m_pListCtrl->SetItemText(nValue, 3, pInfo->szBDName);
		if (wcslen(pInfo->szBlock) > 0) {
#ifndef ENGLISH_MODE
			sTemp.Format(L"%s동", pInfo->szBlock);
#else
			sTemp.Format(L"B.BLCK %s", pInfo->szBlock);
#endif
		}
		else {
			sTemp = L"";
		}
		m_pListCtrl->SetItemText(nValue, 4, sTemp);

		if (pInfo->nStair > 0) {
#ifndef ENGLISH_MODE
			sTemp.Format(L"%d계단", (int)pInfo->nStair);
#else
			sTemp.Format(L"LINE %d", (int)pInfo->nStair);
#endif
		}
		else {
			sTemp = L"";
		}
		m_pListCtrl->SetItemText(nValue, 5, sTemp);
		if (pInfo->nFloor > 0) {
			if (pInfo->bRooftop)	// 옥탑층
			{
				sTemp = strRooftop;
			}
			else
			{
				sTemp.Format(L"%dF", (int)pInfo->nFloor);
			}	
		}
		else if (pInfo->nFloor < 0) {
			sTemp.Format(L"B%dF", (int)abs(pInfo->nFloor));
		}
		else {
			sTemp = L"";
		}
		m_pListCtrl->SetItemText(nValue, 6, sTemp);
		m_pListCtrl->SetItemText(nValue, 7, pInfo->szRoomName);
		sTemp = L"";
		if (pInfo->nCircuitNo > 0) {
			sTemp.Format(L"%d", (int)pInfo->nCircuitNo);
		}
		m_pListCtrl->SetItemText(nValue, 8, sTemp);

		++nValue;
	}
}

bool CCircuitChartDlg::DisplayListItem(int nChartIndex)
{

	//20221102 GBM start - 보기 편한 소스로 변경
#if 1
	m_pListCtrl->ReleaseListItem();

	SetBackgroundColorAfterMaxCircuit(m_nCircuit);		//20221028 GBM - 리스트가 자신의 계통 정보를 알 수 있도록 추가

	SetupPopupList();

#ifndef ENGLISH_MODE
	CString sSystem = L"0 계통";
	if (nChartIndex == 1)
		sSystem = L"1 계통";
#else
	CString sSystem = L"LOOP 0";
	if (nChartIndex == 1)
		sSystem = L"LOOP 1";
#endif

	CString sBlock, sStair, sFloor, sCircuitName, sCircuitCount, sTemp;
	pSelectCircuit pCircuit = NULL;
	CStringArray arrayList;
	bool bEmpty = false;
	int nValue = 0;
	int nCount = 0;
	int nCircuitNo = 0;

	if (nChartIndex == 0) 
		nCount = CCommonState::ie()->m_selectCircuit_0.GetCount();
	else 
		nCount = CCommonState::ie()->m_selectCircuit_1.GetCount();

	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		if (nChartIndex == 0)
			pCircuit = CCommonState::ie()->m_selectCircuit_0.GetAt(CCommonState::ie()->m_selectCircuit_0.FindIndex(nIndex));
		else
			pCircuit = CCommonState::ie()->m_selectCircuit_1.GetAt(CCommonState::ie()->m_selectCircuit_1.FindIndex(nIndex));

		if (!pCircuit)
			continue;

		sBlock = pCircuit->sBlock;
		sStair = pCircuit->sStair;
		sFloor = pCircuit->sFloor;
		sCircuitName = pCircuit->sCircuitName;

		int nCircuitCount = 0;
		nCircuitCount = pCircuit->nCount;
		if(nCircuitCount == 0)
			continue;

		for (int nCircuit = 0; nCircuit < nCircuitCount; nCircuit++)
		{
			arrayList.RemoveAll();
			bool bDetector = false;
			bDetector = CCircuitBasicInfo::Instance()->IsDetector(sCircuitName);
			if (!bDetector)
			{
				CCircuitBasicInfo::Instance()->GetCircuitChild(sCircuitName, arrayList);

				for (int nChildCircuit = 0; nChildCircuit < arrayList.GetSize(); nChildCircuit++)
				{
					//회로 번호가 2개 할당 회로인지 검사
					int nCircuitAddressCount = 0;
					nCircuitAddressCount = CCommonState::ie()->CalculateCircuitAddressCount(arrayList[nChildCircuit]);

					//중계기 번호
					if (nChildCircuit == 0)
					{
						sTemp.Format(L"%d", nCircuitNo + 1);
						m_pListCtrl->SetItemText(nCircuitNo, 0, sTemp);
					}

					//회로 번호
					sTemp.Format(L"%d", nCircuitNo + 1);
					m_pListCtrl->SetItemText(nCircuitNo, 1, sTemp);

					//설비명
					m_pListCtrl->SetItemText(nCircuitNo, 2, arrayList[nChildCircuit]);

					//건물명
					m_pListCtrl->SetItemText(nCircuitNo, 3, CCircuitBasicInfo::Instance()->m_sBuildingName);

					//동
					m_pListCtrl->SetItemText(nCircuitNo, 4, sBlock);

					//계단
					m_pListCtrl->SetItemText(nCircuitNo, 5, sStair);

					//층
					m_pListCtrl->SetItemText(nCircuitNo, 6, sFloor);

					//실명

					//설비번호
					if (nCircuitCount > 1)
					{
						sTemp.Format(L"%d", nCircuit + 1);
						m_pListCtrl->SetItemText(nCircuitNo, 8, sTemp);
					}

					nCircuitNo += nCircuitAddressCount;
				}
			}
			else
			{
				//회로 번호가 2개 할당 회로인지 검사
				int nCircuitAddressCount = 0;
				nCircuitAddressCount = CCommonState::ie()->CalculateCircuitAddressCount(sCircuitName);

				if (nCircuitAddressCount == 1)
				{
					if (nCircuit % 2 == 0)
					{
						sTemp.Format(L"%d", nCircuitNo + 1);
						m_pListCtrl->SetItemText(nCircuitNo, 0, sTemp);
					}
				}
				else
				{
					sTemp.Format(L"%d", nCircuitNo + 1);
					m_pListCtrl->SetItemText(nCircuitNo, 0, sTemp);
				}

				//회로 번호
				sTemp.Format(L"%d", nCircuitNo + 1);
				m_pListCtrl->SetItemText(nCircuitNo, 1, sTemp);

				//설비명
				m_pListCtrl->SetItemText(nCircuitNo, 2, sCircuitName);

				//건물명
				m_pListCtrl->SetItemText(nCircuitNo, 3, CCircuitBasicInfo::Instance()->m_sBuildingName);

				//동
				m_pListCtrl->SetItemText(nCircuitNo, 4, sBlock);

				//계단
				m_pListCtrl->SetItemText(nCircuitNo, 5, sStair);

				//층
				m_pListCtrl->SetItemText(nCircuitNo, 6, sFloor);

				//실명

				//설비번호
				if (nCircuitCount > 1)
				{
					sTemp.Format(L"%d", nCircuit + 1);
					m_pListCtrl->SetItemText(nCircuitNo, 8, sTemp);
				}

				nCircuitNo += nCircuitAddressCount;
			}
		}
	}

	return true;

#else
	m_pListCtrl->ReleaseListItem();

	SetBackgroundColorAfterMaxCircuit(m_nCircuit);		//20221028 GBM - 리스트가 자신의 계통 정보를 알 수 있도록 추가

	SetupPopupList();

	CString sBlock, sStair, sFloor, sCircuitCount, sTemp;
	pSelectCircuit pCircuit = NULL;
	CStringArray arrayList;
	bool bEmpty = false;
	int nValue = 0;
	int nCount = 0;
	int nCircuitCount = 0;
	if (nChartIndex == 0) {
		nCount = CCommonState::ie()->m_selectCircuit_0.GetCount();
	}
	else if (nChartIndex == 1) {
		nCount = CCommonState::ie()->m_selectCircuit_1.GetCount();
	}
	//bool bFan = false;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		if (nChartIndex == 0) {
			pCircuit = CCommonState::ie()->m_selectCircuit_0.GetAt(CCommonState::ie()->m_selectCircuit_0.FindIndex(nIndex));
		}
		else if (nChartIndex == 1) {
			pCircuit = CCommonState::ie()->m_selectCircuit_1.GetAt(CCommonState::ie()->m_selectCircuit_1.FindIndex(nIndex));
		}
		if (!pCircuit) {
			continue;
		}
		if (nIndex == 0) {
			sStair = pCircuit->sStair;
			sFloor = pCircuit->sFloor;
			sBlock = pCircuit->sBlock;
		}
		if (sBlock.Compare(pCircuit->sBlock) != 0 || sStair.Compare(pCircuit->sStair) != 0 || sFloor.Compare(pCircuit->sFloor) != 0) {
			sStair = pCircuit->sStair;
			sFloor = pCircuit->sFloor;
			sBlock = pCircuit->sBlock;
			if (nValue % 2 == 1) {
				++nValue;
			}
		}
		bool bFind = false;
		for (int nIndex = 설비명_감지기_계단; nIndex < 설비명_감지기_A_SVP; nIndex++) {
			if (pCircuit->sCircuitName.Compare(g_lpszCircuitName[nIndex]) == 0) {
				bFind = true;
				break;
			}
		}
		if (bFind) {
			//아래 코드 의미 없음
// 			if (bFan) {
// 				if (nValue % 2 == 1) {
// 					nValue++;
// 				}
// 				bFan = false;
// 			}
			nCircuitCount = 0;
			for (int ii = 0; ii < pCircuit->nCount; ii++) {
				sCircuitCount.Format(L"%d", nCircuitCount + 1);
				++nCircuitCount;
				bEmpty = false;
				if (pCircuit->sCircuitName == g_lpszCircuitName[설비명_감지기_계단]
					|| pCircuit->sCircuitName == g_lpszCircuitName[설비명_감지기_ELEV]) {
					if (nValue % 2 == 1) {
						nValue++;
					}
					bEmpty = true;
				}
				if (nValue % 2 == 0) {
					sTemp.Format(L"%d", nValue + 1);
					m_pListCtrl->SetItemText(nValue, 0, sTemp);
				}
				m_pListCtrl->SetItemText(nValue, 2, pCircuit->sCircuitName);
				m_pListCtrl->SetItemText(nValue, 3, CCircuitBasicInfo::Instance()->m_sBuildingName);
				m_pListCtrl->SetItemText(nValue, 4, pCircuit->sBlock);
				m_pListCtrl->SetItemText(nValue, 5, pCircuit->sStair);
				m_pListCtrl->SetItemText(nValue, 6, pCircuit->sFloor);
				int nCNoIndex = CCircuitBasicInfo::Instance()->GetIndexCircuitName(pCircuit->sCircuitName);
				//m_pListCtrl->SetItemText(nValue, 7, g_lpszCircuitNo[nCNoIndex]);
				if (pCircuit->nCount > 1) {
					m_pListCtrl->SetItemText(nValue, 8, sCircuitCount);
				}

				++nValue;
				if (bEmpty) {
					++nValue;
				}
			}
			if (nValue > MAX_CIRCUIT) {
				return false;
			}
			continue;
		}
		nCircuitCount = 0;
		CCircuitBasicInfo::Instance()->GetCircuitChild(pCircuit->sCircuitName, arrayList);
		int nCircuitIndex = CCircuitBasicInfo::Instance()->GetCircuitParentIndex(pCircuit->sCircuitName);
		for (int ii = 0; ii < pCircuit->nCount; ii++) {
			switch (nCircuitIndex) {
			case 2: // 알람밸브 PS 1, TS 1
			case 5: // 전실 제연 급기 2
			case 6: // 전실 제연 배기 2
			case 15: // 저수위 감시 2
			case 3: // 프리액션 밸브 4
			//case 7: // 상가 제연 4
			case 8: // 방화셔터 4
				if (nValue % 2 == 1) {
					++nValue;
				}
				break;
			default:
				break;
			}
			sCircuitCount.Format(L"%d", nCircuitCount + 1);
			++nCircuitCount;
			for (int i = 0; i < arrayList.GetCount(); i++) {
				if (i == 0 && nValue % 2 == 0) {
					sTemp.Format(L"%d", nValue + 1);
					m_pListCtrl->SetItemText(nValue, 0, sTemp);
				}
				m_pListCtrl->SetItemText(nValue, 2, arrayList.GetAt(i));
				m_pListCtrl->SetItemText(nValue, 3, CCircuitBasicInfo::Instance()->m_sBuildingName);
				m_pListCtrl->SetItemText(nValue, 4, pCircuit->sBlock);
				m_pListCtrl->SetItemText(nValue, 5, pCircuit->sStair);
				m_pListCtrl->SetItemText(nValue, 6, pCircuit->sFloor);
				int nCNoIndex = CCircuitBasicInfo::Instance()->GetIndexCircuitName(arrayList.GetAt(i));
				//m_pListCtrl->SetItemText(nValue, 7, g_lpszCircuitNo[nCNoIndex]);
				if (pCircuit->nCount > 1) {
					m_pListCtrl->SetItemText(nValue, 8, sCircuitCount);
				}
				++nValue;
			}
			switch (nCircuitIndex) {
			//case 2: // 알람밸브 PS 1, TS 1
			//case 5: // 전실 제연 급기 2
			//case 6: // 전실 제연 배기 2
			//case 15: // 저수위 감시 2
			//	break;
			//case 3: // 프리액션 밸브 4
			//case 7: // 상가 제연 4
			//case 8: // 방화셔터 4
			//	break;
				//sTemp.Format(L"%d", nValue + 1 - 2);
				//m_pListCtrl->SetItemText(nValue - 2, 0, sTemp);
				//break;
			case 4: // 시각경보1 //가스소화설비 4
			case 9: // 배연창 1
			case 10: // 방화문 1
			case 13: // 소화전 기동확인 1
			case 14: // 옥상 개폐 확인 1
			case 0: // 소화전 1 
			case 1: // 발신기 1
			case 11: // 창폐 장치 1
			case 12: // 자폐 장치 1
			case 16: // 급기팬
			case 17: // 배기팬
				nValue++;
				break;
				//bFan = true;
				break;
			case 18: // 감지기 1
				/*if (nValue % 2 == 1) {
					nValue++;
				}*/
				break;
			default: break;
			}
		}
		if (nValue > MAX_CIRCUIT) {
			return false;
		}
	}
	return true;
#endif
	//20221102 GBM end
}

void CCircuitChartDlg::Redisplay()
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

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_pListCtrl) {
		rt.CopyRect(&rect);

		m_pListCtrl->MoveWindow(rt);
		m_pListCtrl->ShowWindow(SW_SHOW);
		m_pListCtrl->Invalidate();
	}
}

void CCircuitChartDlg::SetBackgroundColorAfterMaxCircuit(int nChartIndex)
{
	m_pListCtrl->SetBackgroundColorAfterMaxCircuit(nChartIndex);
}