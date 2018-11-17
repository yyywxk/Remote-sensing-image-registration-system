#if !defined(AFX_DlgEnhance_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_)
#define AFX_DlgEnhance_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_

#include "RSIDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnhance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEnhance dialog

class CDlgEnhance : public CDialog
{
// Construction
public:
	CRSIDoc* m_pDoc;
	CDlgEnhance(CWnd* pParent = NULL, CRSIDoc* pDoc = NULL);   // standard constructor
		// ֱ��ͼ���⻯����
	BOOL Histogram_Equalization( );
	// ֱ��ͼ�涨����ƥ�䣩����
	BOOL Histogram_Match();
	//ֱ��ͼͳ�ƺ���
	void Histogram_Statistic( float *probability);
	// ���ԻҶ���ǿ
	BOOL Linear1( );
	// �ֶ����ԻҶ���ǿ
	BOOL Linear2( );
		// �������������ԻҶ���ǿ
	BOOL Linear3( );
	    // ����ƽ��ƽ������
	BOOL Smooth1( );
		//��Ȩƽ��ƽ������
	BOOL Smooth2( );
	// ѡ��ʽ��Ĥƽ������
	BOOL Smooth3( );
	// ��ֵ�˲�ƽ������
	BOOL Smooth4( );
	// �����ݶ��񻯺���
	BOOL Sharpen1( );
	// ������˹�񻯺���
	BOOL Sharpen2( );
		//������㺯��
    BOOL Convolution(int tempH, int tempW, int tempMX, int tempMY, float *fpTempArray, float fCoef);
		// α��ɫ��ǿ
	BOOL Color();
	// ֱ��ͼ�涨����ƥ�䣩����
// Dialog Data
	//{{AFX_DATA(CDlgEnhance)
	enum { IDD = IDD_DLG_ENHANCE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEnhance)
	protected:
		//ֱ��ͼ��ǿ��ģʽ
	int Histogram_mode;
	int Linear_mode;
	int Smooth_mode;
	int Sharpen_mode;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// ����ͼ��λ�õı�־λ��FALSE��ʾ��û�м���ͼ��λ��
	BOOL m_bCalImgLoc;		

// ����ͼ��ȿؼ���λ�ô�С	
	void CalImageLocation();	

	// ��׼ͼ��
	CDib* m_pDibInit;		 

	// ��ǿ���ͼ��
	CDib* m_pDibResult;		

	// ��׼ͼ����ʾ����
	CRect m_rectInitImage;		

	// ��ǿ��ͼ����ʾ����
	CRect m_rectResltImage;	

	// Generated message map functions
	//{{AFX_MSG(CDlgEnhance)
	afx_msg void OnPaint();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedHistogramEqual();
	afx_msg void OnBnClickedHistogramMatch();
	afx_msg void OnBnClickedButtonHistogram();
	afx_msg void OnBnClickedButtonLinear();
	afx_msg void OnBnClickedLinear1();
	afx_msg void OnBnClickedLinear2();
	afx_msg void OnBnClickedLinear3();

	afx_msg void OnBnClickedButtonSmooth();
	afx_msg void OnBnClickedSmooth1();
	afx_msg void OnBnClickedSmooth2();
	afx_msg void OnBnClickedSmooth3();
	afx_msg void OnBnClickedSmooth4();
	afx_msg void OnBnClickedButtonSharpen();
	afx_msg void OnBnClickedSharpen1();
	afx_msg void OnBnClickedSharpen2();

	afx_msg void OnBnClickedButtonColor();

	double alpha_init;
	double alpha_samp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DlgEnhance_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_)
