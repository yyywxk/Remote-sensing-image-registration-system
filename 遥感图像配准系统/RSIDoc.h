// RSIDoc.h : interface of the CRSIDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSINGDOC_H__613B030D_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)
#define AFX_IMAGEPROCESSINGDOC_H__613B030D_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CDib.h"
#include "CDibt.h"
#include "Chooseband.h"
#include "ChoseShow.h"
#include "resource.h" 
#include "gdal_priv.h"
//#include <opencv2\highgui.hpp>
#include<opencv\highgui.h>
#include<opencv\cv.h>
#include"MyCpoint.h"
#include"CGISImage.h"
#include<opencv2\highgui\highgui.hpp>

class CRSIDoc : public CDocument
{
protected: // create from serialization only
	CRSIDoc();
	DECLARE_DYNCREATE(CRSIDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSIDoc)
	public:
	static CRSIDoc* create()
	{
		return new CRSIDoc() ;//调用保护的构造函数 
	}
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	CString OnFileSaveAs_result();
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nColorIndex;		// 伪彩色编码编号
	CDib* m_pDibTest;
	int InitDocVariable();
	int FreeDocVariable();
	CDib* m_pDibInit;
	
	//CDibt* m_pDibtInit;
	//CDibt* m_pDibtTest;

	//BYTE *pData;
	WORD *pData;
	int nbands;
	DWORD lHeight;
	DWORD lWidth;
	CDibt m_DIBt;
	DWORD ChangeToBmp(const char* filename);


	//CDibt m_DIBt;
	virtual ~CRSIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRSIDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileReload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//public:
	//afx_msg void OnGdalOpen();
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSINGDOC_H__613B030D_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)
