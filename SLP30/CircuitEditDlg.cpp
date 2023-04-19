// CircuitEditDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CircuitEditDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"

// CCircuitEditDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCircuitEditDlg, CDialogEx)

bool g_bThread = false;
void func_thread(CCircuitEditDlg* pWnd)
{
	// ������ ����
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
		_T("����")); // lpszFacename 

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

// CCircuitEditDlg �޽��� ó�����Դϴ�.
BOOL CCircuitEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CCommonState::ie()->m_pEditWnd = this;

	m_pTabButton[0] = new CTextTabBtn(0, L"0 ����", this);
	m_pTabButton[0]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[0]->MoveWindow(170, 14, 89, 37);
	m_pTabButton[0]->ShowWindow(SW_SHOW);

	m_pTabButton[0]->SetCheck(true);

	m_pTabButton[1] = new CTextTabBtn(1, L"1 ����", this);
	m_pTabButton[1]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[1]->MoveWindow(265, 14, 89, 37);
	m_pTabButton[1]->ShowWindow(SW_SHOW);

	m_pTabButton[2] = new CTextTabBtn(2, L"PS/PUMP", this);
	m_pTabButton[2]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[2]->MoveWindow(360, 14, 89, 37);
	m_pTabButton[2]->ShowWindow(SW_SHOW);

	m_pTabButton[3] = new CTextTabBtn(3, L"�����", this);
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CCircuitEditDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CCircuitEditDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCircuitEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircuitEditDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircuitEditDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCircuitEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();

	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_pTabButton[nIndex]->Invalidate();
	}
}

void CCircuitEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Invalidate();
}

void CCircuitEditDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
		wcscpy_s(info.szCircuitName, L"�����");
		info.nNo = nIndex + 1;
		CPatternManager::ie()->AddPatternInfo(&info);
	}
}

void CCircuitEditDlg::OnNextClick()
{
	//�߰�� �϶�ǥ�� �ѹ��̶� �����ߴ��� ����
	if (!m_bExcelSaved)
	{
		CMessagePopup popup(L"�߰�� �϶�ǥ\nExcel ���� �̽���", L"\n\n\n\n[�߰�� �϶�ǥ Excel ����]�� �ݵ�� �����ؾ�\n\n���������� ���� �����ܰ�� ���� ������", MB_OK, this);
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
	//20221018 GBM start - �ѹ� �߰�� �϶�ǥ ���� �� �������� ���ư� ���� ���� �ٲ���� �� �߰�/������ ���� ���� ���� ���� ������ ������ ��
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

	/*CMessagePopup popup(L"���� ȭ��", L"\n����ȭ������ �̵��Ͻø�\n\n�߰�� �϶�ǥ���� �۾��� ������ �ʱ�ȭ�˴ϴ�.\n\n���� ȭ������ ���ư��ðڽ��ϱ�?", MB_YESNO, this);
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
	CString str = _T("Excel ����(*.xlsx)|*.xlsx|");
	CFileDialog dlg(false, _T("*.xlsx"), _T("slp.xlsx"), /*OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/NULL, str, this);
	CString strPathName;
	if (dlg.DoModal() == IDOK) {
		strPathName = dlg.GetPathName();
		// ���� ��θ� ������ ����� ���, Edit Control�� �� ����
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
		//���� ������ �������� ���
		if (!m_bExcelSaved)
			m_bExcelSaved = true;
	}
	else if (nResult == -2) {
		CMessagePopup popup(L"EXCEL ����", L"\n\n\nEXCEL ���� ������", MB_OK, this);
		popup.DoModal();
	}
}

LRESULT CCircuitEditDlg::OnExcelSaveMsg(WPARAM wParam, LPARAM lParam)
{
	int nMsg = (int)wParam;

	switch (nMsg) {
	case 0:
	{
		CMessagePopup popup(L"���� ����", L"\n\n���� ���� �Ϸ��\n\n������ Ȯ���ϰڽ��ϱ�?", MB_YESNO, this);
		UINT nResult = popup.DoModal();
		if (nResult == IDOK) {
			CCommonFunc::ExecuteProgram(m_sExcelPath.GetBuffer(0));
		}
	}
		break;
	case 1:
	{
		CMessagePopup popup(L"EXCEL ����", L"\n\n\nEXCEL ���� ������", MB_OK, this);
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
	CString str = _T("SLP ����(*.slp)|*.slp|");
	CFileDialog dlg(false, _T("*.slp"), _T("��������.slp"), /*OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/NULL, str, this);
	CString strPathName;
	if (dlg.DoModal() == IDOK) {
		strPathName = dlg.GetPathName();
		// ���� ��θ� ������ ����� ���, Edit Control�� �� ����
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
		CMessagePopup popup(L"����", L"\n\n\n���� �Ϸ��", MB_OK, this);
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
	CCommonDisplay::DrawCaption(&memDC, L"�߰�� �϶�ǥ", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

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