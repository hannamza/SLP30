// CircuitBasicInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CircuitBasicInfoDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CCircuitBasicInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCircuitBasicInfoDlg, CDialogEx)

CCircuitBasicInfoDlg::CCircuitBasicInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex] = NULL;
	}
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

	m_bFirstWork = true;
	m_bInit = false;

	m_bBuildingInfoComplate = false;
}

CCircuitBasicInfoDlg::~CCircuitBasicInfoDlg()
{
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		SAFE_DELETE(m_pCheck[nIndex]);
	}
}

void CCircuitBasicInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircuitBasicInfoDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnNextClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCircuitBasicInfoDlg 메시지 처리기입니다.
#ifndef ENGLISH_MODE
	const TCHAR* lpszHeaderBasic[] = { _T("건물명"), _T("동"), _T("동이름"), _T("계단"), _T("지상층"), _T("지하층"), _T("옥탑층"), NULL };
#else
	const TCHAR* lpszHeaderBasic[] = { _T("B.NAME"), _T("B.BLCK"), _T("BLCK.NAME"), _T("LINE"), _T("G.FLOR"), _T("B.FLOR"), _T("R.FLOR"), NULL };
#endif
const int colWidthListBasic[] = { 200, 70, 180, 80, 80, 80, 80, 0 }; // pixel
const int colTypeBasic[] = { POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, 0 };
const int colAlignBasic[] = { DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER , -1 }; // option, row align text, default: DT_LEFT
const int colLimitBasic[] = { 0, 6, 0, 6, 6, 6, 6, -1 }; // 0: 무제한, > 0 : 텍스트 사이즈 제한

BOOL CCircuitBasicInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CRect rect;
	GetClientRect(rect);

	CRuntimeClass	*pRuntimeClass;
	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomListCtrl))) {
		if (NULL != (m_pListCtrl = (CCustomListCtrl *)(pRuntimeClass->CreateObject()))) {
			m_pListCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}
	m_pListCtrl->SetListIndex(0);
	m_pListCtrl->SetHeaderInfo((TCHAR**)lpszHeaderBasic, (int*)colWidthListBasic, (int*)colTypeBasic, (int*)colAlignBasic); // Align: option, row align text, default: DT_LEFT
	m_pListCtrl->SetColumnLimit((int*)colLimitBasic);
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
	m_pListCtrl->FixHeaderSize(true);
	m_pListCtrl->SetDisableMenu(true);

	m_pListCtrl->SendMessage(WM_INITIALUPDATE);

	m_pListCtrl->AddItem();
	m_pListCtrl->SetItemText(0, 0, L"");
	m_pListCtrl->SetItemText(0, 1, L"1");
	m_pListCtrl->SetItemText(0, 2, L"101");
	m_pListCtrl->SetItemText(0, 3, L"1");
	m_pListCtrl->SetItemText(0, 4, L"2");
	m_pListCtrl->SetItemText(0, 5, L"0");
	m_pListCtrl->SetItemText(0, 6, L"0");

	int nY = 188;
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		switch (nIndex) {
#ifndef ENGLISH_MODE
		case 0:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[소화전] PBL SET", this); break;
		case 1:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[발신기] PBL SET", this); break;
		case 2:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[알람밸브] 스프링클러 알람밸브", this); break;
		case 3:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[프리액션밸브] 스프링클러 프리액션밸브", this); break;
		case 4:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[시각경보] 시각경보 설비", this); break;
		case 5:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[전실 제연] 전실 제연 급기설비", this); break;
		case 6:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[전실 제연] 전실 제연 배기설비", this); break;
		case 7:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[상가 제연] 상가 제연 설비", this); break;
		case 8:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[방화 셔터] 방화 셔터 설비", this); break;
		case 9:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[배연창] 배연창 설비", this); break;
		case 10:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[방화문] 방화문 설비", this); break;
		case 11:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[창폐 장치] 창문 폐쇄장치", this); break;
		case 12:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[자폐 장치] 자동 폐쇄장치", this); break;
		case 13:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[소화전 기동 확인] 소화전 기동 표시등", this); break;
		case 14:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[옥상 개폐 확인] 옥상 개폐 확인", this); break;
		case 15:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[저수위 감시] 저수위 감시", this); break;
		case 16:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[제연 급기팬] 급기팬 설비", this); break;
		case 17:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[제연 배기팬] 배기팬 설비", this); break;
		case 18:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[감지기] 아날로그형, 주소형, 일반형", this); break;
		case 19:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[T/S 설비] T/S 설비", this); break;
		case 20:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[피난사다리] 피난사다리", this); break;
#else
		case 0:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[HYDRANT] PBL SET", this); break;
		case 1:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[MANUAL STATION] PBL SET", this); break;
		case 2:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[ALARM VALVE] ALARM VALVE in Sprinkler Facility", this); break;
		case 3:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[PRE-ACTION VALVE] PRE-ACTION VALVE in Sprinkler Facility", this); break;
		case 4:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[STROBE] STROBE", this); break;
		case 5:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[SMOKE CONTROL(VESTIBULE)] AIR SUPPLY EQUIPMENT", this); break;
		case 6:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[SMOKE CONTROL(VESTIBULE)] SMOKE EXHAUST EQUIPMENT", this); break;
		case 7:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[SMOKE CONTROL(HALL)] SMOKE CONTROL", this); break;
		case 8:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[FIRE SHUTTER] SHUTTER", this); break;
		case 9:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[SMOKE WINDOW] WINDOW", this); break;
		case 10:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[FIRE DOOR] DOOR", this); break;
		case 11:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[WINDOW CLOSER] WINDOW CLOSER", this); break;
		case 12:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[AUTOMATIC DOOR CLOSER] AUTO DOOR CLOSER", this); break;
		case 13:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[HYDRANT] PILOT LAMP", this); break;
		case 14:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[ROOF DOOR ACTION] PILOT LAMP", this); break;
		case 15:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[LOW WATER LEVEL] FLOAT SWITCH", this); break;
		case 16:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[SUPPLY FAN] AIR SUPPLY FAN", this); break;
		case 17:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[EXHAUST FAN] SMOKE EXHAUST FAN", this); break;
		case 18:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[DETECTOR] DETECTOR for ANALOG, ADDRESSABLE, CONVENTIONAL", this); break;
		case 19:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[TAMPER SWITCH] TAMPER SWITCH T/S", this); break;
		case 20:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[ESCAPE LADDER] ESCAPE LADDER", this); break;
#endif
		}
		
		m_pCheck[nIndex]->Create(IDD_COMMON_CHILD_DIALOG, this);
		m_pCheck[nIndex]->ShowWindow(SW_SHOW);

		m_pCheck[nIndex]->MoveWindow(50, nY, m_pCheck[nIndex]->GetWidthExtent() + 18, 22);
		nY += 22;
	}

#ifndef ENGLISH_MODE
	m_btnNext.Create(IDB_BMP_NEXT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
#else
	m_btnNext.Create(IDB_BMP_NEXT_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
#endif
	m_btnNext.MoveWindow(875, 0, 83, 48);

	rect.top += 75;
	rect.left += 50;
	rect.right = 820;
	rect.bottom = rect.top + 75;

	m_pListCtrl->MoveWindow(rect);

	m_bInit = true;;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCircuitBasicInfoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CCircuitBasicInfoDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CCircuitBasicInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCircuitBasicInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircuitBasicInfoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircuitBasicInfoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCircuitBasicInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex]->Invalidate();
	}
	m_btnNext.Invalidate();
	m_btnNext.Invalidate();

	m_pListCtrl->Invalidate();
}

void CCircuitBasicInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CCircuitBasicInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CCircuitBasicInfoDlg::OnNextClick()
{
	CString sBD, sStair, sFloor, sBase, sRoofTop, sBlock, sBlockName, sToken, sTemp;
	m_pListCtrl->GetItemText(0, 0, sBD);
	m_pListCtrl->GetItemText(0, 1, sBlock);
	m_pListCtrl->GetItemText(0, 2, sBlockName);
	m_pListCtrl->GetItemText(0, 3, sStair);
	m_pListCtrl->GetItemText(0, 4, sFloor);
	m_pListCtrl->GetItemText(0, 5, sBase);
	m_pListCtrl->GetItemText(0, 6, sRoofTop);

	sBD.Trim();
	sStair.Trim();
	sFloor.Trim();
	sBase.Trim();
	if (sBD.GetLength() == 0) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n건물명을 입력하세요.", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Building Name.", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sBlock.GetLength() == 0 || !CCommonFunc::CheckDigit(sBlock.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n동 개수를 입력하세요.\n\n(숫자만 입력 가능)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Building Block.\n\n(Only Number)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sBlockName.GetLength() == 0) {// || !CCommonFunc::CheckDigit(sBlockName.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n동 이름을 입력하세요.\n\n(예: 101,102,103,가,나,다)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Building Block Name\n\n(Ex: 101,102,103,A,B,C)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sStair.GetLength() == 0 || !CCommonFunc::CheckDigit(sStair.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n계단수를 입력하세요.\n\n(숫자만 입력 가능)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Line in Building.\n\n(Only Number)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sFloor.GetLength() == 0 || !CCommonFunc::CheckDigit(sFloor.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n지상층수를 입력하세요.\n\n(숫자만 입력 가능)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Ground Floor.\n\n(Only Number)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sBase.GetLength() == 0 || !CCommonFunc::CheckDigit(sBase.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n지하층수를 입력하세요.\n\n(숫자만 입력 가능)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Basement Floor.\n\n(Only Number)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sRoofTop.GetLength() == 0 || !CCommonFunc::CheckDigit(sRoofTop.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n옥탑층수를 입력하세요.\n\n(숫자만 입력 가능)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Rooftop Floor\n\n(Only Number)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}

	bool bChange = false;
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		if (CCircuitBasicInfo::Instance()->m_bCheck[nIndex] != m_pCheck[nIndex]->GetCheck()) {
			bChange = true;
		}
	}
	int nStair = _ttoi(sStair.GetBuffer());
	int nFloor = _ttoi(sFloor.GetBuffer());
	int nBase = _ttoi(sBase.GetBuffer());
	INT nRooftop = _ttoi(sRoofTop.GetBuffer());
	int nBlock = _ttoi(sBlock.GetBuffer());
	//int nBlockName = _ttoi(sBlockName.GetBuffer());
	if (nBlock < 0 || nBlock > 3) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n동수를 다시 입력하세요.\n\n(입력범위: 1 ~ 3)", MB_OK, this);
		//CMessagePopup popup(L"회로 기본 입력", L"\n\n\n동수를 다시 입력하세요.\n\n(입력범위: 0 ~ 3, 0: 사용안함)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Building Block, again.\n\n(Range: 1 ~ 3)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	/*if (nBlock > 0 && (nBlockName < 0 || nBlockName > 99999)) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n동 이름 정보를 다시 입력하여 주십시오.\n\n(입력범위: 1 ~ 99999, 예) 101,102,103)", MB_OK, this);
		popup.DoModal();
		return;
	}*/
	if (nStair < 1 || nStair > 3) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n계단수를 다시 입력하세요.\n\n(입력범위: 1 ~ 3)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Line in Building, again.\n\n(Range: 1 ~ 3)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (nFloor < 1 || nFloor > 25) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n지상층수를 다시 입력하세요.\n\n(입력범위: 1 ~ 25)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Ground Floor, again.\n\n(Range: 1 ~ 25)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (nBase < 0 || nBase > 5) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n지하층수를 다시 입력하세요.\n\n(입력범위: 0 ~ 5)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Basement Floor.\n\n(Range: 0 ~ 5)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (nRooftop < 0 || nRooftop > 1) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"건물 정보 입력", L"\n\n\n옥탑층수를 다시 입력하세요.\n\n(입력범위: 0 ~ 1)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Rooftop Floor.\n\n(Range: 0 ~ 1)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	CStringArray arrayBlock;
	for (int nIndex = 0; nIndex < nBlock; nIndex++) {
		bool bResult = AfxExtractSubString(sToken, sBlockName, nIndex, L',');
		sToken.TrimLeft();
		sToken.TrimRight();
		if (!bResult || sToken.GetLength() == 0) {
#ifndef ENGLISH_MODE
			CMessagePopup popup(L"건물 정보 입력", L"\n\n\n동 이름을 다시 입력하세요.\n\n(예: 101,102,103,가,나,다)", MB_OK, this);
#else
			CMessagePopup popup(L"Building Information", L"\n\n\nEnter Building Block Name, again.\n\n(Ex: 101,102,103,A,B,C)", MB_OK, this);
#endif
			popup.DoModal();
			return;
		}
		arrayBlock.Add(sToken);
	}
	bool bCompare = true;
	if (arrayBlock.GetCount() == CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount()) {
		for (int nIndex = 0; nIndex < arrayBlock.GetCount(); nIndex++) {
			if (arrayBlock.GetAt(nIndex) != CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nIndex)) {
				bCompare = false;
				break;
			}
		}
	}
	else {
		bCompare = false;
	}

	if (!m_bFirstWork) {
		if (bChange || CCircuitBasicInfo::Instance()->m_sBuildingName != sBD || CCircuitBasicInfo::Instance()->m_nStair != _ttoi(sStair.GetBuffer())
			|| CCircuitBasicInfo::Instance()->m_nFloor != _ttoi(sFloor.GetBuffer()) || CCircuitBasicInfo::Instance()->m_nBasement != _ttoi(sBase.GetBuffer())
			|| CCircuitBasicInfo::Instance()->m_nRooftop != _ttoi(sRoofTop.GetBuffer())
			|| CCircuitBasicInfo::Instance()->m_nBlock != _ttoi(sBlock.GetBuffer()) || !bCompare) {
#ifndef ENGLISH_MODE
			CMessagePopup popup(L"설비 정보 선택", L"\n\n\n새로운 [설비 정보 선택]을 적용하시겠습니까?", MB_YESNO, this);
#else
			CMessagePopup popup(L"Equipment info.\nSelection", L"\n\n\n\nDo you apply the new [Equipment Info. Selection]?", MB_YESNO, this);
#endif
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				bChange = true;
			}
			else {
				bChange = false;
			}
		}
	}

	if (m_bFirstWork || bChange) {
		
		//20221013 GBM start - 건물 정보는 한번 입력하면 고정
#if 1
		if (!m_bBuildingInfoComplate)
		{
#ifndef ENGLISH_MODE
			CMessagePopup popup(L"건물 정보 확정", L"\n\n\n건물 정보 확정 후 변경 불가능함\n\n적용하시겠습니까?", MB_YESNO, this);
#else
			CMessagePopup popup(L"Confirmation of\nBuilding Information", L"\n\n\n\nBuilding Info. can NOT be changed\nafter confirmation.\n\nDo you apply it?", MB_YESNO, this);
#endif
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				CCircuitBasicInfo::Instance()->m_sBuildingName = sBD;
				CCircuitBasicInfo::Instance()->m_nStair = nStair;
				CCircuitBasicInfo::Instance()->m_nFloor = nFloor;
				CCircuitBasicInfo::Instance()->m_nBasement = nBase;
				CCircuitBasicInfo::Instance()->m_nRooftop = nRooftop;
				CCircuitBasicInfo::Instance()->m_nBlock = nBlock;
				CCircuitBasicInfo::Instance()->m_sBlockName = sBlockName;

				CCircuitBasicInfo::Instance()->m_arrayBlockName.RemoveAll();
				CCircuitBasicInfo::Instance()->m_arrayBlockName.Copy(arrayBlock);

				m_bBuildingInfoComplate = true;

				m_pListCtrl->SetReadOnly(true);

				//건물 정보가 확정이 되면 회로 설정 비교 정보를 초기화 해둔다.
				CCommonState::ie()->InitSelectCircuitCompInfo(0);
				CCommonState::ie()->InitSelectCircuitCompInfo(1);

				CCommonState::ie()->InitSelectCircuitRepeaterList(0);
				CCommonState::ie()->InitSelectCircuitRepeaterList(1);

			}
			else {
				return;
			}
		}

#else
		CCircuitBasicInfo::Instance()->m_sBuildingName = sBD;
		CCircuitBasicInfo::Instance()->m_nStair = nStair;
		CCircuitBasicInfo::Instance()->m_nFloor = nFloor;
		CCircuitBasicInfo::Instance()->m_nBasement = nBase;
		CCircuitBasicInfo::Instance()->m_nBlock = nBlock;
		CCircuitBasicInfo::Instance()->m_sBlockName = sBlockName;

		CCircuitBasicInfo::Instance()->m_arrayBlockName.RemoveAll();
		CCircuitBasicInfo::Instance()->m_arrayBlockName.Copy(arrayBlock);
#endif
		//20221013 GBM end

		for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
			CCircuitBasicInfo::Instance()->m_bCheck[nIndex] = m_pCheck[nIndex]->GetCheck();
		}
		m_bFirstWork = false;
		bChange = true;
	}
	else
	{
		//20221014 GBM start - 편집 내용 미적용 시 체크박스 컨트롤 내용을 기존 내용으로 회귀
		for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
			m_pCheck[nIndex]->SetCheck(CCircuitBasicInfo::Instance()->m_bCheck[nIndex]);
		}
		//20221014 GBM end
	}

	GetParent()->PostMessage(SELECTION_PROJECT, 11, bChange);
}

void CCircuitBasicInfoDlg::InfoInit()
{
	m_pListCtrl->SetItemText(0, 0, L"");
	//20221012 GBM start - 건물 기본 값을 1로 변경해서 기존 0일 경우 프로젝트 내용 로드 시 설비 개수 초기화 되는 오류 수정
#if 1
	m_pListCtrl->SetItemText(0, 1, L"1");
#else
	m_pListCtrl->SetItemText(0, 1, L"0");
#endif
	//20221012 GBM end
	m_pListCtrl->SetItemText(0, 2, L"101");
	m_pListCtrl->SetItemText(0, 3, L"1");
	m_pListCtrl->SetItemText(0, 4, L"2");
	m_pListCtrl->SetItemText(0, 5, L"0");

	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex]->SetCheck(false);
	}

	m_bFirstWork = true;
}

void CCircuitBasicInfoDlg::LoadInfo()
{
	CString sTemp;
	m_pListCtrl->SetItemText(0, 0, CCircuitBasicInfo::Instance()->m_sBuildingName);
	sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nBlock);
	m_pListCtrl->SetItemText(0, 1, sTemp);
	//sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nBlockName);
	m_pListCtrl->SetItemText(0, 2, CCircuitBasicInfo::Instance()->m_sBlockName);
	sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nStair);
	m_pListCtrl->SetItemText(0, 3, sTemp);
	sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nFloor);
	m_pListCtrl->SetItemText(0, 4, sTemp);
	sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nBasement);
	m_pListCtrl->SetItemText(0, 5, sTemp);
	sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nRooftop);
	m_pListCtrl->SetItemText(0, 6, sTemp);

	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex]->SetCheck(CCircuitBasicInfo::Instance()->m_bCheck[nIndex]);
	}
	m_bFirstWork = false;

	//20221013 GBM start - 프로젝트 파일이 로드됐다는 건 건물 정보가 확정되었다는 의미
	m_bBuildingInfoComplate = true;
	m_pListCtrl->SetReadOnly(true);

	//건물 정보가 확정이 되면 회로 설정 비교 정보를 초기화 해둔다.
	CCommonState::ie()->InitSelectCircuitCompInfo(0);
	CCommonState::ie()->InitSelectCircuitCompInfo(1);

	CCommonState::ie()->InitSelectCircuitRepeaterList(0);
	CCommonState::ie()->InitSelectCircuitRepeaterList(1);

	//20221025 GBM start - 프로젝트 로드 첫화면이 설비 개수 설정화면이 아니라 기본 설비 설정화면이 나오도록 수정
	//GetParent()->PostMessage(SELECTION_PROJECT, 11, false);
	GetParent()->PostMessage(SELECTION_PROJECT, 0, 0);
	//20221025 GBM end
}

void CCircuitBasicInfoDlg::GetCircuitInfo(CString & sName, int & nStair, int & nFloor, int & nBasement)
{
	m_pListCtrl->GetItemText(0, 0, sName);
	CString sTemp;
	m_pListCtrl->GetItemText(0, 1, sTemp);
	nStair = _ttoi(sTemp.GetBuffer(0));
	m_pListCtrl->GetItemText(0, 2, sTemp);
	nFloor = _ttoi(sTemp.GetBuffer(0));
	m_pListCtrl->GetItemText(0, 3, sTemp);
	nBasement = _ttoi(sTemp.GetBuffer(0));
}

void CCircuitBasicInfoDlg::Redisplay()
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
	rt.SetRect(20, 40, 850, 155);
	CCommonDisplay::DrawRect(&memDC, false, RGB(150, 150, 150), 0, rt);
// 	rt.SetRect(20, 160, 965, 610);
// 	CCommonDisplay::DrawRect(&memDC, false, RGB(150, 150, 150), 0, rt);
	rt.SetRect(20, 160, 965, 660);
	CCommonDisplay::DrawRect(&memDC, false, RGB(150, 150, 150), 0, rt);
	rt.SetRect(20, 15, 350, 50);
#ifndef ENGLISH_MODE
	//CCommonDisplay::DrawCaption(&memDC, L"회로 정보 입력", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 48, 350, 80);
	CCommonDisplay::DrawCaption(&memDC, L"건물 정보 입력", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 160, 350, 190);
	CCommonDisplay::DrawCaption(&memDC, L"설비 정보 선택", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
#else
	//CCommonDisplay::DrawCaption(&memDC, L"Circuit Info.", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 48, 350, 80);
	CCommonDisplay::DrawCaption(&memDC, L"Building Info.", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 160, 350, 190);
	CCommonDisplay::DrawCaption(&memDC, L"Equipment Info. Selection", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
#endif

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInit) {
		m_btnNext.MoveWindow(rect.right - 87, 0, 83, 48);
	}
}

bool CCircuitBasicInfoDlg::IsBuildingInfoComplete()
{
	bool bRet = false;
	bRet = m_bBuildingInfoComplate;
	return bRet;
}

void CCircuitBasicInfoDlg::LoadCheckInfoOnly()
{
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex]->SetCheck(CCircuitBasicInfo::Instance()->m_bCheck[nIndex]);
	}
}