#if !defined(AFX_AFTREG_H__986DCC9D_F114_4D88_A06B_FC955F33D7BB__INCLUDED_)
#define AFX_AFTREG_H__986DCC9D_F114_4D88_A06B_FC955F33D7BB__INCLUDED_

#include "Cdib.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AftReg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAftReg dialog

class CDlgAftReg : public CDialog
{
// Construction
public:
	// ����ؼ�λ�ñ�־λ
	BOOL m_bCalImgLoc;

	// ��ʾͼ�������
	CRect m_rectImage;

	// ����ؼ�λ��
	void  CalImgLocation();

	// ��ʾ��ͼ��
	CDib* m_pDib;

	// ��������
	CDlgAftReg(CWnd* pParent = NULL, CDib* pDibShow = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAftReg)
	enum { IDD = IDD_DLG_AFTREG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAftReg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAftReg)
	afx_msg void OnPaint();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	double alpha_init;
	double alpha_samp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AFTREG_H__986DCC9D_F114_4D88_A06B_FC955F33D7BB__INCLUDED_)
