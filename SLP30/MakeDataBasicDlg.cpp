// MakeDataBasicDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "MakeDataBasicDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CMakeDataBasicDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMakeDataBasicDlg, CDialogEx)

CMakeDataBasicDlg::CMakeDataBasicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_pMakeData = NULL;
	m_pSetupData = NULL;

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

	m_nDisplay = 0;
	m_bInit = false;
}

CMakeDataBasicDlg::~CMakeDataBasicDlg()
{
	SAFE_DELETE(m_pMakeData);
	SAFE_DELETE(m_pSetupData);
}

void CMakeDataBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMakeDataBasicDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnNextClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnPrevClick)
	ON_COMMAND(IDC_COMMON_BUTTON3, OnSaveClick)
	ON_MESSAGE(SELECTION_PROJECT, OnChangePage)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMakeDataBasicDlg 메시지 처리기입니다.

BOOL CMakeDataBasicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

#ifndef ENGLISH_MODE
	m_btnNext.Create(IDB_BMP_NEXT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnPrev.Create(IDB_BMP_PREV, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnSave.Create(IDB_BMP_SAVE, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
#else
	m_btnNext.Create(IDB_BMP_NEXT_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnPrev.Create(IDB_BMP_PREV_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnSave.Create(IDB_BMP_SAVE_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
#endif

	m_pMakeData = new CMakeDataDlg(this);
	m_pMakeData->Create(IDD_DIALOG_DATA, this);
	m_pMakeData->ShowWindow(SW_HIDE);

	m_pSetupData = new CSetupMakeDataDlg(this);
	m_pSetupData->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pSetupData->ShowWindow(SW_SHOW);

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMakeDataBasicDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CMakeDataBasicDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CMakeDataBasicDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CMakeDataBasicDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMakeDataBasicDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMakeDataBasicDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CMakeDataBasicDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CMakeDataBasicDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_bInit) {
		Redisplay();
	}
}

void CMakeDataBasicDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CMakeDataBasicDlg::OnChangePage(WPARAM wParam, LPARAM lParam)
{
	int nPage = (int)wParam;
	switch (nPage) {
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}
	return 0;
}

void CMakeDataBasicDlg::CallFirstDisplay()
{
	m_nDisplay = 0;
	m_pMakeData->ShowWindow(SW_HIDE);
	m_pSetupData->ShowWindow(SW_SHOW);
	m_pSetupData->InitData();
}

void CMakeDataBasicDlg::OnNextClick()
{
	m_pSetupData->SaveData();

	m_pMakeData->ShowWindow(SW_SHOW);
	m_pSetupData->ShowWindow(SW_HIDE);
	m_nDisplay = 1;
}

void CMakeDataBasicDlg::OnPrevClick()
{
	if (m_nDisplay == 1) {
		m_nDisplay = 0;
		m_pMakeData->ShowWindow(SW_HIDE);
		m_pSetupData->ShowWindow(SW_SHOW);
		m_pSetupData->InitData();
	}
	else {
		m_nDisplay = 0;
		GetParent()->PostMessage(SELECTION_PROJECT, 13, 0);
	}
}

void CMakeDataBasicDlg::OnSaveClick()
{
	m_pSetupData->SaveData();

#ifndef ENGLISH_MODE
	CString str = _T("SLP 파일(*.slp)|*.slp|");
	CFileDialog dlg(false, _T("*.slp"), _T("편집정보.slp"), /*OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/NULL, str, this);
#else
	CString str = _T("SLP File(*.slp)|*.slp|");
	CFileDialog dlg(false, _T("*.slp"), _T("Edit Info.slp"), /*OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/NULL, str, this);
#endif

	CString strPathName;
	if (dlg.DoModal() == IDOK) {
		strPathName = dlg.GetPathName();
		// 파일 경로를 가져와 사용할 경우, Edit Control에 값 저장
	}
	else {
		return;
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
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"편집정보 저장", L"\n\n\n저장이 완료됨", MB_OK, this);
#else
		CMessagePopup popup(L"Save Edit Info", L"\n\n\nSaved", MB_OK, this);
#endif
		popup.DoModal();
	}
}

void CMakeDataBasicDlg::Redisplay()
{
	CRect rect, rt, rtemp;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawRect(&memDC, true, RGB(255, 255, 255), RGB(255, 255, 255), rect);

	rt.SetRect(20, 15, 350, 50);
#ifndef ENGLISH_MODE
	CCommonDisplay::DrawCaption(&memDC, L"연동 설정", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
#else
	CCommonDisplay::DrawCaption(&memDC, L"Site Logic Data Settings", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
#endif
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

	if (m_pMakeData && m_pSetupData) {
		rt.left += 5;
		rt.right -= 5;
		rt.bottom -= 5;
		rt.top += 5;
		m_pMakeData->MoveWindow(rt);
		m_pSetupData->MoveWindow(rt);

		if (m_nDisplay == 0) {
			m_btnNext.ShowWindow(SW_SHOW);
			m_btnSave.ShowWindow(SW_SHOW);
			m_btnNext.MoveWindow(rect.right - 87, 0, 83, 48);
			m_btnPrev.MoveWindow(rect.right - 180, 0, 83, 48);
			m_btnSave.MoveWindow(rect.right - 275, 0, 83, 48);
		}
		else {
			m_btnNext.ShowWindow(SW_HIDE);
			m_btnSave.ShowWindow(SW_HIDE);
			m_btnPrev.MoveWindow(rect.right - 87, 0, 83, 48);
		}

		m_btnNext.Invalidate();
		m_btnPrev.Invalidate();
	}
}