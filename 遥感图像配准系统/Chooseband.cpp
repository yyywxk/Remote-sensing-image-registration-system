// Chooseband.cpp : implementation file
//

#include "stdafx.h"
#include "CDibt.h"
#include "Chooseband.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChooseband dialog


CChooseband::CChooseband(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseband::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChooseband)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChooseband::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChooseband)
	DDX_Control(pDX, IDC_COMBORed, m_red);
	DDX_Control(pDX, IDC_COMBOGreen, m_green);
	DDX_Control(pDX, IDC_COMBOBlue, m_blue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChooseband, CDialog)
	//{{AFX_MSG_MAP(CChooseband)
	ON_CBN_DROPDOWN(IDC_COMBORed, OnDropdownCOMBORed)
	ON_CBN_DROPDOWN(IDC_COMBOGreen, OnDropdownCOMBOGreen)
	ON_CBN_DROPDOWN(IDC_COMBOBlue, OnDropdownCOMBOBlue)
	ON_CBN_SELCHANGE(IDC_COMBOBlue, OnSelchangeCOMBOBlue)
	ON_CBN_SELCHANGE(IDC_COMBOGreen, OnSelchangeCOMBOGreen)
	ON_CBN_SELCHANGE(IDC_COMBORed, OnSelchangeCOMBORed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChooseband message handlers

void CChooseband::OnDropdownCOMBORed()
{
	// TODO: Add your control notification handler code here
	int i;
	CString t;
	if (nbands != 0)
	{
		m_red.ResetContent();
		for (i = 0; i < nbands; i++)
		{
			t.Format("%d", i + 1);
			m_red.AddString(t);
		}
		UpdateData(FALSE);
	}
}

void CChooseband::OnDropdownCOMBOGreen()
{
	// TODO: Add your control notification handler code here
	int i;
	CString t;
	if (nbands != 0)
	{
		m_green.ResetContent();
		for (i = 0; i < nbands; i++)
		{
			t.Format("%d", i + 1);
			m_green.AddString(t);
		}
		UpdateData(FALSE);
	}
}

void CChooseband::OnDropdownCOMBOBlue()
{
	// TODO: Add your control notification handler code here
	int i;
	CString t;
	if (nbands != 0)
	{
		m_blue.ResetContent();
		for (i = 0; i < nbands; i++)
		{
			t.Format("%d", i + 1);
			m_blue.AddString(t);
		}
		UpdateData(FALSE);
	}
}

void CChooseband::OnSelchangeCOMBOBlue()
{
	// TODO: Add your control notification handler code here
	if (m_blue.GetCurSel() == CB_ERR)
		B = 1;
	else
	{
		m_choice = m_blue.GetCurSel();
		B = m_choice + 1;
	}
	

}

void CChooseband::OnSelchangeCOMBOGreen()
{
	// TODO: Add your control notification handler code here
	if (m_green.GetCurSel() == CB_ERR)
		G = 2;
	else
	{
		m_choice = m_green.GetCurSel();
		G = m_choice + 1;
	}

}

void CChooseband::OnSelchangeCOMBORed()
{
	// TODO: Add your control notification handler code here
	if (m_red.GetCurSel() == CB_ERR)
		R = 3;
	else
	{
		m_choice = m_red.GetCurSel();

		R = m_choice + 1;
	}

}

