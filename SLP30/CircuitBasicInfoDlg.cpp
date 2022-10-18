// CircuitBasicInfoDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "CircuitBasicInfoDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CCircuitBasicInfoDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCircuitBasicInfoDlg, CDialogEx)

CCircuitBasicInfoDlg::CCircuitBasicInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex] = NULL;
	}
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
		_T("����")); // lpszFacename 

	m_bFirstWork = true;
	m_bInit = false;

	m_bBuildingInfoComplate = false;
}

CCircuitBasicInfoDlg::~CCircuitBasicInfoDlg()
{
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		SAFE_DELETE(m_pCheck[nIndex]);
	}
}

void CCircuitBasicInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircuitBasicInfoDlg, CDialogEx)
	ON_COMMAND(IDC_COMMON_BUTTON1, OnNextClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCircuitBasicInfoDlg �޽��� ó�����Դϴ�.
const TCHAR* lpszHeaderBasic[] = { _T("�ǹ���"), _T("��"), _T("���̸�"), _T("���"), _T("������"), _T("������"), NULL };
const int colWidthListBasic[] = { 200, 90, 180, 100, 100, 100, 0 }; // pixel
const int colTypeBasic[] = { POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, 0 };
const int colAlignBasic[] = { DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, -1 }; // option, row align text, default: DT_LEFT
const int colLimitBasic[] = { 0, 6, 0, 6, 6, 6, -1 }; // 0: ������, > 0 : �ؽ�Ʈ ������ ����

BOOL CCircuitBasicInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	CRect rect;
	GetClientRect(rect);

	CRuntimeClass	*pRuntimeClass;
	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomListCtrl))) {
		if (NULL != (m_pListCtrl = (CCustomListCtrl *)(pRuntimeClass->CreateObject()))) {
			m_pListCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}
	m_pListCtrl->SetListIndex(0);
	m_pListCtrl->SetHeaderInfo((TCHAR**)lpszHeaderBasic, (int*)colWidthListBasic, (int*)colTypeBasic, (int*)colAlignBasic); // Align: option, row align text, default: DT_LEFT
	m_pListCtrl->SetColumnLimit((int*)colLimitBasic);
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
	m_pListCtrl->FixHeaderSize(true);
	m_pListCtrl->SetDisableMenu(true);

	m_pListCtrl->SendMessage(WM_INITIALUPDATE);

	m_pListCtrl->AddItem();
	m_pListCtrl->SetItemText(0, 0, L"");
	m_pListCtrl->SetItemText(0, 1, L"0");
	m_pListCtrl->SetItemText(0, 2, L"101");
	m_pListCtrl->SetItemText(0, 3, L"1");
	m_pListCtrl->SetItemText(0, 4, L"2");
	m_pListCtrl->SetItemText(0, 5, L"0");

	int nY = 188;
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		switch (nIndex) {
		case 0:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[��ȭ��] PBL SET �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 1:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�߽ű�] PBL SET �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 2:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�˶����] ������Ŭ�� ������ �˶���갡 �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 3:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�����׼ǹ��] ������Ŭ�� ������ �����׼ǹ�갡 �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 4:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�ð��溸] �ð��溸 ���� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 5:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[��������] �������� �ޱ⼳�� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 6:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[��������] �������� ��⼳�� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 7:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[������] ������ ���� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 8:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[��ȭ����] ��ȭ���� ���� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 9:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�迬â] �迬â ���� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 10:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[��ȭ��] ��ȭ�� ���� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 11:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[â����ġ] â�������ġ�� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 12:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[������ġ] �ڵ������ġ�� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 13:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[��ȭ���⵿Ȯ��] ��ȭ���⵿ ǥ�õ��� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 14:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[������Ȯ��] ������ Ȯ���� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 15:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[����������] ���������ð� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 16:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�����ޱ���] �ޱ��� ���� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 17:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[���������] ����� ���� �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		case 18:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[������] �ּ���, ����, ����/�Ƴ��α� ����, ����/��ܰ�����/ELEV������/�����Ⱑ �ִ� ��� üũ�Ͽ� �ֽʽÿ�", this); break;
		}
		
		m_pCheck[nIndex]->Create(IDD_COMMON_CHILD_DIALOG, this);
		m_pCheck[nIndex]->ShowWindow(SW_SHOW);

		m_pCheck[nIndex]->MoveWindow(50, nY, m_pCheck[nIndex]->GetWidthExtent() + 18, 22);
		nY += 22;
	}
	m_btnNext.Create(IDB_BMP_NEXT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
	m_btnNext.MoveWindow(875, 0, 83, 48);

	rect.top += 75;
	rect.left += 50;
	rect.right = 820;
	rect.bottom = rect.top + 75;

	m_pListCtrl->MoveWindow(rect);

	m_bInit = true;;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CCircuitBasicInfoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CCircuitBasicInfoDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CCircuitBasicInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CCircuitBasicInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircuitBasicInfoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircuitBasicInfoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCircuitBasicInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();

	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex]->Invalidate();
	}
	m_btnNext.Invalidate();
	m_btnNext.Invalidate();

	m_pListCtrl->Invalidate();
}

void CCircuitBasicInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CCircuitBasicInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

void CCircuitBasicInfoDlg::OnNextClick()
{
	CString sBD, sStair, sFloor, sBase, sBlock, sBlockName, sToken, sTemp;
	m_pListCtrl->GetItemText(0, 0, sBD);
	m_pListCtrl->GetItemText(0, 1, sBlock);
	m_pListCtrl->GetItemText(0, 2, sBlockName);
	m_pListCtrl->GetItemText(0, 3, sStair);
	m_pListCtrl->GetItemText(0, 4, sFloor);
	m_pListCtrl->GetItemText(0, 5, sBase);
	sBD.Trim();
	sStair.Trim();
	sFloor.Trim();
	sBase.Trim();
	if (sBD.GetLength() == 0) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n�ǹ����� �Է��Ͽ� �ֽʽÿ�.", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (sBlock.GetLength() == 0 || !CCommonFunc::CheckDigit(sBlock.GetBuffer(0))) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n�� ���� �Է��Ͽ� �ֽʽÿ�.\n\n(���ڸ� �Է� �����մϴ�.)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (sBlockName.GetLength() == 0) {// || !CCommonFunc::CheckDigit(sBlockName.GetBuffer(0))) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n�� �̸��� �Է��Ͽ� �ֽʽÿ�.\n\n(�Է¹���: 1 ~ 99999, ��) 101,102,103)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (sStair.GetLength() == 0 || !CCommonFunc::CheckDigit(sStair.GetBuffer(0))) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n��ܼ��� �Է��Ͽ� �ֽʽÿ�.\n\n(���ڸ� �Է� �����մϴ�.)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (sFloor.GetLength() == 0 || !CCommonFunc::CheckDigit(sFloor.GetBuffer(0))) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n�������� �Է��Ͽ� �ֽʽÿ�.\n\n(���ڸ� �Է� �����մϴ�.)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (sBase.GetLength() == 0 || !CCommonFunc::CheckDigit(sBase.GetBuffer(0))) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n�������� �Է��Ͽ� �ֽʽÿ�.\n\n(���ڸ� �Է� �����մϴ�.)", MB_OK, this);
		popup.DoModal();
		return;
	}
	bool bChange = false;
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		if (CCircuitBasicInfo::Instance()->m_bCheck[nIndex] != m_pCheck[nIndex]->GetCheck()) {
			bChange = true;
		}
	}
	int nStair = _ttoi(sStair.GetBuffer());
	int nFloor = _ttoi(sFloor.GetBuffer());
	int nBase = _ttoi(sBase.GetBuffer());
	int nBlock = _ttoi(sBlock.GetBuffer());
	//int nBlockName = _ttoi(sBlockName.GetBuffer());
	if (nBlock < 0 || nBlock > 3) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n�� ������ �ٽ� �Է��Ͽ� �ֽʽÿ�.\n\n(�Է¹���: 0 ~ 3��, 0: ������)", MB_OK, this);
		popup.DoModal();
		return;
	}
	/*if (nBlock > 0 && (nBlockName < 0 || nBlockName > 99999)) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n�� �̸� ������ �ٽ� �Է��Ͽ� �ֽʽÿ�.\n\n(�Է¹���: 1 ~ 99999, ��) 101,102,103)", MB_OK, this);
		popup.DoModal();
		return;
	}*/
	if (nStair < 1 || nStair > 3) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n��� ������ �ٽ� �Է��Ͽ� �ֽʽÿ�.\n\n(�Է¹���: 1 ~ 3���)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (nFloor < 1 || nFloor > 25) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n������ ������ �ٽ� �Է��Ͽ� �ֽʽÿ�.\n\n(�Է¹���: 1 ~ 25��)", MB_OK, this);
		popup.DoModal();
		return;
	}
	if (nBase < 0 || nBase > 5) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n������ ������ �ٽ� �Է��Ͽ� �ֽʽÿ�.\n\n(�Է¹���: 0 ~ 5��)", MB_OK, this);
		popup.DoModal();
		return;
	}
	CStringArray arrayBlock;
	for (int nIndex = 0; nIndex < nBlock; nIndex++) {
		bool bResult = AfxExtractSubString(sToken, sBlockName, nIndex, L',');
		sToken.TrimLeft();
		sToken.TrimRight();
		if (!bResult || sToken.GetLength() == 0) {
			CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n�� �̸� ������ �ٽ� �Է��Ͽ� �ֽʽÿ�.\n\n(�Է¹���: 1 ~ 99999, ��) 101,102,103)", MB_OK, this);
			popup.DoModal();
			return;
		}
		arrayBlock.Add(sToken);
	}
	bool bCompare = true;
	if (arrayBlock.GetCount() == CCircuitBasicInfo::Instance()->m_arrayBlockName.GetCount()) {
		for (int nIndex = 0; nIndex < arrayBlock.GetCount(); nIndex++) {
			if (arrayBlock.GetAt(nIndex) != CCircuitBasicInfo::Instance()->m_arrayBlockName.GetAt(nIndex)) {
				bCompare = false;
				break;
			}
		}
	}
	else {
		bCompare = false;
	}

	if (!m_bFirstWork) {
		//20221011 GBM start
		if (bChange || CCircuitBasicInfo::Instance()->m_sBuildingName != sBD || CCircuitBasicInfo::Instance()->m_nStair != _ttoi(sStair.GetBuffer())
			|| CCircuitBasicInfo::Instance()->m_nFloor != _ttoi(sFloor.GetBuffer()) || CCircuitBasicInfo::Instance()->m_nBasement != _ttoi(sBase.GetBuffer())
			|| CCircuitBasicInfo::Instance()->m_nBlock != _ttoi(sBlock.GetBuffer()) || !bCompare) {
			CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n���ο� [ȸ�� ���� ����]�� �ݿ��Ͻðڽ��ϱ�?\n\n(Ȯ��: ���� ���� / ���: ���� ���)", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				bChange = true;
			}
			else {
				bChange = false;
			}
		}
			//CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n���ο� ������ �ݿ��Ͻðڽ��ϱ�?\n\n(Ȯ��: ���۾� / ���: �����۾��߰�)", MB_YESNO, this);
			//UINT nResult = popup.DoModal();
			//if (nResult == IDOK) {
			//	bChange = true;
			//}
			//else {
			//	bChange = false;
			//	GetParent()->PostMessage(SELECTION_PROJECT, 20, bChange);
			//}
		//20221011 GBM end
	}

	if (m_bFirstWork || bChange) {
		
		//20221013 GBM start - �ǹ� ������ �ѹ� �Է��ϸ� ����
#if 1
		if (!m_bBuildingInfoComplate)
		{
			CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n�ǹ������� �ѹ� �����Ǹ� ������ �Ұ����մϴ�.\n\n�ݿ��Ͻðڽ��ϱ�?\n\n(���Ŀ� [ȸ�� ���� ����]�� ���� ����)", MB_YESNO, this);
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				CCircuitBasicInfo::Instance()->m_sBuildingName = sBD;
				CCircuitBasicInfo::Instance()->m_nStair = nStair;
				CCircuitBasicInfo::Instance()->m_nFloor = nFloor;
				CCircuitBasicInfo::Instance()->m_nBasement = nBase;
				CCircuitBasicInfo::Instance()->m_nBlock = nBlock;
				CCircuitBasicInfo::Instance()->m_sBlockName = sBlockName;

				CCircuitBasicInfo::Instance()->m_arrayBlockName.RemoveAll();
				CCircuitBasicInfo::Instance()->m_arrayBlockName.Copy(arrayBlock);

				m_bBuildingInfoComplate = true;

				m_pListCtrl->SetReadOnly(true);

				//�ǹ� ������ Ȯ���� �Ǹ� ȸ�� ���� �� ������ �ʱ�ȭ �صд�.
				CCommonState::ie()->InitSelectCircuitCompInfo(0);
				CCommonState::ie()->InitSelectCircuitCompInfo(1);
			}
			else {
				return;
			}
		}

#else
		CCircuitBasicInfo::Instance()->m_sBuildingName = sBD;
		CCircuitBasicInfo::Instance()->m_nStair = nStair;
		CCircuitBasicInfo::Instance()->m_nFloor = nFloor;
		CCircuitBasicInfo::Instance()->m_nBasement = nBase;
		CCircuitBasicInfo::Instance()->m_nBlock = nBlock;
		CCircuitBasicInfo::Instance()->m_sBlockName = sBlockName;

		CCircuitBasicInfo::Instance()->m_arrayBlockName.RemoveAll();
		CCircuitBasicInfo::Instance()->m_arrayBlockName.Copy(arrayBlock);
#endif
		//20221013 GBM end

		for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
			CCircuitBasicInfo::Instance()->m_bCheck[nIndex] = m_pCheck[nIndex]->GetCheck();
		}
		m_bFirstWork = false;
		bChange = true;
	}
	else
	{
		//20221014 GBM start - ���� ���� ������ �� üũ�ڽ� ��Ʈ�� ������ ���� �������� ȸ��
		for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
			m_pCheck[nIndex]->SetCheck(CCircuitBasicInfo::Instance()->m_bCheck[nIndex]);
		}
		//20221014 GBM end
	}

	GetParent()->PostMessage(SELECTION_PROJECT, 11, bChange);
}

void CCircuitBasicInfoDlg::InfoInit()
{
	m_pListCtrl->SetItemText(0, 0, L"");
	//20221012 GBM start - �ǹ� �⺻ ���� 1�� �����ؼ� ���� 0�� ��� ������Ʈ ���� �ε� �� ���� ���� �ʱ�ȭ �Ǵ� ���� ����
#if 1
	m_pListCtrl->SetItemText(0, 1, L"1");
#else
	m_pListCtrl->SetItemText(0, 1, L"0");
#endif
	//20221012 GBM end
	m_pListCtrl->SetItemText(0, 2, L"101");
	m_pListCtrl->SetItemText(0, 3, L"1");
	m_pListCtrl->SetItemText(0, 4, L"2");
	m_pListCtrl->SetItemText(0, 5, L"0");

	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex]->SetCheck(false);
	}

	m_bFirstWork = true;
}

void CCircuitBasicInfoDlg::LoadInfo()
{
	CString sTemp;
	m_pListCtrl->SetItemText(0, 0, CCircuitBasicInfo::Instance()->m_sBuildingName);
	sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nBlock);
	m_pListCtrl->SetItemText(0, 1, sTemp);
	//sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nBlockName);
	m_pListCtrl->SetItemText(0, 2, CCircuitBasicInfo::Instance()->m_sBlockName);
	sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nStair);
	m_pListCtrl->SetItemText(0, 3, sTemp);
	sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nFloor);
	m_pListCtrl->SetItemText(0, 4, sTemp);
	sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nBasement);
	m_pListCtrl->SetItemText(0, 5, sTemp);

	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex]->SetCheck(CCircuitBasicInfo::Instance()->m_bCheck[nIndex]);
	}
	m_bFirstWork = false;

	//20221013 GBM start - ������Ʈ ������ �ε�ƴٴ� �� �ǹ� ������ Ȯ���Ǿ��ٴ� �ǹ�
	m_bBuildingInfoComplate = true;		
	m_pListCtrl->SetReadOnly(true);
	//20221013 GBM end

	GetParent()->PostMessage(SELECTION_PROJECT, 11, false);
}

void CCircuitBasicInfoDlg::GetCircuitInfo(CString & sName, int & nStair, int & nFloor, int & nBasement)
{
	m_pListCtrl->GetItemText(0, 0, sName);
	CString sTemp;
	m_pListCtrl->GetItemText(0, 1, sTemp);
	nStair = _ttoi(sTemp.GetBuffer(0));
	m_pListCtrl->GetItemText(0, 2, sTemp);
	nFloor = _ttoi(sTemp.GetBuffer(0));
	m_pListCtrl->GetItemText(0, 3, sTemp);
	nBasement = _ttoi(sTemp.GetBuffer(0));
}

void CCircuitBasicInfoDlg::Redisplay()
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
	rt.SetRect(20, 40, 850, 155);
	CCommonDisplay::DrawRect(&memDC, false, RGB(150, 150, 150), 0, rt);
	rt.SetRect(20, 160, 965, 610);
	CCommonDisplay::DrawRect(&memDC, false, RGB(150, 150, 150), 0, rt);
	rt.SetRect(20, 15, 350, 50);
	CCommonDisplay::DrawCaption(&memDC, L"ȸ�� ���� �Է�", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 48, 350, 80);
	CCommonDisplay::DrawCaption(&memDC, L"�⺻ ���� �Է�", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 160, 350, 190);
	CCommonDisplay::DrawCaption(&memDC, L"ȸ�� ���� ����", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInit) {
		m_btnNext.MoveWindow(rect.right - 87, 0, 83, 48);
	}
}