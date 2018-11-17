// DlgEnhance.cpp : implementation file
//

#include "stdafx.h"
#include "遥感图像配准系统.h"
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

	// 获取文档类指针
	m_pDoc = pDoc;

	// 设置计算图像位置标志位位FALSE
	m_bCalImgLoc = FALSE;

	// 设置基准图像为原始打开的图像
	m_pDibInit = pDoc->m_pDibInit;

	// 设置待配准图像
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
	
	// 如果还没有计算图像的位置，则进行计算
	if(!m_bCalImgLoc)
		CalImageLocation();

	CSize sizeDisplay;
	CPoint pointDisplay;
	
	// 显示基准图像
	if(!m_pDibInit->IsEmpty())
	{
		sizeDisplay.cx = IMG_INIT_W;
		sizeDisplay.cy = IMG_INIT_H;
		pointDisplay.x = m_rectInitImage.left;
		pointDisplay.y = m_rectInitImage.top;


		m_pDibInit->Draw(&dc, pointDisplay, sizeDisplay);
	}
	
	// 显示增强后图像
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
 * \函数名称：
 *   CalImageLocation()
 * \输入参数:无
 * \返回值:无
 * \说明:
 *该函数设置对话框中的控件位置和大小，并设置显示图像的位置。默认的图像大小为352×288，如果图像小于
 *此大小，则控件大小设置为352×288，并将图像放置在控件中间。
 *************************************************************************/
void CDlgEnhance::CalImageLocation()
{
	// 获得控件IDC_REG_INIT_IMAGE的句柄，并获得控件的初始位置信息
	CWnd* pWnd=GetDlgItem(IDC_ENHANCE_INIT_IMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement=new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);
	
	// 图像宽度
	int nImageWidth1;
	nImageWidth1= m_pDibInit->m_lpBMIH->biWidth;

	// 图像高
	int nImageHeight1;
	nImageHeight1 = m_pDibInit->m_lpBMIH->biHeight;
	
	// 调整控件IDC_ENHANCE_INIT_IMAGE的大小位置，并同时设置显示基准图像的位置
	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + IMG_INIT_H;
	m_rectInitImage.bottom = winPlacement->rcNormalPosition.bottom;
	m_rectInitImage.top = winPlacement->rcNormalPosition.top;

	winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + IMG_INIT_W;
	m_rectInitImage.right = winPlacement->rcNormalPosition.right;
	m_rectInitImage.left = winPlacement->rcNormalPosition.left;

	// 设置IDC_ENHANCE_INIT_IMAGE控件的大小位置
	pWnd->SetWindowPlacement(winPlacement);	
			
	// 获得显示基准图像控件的右边位置，以便确认显示待配准图像控件的位置
	int nIniImgRight;
	nIniImgRight = winPlacement->rcNormalPosition.right;
	
	int  nIniImgLeft;
	nIniImgLeft   = winPlacement->rcNormalPosition.left;

	// 获得IDC_ENHANCE_INIT_IMAGE控件的下边位置，以便调整其他控件的位置
	int nIniImgBottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;

	// 获得控件IDC_ENHANCE_RESLT_IMAGE的句柄，并获得初始位置信息
	pWnd=GetDlgItem(IDC_ENHANCE_RESLT_IMAGE);
	pWnd->GetWindowPlacement(winPlacement);
	
	int nImageHeight2,nImageWidth2;
	// 如果还未打开待配准图像，则设置待配准图像大小和基准图像大小相等
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
	// 调整控件IDC_ENHANCE_RESLT_IMAGE的大小位置，并同时设置显示待配准图像的位置

	// 先调整控件的左边位置，和IDC_ENHANCE_INIT_IMAGE控件相隔15个象素
	winPlacement->rcNormalPosition.left = nIniImgRight + 15;

	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + IMG_SAMP_H;
	m_rectResltImage.bottom = winPlacement->rcNormalPosition.bottom;
	m_rectResltImage.top = winPlacement->rcNormalPosition.top;

	winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + IMG_SAMP_W;
	m_rectResltImage.right = winPlacement->rcNormalPosition.right;
	m_rectResltImage.left = winPlacement->rcNormalPosition.left;

	// 设置IDC_ENHANCE_RESLT_IMAGE控件的大小位置
	pWnd->SetWindowPlacement(winPlacement);
	
	if(nIniImgBottom < winPlacement->rcNormalPosition.bottom)
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	nIniImgRight  = winPlacement->rcNormalPosition.right;
	
	if(nImageHeight2<nImageHeight1)
	{
		 // 获得控件IDC_ENHANCE_INIT_IMAGE的句柄，并获得控件的初始位置信息
	CWnd* pWnd=GetDlgItem(IDC_ENHANCE_INIT_IMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement=new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);
	if(nIniImgBottom < winPlacement->rcNormalPosition.bottom)
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	}	
		// 设置控件IDC_STATIC_BASEENHANCE的位置大小
	pWnd=GetDlgItem(IDC_STATIC_BASEENHANCE);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.left = (m_rectInitImage.right+m_rectInitImage.left)/2-35;
	winPlacement->rcNormalPosition.right = (m_rectInitImage.right+m_rectInitImage.left)/2+35;
	pWnd->SetWindowPlacement(winPlacement);

	// 设置控件IDC_STATIC_ENHANCE的位置大小
	pWnd=GetDlgItem(IDC_STATIC_ENHANCE);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.left = (m_rectResltImage.right+m_rectResltImage.left)/2-45;
	winPlacement->rcNormalPosition.right = (m_rectResltImage.right+m_rectResltImage.left)/2+45;
	pWnd->SetWindowPlacement(winPlacement);

	// 设置控件IDOK的位置大小
	pWnd=GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +95;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 120;
	pWnd->SetWindowPlacement(winPlacement);
	
	// 设置控件IDCANCEL的位置大小
	pWnd=GetDlgItem(IDCANCEL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +95;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 120;
	pWnd->SetWindowPlacement(winPlacement);

		// 设置控件IDC_BUTTON_HISTOGRAM的位置大小
	pWnd=GetDlgItem(IDC_BUTTON_HISTOGRAM);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+5 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 25;
	pWnd->SetWindowPlacement(winPlacement);

		// 设置控件IDC_HISTOGRAM_EQUAL的位置大小
	pWnd=GetDlgItem(IDC_HISTOGRAM_EQUAL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+30 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 44;
	pWnd->SetWindowPlacement(winPlacement);
		// 设置控件IDC_HISTOGRAM_MATCH的位置大小
	pWnd=GetDlgItem(IDC_HISTOGRAM_MATCH);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+46 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);

		// 设置控件IDC_BUTTON_LINEAR的位置大小
	pWnd=GetDlgItem(IDC_BUTTON_LINEAR);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+5 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 25;
	pWnd->SetWindowPlacement(winPlacement);

		// 设置控件IDC_LINEAR_1的位置大小
	pWnd=GetDlgItem(IDC_LINEAR_1);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+30 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 44;
	pWnd->SetWindowPlacement(winPlacement);
		// 设置控件IDC_LINEAR_2的位置大小
	pWnd=GetDlgItem(IDC_LINEAR_2);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+46 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
			// 设置控件IDC_LINEAR_3的位置大小
	pWnd=GetDlgItem(IDC_LINEAR_3);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+62 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 76;
	pWnd->SetWindowPlacement(winPlacement);

		// 设置控件IDC_BUTTON_SMOOTH的位置大小
	pWnd=GetDlgItem(IDC_BUTTON_SMOOTH);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+5 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 25;
	pWnd->SetWindowPlacement(winPlacement);

		// 设置控件IDC_SMOOTH_1的位置大小
	pWnd=GetDlgItem(IDC_SMOOTH_1);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+30 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 44;
	pWnd->SetWindowPlacement(winPlacement);
		// 设置控件IDC_SMOOTH_2的位置大小
	pWnd=GetDlgItem(IDC_SMOOTH_2);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+46 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	        // 设置控件IDC_SMOOTH_3的位置大小
	pWnd=GetDlgItem(IDC_SMOOTH_3);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+62 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 76;
	pWnd->SetWindowPlacement(winPlacement);
		// 设置控件IDC_SMOOTH_4的位置大小
	pWnd=GetDlgItem(IDC_SMOOTH_4);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+78 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 92;
	pWnd->SetWindowPlacement(winPlacement);

		// 设置控件IDC_BUTTON_SHARPEN的位置大小
	pWnd=GetDlgItem(IDC_BUTTON_SHARPEN);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+5 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 25;
	pWnd->SetWindowPlacement(winPlacement);

		// 设置控件IDC_SHARPEN_1的位置大小
	pWnd=GetDlgItem(IDC_SHARPEN_1);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+30 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 44;
	pWnd->SetWindowPlacement(winPlacement);
		// 设置控件IDC_SHARPEN_2的位置大小
	pWnd=GetDlgItem(IDC_SHARPEN_2);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+46 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);

		// 设置控件IDC_BUTTON_COLOR的位置大小
	pWnd=GetDlgItem(IDC_BUTTON_COLOR);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom+5 ;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 25;
	pWnd->SetWindowPlacement(winPlacement);

	// 调整此对话框的大小
	this->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 300;
	winPlacement->rcNormalPosition.left   = nIniImgLeft   - 20;
	winPlacement->rcNormalPosition.right  = nIniImgRight  + 50;
	this->SetWindowPlacement(winPlacement);

	// 释放已分配内存
	delete winPlacement;

	// 设置计算图像控件位置标志位为TRUE
	m_bCalImgLoc = TRUE;
}

void CDlgEnhance::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
void CDlgEnhance::OnBnClickedHistogramEqual()
{
	// TODO: 在此添加控件通知处理程序代码
	Histogram_mode=1;
}

void CDlgEnhance::OnBnClickedHistogramMatch()
{
	// TODO: 在此添加控件通知处理程序代码
	Histogram_mode=2;
}

void CDlgEnhance::OnBnClickedButtonHistogram()
{
	// TODO: 在此添加控件通知处理程序代码
	if(Histogram_mode==1)
		Histogram_Equalization();
	else
		Histogram_Match();
}
void CDlgEnhance::OnBnClickedLinear1()
{
	// TODO: 在此添加控件通知处理程序代码
	Linear_mode=1;
}
void CDlgEnhance::OnBnClickedLinear2()
{
	// TODO: 在此添加控件通知处理程序代码
	Linear_mode=2;
}
void CDlgEnhance::OnBnClickedLinear3()
{
	// TODO: 在此添加控件通知处理程序代码
	Linear_mode=3;
}
void CDlgEnhance::OnBnClickedButtonLinear()
{
	// TODO: 在此添加控件通知处理程序代码
	switch(Linear_mode)
	{
	case 1: Linear1(); break;
	case 2: Linear2(); break;
	case 3: Linear3(); break;
	}
}

void CDlgEnhance::OnBnClickedSmooth1()
{
	// TODO: 在此添加控件通知处理程序代码
	Smooth_mode=1;
}
void CDlgEnhance::OnBnClickedSmooth2()
{
	// TODO: 在此添加控件通知处理程序代码
	Smooth_mode=2;
}
void CDlgEnhance::OnBnClickedSmooth3()
{
	// TODO: 在此添加控件通知处理程序代码
	Smooth_mode=3;
}
void CDlgEnhance::OnBnClickedSmooth4()
{
	// TODO: 在此添加控件通知处理程序代码
	Smooth_mode=4;
}
void CDlgEnhance::OnBnClickedButtonSmooth()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	Sharpen_mode=1;
}
void CDlgEnhance::OnBnClickedSharpen2()
{
	// TODO: 在此添加控件通知处理程序代码
	Sharpen_mode=2;
}
void CDlgEnhance::OnBnClickedButtonSharpen()
{
	// TODO: 在此添加控件通知处理程序代码
	switch(Sharpen_mode)
	{
	case 1: Sharpen1(); break;
	case 2: Sharpen2(); break;
	}
}

void CDlgEnhance::OnBnClickedButtonColor()
{
	// TODO: 在此添加控件通知处理程序代码
	Color();
}

//=======================================================
// 函数功能： 直方图统计函数
// 输入参数： float *probability -灰度分布概率密度
// 返回值：   无
//=======================================================
void CDlgEnhance::Histogram_Statistic( float *probability)
{
	LPBYTE	lpSrc;			// 指向原图像的指针	
	long i,j;			//循环变量	
	int gray[256]; // 灰度计数
	BYTE pixel;	//像素值
		// 基准图像数据指针	
	unsigned char* lpDIBBits;
	lpDIBBits = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	// 灰度计数变量初始化
	memset(gray,0,sizeof(gray));
	//逐个扫面图像中的像素点,进行灰度计数统计
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			// 指向原图像倒数第j行，第i个像素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lLineBytes * j + i;
			//取得当前指针处的像素值，注意要转换为BYTE类型
			pixel = (BYTE)*lpSrc;
			// 灰度统计计数
			gray[pixel]++;
		}
	}
	// 计算灰度概率密度
	for(i=0;i<256;i++)
		probability[i] = gray[i] / (lHeight * lWidth *1.0f);
}

BOOL CDlgEnhance::Histogram_Equalization( )
{
	unsigned char*	lpSrc;			// 原图像数据指针	
	long i,j;			//循环变量

	//原图像灰度分布概率密度变量
	float fPro[256];
	//中间变量
	float temp[256];
	BYTE nRst[256];
	
	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	// 初始化中间变量temp
	memset(temp, 0, sizeof(temp));

	// 获取原图像灰度分布的概率密度
	Histogram_Statistic(fPro);

	//进行直方图均衡化处理
	for(i = 0; i < 256; i++)
	{
		if(i == 0)
			temp[0] = fPro[0];	 
		else
			temp[i] = temp[i-1] + fPro[i];	 
		nRst[i] = (BYTE)(255.0f * temp[i] + 0.5f);
	}	

	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	// 分配内存给合并后的图像
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];
	
	//将直方图均衡化后的结果写到目标图像中
	for(j = 0; j <lHeight; j++)
		for(i = 0;i <lWidth; i++)
			lpDst[lLineBytes * j + i] = (nRst[lpSrc[lLineBytes * j + i]]);
	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpDst;
		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Histogram_Match()
{
	unsigned char*	lpSrc;			// 原图像数据指针	
	long i,j;			//循环变量

	//原图像灰度分布概率密度变量
	float fPro[256];
	//中间变量
	float temp[256];
    //灰度映射表变量
	BYTE nMap[256];

	BYTE bGray=64;
    int npMap[64];  //规定直方图映射关系,这里规定直方图的灰度级为64
    float fpPro[64]; //规定灰度分布概率
    float a=1.0f/(32.0f*63.0f); 
    for(int i=0;i<64;i++)
    {
        npMap[i]=i*4;
        fpPro[i]=a*i;
    } 

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	// 初始化中间变量temp
	memset(temp, 0, sizeof(temp));

	// 获取原图像灰度分布的概率密度
	Histogram_Statistic(fPro);
	
	// 计算原图像累计直方图
	for (i = 0; i < 256; i++)
	{
		if (i == 0)
			temp[0] = fPro[0];			 
		else
			temp[i] = temp[i-1] + fPro[i];			 
		fPro[i] = temp[i];
	}

	// 计算规定变换后的累计直方图
	for (i = 0; i < bGray; i++)
	{
		if (i == 0)
			temp[0] = fpPro[0];
		else
			temp[i] = temp[i-1] + fpPro[i];
		fpPro[i] = temp[i];
	}

	// 确定映射关系
	for (i = 0; i < 256; i++)
	{
		// 最接近的规定直方图灰度级
		int m = 0;
		// 最小差值
		float min_value = 1.0f;
		// 枚举规定直方图各灰度
		for (j = 0; j < bGray; j++)
		{
			// 当前差值
			float now_value = 0.0f;
			//  差值计算
			if (fPro[i] - fpPro[j] >= 0.0f)
				now_value = fPro[i] - fpPro[j];
			else
				now_value = fpPro[j] - fPro[i];
			// 寻找最接近的规定直方图灰度级
			if (now_value < min_value)
			{
				// 最接近的灰度级
				m = j;
				// 最小差值
				min_value = now_value;
			}
		}
		// 建立灰度映射表
		nMap[i] = npMap[m];
	}
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	// 分配内存给合并后的图像
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];
	
	// 对各像素进行直方图规定化映射处理
	for (j = 0; j < lHeight; j ++)
		for (i = 0; i < lWidth; i ++)
			lpDst[lLineBytes * j + i] = (nMap[lpSrc[lLineBytes * j + i]]);

		// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpDst;
		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}

BOOL CDlgEnhance::Linear1( )
{
	unsigned char*	lpSrc;			// 原图像数据指针
	long i,j;			//循环变量
	BYTE pixel;	//像素值
    BYTE gMin,gMax;  //设置变换后的灰度区间
    gMin=100;
    gMax=200;

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	// 分配内存给合并后的图像
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	//逐个扫面图像中的像素点,进行灰度线性变换
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			//取得当前指针处的像素值，注意要转换为BYTE类型
			pixel = (BYTE)lpSrc[lLineBytes * j + i];

			//根据公式(5-2)求出目标图像中与当前点对应的像素点的灰度值
			lpDst[lLineBytes * j + i] = (BYTE)(((float)(gMax-gMin)/255)*pixel+gMin+0.5);
		}
	}
	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpDst;
		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Linear2( )
{
	BYTE gSrc1,gSrc2, gDst1, gDst2;  //设置分段点
    gSrc1=100;
    gSrc2=150;
    gDst1=50;
    gDst2=200;

	unsigned char*	lpSrc;			// 原图像数据指针	
	long i,j;			//循环变量
	BYTE pixel;	//像素值

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	// 分配内存给合并后的图像
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];
		//逐个扫面图像中的像素点,进行灰度分段线性变换
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			//取得当前指针处的像素值，注意要转换为BYTE类型
			pixel = (BYTE)lpSrc[lLineBytes * j + i];

			//根据公式(5-3)右边上式求出目标图像中与当前点对应的像素点的灰度值
			if(pixel<gSrc1)
				lpDst[lLineBytes * j + i] = (BYTE)(((float)gDst1/gSrc1)*pixel+0.5);

			//根据公式(5-3)右边中式求出目标图像中与当前点对应的像素点的灰度值
			if((pixel>=gSrc1)&&(pixel<=gSrc2))
				lpDst[lLineBytes * j + i] = (BYTE)(((float)(gDst2-gDst1)/(gSrc2-gSrc1))*(pixel-gSrc1)+gDst1+0.5);

			//根据公式(5-3)右边下式求出目标图像中与当前点对应的像素点的灰度值
			if((pixel>gSrc2)&&(pixel<=255))
				lpDst[lLineBytes * j + i] = (BYTE)(((float)(255-gDst2)/(255-gSrc2))*(pixel-gSrc2)+gDst2+0.5);
		}
	}
	
	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpDst;
		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Linear3( )
{
	unsigned char*	lpSrc;			// 原图像数据指针	
	long i,j;			//循环变量
	BYTE pixel;	//像素值
    double a,b,c;  //对数函数变换参数设置
    a=50.0;
    b=0.8;
    c=1.05;  

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	// 分配内存给合并后的图像
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	//逐个扫面图像中的像素点,进行对数函数非线性灰度变换
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			//取得当前指针处的像素值，注意要转换为BYTE类型
			pixel = (BYTE)lpSrc[lLineBytes * j + i];

			//根据公式求出目标图像中与当前点对应的像素点的灰度值
			lpDst[lLineBytes * j + i] = (BYTE)((log((double)(pixel+1)))/(b*log(c))+a+0.5);
		}
	}

	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpDst;
		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Smooth1( )
{
	unsigned char*	lpSrc;			// 原图像数据指针
	long i,j;			//循环变量

	BYTE average;	//领域均值变量

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	// 分配内存给合并后的图像
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];	

	//逐个扫描图像中的像素点,求其邻域均值
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			 //求当前点及其周围8个点的均值
			average=(BYTE)((float)(lpSrc[(j-1)*lLineBytes+(i-1)]+lpSrc[(j-1)*lLineBytes+i]+lpSrc[(j-1)*lLineBytes+(i+1)]
					+lpSrc[j*lLineBytes+(i-1)]+lpSrc[j*lLineBytes+i]+lpSrc[j*lLineBytes+i+1]
					+lpSrc[(j+1)*lLineBytes+(i-1)]+lpSrc[(j+1)*lLineBytes+i]+lpSrc[(j+1)*lLineBytes+i+1])/9+0.5);

			//将求得的均值赋值给目标图像中与当前点对应的像素点
			lpDst[lLineBytes * j + i] = average;
		}
	}

	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpDst;
		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Smooth2( )
{
	unsigned char*	lpSrc;			// 原图像数据指针
	long i,j,m,n;			//循环变量
	int sum=0; //模板中各个元素总和
	BYTE value_average;	//领域加权均值变量
	int Structure[3][3]={1,2,1,2,4,2,1,2,1};//定义加权模板

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	//求模板中各元素权值总和
	for (m = 0;m < 3;m++ )
		for (n = 0;n < 3;n++)
			sum+=Structure[m][n];

	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	// 分配内存给合并后的图像
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	//逐个扫描图像中的像素点,求其邻域加权均值
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
            //求加权均值
			value_average=(BYTE)((float)(lpSrc[(j+1)*lLineBytes+(i-1)]*Structure[0][0]+lpSrc[(j+1)*lLineBytes+i]*Structure[0][1]
			                     +lpSrc[(j+1)*lLineBytes+i+1]*Structure[0][2]+lpSrc[j*lLineBytes+(i-1)]*Structure[1][0]
								 +lpSrc[j*lLineBytes+i]*Structure[1][1]+lpSrc[j*lLineBytes+i+1]*Structure[1][2]
					             +lpSrc[(j-1)*lLineBytes+(i-1)]*Structure[2][0]+lpSrc[(j-1)*lLineBytes+i]*Structure[2][1]
								 +lpSrc[(j-1)*lLineBytes+(i+1)]*Structure[2][2])/sum+0.5); 				
			//将求得的加权均值赋值给目标图像中与当前点对应的像素点
			lpDst[lLineBytes * j + i] = value_average;
		}
	}
	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpDst;
		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Smooth3( )
{
	unsigned char*	lpSrc;			// 原图像数据指针
	int i,j,n;			//循环变量

	BYTE pixel[9];	//领域各点的像素值
    float mean[9],var[9],varMin; //邻域均值，邻域方差，方差最小值
    int nMin; //方差最小时的邻域号

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	// 分配内存给合并后的图像
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

    //求9种邻域的均值与方差
    for(j=2;j<=lHeight-3;j++)
    {
	    for(i=2;i<=lWidth-3;i++)
		{
		    //第1邻域
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
			//第2邻域
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
			//第3邻域
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
			//第4邻域
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
			//第5邻域
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
			//第6邻域
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
			//第7邻域
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
			//第8邻域
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
			//第9邻域
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

			//求方差最小的邻域nMin
			varMin=var[0];
			nMin=0;
			for(n=0;n<=8;n++)
				if(varMin>var[n])
				{
					varMin=var[n];
					nMin=n;
				}
            //将方差最小的邻域均值赋值给目标像素点
            lpDst[lLineBytes * j + i] = (BYTE)(mean[nMin]+0.5);  
		}
    }
	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpDst;
		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Smooth4( )
{
	unsigned char*	lpSrc;			// 原图像数据指针
	int i,j,x,y,m;			//循环变量
	int flag=1;  //循环标志变量
	BYTE pixel[10],mid; //窗口像素值及中值
	BYTE temp;//中间变量

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	
	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	// 分配内存给合并后的图像
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	//中值滤波
	for(j=1;j<lHeight-1;j++)
	{
		for(i=1;i<lWidth-1;i++)
		{
			//把3*3屏蔽窗口的所有像素值放入pixel[m]
			m=0;
			for(y=j-1;y<=j+1;y++)
				for(x=i-1;x<=i+1;x++)
				{
					pixel[m]=lpSrc[y*lLineBytes+x];
					m++;
				}
			//把pixel[m]中的值按降序排序
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

			//求中值mid				
			mid=pixel[4];
            //将中值赋给目标图像的当前点
            lpDst[lLineBytes * j + i] = mid;
		}
	}
	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpDst;
		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Sharpen1( )
{
	unsigned char*	lpSrc;			// 原图像数据指针	
	long i,j;			//循环变量
	BYTE t=20;
    BYTE temp; //暂存双向一次微分结果

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();
	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));
	// 应用调色板
	m_pDibResult->MakePalette();
	// 分配内存给合并后的图像
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];
	
	//逐个扫描图像中的像素点,进行门限梯度锐化处理
	for(j=1;j<lHeight-1;j++)
    {
		for(i=1;i<lWidth-1;i++)
		{
			//根据双向一次微分公式计算当前像素的灰度值
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
	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpDst;
		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}
BOOL CDlgEnhance::Sharpen2( )
{
	int tempH;  //模板高度
	int tempW;  //模板宽度
	float tempC;  //模板系数
	int tempMY;   //模板中心元素Y坐标
	int tempMX;   //模板中心元素X坐标
	float Template[9];   //模板数组	

	//设置拉普拉斯模板参数
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
	//调用卷积函数
	Convolution(tempH,tempW,tempMX,tempMY,Template,tempC); 
	return TRUE;
}
BOOL CDlgEnhance::Convolution(int tempH, int tempW, int tempMX, int tempMY, float *fpTempArray, float fCoef)
{
	unsigned char*	lpSrc;			// 原图像数据指针	
	long i,j,k,l;			//循环变量
	float fResult;    //暂存计算中间结果     

	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);
	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();
	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));
	// 应用调色板
	m_pDibResult->MakePalette();
	// 分配内存给合并后的图像
	LPBYTE lpDst;
	lpDst = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];
	
	//逐个扫描图像中的像素点,进行卷积运算
	for(j=tempMY;j<lHeight-tempH+tempMY+1;j++)
    {
		for(i=tempMX;i<lWidth-tempW+tempMX+1;i++)
		{
			//计算像素值
			fResult=0;
			for(k=0;k<tempH;k++)
				for(l=0;l<tempW;l++)
					fResult=fResult+lpSrc[(j-tempMY+k)*lLineBytes+(i-tempMX+l)]*fpTempArray[k*tempW+l];
			//乘上系数
			fResult*=fCoef;
			//取绝对值
			fResult=(float)fabs(fResult);
			//判断是否超过255
			if(fResult>255)
				//若超过255，直接赋值为255
                lpDst[j*lLineBytes+i]=255;
			else
				//未超过255，赋值为计算结果
				lpDst[j*lLineBytes+i]=(BYTE)(fResult+0.5);
		}
    }

	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpDst;
		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;  
}

BOOL CDlgEnhance::Color( )
{     
	int i;					// 循环变量
	unsigned char*	lpSrc;			// 原图像数据指针
	lpSrc = (unsigned char *)m_pDibInit->m_lpImage;
	LONG lWidth=m_pDibInit->m_lpBMIH->biWidth;   //获得原图像的宽度
	LONG lHeight=m_pDibInit->m_lpBMIH->biHeight;    //获得原图像的高度
	LONG lLineBytes=WIDTHBYTES(lWidth*8);
	m_pDibResult = new CDib(CSize(lWidth,lHeight), 8);

	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpSrc;

	DWORD wNumColors;		// 颜色表中的颜色数目
	wNumColors =m_pDibResult->m_lpBMIH->biBitCount;	// 获取原图像颜色表中的颜色数目
	LPRGBQUAD m_lpRgbQuad;
	m_lpRgbQuad=(LPRGBQUAD) (m_pDibResult->m_lpvColorTable);

	if (wNumColors == 8)				// 判断颜色数目是否是256色
		// 读取伪彩色编码，更新调色板
		for (i = 0; i < 256; i++)
		{    
            // 更新调色板红色绿色蓝色分量
		    (m_lpRgbQuad+i)->rgbBlue = ColorsTable[8][i][0];
			(m_lpRgbQuad+i)->rgbGreen = ColorsTable[8][i][1];
			(m_lpRgbQuad+i)->rgbRed = ColorsTable[8][i][2];				
			// 更新调色板保留位
			(m_lpRgbQuad+i)->rgbReserved = 0; 			
		}

		// 更新显示
	this->UpdateData();
	this->Invalidate();
	return TRUE;
}