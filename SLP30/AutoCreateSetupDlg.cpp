// AutoCreateSetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "AutoCreateSetupDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CAutoCreateSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoCreateSetupDlg, CDialogEx)

CAutoCreateSetupDlg::CAutoCreateSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_pListCtrl = NULL;
	m_pRadioBtn1 = NULL;
	m_pRadioBtn2 = NULL;
	m_pRadioBtn3 = NULL;
	m_pRadioBtn4 = NULL;
}

CAutoCreateSetupDlg::~CAutoCreateSetupDlg()
{
	SAFE_DELETE(m_pRadioBtn1);
	SAFE_DELETE(m_pRadioBtn2);
	SAFE_DELETE(m_pRadioBtn3);
	SAFE_DELETE(m_pRadioBtn4);
}

void CAutoCreateSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAutoCreateSetupDlg, CDialogEx)
	ON_MESSAGE(RADIO_BUTTON_CLICK, OnRadioButtnClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(LIST_CONTROL, OnListControl)
END_MESSAGE_MAP()

// CAutoCreateSetupDlg 메시지 처리기입니다.

const TCHAR* lpszHeader[] = { _T("NO"), _T("이름"), _T("전화\n번호"), _T("상태"), _T("담당 업무"), _T("비고"), NULL };
const int colWidthList[] = { 100, 150, 150, 100, 200, 250, 0 }; // pixel
const int colType[] = { POPUP_TYPE_SEQ, POPUP_TYPE_LIST_ADD , POPUP_TYPE_LIST_NOINPUT, POPUP_TYPE_SPACE, POPUP_TYPE_TEXT, POPUP_TYPE_EDIT, 0 };
const int colAlign[] = { DT_CENTER, DT_LEFT, DT_RIGHT, DT_CENTER, DT_LEFT, DT_CENTER, -1 }; // option, row align text, default: DT_LEFT
const int colLimit[] = { 0, 10, 0, 0, 40, 10, -1 }; // 0: 무제한, > 0 : 텍스트 사이즈 제한

BOOL CAutoCreateSetupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rect;
	GetClientRect(rect);

	CString sCaption = L"지하층/지상층 분할";
	m_pRadioBtn1 = new CRadioTextBtn(0, sCaption, this);
	m_pRadioBtn1->Create(IDD_COMMON_CHILD_DIALOG, this);
	int nX = 40;
	m_pRadioBtn1->MoveWindow(nX, 100, m_pRadioBtn1->GetWidthExtent(sCaption), 16);
	nX += m_pRadioBtn1->GetWidthExtent(sCaption) + 15;
	m_pRadioBtn1->ShowWindow(SW_SHOW);

	sCaption = L"자동 층 분할";
	m_pRadioBtn2 = new CRadioTextBtn(1, sCaption, this);
	m_pRadioBtn2->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pRadioBtn2->MoveWindow(nX, 100, m_pRadioBtn2->GetWidthExtent(sCaption), 16);
	nX += m_pRadioBtn2->GetWidthExtent(sCaption) + 15;
	m_pRadioBtn2->ShowWindow(SW_SHOW);

	sCaption = L"계단 분할";
	m_pRadioBtn3 = new CRadioTextBtn(2, sCaption, this);
	m_pRadioBtn3->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pRadioBtn3->MoveWindow(nX, 100, m_pRadioBtn3->GetWidthExtent(sCaption), 16);
	nX += m_pRadioBtn3->GetWidthExtent(sCaption) + 15;
	m_pRadioBtn3->ShowWindow(SW_SHOW);

	sCaption = L"분할 안 함";
	m_pRadioBtn4 = new CRadioTextBtn(3, sCaption, this);
	m_pRadioBtn4->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pRadioBtn4->MoveWindow(nX, 100, m_pRadioBtn4->GetWidthExtent(sCaption), 16);
	nX += m_pRadioBtn4->GetWidthExtent(sCaption) + 15;
	m_pRadioBtn4->ShowWindow(SW_SHOW);

	CRuntimeClass	*pRuntimeClass;
	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomListCtrl))) {
		if (NULL != (m_pListCtrl = (CCustomListCtrl *)(pRuntimeClass->CreateObject()))) {
			m_pListCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}
	m_pListCtrl->SetListIndex(0);
	m_pListCtrl->SetHeaderInfo((TCHAR**)lpszHeader, (int*)colWidthList, (int*)colType, (int*)colAlign); // Align: option, row align text, default: DT_LEFT
	m_pListCtrl->SetColumnLimit((int*)colLimit);
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

	// 폰트 관련 (option)
	/*m_pListCtrl->SetRowFontSize(20);
	m_pListCtrl->SetRowFontBold(FW_BOLD);
	m_pListCtrl->SetRowFontName(L"돋움");
	m_pListCtrl->SetHeaderFontSize(20);
	m_pListCtrl->SetHeaderFontBold(FW_BOLD);
	m_pListCtrl->SetHeaderFontName(L"돋움");*/
	// 리스트 팝업 아이템 추가
	m_pListCtrl->AddPopupListItem(2, L"지상 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B1F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B2F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B3F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B4F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B5F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B6F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B7F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B8F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B9F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B10F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B11F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B12F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B13F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B14F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B15F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B16F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B17F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B18F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B19F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B20F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B21F 주차장");
	m_pListCtrl->AddPopupListItem(2, L"B22F 주차장");

	m_pListCtrl->AddPopupListItem(1, L"김혜자");
	m_pListCtrl->AddPopupListItem(1, L"최불암");
	m_pListCtrl->AddPopupListItem(1, L"송강호");
	m_pListCtrl->AddPopupListItem(1, L"장진영");
	m_pListCtrl->AddPopupListItem(1, L"김혜수");
	m_pListCtrl->AddPopupListItem(1, L"유재석");
	m_pListCtrl->AddPopupListItem(1, L"강호동");
	m_pListCtrl->AddPopupListItem(1, L"김형섭");
	m_pListCtrl->AddPopupListItem(1, L"황인성");
	m_pListCtrl->AddPopupListItem(1, L"김호");
	m_pListCtrl->AddPopupListItem(1, L"엄태화");
	m_pListCtrl->AddPopupListItem(1, L"이나영");

	m_pListCtrl->AddListItemState(3, L"활성상태");
	m_pListCtrl->AddListItemState(3, L"비활성상태");
	m_pListCtrl->AddListItemState(3, L"대기상태");

	m_pListCtrl->SendMessage(WM_INITIALUPDATE);

	rect.top = 100;
	rect.bottom -= 100;

	m_pListCtrl->MoveWindow(rect);
	m_pListCtrl->ShowWindow(SW_SHOW);

	// test code
	CString sText;
	m_pListCtrl->AddItem(255);
	for (int nIndex = 0; nIndex < 255; nIndex++) {
		sText.Format(L"홍길동 %d", nIndex + 1);
		m_pListCtrl->SetItemText(nIndex, 1, sText);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAutoCreateSetupDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CAutoCreateSetupDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CAutoCreateSetupDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CAutoCreateSetupDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rectES;
	CRect moveableRect;
	GetClientRect(&moveableRect);

	rectES.SetRect(moveableRect.right - 20, moveableRect.bottom - 20, moveableRect.right, moveableRect.bottom);
	if (rectES.PtInRect(point)) {
		GetParent()->SendMessage(WM_MOUSEMOVE, nFlags, 0);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CAutoCreateSetupDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CAutoCreateSetupDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rectES;
	CRect moveableRect;
	GetClientRect(&moveableRect);

	rectES.SetRect(moveableRect.right - 20, moveableRect.bottom - 20, moveableRect.right, moveableRect.bottom);
	if (rectES.PtInRect(point)) {
		GetParent()->SendMessage(WM_MOUSEMOVE, 0, 0);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CAutoCreateSetupDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	m_pRadioBtn1->Invalidate();
	m_pRadioBtn2->Invalidate();
	m_pRadioBtn3->Invalidate();
	m_pRadioBtn4->Invalidate();
}

void CAutoCreateSetupDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!IsIconic()) {
		Redisplay();
	}
}

void CAutoCreateSetupDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CAutoCreateSetupDlg::OnRadioButtnClick(WPARAM wParam, LPARAM lParam)
{
	int nIndex = wParam;

	m_pRadioBtn1->SetCheck(false);
	m_pRadioBtn2->SetCheck(false);
	m_pRadioBtn3->SetCheck(false);
	m_pRadioBtn4->SetCheck(false);
	switch (nIndex) {
	case 0: m_pRadioBtn1->SetCheck(true); break;
	case 1: m_pRadioBtn2->SetCheck(true); break;
	case 2: m_pRadioBtn3->SetCheck(true); break;
	case 3: m_pRadioBtn4->SetCheck(true); break;
	default: break;
	}

	return 0;
}

LRESULT CAutoCreateSetupDlg::OnListControl(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;
	int nSelectItem = (int)lParam;
	switch (nType)
	{
	case 1: // del
		if (m_pListCtrl) {
			CMessagePopup popup(L"목록 삭제", L"\n\n선택한 목록을 삭제하시면\n\n마지막 목록에 신규 목록이 1개 추가됩니다.\n\n선택한 목록을 삭제하시겠습니까?", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				m_pListCtrl->DelListItem(nSelectItem);
				m_pListCtrl->AddItem();
			}
		}
		break;
	case 2: // up add
		if (m_pListCtrl) {
			CMessagePopup popup(L"목록 추가", L"\n\n신규 목록을 추가하시면\n\n마지막 목록 1개가 삭제됩니다.\n\n신규 목록을 추가하시겠습니까?", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				m_pListCtrl->InsertListItemUp(nSelectItem);
				m_pListCtrl->DelListItem(m_pListCtrl->GetItemCount() - 1);
				m_pListCtrl->Invalidate();
			}
		}
		break;
	case 3: // down add
		if (m_pListCtrl) {
			CMessagePopup popup(L"목록 추가", L"\n\n신규 목록을 추가하시면\n\n마지막 목록 1개가 삭제됩니다.\n\n신규 목록을 추가하시겠습니까?", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				m_pListCtrl->InsertListItemDown(nSelectItem);
				m_pListCtrl->DelListItem(m_pListCtrl->GetItemCount() - 1);
				m_pListCtrl->Invalidate();
			}
		}
		break;
	default:
		break;

	}
	return 0;
}

void CAutoCreateSetupDlg::Redisplay()
{
	CRect rect, rt;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	rt.CopyRect(&rect);
	rt.top -= 2;
	CCommonDisplay::DrawRect(&memDC, true, RGB(120, 120, 120), RGB(238, 238, 238), rt);

	rt.top += 17;
	rt.left += 15;
	rt.right -= 15;
	rt.bottom -= 15;
	CCommonDisplay::DrawRect(&memDC, true, RGB(255, 255, 255), RGB(255, 255, 255), rt);

	CCommonDisplay::DrawImage(&memDC, IDB_BMP_BOX_LT, rt.left, rt.top);
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_BOX_RT, rt.right - 13, rt.top);
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_BOX_LB, rt.left, rt.bottom - 13);
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_BOX_RB, rt.right - 13, rt.bottom - 13);

	CCommonDisplay::DrawLine(&memDC, rt.left, rt.top + 53, rt.right, rt.top + 53, PS_SOLID, 2, RGB(211, 122, 56));
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_TITLEBAR, rt.left + 40, rt.top + 48);

	CCommonDisplay::DrawImage(&memDC, IDB_BMP_AUTOTITLE, 60, 30);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_pListCtrl) {
		rt.top += 150;
		rt.bottom -= 20;
		rt.left += 10;
		rt.right -= 10;

		m_pListCtrl->MoveWindow(rt);
		m_pListCtrl->ShowWindow(SW_SHOW);
		m_pListCtrl->Invalidate();
	}
}
