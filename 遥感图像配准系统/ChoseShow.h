#pragma once
#include "afxwin.h"


// ChoseShow �Ի���

class ChoseShow : public CDialog
{
	DECLARE_DYNAMIC(ChoseShow)

public:
	ChoseShow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ChoseShow();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHOSE_SHOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	LPCTSTR filename;
	CStatic m_picture;
};
