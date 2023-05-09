// BroadcastDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "BroadcastDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"

// CBroadcastDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBroadcastDlg, CDialogEx)

CBroadcastDlg::CBroadcastDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_font.CreateFont(16, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
#ifndef ENGLISH_MODE
		_T("굴림")); // lpszFacename
#else
		_T("arial")); // lpszFacename
#endif

	m_pListCtrl = NULL;

	m_pRadioBtn1 = NULL;
	m_pRadioBtn2 = NULL;
	m_pRadioBtn3 = NULL;
}

CBroadcastDlg::~CBroadcastDlg()
{
	SAFE_DELETE(m_pRadioBtn1);
	SAFE_DELETE(m_pRadioBtn2);
	SAFE_DELETE(m_pRadioBtn3);
}

void CBroadcastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBroadcastDlg, CDialogEx)
	ON_MESSAGE(LIST_CONTROL, OnListControl)
	ON_MESSAGE(RADIO_BUTTON_CLICK, OnRadioButtnClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CBroadcastDlg 메시지 처리기입니다.
#ifndef ENGLISH_MODE
	const TCHAR* g_editHeaderB[] = { _T("번호"), _T("건물명"), _T("동"), _T("계단"), _T("층"), _T("방송정보"), NULL };
#else
	const TCHAR* g_editHeaderB[] = { _T("No."), _T("B.NAME"), _T("B.BLOCK"), _T("LINE"), _T("FLOOR"), _T("PA.INFO"), NULL };
#endif

const int g_editSizeB[] = { 90, 180, 100, 100, 100, 220, 0 }; // pixel

	//20230419 GBM start - 방송정보를 제외한 컬럼은 임의 수정 불가
#if 1
	const int g_editTypeB[] = { POPUP_TYPE_SEQ , POPUP_TYPE_NONE, POPUP_TYPE_NONE, POPUP_TYPE_NONE, POPUP_TYPE_NONE, POPUP_TYPE_EDIT, 0 };
#else
	const int g_editTypeB[] = { POPUP_TYPE_SEQ , POPUP_TYPE_NONE, POPUP_TYPE_LIST_ADD, POPUP_TYPE_LIST_ADD, POPUP_TYPE_LIST_ADD, POPUP_TYPE_EDIT, 0 };
#endif
	//20230419 GBM end

const int g_editAlignB[] = { DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, -1 }; // option, row align text, default: DT_LEFT
const int g_editLimitB[] = { 0, 0, 0, 0, 0, 0, -1 }; // 0: 무제한, > 0 : 텍스트 사이즈 제한

BOOL CBroadcastDlg::OnInitDialog()
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
	m_pListCtrl->SetHeaderInfo((TCHAR**)g_editHeaderB, (int*)g_editSizeB, (int*)g_editTypeB, (int*)g_editAlignB); // Align: option, row align text, default: DT_LEFT
	m_pListCtrl->SetColumnLimit((int*)g_editLimitB);
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
	m_pListCtrl->EnableLastInsertRow(true, true);

	m_pListCtrl->SetDisableMenu(true);

	m_pListCtrl->SendMessage(WM_INITIALUPDATE);

#ifndef ENGLISH_MODE
	CString sCaption = L"12바이트 통신";
#else
	CString sCaption = L"12 BYTE";
#endif
	m_pRadioBtn1 = new CRadioTextBtn(0, sCaption, this);
	m_pRadioBtn1->Create(IDD_COMMON_CHILD_DIALOG, this);
	int nX = 495;
	m_pRadioBtn1->MoveWindow(nX, 5, m_pRadioBtn1->GetWidthExtent(sCaption), 22);
	nX += m_pRadioBtn1->GetWidthExtent(sCaption) + 15;
	m_pRadioBtn1->ShowWindow(SW_SHOW);

#ifndef ENGLISH_MODE
	sCaption = L"10바이트 통신";
#else
	sCaption = L"10 BYTE";
#endif
	m_pRadioBtn2 = new CRadioTextBtn(1, sCaption, this);
	m_pRadioBtn2->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pRadioBtn2->MoveWindow(nX, 5, m_pRadioBtn2->GetWidthExtent(sCaption), 22);
	nX += m_pRadioBtn2->GetWidthExtent(sCaption) + 15;
	m_pRadioBtn2->ShowWindow(SW_SHOW);

#ifndef ENGLISH_MODE
	sCaption = L"접점출력";
#else
	sCaption = L"REL OUT";
#endif
	m_pRadioBtn3 = new CRadioTextBtn(2, sCaption, this);
	m_pRadioBtn3->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pRadioBtn3->MoveWindow(nX, 5, m_pRadioBtn3->GetWidthExtent(sCaption), 22);
	nX += m_pRadioBtn3->GetWidthExtent(sCaption) + 15;
	m_pRadioBtn3->ShowWindow(SW_SHOW);

	m_pRadioBtn1->SetCheck(true);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CBroadcastDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CBroadcastDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CBroadcastDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CBroadcastDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CBroadcastDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CBroadcastDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CBroadcastDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CBroadcastDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CBroadcastDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

bool CBroadcastDlg::SaveInformation()
{
	CString strRooftop;
	strRooftop = ROOFTOP_NAME;

	if (!m_pListCtrl) {
		return false;
	}
	if (m_pRadioBtn1->GetCheck()) {
		CSaveManager::ie()->m_nBroadType = 0;
	}
	else if(m_pRadioBtn2->GetCheck()) {
		CSaveManager::ie()->m_nBroadType = 1;
	}
	else if (m_pRadioBtn3->GetCheck()) {
		CSaveManager::ie()->m_nBroadType = 2;
	}
	int nCount = m_pListCtrl->GetItemCount();
	CString sStair, sFloor, sBlock, sBRContent;
	int nStair = 1, nFloor;

	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		m_pListCtrl->GetItemText(nIndex, 2, sBlock);
		m_pListCtrl->GetItemText(nIndex, 3, sStair);
		m_pListCtrl->GetItemText(nIndex, 4, sFloor);
		m_pListCtrl->GetItemText(nIndex, 5, sBRContent);

#ifndef ENGLISH_MODE
		sStair.Replace(L"계단", L"");
#else
		sStair.Replace(L"LINE ", L"");
#endif
		nStair = _wtoi(sStair.GetBuffer(0));

		if (sFloor.Find(L"B") >= 0) {
			sFloor.Replace(L"B", L"");
			sFloor.Replace(L"F", L"");
			nFloor = -_wtoi(sFloor.GetBuffer(0));
		}
		else {
			if (sFloor.Compare(strRooftop) == 0)
			{
				nFloor = CCircuitBasicInfo::Instance()->m_nFloor + 1;
			}
			else
			{
				sFloor.Replace(L"F", L"");
				nFloor = _wtoi(sFloor.GetBuffer(0));
			}			
		}

#ifndef ENGLISH_MODE
		sBlock.Replace(L"동", L"");
#else
		sBlock.Replace(L"B.BLCK ", L"");
#endif

		CSaveManager::ie()->SetBroadcast(sBlock, nStair, nFloor, sBRContent.GetBuffer(0));
	}
	return true;
}

void CBroadcastDlg::SetupPopupList()
{
	CString sTemp;
	int nStairCount = CCircuitBasicInfo::Instance()->m_nStair;
	int nBasement = CCircuitBasicInfo::Instance()->m_nBasement;
	int nFloor = CCircuitBasicInfo::Instance()->m_nFloor;
	m_pListCtrl->RemovePopupListItem(2);
	for (int nIndex = 0; nIndex < CCircuitBasicInfo::Instance()->m_nBlock; nIndex++) {
		if (CCircuitBasicInfo::Instance()->m_nBlock > 1) {
			sTemp = CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nIndex);
#ifndef ENGLISH_MODE
			sTemp += L"동";
#else
			sTemp += L"B.BLCK ";
#endif
			m_pListCtrl->AddPopupListItem(2, sTemp);
		}
	}
	m_pListCtrl->RemovePopupListItem(3);
	if (nStairCount > 1) {
		for (int nIndex = 0; nIndex < nStairCount; nIndex++) {
#ifndef ENGLISH_MODE
			sTemp.Format(L"%d계단", nIndex + 1);
#else
			sTemp.Format(L"LINE %d", nIndex + 1);
#endif
			m_pListCtrl->AddPopupListItem(3, sTemp);
		}
	}
	m_pListCtrl->RemovePopupListItem(4);
	for (int nIndex = nBasement; nIndex > 0; nIndex--) {
		sTemp.Format(L"B%dF", nIndex);
		m_pListCtrl->AddPopupListItem(4, sTemp);
	}
	for (int nIndex = 0; nIndex < nFloor; nIndex++) {
		sTemp.Format(L"%dF", nIndex + 1);
		m_pListCtrl->AddPopupListItem(4, sTemp);
	}
}

void CBroadcastDlg::DisplayLoadFile()
{
	CString strRooftop;
	strRooftop = ROOFTOP_NAME;

	if (!m_pListCtrl) {
		return;
	}
	m_pListCtrl->ReleaseListItem();
	m_pListCtrl->DeleteAllItems();

	SetupPopupList();

	m_pRadioBtn1->SetCheck(false);
	m_pRadioBtn2->SetCheck(false);
	m_pRadioBtn3->SetCheck(false);
	switch (CSaveManager::ie()->m_nBroadType) {
	case 0: m_pRadioBtn1->SetCheck(true); break;
	case 1: m_pRadioBtn2->SetCheck(true); break;
	case 2: m_pRadioBtn3->SetCheck(true); break;
	default: break;
	}

	int nAllCount = CSaveManager::ie()->m_listBC.GetCount();
	m_pListCtrl->AddItem(nAllCount);
	CString sTemp, sStair, sBlock;
	BC_INFO* pInfo;
	for (int nIndex = 0; nIndex < nAllCount; nIndex++) {
		pInfo = CSaveManager::ie()->m_listBC.GetAt(CSaveManager::ie()->m_listBC.FindIndex(nIndex));

		m_pListCtrl->SetItemText(nIndex, 1, CCircuitBasicInfo::Instance()->m_sBuildingName);

#ifndef ENGLISH_MODE
		sBlock.Format(L"%s동", pInfo->szBlock);
		m_pListCtrl->SetItemText(nIndex, 2, sBlock);
		sStair.Format(L"%d계단", pInfo->nStair);
		m_pListCtrl->SetItemText(nIndex, 3, sStair);
#else
		sBlock.Format(L"B.BLCK %s", pInfo->szBlock);
		m_pListCtrl->SetItemText(nIndex, 2, sBlock);
		sStair.Format(L"LINE %d", pInfo->nStair);
		m_pListCtrl->SetItemText(nIndex, 3, sStair);
#endif
		if (pInfo->nFloor > 0) {
			if (pInfo->nFloor == CCircuitBasicInfo::Instance()->m_nFloor + 1)	//옥탑층
			{
				sTemp = strRooftop;
			}
			else
			{
				sTemp.Format(L"%dF", pInfo->nFloor);
			}
		}
		else {
			sTemp.Format(L"B%dF", abs(pInfo->nFloor));
		}
		m_pListCtrl->SetItemText(nIndex, 4, sTemp);
		m_pListCtrl->SetItemText(nIndex, 5, pInfo->BC_CONTAIN);
	}
}

void CBroadcastDlg::DisplayListItem()
{
	if (!m_pListCtrl) {
		return;
	}
	m_pListCtrl->ReleaseListItem();
	m_pListCtrl->DeleteAllItems();

	SetupPopupList();

	CString sTemp, sStair, sBlock;
	CString strRooftop;
	strRooftop = ROOFTOP_NAME;
	int nStairCount = CCircuitBasicInfo::Instance()->m_nStair;
	int nBasement = CCircuitBasicInfo::Instance()->m_nBasement;
	int nRooftop = CCircuitBasicInfo::Instance()->m_nRooftop;
	int nFloor = CCircuitBasicInfo::Instance()->m_nFloor;
	int nBlock = CCircuitBasicInfo::Instance()->m_nBlock;
	int nAllCount = (nBasement + nFloor + nRooftop) * nStairCount * ((nBlock <= 0) ? 1 : nBlock);
	m_pListCtrl->AddItem(nAllCount);

	int nListItem = 0;
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
#ifndef ENGLISH_MODE
			sBlock += L"동";
#else
			sBlock = L"B.BLCK " + sBlock;
#endif
		}
		++nBlockIndex;
		for (int nIndex = 0; nIndex < nStairCount; nIndex++) {
#ifndef ENGLISH_MODE
			sStair.Format(L"%d계단", nIndex + 1);
#else
			sStair.Format(L"LINE %d", nIndex + 1);
#endif
			for (int i = nBasement; i > 0; i--) {
				sTemp.Format(L"B%dF", i);
				m_pListCtrl->SetItemText(nListItem, 1, CCircuitBasicInfo::Instance()->m_sBuildingName);
				m_pListCtrl->SetItemText(nListItem, 2, sBlock);
				m_pListCtrl->SetItemText(nListItem, 3, sStair);
				m_pListCtrl->SetItemText(nListItem, 4, sTemp);
				++nListItem;
			}
			for (int i = 0; i < nFloor; i++) {
				sTemp.Format(L"%dF", i + 1);
				m_pListCtrl->SetItemText(nListItem, 1, CCircuitBasicInfo::Instance()->m_sBuildingName);
				m_pListCtrl->SetItemText(nListItem, 2, sBlock);
				m_pListCtrl->SetItemText(nListItem, 3, sStair);
				m_pListCtrl->SetItemText(nListItem, 4, sTemp);
				++nListItem;
			}
			for (int i = 0; i < nRooftop; i++) {
				sTemp = strRooftop;
				m_pListCtrl->SetItemText(nListItem, 1, CCircuitBasicInfo::Instance()->m_sBuildingName);
				m_pListCtrl->SetItemText(nListItem, 2, sBlock);
				m_pListCtrl->SetItemText(nListItem, 3, sStair);
				m_pListCtrl->SetItemText(nListItem, 4, sTemp);
				++nListItem;
			}
		}
	}
	BroadcastStandard();
}

void CBroadcastDlg::BroadcastStandard()
{
	int nType = 0;
	if (m_pRadioBtn2->GetCheck()) {
		nType = 1;
	}
	else if (m_pRadioBtn3->GetCheck()) {
		nType = 2;
	}
	switch (nType) {
	case 0:
		BroadcastStandardType1();
		break;
	case 1:
		BroadcastStandardType2();
		break;
	case 2:
		BroadcastStandardType3();
		break;
	default:
		break;
	}
}

int CBroadcastDlg::GetStairValue(CString sStair)
{
	int nStair = -1;

#ifndef ENGLISH_MODE
	if (sStair.Compare(L"1계단") == 0) {
		nStair = 1;
	}
	else if (sStair.Compare(L"2계단") == 0) {
		nStair = 2;
	}
	else if (sStair.Compare(L"3계단") == 0) {
		nStair = 3;
	}
#else
	if (sStair.Compare(L"LINE 1") == 0) {
		nStair = 1;
	}
	else if (sStair.Compare(L"LINE 2") == 0) {
		nStair = 2;
	}
	else if (sStair.Compare(L"LINE 3") == 0) {
		nStair = 3;
	}
#endif

	return nStair;
}

int CBroadcastDlg::GetFloorValue(CString sFloor)
{
	CString strRooftop;
	strRooftop = ROOFTOP_NAME;

	int nValue = 0;

	if (sFloor.Compare(strRooftop) == 0)	//옥탑
	{
		nValue = CCircuitBasicInfo::Instance()->m_nFloor + 1;	// 일반층 + 1 == 옥탑층
		sFloor.ReleaseBuffer();
		return nValue;
	}

	if (sFloor.Find(L"B") >= 0) {
		sFloor.Replace(L"B", L"");
		sFloor.Replace(L"F", L"");
		nValue = _wtoi(sFloor.GetBuffer(0));
		if (nValue == 0) {
			return 0;
		}
		sFloor.ReleaseBuffer();
		return -nValue;
	}

	sFloor.Replace(L"F", L"");
	nValue = _wtoi(sFloor.GetBuffer(0));
	if (nValue == 0) {
		return 0;
	}
	sFloor.ReleaseBuffer();
	return nValue;
}

void CBroadcastDlg::BroadcastStandardType1()
{
	CString sBlock, sFloor, sStair, sText, sTemp;
	int nBlock, nFloor, nStair;
	int nCount = m_pListCtrl->GetItemCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		sStair = L"";
		sFloor = L"";
		sBlock = L"";
		m_pListCtrl->GetItemText(nIndex, 2, sBlock);
		m_pListCtrl->GetItemText(nIndex, 3, sStair);
		m_pListCtrl->GetItemText(nIndex, 4, sFloor);
		if (sStair.GetLength() == 0 || sFloor.GetLength() == 0) {
			continue;
		}
		nStair = GetStairValue(sStair);
		if (nStair == -1) {
			continue;
		}
		nFloor = GetFloorValue(sFloor);
		if (nFloor == 0) {
			continue;
		}
#ifndef ENGLISH_MODE
		sBlock.Replace(L"동", L"");
#else
		sBlock.Replace(L"B.BLCK ", L"");
#endif
		nBlock = 1;
		for (int i = 0; i < CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount(); i++) {
			sTemp = CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(i);
			if (sBlock == sTemp) {
				nBlock = i + 1;
				break;
			}
		}
		sText.Format(L"%02d-%02d-%02d", nBlock, nStair, (nFloor < 0) ? 0 : nFloor);
		m_pListCtrl->SetItemText(nIndex, 5, sText);
	}
}
void CBroadcastDlg::BroadcastStandardType2()
{
	CString sBlock, sFloor, sStair, sText, sTemp;
	int nBlock, nFloor, nStair;
	int nCount = m_pListCtrl->GetItemCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		sStair = L"";
		sFloor = L"";
		sBlock = L"";
		m_pListCtrl->GetItemText(nIndex, 2, sBlock);
		m_pListCtrl->GetItemText(nIndex, 3, sStair);
		m_pListCtrl->GetItemText(nIndex, 4, sFloor);
		if (sStair.GetLength() == 0 || sFloor.GetLength() == 0) {
			continue;
		}
		nStair = GetStairValue(sStair);
		if (nStair == -1) {
			continue;
		}
		nFloor = GetFloorValue(sFloor);
		if (nFloor == 0) {
			continue;
		}
#ifndef ENGLISH_MODE
		sBlock.Replace(L"동", L"");
#else
		sBlock.Replace(L"B.BLCK ", L"");
#endif
		nBlock = 1;
		for (int i = 0; i < CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount(); i++) {
			sTemp = CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(i);
			if (sBlock == sTemp) {
				nBlock = i + 1;
				break;
			}
		}
		sText.Format(L"%02d%d%s%02d", nBlock, nStair, (nFloor < 0) ? L"B" : L" ", abs(nFloor));
		m_pListCtrl->SetItemText(nIndex, 5, sText);
	}
}
void CBroadcastDlg::BroadcastStandardType3()
{
	CString sFloor, sStair, sText;
	int nCount = m_pListCtrl->GetItemCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		sText.Format(L"%d", nIndex + 1);
		m_pListCtrl->SetItemText(nIndex, 5, sText);
	}
}

LRESULT CBroadcastDlg::OnListControl(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;
	int nSelectItem = (int)lParam;
	switch (nType)
	{
	case 1: // del
		if (m_pListCtrl) {
			m_pListCtrl->DelListItem(nSelectItem);
			BroadcastStandard();
			m_pListCtrl->Invalidate();
		}
		break;
	case 2: // up add
		if (m_pListCtrl) {
			m_pListCtrl->InsertListItemUp(nSelectItem);
			if (m_pListCtrl->GetItemCount() > nSelectItem) {
				m_pListCtrl->SetItemText(nSelectItem, 1, CCircuitBasicInfo::Instance()->m_sBuildingName);
			}
			BroadcastStandard();
			m_pListCtrl->Invalidate();
		}
		break;
	case 3: // down add
		if (m_pListCtrl) {
			m_pListCtrl->InsertListItemDown(nSelectItem);
			if (m_pListCtrl->GetItemCount() > nSelectItem + 1) {
				m_pListCtrl->SetItemText(nSelectItem + 1, 1, CCircuitBasicInfo::Instance()->m_sBuildingName);
			}
			BroadcastStandard();
			m_pListCtrl->Invalidate();
		}
		break;
	default:
		break;

	}
	return 0;
}

LRESULT CBroadcastDlg::OnRadioButtnClick(WPARAM wParam, LPARAM lParam)
{
	int nIndex = wParam;

	m_pRadioBtn1->SetCheck(false);
	m_pRadioBtn2->SetCheck(false);
	m_pRadioBtn3->SetCheck(false);
	switch (nIndex) {
	case 0: m_pRadioBtn1->SetCheck(true); break;
	case 1: m_pRadioBtn2->SetCheck(true); break;
	case 2: m_pRadioBtn3->SetCheck(true); break;
	default: break;
	}
	BroadcastStandard();

	return 0;
}

void CBroadcastDlg::Redisplay()
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
		rt.top += 30;

		m_pListCtrl->MoveWindow(rt);
		m_pListCtrl->ShowWindow(SW_SHOW);
		m_pListCtrl->Invalidate();
	}
}