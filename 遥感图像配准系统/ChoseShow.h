#pragma once
#include "afxwin.h"


// ChoseShow 对话框

class ChoseShow : public CDialog
{
	DECLARE_DYNAMIC(ChoseShow)

public:
	ChoseShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ChoseShow();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHOSE_SHOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	LPCTSTR filename;
	CStatic m_picture;
};
