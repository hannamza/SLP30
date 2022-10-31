// CircuitChartDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CircuitChartDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CCircuitChartDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCircuitChartDlg, CDialogEx)

CCircuitChartDlg::CCircuitChartDlg(CWnd* pParent /*=NULL*/, int nCircuit)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_pListCtrl = NULL;
	m_nCircuit = nCircuit;
}

CCircuitChartDlg::~CCircuitChartDlg()
{
}

void CCircuitChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircuitChartDlg, CDialogEx)
	ON_MESSAGE(LIST_CONTROL, OnListControl)
	ON_MESSAGE(LIST_POPUP_SEL, OnPopupSelect)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCircuitChartDlg �޽��� ó�����Դϴ�.
const TCHAR* g_editHeader[] = { _T("�߰��No"), _T("ȸ��No"), _T("�����"), _T("�ǹ���"), _T("��"), _T("���"), _T("��"), _T("�Ǹ�"), _T("�����ȣ"), NULL };
const int g_editSize[] = { 80, 70, 200, 80, 120, 90, 90, 100, 75, 0 }; // pixel

//20221026 GBM start - �߰��No, �Ǹ� ���������ϵ��� ��
#if 1
	const int g_editType[] = { POPUP_TYPE_EDIT, POPUP_TYPE_SEQ , POPUP_TYPE_NONE, POPUP_TYPE_NONE, POPUP_TYPE_NONE, POPUP_TYPE_NONE, POPUP_TYPE_NONE, POPUP_TYPE_LIST_ADD, POPUP_TYPE_NONE, 0 };
#else
	const int g_editType[] = { POPUP_TYPE_EDIT, POPUP_TYPE_SEQ , POPUP_TYPE_LIST_NOINPUT, POPUP_TYPE_EDIT, POPUP_TYPE_LIST_NOINPUT, POPUP_TYPE_LIST_NOINPUT, POPUP_TYPE_LIST_NOINPUT, POPUP_TYPE_LIST_ADD, POPUP_TYPE_EDIT, 0 };
#endif
//20221026 GBM end

const int g_editAlign[] = { DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, -1 }; // option, row align text, default: DT_LEFT
const int g_editLimit[] = { 10, 10, 0, 0, 0, 20, 20, 10, -1 }; // 0: ������, > 0 : �ؽ�Ʈ ������ ����

BOOL CCircuitChartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	CRuntimeClass	*pRuntimeClass;
	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomListCtrl))) {
		if (NULL != (m_pListCtrl = (CCustomListCtrl *)(pRuntimeClass->CreateObject()))) {
			m_pListCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}

	m_pListCtrl->SetListIndex(0);
	m_pListCtrl->SetHeaderInfo((TCHAR**)g_editHeader, (int*)g_editSize, (int*)g_editType, (int*)g_editAlign); // Align: option, row align text, default: DT_LEFT
	m_pListCtrl->SetColumnLimit((int*)g_editLimit);
	// ���� ���� (option)
	m_pListCtrl->SetHeaderHeight(40);
	m_pListCtrl->SetRowHeight(30);
	//m_pListCtrl->SetListPopupSize(500, 500);
	// ������ (option)
	m_pListCtrl->SetHeaderBkColor(RGB(80, 80, 80), RGB(255, 255, 255));
	m_pListCtrl->SetHeaderTextColor(RGB(220, 220, 220));
	m_pListCtrl->SetRowBkColor(RGB(160, 160, 160), RGB(255, 255, 0), RGB(255, 255, 255));
	m_pListCtrl->SetRowTextColor(RGB(255, 255, 255), RGB(80, 80, 80));
	m_pListCtrl->SetHeaderFontSize(15);
	m_pListCtrl->FixHeaderSize(false);

	CStringArray arrayName;
	CCircuitBasicInfo::Instance()->GetCircuitName(arrayName);
	for (int nIndex = 0; nIndex < CIRCUIT_CNT; nIndex++) {
		m_pListCtrl->AddPopupListItem(2, arrayName.GetAt(nIndex));
	}

	m_pListCtrl->SendMessage(WM_INITIALUPDATE);

	m_pListCtrl->AddItem(MAX_CIRCUIT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CCircuitChartDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CCircuitChartDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CCircuitChartDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCircuitChartDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircuitChartDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircuitChartDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCircuitChartDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();
}

void CCircuitChartDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CCircuitChartDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CCircuitChartDlg::OnPopupSelect(WPARAM wParam, LPARAM lParam)
{
	int nItemIndex = (int)wParam;
	int nSelCol = (int)lParam;
	if (nSelCol == 2) {
		m_pListCtrl->SetItemText(nItemIndex, 3, CCircuitBasicInfo::Instance()->m_sBuildingName);
	}
	return 0;
}

LRESULT CCircuitChartDlg::OnListControl(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;
	int nSelectItem = (int)lParam;
	switch (nType)
	{
	case 1: // del
		if (m_pListCtrl) {
			CMessagePopup popup(L"��� ����", L"\n\n������ ����� �����Ͻø�\n\n������ ��Ͽ� �ű� ����� 1�� �߰��˴ϴ�.\n\n������ ����� �����Ͻðڽ��ϱ�?", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				m_pListCtrl->DelListItem(nSelectItem);
				m_pListCtrl->AddItem();

				SettingChartNumber();
			}
		}
		break;
	case 2: // up add
		if (m_pListCtrl) {
			CMessagePopup popup(L"��� �߰�", L"\n\n�ű� ����� �߰��Ͻø�\n\n������ ��� 1���� �����˴ϴ�.\n\n�ű� ����� �߰��Ͻðڽ��ϱ�?", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				m_pListCtrl->InsertListItemUp(nSelectItem);
				m_pListCtrl->DelListItem(m_pListCtrl->GetItemCount() - 1);
				m_pListCtrl->Invalidate();

				SettingChartNumber();
			}
		}
		break;
	case 3: // down add
		if (m_pListCtrl) {
			CMessagePopup popup(L"��� �߰�", L"\n\n�ű� ����� �߰��Ͻø�\n\n������ ��� 1���� �����˴ϴ�.\n\n�ű� ����� �߰��Ͻðڽ��ϱ�?", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				m_pListCtrl->InsertListItemDown(nSelectItem);
				m_pListCtrl->DelListItem(m_pListCtrl->GetItemCount() - 1);
				m_pListCtrl->Invalidate();

				SettingChartNumber();
			}
		}
		break;
	//20221027 GBM start - ȸ�ι�ȣ �̵� ���̽� �߰�
	case 4:
	{
		int* nCircuitMoveNum = (int*)lParam;
		int nCurrentIndex = nCircuitMoveNum[0];
		int nNewIndex = nCircuitMoveNum[1];
		int nMaxCircuitAddress = MAX_CIRCUIT_ADDRESS;

		bool bIsVacantCircuit = false;
		CString strMsg = L"";

		//���� ������ ��ü ȸ�� ������ ������ ȸ�� ��ȣ�� �ʰ��ϴ��� �˻�
		if (m_nCircuit == 0)
		{
			if (nNewIndex + 1 > CCommonState::ie()->m_nTotalCountCircuit_0)
			{
				strMsg.Format(L"\n\n�� ȸ�ι�ȣ(%d)��\n���� ������ �ִ� ȸ�ι�ȣ(%d)�� �ʰ��� �� �����ϴ�.\n�ִ� ȸ�ι�ȣ(%d) �̻� ���� ��������\n[����]���� ���ư� ���ϴ� ���� ������ �÷��ּ���.",
					nNewIndex + 1, CCommonState::ie()->m_nTotalCountCircuit_0, CCommonState::ie()->m_nTotalCountCircuit_0);
				CMessagePopup popup(L"�ִ� ȸ�ι�ȣ �ʰ�", strMsg, MB_OK, this);
				popup.DoModal();
				break;
			}
		}
		else
		{
			if (nNewIndex + 1 > CCommonState::ie()->m_nTotalCountCircuit_1)
			{
				strMsg.Format(L"\n\n�� ȸ�ι�ȣ(%d)��\n���� ������ �ִ� ȸ�ι�ȣ(%d)�� �ʰ��� �� �����ϴ�.\n�ִ� ȸ�ι�ȣ(%d) �̻� ���� ��������\n[����]���� ���ư� ���ϴ� ���� ������ �÷��ּ���.",
					nNewIndex + 1, CCommonState::ie()->m_nTotalCountCircuit_1, CCommonState::ie()->m_nTotalCountCircuit_1);
				CMessagePopup popup(L"�ִ� ȸ�ι�ȣ �ʰ�", strMsg, MB_OK, this);
				popup.DoModal();
				break;
			}
		}

		// �� ������ �ѹ��� �߰�� �϶�ǥ ȸ�� ������ ����� ���� ���ٸ� ����
		int nSystemInfo = 0;
		nSystemInfo = CSaveManager::ie()->m_listSystem.GetSize();
		if (nSystemInfo <= 0)
		{
			SaveInformation(0);
			SaveInformation(1);
		}

		if (m_nCircuit == 1)
		{
			nCurrentIndex += nMaxCircuitAddress;
			nNewIndex += nMaxCircuitAddress;
		}

		bIsVacantCircuit = CSaveManager::ie()->MoveCircuitInfo(nCurrentIndex, nNewIndex);

		if (!bIsVacantCircuit)
		{
			int nNewCircuitNum = 0;
			if (m_nCircuit == 0)
				nNewCircuitNum = nNewIndex + 1;
			else
				nNewCircuitNum = nNewIndex + 1 - nMaxCircuitAddress;

			strMsg.Format(L"\n\n�� ȸ�ι�ȣ(%d)���� ���� ȸ�ΰ� �����մϴ�.", nNewCircuitNum);

			CMessagePopup popup(L"���� ȸ�� ����", strMsg, MB_OK, this);
			popup.DoModal();
			break;
		}

		if (m_nCircuit == 0)
		{
			DisplayLoadFile(0);
		}
		else
		{
			DisplayLoadFile(1);
		}
	}
		break;
	//20221027 GBM end
	default:
		break;

	}
	return 0;
}

void CCircuitChartDlg::SettingChartNumber()
{
	CString sText;
	int nCount = m_pListCtrl->GetItemCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		m_pListCtrl->GetItemText(nIndex, 0, sText);
		if (sText.GetLength() > 0) {
			sText.Format(L"%d", nIndex + 1);
			m_pListCtrl->SetItemText(nIndex, 0, sText);
		}
		sText = L"";
	}
}

bool CCircuitChartDlg::SaveInformation(int nChartIndex)
{
	if (!m_pListCtrl) {
		return false;
	}

	int nCount = m_pListCtrl->GetItemCount();
	CString sBlock, sStair, sFloor, sSystemNo, sCircuitNo, sCircuitName, sBDName, sRoomName;
	int nStair, nFloor, nSystemNo, nCircuitNo;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		m_pListCtrl->GetItemText(nIndex, 0, sSystemNo);
		m_pListCtrl->GetItemText(nIndex, 2, sCircuitName);
		m_pListCtrl->GetItemText(nIndex, 3, sBDName);
		m_pListCtrl->GetItemText(nIndex, 4, sBlock);
		m_pListCtrl->GetItemText(nIndex, 5, sStair);
		m_pListCtrl->GetItemText(nIndex, 6, sFloor);
		m_pListCtrl->GetItemText(nIndex, 7, sRoomName);
		m_pListCtrl->GetItemText(nIndex, 8, sCircuitNo);

		sStair.Replace(L"���", L"");
		nStair = _wtoi(sStair.GetBuffer(0));
		if (sFloor.Find(L"B") >= 0) {
			sFloor.Replace(L"B", L"");
			sFloor.Replace(L"F", L"");
			nFloor = -_wtoi(sFloor.GetBuffer(0));
		}
		else {
			sFloor.Replace(L"F", L"");
			nFloor = _wtoi(sFloor.GetBuffer(0));
		}
		nSystemNo = _wtoi(sSystemNo.GetBuffer(0));
		nCircuitNo = _wtoi(sCircuitNo.GetBuffer(0));

		sBlock.Replace(L"��", L"");

		CSaveManager::ie()->SetSystemInfo(sBDName.GetBuffer(0), sCircuitName.GetBuffer(0), sRoomName.GetBuffer(0), sBlock.GetBuffer(0),
			(short)nStair, (short)nSystemNo, (short)nChartIndex, (short)nFloor, (short)nCircuitNo);
	}
	for (int nIndex = 0; nIndex < 6; nIndex++) {
		CSaveManager::ie()->SetSystemInfo(L"", L"", L"", L"", 0, 0, 0, 0, 250 + nIndex);
	}
	return true;
}

void CCircuitChartDlg::SetupPopupList()
{
	CString sTemp;
	int nStairCount = CCircuitBasicInfo::Instance()->m_nStair;
	m_pListCtrl->RemovePopupListItem(4);
	m_pListCtrl->RemovePopupListItem(5);
	m_pListCtrl->RemovePopupListItem(6);

	for (int nIndex = 0; nIndex < CCircuitBasicInfo::Instance()->m_nBlock; nIndex++) {
		if (CCircuitBasicInfo::Instance()->m_nBlock > 0) {
			sTemp = CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nIndex);
			sTemp += L"��";
			m_pListCtrl->AddPopupListItem(4, sTemp);
		}
	}
	for (int nIndex = 0; nIndex < nStairCount; nIndex++) {
		sTemp.Format(L"%d���", nIndex + 1);
		m_pListCtrl->AddPopupListItem(5, sTemp);
	}
	int nBasement = CCircuitBasicInfo::Instance()->m_nBasement;
	for (int nIndex = nBasement; nIndex > 0; nIndex--) {
		sTemp.Format(L"B%dF", nIndex);
		m_pListCtrl->AddPopupListItem(6, sTemp);
	}
	int nFloor = CCircuitBasicInfo::Instance()->m_nFloor;
	for (int nIndex = 0; nIndex < nFloor; nIndex++) {
		sTemp.Format(L"%dF", nIndex + 1);
		m_pListCtrl->AddPopupListItem(6, sTemp);
	}
}

void CCircuitChartDlg::DisplayLoadFile(int nChartIndex)
{
	m_pListCtrl->ReleaseListItem();

	SetBackgroundColorAfterMaxCircuit(m_nCircuit);		//20221028 GBM - ����Ʈ�� �ڽ��� ���� ������ �� �� �ֵ��� �߰�

	SetupPopupList();

	CString sTemp;
	int nCount = CSaveManager::ie()->m_listSystem.GetCount();
	int nValue = 0;
	SYSTEM_INFO_* pInfo = NULL;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		pInfo = CSaveManager::ie()->m_listSystem.GetAt(CSaveManager::ie()->m_listSystem.FindIndex(nIndex));
		if (pInfo->nSystem != nChartIndex) {
			continue;
		}
		sTemp = L"";
		if (pInfo->nSystemNo > 0) {
			sTemp.Format(L"%d", (int)pInfo->nSystemNo);
		}
		m_pListCtrl->SetItemText(nValue, 0, sTemp);
		m_pListCtrl->SetItemText(nValue, 2, pInfo->szCircuitName);
		m_pListCtrl->SetItemText(nValue, 3, pInfo->szBDName);
		if (wcslen(pInfo->szBlock) > 0) {
			sTemp.Format(L"%s��", pInfo->szBlock);
		}
		else {
			sTemp = L"";
		}
		m_pListCtrl->SetItemText(nValue, 4, sTemp);

		if (pInfo->nStair > 0) {
			sTemp.Format(L"%d���", (int)pInfo->nStair);
		}
		else {
			sTemp = L"";
		}
		m_pListCtrl->SetItemText(nValue, 5, sTemp);
		if (pInfo->nFloor > 0) {
			sTemp.Format(L"%dF", (int)pInfo->nFloor);
		}
		else if (pInfo->nFloor < 0) {
			sTemp.Format(L"B%dF", (int)abs(pInfo->nFloor));
		}
		else {
			sTemp = L"";
		}
		m_pListCtrl->SetItemText(nValue, 6, sTemp);
		m_pListCtrl->SetItemText(nValue, 7, pInfo->szRoomName);
		sTemp = L"";
		if (pInfo->nCircuitNo > 0) {
			sTemp.Format(L"%d", (int)pInfo->nCircuitNo);
		}
		m_pListCtrl->SetItemText(nValue, 8, sTemp);

		++nValue;
	}
}

bool CCircuitChartDlg::DisplayListItem(int nChartIndex)
{
	m_pListCtrl->ReleaseListItem();

	SetBackgroundColorAfterMaxCircuit(m_nCircuit);		//20221028 GBM - ����Ʈ�� �ڽ��� ���� ������ �� �� �ֵ��� �߰�

	SetupPopupList();

	CString sBlock, sStair, sFloor, sCircuitCount, sTemp;
	pSelectCircuit pCircuit = NULL;
	CStringArray arrayList;
	bool bEmpty = false;
	int nValue = 0;
	int nCount = 0;
	int nCircuitCount = 0;
	if (nChartIndex == 0) {
		nCount = CCommonState::ie()->m_selectCircuit_0.GetCount();
	}
	else if (nChartIndex == 1) {
		nCount = CCommonState::ie()->m_selectCircuit_1.GetCount();
	}
	bool bFan = false;
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		if (nChartIndex == 0) {
			pCircuit = CCommonState::ie()->m_selectCircuit_0.GetAt(CCommonState::ie()->m_selectCircuit_0.FindIndex(nIndex));
		}
		else if (nChartIndex == 1) {
			pCircuit = CCommonState::ie()->m_selectCircuit_1.GetAt(CCommonState::ie()->m_selectCircuit_1.FindIndex(nIndex));
		}
		if (!pCircuit) {
			continue;
		}
		if (nIndex == 0) {
			sStair = pCircuit->sStair;
			sFloor = pCircuit->sFloor;
			sBlock = pCircuit->sBlock;
		}
		if (sBlock.Compare(pCircuit->sBlock) != 0 || sStair.Compare(pCircuit->sStair) != 0 || sFloor.Compare(pCircuit->sFloor) != 0) {
			sStair = pCircuit->sStair;
			sFloor = pCircuit->sFloor;
			sBlock = pCircuit->sBlock;
			if (nValue % 2 == 1) {
				++nValue;
			}
		}
		bool bFind = false;
		for (int nIndex = �����_��ܰ�����; nIndex < �����_SVP������A; nIndex++) {
			if (pCircuit->sCircuitName.Compare(g_lpszCircuitName[nIndex]) == 0) {
				bFind = true;
				break;
			}
		}
		if (bFind) {
			if (bFan) {
				if (nValue % 2 == 1) {
					nValue++;
				}
				bFan = false;
			}
			nCircuitCount = 0;
			for (int ii = 0; ii < pCircuit->nCount; ii++) {
				sCircuitCount.Format(L"%d", nCircuitCount + 1);
				++nCircuitCount;
				bEmpty = false;
				if (pCircuit->sCircuitName == g_lpszCircuitName[�����_��ܰ�����]
					|| pCircuit->sCircuitName == g_lpszCircuitName[�����_ELEV������]) {
					if (nValue % 2 == 1) {
						nValue++;
					}
					bEmpty = true;
				}
				if (nValue % 2 == 0) {
					sTemp.Format(L"%d", nValue + 1);
					m_pListCtrl->SetItemText(nValue, 0, sTemp);
				}
				m_pListCtrl->SetItemText(nValue, 2, pCircuit->sCircuitName);
				m_pListCtrl->SetItemText(nValue, 3, CCircuitBasicInfo::Instance()->m_sBuildingName);
				m_pListCtrl->SetItemText(nValue, 4, pCircuit->sBlock);
				m_pListCtrl->SetItemText(nValue, 5, pCircuit->sStair);
				m_pListCtrl->SetItemText(nValue, 6, pCircuit->sFloor);
				int nCNoIndex = CCircuitBasicInfo::Instance()->GetIndexCircuitName(pCircuit->sCircuitName);
				//m_pListCtrl->SetItemText(nValue, 7, g_lpszCircuitNo[nCNoIndex]);
				if (pCircuit->nCount > 1) {
					m_pListCtrl->SetItemText(nValue, 8, sCircuitCount);
				}

				++nValue;
				if (bEmpty) {
					++nValue;
				}
			}
			if (nValue > MAX_CIRCUIT) {
				return false;
			}
			continue;
		}
		nCircuitCount = 0;
		CCircuitBasicInfo::Instance()->GetCircuitChild(pCircuit->sCircuitName, arrayList);
		int nCircuitIndex = CCircuitBasicInfo::Instance()->GetCircuitParentIndex(pCircuit->sCircuitName);
		for (int ii = 0; ii < pCircuit->nCount; ii++) {
			switch (nCircuitIndex) {
			case 2: // �˶���� PS 1, TS 1
			case 5: // �������� �ޱ� 2
			case 6: // �������� ��� 2
			case 15: // ������ ���� 2
			case 3: // �����׼� ��� 4
			case 7: // ������ 4
			case 8: // ��ȭ���� 4
				if (nValue % 2 == 1) {
					++nValue;
				}
				break;
			default:
				break;
			}
			sCircuitCount.Format(L"%d", nCircuitCount + 1);
			++nCircuitCount;
			for (int i = 0; i < arrayList.GetCount(); i++) {
				if (i == 0 && nValue % 2 == 0) {
					sTemp.Format(L"%d", nValue + 1);
					m_pListCtrl->SetItemText(nValue, 0, sTemp);
				}
				m_pListCtrl->SetItemText(nValue, 2, arrayList.GetAt(i));
				m_pListCtrl->SetItemText(nValue, 3, CCircuitBasicInfo::Instance()->m_sBuildingName);
				m_pListCtrl->SetItemText(nValue, 4, pCircuit->sBlock);
				m_pListCtrl->SetItemText(nValue, 5, pCircuit->sStair);
				m_pListCtrl->SetItemText(nValue, 6, pCircuit->sFloor);
				int nCNoIndex = CCircuitBasicInfo::Instance()->GetIndexCircuitName(arrayList.GetAt(i));
				//m_pListCtrl->SetItemText(nValue, 7, g_lpszCircuitNo[nCNoIndex]);
				if (pCircuit->nCount > 1) {
					m_pListCtrl->SetItemText(nValue, 8, sCircuitCount);
				}
				++nValue;
			}
			switch (nCircuitIndex) {
			//case 2: // �˶���� PS 1, TS 1
			//case 5: // �������� �ޱ� 2
			//case 6: // �������� ��� 2
			//case 15: // ������ ���� 2
			//	break;
			//case 3: // �����׼� ��� 4
			//case 7: // ������ 4
			//case 8: // ��ȭ���� 4
			//	break;
				//sTemp.Format(L"%d", nValue + 1 - 2);
				//m_pListCtrl->SetItemText(nValue - 2, 0, sTemp);
				//break;
			case 4: // �ð��溸1 //������ȭ���� 4
			case 9: // �迬â 1
			case 10: // ��ȭ�� 1
			case 13: // ��ȭ�� �⵿Ȯ�� 1
			case 14: // ������Ȯ�� 1
			case 0: // ��ȭ�� 1 
			case 1: // �߽ű� 1
			case 11: // â����ġ 1
			case 12: // ������ġ 1
			case 16: // �ޱ���
			case 17: // �����
				nValue++;
				break;
				//bFan = true;
				break;
			case 18: // ������ 1
				/*if (nValue % 2 == 1) {
					nValue++;
				}*/
				break;
			default: break;
			}
		}
		if (nValue > MAX_CIRCUIT) {
			return false;
		}
	}
	return true;
}

void CCircuitChartDlg::Redisplay()
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

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_pListCtrl) {
		rt.CopyRect(&rect);

		m_pListCtrl->MoveWindow(rt);
		m_pListCtrl->ShowWindow(SW_SHOW);
		m_pListCtrl->Invalidate();
	}
}

void CCircuitChartDlg::SetBackgroundColorAfterMaxCircuit(int nChartIndex)
{
	m_pListCtrl->SetBackgroundColorAfterMaxCircuit(nChartIndex);
}