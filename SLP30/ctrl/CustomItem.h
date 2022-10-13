#pragma once

#include <afxtempl.h>
#include "afxwin.h"
// CCustomItem ��ȭ �����Դϴ�.

#define		WM_LBUTTON_BUTTONLIST_CLICK		(WM_USER+1004)
#define		WM_RBUTTON_BUTTONLIST_CLICK		(WM_USER+1005)
#define		WM_RESIZE_COLUMN				(WM_USER+1006)
#define		WM_PAGE_MOVE_KEY				(WM_USER+1007)
#define		WM_MOUSE_CLICK					(WM_USER+1008)

#define		LIST_ITEM_HEIGHT				35

enum ITEM_TYPE
{
	POPUP_TYPE_NONE = 0,	// ����� �Է°��� ǥ��
	POPUP_TYPE_SEQ,			// row sequential ���� ǥ��
	POPUP_TYPE_SPACE,		// �����̽��� ���� ��ȯ
	POPUP_TYPE_EDIT,		// ����Ʈ �ڽ�
	POPUP_TYPE_TEXT,		// ���� �� �ؽ�Ʈ �Է� â �˾�(�ؽ�Ʈ Ÿ���� �Է�)
	POPUP_TYPE_LIST,		// ���� �� ����Ʈ ����/�Է� â �˾�(�ؽ�Ʈ Ÿ���� �Է� ����)
	POPUP_TYPE_LIST_ADD,	// ���� �� ����Ʈ ����/�Է� â �˾�(�ؽ�Ʈ Ÿ���� �Է� �� ���ο� �Է� �� �߰�)
	POPUP_TYPE_LIST_NOINPUT	// ���� �� ����Ʈ ����/�Է� â �˾�(�˻� ����, Ÿ���� �Է� �Ұ�) dropdown list
};

typedef struct STATE_STRING
{
	int nColumn;
	CStringArray value;
}stateString, *pStateString;

typedef struct ITEM_VALUE
{
	ITEM_VALUE() {
		nColIndex = -1;
		sText = L"";
	}
	int nColIndex;
	CString sText;
}itemValue, *pItemValue;

class CCustomItem : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomItem)

public:
	CCustomItem(CWnd* pParent);   // ǥ�� �������Դϴ�.
	virtual ~CCustomItem();

public:
	void SetItemIndex(int nIndex);
	int  GetItemIndex();
	void SetSelectedState(bool bState, int nCol);
	bool GetSelectedState();
	bool SetItemText(CList<pItemValue, pItemValue> & listText);
	void SetRowColor(COLORREF bkColor, COLORREF bkSelColor, COLORREF textColor, COLORREF textSelColor, COLORREF lineColor);
	bool GetSelectItemRect(CRect & rect);
	void SetRowInfo(CUIntArray & arraySize, CUIntArray & arrayType, CUIntArray & arrayAlign);
	void SetFontSize(int nSize);
	void SetFontBold(UINT nValue);
	void SetFontName(CString sName);
	void SetItemLimit(CUIntArray & arrayList);
	CString GetEditText();
	void SetFocusItem(int nType);
	void CheckLimitEditText();
	void GetItemRect(int nCol, CRect & rect);

	void SetReadOnly(bool bReadOnly);	//20221013 GBM - ReadOnly �ֱ� ���� �߰�

private:
	void Redisplay();
	void CheckEditItem(int nCol);
	void TypeTextRefresh();
	void CheckSelectItemRefresh(UINT nFlags, CPoint point);

private:
	CFont	m_font, m_font2, m_font3;
	int		m_nIndex, m_nSelectIndex;
	bool	m_bMouseRbtn;
	bool	m_bMouseLbtn;

	bool	m_bMouseOn;
	bool	m_bSelected;

	COLORREF m_bkColor, m_bkSelColor, m_textColor, m_textSelColor, m_lineColor;

	CWnd*	m_pWnd;

	CUIntArray	m_arraySize, m_arrayType, m_arrayAlign, m_arrayLimit;
	CCriticalSectionEx m_sizeSync, m_textSync;
	CList<pItemValue, pItemValue> m_listItemValue;

	bool m_bReadOnlyState;	//20221013 GBM - ReadOnly �ֱ� ���� �߰�

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOM_ITEM_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnResizeColumn(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CEdit m_editText;
	afx_msg void OnEnKillfocusItemEdit();
};
