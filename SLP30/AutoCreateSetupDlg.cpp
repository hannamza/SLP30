// AutoCreateSetupDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "AutoCreateSetupDlg.h"
#include "afxdialogex.h"
#include "MessagePopup.h"


// CAutoCreateSetupDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAutoCreateSetupDlg, CDialogEx)

CAutoCreateSetupDlg::CAutoCreateSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_pListCtrl = NULL;
	m_pRadioBtn1 = NULL;
	m_pRadioBtn2 = NULL;
	m_pRadioBtn3 = NULL;
	m_pRadioBtn4 = NULL;
}

CAutoCreateSetupDlg::~CAutoCreateSetupDlg()
{
	SAFE_DELETE(m_pRadioBtn1);
	SAFE_DELETE(m_pRadioBtn2);
	SAFE_DELETE(m_pRadioBtn3);
	SAFE_DELETE(m_pRadioBtn4);
}

void CAutoCreateSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAutoCreateSetupDlg, CDialogEx)
	ON_MESSAGE(RADIO_BUTTON_CLICK, OnRadioButtnClick)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(LIST_CONTROL, OnListControl)
END_MESSAGE_MAP()

// CAutoCreateSetupDlg �޽��� ó�����Դϴ�.

const TCHAR* lpszHeader[] = { _T("NO"), _T("�̸�"), _T("��ȭ\n��ȣ"), _T("����"), _T("��� ����"), _T("���"), NULL };
const int colWidthList[] = { 100, 150, 150, 100, 200, 250, 0 }; // pixel
const int colType[] = { POPUP_TYPE_SEQ, POPUP_TYPE_LIST_ADD , POPUP_TYPE_LIST_NOINPUT, POPUP_TYPE_SPACE, POPUP_TYPE_TEXT, POPUP_TYPE_EDIT, 0 };
const int colAlign[] = { DT_CENTER, DT_LEFT, DT_RIGHT, DT_CENTER, DT_LEFT, DT_CENTER, -1 }; // option, row align text, default: DT_LEFT
const int colLimit[] = { 0, 10, 0, 0, 40, 10, -1 }; // 0: ������, > 0 : �ؽ�Ʈ ������ ����

BOOL CAutoCreateSetupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CRect rect;
	GetClientRect(rect);

	CString sCaption = L"������/������ ����";
	m_pRadioBtn1 = new CRadioTextBtn(0, sCaption, this);
	m_pRadioBtn1->Create(IDD_COMMON_CHILD_DIALOG, this);
	int nX = 40;
	m_pRadioBtn1->MoveWindow(nX, 100, m_pRadioBtn1->GetWidthExtent(sCaption), 16);
	nX += m_pRadioBtn1->GetWidthExtent(sCaption) + 15;
	m_pRadioBtn1->ShowWindow(SW_SHOW);

	sCaption = L"�ڵ� �� ����";
	m_pRadioBtn2 = new CRadioTextBtn(1, sCaption, this);
	m_pRadioBtn2->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pRadioBtn2->MoveWindow(nX, 100, m_pRadioBtn2->GetWidthExtent(sCaption), 16);
	nX += m_pRadioBtn2->GetWidthExtent(sCaption) + 15;
	m_pRadioBtn2->ShowWindow(SW_SHOW);

	sCaption = L"��� ����";
	m_pRadioBtn3 = new CRadioTextBtn(2, sCaption, this);
	m_pRadioBtn3->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pRadioBtn3->MoveWindow(nX, 100, m_pRadioBtn3->GetWidthExtent(sCaption), 16);
	nX += m_pRadioBtn3->GetWidthExtent(sCaption) + 15;
	m_pRadioBtn3->ShowWindow(SW_SHOW);

	sCaption = L"���� �� ��";
	m_pRadioBtn4 = new CRadioTextBtn(3, sCaption, this);
	m_pRadioBtn4->Create(IDD_COMMON_CHILD_DIALOG, this);
	m_pRadioBtn4->MoveWindow(nX, 100, m_pRadioBtn4->GetWidthExtent(sCaption), 16);
	nX += m_pRadioBtn4->GetWidthExtent(sCaption) + 15;
	m_pRadioBtn4->ShowWindow(SW_SHOW);

	CRuntimeClass	*pRuntimeClass;
	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CCustomListCtrl))) {
		if (NULL != (m_pListCtrl = (CCustomListCtrl *)(pRuntimeClass->CreateObject()))) {
			m_pListCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}
	m_pListCtrl->SetListIndex(0);
	m_pListCtrl->SetHeaderInfo((TCHAR**)lpszHeader, (int*)colWidthList, (int*)colType, (int*)colAlign); // Align: option, row align text, default: DT_LEFT
	m_pListCtrl->SetColumnLimit((int*)colLimit);
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

	// ��Ʈ ���� (option)
	/*m_pListCtrl->SetRowFontSize(20);
	m_pListCtrl->SetRowFontBold(FW_BOLD);
	m_pListCtrl->SetRowFontName(L"����");
	m_pListCtrl->SetHeaderFontSize(20);
	m_pListCtrl->SetHeaderFontBold(FW_BOLD);
	m_pListCtrl->SetHeaderFontName(L"����");*/
	// ����Ʈ �˾� ������ �߰�
	m_pListCtrl->AddPopupListItem(2, L"���� ������");
	m_pListCtrl->AddPopupListItem(2, L"B1F ������");
	m_pListCtrl->AddPopupListItem(2, L"B2F ������");
	m_pListCtrl->AddPopupListItem(2, L"B3F ������");
	m_pListCtrl->AddPopupListItem(2, L"B4F ������");
	m_pListCtrl->AddPopupListItem(2, L"B5F ������");
	m_pListCtrl->AddPopupListItem(2, L"B6F ������");
	m_pListCtrl->AddPopupListItem(2, L"B7F ������");
	m_pListCtrl->AddPopupListItem(2, L"B8F ������");
	m_pListCtrl->AddPopupListItem(2, L"B9F ������");
	m_pListCtrl->AddPopupListItem(2, L"B10F ������");
	m_pListCtrl->AddPopupListItem(2, L"B11F ������");
	m_pListCtrl->AddPopupListItem(2, L"B12F ������");
	m_pListCtrl->AddPopupListItem(2, L"B13F ������");
	m_pListCtrl->AddPopupListItem(2, L"B14F ������");
	m_pListCtrl->AddPopupListItem(2, L"B15F ������");
	m_pListCtrl->AddPopupListItem(2, L"B16F ������");
	m_pListCtrl->AddPopupListItem(2, L"B17F ������");
	m_pListCtrl->AddPopupListItem(2, L"B18F ������");
	m_pListCtrl->AddPopupListItem(2, L"B19F ������");
	m_pListCtrl->AddPopupListItem(2, L"B20F ������");
	m_pListCtrl->AddPopupListItem(2, L"B21F ������");
	m_pListCtrl->AddPopupListItem(2, L"B22F ������");

	m_pListCtrl->AddPopupListItem(1, L"������");
	m_pListCtrl->AddPopupListItem(1, L"�ֺҾ�");
	m_pListCtrl->AddPopupListItem(1, L"�۰�ȣ");
	m_pListCtrl->AddPopupListItem(1, L"������");
	m_pListCtrl->AddPopupListItem(1, L"������");
	m_pListCtrl->AddPopupListItem(1, L"���缮");
	m_pListCtrl->AddPopupListItem(1, L"��ȣ��");
	m_pListCtrl->AddPopupListItem(1, L"������");
	m_pListCtrl->AddPopupListItem(1, L"Ȳ�μ�");
	m_pListCtrl->AddPopupListItem(1, L"��ȣ");
	m_pListCtrl->AddPopupListItem(1, L"����ȭ");
	m_pListCtrl->AddPopupListItem(1, L"�̳���");

	m_pListCtrl->AddListItemState(3, L"Ȱ������");
	m_pListCtrl->AddListItemState(3, L"��Ȱ������");
	m_pListCtrl->AddListItemState(3, L"������");

	m_pListCtrl->SendMessage(WM_INITIALUPDATE);

	rect.top = 100;
	rect.bottom -= 100;

	m_pListCtrl->MoveWindow(rect);
	m_pListCtrl->ShowWindow(SW_SHOW);

	// test code
	CString sText;
	m_pListCtrl->AddItem(255);
	for (int nIndex = 0; nIndex < 255; nIndex++) {
		sText.Format(L"ȫ�浿 %d", nIndex + 1);
		m_pListCtrl->SetItemText(nIndex, 1, sText);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CAutoCreateSetupDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CAutoCreateSetupDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CAutoCreateSetupDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CAutoCreateSetupDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rectES;
	CRect moveableRect;
	GetClientRect(&moveableRect);

	rectES.SetRect(moveableRect.right - 20, moveableRect.bottom - 20, moveableRect.right, moveableRect.bottom);
	if (rectES.PtInRect(point)) {
		GetParent()->SendMessage(WM_MOUSEMOVE, nFlags, 0);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CAutoCreateSetupDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CAutoCreateSetupDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rectES;
	CRect moveableRect;
	GetClientRect(&moveableRect);

	rectES.SetRect(moveableRect.right - 20, moveableRect.bottom - 20, moveableRect.right, moveableRect.bottom);
	if (rectES.PtInRect(point)) {
		GetParent()->SendMessage(WM_MOUSEMOVE, 0, 0);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CAutoCreateSetupDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();

	m_pRadioBtn1->Invalidate();
	m_pRadioBtn2->Invalidate();
	m_pRadioBtn3->Invalidate();
	m_pRadioBtn4->Invalidate();
}

void CAutoCreateSetupDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (!IsIconic()) {
		Redisplay();
	}
}

void CAutoCreateSetupDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CAutoCreateSetupDlg::OnRadioButtnClick(WPARAM wParam, LPARAM lParam)
{
	int nIndex = wParam;

	m_pRadioBtn1->SetCheck(false);
	m_pRadioBtn2->SetCheck(false);
	m_pRadioBtn3->SetCheck(false);
	m_pRadioBtn4->SetCheck(false);
	switch (nIndex) {
	case 0: m_pRadioBtn1->SetCheck(true); break;
	case 1: m_pRadioBtn2->SetCheck(true); break;
	case 2: m_pRadioBtn3->SetCheck(true); break;
	case 3: m_pRadioBtn4->SetCheck(true); break;
	default: break;
	}

	return 0;
}

LRESULT CAutoCreateSetupDlg::OnListControl(WPARAM wParam, LPARAM lParam)
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
			}
		}
		break;
	default:
		break;

	}
	return 0;
}

void CAutoCreateSetupDlg::Redisplay()
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
	CCommonDisplay::DrawRect(&memDC, true, RGB(120, 120, 120), RGB(238, 238, 238), rt);

	rt.top += 17;
	rt.left += 15;
	rt.right -= 15;
	rt.bottom -= 15;
	CCommonDisplay::DrawRect(&memDC, true, RGB(255, 255, 255), RGB(255, 255, 255), rt);

	CCommonDisplay::DrawImage(&memDC, IDB_BMP_BOX_LT, rt.left, rt.top);
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_BOX_RT, rt.right - 13, rt.top);
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_BOX_LB, rt.left, rt.bottom - 13);
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_BOX_RB, rt.right - 13, rt.bottom - 13);

	CCommonDisplay::DrawLine(&memDC, rt.left, rt.top + 53, rt.right, rt.top + 53, PS_SOLID, 2, RGB(211, 122, 56));
	CCommonDisplay::DrawImage(&memDC, IDB_BMP_TITLEBAR, rt.left + 40, rt.top + 48);

	CCommonDisplay::DrawImage(&memDC, IDB_BMP_AUTOTITLE, 60, 30);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);

	if (m_pListCtrl) {
		rt.top += 150;
		rt.bottom -= 20;
		rt.left += 10;
		rt.right -= 10;

		m_pListCtrl->MoveWindow(rt);
		m_pListCtrl->ShowWindow(SW_SHOW);
		m_pListCtrl->Invalidate();
	}
}
