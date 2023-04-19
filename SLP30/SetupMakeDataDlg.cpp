// SetupMakeDataDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SLP30.h"
#include "SetupMakeDataDlg.h"
#include "afxdialogex.h"


// CSetupMakeDataDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSetupMakeDataDlg, CDialogEx)

CSetupMakeDataDlg::CSetupMakeDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMON_CHILD_DIALOG, pParent)
{
	m_bInit = false;

	m_font.CreateFont(13, // nHeight 
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

CSetupMakeDataDlg::~CSetupMakeDataDlg()
{
	for (int nIndex = 0; nIndex < CHECK_COUNT; nIndex++) {
		SAFE_DELETE(m_pCheck[nIndex]);
	}
}

void CSetupMakeDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSetupMakeDataDlg, CDialogEx)
	ON_MESSAGE(CHECK_BUTTON_CLICK, ChangeCheckButtonState)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSetupMakeDataDlg �޽��� ó�����Դϴ�.

BOOL CSetupMakeDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	for (int nIndex = 0; nIndex < CHECK_COUNT; nIndex++) {
		switch (nIndex) {
		case ����_����_����:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ��������", this); break;
		case ����_����_��ܺ�������:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"��ܺ� ������ ����(����Ʈ)", this); break;
		case ����_����_����������:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ������ ����", this); break;
		case ����_���_����:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"��ȭ����� ������ ����ȸ�� ����", this); break;
		case ����_���_�Ƴ��α�:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"��ȭ����� ������ 1�� ����", this); break;
		case ����_���̷�_�۵�����:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ������ �۵��� ����", this); break;
		case ����_���̷�_����Ȯ��:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"��� ���� Ȯ�ν� ����", this); break;
		case ����_��_����:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ���� ����", this); break;
		case ����_��_��ܺ�����:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ��ܺ� ���� ����", this); break;
		case ����_�迬â_�ǹ��ش���:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� �ش��� ����", this); break;
		case ����_�迬â_��ܺ��ش���:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ��ܺ� �ش��� ����", this); break;
		case ����_�ޱ�_�ǹ�����:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ���� ����", this); break;
		case ����_�ޱ�_��ܺ�����:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ��ܺ� ���� ����", this); break;
		case ����_���_�ǹ��ش���:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� �ش��� ����", this); break;
		case ����_���_��ܺ��ش���:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ��ܺ� �ش��� ����", this); break;
		case ����_�ޱ���_��ܺ�:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"��ܺ� �ޱ���� �۵��� ����", this); break;
		case ����_�����_��ܺ�:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"��ܺ� ������ �۵��� ����", this); break;
		case ����_����_����1_����2:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ���Ⱘ���� �۵��� 1�� ��� ����, ���� �������� �۵��� 2�� ��� ����", this); break;
		case ����_����_1����:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ����, �� ������ ���� ���� ������ �۵� �� 1�� ��� ����", this); break;
		case ����_����_2����:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"���� ����, �� ������ ���� ���� ������ �۵� �� 2�� ��� ����", this); break;
		}

		m_pCheck[nIndex]->Create(IDD_COMMON_CHILD_DIALOG, this);
		m_pCheck[nIndex]->SetFontSize(13, FW_NORMAL);
		m_pCheck[nIndex]->ChangeEventInfo(true);
		m_pCheck[nIndex]->ShowWindow(SW_SHOW);
	}
	CSaveManager::ie()->m_makeData.data[����_����_����] = 1;
	CSaveManager::ie()->m_makeData.data[����_���_����] = 1;
	CSaveManager::ie()->m_makeData.data[����_���_�Ƴ��α�] = 1;
	CSaveManager::ie()->m_makeData.data[����_���̷�_�۵�����] = 1;
	CSaveManager::ie()->m_makeData.data[����_���̷�_����Ȯ��] = 1;
	CSaveManager::ie()->m_makeData.data[����_��_����] = 1;
	CSaveManager::ie()->m_makeData.data[����_�迬â_�ǹ��ش���] = 1;
	CSaveManager::ie()->m_makeData.data[����_�ޱ�_�ǹ�����] = 1;
	CSaveManager::ie()->m_makeData.data[����_���_�ǹ��ش���] = 1;
	CSaveManager::ie()->m_makeData.data[����_�ޱ���_��ܺ�] = 1;
	CSaveManager::ie()->m_makeData.data[����_�����_��ܺ�] = 1;
	CSaveManager::ie()->m_makeData.data[����_����_����1_����2] = 1;

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CSetupMakeDataDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CSetupMakeDataDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CSetupMakeDataDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CSetupMakeDataDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CSetupMakeDataDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CSetupMakeDataDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CSetupMakeDataDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Redisplay();
}

void CSetupMakeDataDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Redisplay();
}

void CSetupMakeDataDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CSetupMakeDataDlg::ChangeCheckButtonState(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)wParam;
	bool bState = m_pCheck[nIndex]->GetCheck();
	switch (nIndex) {
	case ����_����_����:
	case ����_����_��ܺ�������:
	case ����_����_����������:
		m_pCheck[����_����_����]->SetCheck(false);
		m_pCheck[����_����_��ܺ�������]->SetCheck(false);
		m_pCheck[����_����_����������]->SetCheck(false);
		m_pCheck[nIndex]->SetCheck(true);
		break;
	case ����_���_����:
	case ����_���_�Ƴ��α�:
		m_pCheck[����_���_����]->SetCheck(true);
		m_pCheck[����_���_�Ƴ��α�]->SetCheck(true);
		break;
	case ����_���̷�_�۵�����:
		break;
	case ����_���̷�_����Ȯ��:
		m_pCheck[����_���̷�_����Ȯ��]->SetCheck(true);
		break;
	case ����_��_����:
	case ����_��_��ܺ�����:
		m_pCheck[����_��_����]->SetCheck(false);
		m_pCheck[����_��_��ܺ�����]->SetCheck(false);
		m_pCheck[nIndex]->SetCheck(true);
		break;
	case ����_�迬â_�ǹ��ش���:
	case ����_�迬â_��ܺ��ش���:
		m_pCheck[����_�迬â_�ǹ��ش���]->SetCheck(false);
		m_pCheck[����_�迬â_��ܺ��ش���]->SetCheck(false);
		m_pCheck[nIndex]->SetCheck(true);
		break;
	case ����_�ޱ�_�ǹ�����:
	case ����_�ޱ�_��ܺ�����:
		m_pCheck[����_�ޱ�_�ǹ�����]->SetCheck(false);
		m_pCheck[����_�ޱ�_��ܺ�����]->SetCheck(false);
		m_pCheck[nIndex]->SetCheck(true);
		break;
	case ����_���_�ǹ��ش���:
	case ����_���_��ܺ��ش���:
		m_pCheck[����_���_�ǹ��ش���]->SetCheck(false);
		m_pCheck[����_���_��ܺ��ش���]->SetCheck(false);
		m_pCheck[nIndex]->SetCheck(true);
		break;
	case ����_�ޱ���_��ܺ�:
		m_pCheck[����_�ޱ���_��ܺ�]->SetCheck(true);
		break;
	case ����_�����_��ܺ�:
		m_pCheck[����_�����_��ܺ�]->SetCheck(true);
		break;
	case ����_����_����1_����2:
	case ����_����_1����:
	case ����_����_2����:
		m_pCheck[����_����_����1_����2]->SetCheck(false);
		m_pCheck[����_����_1����]->SetCheck(false);
		m_pCheck[����_����_2����]->SetCheck(false);
		m_pCheck[nIndex]->SetCheck(true);
		break;
	}
	return 0;
}

void CSetupMakeDataDlg::SaveData()
{
	for (int nIndex = 0; nIndex < CHECK_COUNT; nIndex++) {
		CSaveManager::ie()->m_makeData.data[nIndex] = m_pCheck[nIndex]->GetCheck();
	}
}

void CSetupMakeDataDlg::LoadData()
{
	for (int nIndex = 0; nIndex < CHECK_COUNT; nIndex++) {
		m_pCheck[nIndex]->SetCheck(CSaveManager::ie()->m_makeData.data[nIndex]);
	}
}

void CSetupMakeDataDlg::InitData()
{
	for (int nIndex = 0; nIndex < CHECK_COUNT; nIndex++) {
		m_pCheck[nIndex]->SetCheck(CSaveManager::ie()->m_makeData.data[nIndex]);
	}
}

void CSetupMakeDataDlg::Redisplay()
{
	if (!m_bInit) {
		return;
	}
	CRect rect, rt, rtemp, rtemp1, rtemp2;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawRect(&memDC, true, RGB(255, 255, 255), RGB(255, 255, 255), rect);

	CString sTemp;
	int nTerm = 21, nDefault = 21, nMax = (4*nDefault)/3, nSpace = 8;
	int nCheckIndex = 0;
	bool bRect = false;
	rt.CopyRect(&rect);
	for (int nIndex = 0; nIndex < 24; nIndex++) {
		nTerm = nDefault;
		bRect = false;
		switch (nIndex) {
		case 0:
			sTemp = L"����";
			rtemp.SetRect(rt.left + 10, rt.top + 3, rt.left + 150, rt.top + 8);
			rtemp1.left = rtemp.right - 1;
			rtemp1.right = rtemp1.left + 808;
			rtemp2.CopyRect(rtemp);
			rtemp2.left = rtemp.right + 10;
			break;
		case 1:	sTemp = L"�ð��溸"; rtemp1.top = rtemp.top; break;
		case 2:	sTemp = L"�����"; break;
		case 3: sTemp = L""; rtemp1.bottom = rtemp.bottom; nTerm = nSpace; break;
		case 4: sTemp = L"���"; nTerm = (nDefault * 2); bRect = true; break;
		case 5: sTemp = L"���̷�"; nTerm = (nDefault * 2); bRect = true; break;
		case 6: sTemp = L""; nTerm = nSpace; break;
		case 7: sTemp = L"�ڵ� ���"; break;
		case 8: sTemp = L"â�� ���"; rtemp1.top = rtemp.top; break;
		case 9: sTemp = L"���� ����"; break;
		case 10: sTemp = L"�κ���"; break;
		case 11: sTemp = L"�ڵ���"; break;
		case 12: sTemp = L"����"; break;
		case 13: sTemp = L""; rtemp1.bottom = rtemp.bottom; nTerm = nSpace; break;
		case 14: sTemp = L"�迬â"; nTerm = (nDefault * 2); bRect = true; break;
		case 15: sTemp = L""; nTerm = nSpace; break;
		case 16: sTemp = L"���� �ޱ����"; nTerm = (nDefault * 2); bRect = true; break;
		case 17: sTemp = L"���� ������"; nTerm = (nDefault * 2); bRect = true; break;
		case 18: sTemp = L"�ޱ���"; bRect = true; break;
		case 19: sTemp = L"�����"; bRect = true; break;
		case 20: sTemp = L""; rtemp1.bottom = rtemp.bottom; nTerm = nSpace; break;
		case 21: sTemp = L"���� 1�� ���"; nTerm = (nDefault * 2); break;
		case 22: sTemp = L"���� 2�� ���"; nTerm = (nDefault * 2); rtemp1.top = rtemp.top; break;
		case 23: sTemp = L""; rtemp1.bottom = rtemp.bottom; nTerm = nSpace; break;
		default: break;
		}
		rtemp.top = rtemp.bottom - 1;
		rtemp.bottom += nTerm - 1;
		if (bRect) {
			rtemp1.top = rtemp.top;
			rtemp1.bottom = rtemp.bottom;
		}
		if (sTemp.GetLength() > 0)
		{
			if (nCheckIndex < CHECK_COUNT) {
				if (nIndex == 4 || nIndex == 5 || nIndex == 14 || nIndex == 16 || nIndex == 17) {
					rtemp2.top = rtemp.top + 1;
					rtemp2.bottom = rtemp.bottom - 1 - nDefault;
					rtemp2.right = rtemp2.left + m_pCheck[nCheckIndex]->GetWidthExtent() + 22;
					m_pCheck[nCheckIndex]->MoveWindow(rtemp2);
					++nCheckIndex;
					rtemp2.top = rtemp.top + 1 + nDefault;
					rtemp2.bottom = rtemp.bottom - 1;
					rtemp2.right = rtemp2.left + m_pCheck[nCheckIndex]->GetWidthExtent() + 22;
					m_pCheck[nCheckIndex]->MoveWindow(rtemp2);
					++nCheckIndex;
				}
				else if (nIndex == 21) {
					rtemp2.top = rtemp.top + 1;
					rtemp2.bottom = rtemp2.top + nMax - 1;
					rtemp2.right = rtemp2.left + m_pCheck[nCheckIndex]->GetWidthExtent() + 22;
					m_pCheck[nCheckIndex]->MoveWindow(rtemp2);
					++nCheckIndex;
					rtemp2.top += nMax-1;
					rtemp2.bottom = rtemp2.top + nMax - 1;
					rtemp2.right = rtemp2.left + m_pCheck[nCheckIndex]->GetWidthExtent() + 22;
					m_pCheck[nCheckIndex]->MoveWindow(rtemp2);
					++nCheckIndex;
					rtemp2.top += nMax-1;
					rtemp2.bottom = rtemp2.top + nMax - 1;
					rtemp2.right = rtemp2.left + m_pCheck[nCheckIndex]->GetWidthExtent() + 22;
					m_pCheck[nCheckIndex]->MoveWindow(rtemp2);
					++nCheckIndex;
				}
				else if (nIndex == 7) {
					rtemp2.top = rtemp.top + 1 + nTerm + 15;
					rtemp2.bottom = rtemp2.top + 22;//rtemp.bottom - 1 + (4 * nTerm) - 13;
					rtemp2.right = rtemp2.left + m_pCheck[nCheckIndex]->GetWidthExtent() + 22;
					m_pCheck[nCheckIndex]->MoveWindow(rtemp2);
					++nCheckIndex;
					rtemp2.top = rtemp.top + 1 + nTerm + 39;
					rtemp2.bottom = rtemp2.top + 22;//rtemp.bottom - 1 + (4 * nTerm) + 13;
					rtemp2.right = rtemp2.left + m_pCheck[nCheckIndex]->GetWidthExtent() + 22;
					m_pCheck[nCheckIndex]->MoveWindow(rtemp2);
					++nCheckIndex;
				}
				else if (nIndex > 7 && nIndex <= 13) {

				}
				else {
					rtemp2.top = rtemp.top + 1;
					rtemp2.bottom = rtemp.bottom - 1;

					rtemp2.right = rtemp2.left + m_pCheck[nCheckIndex]->GetWidthExtent() + 22;
					m_pCheck[nCheckIndex]->MoveWindow(rtemp2);
					++nCheckIndex;
				}
			}
			CCommonDisplay::DrawRect(&memDC, false, RGB(220, 220, 220), 0, rtemp);
			CCommonDisplay::DrawCaption(&memDC, sTemp, RGB(80, 80, 80), m_font, false, 0, rtemp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		if(bRect || sTemp.GetLength() == 0) {
			CCommonDisplay::DrawRect(&memDC, false, RGB(220, 220, 220), 0, rtemp1);
		}
	}
	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);
}