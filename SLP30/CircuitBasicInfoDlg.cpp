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
#ifndef ENGLISH_MODE
		_T("����")); // lpszFacename 
#else
		_T("arial")); // lpszFacename 
#endif

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
#ifndef ENGLISH_MODE
	const TCHAR* lpszHeaderBasic[] = { _T("�ǹ���"), _T("��"), _T("���̸�"), _T("���"), _T("������"), _T("������"), _T("��ž��"), NULL };
#else
	const TCHAR* lpszHeaderBasic[] = { _T("B.NAME"), _T("B.BLCK"), _T("BLCK.NAME"), _T("LINE"), _T("G.FLOR"), _T("B.FLOR"), _T("R.FLOR"), NULL };
#endif
const int colWidthListBasic[] = { 200, 70, 180, 80, 80, 80, 80, 0 }; // pixel
const int colTypeBasic[] = { POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, POPUP_TYPE_EDIT, 0 };
const int colAlignBasic[] = { DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER, DT_CENTER , -1 }; // option, row align text, default: DT_LEFT
const int colLimitBasic[] = { 0, 6, 0, 6, 6, 6, 6, -1 }; // 0: ������, > 0 : �ؽ�Ʈ ������ ����

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
	m_pListCtrl->SetItemText(0, 1, L"1");
	m_pListCtrl->SetItemText(0, 2, L"101");
	m_pListCtrl->SetItemText(0, 3, L"1");
	m_pListCtrl->SetItemText(0, 4, L"2");
	m_pListCtrl->SetItemText(0, 5, L"0");
	m_pListCtrl->SetItemText(0, 6, L"0");

	int nY = 188;
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		switch (nIndex) {
#ifndef ENGLISH_MODE
		case 0:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[��ȭ��] PBL SET", this); break;
		case 1:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�߽ű�] PBL SET", this); break;
		case 2:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�˶����] ������Ŭ�� �˶����", this); break;
		case 3:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�����׼ǹ��] ������Ŭ�� �����׼ǹ��", this); break;
		case 4:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�ð��溸] �ð��溸 ����", this); break;
		case 5:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[���� ����] ���� ���� �ޱ⼳��", this); break;
		case 6:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[���� ����] ���� ���� ��⼳��", this); break;
		case 7:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�� ����] �� ���� ����", this); break;
		case 8:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[��ȭ ����] ��ȭ ���� ����", this); break;
		case 9:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�迬â] �迬â ����", this); break;
		case 10:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[��ȭ��] ��ȭ�� ����", this); break;
		case 11:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[â�� ��ġ] â�� �����ġ", this); break;
		case 12:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[���� ��ġ] �ڵ� �����ġ", this); break;
		case 13:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[��ȭ�� �⵿ Ȯ��] ��ȭ�� �⵿ ǥ�õ�", this); break;
		case 14:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[���� ���� Ȯ��] ���� ���� Ȯ��", this); break;
		case 15:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[������ ����] ������ ����", this); break;
		case 16:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[���� �ޱ���] �ޱ��� ����", this); break;
		case 17:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[���� �����] ����� ����", this); break;
		case 18:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[������] �Ƴ��α���, �ּ���, �Ϲ���", this); break;
		case 19:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[T/S ����] T/S ����", this); break;
		case 20:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[�ǳ���ٸ�] �ǳ���ٸ�", this); break;
#else
		case 0:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[HYDRANT] PBL SET", this); break;
		case 1:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[MANUAL STATION] PBL SET", this); break;
		case 2:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[ALARM VALVE] ALARM VALVE in Sprinkler Facility", this); break;
		case 3:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[PRE-ACTION VALVE] PRE-ACTION VALVE in Sprinkler Facility", this); break;
		case 4:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[STROBE] STROBE", this); break;
		case 5:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[SMOKE CONTROL(VESTIBULE)] AIR SUPPLY EQUIPMENT", this); break;
		case 6:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[SMOKE CONTROL(VESTIBULE)] SMOKE EXHAUST EQUIPMENT", this); break;
		case 7:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[SMOKE CONTROL(HALL)] SMOKE CONTROL", this); break;
		case 8:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[FIRE SHUTTER] SHUTTER", this); break;
		case 9:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[SMOKE WINDOW] WINDOW", this); break;
		case 10:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[FIRE DOOR] DOOR", this); break;
		case 11:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[WINDOW CLOSER] WINDOW CLOSER", this); break;
		case 12:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[AUTOMATIC DOOR CLOSER] AUTO DOOR CLOSER", this); break;
		case 13:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[HYDRANT] PILOT LAMP", this); break;
		case 14:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[ROOF DOOR ACTION] PILOT LAMP", this); break;
		case 15:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[LOW WATER LEVEL] FLOAT SWITCH", this); break;
		case 16:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[SUPPLY FAN] AIR SUPPLY FAN", this); break;
		case 17:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[EXHAUST FAN] SMOKE EXHAUST FAN", this); break;
		case 18:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[DETECTOR] DETECTOR for ANALOG, ADDRESSABLE, CONVENTIONAL", this); break;
		case 19:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[TAMPER SWITCH] TAMPER SWITCH T/S", this); break;
		case 20:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"[ESCAPE LADDER] ESCAPE LADDER", this); break;
#endif
		}
		
		m_pCheck[nIndex]->Create(IDD_COMMON_CHILD_DIALOG, this);
		m_pCheck[nIndex]->ShowWindow(SW_SHOW);

		m_pCheck[nIndex]->MoveWindow(50, nY, m_pCheck[nIndex]->GetWidthExtent() + 18, 22);
		nY += 22;
	}

#ifndef ENGLISH_MODE
	m_btnNext.Create(IDB_BMP_NEXT, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
#else
	m_btnNext.Create(IDB_BMP_NEXT_EN, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_COMMON_BUTTON1);
#endif
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
	CString sBD, sStair, sFloor, sBase, sRoofTop, sBlock, sBlockName, sToken, sTemp;
	m_pListCtrl->GetItemText(0, 0, sBD);
	m_pListCtrl->GetItemText(0, 1, sBlock);
	m_pListCtrl->GetItemText(0, 2, sBlockName);
	m_pListCtrl->GetItemText(0, 3, sStair);
	m_pListCtrl->GetItemText(0, 4, sFloor);
	m_pListCtrl->GetItemText(0, 5, sBase);
	m_pListCtrl->GetItemText(0, 6, sRoofTop);

	sBD.Trim();
	sStair.Trim();
	sFloor.Trim();
	sBase.Trim();
	if (sBD.GetLength() == 0) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n�ǹ����� �Է��ϼ���.", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Building Name.", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sBlock.GetLength() == 0 || !CCommonFunc::CheckDigit(sBlock.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n�� ������ �Է��ϼ���.\n\n(���ڸ� �Է� ����)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Building Block.\n\n(Only Number)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sBlockName.GetLength() == 0) {// || !CCommonFunc::CheckDigit(sBlockName.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n�� �̸��� �Է��ϼ���.\n\n(��: 101,102,103,��,��,��)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Building Block Name\n\n(Ex: 101,102,103,A,B,C)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sStair.GetLength() == 0 || !CCommonFunc::CheckDigit(sStair.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n��ܼ��� �Է��ϼ���.\n\n(���ڸ� �Է� ����)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Line in Building.\n\n(Only Number)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sFloor.GetLength() == 0 || !CCommonFunc::CheckDigit(sFloor.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n���������� �Է��ϼ���.\n\n(���ڸ� �Է� ����)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Ground Floor.\n\n(Only Number)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sBase.GetLength() == 0 || !CCommonFunc::CheckDigit(sBase.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n���������� �Է��ϼ���.\n\n(���ڸ� �Է� ����)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Basement Floor.\n\n(Only Number)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (sRoofTop.GetLength() == 0 || !CCommonFunc::CheckDigit(sRoofTop.GetBuffer(0))) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n��ž������ �Է��ϼ���.\n\n(���ڸ� �Է� ����)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Rooftop Floor\n\n(Only Number)", MB_OK, this);
#endif
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
	INT nRooftop = _ttoi(sRoofTop.GetBuffer());
	int nBlock = _ttoi(sBlock.GetBuffer());
	//int nBlockName = _ttoi(sBlockName.GetBuffer());
	if (nBlock < 0 || nBlock > 3) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n������ �ٽ� �Է��ϼ���.\n\n(�Է¹���: 1 ~ 3)", MB_OK, this);
		//CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n������ �ٽ� �Է��ϼ���.\n\n(�Է¹���: 0 ~ 3, 0: ������)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Building Block, again.\n\n(Range: 1 ~ 3)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	/*if (nBlock > 0 && (nBlockName < 0 || nBlockName > 99999)) {
		CMessagePopup popup(L"ȸ�� �⺻ �Է�", L"\n\n\n�� �̸� ������ �ٽ� �Է��Ͽ� �ֽʽÿ�.\n\n(�Է¹���: 1 ~ 99999, ��) 101,102,103)", MB_OK, this);
		popup.DoModal();
		return;
	}*/
	if (nStair < 1 || nStair > 3) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n��ܼ��� �ٽ� �Է��ϼ���.\n\n(�Է¹���: 1 ~ 3)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Line in Building, again.\n\n(Range: 1 ~ 3)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (nFloor < 1 || nFloor > 25) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n���������� �ٽ� �Է��ϼ���.\n\n(�Է¹���: 1 ~ 25)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Ground Floor, again.\n\n(Range: 1 ~ 25)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (nBase < 0 || nBase > 5) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n���������� �ٽ� �Է��ϼ���.\n\n(�Է¹���: 0 ~ 5)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Basement Floor.\n\n(Range: 0 ~ 5)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	if (nRooftop < 0 || nRooftop > 1) {
#ifndef ENGLISH_MODE
		CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n��ž������ �ٽ� �Է��ϼ���.\n\n(�Է¹���: 0 ~ 1)", MB_OK, this);
#else
		CMessagePopup popup(L"Building Information", L"\n\n\n\nEnter Number of Rooftop Floor.\n\n(Range: 0 ~ 1)", MB_OK, this);
#endif
		popup.DoModal();
		return;
	}
	CStringArray arrayBlock;
	for (int nIndex = 0; nIndex < nBlock; nIndex++) {
		bool bResult = AfxExtractSubString(sToken, sBlockName, nIndex, L',');
		sToken.TrimLeft();
		sToken.TrimRight();
		if (!bResult || sToken.GetLength() == 0) {
#ifndef ENGLISH_MODE
			CMessagePopup popup(L"�ǹ� ���� �Է�", L"\n\n\n�� �̸��� �ٽ� �Է��ϼ���.\n\n(��: 101,102,103,��,��,��)", MB_OK, this);
#else
			CMessagePopup popup(L"Building Information", L"\n\n\nEnter Building Block Name, again.\n\n(Ex: 101,102,103,A,B,C)", MB_OK, this);
#endif
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
		if (bChange || CCircuitBasicInfo::Instance()->m_sBuildingName != sBD || CCircuitBasicInfo::Instance()->m_nStair != _ttoi(sStair.GetBuffer())
			|| CCircuitBasicInfo::Instance()->m_nFloor != _ttoi(sFloor.GetBuffer()) || CCircuitBasicInfo::Instance()->m_nBasement != _ttoi(sBase.GetBuffer())
			|| CCircuitBasicInfo::Instance()->m_nRooftop != _ttoi(sRoofTop.GetBuffer())
			|| CCircuitBasicInfo::Instance()->m_nBlock != _ttoi(sBlock.GetBuffer()) || !bCompare) {
#ifndef ENGLISH_MODE
			CMessagePopup popup(L"���� ���� ����", L"\n\n\n���ο� [���� ���� ����]�� �����Ͻðڽ��ϱ�?", MB_YESNO, this);
#else
			CMessagePopup popup(L"Equipment info.\nSelection", L"\n\n\n\nDo you apply the new [Equipment Info. Selection]?", MB_YESNO, this);
#endif
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				bChange = true;
			}
			else {
				bChange = false;
			}
		}
	}

	if (m_bFirstWork || bChange) {
		
		//20221013 GBM start - �ǹ� ������ �ѹ� �Է��ϸ� ����
#if 1
		if (!m_bBuildingInfoComplate)
		{
#ifndef ENGLISH_MODE
			CMessagePopup popup(L"�ǹ� ���� Ȯ��", L"\n\n\n�ǹ� ���� Ȯ�� �� ���� �Ұ�����\n\n�����Ͻðڽ��ϱ�?", MB_YESNO, this);
#else
			CMessagePopup popup(L"Confirmation of\nBuilding Information", L"\n\n\n\nBuilding Info. can NOT be changed\nafter confirmation.\n\nDo you apply it?", MB_YESNO, this);
#endif
			UINT nResult = popup.DoModal();
			if (nResult == IDOK) {
				CCircuitBasicInfo::Instance()->m_sBuildingName = sBD;
				CCircuitBasicInfo::Instance()->m_nStair = nStair;
				CCircuitBasicInfo::Instance()->m_nFloor = nFloor;
				CCircuitBasicInfo::Instance()->m_nBasement = nBase;
				CCircuitBasicInfo::Instance()->m_nRooftop = nRooftop;
				CCircuitBasicInfo::Instance()->m_nBlock = nBlock;
				CCircuitBasicInfo::Instance()->m_sBlockName = sBlockName;

				CCircuitBasicInfo::Instance()->m_arrayBlockName.RemoveAll();
				CCircuitBasicInfo::Instance()->m_arrayBlockName.Copy(arrayBlock);

				m_bBuildingInfoComplate = true;

				m_pListCtrl->SetReadOnly(true);

				//�ǹ� ������ Ȯ���� �Ǹ� ȸ�� ���� �� ������ �ʱ�ȭ �صд�.
				CCommonState::ie()->InitSelectCircuitCompInfo(0);
				CCommonState::ie()->InitSelectCircuitCompInfo(1);

				CCommonState::ie()->InitSelectCircuitRepeaterList(0);
				CCommonState::ie()->InitSelectCircuitRepeaterList(1);

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
	sTemp.Format(L"%d", CCircuitBasicInfo::Instance()->m_nRooftop);
	m_pListCtrl->SetItemText(0, 6, sTemp);

	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex]->SetCheck(CCircuitBasicInfo::Instance()->m_bCheck[nIndex]);
	}
	m_bFirstWork = false;

	//20221013 GBM start - ������Ʈ ������ �ε�ƴٴ� �� �ǹ� ������ Ȯ���Ǿ��ٴ� �ǹ�
	m_bBuildingInfoComplate = true;
	m_pListCtrl->SetReadOnly(true);

	//�ǹ� ������ Ȯ���� �Ǹ� ȸ�� ���� �� ������ �ʱ�ȭ �صд�.
	CCommonState::ie()->InitSelectCircuitCompInfo(0);
	CCommonState::ie()->InitSelectCircuitCompInfo(1);

	CCommonState::ie()->InitSelectCircuitRepeaterList(0);
	CCommonState::ie()->InitSelectCircuitRepeaterList(1);

	//20221025 GBM start - ������Ʈ �ε� ùȭ���� ���� ���� ����ȭ���� �ƴ϶� �⺻ ���� ����ȭ���� �������� ����
	//GetParent()->PostMessage(SELECTION_PROJECT, 11, false);
	GetParent()->PostMessage(SELECTION_PROJECT, 0, 0);
	//20221025 GBM end
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
// 	rt.SetRect(20, 160, 965, 610);
// 	CCommonDisplay::DrawRect(&memDC, false, RGB(150, 150, 150), 0, rt);
	rt.SetRect(20, 160, 965, 660);
	CCommonDisplay::DrawRect(&memDC, false, RGB(150, 150, 150), 0, rt);
	rt.SetRect(20, 15, 350, 50);
#ifndef ENGLISH_MODE
	//CCommonDisplay::DrawCaption(&memDC, L"ȸ�� ���� �Է�", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 48, 350, 80);
	CCommonDisplay::DrawCaption(&memDC, L"�ǹ� ���� �Է�", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 160, 350, 190);
	CCommonDisplay::DrawCaption(&memDC, L"���� ���� ����", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
#else
	//CCommonDisplay::DrawCaption(&memDC, L"Circuit Info.", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 48, 350, 80);
	CCommonDisplay::DrawCaption(&memDC, L"Building Info.", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rt.SetRect(50, 160, 350, 190);
	CCommonDisplay::DrawCaption(&memDC, L"Equipment Info. Selection", RGB(80, 80, 80), m_font, false, 0, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
#endif

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_bInit) {
		m_btnNext.MoveWindow(rect.right - 87, 0, 83, 48);
	}
}

bool CCircuitBasicInfoDlg::IsBuildingInfoComplete()
{
	bool bRet = false;
	bRet = m_bBuildingInfoComplate;
	return bRet;
}

void CCircuitBasicInfoDlg::LoadCheckInfoOnly()
{
	for (int nIndex = 0; nIndex < CIRCUIT_PARENT; nIndex++) {
		m_pCheck[nIndex]->SetCheck(CCircuitBasicInfo::Instance()->m_bCheck[nIndex]);
	}
}