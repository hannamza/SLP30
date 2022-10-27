// CircuitNoMove.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CircuitNoMoveDlg.h"
#include "afxdialogex.h"


// CCircuitNoMove 대화 상자입니다.

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


// CCircuitNoMove 메시지 처리기입니다.


BOOL CCircuitNoMoveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	this->SetWindowPos(NULL, 0, 0, 420, 380, SWP_NOMOVE);

	m_ctrlEditNewCircuitNo.MoveWindow(115, 118, 194, 30);

	m_btnConfirm.Create(IDB_BMP_CONFIRM, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON10);
	m_btnCancel.Create(IDB_BMP_CANCEL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON11);

	m_btnConfirm.MoveWindow(67, 236, 132, 44);
	m_btnCancel.MoveWindow(218, 236, 132, 44);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCircuitNoMoveDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	m_btnConfirm.Invalidate();
	m_btnCancel.Invalidate();
}


HBRUSH CCircuitNoMoveDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
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