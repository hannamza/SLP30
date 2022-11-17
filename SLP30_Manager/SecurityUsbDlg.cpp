// SecurityUsbDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30_Manager.h"
#include "SecurityUsbDlg.h"
#include "afxdialogex.h"
#include <dbt.h>

// CSecurityUsbDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSecurityUsbDlg, CDialogEx)

CSecurityUsbDlg::CSecurityUsbDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SECURITY, pParent)
{
	m_font.CreateFont(18, // nHeight 
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
		_T("굴림")); // lpszFacename 
}

CSecurityUsbDlg::~CSecurityUsbDlg()
{
}

void CSecurityUsbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DRIVE, m_comboUsb);
}

BEGIN_MESSAGE_MAP(CSecurityUsbDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnConfirmClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnCancelClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSecurityUsbDlg 메시지 처리기입니다.

BOOL CSecurityUsbDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	this->SetWindowPos(NULL, 0, 0, 420, 380, SWP_NOMOVE);
	m_comboUsb.SetFont(&m_font);
	m_comboUsb.ModifyStyle(WS_BORDER, 0);
	m_comboUsb.MoveWindow(150, 173, 188, 400);
	CUSBDriveManger::Instance()->GetUSBList();
	for (int nIndex = 0; nIndex < CUSBDriveManger::Instance()->m_list.GetCount(); nIndex++) {
		pUsbList pUsb = CUSBDriveManger::Instance()->m_list.GetAt(CUSBDriveManger::Instance()->m_list.FindIndex(nIndex));
		m_comboUsb.AddString(pUsb->sDriveLetter);
	}
	if (m_comboUsb.GetCount() > 0) {
		m_comboUsb.SetCurSel(0);
	}

	m_btnConfirm.Create(IDB_BMP_CONFIRM, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnCancel.Create(IDB_BMP_CANCEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnConfirm.MoveWindow(67, 276, 132, 44);
	m_btnCancel.MoveWindow(218, 276, 132, 44);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSecurityUsbDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CSecurityUsbDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CSecurityUsbDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CSecurityUsbDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CSecurityUsbDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CSecurityUsbDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CSecurityUsbDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	m_comboUsb.Invalidate();
	m_btnConfirm.Invalidate();
	m_btnCancel.Invalidate();
}

void CSecurityUsbDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CSecurityUsbDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CSecurityUsbDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	switch (nEventType)
	{
	case DBT_DEVICEARRIVAL:
		TRACE("DBT_DEVICEARRIVAL\n");
		//장치 연결 됬을 때 처리
		break;
	case DBT_DEVICEREMOVECOMPLETE:
		TRACE("DBT_DEVICEREMOVECOMPLETE\n");
		//장치 연결 해제 됬을 때 처리
		break;
	case DBT_DEVNODES_CHANGED:
		TRACE("DBT_DEVNODES_CHANGED\n");
		{
			CUSBDriveManger::Instance()->GetUSBList();
			while (m_comboUsb.GetCount() > 0) {
				m_comboUsb.DeleteString(0);
			}
			for (int nIndex = 0; nIndex < CUSBDriveManger::Instance()->m_list.GetCount(); nIndex++) {
				pUsbList pUsb = CUSBDriveManger::Instance()->m_list.GetAt(CUSBDriveManger::Instance()->m_list.FindIndex(nIndex));
				m_comboUsb.AddString(pUsb->sDriveLetter);
			}
			if (m_comboUsb.GetCount() > 0) {
				m_comboUsb.SetCurSel(0);
			}
			m_comboUsb.Invalidate();
		}
		break;
	default:
		break;
	}

	return 0;
}

void CSecurityUsbDlg::OnConfirmClick()
{
	m_comboUsb.GetWindowText(m_sDriveLetter);

	OnOK();
}

void CSecurityUsbDlg::OnCancelClick()
{
	OnCancel();
}

CString CSecurityUsbDlg::GetDriveLetter()
{
	return m_sDriveLetter;
}

void CSecurityUsbDlg::Redisplay()
{
	CRect rect, rt;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CCommonDisplay::DrawImageGdiplusPNG(_pDC, IDB_PNG_USB, 0, 0);

	ReleaseDC(_pDC);
}