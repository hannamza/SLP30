// CircuitEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CircuitEditDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"

// CCircuitEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCircuitEditDlg, CDialogEx)

bool g_bThread = false;
void func_thread(CCircuitEditDlg* pWnd)
{
	// 쓰레드 메인
	int nResult = CSaveManager::ie()->ExcelFileSave(pWnd->m_sExcelPath);
	if (nResult >= 0) {
		CSaveManager::ie()->ReleaseExcelWaitPopup();
		pWnd->PostMessage(EXCEL_SAVE_MSG, 0, 0);
		return;
	}
	/*else if (nResult != -1) {
		pWnd->PostMessage(EXCEL_SAVE_MSG, 1, 1);
	}*/
	CSaveManager::ie()->ReleaseExcelWaitPopup();
}

CCircuitEditDlg::CCircuitEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_bInit = false;

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

	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_pTabButton[nIndex] = NULL;
	}

	m_pCircuitChartDlg[0] = NULL;
	m_pCircuitChartDlg[1] = NULL;

	m_pPSPumpDlg = NULL;
	m_pBroadcastDlg = NULL;

	m_pThread = NULL;

	m_bExcelSaved = false;
}

CCircuitEditDlg::~CCircuitEditDlg()
{
	g_bThread = false;
	if (m_pThread)
	{
		m_pThread->join();
		SAFE_DELETE(m_pThread);
	}

	for (int nIndex = 0; nIndex < 4; nIndex++) {
		SAFE_DELETE(m_pTabButton[nIndex]);
	}
	SAFE_DELETE(m_pCircuitChartDlg[0]);
	SAFE_DELETE(m_pCircuitChartDlg[1]);

	SAFE_DELETE(m_pPSPumpDlg);
	SAFE_DELETE(m_pBroadcastDlg);
}

void CCircuitEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircuitEditDlg, CDialogEx)
	ON_MESSAGE(TEXTTAB_BUTTON_CLICK, OnTabButtnClick)
	ON_MESSAGE(EDIT_SAVE_MSG, OnSaveMessage)
	ON_MESSAGE(EXCEL_SAVE_MSG, OnExcelSaveMsg)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnNextClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnPrevClick)
	ON_COMMAND(IDC_COMMON_BUTTON3, OnSaveClick)
	ON_COMMAND(IDC_COMMON_BUTTON4, OnExcelClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCircuitEditDlg 메시지 처리기입니다.
BOOL CCircuitEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CCommonState::ie()->m_pEditWnd = this;

	m_pTabButton[0] = new CTextTabBtn(0, L"0 계통", this);
	m_pTabButton[0]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[0]->MoveWindow(170, 14, 89, 37);
	m_pTabButton[0]->ShowWindow(SW_SHOW);

	m_pTabButton[0]->SetCheck(true);

	m_pTabButton[1] = new CTextTabBtn(1, L"1 계통", this);
	m_pTabButton[1]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[1]->MoveWindow(265, 14, 89, 37);
	m_pTabButton[1]->ShowWindow(SW_SHOW);

	m_pTabButton[2] = new CTextTabBtn(2, L"PS/PUMP", this);
	m_pTabButton[2]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[2]->MoveWindow(360, 14, 89, 37);
	m_pTabButton[2]->ShowWindow(SW_SHOW);

	m_pTabButton[3] = new CTextTabBtn(3, L"비상방송", this);
	m_pTabButton[3]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[3]->MoveWindow(455, 14, 89, 37);
	m_pTabButton[3]->ShowWindow(SW_SHOW);

	m_btnNext.Create(IDB_BMP_NEXT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnPrev.Create(IDB_BMP_PREV, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnSave.Create(IDB_BMP_SAVE, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
	m_btnSave.ShowWindow(SW_HIDE);
	m_btnExcel.Create(IDB_BMP_EXCEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON4);

	m_pCircuitChartDlg[0] = new CCircuitChartDlg(this, 0);
	m_pCircuitChartDlg[0]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pCircuitChartDlg[0]->ShowWindow(SW_SHOW);

	m_pCircuitChartDlg[1] = new CCircuitChartDlg(this, 1);
	m_pCircuitChartDlg[1]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pCircuitChartDlg[1]->ShowWindow(SW_HIDE);

	m_pPSPumpDlg = new CPSPumpSetupDlg(this);
	m_pPSPumpDlg->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pPSPumpDlg->ShowWindow(SW_HIDE);

	m_pBroadcastDlg = new CBroadcastDlg(this);
	m_pBroadcastDlg->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pBroadcastDlg->ShowWindow(SW_HIDE);

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCircuitEditDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CCircuitEditDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CCircuitEditDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCircuitEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircuitEditDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircuitEditDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCircuitEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_pTabButton[nIndex]->Invalidate();
	}
}

void CCircuitEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Invalidate();
}

void CCircuitEditDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CCircuitEditDlg::MakePattern()
{
	int nCount = CSaveManager::ie()->m_listSystem.GetCount();
	SYSTEM_INFO_* pInfo = NULL;
	CString sOutputType;
	int nCircuitIndex;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pInfo = CSaveManager::ie()->m_listSystem.GetAt(CSaveManager::ie()->m_listSystem.FindIndex(nIndex));
		CPatternManager::ie()->AddPatternInfo(pInfo);
	}

	nCount = CSaveManager::ie()->m_listBC.GetCount();
	BC_INFO* pBcInfo = NULL;
	SYSTEM_INFO_ info;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pBcInfo = CSaveManager::ie()->m_listBC.GetAt(CSaveManager::ie()->m_listBC.FindIndex(nIndex));
		info.nSystem = 99;
		info.nFloor = pBcInfo->nFloor;
		info.nStair = pBcInfo->nStair;
		wcscpy_s(info.szBlock, pBcInfo->szBlock);
		wcscpy_s(info.szCircuitName, L"비상방송");
		info.nNo = nIndex + 1;
		CPatternManager::ie()->AddPatternInfo(&info);
	}
}

void CCircuitEditDlg::OnNextClick()
{
	//중계기 일람표를 한번이라도 저장했는지 여부
	if (!m_bExcelSaved)
	{
		CMessagePopup popup(L"중계기 일람표\nExcel 저장 미실행", L"\n\n\n\n[중계기 일람표 Excel 저장]을 반드시 실행해야\n\n연동데이터 생성 편집단계로 진행 가능함", MB_OK, this);
		UINT nResult = popup.DoModal();
		return;
	}

	// release
	CSaveManager::ie()->ReleaseInfo();
	if (m_pCircuitChartDlg[0] && m_pCircuitChartDlg[1]) {
		m_pCircuitChartDlg[0]->SaveInformation(0);
		m_pCircuitChartDlg[1]->SaveInformation(1);
	}
	if (m_pPSPumpDlg) {
		m_pPSPumpDlg->SaveInformation();
	}
	if (m_pBroadcastDlg) {
		m_pBroadcastDlg->SaveInformation();
	}

	int nStairCount = CCircuitBasicInfo::Instance()->m_nStair;
	int nBasement = CCircuitBasicInfo::Instance()->m_nBasement;
	int nFloor = CCircuitBasicInfo::Instance()->m_nFloor;
	CPatternManager::ie()->ReleasePattern();
	MakePattern();

	GetParent()->PostMessage(SELECTION_PROJECT, 6, false);
}

void CCircuitEditDlg::OnPrevClick()
{
	//20221018 GBM start - 한번 중계기 일람표 설정 후 이전으로 돌아가 설비 개수 바뀌었을 때 추가/삭제를 위해 현재 설비 구성 정보를 저장해 둠
	// release
	CSaveManager::ie()->ReleaseInfo();
	if (m_pCircuitChartDlg[0] && m_pCircuitChartDlg[1]) {
		m_pCircuitChartDlg[0]->SaveInformation(0);
		m_pCircuitChartDlg[1]->SaveInformation(1);
	}
	if (m_pPSPumpDlg) {
		m_pPSPumpDlg->SaveInformation();
	}
	if (m_pBroadcastDlg) {
		m_pBroadcastDlg->SaveInformation();
	}

	int nStairCount = CCircuitBasicInfo::Instance()->m_nStair;
	int nBasement = CCircuitBasicInfo::Instance()->m_nBasement;
	int nFloor = CCircuitBasicInfo::Instance()->m_nFloor;
	CPatternManager::ie()->ReleasePattern();
	MakePattern();
	//20221018 GBM end

	/*CMessagePopup popup(L"이전 화면", L"\n이전화면으로 이동하시면\n\n중계기 일람표에서 작업한 내용이 초기화됩니다.\n\n이전 화면으로 돌아가시겠습니까?", MB_YESNO, this);
	UINT nResult = popup.DoModal();
	if (nResult == IDOK) {
		GetParent()->PostMessage(SELECTION_PROJECT, 11, false);
	}*/

	GetParent()->PostMessage(SELECTION_PROJECT, 11, false);
}

void CCircuitEditDlg::OnSaveClick()
{
	FileSave();
}

int CCircuitEditDlg::ExcelSave()
{
	CString str = _T("Excel 파일(*.xlsx)|*.xlsx|");
	CFileDialog dlg(false, _T("*.xlsx"), _T("slp.xlsx"), /*OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/NULL, str, this);
	CString strPathName;
	if (dlg.DoModal() == IDOK) {
		strPathName = dlg.GetPathName();
		// 파일 경로를 가져와 사용할 경우, Edit Control에 값 저장
	}
	else {
		return -1;
	}

	CSaveManager::ie()->ReleaseInfo();

	if (m_pCircuitChartDlg[0] && m_pCircuitChartDlg[1]) {
		if (!m_pCircuitChartDlg[0]->SaveInformation(0) || !m_pCircuitChartDlg[1]->SaveInformation(1)) {
			goto _EXIT_ERROR;
		}
	}
	if (m_pPSPumpDlg) {
		if (!m_pPSPumpDlg->SaveInformation()) {
			goto _EXIT_ERROR;
		}
	}
	if (m_pBroadcastDlg) {
		if (!m_pBroadcastDlg->SaveInformation()) {
			goto _EXIT_ERROR;
		}
	}

	m_sExcelPath = strPathName;

	g_bThread = false;
	if (m_pThread)
	{
		m_pThread->join();
		SAFE_DELETE(m_pThread);
	}

	if (!g_bThread) {
		g_bThread = true;
		m_pThread = new thread(&func_thread, this);
	}

	return 0;

_EXIT_ERROR:
	return -2;
}

void CCircuitEditDlg::OnExcelClick()
{
	int nResult = ExcelSave();
	if (nResult == 0) {
		//엑셀 저장이 성공했을 경우
		if (!m_bExcelSaved)
			m_bExcelSaved = true;
	}
	else if (nResult == -2) {
		CMessagePopup popup(L"EXCEL 저장", L"\n\n\nEXCEL 저장 실패함", MB_OK, this);
		popup.DoModal();
	}
}

LRESULT CCircuitEditDlg::OnExcelSaveMsg(WPARAM wParam, LPARAM lParam)
{
	int nMsg = (int)wParam;

	switch (nMsg) {
	case 0:
	{
		CMessagePopup popup(L"정보 저장", L"\n\n파일 저장 완료됨\n\n파일을 확인하겠습니까?", MB_YESNO, this);
		UINT nResult = popup.DoModal();
		if (nResult == IDOK) {
			CCommonFunc::ExecuteProgram(m_sExcelPath.GetBuffer(0));
		}
	}
		break;
	case 1:
	{
		CMessagePopup popup(L"EXCEL 저장", L"\n\n\nEXCEL 저장 실패함", MB_OK, this);
		popup.DoModal();
	}
		break;
	default:
		break;
	}
	return 0;
}

int CCircuitEditDlg::FileSave()
{
	CString str = _T("SLP 파일(*.slp)|*.slp|");
	CFileDialog dlg(false, _T("*.slp"), _T("편집정보.slp"), /*OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/NULL, str, this);
	CString strPathName;
	if (dlg.DoModal() == IDOK) {
		strPathName = dlg.GetPathName();
		// 파일 경로를 가져와 사용할 경우, Edit Control에 값 저장
	}
	else {
		return -1;
	}

	CSaveManager::ie()->ReleaseInfo();

	if (m_pCircuitChartDlg[0] && m_pCircuitChartDlg[1]) {
		if (!m_pCircuitChartDlg[0]->SaveInformation(0) || !m_pCircuitChartDlg[1]->SaveInformation(1)) {
			goto _EXIT_ERROR;
		}
	}
	if (m_pPSPumpDlg) {
		if (!m_pPSPumpDlg->SaveInformation()) {
			goto _EXIT_ERROR;
		}
	}
	if (m_pBroadcastDlg) {
		if (!m_pBroadcastDlg->SaveInformation()) {
			goto _EXIT_ERROR;
		}
	}
	CTime time = CTime::GetCurrentTime();
	CSaveManager::ie()->m_versionDate.nYear = time.GetYear();
	CSaveManager::ie()->m_versionDate.nDay = time.GetDay();
	CSaveManager::ie()->m_versionDate.nMonth = time.GetMonth();
	CSaveManager::ie()->m_versionDate.nHour = time.GetHour();
	CSaveManager::ie()->m_versionDate.nMin = time.GetMinute();
	CSaveManager::ie()->m_versionDate.nSec = time.GetSecond();
	/*if (++CSaveManager::ie()->m_versionDate.cMinorVer >= 100) {
		++CSaveManager::ie()->m_versionDate.cMajorVer;
		CSaveManager::ie()->m_versionDate.cMinorVer = 0;
	}*/

	if (CSaveManager::ie()->FileSave(strPathName)) {
		CMessagePopup popup(L"저장", L"\n\n\n저장 완료됨", MB_OK, this);
		popup.DoModal();
	}

	return 0;

_EXIT_ERROR:
	return -2;
}

int CCircuitEditDlg::DisplayListItem()
{
	bool bResult = true;
	if (m_pCircuitChartDlg[0]) {
		bResult = m_pCircuitChartDlg[0]->DisplayListItem(0);
		if (!bResult) {
			return 1;
		}
	}
	if (m_pCircuitChartDlg[1]) {
		bResult = m_pCircuitChartDlg[1]->DisplayListItem(1);
		if (!bResult) {
			return 2;
		}
	}
	if (m_pBroadcastDlg) {
		m_pBroadcastDlg->DisplayListItem();
	}
	if (m_pPSPumpDlg) {
		m_pPSPumpDlg->DisplayListItem();
	}
	return 0;
}

bool CCircuitEditDlg::LoadInfo()
{
	bool bResult = true;
	if (m_pCircuitChartDlg[0]) {
		m_pCircuitChartDlg[0]->DisplayLoadFile(0);
	}
	if (m_pCircuitChartDlg[1]) {
		m_pCircuitChartDlg[1]->DisplayLoadFile(1);
	}
	if (m_pBroadcastDlg) {
		m_pBroadcastDlg->DisplayLoadFile();
	}
	if (m_pPSPumpDlg) {
		m_pPSPumpDlg->DisplayLoadFile();
	}
	return true;
}

LRESULT CCircuitEditDlg::OnTabButtnClick(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)wParam;
	switch (nIndex) {
	case 0:
		m_pTabButton[0]->SetCheck(true);
		m_pTabButton[1]->SetCheck(false);
		m_pTabButton[2]->SetCheck(false);
		m_pTabButton[3]->SetCheck(false);
		m_pCircuitChartDlg[0]->ShowWindow(SW_SHOW);
		m_pCircuitChartDlg[1]->ShowWindow(SW_HIDE);
		m_pPSPumpDlg->ShowWindow(SW_HIDE);
		m_pBroadcastDlg->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pTabButton[1]->SetCheck(true);
		m_pTabButton[0]->SetCheck(false);
		m_pTabButton[2]->SetCheck(false);
		m_pTabButton[3]->SetCheck(false);
		m_pCircuitChartDlg[0]->ShowWindow(SW_HIDE);
		m_pCircuitChartDlg[1]->ShowWindow(SW_SHOW);
		m_pPSPumpDlg->ShowWindow(SW_HIDE);
		m_pBroadcastDlg->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pTabButton[2]->SetCheck(true);
		m_pTabButton[1]->SetCheck(false);
		m_pTabButton[0]->SetCheck(false);
		m_pTabButton[3]->SetCheck(false);
		m_pPSPumpDlg->ShowWindow(SW_SHOW);
		m_pCircuitChartDlg[0]->ShowWindow(SW_HIDE);
		m_pCircuitChartDlg[1]->ShowWindow(SW_HIDE);
		m_pBroadcastDlg->ShowWindow(SW_HIDE);
		break;
	case 3:
		m_pTabButton[3]->SetCheck(true);
		m_pTabButton[1]->SetCheck(false);
		m_pTabButton[0]->SetCheck(false);
		m_pTabButton[2]->SetCheck(false);
		m_pBroadcastDlg->ShowWindow(SW_SHOW);
		m_pCircuitChartDlg[0]->ShowWindow(SW_HIDE);
		m_pCircuitChartDlg[1]->ShowWindow(SW_HIDE);
		m_pPSPumpDlg->ShowWindow(SW_HIDE);
		break;
	default: break;
	}
	return 0;
}

LRESULT CCircuitEditDlg::OnSaveMessage(WPARAM wParam, LPARAM lParam)
{
	FileSave();

	return 0;
}

void CCircuitEditDlg::Redisplay()
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

	rt.SetRect(20, 15, 350, 50);
	CCommonDisplay::DrawCaption(&memDC, L"중계기 일람표", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	rt.CopyRect(&rect);
	rt.left += 4;
	rt.right -= 4;
	rt.top += 50;
	rt.bottom -= 7;
	CCommonDisplay::DrawRect(&memDC, false, RGB(220, 220, 220), 0, rt);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInit) {
		m_btnNext.MoveWindow(rect.right - 87, 0, 83, 48);
		m_btnPrev.MoveWindow(rect.right - 180, 0, 83, 48);
		m_btnSave.MoveWindow(rect.right - 275, 0, 83, 48);
		m_btnExcel.MoveWindow(rect.right - 420, 0, 155, 48);
		m_btnNext.Invalidate();
		m_btnPrev.Invalidate();
		m_btnSave.Invalidate();
		m_btnExcel.Invalidate();

		if (m_pTabButton[0]) {
			m_pTabButton[0]->ShowWindow(SW_SHOW);
		}
		if (m_pTabButton[1]) {
			m_pTabButton[1]->ShowWindow(SW_SHOW);
		}
		if (m_pTabButton[2]) {
			m_pTabButton[2]->ShowWindow(SW_SHOW);
		}
		if (m_pTabButton[3]) {
			m_pTabButton[3]->ShowWindow(SW_SHOW);
		}

		rt.CopyRect(&rect);
		rt.left += 4;
		rt.right -= 4;
		rt.top += 50;
		rt.bottom -= 7;
		CCommonDisplay::DrawRect(&memDC, false, RGB(220, 220, 220), 0, rt);

		if (m_pCircuitChartDlg[0] && m_pCircuitChartDlg[1] && m_pPSPumpDlg && m_pBroadcastDlg) {
			rt.left += 5;
			rt.right -= 5;
			rt.bottom -= 5;
			rt.top += 5;
			m_pCircuitChartDlg[0]->MoveWindow(rt);
			m_pCircuitChartDlg[1]->MoveWindow(rt);
			m_pPSPumpDlg->MoveWindow(rt);
			m_pBroadcastDlg->MoveWindow(rt);
		}
	}
}

void CCircuitEditDlg::SetExcelSaveFlag(bool bFlag)
{
	m_bExcelSaved = bFlag;
}