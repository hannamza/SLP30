// MainFrameDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "MainFrameDlg.h"
#include "afxdialogex.h"


// CMainFrameDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMainFrameDlg, CDialogEx)

CMainFrameDlg::CMainFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_pCircuitBasicDlg = NULL;
	m_pCircuitDlg = NULL;
	m_pSelectionDlg = NULL;
	m_pCircuitEditDlg = NULL;
	//m_pSetupDlg = NULL;
	m_bInitialize = false;
	m_pWaitPopup = NULL;
	m_pMakeDataDlg = NULL;

	m_nStair = 1;
	m_nFloor = 2;
	m_nBasement = 0;
}

CMainFrameDlg::~CMainFrameDlg()
{
	SAFE_DELETE(m_pCircuitBasicDlg);
	SAFE_DELETE(m_pCircuitDlg);
	SAFE_DELETE(m_pSelectionDlg);
	SAFE_DELETE(m_pCircuitEditDlg);
	SAFE_DELETE(m_pWaitPopup);
	SAFE_DELETE(m_pMakeDataDlg);
}

void CMainFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainFrameDlg, CDialogEx)
	ON_MESSAGE(SELECTION_PROJECT, OnSelectionProject)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnSelectClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnCircuitClick)
	ON_COMMAND(IDC_COMMON_BUTTON3, OnDataClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMainFrameDlg 메시지 처리기입니다.

BOOL CMainFrameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_pSelectionDlg = new CStartProjectSelectionDlg(this);
	m_pSelectionDlg->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pSelectionDlg->ShowWindow(SW_SHOW);

	m_pCircuitBasicDlg = new CCircuitBasicInfoDlg(this);
	m_pCircuitBasicDlg->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pCircuitBasicDlg->ShowWindow(SW_HIDE);

	m_pCircuitDlg = new CCircuitInfoDlg(this);
	m_pCircuitDlg->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pCircuitDlg->ShowWindow(SW_HIDE);

	m_pCircuitEditDlg = new CCircuitEditDlg(this);
	m_pCircuitEditDlg->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pCircuitEditDlg->ShowWindow(SW_HIDE);

	m_pMakeDataDlg = new CMakeDataBasicDlg(this);
	m_pMakeDataDlg->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pMakeDataDlg->ShowWindow(SW_HIDE);

	/*m_pSetupDlg = new CAutoCreateSetupDlg(this);
	m_pSetupDlg->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pSetupDlg->ShowWindow(SW_HIDE);*/

	m_btnStart.Create(IDB_BMP_SELPROJECT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnStart.MoveWindow(45, 43, 111, 29);
	m_btnCircuit.Create(IDB_BMP_CIRCUIT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnCircuit.MoveWindow(181, 43, 80, 29);
	m_btnData.Create(IDB_BMP_CREATEDATA, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
	m_btnData.MoveWindow(286, 43, 131, 29);

	m_btnData.EnableWindow(false);
	m_btnStart.SetCheckEx(true);

	m_bInitialize = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMainFrameDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CMainFrameDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CMainFrameDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CMainFrameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rectES;
	CRect moveableRect;
	GetClientRect(&moveableRect);

	rectES.SetRect(moveableRect.right - 20, moveableRect.bottom - 20, moveableRect.right, moveableRect.bottom);
	if (rectES.PtInRect(point)) {
		GetParent()->SendMessage(WM_MOUSEMOVE, 0, 0);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMainFrameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMainFrameDlg::OnMouseMove(UINT nFlags, CPoint point)
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

void CMainFrameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	m_btnStart.Invalidate();
	m_btnCircuit.Invalidate();
	m_btnData.Invalidate();
}

void CMainFrameDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_bInitialize) {
		Redisplay();
	}
}

void CMainFrameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1) {
		KillTimer(1);
		//m_pCircuitDlg->PostMessage(MAKE_CIRCUIT_INFO, 0, 0);
		SetTimer(2, 100, NULL);
	}
	else if (nIDEvent == 2) {
		if (!CCommonState::ie()->m_bWaitCircuit) {
			KillTimer(2);
			if (m_pWaitPopup) {
				m_pWaitPopup->ShowWindow(SW_HIDE);
			}
			SetTabButton(11);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CMainFrameDlg::SetTabButton(int nIndex)
{
	if (m_btnStart.GetCheckEx()) {
		m_btnStart.SetCheckEx(false);
	}
	if (m_btnCircuit.GetCheckEx()) {
		m_btnCircuit.SetCheckEx(false);
	}
	if (m_btnData.GetCheckEx()) {
		m_btnData.SetCheckEx(false);
	}

	int nValue = 0;
	switch (nIndex)
	{
	case 0:
		m_btnStart.SetCheckEx(true);
		m_pSelectionDlg->ShowWindow(SW_SHOW);
		m_pCircuitDlg->ShowWindow(SW_HIDE);
		m_pCircuitBasicDlg->ShowWindow(SW_HIDE);
		m_pCircuitEditDlg->ShowWindow(SW_HIDE);
		m_pMakeDataDlg->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_btnCircuit.SetCheckEx(true);
		//m_pSetupDlg->ShowWindow(SW_SHOW);
		m_pCircuitBasicDlg->ShowWindow(SW_SHOW);
		m_pSelectionDlg->ShowWindow(SW_HIDE);
		m_pCircuitDlg->ShowWindow(SW_HIDE);
		m_pCircuitEditDlg->ShowWindow(SW_HIDE);
		m_pMakeDataDlg->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_btnData.SetCheckEx(true);
		//m_pSetupDlg->ShowWindow(SW_SHOW);
		m_pMakeDataDlg->CallFirstDisplay();
		m_pMakeDataDlg->ShowWindow(SW_SHOW);
		m_pCircuitBasicDlg->ShowWindow(SW_HIDE);
		m_pSelectionDlg->ShowWindow(SW_HIDE);
		m_pCircuitDlg->ShowWindow(SW_HIDE);
		m_pCircuitEditDlg->ShowWindow(SW_HIDE);
		break;
	case 11:
		/*nValue = m_pCircuitEditDlg->DisplayListItem();
		if (nValue != 0)
		{
			CString sText;
			sText.Format(L"\n\n%d계통 최대 입력 개수(250개)를 초과하였습니다.\n\n다시 입력하여 주시기 바랍니다.", nValue - 1);
			CMessagePopup popup(L"중계기 일람표", sText, MB_OK, this);
			UINT nResult = popup.DoModal();
			CCommonState::ie()->ReleaseCircuit(0);
			CCommonState::ie()->ReleaseCircuit(1);
			CCommonState::ie()->m_bInitCircuit = true;
			return;
		}*/
		m_btnCircuit.SetCheckEx(true);
		m_pCircuitDlg->ShowWindow(SW_SHOW);
		m_pCircuitBasicDlg->ShowWindow(SW_HIDE);
		m_pSelectionDlg->ShowWindow(SW_HIDE);
		m_pCircuitEditDlg->ShowWindow(SW_HIDE);
		m_pMakeDataDlg->ShowWindow(SW_HIDE);
		break;
	case 12:

		//20221019 GBM start - 중계기 일람표가 기작성된 상태에서 회로 편집 후 중계기 일람표로 돌아오는 상황이라면 리스트 UI와 데이터를 삭제하지 않고 추가/삭제 분을 적용하도록 한다.
#if 1
		// 중계기 일람표가 최초 작성되는 것이라면 기존 루틴 태운다.
		if (CCommonState::ie()->m_bInitCircuit)
		{
			nValue = m_pCircuitEditDlg->DisplayListItem();
			if (nValue != 0)
			{
				CString sText;
				sText.Format(L"\n\n%d계통 최대 입력 개수(%d개)를 초과하였습니다.\n\n다시 입력하여 주시기 바랍니다.", nValue - 1, MAX_CIRCUIT);
				CMessagePopup popup(L"중계기 일람표", sText, MB_OK, this);
				UINT nResult = popup.DoModal();
				CCommonState::ie()->ReleaseCircuit(0);
				CCommonState::ie()->ReleaseCircuit(1);
				//CCommonState::ie()->m_bInitCircuit = true;
				return;
			}
			CCommonState::ie()->m_bInitCircuit = false;
		}
		else
		{
			// 4. 회로 데이터 리스트를 초기화 하지 않도록 한다.

			// 5. 삭제되어야할 설비 리스트의 회로 리스트를 토대로 매칭된 기존 중계기 일람표 회로 리스트에서 역순으로 찾아 동,계단,층,회로명이 매칭된 회로를 삭제해야할 회로 개수만큼을 지운다.
			CSaveManager::ie()->DeleteSystemInfo();

			// 6. 새 회로 데이터 리스트를 기존 회로 데이터 리스트 뒤에 추가한다.
			CSaveManager::ie()->AddSystemInfo();

			CCommonState::ie()->m_bInitCircuit = false;

			// 7. 새로 편집된 회로 데이터 리스트를 UI로 표출한다.
			m_pCircuitEditDlg->LoadInfo();
		}

#else
		nValue = m_pCircuitEditDlg->DisplayListItem();
		if (nValue != 0)
		{
			CString sText;
			sText.Format(L"\n\n%d계통 최대 입력 개수(%d개)를 초과하였습니다.\n\n다시 입력하여 주시기 바랍니다.", nValue - 1, MAX_CIRCUIT);
			CMessagePopup popup(L"중계기 일람표", sText, MB_OK, this);
			UINT nResult = popup.DoModal();
			CCommonState::ie()->ReleaseCircuit(0);
			CCommonState::ie()->ReleaseCircuit(1);
			CCommonState::ie()->m_bInitCircuit = true;
			return;
		}
		CCommonState::ie()->m_bInitCircuit = false;
#endif
		//20221019 GBM end

		m_btnCircuit.SetCheckEx(true);
		m_pCircuitEditDlg->ShowWindow(SW_SHOW);
		m_pCircuitDlg->ShowWindow(SW_HIDE);
		m_pCircuitBasicDlg->ShowWindow(SW_HIDE);
		m_pSelectionDlg->ShowWindow(SW_HIDE);
		m_pMakeDataDlg->ShowWindow(SW_HIDE);
		break;
	case 13:
		m_btnCircuit.SetCheckEx(true);
		m_pCircuitEditDlg->ShowWindow(SW_SHOW);
		m_pCircuitDlg->ShowWindow(SW_HIDE);
		m_pCircuitBasicDlg->ShowWindow(SW_HIDE);
		m_pSelectionDlg->ShowWindow(SW_HIDE);
		m_pMakeDataDlg->ShowWindow(SW_HIDE);
		break;
	default: break;
	}
}

LRESULT CMainFrameDlg::OnSelectionProject(WPARAM wParam, LPARAM lParam)
{
	int nResult = (int)wParam;
	int nValue = (int)lParam;
	m_btnData.EnableWindow(false);
	switch (nResult) {
	case 0: // new (circuit 1)
		if (nValue == 1) {
			m_pCircuitBasicDlg->InfoInit();
			m_btnData.EnableWindow(false);
		}
		wcscpy_s(CSaveManager::ie()->m_versionDate.szUserId, CCommonState::ie()->m_sUserID.GetBuffer(0));
		CSaveManager::ie()->m_versionDate.cMajorVer = 1;
		CSaveManager::ie()->m_versionDate.cMinorVer = 1;
		SetTabButton(1);
		break;
	case 1: // load	
		SetTabButton(1);
		break;
	case 6: // 연동 데이터
		SetTabButton(2);
		m_btnData.EnableWindow(true);
		break;
	case 7: // disable
		m_btnData.EnableWindow(false);
		break;
	case 10: // load
		m_pCircuitBasicDlg->LoadInfo();
		m_pCircuitDlg->LoadInfo();
		m_pCircuitEditDlg->LoadInfo();
		SetTabButton(10);
		break;
	case 11: // circuit 2
		if (nValue != 0) {
			if (m_pCircuitBasicDlg) {
				m_pCircuitBasicDlg->GetCircuitInfo(m_sBuildName, m_nStair, m_nFloor, m_nBasement);
			}
			if (m_pCircuitDlg) {
				m_pCircuitDlg->InitCircuitInfo();
			}
			CRect rect, rt, rect1;
			GetClientRect(&rect);
			ClientToScreen(&rect);
			if (m_pWaitPopup) {
				SAFE_DELETE(m_pWaitPopup);
			}
			m_pWaitPopup = new CMessagePopup(L"회로 정보 생성", L"\n\n\n회로 정보를 생성중입니다. \n\n잠시만 기다려 주세요.", -1, this);
			m_pWaitPopup->Create(IDD_COMMON_POPUP_DIALOG, this);
			m_pWaitPopup->ShowWindow(SW_HIDE);

			m_pWaitPopup->GetClientRect(&rt);
			m_pWaitPopup->ClientToScreen(&rt);

			rect1.CopyRect(&rect);
			rect1.top += rect.Height() / 2 - rt.Height() / 2;
			rect1.left += rect.Width() / 2 - rt.Width() / 2;
			rect1.bottom = rect1.top + rt.Height();
			rect1.right = rect1.left + rt.Width();
			m_pWaitPopup->MoveWindow(&rect1);
			CCommonState::ie()->m_bWaitCircuit = true;
			m_pWaitPopup->ShowWindow(SW_SHOW);
			SetTimer(1, 100, NULL);
		}
		else {
			SetTabButton(11);
		}
		break;
	case 12:
		if (nValue != 0) {
			SetTabButton(12);
		}
		else {
			SetTabButton(12);
		}
		break;
	case 13:
		SetTabButton(13);
		break;
	default:
		break;
	}
	return 0;
}

void CMainFrameDlg::OnSelectClick()
{
	m_btnData.EnableWindow(false);
	SetTabButton(0);
}

void CMainFrameDlg::OnCircuitClick()
{
	m_btnData.EnableWindow(false);
	SetTabButton(1);
}

void CMainFrameDlg::OnDataClick()
{
	SetTabButton(2);
}

void CMainFrameDlg::Redisplay()
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
	//CCommonDisplay::DrawImage(&memDC, IDB_BMP_TITLEBAR, rt.left + 40, rt.top + 48);

	//CCommonDisplay::DrawImage(&memDC, IDB_BMP_AUTOTITLE, 60, 30);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInitialize && m_pCircuitBasicDlg && m_pCircuitDlg) {
		m_pCircuitBasicDlg->MoveWindow(rt.left + 5, rt.top + 58, rt.Width() - 10, rt.Height() - 63);
		m_pCircuitDlg->MoveWindow(rt.left + 5, rt.top + 58, rt.Width() - 10, rt.Height() - 63);
		m_pSelectionDlg->MoveWindow(rt.left + 5, rt.top + 58, rt.Width() - 10, rt.Height() - 63);
		m_pCircuitEditDlg->MoveWindow(rt.left + 5, rt.top + 58, rt.Width() - 10, rt.Height() - 63);
		m_pMakeDataDlg->MoveWindow(rt.left + 5, rt.top + 58, rt.Width() - 10, rt.Height() - 63);
		//m_pSetupDlg->MoveWindow(rt.left + 5, rt.top + 60, rt.Width() - 10, rt.Height() - 65);
	}
}