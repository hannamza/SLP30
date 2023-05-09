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
	ON_COMMAND(IDC_COMMON_BUTTON3, OnCircuitListInit)
END_MESSAGE_MAP()

// CCircuitInfoDlg �޽��� ó�����Դϴ�.

BOOL CCircuitInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
#ifndef ENGLISH_MODE
	m_pTabButton[0] = new CTextTabBtn(0, L"0 ����", this);
#else
	m_pTabButton[0] = new CTextTabBtn(0, L"LOOP 0", this);
#endif
	m_pTabButton[0]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[0]->MoveWindow(20, 14, 89, 37);

	m_pTabButton[0]->SetCheck(true);

#ifndef ENGLISH_MODE
	m_pTabButton[1] = new CTextTabBtn(1, L"1 ����", this);
#else
	m_pTabButton[1] = new CTextTabBtn(1, L"LOOP 1", this);
#endif
	m_pTabButton[1]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pTabButton[1]->MoveWindow(115, 14, 89, 37);

	m_pSetupDlg[0] = new CCircuitSetupDlg(this);
	m_pSetupDlg[0]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pSetupDlg[0]->ShowWindow(SW_SHOW);

	m_pSetupDlg[1] = new CCircuitSetupDlg(this);
	m_pSetupDlg[1]->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pSetupDlg[1]->ShowWindow(SW_HIDE);

#ifndef ENGLISH_MODE
	m_btnNext.Create(IDB_BMP_NEXT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnPrev.Create(IDB_BMP_PREV, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnCircuitListInit.Create(IDB_BMP_CIRCUIT_LIST_INIT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
#else
	m_btnNext.Create(IDB_BMP_NEXT_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnPrev.Create(IDB_BMP_PREV_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON2);
	m_btnCircuitListInit.Create(IDB_BMP_CIRCUIT_LIST_INIT_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON3);
#endif

	m_btnCircuitListInit.ShowWindow(FALSE);

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

	//20221122 GBM -  ���� ���� �� ����� ��쿡 �ѹ� ���� ���� ������ ���� �����صд�.
	m_pSetupDlg[0]->BackupCircuitInfo(0);
	m_pSetupDlg[1]->BackupCircuitInfo(1);

	//�ѹ� ��� ȸ�� ���� ����
	m_pSetupDlg[0]->BackupCircuitCountInfo(0);
	m_pSetupDlg[1]->BackupCircuitCountInfo(1);

	//20221024 GBM start - ȸ�� ���� ������ �ε�Ǿ����Ƿ� ���� �񱳸� ���� ���� �� �����Ϳ� �ִ´�.
	m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0, true);
	m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1, true);

	//���� �ε� �� �߰�� �϶�ǥ ���ش�� �߰�� �϶�ǥ ȸ�������� �ִ´�.
	CCommonState::ie()->InitSelectCircuitRepeaterListOnLoadFile(0);
	CCommonState::ie()->InitSelectCircuitRepeaterListOnLoadFile(1);

	//�߰�� �϶�ǥ�� �ϴ� Ȯ���Ǵ� ���� ��ü ȸ�� ������ ���ϰ� �߰��п� ���ؼ��� ���� �߰��ؼ� ���� ���� ȸ�� ������ �׻� �˰� �ֵ��� ��
	CCommonState::ie()->m_nTotalCountCircuit_0 = CCommonState::ie()->CalculateTotalCircuitCount(0);
	CCommonState::ie()->m_nTotalCountCircuit_1 = CCommonState::ie()->CalculateTotalCircuitCount(1);
	//20221024 GBM end

	CCommonState::ie()->m_bInitCircuit = false;
	m_btnCircuitListInit.ShowWindow(TRUE);
}

void CCircuitInfoDlg::OnNextClick()
{
	bool bCheckMaxCircuit1 = false;
	bool bCheckMaxCircuit2 = false;

	//20221018 GBM start - �߰�� �϶�ǥ�� ȸ�������� Ȯ���Ǿ� ó�� �ۼ��� �� ����ڿ��� ������ ���� ���� ������ �߰��� ��� ���� �߰�� �϶�ǥ �Ʒ��� �߰��ǰ� ������ ��� ���� ��Ұ� �������� ��ü���� �˷��� ��
	if (CCommonState::ie()->m_bInitCircuit)
	{
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�߰�� �϶�ǥ Ȯ��", L"\n���� ������ �����Ͽ� �߰�� �϶�ǥ Ȯ����\n\n�߰�� �϶�ǥ Ȯ�� ����\n���� �߰��� ��� ���� �߰�� �϶�ǥ �ϴܿ� �߰�,\n���� ������ ��� �߰�� �϶�ǥ���� ���� ���ŵ�\n\n����Ͻðڽ��ϱ�?", MB_YESNO, this);
#else
		CMessagePopup popup(L"Confirmation of\nModule Table", L"\n\nModule Table is finalized\napplying the current Equipment Settings.\nAfter confirmation, If Equipment is added,\nit will be added to the bottom of Module Table,\nIf Equipment is deleted,\nthe Equipment will be deleted from Module Table.", MB_YESNO, this);
#endif
		UINT nResult = popup.DoModal();
		if (nResult != IDOK) {
			return;
		}
	}
	//20221018 GBM end

// 	bool bCircuitChanged = false;
// 	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
// 		bool bCheck = CCircuitBasicInfo::Instance()->m_bCheck[nIndex];
// 		if (bCheck != CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex])
// 		{
// 			bCircuitChanged = true;
// 			break;
// 		}
// 	}

	//20221124 GBM start - test
#if 1
	//������ ȸ�� ������ ��ȭ�� üũ
	bool bResult1 = m_pSetupDlg[0]->CompareNewCircuitInfoNeo(0);
	bool bResult2 = m_pSetupDlg[1]->CompareNewCircuitInfoNeo(1);
#else
	//������ ȸ�� ������ ��ȭ�� üũ
	bool bResult1 = m_pSetupDlg[0]->CompareNewCircuitInfo(0);
	bool bResult2 = m_pSetupDlg[1]->CompareNewCircuitInfo(1);
#endif
	//20221124 GBM end

	//������ ���� ������ ��ȭ�� üũ
	//20221123 GBM start - test
	bool bResult3 = true;
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++)
	{
		bool bCheck = CCircuitBasicInfo::Instance()->m_bCheck[nIndex];
		if (CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex] != bCheck)
		{
			bResult3 = false;
			break;
		}
	}
	//20221123 GBM end

	//20221021 GBM start - �߰�� �϶�ǥ Ȯ�� ���� ȸ�� �⺻ ȭ������ ���ư��ٰ� �ƹ� ������ ���� ���� ��� �߰�� �϶�ǥ�� �ۼ��Ǿ� ���� �������� �ұ��ϰ� ȸ�� ������ ��ȭ�� ���� �߰�� �϶�ǥ�� �ۼ����� �ʴ� ���� ����
	//if (!bResult1 || !bResult2) 
	if (!bResult1 || !bResult2 || !bResult3 || CCommonState::ie()->m_bInitCircuit)
	//20221021 GBM end
	{

		//20221018 GBM start - �߰�� �϶�ǥ�� ���ۼ� / ���ۼ� �Ǿ��� �� �б�� ���� ����
#if 1
		if (!CCommonState::ie()->m_bInitCircuit) 
		{
#ifndef ENGLISH_MODE
			CMessagePopup popup(L"�߰�� �϶�ǥ ����", L"\n���� ���� ������ �߰�� �϶�ǥ�� �����\n\n���� �߰� ��: �߰�� �϶�ǥ �ϴ� �߰�,\n���� ���� ��: ���� �������� ������ ������ŭ\n�߰�� �϶�ǥ �� �������� ���� ����", MB_YESNO, this);
#else
			CMessagePopup popup(L"Change of Module Table", L"\nChange to Equipment Settings\nareapplied to Module Table.\nIn case of Equipment Addition:\nAt the bottom of Module Table\nIn case of Equipment Deletion:\nEquipment is deleted\nin reverse order on Module Table.", MB_YESNO, this);
#endif
			UINT nResult = popup.DoModal();
			if (nResult == IDOK)
			{
				// 1. ���� ���� ����Ʈ�� ������ m_selectciruit�� ����
				if (!CheckCircuitCount()) {
					return;
				}

				int nTotalCircuit0 = 0;
				int nTotalCircuit1 = 0;

				//�ִ� ȸ�� ���� �ʰ����� Ȯ��
				bCheckMaxCircuit1 = m_pSetupDlg[0]->CheckMaxCircuitCount(0, &nTotalCircuit0);
				bCheckMaxCircuit2 = m_pSetupDlg[1]->CheckMaxCircuitCount(1, &nTotalCircuit1);

				if (!bCheckMaxCircuit1 || !bCheckMaxCircuit2)
				{
					// ����/���� ���� �� ������ , ���� ȸ�� ���� �ѹ�
// 					m_pSetupDlg[0]->RollbackCircuitCount(0);
// 					m_pSetupDlg[1]->RollbackCircuitCount(1);

// 					m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
// 					m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

					CString strMsg = L"";

					if (!bCheckMaxCircuit1 && bCheckMaxCircuit2)
					{
#ifndef ENGLISH_MODE
						strMsg.Format(L"\n\n0 ���� ������ ������(%d)��\n�ִ� ȸ�μ�(%d)�� �ʰ���\n\n���� ������� ����\n(���� �߰�� �϶�ǥ ����)", nTotalCircuit0, MAX_CIRCUIT);
#else
						strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 0\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, MAX_CIRCUIT);
#endif
					}
					else if (bCheckMaxCircuit1 && !bCheckMaxCircuit2)
					{
#ifndef ENGLISH_MODE
						strMsg.Format(L"\n\n1 ���� ������ ������(%d)��\n�ִ� ȸ�μ�(%d)�� �ʰ���\n\n���� ������� ����\n(���� �߰�� �϶�ǥ ����)", nTotalCircuit1, MAX_CIRCUIT);
#else
						strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 1\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit1, MAX_CIRCUIT);
#endif
					}
					else
					{
#ifndef ENGLISH_MODE
						strMsg.Format(L"\n\n0 ����, 1 ���� ������ ������(%d, %d)��\n�ִ� ȸ�μ�(%d)�� �ʰ���\n\n���� ������� ����\n(���� �߰�� �϶�ǥ ����)", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#else
						strMsg.Format(L"\n\nThe number of Equipment Settings(%d, %d)\nof Loop 0 and Loop 1 exceeded\nthe maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#endif
					}

#ifndef ENGLISH_MODE
					CMessagePopup popup(L"�ִ� ȸ�� ���� �ʰ�", strMsg, MB_OK, this);
#else
					CMessagePopup popup(L"Exceed the maximum\nnumber of Circuit", strMsg, MB_OK, this);
#endif
					popup.DoModal();

					// ���� ������ Ȯ�� �� ���� ���� �ѹ���
					for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++)
					{
						bool bCheck = CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex];
						if (bCheck != CCircuitBasicInfo::Instance()->m_bCheck[nIndex])
						{
							CCircuitBasicInfo::Instance()->m_bCheck[nIndex] = bCheck;
						}
					}

					m_pSetupDlg[0]->CopyPreviousCircuitInfo(0);
					m_pSetupDlg[1]->CopyPreviousCircuitInfo(1);

					m_pSetupDlg[0]->InitCircuitInfo(0);
					m_pSetupDlg[1]->InitCircuitInfo(1);

					m_pSetupDlg[0]->Invalidate();
					m_pSetupDlg[1]->Invalidate();

					m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
					m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

					m_pSetupDlg[0]->LoadInfo(0);
					m_pSetupDlg[1]->LoadInfo(1);

					return;
				}

				// ������ Ȯ���������Ƿ� ���� ���� ��Ȳ�� old���� ������
				for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
					bool bCheck = CCircuitBasicInfo::Instance()->m_bCheck[nIndex];
					CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex] = bCheck;
				}

				// 2. ���� �� ����ü �� ���� ���� ������ ���� ���� ������ ����

				//20221014 GBM start - ���� ���� ������ ����صд�, 

				//�߰�� �϶�ǥ Ȯ�� ���� �ռ� ���������� ������ ���·� ����/ȸ�� ȭ�鸸 �Դٰ����ϴٰ� 
				//�ٽ� �߰�� �϶�ǥ Ȯ�� �� ���� ȸ�� ������ ��������Ʈ�� ��Ȳ�̶� 
				//�߰�� �϶�ǥ�� ������ ������ ������� �ʴ� ���� ������ ���� 
				//�߰�� �϶�ǥ�� Ȯ���Ǵ� ������ ������ �޸𸮿� ����� ��

				m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0);
				m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1);

				m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
				m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

				//20221122 GBM - ���� ���� �� ����� ��쿡 �ѹ� ���� ���� ������ ���� �����صд�.
				m_pSetupDlg[0]->BackupCircuitInfo(0);
				m_pSetupDlg[1]->BackupCircuitInfo(1);

				//�ѹ� ��� ȸ�� ���� ����
				m_pSetupDlg[0]->BackupCircuitCountInfo(0);
				m_pSetupDlg[1]->BackupCircuitCountInfo(1);

				//20221014 GBM end

				// 3. ���� ���� ������ ���� ���� ������ ���ؼ� �߰�/������ ����� ����Ʈ�� ��´�.
				CCommonState::ie()->m_vecAddCircuit.clear();
				CCommonState::ie()->m_vecDeleteCircuit.clear();

				m_pSetupDlg[0]->MakeAddCircuitInfoAndDeleteCircuitInfo(0);
				m_pSetupDlg[1]->MakeAddCircuitInfoAndDeleteCircuitInfo(1);

				GetParent()->PostMessage(SELECTION_PROJECT, 12, 0);
			}
			else
			{
				// ������ ��������Ƿ� ���� ������ Ȯ�� �� ���� ���� �ѹ���
				for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) 
				{
					bool bCheck = CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex];
					if (bCheck != CCircuitBasicInfo::Instance()->m_bCheck[nIndex])
					{
						CCircuitBasicInfo::Instance()->m_bCheck[nIndex] = bCheck;
					}
				}

				//���� ���� ���� ������ ���� ���� �߰�� �϶�ǥ Ȯ�� �� ����ߴ� ȸ�� ������ �����ͼ� ����ڰ� [���]�� ������ ��� ����

				m_pSetupDlg[0]->CopyPreviousCircuitInfo(0);
				m_pSetupDlg[1]->CopyPreviousCircuitInfo(1);

				//���� ������ �ݿ����� ���� ���̹Ƿ� ���� ������ ���ư�
				m_pSetupDlg[0]->InitCircuitInfo(0);
				m_pSetupDlg[1]->InitCircuitInfo(1);

				m_pSetupDlg[0]->Invalidate();
				m_pSetupDlg[1]->Invalidate();

				m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
				m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

				m_pSetupDlg[0]->LoadInfo(0);
				m_pSetupDlg[1]->LoadInfo(1);

				return;
				//GetParent()->PostMessage(SELECTION_PROJECT, 13, 0);
			}
		}
		else
		{

			// 1. ���� ���� ����Ʈ�� ������ m_selectciruit�� ����
			if (!CheckCircuitCount()) {
				return;
			}

			int nTotalCircuit0 = 0;
			int nTotalCircuit1 = 0;

			//�ִ� ȸ�� ���� �ʰ����� Ȯ��
			bCheckMaxCircuit1 = m_pSetupDlg[0]->CheckMaxCircuitCount(0, &nTotalCircuit0);
			bCheckMaxCircuit2 = m_pSetupDlg[1]->CheckMaxCircuitCount(1, &nTotalCircuit1);

			if (!bCheckMaxCircuit1 || !bCheckMaxCircuit2)
			{
				// ����/���� ���� �� ������ , ���� ȸ�� ���� �ѹ�
// 				m_pSetupDlg[0]->RollbackCircuitCount(0);
// 				m_pSetupDlg[1]->RollbackCircuitCount(1);

// 				m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
// 				m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

				CString strMsg = L"";

				if (!bCheckMaxCircuit1 && bCheckMaxCircuit2)
				{
#ifndef ENGLISH_MODE
					strMsg.Format(L"\n\n0 ���� ������ ������(%d)��\n�ִ� ȸ�μ�(%d)�� �ʰ���\n\n���� ������� ����", nTotalCircuit0, MAX_CIRCUIT);
#else
					strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 0\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, MAX_CIRCUIT);
#endif
				}
				else if (bCheckMaxCircuit1 && !bCheckMaxCircuit2)
				{
#ifndef ENGLISH_MODE
					strMsg.Format(L"\n\n1 ���� ������ ������(%d)��\n�ִ� ȸ�μ�(%d)�� �ʰ���\n\n���� ������� ����", nTotalCircuit1, MAX_CIRCUIT);
#else
					strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 1\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit1, MAX_CIRCUIT);
#endif
				}
				else
				{
#ifndef ENGLISH_MODE
					strMsg.Format(L"\n\n0 ����, 1 ���� ������ ������(%d, %d)��\n�ִ� ȸ�μ�(%d)�� �ʰ���\n\n���� ������� ����", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#else
					strMsg.Format(L"\n\nThe number of Equipment Settings(%d, %d)\nof Loop 0 and Loop 1 exceeded\nthe maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#endif
				}

#ifndef ENGLISH_MODE
				CMessagePopup popup(L"�ִ� ȸ�� ���� �ʰ�", strMsg, MB_OK, this);
#else
				CMessagePopup popup(L"Exceed the maximum\nnumber of Circuit", strMsg, MB_OK, this);
#endif
				popup.DoModal();

				// ���� ������ Ȯ�� �� ���� ���� �ѹ���
				for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++)
				{
					bool bCheck = CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex];
					if (bCheck != CCircuitBasicInfo::Instance()->m_bCheck[nIndex])
					{
						CCircuitBasicInfo::Instance()->m_bCheck[nIndex] = bCheck;
					}
				}

				m_pSetupDlg[0]->CopyPreviousCircuitInfo(0);
				m_pSetupDlg[1]->CopyPreviousCircuitInfo(1);

				m_pSetupDlg[0]->InitCircuitInfo(0);
				m_pSetupDlg[1]->InitCircuitInfo(1);

				m_pSetupDlg[0]->Invalidate();
				m_pSetupDlg[1]->Invalidate();

				m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
				m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

				m_pSetupDlg[0]->LoadInfo(0);
				m_pSetupDlg[1]->LoadInfo(1);

				return;
			}

			// ������ Ȯ���������Ƿ� ���� ���� ��Ȳ�� old���� ������
			for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
				bool bCheck = CCircuitBasicInfo::Instance()->m_bCheck[nIndex];
				CCircuitBasicInfo::Instance()->m_bOldCheck[nIndex] = bCheck;
			}

			//20221014 GBM start - ���� ���� ������ ����صд�.
			m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0);
			m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1);
			//20221014 GBM end

			m_pSetupDlg[0]->CopyPreviousCircuitCompInfo(0);
			m_pSetupDlg[1]->CopyPreviousCircuitCompInfo(1);

			//20221122 GBM - ���� ���� �� ����� ��쿡 �ѹ� ���� ���� ������ ���� �����صд�.
			m_pSetupDlg[0]->BackupCircuitInfo(0);
			m_pSetupDlg[1]->BackupCircuitInfo(1);

			//�ѹ� ��� ȸ�� ���� ����
			m_pSetupDlg[0]->BackupCircuitCountInfo(0);
			m_pSetupDlg[1]->BackupCircuitCountInfo(1);

			//�߰�� �϶�ǥ�� �ϴ� Ȯ���Ǵ� ���� ��ü ȸ�� ������ ���ϰ� �߰��п� ���ؼ��� ���� �߰��ؼ� ���� ���� ȸ�� ������ �׻� �˰� �ֵ��� ��
			CCommonState::ie()->m_nTotalCountCircuit_0 = CCommonState::ie()->CalculateTotalCircuitCount(0);
			CCommonState::ie()->m_nTotalCountCircuit_1 = CCommonState::ie()->CalculateTotalCircuitCount(1);

			//�߰�� �϶�ǥ Ȯ�� ��� ��ư Ȱ��ȭ
			m_btnCircuitListInit.ShowWindow(TRUE);
			m_btnCircuitListInit.Invalidate();

			GetParent()->PostMessage(SELECTION_PROJECT, 12, 0);
		}
#else
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
#endif
		//20221018 GBM end

	}
	else {
		GetParent()->PostMessage(SELECTION_PROJECT, 13, 0);
	}
}

void CCircuitInfoDlg::OnPrevClick()
{
	bool bCheckMaxCircuit1 = false;
	bool bCheckMaxCircuit2 = false;

	//20221014 GBM start - ���� ȸ�� ���� �۾� ���� �޸𸮿� ����
	m_pSetupDlg[0]->SaveCircuitInfo(0);
	m_pSetupDlg[1]->SaveCircuitInfo(1);
	//20221014 GBM end

	int nTotalCircuit0 = 0;
	int nTotalCircuit1 = 0;

	//�ִ� ȸ�� ���� �ʰ����� Ȯ��
	bCheckMaxCircuit1 = m_pSetupDlg[0]->CheckMaxCircuitCount(0, &nTotalCircuit0);
	bCheckMaxCircuit2 = m_pSetupDlg[1]->CheckMaxCircuitCount(1, &nTotalCircuit1);

	if (!bCheckMaxCircuit1 || !bCheckMaxCircuit2)
	{
		// ����/���� ���� �� ������ , ���� ȸ�� ���� �ѹ�, �߰�� �϶�ǥ�� �Ѿ ���ʹ� �޸� �ٷ� ���� ������ �����ͷ� �ǵ��� ��
		m_pSetupDlg[0]->RollbackCircuitCount(0);
		m_pSetupDlg[1]->RollbackCircuitCount(1);

		CString strMsg = L"";

		if (!bCheckMaxCircuit1 && bCheckMaxCircuit2)
		{
#ifndef ENGLISH_MODE
			strMsg.Format(L"\n\n0 ���� ������ ������(%d)��\n�ִ� ȸ�μ�(%d)�� �ʰ���\n\n���� ������� ����", nTotalCircuit0, MAX_CIRCUIT);
#else
			strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 0\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, MAX_CIRCUIT);
#endif
		}
		else if (bCheckMaxCircuit1 && !bCheckMaxCircuit2)
		{
#ifndef ENGLISH_MODE
			strMsg.Format(L"\n\n1 ���� ������ ������(%d)��\n�ִ� ȸ�μ�(%d)�� �ʰ���\n\n���� ������� ����", nTotalCircuit1, MAX_CIRCUIT);
#else
			strMsg.Format(L"\n\nThe number of Equipment Settings(%d) of Loop 1\nexceeded the maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit1, MAX_CIRCUIT);
#endif
		}
		else
		{
#ifndef ENGLISH_MODE
			strMsg.Format(L"\n\n0 ����, 1 ���� ������ ������(%d, %d)��\n�ִ� ȸ�μ�(%d)�� �ʰ���\n\n���� ������� ����", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#else
			strMsg.Format(L"\n\nThe number of Equipment Settings(%d, %d)\nof Loop 0 and Loop 1 exceeded\nthe maximum number of Circuit(%d).\n\nReturn to the number of existed Equipment Settings\nbased on the current Module Table", nTotalCircuit0, nTotalCircuit1, MAX_CIRCUIT);
#endif
		}

#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ִ� ȸ�� ���� �ʰ�", strMsg, MB_OK, this);
#else
		CMessagePopup popup(L"Exceed the maximum\nnumber of Circuit", strMsg, MB_OK, this);
#endif
		popup.DoModal();

		m_pSetupDlg[0]->InitCircuitInfo(0);
		m_pSetupDlg[1]->InitCircuitInfo(1);

		m_pSetupDlg[0]->LoadInfo(0);
		m_pSetupDlg[1]->LoadInfo(1);

		return;
	}

	//20221014 GBM start - ���� ���� ������ ����صд�.
	m_pSetupDlg[0]->CopyNewCircuitInfoToOldCircuitInfo(0);
	m_pSetupDlg[1]->CopyNewCircuitInfoToOldCircuitInfo(1);
	//20221014 GBM end

	GetParent()->PostMessage(SELECTION_PROJECT, 0, 2);
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
		m_btnCircuitListInit.MoveWindow(rect.right - 350, 0, 155, 48);
		m_btnNext.Invalidate();
		m_btnPrev.Invalidate();
		m_btnCircuitListInit.Invalidate();
	}
}

void CCircuitInfoDlg::OnCircuitListInit()
{
#ifndef ENGLISH_MODE
	CMessagePopup popup(L"�߰�� �϶�ǥ �ʱ�ȭ", L"\n�߰�� �϶�ǥ�� �ʱ�ȭ��\n\n�� ��� : [Ȯ��] ���� ��\n������ ������ �߰�� �϶�ǥ�� ��� �����Ǿ�\n�ٽ� �߰�� �϶�ǥ�� Ȯ���ص�\n[�� ȸ�ι�ȣ]�� ����˴ϴ�.\n������ �����ϼ���.", MB_YESNO, this);
#else
	CMessagePopup popup(L"Initialization\nof Module Table", L"\n\nThe Module Table will be initialized\n\n�� Warning : When [OK] is selected,\nAll of created Module Table is deleted,\nso even if Module Table is confirmed again,\n[New Circuit Number] will be applied.", MB_YESNO, this);
#endif

	UINT nResult = popup.DoModal();
	if (nResult != IDOK)
	{
		return;
	}

	//�߰�� �϶�ǥ Ȯ�� �÷��� false
	CCommonState::ie()->m_bInitCircuit = true;

	//ȸ�� ����Ʈ ����
	CSaveManager::ie()->ReleaseInfo();

	//�߰�� �϶�ǥ Ȯ�� ��� ��ư ��Ȱ��ȭ
	m_btnCircuitListInit.ShowWindow(FALSE);
	m_btnCircuitListInit.Invalidate();

	//�߰�� �϶�ǥ Ȯ�� ���� ����, �߰�� �϶�ǥ Excel ���� �÷��� false
	GetParent()->PostMessage(CIRCUIT_LIST_INIT_MSG, 0, 0);

}