// CircuitSetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CircuitSetupDlg.h"
#include "afxdialogex.h"


// CCircuitSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCircuitSetupDlg, CDialogEx)

CCircuitSetupDlg::CCircuitSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	for (int nIndex = 0; nIndex < STAIR_TAB_COUNT; nIndex++) {
		m_pTabButton[nIndex] = NULL;
		m_p_List[nIndex] = NULL;
	}

	m_bInit = false;
}

CCircuitSetupDlg::~CCircuitSetupDlg()
{
	for (int nIndex = 0; nIndex < STAIR_TAB_COUNT; nIndex++) {
		SAFE_DELETE(m_pTabButton[nIndex]);
	}
}

void CCircuitSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircuitSetupDlg, CDialogEx)
	ON_MESSAGE(TEXTTAB_BUTTON_CLICK, OnTabButtnClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCircuitSetupDlg 메시지 처리기입니다.

BOOL CCircuitSetupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	int nX = 20;
	CString sText;
	for (int nIndex = 0; nIndex < STAIR_TAB_COUNT; nIndex++) {
		sText.Format(L"%d계단", nIndex + 1);
		m_pTabButton[nIndex] = new CTextTabBtn(nIndex, sText, this);
		m_pTabButton[nIndex]->Create(IDD_COMMON_CHILD_DIALOG, this);
		if (nIndex < CCircuitBasicInfo::Instance()->m_nStair) {
			m_pTabButton[nIndex]->ShowWindow(SW_SHOW);
		}
		else {
			m_pTabButton[nIndex]->ShowWindow(SW_HIDE);
		}
		m_pTabButton[nIndex]->MoveWindow(nX, 3, 100, 37);

		if (nIndex == 0) {
			m_pTabButton[nIndex]->SetCheck(true);
		}
		nX += 105;
	}

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCircuitSetupDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CCircuitSetupDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CCircuitSetupDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCircuitSetupDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircuitSetupDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircuitSetupDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCircuitSetupDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	int nCount = CCircuitBasicInfo::Instance()->m_nStair * CCircuitBasicInfo::Instance()->m_nBlock;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		m_pTabButton[nIndex]->Invalidate();
	}
}

void CCircuitSetupDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Redisplay();
}

void CCircuitSetupDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

CCustomListCtrl* CCircuitSetupDlg::NewCircuitListCtrl(CCustomListCtrl* pListCtrl)
{
	if (pListCtrl) {
		pListCtrl->ShowWindow(SW_HIDE);
		pListCtrl->DestroyWindow();
	}
	pListCtrl = CreateListCtrl();

	int nFloor = CCircuitBasicInfo::Instance()->m_nBasement;
	nFloor += CCircuitBasicInfo::Instance()->m_nFloor;
	pListCtrl->AddItem(nFloor);
	int nItemIndex = 0;
	CString sText;
	for (int nIndex = CCircuitBasicInfo::Instance()->m_nBasement; nIndex > 0; nIndex--)
	{
		sText.Format(L"B%dF", nIndex);
		pListCtrl->SetItemText(nItemIndex, 0, sText);
		for (int i = 1; i < m_arrayHeaderName.GetCount(); i++) {
			pListCtrl->SetItemText(nItemIndex, i, L"0");
		}
		++nItemIndex;
	}
	for (int nIndex = 1; nIndex <= CCircuitBasicInfo::Instance()->m_nFloor; nIndex++)
	{
		sText.Format(L"%dF", nIndex);
		pListCtrl->SetItemText(nItemIndex, 0, sText);
		for (int i = 1; i < m_arrayHeaderName.GetCount(); i++) {
			pListCtrl->SetItemText(nItemIndex, i, L"0");
		}
		++nItemIndex;
	}
	return pListCtrl;
}

//20221013 GBM start - 메모리에 저장되어 있는 기존 설비 개수 정보를 리스트에 표현
void CCircuitSetupDlg::FillDataInCircuitListCtrl(int nSystem)
{
	CString sSystem = L"0 계통";
	if (nSystem == 1) {
		sSystem = L"1 계통";
	}
	CString sBlock, sStair, sTemp, sFloor;
	pSelectCircuit pCircuit = NULL;
	CCustomListCtrl* pList = NULL;
	CStringArray header;
	int nCount = 0;
	MakeHeader(header);

	int nBlockIndex = 0, nStairIndex = 0;
	bool bExit = false;
	while (true)
	{
		if (bExit) {
			break;
		}
		if (CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount() > 0
			&& CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount() < nBlockIndex + 1) {
			break;
		}
		if (CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount() == 0) {
			bExit = true;
		}
		if (CCircuitBasicInfo::Instance()->m_nBlock > 0) {
			sBlock = CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nBlockIndex);
			sBlock += L"동";
		}
		++nBlockIndex;

		for (int nIndex = 0; nIndex < CCircuitBasicInfo::Instance()->m_nStair; nIndex++) {
			sStair.Format(L"%d계단", nIndex + 1);

			pList = m_p_List[nStairIndex];
			++nStairIndex;
			for (int i = 0; i < pList->GetItemCount(); i++) {
				for (int ii = 0; ii < header.GetCount(); ii++) {
					pList->GetItemText(i, ii, sTemp);
					nCount = _ttoi(sTemp.GetBuffer(0));
					if (ii == 0) {
						pList->GetItemText(i, 0, sFloor);
						continue;
					}
					//else if (nCount == 0) {
					//	continue;
					//}

					pList->SetItemText(i, ii, L"0");

					CString sCircuitName = L"";
					if (nSystem == 0)
					{
						for (int j = 0; j < CCommonState::ie()->m_selectCircuit_0.GetCount(); j++)
						{
							pSelectCircuit pCircuit = NULL;
							pCircuit = CCommonState::ie()->m_selectCircuit_0.GetAt(CCommonState::ie()->m_selectCircuit_0.FindIndex(j));

							//계통, 동, 계단, 층, 설비명이 일치하는 기존 정보가 있다면 그 개수를 채움							
							sCircuitName = header.GetAt(ii);
							if((sSystem.Compare(pCircuit->sSystem) == 0) 
								&& (sBlock.Compare(pCircuit->sBlock) == 0) 
								&& (sStair.Compare(pCircuit->sStair) == 0) 
								&& (sFloor.Compare(pCircuit->sFloor) == 0) 
								&& (sCircuitName.Compare(pCircuit->sCircuitName) == 0))
							{
								CString strNum = L"";
								strNum.Format(L"%d", pCircuit->nCount);
								pList->SetItemText(i, ii, strNum);
							}
						}						
					}
					else
					{
						for (int j = 0; j < CCommonState::ie()->m_selectCircuit_1.GetCount(); j++)
						{
							pSelectCircuit pCircuit = NULL;
							pCircuit = CCommonState::ie()->m_selectCircuit_1.GetAt(CCommonState::ie()->m_selectCircuit_1.FindIndex(j));

							//계통, 동, 계단, 층, 설비명이 일치하는 기존 정보가 있다면 그 개수를 채움
							sCircuitName = header.GetAt(ii);
							if ((sSystem.Compare(pCircuit->sSystem) == 0)
								&& (sBlock.Compare(pCircuit->sBlock) == 0)
								&& (sStair.Compare(pCircuit->sStair) == 0)
								&& (sFloor.Compare(pCircuit->sFloor) == 0)
								&& (sCircuitName.Compare(pCircuit->sCircuitName) == 0))
							{
								CString strNum = L"";
								strNum.Format(L"%d", pCircuit->nCount);
								pList->SetItemText(i, ii, strNum);
							}
						}
					}
				}
			}
		}
	}
}
//20221013 GBM end

void CCircuitSetupDlg::InitCircuitInfo(int nSystem)
{
	for (int i = 0; i < STAIR_TAB_COUNT; i++) {
		m_pTabButton[i]->SetCheck(false);
	}
	m_pTabButton[0]->SetCheck(true);

	m_sync.Enter();

	m_arrayHeaderName.RemoveAll();
	m_arrayHeaderWidth.RemoveAll();
	m_arrayHeaderType.RemoveAll();
	m_arrayHeaderAlign.RemoveAll();
	m_arrayHeaderLimit.RemoveAll();

	MakeHeader(m_arrayHeaderName);
	MakeHeaderWidth(m_arrayHeaderWidth);
	MakeHeaderType(m_arrayHeaderType);
	MakeHeaderAlign(m_arrayHeaderAlign);
	MakeHeaderLimit(m_arrayHeaderLimit);

	int nCount = CCircuitBasicInfo::Instance()->m_nStair;
	if (CCircuitBasicInfo::Instance()->m_nBlock > 0) {
		nCount *= CCircuitBasicInfo::Instance()->m_nBlock;
	}
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		switch (nIndex) {
		case 0:
			m_p_List[0] = NewCircuitListCtrl(m_p_List[0]);
			m_p_List[0]->ShowWindow(SW_SHOW);
			break;
		default:
			m_p_List[nIndex] = NewCircuitListCtrl(m_p_List[nIndex]);
			m_p_List[nIndex]->ShowWindow(SW_HIDE);
			break;
		}
	}

	int nCircuitCnt = 0;
	if (nSystem == 0)
	{
		nCircuitCnt = CCommonState::ie()->m_selectCircuit_0.GetCount();
		FillDataInCircuitListCtrl(0);
	}
	else
	{
		nCircuitCnt = CCommonState::ie()->m_selectCircuit_1.GetCount();
		FillDataInCircuitListCtrl(1);
	}

	m_sync.Leave();
}

CCustomListCtrl* CCircuitSetupDlg::GetStairValue(CString sBlock, CString sStair)
{
	CString sTemp, sTemp1;
	for (int nIndex = 0; nIndex < CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount(); nIndex++) {
		sTemp = CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nIndex);
		sTemp += L"동";
		if (sTemp == sBlock) {
			for (int i = 0; i < CCircuitBasicInfo::Instance()->m_nStair; i++) {
				sTemp1.Format(L"%d계단", i + 1);
				if (sTemp1 == sStair) {
					int nValue = (nIndex * CCircuitBasicInfo::Instance()->m_nStair) + i;
					return m_p_List[nValue];
				}
			}
		}
	}

	/*if (sStair == L"1계단") {
		return m_p_1_List;
	}
	else if (sStair == L"2계단") {
		return m_p_2_List;
	}
	return m_p_3_List;*/
	return NULL;
}

int CCircuitSetupDlg::GetFloorValue(CCustomListCtrl* p_List, CString sFloor)
{
	if (!p_List) {
		return -1;
	}
	CString sTemp;
	int nCount = p_List->GetItemCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		p_List->GetItemText(nIndex, 0, sTemp);
		if (sTemp == sFloor) {
			return nIndex;
		}
	}
	return -1;
}

int CCircuitSetupDlg::GetCircuitValue(CString sCircuit)
{
	CString sName;
	int nCount = m_arrayHeaderName.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		sName = m_arrayHeaderName.GetAt(nIndex);
		if (sName == sCircuit) {
			return nIndex;
		}
	}
	return -1;
}

bool CCircuitSetupDlg::LoadInfo(int nSystem)
{
	CString sSystem, sCount;
	sSystem.Format(L"%d 계통", nSystem);
	int nCount = 0;
	switch (nSystem) {
	case 0: nCount = CCommonState::ie()->m_selectCircuit_0.GetCount(); break;
	case 1: nCount = CCommonState::ie()->m_selectCircuit_1.GetCount(); break;
	default: break;
	}
	pSelectCircuit pCircuit = NULL;
	CCustomListCtrl* pList = NULL;

	int nFloor = CCircuitBasicInfo::Instance()->m_nBasement;
	nFloor += CCircuitBasicInfo::Instance()->m_nFloor;

	int nListIndex = 0;
	int nFloorIndex = 0;
	int nCircuitIndex = 0;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		switch (nSystem) {
		case 0: pCircuit = CCommonState::ie()->m_selectCircuit_0.GetAt(CCommonState::ie()->m_selectCircuit_0.FindIndex(nIndex)); break;
		case 1: pCircuit = CCommonState::ie()->m_selectCircuit_1.GetAt(CCommonState::ie()->m_selectCircuit_1.FindIndex(nIndex)); break;
		default: break;
		}
		
		if (pCircuit->sSystem == sSystem) {
			pList = GetStairValue(pCircuit->sBlock, pCircuit->sStair);
			nFloorIndex = GetFloorValue(pList, pCircuit->sFloor);
			nCircuitIndex = GetCircuitValue(pCircuit->sCircuitName);
			if (!pList || nFloorIndex < 0 || nCircuitIndex < 0) {
				return false;
			}
			sCount.Format(L"%d", pCircuit->nCount);
			pList->SetItemText(nFloorIndex, nCircuitIndex, sCount);
		}
	}
	return true;
}

void CCircuitSetupDlg::MakeHeader(CStringArray & arrayList)
{
	arrayList.RemoveAll();
	arrayList.Add(L"구분");
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		if (!CCircuitBasicInfo::Instance()->m_bCheck[nIndex]) {
			continue;
		}
		if (nIndex == CIRCUIT_PARENT - 1) {
			CStringArray listChild;
			if (CCircuitBasicInfo::Instance()->GetCircuitChild(CCircuitBasicInfo::Instance()->GetCircuitParent(nIndex), listChild)) {
				for (int i = 0; i < listChild.GetCount(); i++) {
					arrayList.Add(listChild.GetAt(i));
					TRACE(L"==> %s\n", listChild.GetAt(i));
				}
			}
		}
		else {
			arrayList.Add(CCircuitBasicInfo::Instance()->GetCircuitParent(nIndex));
		}
	}
}

void CCircuitSetupDlg::MakeHeaderWidth(CUIntArray & arrayList)
{
	int nCount = m_arrayHeaderName.GetCount();
	--nCount;
	arrayList.Add(50);
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		arrayList.Add(70);
	}
}

void CCircuitSetupDlg::MakeHeaderType(CUIntArray & arrayList)
{
	int nCount = m_arrayHeaderName.GetCount();
	--nCount;
	arrayList.Add(POPUP_TYPE_NONE);
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		arrayList.Add(POPUP_TYPE_EDIT);
	}
}

void CCircuitSetupDlg::MakeHeaderAlign(CUIntArray & arrayList)
{
	int nCount = m_arrayHeaderName.GetCount();
	--nCount;
	arrayList.Add(DT_CENTER);
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		arrayList.Add(DT_CENTER);
	}
}

void CCircuitSetupDlg::MakeHeaderLimit(CUIntArray & arrayList)
{
	int nCount = m_arrayHeaderName.GetCount();
	--nCount;
	arrayList.Add(0);
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		arrayList.Add(6);
	}
}

CCustomListCtrl* CCircuitSetupDlg::CreateListCtrl()
{
	CCustomListCtrl* pList;
	CRuntimeClass	*pRuntimeClass;
	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomListCtrl))) {
		if (NULL != (pList = (CCustomListCtrl *)(pRuntimeClass->CreateObject()))) {
			pList->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}
	pList->SetListIndex(0);
	pList->SetHeaderInfo(m_arrayHeaderName, m_arrayHeaderWidth, m_arrayHeaderType, m_arrayHeaderAlign); // Align: option, row align text, default: DT_LEFT
	pList->SetColumnLimit(m_arrayHeaderLimit);
	// 높이 변경 (option)
	pList->SetHeaderHeight(50);
	pList->SetRowHeight(30);
	//pList->SetListPopupSize(500, 500);
	// 색변경 (option)
	pList->SetHeaderBkColor(RGB(80, 80, 80), RGB(255, 255, 255));
	pList->SetHeaderTextColor(RGB(220, 220, 220));
	pList->SetRowBkColor(RGB(160, 160, 160), RGB(255, 255, 0), RGB(255, 255, 255));
	pList->SetRowTextColor(RGB(255, 255, 255), RGB(80, 80, 80));
	pList->SetHeaderFontSize(14);
	pList->FixHeaderSize(false);
	pList->SetRowHeight(30);
	pList->SetAutoHeaderSize(true);
	pList->SetDisableMenu(true);

	pList->SendMessage(WM_INITIALUPDATE);

	CRect rect;
	GetClientRect(&rect);

	rect.top += 49;
	rect.left += 10;
	rect.right -= 10;
	rect.bottom -= 10;

	pList->MoveWindow(rect);

	return pList;
}

LRESULT CCircuitSetupDlg::OnTabButtnClick(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < STAIR_TAB_COUNT; i++) {
		m_pTabButton[i]->SetCheck(false);
	}
	int nIndex = (int)wParam;
	m_pTabButton[nIndex]->SetCheck(true);

	for (int i = 0; i < STAIR_TAB_COUNT; i++) {
		if (nIndex != i && m_p_List[i]) {
			m_p_List[i]->ShowWindow(SW_HIDE);
		}
		else if(m_p_List[i]){
			m_p_List[i]->ShowWindow(SW_SHOW);
		}
	}
	return 0;
}

bool CCircuitSetupDlg::CompareNewCircuitInfo(int nSystem)
{
	CString sSystem = L"0 계통";
	if (nSystem == 1) {
		sSystem = L"1 계통";
	}
	CString sBlock, sStair, sTemp, sFloor;
	pSelectCircuit pCircuit = NULL;
	CCustomListCtrl* pList = NULL;
	CStringArray header;
	int nCount = 0;
	int nCheckIndex = 0;
	MakeHeader(header);
	selectCircuit temp;

	int nBlockIndex = 0, nStairIndex = 0;
	bool bExit = false;
	while (true)
	{
		if (bExit) {
			break;
		}
		if (CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount() > 0
			&& CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount() < nBlockIndex + 1) {
			break;
		}
		if (CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount() == 0) {
			bExit = true;
		}
		if (CCircuitBasicInfo::Instance()->m_nBlock > 0) {
			sBlock = CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nBlockIndex);
			sBlock += L"동";
		}
		++nBlockIndex;

		for (int nIndex = 0; nIndex < CCircuitBasicInfo::Instance()->m_nStair; nIndex++) {
			sStair.Format(L"%d계단", nIndex + 1);

			pList = m_p_List[nStairIndex];
			++nStairIndex;
			for (int i = 0; i < pList->GetItemCount(); i++) {
				for (int ii = 0; ii < header.GetCount(); ii++) {
					pList->GetItemText(i, ii, sTemp);
					nCount = _ttoi(sTemp.GetBuffer(0));
					if (ii == 0) {
						pList->GetItemText(i, 0, sFloor);
						continue;
					}
					//20221020 GBM start -  이어도 비교하도록 수정
// 					else if (nCount == 0) {
// 						continue;
// 					}
					//20221020 GBM end
					temp.sSystem = sSystem;
					temp.sStair = sStair;
					temp.sBlock = sBlock;
					temp.sFloor = sFloor;
					temp.sCircuitName = header.GetAt(ii);
					temp.nCount = nCount;

					if (nSystem == 0) {
						if (CCommonState::ie()->m_selectCircuit_0.GetCount() <= nCheckIndex) {
							return false;
						}
						pSelectCircuit pOriginal = CCommonState::ie()->m_selectCircuit_0.GetAt(CCommonState::ie()->m_selectCircuit_0.FindIndex(nCheckIndex));
						if (!CompareCircuit(pOriginal, &temp)) {
							return false;
						}
					}
					else if (nSystem == 1) {
						if (CCommonState::ie()->m_selectCircuit_1.GetCount() <= nCheckIndex) {
							return false;
						}
						pSelectCircuit pOriginal = CCommonState::ie()->m_selectCircuit_1.GetAt(CCommonState::ie()->m_selectCircuit_1.FindIndex(nCheckIndex));
						if (!CompareCircuit(pOriginal, &temp)) {
							return false;
						}
					}
					++nCheckIndex;
				}
			}
		}
	}
	
	if (nSystem == 0) {
		if (CCommonState::ie()->m_selectCircuit_0.GetCount() != nCheckIndex) {
			return false;
		}
	}
	else if (nSystem == 1) {
		if (CCommonState::ie()->m_selectCircuit_1.GetCount() != nCheckIndex) {
			return false;
		}
	}

	return true;
}

bool CCircuitSetupDlg::CompareCircuit(pSelectCircuit pCircuit1, pSelectCircuit pCircuit2)
{
	if (pCircuit1->sCircuitName != pCircuit2->sCircuitName || pCircuit1->sBlock != pCircuit2->sBlock || 
		pCircuit1->sFloor != pCircuit2->sFloor || pCircuit1->sStair != pCircuit2->sStair || 
		pCircuit1->sSystem != pCircuit2->sSystem ||	pCircuit1->nCount != pCircuit2->nCount) {
		return false;
	}
	return true;
}

bool CCircuitSetupDlg::SaveCircuitInfo(int nSystem)
{
	CCommonState::ie()->ReleaseCircuit(nSystem);

	CString sSystem = L"0 계통";
	if (nSystem == 1) {
		sSystem = L"1 계통";
	}
	CString sBlock, sStair, sTemp, sFloor;
	pSelectCircuit pCircuit = NULL;
	CCustomListCtrl* pList = NULL;
	CStringArray header;
	int nCount = 0;
	MakeHeader(header);

	int nBlockIndex = 0, nStairIndex = 0;
	bool bExit = false;
	while (true)
	{
		if (bExit) {
			break;
		}
		if (CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount() > 0
			&& CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount() < nBlockIndex + 1) {
			break;
		}
		if (CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount() == 0) {
			bExit = true;
		}
		if (CCircuitBasicInfo::Instance()->m_nBlock > 0) {
			sBlock = CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nBlockIndex);
			sBlock += L"동";
		}
		++nBlockIndex;

		for (int nIndex = 0; nIndex < CCircuitBasicInfo::Instance()->m_nStair; nIndex++) {
			sStair.Format(L"%d계단", nIndex + 1);

			pList = m_p_List[nStairIndex];
			++nStairIndex;
			for (int i = 0; i < pList->GetItemCount(); i++) {
				for (int ii = 0; ii < header.GetCount(); ii++) {
					pList->GetItemText(i, ii, sTemp);
					nCount = _ttoi(sTemp.GetBuffer(0));
					if (ii == 0) {
						pList->GetItemText(i, 0, sFloor);
						continue;
					}
					//기존 설비 개수 체크를 위해 개수가 0이라도 추가
// 					else if (nCount == 0) {
// 						continue;
// 					}
					pCircuit = new selectCircuit;
					pCircuit->sSystem = sSystem;
					pCircuit->sBlock = sBlock;
					pCircuit->sStair = sStair;
					pCircuit->sFloor = sFloor;
					pCircuit->sCircuitName = header.GetAt(ii);
					pCircuit->nCount = nCount;

					if (nSystem == 0) {
						CCommonState::ie()->m_selectCircuit_0.AddTail(pCircuit);
					}
					else if (nSystem == 1) {
						CCommonState::ie()->m_selectCircuit_1.AddTail(pCircuit);
					}
				}
			}
		}
	}
	return true;
}

void CCircuitSetupDlg::Redisplay()
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
	rt.top += 39;
	CCommonDisplay::DrawRect(&memDC, false, RGB(220, 220, 220), 0, rt);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInit) {
		CString sStair;
		int nTabCount = CCircuitBasicInfo::Instance()->m_nStair;
		if (CCircuitBasicInfo::Instance()->m_nBlock > 0) {
			nTabCount *= CCircuitBasicInfo::Instance()->m_nBlock;
		}
		for (int nIndex = 0; nIndex < nTabCount; nIndex++) {
			int nBlockIndex = (nIndex > 0) ? (nIndex / CCircuitBasicInfo::Instance()->m_nStair) : 0;
			if (CCircuitBasicInfo::Instance()->m_nBlock > 0 
				&& CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount() == CCircuitBasicInfo::Instance()->m_nBlock) {
				sStair.Format(L"%s동 %d계단",
					CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nBlockIndex)
					, ((nIndex > 0) ? (nIndex % CCircuitBasicInfo::Instance()->m_nStair) : 0) + 1);
			}
			else {
				sStair.Format(L"%d계단", nIndex + 1);
			}
			m_pTabButton[nIndex]->SetCaption(sStair);
		}

		for (int nIndex = 0; nIndex < STAIR_TAB_COUNT; nIndex++) {
			if (nIndex < nTabCount) {
				m_pTabButton[nIndex]->ShowWindow(SW_SHOW);
			}
			else {
				m_pTabButton[nIndex]->ShowWindow(SW_HIDE);
			}
		}
		GetClientRect(&rect);

		rect.top += 49;
		rect.left += 10;
		rect.right -= 10;
		rect.bottom -= 10;

		for (int i = 0; i < STAIR_TAB_COUNT; i++) {
			if (m_p_List[i]) {
				m_p_List[i]->MoveWindow(rect);
			}
		}
	}
}

void CCircuitSetupDlg::CopyNewCircuitInfoToOldCircuitInfo(int nSystem)
{
	std::vector<selectCircuitComp*>::iterator iter;
	POSITION pos;
	pSelectCircuit pSC;
	CString sBlock, sStair, sFloor, sCircuitName;
	bool bFind = false;

	CString sSystem = L"0 계통";
	if (nSystem == 1) {
		sSystem = L"1 계통";
	}

	if (nSystem == 0)
	{
		iter = CCommonState::ie()->m_vecCalSelectCircuit_0.begin();
		for (; iter != CCommonState::ie()->m_vecCalSelectCircuit_0.end(); iter++)
		{
			bFind = false;

			//기존 현재 값을 이전 값에 적용
			(*iter)->nPreviousCount = (*iter)->nCurrentCount;

			//현재 리스트에서 편집된 현재 값을 비교를 위한 현재 값에 적용
			pos = CCommonState::ie()->m_selectCircuit_0.GetHeadPosition();

			while (pos)
			{
				pSC = CCommonState::ie()->m_selectCircuit_0.GetNext(pos);

				sBlock = (*iter)->sBlock;
				sStair = (*iter)->sStair;
				sFloor = (*iter)->sFloor;
				sCircuitName = (*iter)->sCircuitName;

				if ((sSystem.Compare(pSC->sSystem) == 0)
					&& (sBlock.Compare(pSC->sBlock) == 0)
					&& (sStair.Compare(pSC->sStair) == 0)
					&& (sFloor.Compare(pSC->sFloor) == 0)
					&& (sCircuitName.Compare(pSC->sCircuitName) == 0)
					)
				{
					(*iter)->nCurrentCount = pSC->nCount;
					bFind = true;
					break;
				}
			}

			if (!bFind)
			{
				if ((*iter)->nCurrentCount > 0)
				{
					(*iter)->nCurrentCount = 0;
				}
			}
		}
	}
	else
	{
		iter = CCommonState::ie()->m_vecCalSelectCircuit_1.begin();
		for (; iter != CCommonState::ie()->m_vecCalSelectCircuit_1.end(); iter++)
		{
			bFind = false;

			//기존 현재 값을 이전 값에 적용
			(*iter)->nPreviousCount = (*iter)->nCurrentCount;

			//현재 리스트에서 편집된 현재 값을 비교를 위한 현재 값에 적용
			pos = CCommonState::ie()->m_selectCircuit_1.GetHeadPosition();

			while (pos)
			{
				pSC = CCommonState::ie()->m_selectCircuit_1.GetNext(pos);

				sBlock = (*iter)->sBlock;
				sStair = (*iter)->sStair;
				sFloor = (*iter)->sFloor;
				sCircuitName = (*iter)->sCircuitName;

				if ((sSystem.Compare(pSC->sSystem) == 0)
					&& (sBlock.Compare(pSC->sBlock) == 0)
					&& (sStair.Compare(pSC->sStair) == 0)
					&& (sFloor.Compare(pSC->sFloor) == 0)
					&& (sCircuitName.Compare(pSC->sCircuitName) == 0)
					)
				{
					(*iter)->nCurrentCount = pSC->nCount;
					bFind = true;
					break;
				}
			}

			if (!bFind)
			{
				if ((*iter)->nCurrentCount > 0)
				{
					(*iter)->nCurrentCount = 0;
				}
			}
		}
	}

}

void CCircuitSetupDlg::MakeAddCircuitInfoAndDeleteCircuitInfo(int nSystem)
{
	CString sSystem = L"0 계통";
	if (nSystem == 1) {
		sSystem = L"1 계통";
	}

	std::vector<selectCircuitCompRet> vecAddSelectCircuit;
	std::vector<selectCircuitCompRet> vecDeleteSelectCircuit;

	std::vector<selectCircuitComp*>::iterator iter;
	bool bDetector;

	if (nSystem == 0)
	{
		iter = CCommonState::ie()->m_vecCalSelectCircuit_0.begin();
		for (; iter != CCommonState::ie()->m_vecCalSelectCircuit_0.end(); iter++)
		{
			int nCurrentCount = 0;
			int nPrivousCount = 0;

			nCurrentCount = (*iter)->nCurrentCount;
			nPrivousCount = (*iter)->nPreviousCount;

			bDetector = false;

			if (nCurrentCount != nPrivousCount)
			{
				int nDiffCount = nCurrentCount - nPrivousCount;
				selectCircuitCompRet sccr;
				sccr.sSystem = sSystem;
				sccr.sBlock = (*iter)->sBlock;
				sccr.sStair = (*iter)->sStair;
				sccr.sFloor = (*iter)->sFloor;
				sccr.sCircuitName = (*iter)->sCircuitName;
				sccr.nDiffCount = nDiffCount;
				sccr.nLastCircuitNo = nPrivousCount;
				sccr.vecSystemName.clear();

				//설비에 해당하는 회로 리스트 작성-> 감지기가 아니면 설비명, 감지기이면 회로명을 넣는다.
				bDetector = CCircuitBasicInfo::Instance()->IsDetector(sccr.sCircuitName);
				if (!bDetector)
				{
					CStringArray strArr;
					strArr.RemoveAll();
					CCircuitBasicInfo::Instance()->GetCircuitChild(sccr.sCircuitName, strArr);
					
					for (int i = 0; i < strArr.GetSize(); i++)
					{
						sccr.vecSystemName.push_back(strArr[i]);
					}
				}

				//회로 갯수에 따라 분기
				if (nDiffCount > 0)
				{
					CCommonState::ie()->m_vecAddCircuit.push_back(sccr);
				}
				else
				{
					CCommonState::ie()->m_vecDeleteCircuit.push_back(sccr);
				}
			}
		}
	}
	else
	{
		iter = CCommonState::ie()->m_vecCalSelectCircuit_1.begin();
		for (; iter != CCommonState::ie()->m_vecCalSelectCircuit_1.end(); iter++)
		{
			int nCurrentCount = 0;
			int nPrivousCount = 0;

			nCurrentCount = (*iter)->nCurrentCount;
			nPrivousCount = (*iter)->nPreviousCount;

			if (nCurrentCount != nPrivousCount)
			{
				int nDiffCount = nCurrentCount - nPrivousCount;
				selectCircuitCompRet sccr;
				sccr.sSystem = sSystem;
				sccr.sBlock = (*iter)->sBlock;
				sccr.sStair = (*iter)->sStair;
				sccr.sFloor = (*iter)->sFloor;
				sccr.sCircuitName = (*iter)->sCircuitName;
				sccr.nDiffCount = nDiffCount;
				sccr.nLastCircuitNo = nPrivousCount;
				sccr.vecSystemName.clear();

				//설비에 해당하는 회로 리스트 작성-> 감지기가 아니면 설비명, 감지기이면 회로명을 넣는다.
				bDetector = CCircuitBasicInfo::Instance()->IsDetector(sccr.sCircuitName);
				if (!bDetector)
				{
					CStringArray strArr;
					strArr.RemoveAll();
					CCircuitBasicInfo::Instance()->GetCircuitChild(sccr.sCircuitName, strArr);

					for (int i = 0; i < strArr.GetSize(); i++)
					{
						sccr.vecSystemName.push_back(strArr[i]);
					}
				}

				//회로 갯수에 따라 분기
				if (nDiffCount > 0)
				{
					CCommonState::ie()->m_vecAddCircuit.push_back(sccr);
				}
				else
				{
					CCommonState::ie()->m_vecDeleteCircuit.push_back(sccr);
				}
			}
		}
	}

}

bool CCircuitSetupDlg::CheckMaxCircuitCount(int nSystem, int* nTotalCount)
{
	bool bRet = true;

	*nTotalCount = CCommonState::ie()->CalculateTotalCircuitCount(nSystem);
	if (*nTotalCount > MAX_CIRCUIT)
		bRet = false;

	return bRet;
}

void CCircuitSetupDlg::RollbackCircuitCount(int nSystem)
{
	std::vector<selectCircuitComp*>::iterator iter;
	POSITION pos;
	pSelectCircuit pSC;
	CString sSystem, sBlock, sStair, sFloor, sCircuitName;

	if (nSystem == 0)
	{
		//기존 값을 현재 값에 대입
		iter = CCommonState::ie()->m_vecCalSelectCircuit_0.begin();
		for (; iter != CCommonState::ie()->m_vecCalSelectCircuit_0.end(); iter++)
		{
			pos = CCommonState::ie()->m_selectCircuit_0.GetHeadPosition();
			while (pos)
			{
				pSC = CCommonState::ie()->m_selectCircuit_0.GetNext(pos);
				sSystem = pSC->sSystem;
				sBlock = pSC->sBlock;
				sStair = pSC->sStair;
				sFloor = pSC->sFloor;
				sCircuitName = pSC->sCircuitName;

				//기존 값을 현재 값에 대입
				iter = CCommonState::ie()->m_vecCalSelectCircuit_0.begin();
				for (; iter != CCommonState::ie()->m_vecCalSelectCircuit_0.end(); iter++)
				{
					if ((sSystem.Compare((*iter)->sSystem) == 0)
						&& (sBlock.Compare((*iter)->sBlock) == 0)
						&& (sStair.Compare((*iter)->sStair) == 0)
						&& (sFloor.Compare((*iter)->sFloor) == 0)
						&& (sCircuitName.Compare((*iter)->sCircuitName) == 0)
						)
					{
						pSC->nCount = (*iter)->nCurrentCount;
						break;
					}
				}
			}
		}
	}
	else
	{
		//기존 값을 현재 값에 대입
		iter = CCommonState::ie()->m_vecCalSelectCircuit_1.begin();
		for (; iter != CCommonState::ie()->m_vecCalSelectCircuit_1.end(); iter++)
		{
			pos = CCommonState::ie()->m_selectCircuit_1.GetHeadPosition();
			while (pos)
			{
				pSC = CCommonState::ie()->m_selectCircuit_1.GetNext(pos);
				sSystem = pSC->sSystem;
				sBlock = pSC->sBlock;
				sStair = pSC->sStair;
				sFloor = pSC->sFloor;
				sCircuitName = pSC->sCircuitName;

				//기존 값을 현재 값에 대입
				iter = CCommonState::ie()->m_vecCalSelectCircuit_1.begin();
				for (; iter != CCommonState::ie()->m_vecCalSelectCircuit_1.end(); iter++)
				{
					if ((sSystem.Compare((*iter)->sSystem) == 0)
						&& (sBlock.Compare((*iter)->sBlock) == 0)
						&& (sStair.Compare((*iter)->sStair) == 0)
						&& (sFloor.Compare((*iter)->sFloor) == 0)
						&& (sCircuitName.Compare((*iter)->sCircuitName) == 0)
						)
					{
						pSC->nCount = (*iter)->nCurrentCount;
						break;
					}
				}
			}
		}
	}
}
