// MakeDataDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "MakeDataDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"

bool g_bDataLink = false;
void func_thread(CMakeDataDlg* pWnd)
{
	// 쓰레드 메인
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
		CMessagePopup popup(L"EXCEL 저장", L"\n\n파일 저장 완료\n\n파일을 확인하겠습니까?", MB_YESNO, this);
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

// CMakeDataDlg 대화 상자입니다.

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
		_T("굴림")); // lpszFacename 

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

// CMakeDataDlg 메시지 처리기입니다.

BOOL CMakeDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMakeDataDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CMakeDataDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CMakeDataDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CMakeDataDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMakeDataDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMakeDataDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CMakeDataDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CMakeDataDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMakeDataDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg) {
		// 폴더선택 다이얼로그의 초기화가 끝난 경우
	case BFFM_INITIALIZED:
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	}
	return 0;
}

void CMakeDataDlg::OnFolderClick()
{
	BROWSEINFO brInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

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
	brInfo.lpszTitle = _T("연동데이터 저장 폴더를 선택하세요");
	brInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	brInfo.lParam = (LPARAM)m_sPath.GetBuffer(0);
	brInfo.lpfn = BrowseCallbackProc;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&brInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // 파일경로 읽어오기
																// 경로를 가져와 사용할 경우, Edit Control 에 값 저장
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
	sText.Format(L"\n연동데이터 생성 시\n\n인터넷에 연결이 필요함\n\n계속하시겠습니까?", m_sPath);
	CMessagePopup popup(L"연동데이터 생성", sText, MB_YESNO, this);
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
		CMessagePopup popup(L"연동데이터 생성", L"\n\n연동데이터 생성 실패", MB_OK, this);
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
		// 서버 전송
		PostMessage(ROM_SAVE_MSG, 0, 5);
		CCommonState::ie()->m_nReturnValue = -99;
		if (!m_createProcess.SendDataToWebdav(sPath, L"편집정보.slp", CCommonState::ie()->m_sUserID)) {
			::DeleteFile(sPath);
			CMessagePopup popup(L"연동데이터 생성", L"\n\n연동데이터 생성 실패\n\n인터넷 연결을 확인하세요.", MB_OK, this);
			popup.DoModal();
			return;
		}
		while (CCommonState::ie()->m_nReturnValue == -99) {
			Sleep(50);
		}
		if (CCommonState::ie()->m_nReturnValue != 0) {
			::DeleteFile(sPath);
			CMessagePopup popup(L"연동데이터 생성", L"\n\n연동데이터 생성 실패\n\n인터넷 연결/관리자에게 문의하세요.", MB_OK, this);
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
	sTempPath.Format(L"%s\\편집정보.slp", m_sPath);
	while (::PathFileExists(sTempPath)) {
		sTempPath.Format(L"%s\\편집정보(%d).slp", m_sPath, nSlpIndex++);
	}

	CopyFile(sPath, sTempPath, true);

	::DeleteFile(sPath);

	PostMessage(ROM_SAVE_MSG, 0, 30);

	if (CDataLinkManager::ie()->MakeRedboxOPData(this)) {
		CMessagePopup popup1(L"연동데이터 생성", L"\n\n연동데이터 생성 완료\n\n해당 경로로 이동할까요?", MB_YESNO, this);
		UINT nResult = popup1.DoModal();
		if (nResult == IDOK) {
			ShellExecute(NULL, L"open", L"explorer.exe", m_sPath, NULL, SW_SHOW);
		}
	}
	else {
		CMessagePopup popup2(L"연동데이터 생성 실패", L"\n\n연동데이터 생성 실패", MB_OK, this);
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
	CString str = _T("Excel 파일(*.xlsx)|*.xlsx|");
	CFileDialog dlg(false, _T("*.xlsx"), _T("연동데이터.xlsx"), /*OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/NULL, str, this);
	if (dlg.DoModal() == IDOK) {
		m_sExcelPath = dlg.GetPathName();
		// 파일 경로를 가져와 사용할 경우, Edit Control에 값 저장
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
	CCommonDisplay::DrawCaption(&memDC, L"연동데이터 생성", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);

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

	//20230419 GBM start - 경로, 폴더 / 연동데이터 생성 버튼, 프로그래스바 화면 미갱신 오류 수정
	m_btnFolder.Invalidate();
	m_btnMakeData.Invalidate();
	m_editPath.Invalidate();
	m_prgCreate.Invalidate();
	//20230419 GBM end
}