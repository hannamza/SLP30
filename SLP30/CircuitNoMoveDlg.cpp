// CircuitNoMove.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CircuitNoMoveDlg.h"
#include "afxdialogex.h"


// CCircuitNoMove ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCircuitNoMoveDlg, CDialogEx)

CCircuitNoMoveDlg::CCircuitNoMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CIRCUIT_NO_CHANGE, pParent)
{
	m_nNewCircuitNum = -1;
}

CCircuitNoMoveDlg::~CCircuitNoMoveDlg()
{
}

void CCircuitNoMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CIRCUIT_NO, m_ctrlEditNewCircuitNo);
}


BEGIN_MESSAGE_MAP(CCircuitNoMoveDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON10, OnConfirmClick)
	ON_COMMAND(IDC_COMMON_BUTTON11, OnCancelClick)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCircuitNoMove �޽��� ó�����Դϴ�.


BOOL CCircuitNoMoveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	this->SetWindowPos(NULL, 0, 0, 420, 380, SWP_NOMOVE);

	m_ctrlEditNewCircuitNo.MoveWindow(115, 118, 194, 30);

	m_btnConfirm.Create(IDB_BMP_CONFIRM, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON10);
	m_btnCancel.Create(IDB_BMP_CANCEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON11);

	m_btnConfirm.MoveWindow(67, 236, 132, 44);
	m_btnCancel.MoveWindow(218, 236, 132, 44);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CCircuitNoMoveDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();

	m_btnConfirm.Invalidate();
	m_btnCancel.Invalidate();
}


HBRUSH CCircuitNoMoveDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

void CCircuitNoMoveDlg::Redisplay()
{
	CRect rect;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();

	CCommonDisplay::DrawImageGdiplusPNG(_pDC, IDB_PNG_CIRCUIT_NO_CHANGE, 0, 0);

	ReleaseDC(_pDC);
}

void CCircuitNoMoveDlg::OnConfirmClick()
{
	GetDlgItemText(IDC_EDIT_CIRCUIT_NO, m_strNewCircuitNo);

	OnOK();
}

void CCircuitNoMoveDlg::OnCancelClick()
{
	OnCancel();
}