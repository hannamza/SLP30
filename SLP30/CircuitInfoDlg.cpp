// CircuitInfoDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CircuitInfoDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CCircuitInfoDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCircuitInfoDlg, CDialogEx)

CCircuitInfoDlg::CCircuitInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_pTabButton[0] = NULL;
	m_pTabButton[1] = NULL;

	m_pSetupDlg[0] = NULL;
	m_pSetupDlg[1] = NULL;

	m_bInit = false;
}

CCircuitInfoDlg::~CCircuitInfoDlg()
{
	SAFE_DELETE(m_pTabButton[0]);
	SAFE_DELETE(m_pTabButton[1]);

	SAFE_DELETE(m_pSetupDlg[0]);
	SAFE_DELETE(m_pSetupDlg[1]);
}

void CCircuitInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircuitInfoDlg, CDialogEx)
	ON_MESSAGE(TEXTTAB_BUTTON_CLICK, OnTabButtnClick)
	ON_MESSAGE(MAKE_CIRCUIT_INFO, OnResetValue)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnNextClick)
	ON_COMMAND(IDC_COMMON_BUTTON2, OnPrevClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCircuitInfoDlg �޽��� ó�����Դϴ�.

BOOL CCircuitInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_pTabButton[0] = new CTextTabBtn(0, L"0 ����", this);
	m_pTabButton[0]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[0]->MoveWindow(20, 14, 89, 37);

	m_pTabButton[0]->SetCheck(true);

	m_pTabButton[1] = new CTextTabBtn(1, L"1 ����", this);
	m_pTabButton[1]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[1]->MoveWindow(115, 14, 89, 37);

	m_pSetupDlg[0] = new CCircuitSetupDlg(this);
	m_pSetupDlg[0]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pSetupDlg[0]->ShowWindow(SW_SHOW);

	m_pSetupDlg[1] = new CCircuitSetupDlg(this);
	m_pSetupDlg[1]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pSetupDlg[1]->ShowWindow(SW_HIDE);

	m_btnNext.Create(IDB_BMP_NEXT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnPrev.Create(IDB_BMP_PREV, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);

	m_bInit = true;

	//for (int nIndex = 0; nIndex < 6; nIndex++) {
	//	CRuntimeClass	*pRuntimeClass;
	//	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomListCtrl))) {
	//		if (NULL != (m_pListCtrl[nIndex] = (CCustomListCtrl *)(pRuntimeClass->CreateObject()))) {
	//			m_pListCtrl[nIndex]->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
	//		}
	//	}
	//	m_pListCtrl[nIndex]->SetListIndex(0);
	//	m_pListCtrl[nIndex]->SetHeaderInfo((TCHAR**)lpszHeaderBasic, (int*)colWidthListBasic, (int*)colTypeBasic, (int*)colAlignBasic); // Align: option, row align text, default: DT_LEFT
	//	m_pListCtrl[nIndex]->SetColumnLimit((int*)colLimitBasic);
	//	// ���� ���� (option)
	//	m_pListCtrl[nIndex]->SetHeaderHeight(40);
	//	m_pListCtrl[nIndex]->SetRowHeight(30);
	//	//m_pListCtrl[nIndex]->SetListPopupSize(500, 500);
	//	// ������ (option)
	//	m_pListCtrl[nIndex]->SetHeaderBkColor(RGB(80, 80, 80), RGB(255, 255, 255));
	//	m_pListCtrl[nIndex]->SetHeaderTextColor(RGB(220, 220, 220));
	//	m_pListCtrl[nIndex]->SetRowBkColor(RGB(160, 160, 160), RGB(255, 255, 0), RGB(255, 255, 255));
	//	m_pListCtrl[nIndex]->SetRowTextColor(RGB(255, 255, 255), RGB(80, 80, 80));
	//	m_pListCtrl[nIndex]->SetHeaderFontSize(15);
	//	m_pListCtrl[nIndex]->FixHeaderSize(false);

	//	m_pListCtrl[nIndex]->SendMessage(WM_INITIALUPDATE);
	//}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CCircuitInfoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CCircuitInfoDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CCircuitInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCircuitInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircuitInfoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircuitInfoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCircuitInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();

	m_pTabButton[0]->Invalidate();
	m_pTabButton[1]->Invalidate();

	m_pSetupDlg[0]->Invalidate();
	m_pSetupDlg[1]->Invalidate();
}

void CCircuitInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Redisplay();
}

void CCircuitInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CCircuitInfoDlg::OnResetValue(WPARAM wParam, LPARAM lParam)
{
	Sleep(100);
	CCommonState::ie()->m_bWaitCircuit = false;
	return 0;
}

void CCircuitInfoDlg::InitCircuitInfo()
{
	//20221013 GBM start - ���� ������ �Ѱܼ� ���� ���� ���� ������ �������� ���� �߰��� ������ ����� 0���� �ʱ�ȭ
#if 1
	m_pSetupDlg[0]->InitCircuitInfo(0);
	m_pSetupDlg[1]->InitCircuitInfo(1);
#else
	m_pSetupDlg[0]->InitCircuitInfo();
	m_pSetupDlg[1]->InitCircuitInfo();
#endif
	//20221013 GBM end
	m_pSetupDlg[0]->ShowWindow(SW_SHOW);
	m_pSetupDlg[1]->ShowWindow(SW_HIDE);
	PostMessage(TEXTTAB_BUTTON_CLICK, 0, 0);
	PostMessage(MAKE_CIRCUIT_INFO, 0, 0);
}

int CCircuitInfoDlg::GetCircuitCount(int nChartIndex)
{
	pSelectCircuit pCircuit = NULL;
	int nTotal = 0;
	int nCount = 0;
	if (nChartIndex == 0) {
		nCount = CCommonState::ie()->m_selectCircuit_0.GetCount();
	}
	else if(nChartIndex == 1) {
		nCount = CCommonState::ie()->m_selectCircuit_1.GetCount();
	}
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		if (nChartIndex == 0) {
			pCircuit = CCommonState::ie()->m_selectCircuit_0.GetAt(CCommonState::ie()->m_selectCircuit_0.FindIndex(nIndex));
		}
		else if (nChartIndex == 1) {
			pCircuit = CCommonState::ie()->m_selectCircuit_1.GetAt(CCommonState::ie()->m_selectCircuit_1.FindIndex(nIndex));
		}
		nTotal += pCircuit->nCount;
	}
	return nTotal;
}

bool CCircuitInfoDlg::CheckCircuitCount()
{
	if (!m_pSetupDlg[0] || !m_pSetupDlg[1]) {
		return false;
	}
	bool bResult1 = m_pSetupDlg[0]->SaveCircuitInfo(0);
	bool bResult2 = m_pSetupDlg[1]->SaveCircuitInfo(1);
	/*if (!bResult1 || !bResult2) {
		CString sText;
		sText.Format(L"\n\n%d���� �ִ� �Է� ����(250��)�� �ʰ��Ͽ����ϴ�.\n\n�ٽ� �Է��Ͽ� �ֽñ� �ٶ��ϴ�.", (!bResult1) ? 0 : 1);
		CMessagePopup popup(L"�߰�� �϶�ǥ", sText, MB_OK, this);
		UINT nResult = popup.DoModal();
		CCommonState::ie()->ReleaseCircuit(0);
		CCommonState::ie()->ReleaseCircuit(1);
		return false;
	}*/

	/*int nCount1 = GetCircuitCount(0);
	int nCount2 = GetCircuitCount(1);
	if (nCount1 > MAX_CIRCUIT || nCount2 > MAX_CIRCUIT) {
		CString sText;
		sText.Format(L"\n\n%d���� �ִ� �Է� ����(250��)�� �ʰ��Ͽ����ϴ�.\n\n�ٽ� �Է��Ͽ� �ֽñ� �ٶ��ϴ�.", (nCount1 > 250) ? 0 : 1);
		CMessagePopup popup(L"�߰�� �϶�ǥ", sText, MB_OK, this);
		UINT nResult = popup.DoModal();
		CCommonState::ie()->ReleaseCircuit(0);
		CCommonState::ie()->ReleaseCircuit(1);
		return false;
	}*/

	return true;
}

void CCircuitInfoDlg::LoadInfo()
{
	m_pSetupDlg[0]->InitCircuitInfo(0);
	m_pSetupDlg[1]->InitCircuitInfo(1);

	m_pSetupDlg[0]->LoadInfo(0);
	m_pSetupDlg[1]->LoadInfo(1);

	CCommonState::ie()->m_bInitCircuit = false;
}

void CCircuitInfoDlg::OnNextClick()
{
	bool bResult1 = m_pSetupDlg[0]->CompareNewCircuitInfo(0);
	bool bResult2 = m_pSetupDlg[1]->CompareNewCircuitInfo(1);
	if (!bResult1 || !bResult2) {
		//20221011 GBM start
		if (!CCommonState::ie()->m_bInitCircuit) {
			CMessagePopup popup(L"�߰�� �϶�ǥ", L"\n����������� ���Ͽ�\n\n�߰�� �϶�ǥ�� �ʱ�ȭ �˴ϴ�.\n\n����Ͻðڽ��ϱ�?\n\n(Ȯ��: ���۾� / ���: �����۾�)", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {

				if (!CheckCircuitCount()) {
					return;
				}

				//20221014 GBM start - ���� ���� ������ ����صд�.
				m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0);
				m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1);
				//20221014 GBM end

				GetParent()->PostMessage(SELECTION_PROJECT, 12, 0);
			}
			else {
				GetParent()->PostMessage(SELECTION_PROJECT, 13, 0);
			}
			//CMessagePopup popup(L"�߰�� �϶�ǥ", L"\n��������� �����մϴ�.\n���� �����Ͻðڽ��ϱ�?\n\n(Ȯ��: ������, ���: �������� ����)", MB_YESNO, this);
			//UINT nResult = popup.DoModal();
			//if (nResult == IDOK) {
			//	if (!CheckCircuitCount()) {
			//		return;
			//	}
			//	GetParent()->PostMessage(SELECTION_PROJECT, 21, 0);
			//}
			//else {
			//	GetParent()->PostMessage(SELECTION_PROJECT, 22, 0);
			//}
			//20221011 GBM end
		}
		else {
			if (!CheckCircuitCount()) {
				return;
			}

			//20221014 GBM start - ���� ���� ������ ����صд�.
			m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0);
			m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1);
			//20221014 GBM end

			GetParent()->PostMessage(SELECTION_PROJECT, 12, 0);
		}
	}
	else {
		GetParent()->PostMessage(SELECTION_PROJECT, 13, 0);
	}
}

void CCircuitInfoDlg::OnPrevClick()
{

	//20221014 GBM start - ���� ȸ�� ���� �۾� ���� �޸𸮿� ����
	m_pSetupDlg[0]->SaveCircuitInfo(0);
	m_pSetupDlg[1]->SaveCircuitInfo(1);
	//20221014 GBM end

	//20221014 GBM start - ���� ���� ������ ����صд�.
	m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0);
	m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1);
	//20221014 GBM end

	GetParent()->PostMessage(SELECTION_PROJECT, 0, 0);
}

LRESULT CCircuitInfoDlg::OnTabButtnClick(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)wParam;
	switch (nIndex) {
	case 0:
		m_pTabButton[0]->SetCheck(true);
		m_pTabButton[1]->SetCheck(false);
		m_pSetupDlg[0]->ShowWindow(SW_SHOW);
		m_pSetupDlg[1]->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pTabButton[1]->SetCheck(true);
		m_pTabButton[0]->SetCheck(false);
		m_pSetupDlg[1]->ShowWindow(SW_SHOW);
		m_pSetupDlg[0]->ShowWindow(SW_HIDE);
		break;
	default: break;
	}
	return 0;
}

void CCircuitInfoDlg::Redisplay()
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
	rt.CopyRect(&rect);
	rt.left += 4;
	rt.right -= 4;
	rt.top += 50;
	rt.bottom -= 7;
	CCommonDisplay::DrawRect(&memDC, false, RGB(220, 220, 220), 0, rt);

	if (m_pSetupDlg[0] && m_pSetupDlg[1]) {
		rt.left += 5;
		rt.right -= 5;
		rt.bottom -= 5;
		rt.top += 5;
		m_pSetupDlg[0]->MoveWindow(rt);
		m_pSetupDlg[1]->MoveWindow(rt);
	}

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInit) {
		m_btnNext.MoveWindow(rect.right - 87, 0, 83, 48);
		m_btnPrev.MoveWindow(rect.right - 180, 0, 83, 48);
		m_btnNext.Invalidate();
		m_btnPrev.Invalidate();
	}
}