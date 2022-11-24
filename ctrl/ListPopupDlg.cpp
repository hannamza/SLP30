// ListPopupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "ListPopupDlg.h"
#include "afxdialogex.h"


// CListPopupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CListPopupDlg, CDialogEx)

CListPopupDlg::CListPopupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LIST_POPUP_DIALOG, pParent)
{
	m_pParent = pParent;
	m_pListCtrl = NULL;
	m_nPointIndex = 0;
	m_nLimit = 0;
}

CListPopupDlg::~CListPopupDlg()
{
}

void CListPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
}


BEGIN_MESSAGE_MAP(CListPopupDlg, CDialogEx)
	ON_MESSAGE(WM_LBUTTON_BUTTONLIST_CLICK, OnSelectItem)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_EN_CHANGE(IDC_EDIT_TEXT, &CListPopupDlg::OnEnChangeEditText)
END_MESSAGE_MAP()


// CListPopupDlg 메시지 처리기입니다.


BOOL CListPopupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다
	SetBackgroundColor(RGB(255, 255, 255));

	CRuntimeClass	*pRuntimeClass;
	if (NULL != (pRuntimeClass = RUNTIME_CLASS(CPopupListCtrl))) {
		if (NULL != (m_pListCtrl = (CPopupListCtrl *)(pRuntimeClass->CreateObject()))) {
			m_pListCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_BORDER*/, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
		}
	}
	m_pListCtrl->SendMessage(WM_INITIALUPDATE);

	if (m_nLimit > 0) {
		m_editText.SetLimitText(m_nLimit);
	}

	SetHanMode(m_editText.m_hWnd);
	//SetCapture();
	SetTimer(1, 10, NULL);
	SetTimer(2, 50, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CListPopupDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetAsyncKeyState(VK_CONTROL) && pMsg->wParam == VK_RETURN) {
			m_editText.GetWindowText(m_sText);
			m_sSelText = m_sText;
			m_pParent->PostMessage(INPUT_LIST_INFO, m_nItemIndex, m_nSelCol);
		}
		else if (pMsg->wParam == VK_RETURN) {
			//ReleaseCapture();
			if (m_sSelText.GetLength() == 0) {
				m_editText.GetWindowText(m_sText);
			}
			else {
				m_sText = m_sSelText;
			}
			m_pParent->PostMessage(INPUT_LIST_INFO, m_nItemIndex, m_nSelCol);
		}
		else if (pMsg->wParam == VK_ESCAPE) {
		}
		else if(pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN) {
			if (m_pListCtrl) {
				m_pListCtrl->PreTranslateMessage(pMsg);
			}
		}
		else if(m_nLimit > 0) {
			SetTimer(9, 10, NULL);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CListPopupDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return 0;// CDialogEx::OnEraseBkgnd(pDC);
}

void CListPopupDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();

	m_editText.Invalidate();
	m_pListCtrl->Invalidate();
}

void CListPopupDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (!rect.PtInRect(point)) {
		OnCancel();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CListPopupDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (!rect.PtInRect(point)) {
		OnCancel();
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}

void CListPopupDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (!rect.PtInRect(point)) {
		OnCancel();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CListPopupDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(&rect);
	if (!rect.PtInRect(point)) {
		OnCancel();
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CListPopupDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1) {
		KillTimer(1);
		CString sText;
		CRect rect;
		GetClientRect(&rect);
		m_editText.MoveWindow(7, 8, rect.right - 12, 20);
		/*m_editText.GetWindowText(sText);
		m_editText.SetSel(0, -1);
		m_editText.ReplaceSel(sText);*/

		for (int nIndex = 0; nIndex < m_arrayList.GetCount(); nIndex++) {
			m_pListCtrl->AddItem(nIndex, m_arrayList.GetAt(nIndex));
		}
		
		m_pListCtrl->MoveWindow(5, 33, rect.right - 10, rect.bottom - 38);
		m_pListCtrl->ShowWindow(SW_SHOW);

		m_editText.SetFocus();

		::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE);
	}
	else if (nIDEvent == 2) {
		CWnd* pWnd = GetFocus();
		if (!pWnd) {
			OnCancel();
		}
		else if (pWnd != this && pWnd->m_hWnd != m_editText.m_hWnd) {
			OnCancel();
		}
	}
	else if (nIDEvent == 9) {
		KillTimer(9);
		CString sText;
		m_editText.GetWindowText(sText);
		CStringA sTemp = CCommonFunc::WCharToChar(sText.GetBuffer(0));
		int len = sTemp.GetLength();
		if (len > m_nLimit) {
 			sText = CCommonFunc::CharToWCHAR(sTemp.Left(m_nLimit).GetBuffer(0));
			m_editText.SetWindowText(sText);
			m_editText.SetSel(0, -1);
			m_editText.ReplaceSel(sText);
			m_editText.SetFocus();
			sTemp.ReleaseBuffer();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CListPopupDlg::SetHanMode(HWND hWnd)
{
	HIMC hIMC = ImmGetContext(hWnd);
	DWORD dwConv, dwSent;
	DWORD dwTemp;

	ImmGetConversionStatus(hIMC, &dwConv, &dwSent);

	dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
	dwTemp |= IME_CMODE_NATIVE;
	dwConv = dwTemp;
	ImmSetConversionStatus(hIMC, dwConv, dwSent);
	ImmReleaseContext(hWnd, hIMC);
}

void CListPopupDlg::Redisplay()
{
	CRect rect, rt;
	GetClientRect(&rect);

	CDC* _pDC = GetDC();
	CBitmap bitmap;
	CDC memDC;
	bitmap.CreateCompatibleBitmap(_pDC, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(_pDC);
	memDC.SelectObject(bitmap);

	CCommonDisplay::DrawRect(&memDC, true, RGB(60, 60, 60), RGB(255, 255, 255), rect, 1);
	rt.SetRect(4, 32, rect.right - 4, rect.bottom - 4);
	CCommonDisplay::DrawRect(&memDC, true, RGB(60, 60, 60), RGB(255, 255, 255), rt, 1);
	rt.SetRect(4, 4, rect.right - 4, 29);
	CCommonDisplay::DrawRect(&memDC, true, RGB(60, 60, 60), RGB(255, 255, 255), rt, 1);

	_pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	bitmap.DeleteObject();
	ReleaseDC(_pDC);
	::DeleteObject(memDC);
}

void CListPopupDlg::SetItemIndexInfo(int nItemIndex, int nSelCol)
{
	m_nItemIndex = nItemIndex;
	m_nSelCol = nSelCol;
}

CString CListPopupDlg::GetInputText()
{
	return m_sText;
}

CString CListPopupDlg::GetSelectText()
{
	if (!m_pListCtrl) {
		return L"";
	}
	int nIndex = m_pListCtrl->GetSelectedItemSeq();
	if (nIndex == -1) {
		return L"";
	}
	pListItem pItem = m_pListCtrl->GetItemInfo(nIndex);
	if (!pItem) {
		return L"";
	}
	return pItem->sText;
}

void CListPopupDlg::SetInputEditText(CString sText)
{
	//m_editText.SetWindowText(sText);
}

void CListPopupDlg::AddPopupItem(CString sText)
{
	m_arrayList.Add(sText);
}

void CListPopupDlg::SetTextLimit(int nSize)
{
	m_nLimit = nSize;
}

void CListPopupDlg::OnEnChangeEditText()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sText;
	m_editText.GetWindowText(sText);
	m_pListCtrl->SetSearchWord(sText);
}

LRESULT CListPopupDlg::OnSelectItem(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)lParam;
	if (nIndex == -1) {
		m_sSelText = L"";
		return 0;
	}
	if (m_pListCtrl) {
		pListItem pItem = m_pListCtrl->GetItemInfo(nIndex);
		if (pItem) {
			m_sSelText = pItem->sText;
		}
	}
	int nType = (int)wParam;
	if (nType == 1) { // Mouse 'L' button click
		if (m_sSelText.GetLength() == 0) {
			m_editText.GetWindowText(m_sText);
		}
		else {
			m_sText = m_sSelText;
		}
		m_pParent->PostMessage(INPUT_LIST_INFO, m_nItemIndex, m_nSelCol);
		OnOK();
	}

	return 0;
}
