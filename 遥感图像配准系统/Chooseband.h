#if !defined(AFX_CHOOSEBAND_H__523FDA97_E62D_47B3_ABB2_95760119B47A__INCLUDED_)
#define AFX_CHOOSEBAND_H__523FDA97_E62D_47B3_ABB2_95760119B47A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Chooseband.h : header file
//
#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CChooseband dialog

class CChooseband : public CDialog
{
	// Construction
public:
	int R;
	int G;
	int B;
	int m_choice;
	int nbands;
	CChooseband(CWnd* pParent = NULL);   // standard constructor

										 // Dialog Data
										 //{{AFX_DATA(CChooseband)
	enum { IDD = IDD_SelectBand};
	CComboBox	m_red;
	CComboBox	m_green;
	CComboBox	m_blue;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChooseband)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
														//}}AFX_VIRTUAL

														// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChooseband)
	afx_msg void OnDropdownCOMBORed();
	afx_msg void OnDropdownCOMBOGreen();
	afx_msg void OnDropdownCOMBOBlue();
	afx_msg void OnSelchangeCOMBOBlue();
	afx_msg void OnSelchangeCOMBOGreen();
	afx_msg void OnSelchangeCOMBORed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOOSEBAND_H__523FDA97_E62D_47B3_ABB2_95760119B47A__INCLUDED_)
#pragma once
