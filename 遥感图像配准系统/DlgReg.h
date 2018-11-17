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
	// ����ͼ��λ�õı�־λ��FALSE��ʾ��û�м���ͼ��λ��
	BOOL m_bCalImgLoc;
	// ѡȡ�������־λ��FALSE��ʾ��û��ѡȡ
	BOOL m_bChoseFeature;
	//������ѡȡ��ʽ��־λ��FALSE��ʾ�ֶ�ѡ��������
	BOOL auto_Feature;
	//�ֶ�ѡȡ������ʱʹ�ô˱�־λ��false��ʾ��׼ͼ���ϵĵ��Ѿ�ѡȡ��ϵ���׼ͼ��ûѡȡ��Ӧ��
	//true��ʾ��׼ͼ�����Ѿ�ѡ�ö�Ӧ��
	BOOL handle_baseFeaturechosen;
	// ����ͼ��ȿؼ���λ�ô�С	
	void CalImageLocation();

	// ��׼ͼ��
	CDib* m_pDibInit;

	// ����׼ͼ��
	CDib* m_pDibSamp;

	// ��׼���ͼ��
	CDib* m_pDibResult;

	// ��׼ͼ����ʾ����
	CRect m_rectInitImage;

	// ����׼ͼ����ʾ����
	CRect m_rectResltImage;

	// ����׼������λ��	
	MyPoint m_pPointSampl[100];
	// ��׼��������λ��
	//CPoint m_pPointBase[40];
	MyPoint m_pPointBase[100];
	//RMS
	double RMS[100];
	//RMS����
	void RMS_compute();
	//�Զ�ģʽѡȡ�����㲢����ռ�ת������
	void auto_Featurechosen();
	//�Զ�ѡ���ɸѡ
	void auto_Good(UINT nFlags, CPoint point);
	//�ֶ�ģʽѡȡ������
	void handle_Featurechosen(UINT nFlags, CPoint point);
	// Ѱ����׼��
	CPoint FindMatchPoint(CDib* pDibBase, CDib* pDibSamp, CPoint pointSamp);

	// �������ƶ�
	double CalCorrelation(unsigned char* pBase, unsigned char* pSamp, int nBlockLen);
	// ����������
	void   DrawFeature(CDC* pDC);

	// ��ÿռ�任ϵ��
	void   GetAffinePara(MyPoint* pPointBase, MyPoint* pPointSampl, double* pDbBs2SpAffPara);
	// ���������������
	void   CalMatProduct(double* pDbSrc1, double *pDbSrc2, double *pDbDest, int nX, int nY, int nZ);
	// ����������
	BOOL   CalInvMatrix(double* pDbSrc, int nLen);

	// ��ô���׼ͼ��ռ�任�������
	CRect  GetAftAffDim(double* pDbAffPara);

	// ����ռ�任��Ĵ���׼ͼ��
	LPBYTE   SetSampImgAftAff(double* pDbAffPara, CRect rectNewImg);

	// �������β�ֵ
	unsigned char CalSpline(unsigned char *pUnchCorr, double x, double y);

	// ����ռ�任��Ļ�׼ͼ��
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
	//ԭͼ��ʾ����
	double len_init_x;
	//ԭͼ��ʾ����
	double len_init_y;
	//����׼ͼ��ʾ����
	double len_reg_x;
	//����׼ͼ��ʾ����
	double len_reg_y;
	//�ռ�任����
	cv::Mat homo;
	cv::Mat homoR;
	//ԭͼ���Ŵ����
	double alpha_init;
	//����׼ͼ���Ŵ����
	double alpha_samp;
	//ѡ���Զ�ƥ���㷨
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
