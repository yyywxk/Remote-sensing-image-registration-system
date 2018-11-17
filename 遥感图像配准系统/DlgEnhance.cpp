// DlgEnhance.cpp : implementation file
//

#include "stdafx.h"
#include "ң��ͼ����׼ϵͳ.h"
#include "RSIDoc.h"
#include "GlobalApi.h"
#include "DlgEnhance.h"
#include "ColorTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define pi 3.14159265359

/////////////////////////////////////////////////////////////////////////////
// CDlgEnhance dialog

CDlgEnhance::CDlgEnhance(CWnd* pParent /*=NULL*/, CRSIDoc* pDoc)
	: CDialog(CDlgEnhance::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEnhance)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// ��ȡ�ĵ���ָ��
	m_pDoc = pDoc;

	// ���ü���ͼ��λ�ñ�־λλFALSE
	m_bCalImgLoc = FALSE;

	// ���û�׼ͼ��Ϊԭʼ�򿪵�ͼ��
	m_pDibInit = pDoc->m_pDibInit;

	// ���ô���׼ͼ��
	m_pDibResult = new CDib;

	Histogram_mode = 1;
	Linear_mode = 1;
	Smooth_mode = 1;
	Sharpen_mode = 1;
	alpha_init = 1.0;
	alpha_samp = 1.0;
}

void CDlgEnhance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnhance)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	if(Histogram_mode == 1)
	{
	((CButton *)GetDlgItem(IDC_HISTOGRAM_EQUAL))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_HISTOGRAM_MATCH))->SetCheck(FALSE);
	}
	if(Histogram_mode == 2)
	{
	((CButton *)GetDlgItem(IDC_HISTOGRAM_EQUAL))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_HISTOGRAM_MATCH))->SetCheck(TRUE);
	}
	if(Linear_mode == 1)
	{
	((CButton *)GetDlgItem(IDC_LINEAR_1))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_LINEAR_2))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_LINEAR_3))->SetCheck(FALSE);
	}
	if(Linear_mode == 2)
	{
	((CButton *)GetDlgItem(IDC_LINEAR_1))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_LINEAR_2))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_LINEAR_3))->SetCheck(FALSE);
	}
	if(Linear_mode == 3)
	{
	((CButton *)GetDlgItem(IDC_LINEAR_1))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_LINEAR_2))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_LINEAR_3))->SetCheck(TRUE);
	}
	if(Smooth_mode == 1)
	{
	((CButton *)GetDlgItem(IDC_SMOOTH_1))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_SMOOTH_2))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_SMOOTH_3))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_SMOOTH_4))->SetCheck(FALSE);
	}
	if(Smooth_mode == 2)
	{
	((CButton *)GetDlgItem(IDC_SMOOTH_1))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_SMOOTH_2))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_SMOOTH_3))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_SMOOTH_4))->SetCheck(FALSE);
	}
	if(Smooth_mode == 3)
	{
	((CButton *)GetDlgItem(IDC_SMOOTH_1))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_SMOOTH_2))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_SMOOTH_3))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_SMOOTH_4))->SetCheck(FALSE);
	}
	if(Smooth_mode == 4)
	{
	((CButton *)GetDlgItem(IDC_SMOOTH_1))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_SMOOTH_2))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_SMOOTH_3))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_SMOOTH_4))->SetCheck(TRUE);
	}
	if(Sharpen_mode == 1)
	{
	((CButton *)GetDlgItem(IDC_SHARPEN_1))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_SHARPEN_2))->SetCheck(FALSE);
	}
	if(Sharpen_mode == 2)
	{
	((CButton *)GetDlgItem(IDC_SHARPEN_1))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_SHARPEN_2))->SetCheck(TRUE);
	}

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnhance, CDialog)
	//{{AFX_MSG_MAP(CDlgEnhance)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgEnhance::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgEnhance::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_HISTOGRAM_EQUAL, &CDlgEnhance::OnBnClickedHistogramEqual)
	ON_BN_CLICKED(IDC_HISTOGRAM_MATCH, &CDlgEnhance::OnBnClickedHistogramMatch)
	ON_BN_CLICKED(IDC_BUTTON_HISTOGRAM, &CDlgEnhance::OnBnClickedButtonHistogram)
	ON_BN_CLICKED(IDC_LINEAR_1, &CDlgEnhance::OnBnClickedLinear1)
	ON_BN_CLICKED(IDC_LINEAR_2, &CDlgEnhance::OnBnClickedLinear2)
	ON_BN_CLICKED(IDC_LINEAR_3, &CDlgEnhance::OnBnClickedLinear3)
	ON_BN_CLICKED(IDC_BUTTON_LINEAR, &CDlgEnhance::OnBnClickedButtonLinear)
	ON_BN_CLICKED(IDC_SMOOTH_1, &CDlgEnhance::OnBnClickedSmooth1)
	ON_BN_CLICKED(IDC_SMOOTH_2, &CDlgEnhance::OnBnClickedSmooth2)
	ON_BN_CLICKED(IDC_SMOOTH_3, &CDlgEnhance::OnBnClickedSmooth3)
	ON_BN_CLICKED(IDC_SMOOTH_4, &CDlgEnhance::OnBnClickedSmooth4)
	ON_BN_CLICKED(IDC_BUTTON_SMOOTH, &CDlgEnhance::OnBnClickedButtonSmooth)
	ON_BN_CLICKED(IDC_SHARPEN_1, &CDlgEnhance::OnBnClickedSharpen1)
	ON_BN_CLICKED(IDC_SHARPEN_2, &CDlgEnhance::OnBnClickedSharpen2)
	ON_BN_CLICKED(IDC_BUTTON_SHARPEN, &CDlgEnhance::OnBnClickedButtonSharpen)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &CDlgEnhance::OnBnClickedButtonColor)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnhance message handlers

void CDlgEnhance::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// �����û�м���ͼ���λ�ã�����м���
	if(!m_bCalImgLoc)
		CalImageLocation();

	CSize sizeDisplay;
	CPoint pointDisplay;
	
	// ��ʾ��׼ͼ��
	if(!m_pDibInit->IsEmpty())
	{
		sizeDisplay.cx = IMG_INIT_W;
		sizeDisplay.cy = IMG_INIT_H;
		pointDisplay.x = m_rectInitImage.left;
		pointDisplay.y = m_rectInitImage.top;


		m_pDibInit->Draw(&dc, pointDisplay, sizeDisplay);
	}
	
	// ��ʾ��ǿ��ͼ��
	if(!m_pDibResult->IsEmpty())
	{
		sizeDisplay.cx = IMG_SAMP_W;
		sizeDisplay.cy = IMG_SAMP_H;

		pointDisplay.x = m_rectResltImage.left;
		pointDisplay.y = m_rectResltImage.top;

		m_pDibResult->Draw(&dc, pointDisplay, sizeDisplay);
	}
}

/*************************************************************************
 * \�������ƣ�
 *   CalImageLocation()
 * \�������:��
 * \����ֵ:��
 * \˵��:
 *�ú������öԻ����еĿؼ�λ�úʹ�С����������ʾͼ���λ�á�Ĭ�ϵ�ͼ���СΪ352��288�����ͼ��С��
 *�˴�С����ؼ���С����Ϊ352��288������ͼ������ڿؼ��м䡣
 *************************************************************************/
void CDlgEnhance::CalImageLocation()
{
	// ��ÿؼ�IDC_REG_INIT_IMAGE�ľ��������ÿؼ��ĳ�ʼλ����Ϣ
	CWnd* pWnd=GetDlgItem(IDC_ENHANCE_INIT_IMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement=new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);
	
	// ͼ����
	int nImageWidth1;
	nImageWidth1= m_pDibInit->m_lpBMIH->biWidth;

	// ͼ���
	int nImageHeight1;
	nImageHeight1 = m_pDibInit->m_lpBMIH->biHeight;
	
	// �����ؼ�IDC_ENHANCE_INIT_IMAGE�Ĵ�Сλ�ã���ͬʱ������ʾ��׼ͼ���λ��
	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + IMG_INIT_H;
	m_rectInitImage.bottom = winPlacement->rcNormalPosition.bottom;
	m_rectInitImage.top = winPlacement->rcNormalPosition.top;

	winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + IMG_INIT_W;
	m_rectInitImage.right = winPlacement->rcNormalPosition.right;
	m_rectInitImage.left = winPlacement->rcNormalPosition.left;

	// ����IDC_ENHANCE_INIT_IMAGE�ؼ��Ĵ�Сλ��
	pWnd->SetWindowPlacement(winPlacement);	
			
	// �����ʾ��׼ͼ��ؼ����ұ�λ�ã��Ա�ȷ����ʾ����׼ͼ��ؼ���λ��
	int nIniImgRight;
	nIniImgRight = winPlacement->rcNormalPosition.right;
	
	int  nIniImgLeft;
	nIniImgLeft   = winPlacement->rcNormalPosition.left;

	// ���IDC_ENHANCE_INIT_IMAGE�ؼ����±�λ�ã��Ա���������ؼ���λ��
	int nIniImgBottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;

	// ��ÿؼ�IDC_ENHANCE_RESLT_IMAGE�ľ��������ó�ʼλ����Ϣ
	pWnd=GetDlgItem(IDC_ENHANCE_RESLT_IMAGE);
	pWnd->GetWindowPlacement(winPlacement);
	
	int nImageHeight2,nImageWidth2;
	// �����δ�򿪴���׼ͼ�������ô���׼ͼ���С�ͻ�׼ͼ���С���
	if(!m_pDibResult->IsEmpty())
	{
		nImageWidth2  = m_pDibResult->m_lpBMIH->biWidth;
		nImageHeight2 = m_pDibResult->m_lpBMIH->biHeight;
	}
	else
	{
		nImageHeight2=nImageHeight1;
		nImageWidth2=nImageWidth1;
	}
	// �����ؼ�IDC_ENHANCE_RESLT_IMAGE�Ĵ�Сλ�ã���ͬʱ������ʾ����׼ͼ���λ��

	// �ȵ����ؼ������λ�ã���IDC_ENHANCE_INIT_IMAGE�ؼ����15������
	winPlacement->rcNormalPosition.left = nIniImgRight + 15;

	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + IMG_SAMP_H;
	m_rectResltImage.bottom = winPlacement->rcNormalPosition.bottom;
	m_rectResltImage.top = winPlacement->rcNormalPosition.top;

	winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + IMG_SAMP_W;
	m_rectResltImage.right = winPlacement->rcNormalPosition.right;
	m_rectResltImage.left = winPlacement->rcNormalPosition.left;

	// ����IDC_ENHANCE_RESLT_IMAGE�ؼ��Ĵ�Сλ��
	pWnd->SetWindowPlacement(winPlacement);
	
	if(nIniImgBottom < winPlacement->rcNormalPosition.bottom)
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	nIniImgRight  = winPlacement->rcNormalPosition.right;
	
	if(nImageHeight2<nImageHeight1)
	{
		 // ��ÿؼ�IDC_ENHANCE_INIT_IMAGE�ľ��������ÿؼ��ĳ�ʼλ����Ϣ
	CWnd* pWnd=GetDlgItem(IDC_ENHANCE_INIT_IMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement=new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);
	if(nIniImgBottom < winPlacement->rcNormalPosition.bottom)
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	}	
		// ���ÿؼ�IDC_STATIC_BASEENHANCE��λ�ô�С
	pWnd=GetDlgItem(IDC_STATIC_BASEENHANCE);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.left = (m_rectInitImage.right+m_rectInitImage.left)/2-35;
	winPlacement->rcNormalPosition.right = (m_rectInitImage.right+m_rectInitImage.left)/2+35;
	pWnd->SetWindowPlacement(winPlacement);

	// ���ÿؼ�IDC_STATIC_ENHANCE��λ�ô�С
	pWnd=GetDlgItem(IDC_STATIC_ENHANCE);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.left = (m_rectResltImage.right+m_rectResltImage.left)/2-45;
	winPlacement->rcNormalPosition.right = (m_rectResltImage.right+m_rectResltImage.left)/2+45;
	pWnd->SetWindowPlacement(winPlacement);

	// ���ÿؼ�IDOK��λ�ô�С
	pWnd=GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +95;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 120;
	pWnd->SetWindowPlacement(winPlacement);
	
	// ���ÿؼ�IDCANCEL��λ�ô�С
	pWnd=GetDlgItem(IDCANCEL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +95;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 120;
	pWnd->SetWindowPlacement(winPlacement);

		// ���ÿؼ�IDC_BUTTON_HISTOGRAM��λ�ô�С
	pWnd=GetDlgItem(IDC_BUTTON_HISTOGRAM);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+5 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 25;
	pWnd->SetWindowPlacement(winPlacement);

		// ���ÿؼ�IDC_HISTOGRAM_EQUAL��λ�ô�С
	pWnd=GetDlgItem(IDC_HISTOGRAM_EQUAL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+30 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 44;
	pWnd->SetWindowPlacement(winPlacement);
		// ���ÿؼ�IDC_HISTOGRAM_MATCH��λ�ô�С
	pWnd=GetDlgItem(IDC_HISTOGRAM_MATCH);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+46 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);

		// ���ÿؼ�IDC_BUTTON_LINEAR��λ�ô�С
	pWnd=GetDlgItem(IDC_BUTTON_LINEAR);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+5 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 25;
	pWnd->SetWindowPlacement(winPlacement);

		// ���ÿؼ�IDC_LINEAR_1��λ�ô�С
	pWnd=GetDlgItem(IDC_LINEAR_1);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+30 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 44;
	pWnd->SetWindowPlacement(winPlacement);
		// ���ÿؼ�IDC_LINEAR_2��λ�ô�С
	pWnd=GetDlgItem(IDC_LINEAR_2);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+46 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
			// ���ÿؼ�IDC_LINEAR_3��λ�ô�С
	pWnd=GetDlgItem(IDC_LINEAR_3);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+62 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 76;
	pWnd->SetWindowPlacement(winPlacement);

		// ���ÿؼ�IDC_BUTTON_SMOOTH��λ�ô�С
	pWnd=GetDlgItem(IDC_BUTTON_SMOOTH);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+5 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 25;
	pWnd->SetWindowPlacement(winPlacement);

		// ���ÿؼ�IDC_SMOOTH_1��λ�ô�С
	pWnd=GetDlgItem(IDC_SMOOTH_1);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+30 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 44;
	pWnd->SetWindowPlacement(winPlacement);
		// ���ÿؼ�IDC_SMOOTH_2��λ�ô�С
	pWnd=GetDlgItem(IDC_SMOOTH_2);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+46 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	        // ���ÿؼ�IDC_SMOOTH_3��λ�ô�С
	pWnd=GetDlgItem(IDC_SMOOTH_3);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+62 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 76;
	pWnd->SetWindowPlacement(winPlacement);
		// ���ÿؼ�IDC_SMOOTH_4��λ�ô�С
	pWnd=GetDlgItem(IDC_SMOOTH_4);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+78 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 92;
	pWnd->SetWindowPlacement(winPlacement);

		// ���ÿؼ�IDC_BUTTON_SHARPEN��λ�ô�С
	pWnd=GetDlgItem(IDC_BUTTON_SHARPEN);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+5 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 25;
	pWnd->SetWindowPlacement(winPlacement);

		// ���ÿؼ�IDC_SHARPEN_1��λ�ô�С
	pWnd=GetDlgItem(IDC_SHARPEN_1);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+30 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 44;
	pWnd->SetWindowPlacement(winPlacement);
		// ���ÿؼ�IDC_SHARPEN_2��λ�ô�С
	pWnd=GetDlgItem(IDC_SHARPEN_2);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+46 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);

		// ���ÿؼ�IDC_BUTTON_COLOR��λ�ô�С
	pWnd=GetDlgItem(IDC_BUTTON_COLOR);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+5 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 25;
	pWnd->SetWindowPlacement(winPlacement);

	// �����˶Ի���Ĵ�С
	this->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 300;
	winPlacement->rcNormalPosition.left   = nIniImgLeft   - 20;
	winPlacement->rcNormalPosition.right  = nIniImgRight  + 50;
	this->SetWindowPlacement(winPlacement);

	// �ͷ��ѷ����ڴ�
	delete winPlacement;

	// ���ü���ͼ��ؼ�λ�ñ�־λΪTRUE
	m_bCalImgLoc = TRUE;
}

void CDlgEnhance::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CRSIDoc* save_result=CRSIDoc::create();
	save_result->m_pDibInit=m_pDibResult;
	str=save_result->OnFileSaveAs_result();
	CRSIApp* open_result=(CRSIApp*)AfxGetApp(); 
	if(str!="")
	open_result->OpenDocumentFile(str);
	OnOK();
}

void CDlgEnhance::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
void CDlgEnhance::OnBnClickedHistogramEqual()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Histogram_mode=1;
}

void CDlgEnhance::OnBnClickedHistogramMatch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Histogram_mode=2;
}

void CDlgEnhance::OnBnClickedButtonHistogram()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(Histogram_mode==1)
		Histogram_Equalization();
	else
		Histogram_Match();
}
void CDlgEnhance::OnBnClickedLinear1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Linear_mode=1;
}
void CDlgEnhance::OnBnClickedLinear2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Linear_mode=2;
}
void CDlgEnhance::OnBnClickedLinear3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Linear_mode=3;
}
void CDlgEnhance::OnBnClickedButtonLinear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch(Linear_mode)
	{
	case 1: Linear1(); break;
	case 2: Linear2(); break;
	case 3: Linear3(); break;
	}
}

void CDlgEnhance::OnBnClickedSmooth1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Smooth_mode=1;
}
void CDlgEnhance::OnBnClickedSmooth2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Smooth_mode=2;
}
void CDlgEnhance::OnBnClickedSmooth3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Smooth_mode=3;
}
void CDlgEnhance::OnBnClickedSmooth4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Smooth_mode=4;
}
void CDlgEnhance::OnBnClickedButtonSmooth()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch(Smooth_mode)
	{
	case 1: Smooth1(); break;
	case 2: Smooth2(); break;
	case 3: Smooth3(); break;
	case 4: Smooth4(); break;
	}
}

void CDlgEnhance::OnBnClickedSharpen1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Sharpen_mode=1;
}
void CDlgEnhance::OnBnClickedSharpen2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Sharpen_mode=2;
}
void CDlgEnhance::OnBnClickedButtonSharpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch(Sharpen_mode)
	{
	case 1: Sharpen1(); break;
	case 2: Sharpen2(); break;
	}
}

void CDlgEnhance::OnBnClickedButtonColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Color();
}

//=======================================================
// �������ܣ� ֱ��ͼͳ�ƺ���
// ��������� float *probability -�Ҷȷֲ������ܶ�
// ����ֵ��   ��
//=======================================================
void CDlgEnhance::Histogram_Statistic( float *probability)
{
	LPBYTE	lpSrc;			// ָ��ԭͼ���ָ��	
	long i,j;			//ѭ������	
	int gray[256]; // �Ҷȼ���
	BYTE pixel;	//����ֵ
		// ��׼ͼ������ָ��	
	unsigned char* lpDIBBits;
	lpDIBBits = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	// �Ҷȼ���������ʼ��
	memset(gray,0,sizeof(gray));
	//���ɨ��ͼ���е����ص�,���лҶȼ���ͳ��
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			// ָ��ԭͼ������j�У���i�����ص�ָ��			
			lpSrc = (LPBYTE)lpDIBBits + lLineBytes * j + i;
			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE����
			pixel = (BYTE)*lpSrc;
			// �Ҷ�ͳ�Ƽ���
			gray[pixel]++;
		}
	}
	// ����Ҷȸ����ܶ�
	for(i=0;i<256;i++)
		probability[i] = gray[i] / (lHeight * lWidth *1.0f);
}

BOOL CDlgEnhance::Histogram_Equalization( )
{
	unsigned char*	lpSrc;			// ԭͼ������ָ��	
	long i,j;			//ѭ������

	//ԭͼ��Ҷȷֲ������ܶȱ���
	float fPro[256];
	//�м����
	float temp[256];
	BYTE nRst[256];
	
	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	// ��ʼ���м����temp
	memset(temp, 0, sizeof(temp));

	// ��ȡԭͼ��Ҷȷֲ��ĸ����ܶ�
	Histogram_Statistic(fPro);

	//����ֱ��ͼ���⻯����
	for(i = 0; i < 256; i++)
	{
		if(i == 0)
			temp[0] = fPro[0];	 
		else
			temp[i] = temp[i-1] + fPro[i];	 
		nRst[i] = (BYTE)(255.0f * temp[i] + 0.5f);
	}	

	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];
	
	//��ֱ��ͼ���⻯��Ľ��д��Ŀ��ͼ����
	for(j = 0; j <lHeight; j++)
		for(i = 0;i <lWidth; i++)
			lpDst[lLineBytes * j + i] = (nRst[lpSrc[lLineBytes * j + i]]);
	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpDst;
		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Histogram_Match()
{
	unsigned char*	lpSrc;			// ԭͼ������ָ��	
	long i,j;			//ѭ������

	//ԭͼ��Ҷȷֲ������ܶȱ���
	float fPro[256];
	//�м����
	float temp[256];
    //�Ҷ�ӳ������
	BYTE nMap[256];

	BYTE bGray=64;
    int npMap[64];  //�涨ֱ��ͼӳ���ϵ,����涨ֱ��ͼ�ĻҶȼ�Ϊ64
    float fpPro[64]; //�涨�Ҷȷֲ�����
    float a=1.0f/(32.0f*63.0f); 
    for(int i=0;i<64;i++)
    {
        npMap[i]=i*4;
        fpPro[i]=a*i;
    } 

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	// ��ʼ���м����temp
	memset(temp, 0, sizeof(temp));

	// ��ȡԭͼ��Ҷȷֲ��ĸ����ܶ�
	Histogram_Statistic(fPro);
	
	// ����ԭͼ���ۼ�ֱ��ͼ
	for (i = 0; i < 256; i++)
	{
		if (i == 0)
			temp[0] = fPro[0];			 
		else
			temp[i] = temp[i-1] + fPro[i];			 
		fPro[i] = temp[i];
	}

	// ����涨�任����ۼ�ֱ��ͼ
	for (i = 0; i < bGray; i++)
	{
		if (i == 0)
			temp[0] = fpPro[0];
		else
			temp[i] = temp[i-1] + fpPro[i];
		fpPro[i] = temp[i];
	}

	// ȷ��ӳ���ϵ
	for (i = 0; i < 256; i++)
	{
		// ��ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
		int m = 0;
		// ��С��ֵ
		float min_value = 1.0f;
		// ö�ٹ涨ֱ��ͼ���Ҷ�
		for (j = 0; j < bGray; j++)
		{
			// ��ǰ��ֵ
			float now_value = 0.0f;
			//  ��ֵ����
			if (fPro[i] - fpPro[j] >= 0.0f)
				now_value = fPro[i] - fpPro[j];
			else
				now_value = fpPro[j] - fPro[i];
			// Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
			if (now_value < min_value)
			{
				// ��ӽ��ĻҶȼ�
				m = j;
				// ��С��ֵ
				min_value = now_value;
			}
		}
		// �����Ҷ�ӳ���
		nMap[i] = npMap[m];
	}
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];
	
	// �Ը����ؽ���ֱ��ͼ�涨��ӳ�䴦��
	for (j = 0; j < lHeight; j ++)
		for (i = 0; i < lWidth; i ++)
			lpDst[lLineBytes * j + i] = (nMap[lpSrc[lLineBytes * j + i]]);

		// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpDst;
		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}

BOOL CDlgEnhance::Linear1( )
{
	unsigned char*	lpSrc;			// ԭͼ������ָ��
	long i,j;			//ѭ������
	BYTE pixel;	//����ֵ
    BYTE gMin,gMax;  //���ñ任��ĻҶ�����
    gMin=100;
    gMax=200;

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	//���ɨ��ͼ���е����ص�,���лҶ����Ա任
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE����
			pixel = (BYTE)lpSrc[lLineBytes * j + i];

			//���ݹ�ʽ(5-2)���Ŀ��ͼ�����뵱ǰ���Ӧ�����ص�ĻҶ�ֵ
			lpDst[lLineBytes * j + i] = (BYTE)(((float)(gMax-gMin)/255)*pixel+gMin+0.5);
		}
	}
	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpDst;
		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Linear2( )
{
	BYTE gSrc1,gSrc2, gDst1, gDst2;  //���÷ֶε�
    gSrc1=100;
    gSrc2=150;
    gDst1=50;
    gDst2=200;

	unsigned char*	lpSrc;			// ԭͼ������ָ��	
	long i,j;			//ѭ������
	BYTE pixel;	//����ֵ

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];
		//���ɨ��ͼ���е����ص�,���лҶȷֶ����Ա任
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE����
			pixel = (BYTE)lpSrc[lLineBytes * j + i];

			//���ݹ�ʽ(5-3)�ұ���ʽ���Ŀ��ͼ�����뵱ǰ���Ӧ�����ص�ĻҶ�ֵ
			if(pixel<gSrc1)
				lpDst[lLineBytes * j + i] = (BYTE)(((float)gDst1/gSrc1)*pixel+0.5);

			//���ݹ�ʽ(5-3)�ұ���ʽ���Ŀ��ͼ�����뵱ǰ���Ӧ�����ص�ĻҶ�ֵ
			if((pixel>=gSrc1)&&(pixel<=gSrc2))
				lpDst[lLineBytes * j + i] = (BYTE)(((float)(gDst2-gDst1)/(gSrc2-gSrc1))*(pixel-gSrc1)+gDst1+0.5);

			//���ݹ�ʽ(5-3)�ұ���ʽ���Ŀ��ͼ�����뵱ǰ���Ӧ�����ص�ĻҶ�ֵ
			if((pixel>gSrc2)&&(pixel<=255))
				lpDst[lLineBytes * j + i] = (BYTE)(((float)(255-gDst2)/(255-gSrc2))*(pixel-gSrc2)+gDst2+0.5);
		}
	}
	
	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpDst;
		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Linear3( )
{
	unsigned char*	lpSrc;			// ԭͼ������ָ��	
	long i,j;			//ѭ������
	BYTE pixel;	//����ֵ
    double a,b,c;  //���������任��������
    a=50.0;
    b=0.8;
    c=1.05;  

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	//���ɨ��ͼ���е����ص�,���ж������������ԻҶȱ任
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��ΪBYTE����
			pixel = (BYTE)lpSrc[lLineBytes * j + i];

			//���ݹ�ʽ���Ŀ��ͼ�����뵱ǰ���Ӧ�����ص�ĻҶ�ֵ
			lpDst[lLineBytes * j + i] = (BYTE)((log((double)(pixel+1)))/(b*log(c))+a+0.5);
		}
	}

	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpDst;
		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Smooth1( )
{
	unsigned char*	lpSrc;			// ԭͼ������ָ��
	long i,j;			//ѭ������

	BYTE average;	//�����ֵ����

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];	

	//���ɨ��ͼ���е����ص�,���������ֵ
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			 //��ǰ�㼰����Χ8����ľ�ֵ
			average=(BYTE)((float)(lpSrc[(j-1)*lLineBytes+(i-1)]+lpSrc[(j-1)*lLineBytes+i]+lpSrc[(j-1)*lLineBytes+(i+1)]
					+lpSrc[j*lLineBytes+(i-1)]+lpSrc[j*lLineBytes+i]+lpSrc[j*lLineBytes+i+1]
					+lpSrc[(j+1)*lLineBytes+(i-1)]+lpSrc[(j+1)*lLineBytes+i]+lpSrc[(j+1)*lLineBytes+i+1])/9+0.5);

			//����õľ�ֵ��ֵ��Ŀ��ͼ�����뵱ǰ���Ӧ�����ص�
			lpDst[lLineBytes * j + i] = average;
		}
	}

	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpDst;
		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Smooth2( )
{
	unsigned char*	lpSrc;			// ԭͼ������ָ��
	long i,j,m,n;			//ѭ������
	int sum=0; //ģ���и���Ԫ���ܺ�
	BYTE value_average;	//�����Ȩ��ֵ����
	int Structure[3][3]={1,2,1,2,4,2,1,2,1};//�����Ȩģ��

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	//��ģ���и�Ԫ��Ȩֵ�ܺ�
	for (m = 0;m < 3;m++ )
		for (n = 0;n < 3;n++)
			sum+=Structure[m][n];

	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	//���ɨ��ͼ���е����ص�,���������Ȩ��ֵ
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
            //���Ȩ��ֵ
			value_average=(BYTE)((float)(lpSrc[(j+1)*lLineBytes+(i-1)]*Structure[0][0]+lpSrc[(j+1)*lLineBytes+i]*Structure[0][1]
			                     +lpSrc[(j+1)*lLineBytes+i+1]*Structure[0][2]+lpSrc[j*lLineBytes+(i-1)]*Structure[1][0]
								 +lpSrc[j*lLineBytes+i]*Structure[1][1]+lpSrc[j*lLineBytes+i+1]*Structure[1][2]
					             +lpSrc[(j-1)*lLineBytes+(i-1)]*Structure[2][0]+lpSrc[(j-1)*lLineBytes+i]*Structure[2][1]
								 +lpSrc[(j-1)*lLineBytes+(i+1)]*Structure[2][2])/sum+0.5); 				
			//����õļ�Ȩ��ֵ��ֵ��Ŀ��ͼ�����뵱ǰ���Ӧ�����ص�
			lpDst[lLineBytes * j + i] = value_average;
		}
	}
	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpDst;
		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Smooth3( )
{
	unsigned char*	lpSrc;			// ԭͼ������ָ��
	int i,j,n;			//ѭ������

	BYTE pixel[9];	//������������ֵ
    float mean[9],var[9],varMin; //�����ֵ�����򷽲������Сֵ
    int nMin; //������Сʱ�������

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

    //��9������ľ�ֵ�뷽��
    for(j=2;j<=lHeight-3;j++)
    {
	    for(i=2;i<=lWidth-3;i++)
		{
		    //��1����
			pixel[0]=lpSrc[(j-1)*lLineBytes+(i-1)];
			pixel[1]=lpSrc[(j-1)*lLineBytes+i];
			pixel[2]=lpSrc[(j-1)*lLineBytes+(i+1)];
			pixel[3]=lpSrc[j*lLineBytes+(i-1)];
			pixel[4]=lpSrc[j*lLineBytes+i];
			pixel[5]=lpSrc[j*lLineBytes+(i+1)];
			pixel[6]=lpSrc[(j+1)*lLineBytes+(i-1)];
			pixel[7]=lpSrc[(j+1)*lLineBytes+i];
			pixel[8]=lpSrc[(j+1)*lLineBytes+(i+1)];
			mean[0]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6]+pixel[7]+pixel[8])/9;
			var[0]=0;
			for(n=0;n<=8;n++)
				var[0]+=pixel[n]*pixel[n]-mean[0]*mean[0];
			//��2����
			pixel[0]=lpSrc[(j-2)*lLineBytes+(i-1)];
			pixel[1]=lpSrc[(j-2)*lLineBytes+i];
			pixel[2]=lpSrc[(j-2)*lLineBytes+(i+1)];
			pixel[3]=lpSrc[(j-1)*lLineBytes+(i-1)];
			pixel[4]=lpSrc[(j-1)*lLineBytes+i];
			pixel[5]=lpSrc[(j-1)*lLineBytes+(i+1)];
			pixel[6]=lpSrc[j*lLineBytes+i];
			mean[1]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
			var[1]=0;
			for(n=0;n<=6;n++)
				var[1]+=pixel[n]*pixel[n]-mean[1]*mean[1];
			//��3����
			pixel[0]=lpSrc[(j-1)*lLineBytes+(i-2)];
			pixel[1]=lpSrc[(j-1)*lLineBytes+(i-1)];
			pixel[2]=lpSrc[j*lLineBytes+(i-2)];
			pixel[3]=lpSrc[j*lLineBytes+(i-1)];
			pixel[4]=lpSrc[j*lLineBytes+i];
			pixel[5]=lpSrc[(j+1)*lLineBytes+(i-2)];
			pixel[6]=lpSrc[(j+1)*lLineBytes+(i-1)];
			mean[2]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
			var[2]=0;
			for(n=0;n<=6;n++)
				var[2]+=pixel[n]*pixel[n]-mean[2]*mean[2];
			//��4����
			pixel[0]=lpSrc[j*lLineBytes+i];
			pixel[1]=lpSrc[(j+1)*lLineBytes+(i-1)];
			pixel[2]=lpSrc[(j+1)*lLineBytes+i];
			pixel[3]=lpSrc[(j+1)*lLineBytes+(i+1)];
			pixel[4]=lpSrc[(j+2)*lLineBytes+(i-1)];
			pixel[5]=lpSrc[(j+2)*lLineBytes+i];
			pixel[6]=lpSrc[(j+2)*lLineBytes+(i+1)];
			mean[3]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
	  		var[3]=0;
			for(n=0;n<=6;n++)
				var[3]+=pixel[n]*pixel[n]-mean[3]*mean[3];
			//��5����
			pixel[0]=lpSrc[(j-1)*lLineBytes+(i+1)];
			pixel[1]=lpSrc[(j-1)*lLineBytes+(i+2)];
			pixel[2]=lpSrc[j*lLineBytes+i];
			pixel[3]=lpSrc[j*lLineBytes+(i+1)];
			pixel[4]=lpSrc[j*lLineBytes+(i+2)];
			pixel[5]=lpSrc[(j+1)*lLineBytes+(i+1)];
			pixel[6]=lpSrc[(j+1)*lLineBytes+(i+2)]; 
			mean[4]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
			var[4]=0;
			for(n=0;n<=6;n++)
				var[4]+=pixel[n]*pixel[n]-mean[4]*mean[4];	
			//��6����
			pixel[0]=lpSrc[(j-2)*lLineBytes+(i+1)];
			pixel[1]=lpSrc[(j-2)*lLineBytes+(i+2)];
			pixel[2]=lpSrc[(j-1)*lLineBytes+i];
			pixel[3]=lpSrc[(j-1)*lLineBytes+(i+1)];
			pixel[4]=lpSrc[(j-1)*lLineBytes+(i+2)];
			pixel[5]=lpSrc[j*lLineBytes+i];
			pixel[6]=lpSrc[j*lLineBytes+(i+1)]; 
			mean[5]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
	  		var[5]=0;
			for(n=0;n<=6;n++)
				var[5]+=pixel[n]*pixel[n]-mean[5]*mean[5];
			//��7����
			pixel[0]=lpSrc[(j-2)*lLineBytes+(i-2)];
			pixel[1]=lpSrc[(j-2)*lLineBytes+(i-1)];
			pixel[2]=lpSrc[(j-1)*lLineBytes+(i-2)];
			pixel[3]=lpSrc[(j-1)*lLineBytes+(i-1)];
			pixel[4]=lpSrc[(j-1)*lLineBytes+i];
			pixel[5]=lpSrc[j*lLineBytes+(i-1)];
			pixel[6]=lpSrc[j*lLineBytes+i];
			mean[6]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
	 		var[6]=0;
			for(n=0;n<=6;n++)
				var[6]+=pixel[n]*pixel[n]-mean[6]*mean[6];
			//��8����
			pixel[0]=lpSrc[j*lLineBytes+(i-1)];
			pixel[1]=lpSrc[j*lLineBytes+i];
			pixel[2]=lpSrc[(j+1)*lLineBytes+(i-2)];
			pixel[3]=lpSrc[(j+1)*lLineBytes+(i-1)];
			pixel[4]=lpSrc[(j+1)*lLineBytes+i];
			pixel[5]=lpSrc[(j+2)*lLineBytes+(i-2)];
			pixel[6]=lpSrc[(j+2)*lLineBytes+(i-1)];
			mean[7]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
	  		var[7]=0;
			for(n=0;n<=6;n++)
				var[7]+=pixel[n]*pixel[n]-mean[7]*mean[7];
			//��9����
			pixel[0]=lpSrc[j*lLineBytes+i];
			pixel[1]=lpSrc[j*lLineBytes+(i+1)];
			pixel[2]=lpSrc[(j+1)*lLineBytes+i];
			pixel[3]=lpSrc[(j+1)*lLineBytes+(i+1)];
			pixel[4]=lpSrc[(j+1)*lLineBytes+(i+2)];
			pixel[5]=lpSrc[(j+2)*lLineBytes+(i+1)];
			pixel[6]=lpSrc[(j+2)*lLineBytes+(i+2)];
			mean[8]=(float)(pixel[0]+pixel[1]+pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6])/7;
	  		var[8]=0;
			for(n=0;n<=6;n++)
				var[8]+=pixel[n]*pixel[n]-mean[8]*mean[8];

			//�󷽲���С������nMin
			varMin=var[0];
			nMin=0;
			for(n=0;n<=8;n++)
				if(varMin>var[n])
				{
					varMin=var[n];
					nMin=n;
				}
            //��������С�������ֵ��ֵ��Ŀ�����ص�
            lpDst[lLineBytes * j + i] = (BYTE)(mean[nMin]+0.5);  
		}
    }
	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpDst;
		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Smooth4( )
{
	unsigned char*	lpSrc;			// ԭͼ������ָ��
	int i,j,x,y,m;			//ѭ������
	int flag=1;  //ѭ����־����
	BYTE pixel[10],mid; //��������ֵ����ֵ
	BYTE temp;//�м����

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	//��ֵ�˲�
	for(j=1;j<lHeight-1;j++)
	{
		for(i=1;i<lWidth-1;i++)
		{
			//��3*3���δ��ڵ���������ֵ����pixel[m]
			m=0;
			for(y=j-1;y<=j+1;y++)
				for(x=i-1;x<=i+1;x++)
				{
					pixel[m]=lpSrc[y*lLineBytes+x];
					m++;
				}
			//��pixel[m]�е�ֵ����������
			do{
				flag=0;
				for(m=0;m<9;m++)
					if(pixel[m]<pixel[m+1])
					{
						temp=pixel[m];
						pixel[m]=pixel[m+1];
						pixel[m+1]=temp;
						flag=1;
					}

			}while(flag==1);

			//����ֵmid				
			mid=pixel[4];
            //����ֵ����Ŀ��ͼ��ĵ�ǰ��
            lpDst[lLineBytes * j + i] = mid;
		}
	}
	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpDst;
		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Sharpen1( )
{
	unsigned char*	lpSrc;			// ԭͼ������ָ��	
	long i,j;			//ѭ������
	BYTE t=20;
    BYTE temp; //�ݴ�˫��һ��΢�ֽ��

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();
	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));
	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];
	
	//���ɨ��ͼ���е����ص�,���������ݶ��񻯴���
	for(j=1;j<lHeight-1;j++)
    {
		for(i=1;i<lWidth-1;i++)
		{
			//����˫��һ��΢�ֹ�ʽ���㵱ǰ���صĻҶ�ֵ
			temp=(BYTE)sqrt((float)((lpSrc[lLineBytes*j+i]-lpSrc[lLineBytes*j+(i-1)])*(lpSrc[lLineBytes*j+i]-lpSrc[lLineBytes*j+(i-1)])
                +(lpSrc[lLineBytes*j+i]-lpSrc[lLineBytes*(j-1)+i])*(lpSrc[lLineBytes*j+i]-lpSrc[lLineBytes*(j-1)+i])));
			if (temp>=t)
			{   
				if((temp+100)>255)
					lpDst[lLineBytes*j+i]=255;
				else 
					lpDst[lLineBytes*j+i]=temp+100;
			}
			if (temp<t)
				lpDst[lLineBytes*j+i]=lpSrc[lLineBytes*j+i];
		}
    }
	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpDst;
		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Sharpen2( )
{
	int tempH;  //ģ��߶�
	int tempW;  //ģ����
	float tempC;  //ģ��ϵ��
	int tempMY;   //ģ������Ԫ��Y����
	int tempMX;   //ģ������Ԫ��X����
	float Template[9];   //ģ������	

	//����������˹ģ�����
	tempW=3;
	tempH=3;
	tempC=1.0;
	tempMY=1;
	tempMX=1;
	Template[0]=-1.0;
    Template[1]=-1.0;
	Template[2]=-1.0;
	Template[3]=-1.0;
	Template[4]=9.0;
	Template[5]=-1.0;
	Template[6]=-1.0;
	Template[7]=-1.0;
	Template[8]=-1.0;
	//���þ������
	Convolution(tempH,tempW,tempMX,tempMY,Template,tempC); 
	return TRUE;
}
BOOL CDlgEnhance::Convolution(int tempH, int tempW, int tempMX, int tempMY, float *fpTempArray, float fCoef)
{
	unsigned char*	lpSrc;			// ԭͼ������ָ��	
	long i,j,k,l;			//ѭ������
	float fResult;    //�ݴ�����м���     

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();
	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));
	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	// �����ڴ���ϲ����ͼ��
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];
	
	//���ɨ��ͼ���е����ص�,���о������
	for(j=tempMY;j<lHeight-tempH+tempMY+1;j++)
    {
		for(i=tempMX;i<lWidth-tempW+tempMX+1;i++)
		{
			//��������ֵ
			fResult=0;
			for(k=0;k<tempH;k++)
				for(l=0;l<tempW;l++)
					fResult=fResult+lpSrc[(j-tempMY+k)*lLineBytes+(i-tempMX+l)]*fpTempArray[k*tempW+l];
			//����ϵ��
			fResult*=fCoef;
			//ȡ����ֵ
			fResult=(float)fabs(fResult);
			//�ж��Ƿ񳬹�255
			if(fResult>255)
				//������255��ֱ�Ӹ�ֵΪ255
                lpDst[j*lLineBytes+i]=255;
			else
				//δ����255����ֵΪ������
				lpDst[j*lLineBytes+i]=(BYTE)(fResult+0.5);
		}
    }

	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpDst;
		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;  
}

BOOL CDlgEnhance::Color( )
{     
	int i;					// ѭ������
	unsigned char*	lpSrc;			// ԭͼ������ָ��
	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //���ԭͼ��Ŀ��
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //���ԭͼ��ĸ߶�
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);

	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();
	
	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpSrc;

	DWORD wNumColors;		// ��ɫ���е���ɫ��Ŀ
	wNumColors =m_pDibResult->m_lpBMIH->biBitCount;	// ��ȡԭͼ����ɫ���е���ɫ��Ŀ
	LPRGBQUAD m_lpRgbQuad;
	m_lpRgbQuad=(LPRGBQUAD) (m_pDibResult->m_lpvColorTable);

	if (wNumColors == 8)				// �ж���ɫ��Ŀ�Ƿ���256ɫ
		// ��ȡα��ɫ���룬���µ�ɫ��
		for (i = 0; i < 256; i++)
		{    
            // ���µ�ɫ���ɫ��ɫ��ɫ����
		    (m_lpRgbQuad+i)->rgbBlue = ColorsTable[8][i][0];
			(m_lpRgbQuad+i)->rgbGreen = ColorsTable[8][i][1];
			(m_lpRgbQuad+i)->rgbRed = ColorsTable[8][i][2];				
			// ���µ�ɫ�屣��λ
			(m_lpRgbQuad+i)->rgbReserved = 0; 			
		}

		// ������ʾ
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}