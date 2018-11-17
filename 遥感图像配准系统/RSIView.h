// RSIView.h : interface of the CRSIView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSINGVIEW_H__613B030F_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)
#define AFX_IMAGEPROCESSINGVIEW_H__613B030F_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRSIView : public CScrollView
{
protected: // create from serialization only
	CRSIView();
	DECLARE_DYNCREATE(CRSIView)

// Attributes
public:
	CRSIDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSIView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	double* m_pDbImage;
	int m_nSupp;
	int m_nDWTCurDepth;
	virtual ~CRSIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRSIView)
	afx_msg void OnRegReg();
	afx_msg void OnCorCor();
	afx_msg void OnEnhanceEnhance();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

#ifndef _DEBUG  // debug version in ImageProcessingView.cpp
inline CRSIDoc* CRSIView::GetDocument()
   { return (CRSIDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSINGVIEW_H__613B030F_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)
