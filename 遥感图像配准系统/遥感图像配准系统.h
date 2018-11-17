// 遥感图像配准系统.h : main header file for the IMAGEPROCESSING application
//

#if !defined(AFX_IMAGEPROCESSING_H__613B0305_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)
#define AFX_IMAGEPROCESSING_H__613B0305_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "CDibt.h"
#include "RSIDoc.h"


#ifndef IMG_INIT_REG
#define IMG_INIT_REG
#define IMG_W 640 //绘图区原始大小
#define IMG_H 550
//#define IMG_W 288 //绘图区原始大小
//#define IMG_H 324
#define IMG_INIT_W (alpha_init*IMG_W)
#define IMG_INIT_H (alpha_init*IMG_H)
#define IMG_SAMP_W (alpha_samp*IMG_W)
#define IMG_SAMP_H (alpha_samp*IMG_H)
#endif

/////////////////////////////////////////////////////////////////////////////
// CRSIApp:
// See 遥感图像配准系统.cpp for the implementation of this class
//

class CRSIApp : public CWinApp
{
public:
	CRSIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	/*BYTE *pData;
	int nbands;
	DWORD lHeight;
	DWORD lWidth;
	CDibt m_DIBt;
	void ChangeToBmp();*/
	

	// Implementation
	//{{AFX_MSG(CRSIApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnTIFF_OpenFile();
	//afx_msg  void OnGdalOpen();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSING_H__613B0305_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)
