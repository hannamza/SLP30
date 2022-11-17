// SecurityUsbDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30_Manager.h"
#include "SecurityUsbDlg.h"
#include "afxdialogex.h"
#include <dbt.h>

// CSecurityUsbDlg ��ȭ �����Դϴ�.

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
		_T("����")); // lpszFacename 
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

// CSecurityUsbDlg �޽��� ó�����Դϴ�.

BOOL CSecurityUsbDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CSecurityUsbDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CSecurityUsbDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CSecurityUsbDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CSecurityUsbDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CSecurityUsbDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CSecurityUsbDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CSecurityUsbDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();

	m_comboUsb.Invalidate();
	m_btnConfirm.Invalidate();
	m_btnCancel.Invalidate();
}

void CSecurityUsbDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CSecurityUsbDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CSecurityUsbDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	switch (nEventType)
	{
	case DBT_DEVICEARRIVAL:
		TRACE("DBT_DEVICEARRIVAL\n");
		//��ġ ���� ���� �� ó��
		break;
	case DBT_DEVICEREMOVECOMPLETE:
		TRACE("DBT_DEVICEREMOVECOMPLETE\n");
		//��ġ ���� ���� ���� �� ó��
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