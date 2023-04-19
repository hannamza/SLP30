// MakeDataDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "MakeDataDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"

bool g_bDataLink = false;
void func_thread(CMakeDataDlg* pWnd)
{
	// ������ ����
	int nResult = CDataLinkManager::ie()->MakeExcelData(pWnd->m_sExcelPath);
	if (nResult >= 0) {
		CDataLinkManager::ie()->ReleaseExcelWaitPopup();
		pWnd->PostMessage(EXCEL_SAVE_MSG, 0, 0);
		return;
	}
	/*if (nResult < 0) {
		pWnd->PostMessage(EXCEL_SAVE_MSG, 1, 1);
	}*/

	CDataLinkManager::ie()->ReleaseExcelWaitPopup();
}

LRESULT CMakeDataDlg::OnRomSaveMsg(WPARAM wParam, LPARAM lParam)
{
	int nProcess = (int)lParam;
	int nCase = (int)wParam;
	switch (nCase) {
	case 0:
		m_prgCreate.SetPos(nProcess);
		break;
	case 1:
		m_prgCreate.SetPos(0);
		break;
	case 2:
		m_prgCreate.SetPos(100);
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CMakeDataDlg::OnExcelSaveMsg(WPARAM wParam, LPARAM lParam)
{
	int nMsg = (int)wParam;
	int nProcess = (int)lParam;

	switch (nMsg) {
	case 0:
	{
		CMessagePopup popup(L"EXCEL ����", L"\n\n���� ���� �Ϸ�\n\n������ Ȯ���ϰڽ��ϱ�?", MB_YESNO, this);
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

// CMakeDataDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMakeDataDlg, CDialogEx)

CMakeDataDlg::CMakeDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DATA, pParent)
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
		_T("����")); // lpszFacename 

	m_bInit = false;
	m_pThread = NULL;
}

CMakeDataDlg::~CMakeDataDlg()
{
	g_bDataLink = false;
	if (m_pThread)
	{
		m_pThread->join();
		SAFE_DELETE(m_pThread);
	}
}

void CMakeDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_editPath);
	DDX_Control(pDX, IDC_PROGRESS_DATA, m_prgCreate);
}

BEGIN_MESSAGE_MAP(CMakeDataDlg, CDialogEx)
	ON_MESSAGE(EXCEL_SAVE_MSG, OnExcelSaveMsg)
	ON_MESSAGE(ROM_SAVE_MSG, OnRomSaveMsg)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnFolderClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnMakeDataClick)
	//ON_COMMAND(IDC_COMMON_BUTTON3, OnNextClick)
	ON_COMMAND(IDC_COMMON_BUTTON4, OnPrevClick)
	ON_COMMAND(IDC_COMMON_BUTTON5, OnExcelClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMakeDataDlg �޽��� ó�����Դϴ�.

BOOL CMakeDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_btnFolder.Create(IDB_BMP_FOLDER, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnMakeData.Create(IDB_BMP_MAKEDATA, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);

	m_editPath.MoveWindow(49, 87, 475, 22);
	m_prgCreate.MoveWindow(49, 138, 761, 17);

	m_btnFolder.MoveWindow(540, 82, 32, 32);
	m_btnMakeData.MoveWindow(603, 74, 194, 48);

	CReadWriteState state;
	state.SetFileName(L"path.ini", 1);
	if (!state.ReadState(L"PATH", L"DATA_PATH", m_sPath)) {
		WCHAR szDocPath[2048];
		SHGetSpecialFolderPath(NULL, szDocPath, CSIDL_MYDOCUMENTS, FALSE);
		m_sPath = szDocPath;
	}
	SetDlgItemText(IDC_EDIT_PATH, m_sPath);

	m_prgCreate.SetRange(0, 100);

	//m_btnNext.Create(IDB_BMP_NEXT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
	m_btnPrev.Create(IDB_BMP_PREV, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON4);
	m_btnExcel.Create(IDB_BMP_DATAEXCEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON5);
	m_btnPrev.ShowWindow(SW_HIDE);

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CMakeDataDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CMakeDataDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CMakeDataDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CMakeDataDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMakeDataDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMakeDataDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CMakeDataDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();
}

void CMakeDataDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CMakeDataDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg) {
		// �������� ���̾�α��� �ʱ�ȭ�� ���� ���
	case BFFM_INITIALIZED:
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	}
	return 0;
}

void CMakeDataDlg::OnFolderClick()
{
	BROWSEINFO brInfo;
	TCHAR szBuffer[512];                                      // ������� ���� 

	::ZeroMemory(&brInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	if (!::PathIsDirectory(m_sPath)) {
		WCHAR szDocPath[2048];
		SHGetSpecialFolderPath(NULL, szDocPath, CSIDL_MYDOCUMENTS, FALSE);
		SetDlgItemText(IDC_EDIT_PATH, szDocPath);

		m_sPath = szDocPath;
	}

	COLORREF clrBar = RGB(255, 127, 19); // the bar color
	m_prgCreate.SendMessage(PBM_SETBARCOLOR, 0, (LPARAM)clrBar);

	brInfo.hwndOwner = GetSafeHwnd();
	brInfo.lpszTitle = _T("���������� ���� ������ �����ϼ���");
	brInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	brInfo.lParam = (LPARAM)m_sPath.GetBuffer(0);
	brInfo.lpfn = BrowseCallbackProc;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&brInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // ���ϰ�� �о����
																// ��θ� ������ ����� ���, Edit Control �� �� ����
	CString str;
	str.Format(_T("%s"), szBuffer);
	if (str.GetLength() > 0) {
		SetDlgItemText(IDC_EDIT_PATH, str);
		m_sPath = str;
	}
}

void CMakeDataDlg::OnMakeDataClick()
{
	int nValue = 0;
	CString sText;
	sText.Format(L"\n���������� ���� ��\n\n���ͳݿ� ������ �ʿ���\n\n����Ͻðڽ��ϱ�?", m_sPath);
	CMessagePopup popup(L"���������� ����", sText, MB_YESNO, this);
	UINT nResult = popup.DoModal();
	if (nResult != IDOK) {
		return;
	}

	CDataLinkManager::ie()->SetDirPath(m_sPath);

	CString sPath;
	WCHAR szDocPath[2048];
	SHGetSpecialFolderPath(NULL, szDocPath, CSIDL_MYDOCUMENTS, FALSE);

	for (int nIndex = 0; nIndex < 100; nIndex++) {
		sPath.Format(L"%s\\slp30\\edit%d.tmp", szDocPath, nIndex);
		if (::PathFileExists(sPath)) {
			::DeleteFile(sPath);
		}
		if (!::PathFileExists(sPath)) {
			break;
		}
	}
	if (::PathFileExists(sPath)) {
		CMessagePopup popup(L"���������� ����", L"\n\n���������� ���� ����", MB_OK, this);
		popup.DoModal();
		return;
	}
	PostMessage(ROM_SAVE_MSG, 1, 0);

	CTime time = CTime::GetCurrentTime();
	CSaveManager::ie()->m_versionDate.nYear = time.GetYear();
	CSaveManager::ie()->m_versionDate.nDay = time.GetDay();
	CSaveManager::ie()->m_versionDate.nMonth = time.GetMonth();
	CSaveManager::ie()->m_versionDate.nHour = time.GetHour();
	CSaveManager::ie()->m_versionDate.nMin = time.GetMinute();
	CSaveManager::ie()->m_versionDate.nSec = time.GetSecond();
	if (++CSaveManager::ie()->m_versionDate.cMinorVer >= 100) {
		++CSaveManager::ie()->m_versionDate.cMajorVer;
		CSaveManager::ie()->m_versionDate.cMinorVer = 0;
	}

	wcscpy_s(CSaveManager::ie()->m_versionDate.szUserId, CCommonState::ie()->m_sUserID.GetBuffer(0));
	if (CSaveManager::ie()->FileSave(sPath)) {
		// ���� ����
		PostMessage(ROM_SAVE_MSG, 0, 5);
		CCommonState::ie()->m_nReturnValue = -99;
		if (!m_createProcess.SendDataToWebdav(sPath, L"��������.slp", CCommonState::ie()->m_sUserID)) {
			::DeleteFile(sPath);
			CMessagePopup popup(L"���������� ����", L"\n\n���������� ���� ����\n\n���ͳ� ������ Ȯ���ϼ���.", MB_OK, this);
			popup.DoModal();
			return;
		}
		while (CCommonState::ie()->m_nReturnValue == -99) {
			Sleep(50);
		}
		if (CCommonState::ie()->m_nReturnValue != 0) {
			::DeleteFile(sPath);
			CMessagePopup popup(L"���������� ����", L"\n\n���������� ���� ����\n\n���ͳ� ����/�����ڿ��� �����ϼ���.", MB_OK, this);
			popup.DoModal();
			return;
		}
	}
	else {
		return;
	}
	PostMessage(ROM_SAVE_MSG, 0, 25);

	int nSlpIndex = 1;
	CString sTempPath;
	sTempPath.Format(L"%s\\��������.slp", m_sPath);
	while (::PathFileExists(sTempPath)) {
		sTempPath.Format(L"%s\\��������(%d).slp", m_sPath, nSlpIndex++);
	}

	CopyFile(sPath, sTempPath, true);

	::DeleteFile(sPath);

	PostMessage(ROM_SAVE_MSG, 0, 30);

	if (CDataLinkManager::ie()->MakeRedboxOPData(this)) {
		CMessagePopup popup1(L"���������� ����", L"\n\n���������� ���� �Ϸ�\n\n�ش� ��η� �̵��ұ��?", MB_YESNO, this);
		UINT nResult = popup1.DoModal();
		if (nResult == IDOK) {
			ShellExecute(NULL, L"open", L"explorer.exe", m_sPath, NULL, SW_SHOW);
		}
	}
	else {
		CMessagePopup popup2(L"���������� ���� ����", L"\n\n���������� ���� ����", MB_OK, this);
		popup2.DoModal();
	}

	CReadWriteState state;
	state.SetFileName(L"path.ini", 1);
	state.WriteState(L"PATH", L"DATA_PATH", m_sPath);

	return;
}

//void CMakeDataDlg::OnNextClick()
//{
//
//}

void CMakeDataDlg::OnPrevClick()
{
	//GetParent()->PostMessage(SELECTION_PROJECT, 13, 0);
}

void CMakeDataDlg::OnExcelClick()
{
	CString str = _T("Excel ����(*.xlsx)|*.xlsx|");
	CFileDialog dlg(false, _T("*.xlsx"), _T("����������.xlsx"), /*OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/NULL, str, this);
	if (dlg.DoModal() == IDOK) {
		m_sExcelPath = dlg.GetPathName();
		// ���� ��θ� ������ ����� ���, Edit Control�� �� ����
	}
	else {
		return;
	}

	g_bDataLink = false;
	if (m_pThread)
	{
		m_pThread->join();
		SAFE_DELETE(m_pThread);
	}

	if (!g_bDataLink) {
		g_bDataLink = true;
		m_pThread = new thread(&func_thread, this);
	}
}

void CMakeDataDlg::Redisplay()
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
	rt.SetRect(20, 60, 850, 175);
	CCommonDisplay::DrawRect(&memDC, false, RGB(150, 150, 150), 0, rt);
	//rt.SetRect(20, 170, 850, 610);
	//CCommonDisplay::DrawRect(&memDC, false, RGB(150, 150, 150), 0, rt);
	rt.SetRect(20, 15, 350, 50);
	CCommonDisplay::DrawCaption(&memDC, L"���������� ����", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);

	/*rt.CopyRect(&rect);
	rt.left += 4;
	rt.right -= 4;
	rt.top += 50;
	rt.bottom -= 7;
	CCommonDisplay::DrawRect(&memDC, false, RGB(220, 220, 220), 0, rt);*/

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInit) {
		/*m_btnNext.MoveWindow(rect.right - 87, 0, 83, 48);
		m_btnPrev.MoveWindow(rect.right - 180, 0, 83, 48);
		m_btnExcel.MoveWindow(rect.right - 440, 0, 155, 48);*/
		//m_btnPrev.MoveWindow(rect.right - 87, 0, 83, 48);
		m_btnExcel.MoveWindow(/*rect.right*/1024 - 340, 8, 155, 48);
		//m_btnNext.Invalidate();
		m_btnPrev.Invalidate();
		m_btnExcel.Invalidate();
	}

	//20230419 GBM start - ���, ���� / ���������� ���� ��ư, ���α׷����� ȭ�� �̰��� ���� ����
	m_btnFolder.Invalidate();
	m_btnMakeData.Invalidate();
	m_editPath.Invalidate();
	m_prgCreate.Invalidate();
	//20230419 GBM end
}