#if !defined(AFX_DlgCor_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_)
#define AFX_DlgCor_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_

#include "RSIDoc.h"
#include "afxwin.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCor dialog

class CDlgCor : public CDialog
{
// Construction
public:
	CRSIDoc* m_pDoc;
	CDlgCor(CWnd* pParent = NULL, CRSIDoc* pDoc = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgReg)
	enum { IDD = IDD_DLG_COR };
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
	//{{AFX_VIRTUAL(CDlgCor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// ����ͼ��λ�õı�־λ��FALSE��ʾ��û�м���ͼ��λ��
	BOOL m_bCalImgLoc;		

// ѡȡ�������־λ��FALSE��ʾ��û��ѡȡ
	BOOL m_bChoseFeature;	
	//������ѡȡ��ʽ��־λ��FALSE��ʾ�ֶ�ѡ��������
	BOOL auto_Feature;
	//RMS
	double RMS[40];
	//RMS����
	void RMS_compute();
	//�ֶ�ѡȡ������ʱʹ�ô˱�־λ��false��ʾУ��ͼ���ϵĵ��Ѿ�ѡȡ��ϵ���׼ͼ��ûѡȡ��Ӧ��
	//true��ʾ��׼ͼ�����Ѿ�ѡ�ö�Ӧ��
	BOOL handle_baseFeaturechosen;
// ����ͼ��ȿؼ���λ�ô�С	
	void CalImageLocation();	

	// ��׼ͼ��
	CDib* m_pDibInit;		 

	// ��У��ͼ��
	CDib* m_pDibSamp;		

	// У�����ͼ��
	CDib* m_pDibResult;		

	// ��׼ͼ����ʾ����
	CRect m_rectInitImage;		

	// ��У��ͼ����ʾ����
	CRect m_rectResltImage;	

	// ��У��ͼ���������λ��	
	//CPoint m_pPointSampl[3];		
	MyPoint m_pPointSampl[40];
	// ��׼��������λ��
	//CPoint m_pPointBase[40];
	MyPoint m_pPointBase[40];
	// ��׼ͼ���������λ��
	//CPoint m_pPointBase[3];		
	//���Զ�ģʽѡȡ������
	void auto_Featurechosen(UINT nFlags, CPoint point);
	//�ֶ�ģʽѡȡ������
	void handle_Featurechosen(UINT nFlags, CPoint point);
	//�Զ�ѡ���ɸѡ
	void auto_Good(UINT nFlags, CPoint point);
			
	// ����������
	void   DrawFeature(CDC* pDC);			

	// ��ÿռ�任ϵ��
	void   GetAffinePara(MyPoint* pPointBase, MyPoint* pPointSampl, double* pDbBs2SpAffPara);		
	// ���������������
	void   CalMatProduct(double* pDbSrc1, double *pDbSrc2, double *pDbDest, int nX, int nY, int nZ);			
	// ����������
	BOOL   CalInvMatrix(double* pDbSrc, int nLen);	
	
	// ��ô�У��ͼ��ռ�任�������
	CRect  GetAftAffDim(double* pDbAffPara);	

	// ����ռ�任��Ĵ�У��ͼ��
	LPBYTE   SetSampImgAftAff(double* pDbAffPara, CRect rectNewImg);	

	// �����ֵ
	unsigned char CalSpline(unsigned char *pUnchCorr, double x, double y);
	 
	// Generated message map functions
	//{{AFX_MSG(CDlgCor)
	afx_msg void OnPaint();
	afx_msg void OnCorOpen();
	afx_msg void OnCorCor();
	afx_msg void OnCorChoseFeature();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nChsFeatureNum;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
public:
	afx_msg void OnStnClickedStaticCor();
	//ԭͼ��ʾ����
	double len_init_x;
	//ԭͼ��ʾ����
	double len_init_y;
	//��У��ͼ��ʾ����
	double len_cor_x;
	//��У��ͼ��ʾ����
	double len_cor_y;
	double alpha_init;
	double alpha_samp;
	cv::Mat homo;
	cv::Mat homoR;
	CComboBox m_rms_show;
	BOOL MOUSE_R;
	afx_msg void OnFeatureRms();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DlgCor_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_)
