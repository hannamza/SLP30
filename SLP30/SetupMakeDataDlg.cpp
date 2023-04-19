// SetupMakeDataDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLP30.h"
#include "SetupMakeDataDlg.h"
#include "afxdialogex.h"


// CSetupMakeDataDlg 대화 상자입니다.

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
		_T("굴림")); // lpszFacename 
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

// CSetupMakeDataDlg 메시지 처리기입니다.

BOOL CSetupMakeDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	for (int nIndex = 0; nIndex < CHECK_COUNT; nIndex++) {
		switch (nIndex) {
		case 연동_경종_전층:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"동별 전층연동", this); break;
		case 연동_경종_계단별직상층:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"계단별 직상층 연동(아파트)", this); break;
		case 연동_경종_동별직상층:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"동별 직상층 연동", this); break;
		case 연동_밸브_교차:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"소화설비용 감지기 교차회로 연동", this); break;
		case 연동_밸브_아날로그:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"소화설비용 감지기 1개 연동", this); break;
		case 연동_싸이렌_작동연동:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"밸브용 감지기 작동시 연동", this); break;
		case 연동_싸이렌_동작확인:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"밸브 동작 확인시 연동", this); break;
		case 연동_문_전층:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"동별 전층 연동", this); break;
		case 연동_문_계단별전층:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"동별 계단별 전층 연동", this); break;
		case 연동_배연창_건물해당층:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"동별 해당층 연동", this); break;
		case 연동_배연창_계단별해당층:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"동별 계단별 해당층 연동", this); break;
		case 연동_급기_건물전층:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"동별 전층 연동", this); break;
		case 연동_급기_계단별전층:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"동별 계단별 전층 연동", this); break;
		case 연동_배기_건물해당층:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"동별 해당층 연동", this); break;
		case 연동_배기_계단별해당층:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"동별 계단별 해당층 연동", this); break;
		case 연동_급기팬_계단별:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"계단별 급기댐퍼 작동시 연동", this); break;
		case 연동_배기팬_계단별:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"계단별 배기댐퍼 작동시 연동", this); break;
		case 연동_셔터_광전1_정온2:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"셔터 연기감지기 작동시 1차 폐쇄 연동, 셔터 열감지기 작동시 2차 폐쇄 연동", this); break;
		case 연동_셔터_1차만:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"셔터 연기, 열 감지기 구분 없이 감지기 작동 시 1차 폐쇄 연동", this); break;
		case 연동_셔터_2차만:m_pCheck[nIndex] = new CCheckTextBtn(nIndex, L"셔터 연기, 열 감지기 구분 없이 감지기 작동 시 2차 폐쇄 연동", this); break;
		}

		m_pCheck[nIndex]->Create(IDD_COMMON_CHILD_DIALOG, this);
		m_pCheck[nIndex]->SetFontSize(13, FW_NORMAL);
		m_pCheck[nIndex]->ChangeEventInfo(true);
		m_pCheck[nIndex]->ShowWindow(SW_SHOW);
	}
	CSaveManager::ie()->m_makeData.data[연동_경종_전층] = 1;
	CSaveManager::ie()->m_makeData.data[연동_밸브_교차] = 1;
	CSaveManager::ie()->m_makeData.data[연동_밸브_아날로그] = 1;
	CSaveManager::ie()->m_makeData.data[연동_싸이렌_작동연동] = 1;
	CSaveManager::ie()->m_makeData.data[연동_싸이렌_동작확인] = 1;
	CSaveManager::ie()->m_makeData.data[연동_문_전층] = 1;
	CSaveManager::ie()->m_makeData.data[연동_배연창_건물해당층] = 1;
	CSaveManager::ie()->m_makeData.data[연동_급기_건물전층] = 1;
	CSaveManager::ie()->m_makeData.data[연동_배기_건물해당층] = 1;
	CSaveManager::ie()->m_makeData.data[연동_급기팬_계단별] = 1;
	CSaveManager::ie()->m_makeData.data[연동_배기팬_계단별] = 1;
	CSaveManager::ie()->m_makeData.data[연동_셔터_광전1_정온2] = 1;

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupMakeDataDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CSetupMakeDataDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;// CDialogEx::OnEraseBkgnd(pDC);
}

void CSetupMakeDataDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CSetupMakeDataDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CSetupMakeDataDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CSetupMakeDataDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Redisplay();
}

void CSetupMakeDataDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Redisplay();
}

void CSetupMakeDataDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CSetupMakeDataDlg::ChangeCheckButtonState(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)wParam;
	bool bState = m_pCheck[nIndex]->GetCheck();
	switch (nIndex) {
	case 연동_경종_전층:
	case 연동_경종_계단별직상층:
	case 연동_경종_동별직상층:
		m_pCheck[연동_경종_전층]->SetCheck(false);
		m_pCheck[연동_경종_계단별직상층]->SetCheck(false);
		m_pCheck[연동_경종_동별직상층]->SetCheck(false);
		m_pCheck[nIndex]->SetCheck(true);
		break;
	case 연동_밸브_교차:
	case 연동_밸브_아날로그:
		m_pCheck[연동_밸브_교차]->SetCheck(true);
		m_pCheck[연동_밸브_아날로그]->SetCheck(true);
		break;
	case 연동_싸이렌_작동연동:
		break;
	case 연동_싸이렌_동작확인:
		m_pCheck[연동_싸이렌_동작확인]->SetCheck(true);
		break;
	case 연동_문_전층:
	case 연동_문_계단별전층:
		m_pCheck[연동_문_전층]->SetCheck(false);
		m_pCheck[연동_문_계단별전층]->SetCheck(false);
		m_pCheck[nIndex]->SetCheck(true);
		break;
	case 연동_배연창_건물해당층:
	case 연동_배연창_계단별해당층:
		m_pCheck[연동_배연창_건물해당층]->SetCheck(false);
		m_pCheck[연동_배연창_계단별해당층]->SetCheck(false);
		m_pCheck[nIndex]->SetCheck(true);
		break;
	case 연동_급기_건물전층:
	case 연동_급기_계단별전층:
		m_pCheck[연동_급기_건물전층]->SetCheck(false);
		m_pCheck[연동_급기_계단별전층]->SetCheck(false);
		m_pCheck[nIndex]->SetCheck(true);
		break;
	case 연동_배기_건물해당층:
	case 연동_배기_계단별해당층:
		m_pCheck[연동_배기_건물해당층]->SetCheck(false);
		m_pCheck[연동_배기_계단별해당층]->SetCheck(false);
		m_pCheck[nIndex]->SetCheck(true);
		break;
	case 연동_급기팬_계단별:
		m_pCheck[연동_급기팬_계단별]->SetCheck(true);
		break;
	case 연동_배기팬_계단별:
		m_pCheck[연동_배기팬_계단별]->SetCheck(true);
		break;
	case 연동_셔터_광전1_정온2:
	case 연동_셔터_1차만:
	case 연동_셔터_2차만:
		m_pCheck[연동_셔터_광전1_정온2]->SetCheck(false);
		m_pCheck[연동_셔터_1차만]->SetCheck(false);
		m_pCheck[연동_셔터_2차만]->SetCheck(false);
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
			sTemp = L"경종";
			rtemp.SetRect(rt.left + 10, rt.top + 3, rt.left + 150, rt.top + 8);
			rtemp1.left = rtemp.right - 1;
			rtemp1.right = rtemp1.left + 808;
			rtemp2.CopyRect(rtemp);
			rtemp2.left = rtemp.right + 10;
			break;
		case 1:	sTemp = L"시각경보"; rtemp1.top = rtemp.top; break;
		case 2:	sTemp = L"비상방송"; break;
		case 3: sTemp = L""; rtemp1.bottom = rtemp.bottom; nTerm = nSpace; break;
		case 4: sTemp = L"밸브"; nTerm = (nDefault * 2); bRect = true; break;
		case 5: sTemp = L"싸이렌"; nTerm = (nDefault * 2); bRect = true; break;
		case 6: sTemp = L""; nTerm = nSpace; break;
		case 7: sTemp = L"자동 폐쇄"; break;
		case 8: sTemp = L"창문 폐쇄"; rtemp1.top = rtemp.top; break;
		case 9: sTemp = L"옥상 개폐"; break;
		case 10: sTemp = L"로비폰"; break;
		case 11: sTemp = L"자동문"; break;
		case 12: sTemp = L"비상등"; break;
		case 13: sTemp = L""; rtemp1.bottom = rtemp.bottom; nTerm = nSpace; break;
		case 14: sTemp = L"배연창"; nTerm = (nDefault * 2); bRect = true; break;
		case 15: sTemp = L""; nTerm = nSpace; break;
		case 16: sTemp = L"전실 급기댐퍼"; nTerm = (nDefault * 2); bRect = true; break;
		case 17: sTemp = L"전실 배기댐퍼"; nTerm = (nDefault * 2); bRect = true; break;
		case 18: sTemp = L"급기팬"; bRect = true; break;
		case 19: sTemp = L"배기팬"; bRect = true; break;
		case 20: sTemp = L""; rtemp1.bottom = rtemp.bottom; nTerm = nSpace; break;
		case 21: sTemp = L"셔터 1차 폐쇄"; nTerm = (nDefault * 2); break;
		case 22: sTemp = L"셔터 2차 폐쇄"; nTerm = (nDefault * 2); rtemp1.top = rtemp.top; break;
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