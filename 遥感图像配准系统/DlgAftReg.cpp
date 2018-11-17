// DlgAftReg.cpp : implementation file
//

#include "stdafx.h"
#include "遥感图像配准系统.h"
#include "DlgAftReg.h"
#include "CDib.h"
#include "RSIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAftReg dialog


CDlgAftReg::CDlgAftReg(CWnd* pParent /*=NULL*/, CDib* pDibShow)
	: CDialog(CDlgAftReg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAftReg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDib = pDibShow;
	m_bCalImgLoc = FALSE;

	alpha_init = 1.0;
	alpha_samp = 1.0;
}


void CDlgAftReg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAftReg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAftReg, CDialog)
	//{{AFX_MSG_MAP(CDlgAftReg)
	ON_WM_PAINT()
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgAftReg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAftReg message handlers

void CDlgAftReg::OnPaint()
{
	CPaintDC dc(this); 

	if(!m_bCalImgLoc)
		CalImgLocation();

	CSize sizeDisplay;
	CPoint pointDisplay;
	if(m_pDib != NULL){
		if(!m_pDib->IsEmpty()){


			// 获得控件IDC_DlgAftReg_IMAGE的句柄，并获得控件的初始位置信息
			CWnd* pWnd = GetDlgItem(IDC_AFTREG_IMAGE);
			WINDOWPLACEMENT *winPlacement;
			winPlacement = new WINDOWPLACEMENT;
			pWnd->GetWindowPlacement(winPlacement);
			sizeDisplay.cx = winPlacement->rcNormalPosition.right - winPlacement->rcNormalPosition.left;
			sizeDisplay.cy = winPlacement->rcNormalPosition.bottom - winPlacement->rcNormalPosition.top;

			pointDisplay.x = m_rectImage.left;
			pointDisplay.y = m_rectImage.top;
			m_pDib->Draw(&dc,pointDisplay,sizeDisplay);	
		}
	}
		
}

/*************************************************************************
 * \函数名称：CalImageLocation()
 * \输入参数:无
 * \返回值:无
 * \说明:
 *该函数设置对话框中的控件位置和大小，并设置显示图像的位置。默认的图像大小为352×288，如果图像小于
 *此大小，则控件大小设置为352×288，并将图像放置在控件中间。
  *************************************************************************/
void CDlgAftReg::CalImgLocation()
{
	// 获得控件IDC_DlgAftReg_IMAGE的句柄，并获得控件的初始位置信息
	CWnd* pWnd=GetDlgItem(IDC_AFTREG_IMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement=new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);

	// 图像宽度
	int nImageWidth = 0;
	
	// 图像高度
	int nImageHeight = 0;

	if(m_pDib != NULL){
		nImageWidth = m_pDib->m_lpBMIH->biWidth;
		nImageHeight = m_pDib->m_lpBMIH->biHeight;
	}	
	
	// 调整控件IDC_REG_INIT_IMAGE的大小位置，并同时设置显示基准图像的位置
	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + IMG_INIT_H;
	m_rectImage.bottom = winPlacement->rcNormalPosition.bottom;
	m_rectImage.top = winPlacement->rcNormalPosition.top;

	winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + IMG_INIT_W;
	m_rectImage.right = winPlacement->rcNormalPosition.right;
	m_rectImage.left = winPlacement->rcNormalPosition.left;
	
	// 设置IDC_DlgAftReg_IMAGE控件的大小位置
	pWnd->SetWindowPlacement(winPlacement);	

	// 获得IDC_DlgAftReg_IMAGE控件的下边位置，以便调整其他控件的位置
	int nIniImgTop, nIniImgLeft, nIniImgRight, nInitImagMid, nIniImgBottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	nIniImgTop = winPlacement->rcNormalPosition.top;
	nInitImagMid = (winPlacement->rcNormalPosition.right + winPlacement->rcNormalPosition.left) / 2;
	nIniImgLeft = winPlacement->rcNormalPosition.left;
	nIniImgRight = winPlacement->rcNormalPosition.right;

	// 设置控件IDOK的位置大小
	pWnd=GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = (nIniImgTop - 10);
	winPlacement->rcNormalPosition.left =( nInitImagMid - 200);
	winPlacement->rcNormalPosition.right = (nInitImagMid - 20);
	pWnd->SetWindowPlacement(winPlacement);
	
	// 设置控件IDCANCEL的位置大小
	pWnd=GetDlgItem(IDCANCEL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = (nIniImgTop - 10);
	winPlacement->rcNormalPosition.right = (nInitImagMid + 200);
	winPlacement->rcNormalPosition.left = (nInitImagMid + 20);
	pWnd->SetWindowPlacement(winPlacement);

	
		// 调整此对话框的大小
	this->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 200;
	winPlacement->rcNormalPosition.left   = nIniImgLeft   - 20;
	winPlacement->rcNormalPosition.right  = nIniImgRight  + 20;
	this->SetWindowPlacement(winPlacement);

	m_bCalImgLoc = TRUE;
	
	// 释放已分配内存
	delete winPlacement;	
}

void CDlgAftReg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CRSIDoc* save_result=CRSIDoc::create();
	save_result->m_pDibInit=m_pDib;
	str=save_result->OnFileSaveAs_result();
	CRSIApp* open_result=(CRSIApp*)AfxGetApp(); 
	if(str!="")
	open_result->OpenDocumentFile(str);
	OnOK();
}
