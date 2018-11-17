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
		// 直方图均衡化函数
	BOOL Histogram_Equalization( );
	// 直方图规定化（匹配）函数
	BOOL Histogram_Match();
	//直方图统计函数
	void Histogram_Statistic( float *probability);
	// 线性灰度增强
	BOOL Linear1( );
	// 分段线性灰度增强
	BOOL Linear2( );
		// 对数函数非线性灰度增强
	BOOL Linear3( );
	    // 邻域平均平滑函数
	BOOL Smooth1( );
		//加权平均平滑函数
	BOOL Smooth2( );
	// 选择式掩膜平滑函数
	BOOL Smooth3( );
	// 中值滤波平滑函数
	BOOL Smooth4( );
	// 门限梯度锐化函数
	BOOL Sharpen1( );
	// 拉普拉斯锐化函数
	BOOL Sharpen2( );
		//卷积运算函数
    BOOL Convolution(int tempH, int tempW, int tempMX, int tempMY, float *fpTempArray, float fCoef);
		// 伪彩色增强
	BOOL Color();
	// 直方图规定化（匹配）函数
// Dialog Data
	//{{AFX_DATA(CDlgEnhance)
	enum { IDD = IDD_DLG_ENHANCE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEnhance)
	protected:
		//直方图增强的模式
	int Histogram_mode;
	int Linear_mode;
	int Smooth_mode;
	int Sharpen_mode;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// 计算图像位置的标志位。FALSE表示还没有计算图像位置
	BOOL m_bCalImgLoc;		

// 设置图像等控件的位置大小	
	void CalImageLocation();	

	// 基准图像
	CDib* m_pDibInit;		 

	// 增强后的图像
	CDib* m_pDibResult;		

	// 基准图像显示区域
	CRect m_rectInitImage;		

	// 增强后图像显示区域
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
