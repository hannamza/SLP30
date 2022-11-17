#if !defined(AFX_CustomComboBox_H__05957708_2213_4285_AF31_211F3A09A31A__INCLUDED_)
#define AFX_CustomComboBox_H__05957708_2213_4285_AF31_211F3A09A31A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomComboBox window

class CCustomComboBox : public CComboBox
{
// Construction
public:
	CCustomComboBox();

// Attributes

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomComboBox)
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CustomComboBox_H__05957708_2213_4285_AF31_211F3A09A31A__INCLUDED_)
