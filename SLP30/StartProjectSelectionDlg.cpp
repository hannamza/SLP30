// StartProjectSelectionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "StartProjectSelectionDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CStartProjectSelectionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStartProjectSelectionDlg, CDialogEx)

CStartProjectSelectionDlg::CStartProjectSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_font.CreateFont(20, // nHeight 
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
	m_font1.CreateFont(16, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
#ifndef ENGLISH_MODE
		_T("돋움")); // lpszFacename 
#else
		_T("arial")); // lpszFacename 
#endif

	m_bInit = false;
	m_bFirst = true;
}

CStartProjectSelectionDlg::~CStartProjectSelectionDlg()
{
}

void CStartProjectSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStartProjectSelectionDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnNewProjectClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnLoadProjectClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CStartProjectSelectionDlg 메시지 처리기입니다.

BOOL CStartProjectSelectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_btnNew.Create(IDB_BMP_NEWBTN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnLoad.Create(IDB_BMP_LOADBTN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CStartProjectSelectionDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CStartProjectSelectionDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CStartProjectSelectionDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CStartProjectSelectionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CStartProjectSelectionDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CStartProjectSelectionDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CStartProjectSelectionDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CStartProjectSelectionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Redisplay();
}

void CStartProjectSelectionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CStartProjectSelectionDlg::OnNewProjectClick()
{
	if (!m_bFirst) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"신규 프로젝트 생성", L"\n\n\n새로운 작업을 시작합니까?", MB_YESNO, this);
#else
		CMessagePopup popup(L"Create a New Project", L"\n\n\nDo you want to start a new Project?", MB_YESNO, this);
#endif
		UINT nResult = popup.DoModal();
		if (nResult == IDOK) {
			GetParent()->PostMessage(SELECTION_PROJECT, 0, 1);
		}
	}
	else {
		GetParent()->PostMessage(SELECTION_PROJECT, 0, 1);
	}
	m_bFirst = false;
	CCommonState::ie()->m_bInitCircuit = true;
}

void CStartProjectSelectionDlg::OnLoadProjectClick()
{
#ifndef ENGLISH_MODE
	CString str = _T("SLP 파일(*.slp)|*.slp|");
	CFileDialog dlg(true, _T("*.slp"), _T("편집정보.slp"), /*OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/NULL, str, this);
#else
	CString str = _T("SLP File(*.slp)|*.slp|");
	CFileDialog dlg(true, _T("*.slp"), _T("Edit Info.slp"), /*OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/NULL, str, this);
#endif
	CString strPathName;
	if (dlg.DoModal() == IDOK) {
		strPathName = dlg.GetPathName();
	}
	else {
		return;
	}

	if (CSaveManager::ie()->FileLoad(strPathName)) {
		GetParent()->PostMessage(SELECTION_PROJECT, 10, 0);
	}
	else {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"파일 열기 실패", L"\n\n파일 열기에 실패함\n\n다시 시도해 주세요.", MB_OK, this);
#else
		CMessagePopup popup(L"Failed to open File", L"\n\n\nFailed to open File\n\nPlease try again.", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}

	return;

	/*BYTE g_pbszUK[] = { 0x27, 0x6d, 0x66, 0x47, 0x35, 0xe8, 0x1d, 0x6a, 0x6f, 0x1a, 0x39, 0x7a, 0x36, 0x16, 0x21, 0x1a, 0x00 };
	BYTE g_pbszIV[] = { 0xa1, 0xd1, 0x23, 0x2a, 0xc1, 0xc7, 0xa2, 0x28, 0x89, 0xe1, 0x6c, 0xb5, 0x78, 0x01, 0x41, 0xd1, 0x00 };
	BYTE g_pbszIV0[] = { 0x11, 0xa6, 0x33, 0xa6, 0xa1, 0x07, 0x12, 0xdd, 0x54, 0x9a, 0x2c, 0xe3, 0x38, 0x2a, 0x77, 0xbb, 0x00 };
	BYTE g_pbszIV1[] = { 0x11, 0xd6, 0x33, 0xb6, 0xa1, 0x07, 0xb2, 0xdd, 0x54, 0x6b, 0x21, 0xa3, 0x38, 0x4a, 0x32, 0xe0, 0x00 };
	BYTE g_pbszIV2[] = { 0xc3, 0x27, 0x32, 0x16, 0x9c, 0x7a, 0x12, 0xcc, 0xa4, 0x1c, 0x22, 0x13, 0xb3, 0x2c, 0xe8, 0x0e, 0x00 };
	BYTE g_pbszIV3[] = { 0xd2, 0x18, 0x61, 0x36, 0x6a, 0x43, 0xa2, 0xbb, 0x40, 0x2d, 0x23, 0x23, 0xbb, 0xaa, 0x8e, 0x78, 0x00 };

	uploadInfo info;
	wcscpy_s(info.szLocalPath, L"D:\\제목 없음.png");
	wcscpy_s(info.szDate, L"2월25일17시12분");
	wcscpy_s(info.szFileName, L"제목 없음.png");
	wcscpy_s(info.szFolder, L"의정부자이");
	wcscpy_s(info.szUserID, L"gfs_test");
	wcscpy_s(info.szYear, L"2022");

	BYTE value[64];
	memset(value, 0x00, 64);
	int nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV0, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
	nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV1, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
	nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV2, g_pbszIV, 16, value);
	memcpy_s(g_pbszIV, 16, value, 16);
	g_pbszIV[4] = '0x45';

	BYTE szUploadInfo[8192];
	memset(szUploadInfo, 0x00, 8192);
	DWORD dwReadden = 0;
	nValue = SEED_CBC_Encrypt(g_pbszUK, g_pbszIV, (BYTE*)&info, sizeof(uploadInfo), szUploadInfo);

	if (!CSharedMemoryManager::ie()->WriteSharedMemory(szUploadInfo, nValue)) {
		CMessagePopup popup(L"전송 실패", L"\n\n서버 전송에 실패하였습니다.", MB_OK, this);
		popup.DoModal();
		return;
	}

	WCHAR szPath[2048], szArg[16];
	GetModuleFileName(NULL, szPath, 2048);
	PathRemoveFileSpec(szPath);
	wcscat(szPath, L"\\WebDavClient.exe");
	swprintf_s(szArg, L"%d", nValue);

	CCommonFunc::ShellExecuteProgram(szPath, szArg, SW_HIDE);*/
}

void CStartProjectSelectionDlg::Redisplay()
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
	CCommonDisplay::DrawRect(&memDC, true , RGB(255, 255, 255), RGB(255, 255, 255), rt);

#ifndef ENGLISH_MODE
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_NEWPROJECT, (rt.Width() / 4) - 123, rt.top + 100);
	CCommonDisplay::DrawLine(&memDC, rt.Width() / 4 - 163, rt.top + 158, rt.Width() / 4 + 163, rt.top + 158, PS_DOT, 1, RGB(112, 0, 14));
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_LOADPROJECT, (rt.Width() / 4 * 3) - 140, rt.top + 100);
	CCommonDisplay::DrawLine(&memDC, (rt.Width() / 4 * 3) - 180, rt.top + 158, (rt.Width() / 4 * 3) + 180, rt.top + 158, PS_DOT, 1, RGB(112, 0, 14));

	CCommonDisplay::DrawImage(&memDC, IDB_BMP_NEWEX, (rt.Width() / 4) - 80, rt.bottom - 100);
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_LOADEX, (rt.Width() / 4) * 3 - 85, rt.bottom - 100);
	CCommonDisplay::DrawLine(&memDC, rt.Width() / 4 - 163, rt.top + 158, rt.Width() / 4 + 163, rt.top + 158, PS_DOT, 1, RGB(112, 0, 14));
	CCommonDisplay::DrawLine(&memDC, (rt.Width() / 4 * 3) - 180, rt.top + 158, (rt.Width() / 4 * 3) + 180, rt.top + 158, PS_DOT, 1, RGB(112, 0, 14));
#else
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_NEWPROJECT_EN, (rt.Width() / 4) - 123, rt.top + 100);
	CCommonDisplay::DrawLine(&memDC, rt.Width() / 4 - 163, rt.top + 158, rt.Width() / 4 + 163, rt.top + 158, PS_DOT, 1, RGB(112, 0, 14));
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_LOADPROJECT_EN, (rt.Width() / 4 * 3) - 140, rt.top + 100);
	CCommonDisplay::DrawLine(&memDC, (rt.Width() / 4 * 3) - 180, rt.top + 158, (rt.Width() / 4 * 3) + 180, rt.top + 158, PS_DOT, 1, RGB(112, 0, 14));

	CCommonDisplay::DrawImage(&memDC, IDB_BMP_NEWEX_EN, (rt.Width() / 4) - 80, rt.bottom - 100);
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_LOADEX_EN, (rt.Width() / 4) * 3 - 85, rt.bottom - 100);
	CCommonDisplay::DrawLine(&memDC, rt.Width() / 4 - 163, rt.top + 158, rt.Width() / 4 + 163, rt.top + 158, PS_DOT, 1, RGB(112, 0, 14));
	CCommonDisplay::DrawLine(&memDC, (rt.Width() / 4 * 3) - 180, rt.top + 158, (rt.Width() / 4 * 3) + 180, rt.top + 158, PS_DOT, 1, RGB(112, 0, 14));
#endif

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInit) {
		m_btnNew.MoveWindow((rt.Width() / 4) - 125, rt.Height() / 2 - 125, 250, 250);
		m_btnLoad.MoveWindow((rt.Width() / 4 * 3) - 125, rt.Height() / 2 - 125, 250, 250);

		m_btnNew.Invalidate();
		m_btnLoad.Invalidate();
	}
}