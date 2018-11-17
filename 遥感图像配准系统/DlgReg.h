#if !defined(AFX_DlgReg_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_)
#define AFX_DlgReg_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_

#include "RSIDoc.h"
#include "afxwin.h"



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgReg dialog

class CDlgReg : public CDialog
{
	// Construction
public:
	CRSIDoc* m_pDoc;
	CDlgReg(CWnd* pParent = NULL, CRSIDoc* pDoc = NULL);   // standard constructor

														   // Dialog Data
														   //{{AFX_DATA(CDlgReg)
	enum { IDD = IDD_DLG_REG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	WORD *pData;
	int nbands;
	DWORD lHeight;
	DWORD lWidth;
	CDibt m_DIBt;
	DWORD ChangeToBmp(const char* filename);


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgReg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
														//}}AFX_VIRTUAL

														// Implementation
protected:
	// 计算图像位置的标志位。FALSE表示还没有计算图像位置
	BOOL m_bCalImgLoc;
	// 选取特征点标志位。FALSE表示还没有选取
	BOOL m_bChoseFeature;
	//特征点选取方式标志位。FALSE表示手动选择特征点
	BOOL auto_Feature;
	//手动选取特征点时使用此标志位，false表示配准图像上的点已经选取完毕但基准图像没选取对应点
	//true表示基准图像上已经选好对应点
	BOOL handle_baseFeaturechosen;
	// 设置图像等控件的位置大小	
	void CalImageLocation();

	// 基准图像
	CDib* m_pDibInit;

	// 待配准图像
	CDib* m_pDibSamp;

	// 配准后的图像
	CDib* m_pDibResult;

	// 基准图像显示区域
	CRect m_rectInitImage;

	// 待配准图像显示区域
	CRect m_rectResltImage;

	// 待配准特征点位置	
	MyPoint m_pPointSampl[100];
	// 配准的特征点位置
	//CPoint m_pPointBase[40];
	MyPoint m_pPointBase[100];
	//RMS
	double RMS[100];
	//RMS计算
	void RMS_compute();
	//自动模式选取特征点并计算空间转换矩阵
	void auto_Featurechosen();
	//自动选择点筛选
	void auto_Good(UINT nFlags, CPoint point);
	//手动模式选取特征点
	void handle_Featurechosen(UINT nFlags, CPoint point);
	// 寻找配准点
	CPoint FindMatchPoint(CDib* pDibBase, CDib* pDibSamp, CPoint pointSamp);

	// 计算相似度
	double CalCorrelation(unsigned char* pBase, unsigned char* pSamp, int nBlockLen);
	// 画出特征点
	void   DrawFeature(CDC* pDC);

	// 获得空间变换系数
	void   GetAffinePara(MyPoint* pPointBase, MyPoint* pPointSampl, double* pDbBs2SpAffPara);
	// 计算两个矩阵相乘
	void   CalMatProduct(double* pDbSrc1, double *pDbSrc2, double *pDbDest, int nX, int nY, int nZ);
	// 计算矩阵的逆
	BOOL   CalInvMatrix(double* pDbSrc, int nLen);

	// 获得待配准图像空间变换后的区域
	CRect  GetAftAffDim(double* pDbAffPara);

	// 计算空间变换后的待配准图像
	LPBYTE   SetSampImgAftAff(double* pDbAffPara, CRect rectNewImg);

	// 计算三次插值
	unsigned char CalSpline(unsigned char *pUnchCorr, double x, double y);

	// 计算空间变换后的基准图像
	LPBYTE   SetBaseImgAftAff(CRect rectNewImg);

	// Generated message map functions
	//{{AFX_MSG(CDlgReg)
	afx_msg void OnPaint();
	afx_msg void OnRegOpen();
	afx_msg void OnRegReg();
	afx_msg void OnRegChoseFeature();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nChsFeatureNum;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:
	//原图显示区宽
	double len_init_x;
	//原图显示区长
	double len_init_y;
	//待配准图显示区宽
	double len_reg_x;
	//待配准图显示区长
	double len_reg_y;
	//空间变换矩阵
	cv::Mat homo;
	cv::Mat homoR;
	//原图区放大比例
	double alpha_init;
	//待配准图区放大比例
	double alpha_samp;
	//选择自动匹配算法
	CComboBox m_comboauto;
	afx_msg void OnDropdownComboauto();
	CComboBox m_rms_show;

	BOOL MOUSE_R;

	afx_msg void OnFeatureRms();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DlgReg_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_)
