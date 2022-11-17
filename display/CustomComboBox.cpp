// CustomComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "CustomComboBox.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCustomComboBox

CCustomComboBox::CCustomComboBox()
{
}

CCustomComboBox::~CCustomComboBox()
{
}


BEGIN_MESSAGE_MAP(CCustomComboBox, CComboBox)
	//{{AFX_MSG_MAP(CCustomComboBox)
//	ON_WM_CREATE()
	//}}AFX_MSG_MAP	
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomComboBox message handlers


HBRUSH CCustomComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	//if (nCtlColor == CTLCOLOR_EDIT)
	//{
	//	//pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(RGB(255, 255, 0));
	//	pWnd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_NOSIZE | SWP_FRAMECHANGED);
	//	ModifyStyle(WS_BORDER, 0);
	//	//hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//}
	//if (nCtlColor == CTLCOLOR_LISTBOX)
	//{
	//	//pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(RGB(255, 255, 0));
	//	pWnd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_NOSIZE | SWP_FRAMECHANGED);
	//	ModifyStyle(WS_BORDER, 0);
	//	//hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CCustomComboBox::OnPaint()
{
	CPaintDC dc(this);
	// get client rect to draw in
	CRect rect;
	GetClientRect(rect);
	if ((GetStyle() & 0x0000000f) > CBS_SIMPLE)
	{
		CRect rectBtn = rect;
		rectBtn.left = rectBtn.right - ::GetSystemMetrics(SM_CXHTHUMB);
		BOOL bPressed = FALSE;
		if (GetKeyState(VK_LBUTTON)<0)
		{
			CPoint point;
			::GetCursorPos(&point);
			ScreenToClient(&point);
			if (rectBtn.PtInRect(point))
				bPressed = TRUE;
		}
		// draw button
		int nX = rectBtn.left + (rectBtn.Width() - 17) / 2;
		int nY = rectBtn.top + (rectBtn.Height() - 10) / 2;
		if (nX < rectBtn.left) {
			nX = rectBtn.left;
		}
		if (nY < rectBtn.top) {
			nY = rectBtn.top;
		}
		if (bPressed) {
			nY += 1;
		}
		dc.FillSolidRect(rectBtn, ::GetSysColor(COLOR_WINDOW));
		CCommonDisplay::DrawImage(&dc, IDB_BMP_DOWN, nX, nY);
		//dc.DrawFrameControl(rectBtn, DFC_SCROLL, DFCS_SCROLLDOWN | (bPressed ? DFCS_FLAT | DFCS_PUSHED : 0));
		//dc.FillSolidRect(rectBtn, ::GetSysColor(COLOR_WINDOW));
		rect.right = rectBtn.left;
	}
	if ((GetStyle() & 0x0000000f) == CBS_DROPDOWNLIST)
	{
		CString sText;
		GetWindowText(sText);
		// fill rect if none text is displayed
		if (sText.IsEmpty())
			dc.FillSolidRect(rect, ::GetSysColor(COLOR_WINDOW));
		else {
			dc.FillSolidRect(rect, ::GetSysColor(COLOR_WINDOW));
			dc.DrawText(sText, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else
	{
		// draw border around edit control
		dc.Draw3dRect(rect, ::GetSysColor(COLOR_BTNHIGHLIGHT),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
	}
}
