// PSPumpSetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "PSPumpSetupDlg.h"
#include "afxdialogex.h"


// CPSPumpSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPSPumpSetupDlg, CDialogEx)

CPSPumpSetupDlg::CPSPumpSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_pPSCtrl = NULL;
	m_pPumpCtrl = NULL;

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
}

CPSPumpSetupDlg::~CPSPumpSetupDlg()
{
}

void CPSPumpSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPSPumpSetupDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CPSPumpSetupDlg 메시지 처리기입니다.
#ifndef ENGLISH_MODE
const TCHAR* g_editHeaderPS[] = { _T("번호"), _T("압력 스위치"), _T("사용여부"), NULL };
#else
const TCHAR* g_editHeaderPS[] = { _T("No."), _T("P/S"), _T("USE"), NULL };
#endif
const int g_editSizePS[] = { 100, 250, 200, 0 }; // pixel
const int g_editTypePS[] = { POPUP_TYPE_SEQ, POPUP_TYPE_NONE, POPUP_TYPE_SPACE, 0 };
const int g_editAlignPS[] = { DT_CENTER, DT_CENTER, DT_CENTER, -1 }; // option, row align text, default: DT_LEFT
const int g_editLimitPS[] = { 0, 0, 0, -1 }; // 0: 무제한, > 0 : 텍스트 사이즈 제한

#ifndef ENGLISH_MODE
const TCHAR* g_editHeaderPump[] = { _T("번호"), _T("펌프"), _T("사용여부"), _T("기동램프 출력"), NULL };
#else
const TCHAR* g_editHeaderPump[] = { _T("No."), _T("PUMP"), _T("USE"), _T("P/L ON"), NULL };
#endif
const int g_editSizePump[] = { 100, 175, 125, 150, 0 }; // pixel
const int g_editTypePump[] = { POPUP_TYPE_SEQ, POPUP_TYPE_NONE, POPUP_TYPE_SPACE, POPUP_TYPE_SPACE, 0 };
const int g_editAlignPump[] = { DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, -1 }; // option, row align text, default: DT_LEFT
const int g_editLimitPump[] = { 0, 0, 0, 0, -1 }; // 0: 무제한, > 0 : 텍스트 사이즈 제한

BOOL CPSPumpSetupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRuntimeClass	*pRuntimeClass;
	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomListCtrl))) {
		if (NULL != (m_pPSCtrl = (CCustomListCtrl *)(pRuntimeClass->CreateObject()))) {
			m_pPSCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}
	m_pPSCtrl->SetListIndex(0);
	m_pPSCtrl->SetHeaderInfo((TCHAR**)g_editHeaderPS, (int*)g_editSizePS, (int*)g_editTypePS, (int*)g_editAlignPS); // Align: option, row align text, default: DT_LEFT
	m_pPSCtrl->SetColumnLimit((int*)g_editLimitPS);
	// 높이 변경 (option)
	m_pPSCtrl->SetHeaderHeight(40);
	m_pPSCtrl->SetRowHeight(30);
	//m_pPSCtrl->SetListPopupSize(500, 500);
	// 색변경 (option)
	m_pPSCtrl->SetHeaderBkColor(RGB(80, 80, 80), RGB(255, 255, 255));
	m_pPSCtrl->SetHeaderTextColor(RGB(220, 220, 220));
	m_pPSCtrl->SetRowBkColor(RGB(160, 160, 160), RGB(255, 255, 0), RGB(255, 255, 255));
	m_pPSCtrl->SetRowTextColor(RGB(255, 255, 255), RGB(80, 80, 80));
	m_pPSCtrl->SetHeaderFontSize(15);
	m_pPSCtrl->FixHeaderSize(true);
	m_pPSCtrl->SetDisableMenu(true);

	/*m_pPSCtrl->AddListItemState(1, L"감시 지속");
	m_pPSCtrl->AddListItemState(1, L"감시 비지속");*/

#ifndef ENGLISH_MODE
	m_pPSCtrl->AddListItemState(2, L"사용");
	m_pPSCtrl->AddListItemState(2, L"사용 안함");
#else
	m_pPSCtrl->AddListItemState(2, L"USED");
	m_pPSCtrl->AddListItemState(2, L"NOT USED");
#endif

	m_pPSCtrl->SendMessage(WM_INITIALUPDATE);

	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomListCtrl))) {
		if (NULL != (m_pPumpCtrl = (CCustomListCtrl *)(pRuntimeClass->CreateObject()))) {
			m_pPumpCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}
	m_pPumpCtrl->SetListIndex(0);
	m_pPumpCtrl->SetHeaderInfo((TCHAR**)g_editHeaderPump, (int*)g_editSizePump, (int*)g_editTypePump, (int*)g_editAlignPump); // Align: option, row align text, default: DT_LEFT
	m_pPumpCtrl->SetColumnLimit((int*)g_editLimitPump);
	// 높이 변경 (option)
	m_pPumpCtrl->SetHeaderHeight(40);
	m_pPumpCtrl->SetRowHeight(30);
	//m_pPumpCtrl->SetListPopupSize(500, 500);
	// 색변경 (option)
	m_pPumpCtrl->SetHeaderBkColor(RGB(80, 80, 80), RGB(255, 255, 255));
	m_pPumpCtrl->SetHeaderTextColor(RGB(220, 220, 220));
	m_pPumpCtrl->SetRowBkColor(RGB(160, 160, 160), RGB(255, 255, 0), RGB(255, 255, 255));
	m_pPumpCtrl->SetRowTextColor(RGB(255, 255, 255), RGB(80, 80, 80));
	m_pPumpCtrl->SetHeaderFontSize(15);
	m_pPumpCtrl->FixHeaderSize(true);
	m_pPumpCtrl->SetDisableMenu(true);

	/*m_pPumpCtrl->AddListItemState(1, L"주펌프 입력");
	m_pPumpCtrl->AddListItemState(1, L"예비펌프 입력");
	m_pPumpCtrl->AddListItemState(1, L"보조펌프 입력");*/

#ifndef ENGLISH_MODE
	m_pPumpCtrl->AddListItemState(2, L"사용");
	m_pPumpCtrl->AddListItemState(2, L"사용 안함");
	m_pPumpCtrl->AddListItemState(3, L"사용");
	m_pPumpCtrl->AddListItemState(3, L"사용 안함");
#else
	m_pPumpCtrl->AddListItemState(2, L"USED");
	m_pPumpCtrl->AddListItemState(2, L"NOT USED");
	m_pPumpCtrl->AddListItemState(3, L"USED");
	m_pPumpCtrl->AddListItemState(3, L"NOT USED");
#endif

	m_pPumpCtrl->SendMessage(WM_INITIALUPDATE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPSPumpSetupDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CPSPumpSetupDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CPSPumpSetupDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CPSPumpSetupDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CPSPumpSetupDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CPSPumpSetupDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CPSPumpSetupDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CPSPumpSetupDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CPSPumpSetupDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

bool CPSPumpSetupDlg::SaveInformation()
{
	if (!m_pPSCtrl || !m_pPumpCtrl) {
		return false;
	}

	CString sTemp;
	for (int nIndex = 0; nIndex < 3; nIndex++) {
		m_pPSCtrl->GetItemText(nIndex, 2, sTemp);
#ifndef ENGLISH_MODE
		if (sTemp.Compare(L"사용") == 0) {
#else
		if (sTemp.Compare(L"USED") == 0) {
#endif
			CSaveManager::ie()->m_PSInfo.use[nIndex] = 1;
		}
		else {
			CSaveManager::ie()->m_PSInfo.use[nIndex] = 0;
		}
	}
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_pPumpCtrl->GetItemText(nIndex, 2, sTemp);
#ifndef ENGLISH_MODE
		if (sTemp.Compare(L"사용") == 0) {
#else
		if (sTemp.Compare(L"USED") == 0) {
#endif
			CSaveManager::ie()->m_pumpInfo.use[nIndex] = 1;
		}
		else {
			CSaveManager::ie()->m_pumpInfo.use[nIndex] = 0;
		}

		m_pPumpCtrl->GetItemText(nIndex, 3, sTemp);
#ifndef ENGLISH_MODE
		if (sTemp.Compare(L"사용") == 0) {
#else
		if (sTemp.Compare(L"USED") == 0) {
#endif
			CSaveManager::ie()->m_pumpInfo.lamp[nIndex] = 1;
		}
		else {
			CSaveManager::ie()->m_pumpInfo.lamp[nIndex] = 0;
		}
	}
	return true;
}

void CPSPumpSetupDlg::DisplayLoadFile()
{
	if (!m_pPSCtrl || !m_pPumpCtrl) {
		return;
	}
	DisplayListItem();

	for (int nIndex = 0; nIndex < 3; nIndex++) {
#ifndef ENGLISH_MODE
		if (CSaveManager::ie()->m_PSInfo.use[nIndex] == 0) {
			m_pPSCtrl->SetItemText(nIndex, 2, L"사용 안함");
		}
		else {
			m_pPSCtrl->SetItemText(nIndex, 2, L"사용");
		}
#else
		if (CSaveManager::ie()->m_PSInfo.use[nIndex] == 0) {
			m_pPSCtrl->SetItemText(nIndex, 2, L"NOT USED");
		}
		else {
			m_pPSCtrl->SetItemText(nIndex, 2, L"USED");
		}
#endif
	}
	for (int nIndex = 0; nIndex < 4; nIndex++) {
#ifndef ENGLISH_MODE
		if (CSaveManager::ie()->m_pumpInfo.use[nIndex] == 0) {
			m_pPumpCtrl->SetItemText(nIndex, 2, L"사용 안함");
		}
		else {
			m_pPumpCtrl->SetItemText(nIndex, 2, L"사용");
		}
		if (CSaveManager::ie()->m_pumpInfo.lamp[nIndex] == 0) {
			m_pPumpCtrl->SetItemText(nIndex, 3, L"사용 안함");
		}
		else {
			m_pPumpCtrl->SetItemText(nIndex, 3, L"사용");
		}
#else
		if (CSaveManager::ie()->m_pumpInfo.use[nIndex] == 0) {
			m_pPumpCtrl->SetItemText(nIndex, 2, L"NOT USED");
		}
		else {
			m_pPumpCtrl->SetItemText(nIndex, 2, L"USED");
		}
		if (CSaveManager::ie()->m_pumpInfo.lamp[nIndex] == 0) {
			m_pPumpCtrl->SetItemText(nIndex, 3, L"NOT USED");
		}
		else {
			m_pPumpCtrl->SetItemText(nIndex, 3, L"USED");
		}
#endif
	}
}

void CPSPumpSetupDlg::DisplayListItem()
{
	if (!m_pPSCtrl || !m_pPumpCtrl) {
		return;
	}
	m_pPSCtrl->DeleteAllItems();

	m_pPSCtrl->AddItem(3);
#ifndef ENGLISH_MODE
	m_pPSCtrl->SetItemText(0, 1, L"P/S 확인");
	m_pPSCtrl->SetItemText(1, 1, L"P/S 확인");
	m_pPSCtrl->SetItemText(2, 1, L"P/S 확인");
#else
	m_pPSCtrl->SetItemText(0, 1, L"P/S ACT");
	m_pPSCtrl->SetItemText(1, 1, L"P/S ACT");
	m_pPSCtrl->SetItemText(2, 1, L"P/S ACT");
#endif
	//m_pPSCtrl->AddItem();
	//m_pPSCtrl->AddItem();

	m_pPumpCtrl->DeleteAllItems();

	m_pPumpCtrl->AddItem(4);
#ifndef ENGLISH_MODE
	m_pPumpCtrl->SetItemText(0, 1, L"주펌프 입력");
	m_pPumpCtrl->SetItemText(1, 1, L"예비펌프 입력");
	m_pPumpCtrl->SetItemText(2, 1, L"보조펌프 입력");
	m_pPumpCtrl->SetItemText(3, 1, L"비상발전기 전원");
#else
	m_pPumpCtrl->SetItemText(0, 1, L"MAIN PUMP INPUT");
	m_pPumpCtrl->SetItemText(1, 1, L"SPARE PUMP INPUT");
	m_pPumpCtrl->SetItemText(2, 1, L"AUX PUMP INPUT");
	m_pPumpCtrl->SetItemText(3, 1, L"GENERATOR POWER");
#endif
	//m_pPumpCtrl->AddItem();
	//m_pPumpCtrl->AddItem();
}

void CPSPumpSetupDlg::Redisplay()
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

	rt.SetRect(20, 40, 470, 80);
#ifndef ENGLISH_MODE
	CCommonDisplay::DrawCaption(&memDC, L"압력 스위치", RGB(80, 80, 80), m_font, false, 0, rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
#else
	CCommonDisplay::DrawCaption(&memDC, L"P/S", RGB(80, 80, 80), m_font, false, 0, rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
#endif
	rt.SetRect(20, 260, 470, 300);
	CCommonDisplay::DrawCaption(&memDC, L"PUMP", RGB(80, 80, 80), m_font, false, 0, rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	rt.SetRect(20, 475, 600, 500);
#ifndef ENGLISH_MODE
	CCommonDisplay::DrawCaption(&memDC, L"[Space Bar] 키를 눌러 사용여부를 변경하세요", RGB(255, 111, 0), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
#else
	CCommonDisplay::DrawCaption(&memDC, L"Press [Space Bar] key to change the status of use.", RGB(255, 111, 0), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
#endif

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_pPumpCtrl && m_pPSCtrl) {
		m_pPSCtrl->MoveWindow(20, 80, 550, 165);
		m_pPSCtrl->ShowWindow(SW_SHOW);
		m_pPSCtrl->Invalidate();

		m_pPumpCtrl->MoveWindow(20, 300, 550, 165);
		m_pPumpCtrl->ShowWindow(SW_SHOW);
		m_pPumpCtrl->Invalidate();
	}
}