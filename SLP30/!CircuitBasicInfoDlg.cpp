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
		_T("굴림")); // lpszFacename 

	m_bFirstWork = true;
	m_bInit = false;
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
const TCHAR* lpszHeaderBasic[] = { _T("건물명"), _T("동"), _T("동이름"), _T("계단"), _T("지상층"), _T("지하층"), NULL };
const int colWidthListBasic[] = { 200, 90, 180, 100, 100, 100, 0 }; // pixel
const int colTypeBasic[] = { POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, 0 };
const int colAlignBasic[] = { DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, -1 }; // option, row align text, default: DT_LEFT
const int colLimitBasic[] = { 0, 6, 0, 6, 6, 6, -1 }; // 0: 무제한, > 0 : 텍스트 사이즈 제한

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
	m_pListCtrl->SetItemText(0, 1, L"0");
	m_pListCtrl->SetItemText(0, 2, L"101");
	m_pListCtrl->SetItemText(0, 3, L"1");
	m_pListCtrl->SetItemText(0, 4, L"2");
	m_pListCtrl->SetItemText(0, 5, L"0");

	int nY = 203;
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		switch (nIndex) {
		case 0:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[발신기] PBL SET 있는 경우 체크하여 주십시오(단독형, 소화전형)", this); break;
		case 1:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[알람밸브] 스프링클러 설비중 알람밸브가 있는 경우 체크하여 주십시오", this); break;
		case 2:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[프리액션밸브] 스프링클러 설비중 프리액션밸브가 있는 경우 체크하여 주십시오", this); break;
		case 3:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[시각경보] 시각경보 설비가 있는 경우 체크하여 주십시오", this); break;
		case 4:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[전실제연] 전실제연 설비가 있는 경우 체크하여 주십시오", this); break;
		case 5:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[상가제연] 상가제연 설비가 있는 경우 체크하여 주십시오", this); break;
		case 6:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[방화셔터] 방화셔터 설비가 있는 경우 체크하여 주십시오", this); break;
		case 7:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[배연창] 배연창 설비가 있는 경우 체크하여 주십시오", this); break;
		case 8:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[방화문] 방화문 설비가 있는 경우 체크하여 주십시오", this); break;
		case 9:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[자폐장치] 창문형, 방화문 자폐장치가 있는 경우 체크하여 주십시오", this); break;
		case 10:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[소화전기동확인] 소화전기동 표시등이 있는 경우 체크하여 주십시오", this); break;
		case 11:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[옥상개폐확인] 옥상개폐 확인이 있는 경우 체크하여 주십시오", this); break;
		case 12:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[저수위감시] 저수위감시가 있는 경우 체크하여 주십시오", this); break;
		case 13:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[제연급기팬] 급기팬 설비가 있는 경우 체크하여 주십시오", this); break;
		case 14:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[제연배기팬] 배기팬 설비가 있는 경우 체크하여 주십시오", this); break;
		case 15:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[ADD 감지기] 주소형, 아날로그, 계단, ELEV 감지기 회로가 있는 경우 체크하여 주십시오", this); break;
		}
		
		m_pCheck[nIndex]->Create(IDD_COMMON_CHILD_DIALOG, this);
		m_pCheck[nIndex]->ShowWindow(SW_SHOW);

		m_pCheck[nIndex]->MoveWindow(50, nY, m_pCheck[nIndex]->GetWidthExtent() + 22, 25);
		nY += 25;
	}
	m_btnNext.Create(IDB_BMP_NEXT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
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
	CString sBD, sStair, sFloor, sBase, sBlock, sBlockName, sToken, sTemp;
	m_pListCtrl->GetItemText(0, 0, sBD);
	m_pListCtrl->GetItemText(0, 1, sBlock);
	m_pListCtrl->GetItemText(0, 2, sBlockName);
	m_pListCtrl->GetItemText(0, 3, sStair);
	m_pListCtrl->GetItemText(0, 4, sFloor);
	m_pListCtrl->GetItemText(0, 5, sBase);
	sBD.Trim();
	sStair.Trim();
	sFloor.Trim();
	sBase.Trim();
	if (sBD.GetLength() == 0) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n건물명을 입력하여 주십시오.", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (sBlock.GetLength() == 0 || !CCommonFunc::CheckDigit(sBlock.GetBuffer(0))) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n동 수를 입력하여 주십시오.\n\n(숫자만 입력 가능합니다.)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (sBlockName.GetLength() == 0) {// || !CCommonFunc::CheckDigit(sBlockName.GetBuffer(0))) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n동 이름을 입력하여 주십시오.\n\n(입력범위: 1 ~ 99999, 예) 101,102,103)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (sStair.GetLength() == 0 || !CCommonFunc::CheckDigit(sStair.GetBuffer(0))) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n계단수를 입력하여 주십시오.\n\n(숫자만 입력 가능합니다.)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (sFloor.GetLength() == 0 || !CCommonFunc::CheckDigit(sFloor.GetBuffer(0))) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n지상층을 입력하여 주십시오.\n\n(숫자만 입력 가능합니다.)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (sBase.GetLength() == 0 || !CCommonFunc::CheckDigit(sBase.GetBuffer(0))) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n지하층을 입력하여 주십시오.\n\n(숫자만 입력 가능합니다.)", MB_OK, this);
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
	int nBlock = _ttoi(sBlock.GetBuffer());
	if (nBlock == 1) {
		nBlock = 0;
	}
	//int nBlockName = _ttoi(sBlockName.GetBuffer());
	if (nBlock < 0 || nBlock > 3) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n동 정보를 다시 입력하여 주십시오.\n\n(입력범위: 0 ~ 3동, 0: 사용안함)", MB_OK, this);
		popup.DoModal();
		return;
	}
	/*if (nBlock > 0 && (nBlockName < 0 || nBlockName > 99999)) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n동 이름 정보를 다시 입력하여 주십시오.\n\n(입력범위: 1 ~ 99999, 예) 101,102,103)", MB_OK, this);
		popup.DoModal();
		return;
	}*/
	if (nStair < 1 || nStair > 3) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n계단 정보를 다시 입력하여 주십시오.\n\n(입력범위: 1 ~ 3계단)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (nFloor < 1 || nFloor > 25) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n지상층 정보를 다시 입력하여 주십시오.\n\n(입력범위: 1 ~ 25층)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (nBase < 0 || nBase > 5) {
		CMessagePopup popup(L"회로 기본 입력", L"\n\n\n지하층 정보를 다시 입력하여 주십시오.\n\n(입력범위: 0 ~ 5층)", MB_OK, this);
		popup.DoModal();
		return;
	}
	CStringArray arrayBlock;
	for (int nIndex = 0; nIndex < nBlock; nIndex++) {
		bool bResult = AfxExtractSubString(sToken, sBlockName, nIndex, L',');
		sToken.TrimLeft();
		sToken.TrimRight();
		if (!bResult || sToken.GetLength() == 0) {
			CMessagePopup popup(L"회로 기본 입력", L"\n\n\n동 이름 정보를 다시 입력하여 주십시오.\n\n(입력범위: 1 ~ 99999, 예) 101,102,103)", MB_OK, this);
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
			|| CCircuitBasicInfo::Instance()->m_nBlock != _ttoi(sBlock.GetBuffer()) || !bCompare) {
			CMessagePopup popup(L"회로 기본 입력", L"\n\n\n새로운 정보를 반영하시겠습니까?\n\n기존 작업은 삭제됩니다.\n\n(확인: 새작업 / 취소: 기존작업)", MB_YESNO, this);
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
		CCircuitBasicInfo::Instance()->m_sBuildingName = sBD;
		CCircuitBasicInfo::Instance()->m_nStair = nStair;
		CCircuitBasicInfo::Instance()->m_nFloor = nFloor;
		CCircuitBasicInfo::Instance()->m_nBasement = nBase;
		CCircuitBasicInfo::Instance()->m_nBlock = nBlock;
		CCircuitBasicInfo::Instance()->m_sBlockName = sBlockName;

		CCircuitBasicInfo::Instance()->m_arrayBlockName.RemoveAll();
		CCircuitBasicInfo::Instance()->m_arrayBlockName.Copy(arrayBlock);

		for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
			CCircuitBasicInfo::Instance()->m_bCheck[nIndex] = m_pCheck[nIndex]->GetCheck();
		}
		m_bFirstWork = false;
		bChange = true;
	}

	GetParent()->PostMessage(SELECTION_PROJECT, 11, bChange);
}

void CCircuitBasicInfoDlg::InfoInit()
{
	m_pListCtrl->SetItemText(0, 0, L"");
	m_pListCtrl->SetItemText(0, 1, L"0");
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

	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex]->SetCheck(CCircuitBasicInfo::Instance()->m_bCheck[nIndex]);
	}
	m_bFirstWork = false;

	GetParent()->PostMessage(SELECTION_PROJECT, 11, false);
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
	rt.SetRect(20, 170, 850, 610);
	CCommonDisplay::DrawRect(&memDC, false, RGB(150, 150, 150), 0, rt);
	rt.SetRect(20, 15, 350, 50);
	CCommonDisplay::DrawCaption(&memDC, L"회로 정보 입력", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 48, 350, 80);
	CCommonDisplay::DrawCaption(&memDC, L"기본 정보 입력", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 170, 350, 210);
	CCommonDisplay::DrawCaption(&memDC, L"회로 정보 선택", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInit) {
		m_btnNext.MoveWindow(rect.right - 87, 0, 83, 48);
	}
}