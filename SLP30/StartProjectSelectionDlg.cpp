// StartProjectSelectionDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "StartProjectSelectionDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CStartProjectSelectionDlg ��ȭ �����Դϴ�.

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
		_T("����")); // lpszFacename 
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
		_T("����")); // lpszFacename 
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

// CStartProjectSelectionDlg �޽��� ó�����Դϴ�.

BOOL CStartProjectSelectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_btnNew.Create(IDB_BMP_NEWBTN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnLoad.Create(IDB_BMP_LOADBTN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CStartProjectSelectionDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CStartProjectSelectionDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CStartProjectSelectionDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CStartProjectSelectionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CStartProjectSelectionDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CStartProjectSelectionDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CStartProjectSelectionDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();
}

void CStartProjectSelectionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Redisplay();
}

void CStartProjectSelectionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

void CStartProjectSelectionDlg::OnNewProjectClick()
{
	if (!m_bFirst) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ű� ������Ʈ ����", L"\n\n\n���ο� �۾��� �����մϱ�?", MB_YESNO, this);
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
	CString str = _T("SLP ����(*.slp)|*.slp|");
	CFileDialog dlg(true, _T("*.slp"), _T("��������.slp"), /*OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/NULL, str, this);
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
		CMessagePopup popup(L"���� ���� ����", L"\n\n���� ���⿡ ������\n\n�ٽ� �õ��� �ּ���.", MB_OK, this);
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
	wcscpy_s(info.szLocalPath, L"D:\\���� ����.png");
	wcscpy_s(info.szDate, L"2��25��17��12��");
	wcscpy_s(info.szFileName, L"���� ����.png");
	wcscpy_s(info.szFolder, L"����������");
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
		CMessagePopup popup(L"���� ����", L"\n\n���� ���ۿ� �����Ͽ����ϴ�.", MB_OK, this);
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