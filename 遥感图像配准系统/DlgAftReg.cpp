// DlgAftReg.cpp : implementation file
//

#include "stdafx.h"
#include "ң��ͼ����׼ϵͳ.h"
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


			// ��ÿؼ�IDC_DlgAftReg_IMAGE�ľ��������ÿؼ��ĳ�ʼλ����Ϣ
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
 * \�������ƣ�CalImageLocation()
 * \�������:��
 * \����ֵ:��
 * \˵��:
 *�ú������öԻ����еĿؼ�λ�úʹ�С����������ʾͼ���λ�á�Ĭ�ϵ�ͼ���СΪ352��288�����ͼ��С��
 *�˴�С����ؼ���С����Ϊ352��288������ͼ������ڿؼ��м䡣
  *************************************************************************/
void CDlgAftReg::CalImgLocation()
{
	// ��ÿؼ�IDC_DlgAftReg_IMAGE�ľ��������ÿؼ��ĳ�ʼλ����Ϣ
	CWnd* pWnd=GetDlgItem(IDC_AFTREG_IMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement=new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);

	// ͼ����
	int nImageWidth = 0;
	
	// ͼ��߶�
	int nImageHeight = 0;

	if(m_pDib != NULL){
		nImageWidth = m_pDib->m_lpBMIH->biWidth;
		nImageHeight = m_pDib->m_lpBMIH->biHeight;
	}	
	
	// �����ؼ�IDC_REG_INIT_IMAGE�Ĵ�Сλ�ã���ͬʱ������ʾ��׼ͼ���λ��
	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + IMG_INIT_H;
	m_rectImage.bottom = winPlacement->rcNormalPosition.bottom;
	m_rectImage.top = winPlacement->rcNormalPosition.top;

	winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + IMG_INIT_W;
	m_rectImage.right = winPlacement->rcNormalPosition.right;
	m_rectImage.left = winPlacement->rcNormalPosition.left;
	
	// ����IDC_DlgAftReg_IMAGE�ؼ��Ĵ�Сλ��
	pWnd->SetWindowPlacement(winPlacement);	

	// ���IDC_DlgAftReg_IMAGE�ؼ����±�λ�ã��Ա���������ؼ���λ��
	int nIniImgTop, nIniImgLeft, nIniImgRight, nInitImagMid, nIniImgBottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	nIniImgTop = winPlacement->rcNormalPosition.top;
	nInitImagMid = (winPlacement->rcNormalPosition.right + winPlacement->rcNormalPosition.left) / 2;
	nIniImgLeft = winPlacement->rcNormalPosition.left;
	nIniImgRight = winPlacement->rcNormalPosition.right;

	// ���ÿؼ�IDOK��λ�ô�С
	pWnd=GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = (nIniImgTop - 10);
	winPlacement->rcNormalPosition.left =( nInitImagMid - 200);
	winPlacement->rcNormalPosition.right = (nInitImagMid - 20);
	pWnd->SetWindowPlacement(winPlacement);
	
	// ���ÿؼ�IDCANCEL��λ�ô�С
	pWnd=GetDlgItem(IDCANCEL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = (nIniImgTop - 10);
	winPlacement->rcNormalPosition.right = (nInitImagMid + 200);
	winPlacement->rcNormalPosition.left = (nInitImagMid + 20);
	pWnd->SetWindowPlacement(winPlacement);

	
		// �����˶Ի���Ĵ�С
	this->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 200;
	winPlacement->rcNormalPosition.left   = nIniImgLeft   - 20;
	winPlacement->rcNormalPosition.right  = nIniImgRight  + 20;
	this->SetWindowPlacement(winPlacement);

	m_bCalImgLoc = TRUE;
	
	// �ͷ��ѷ����ڴ�
	delete winPlacement;	
}

void CDlgAftReg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CRSIDoc* save_result=CRSIDoc::create();
	save_result->m_pDibInit=m_pDib;
	str=save_result->OnFileSaveAs_result();
	CRSIApp* open_result=(CRSIApp*)AfxGetApp(); 
	if(str!="")
	open_result->OpenDocumentFile(str);
	OnOK();
}
