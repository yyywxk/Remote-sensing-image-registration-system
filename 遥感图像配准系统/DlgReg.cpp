// DlgReg.cpp : implementation file
//
#include "stdafx.h"
#include "ң��ͼ����׼ϵͳ.h"
#include "RSIDoc.h"
#include "GlobalApi.h"
#include "DlgReg.h"
#include "DlgAftReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SURF 0
#define SIFT 1
#define ORB 2
//#define HARRIS 3
/////////////////////////////////////////////////////////////////////////////
// CDlgReg dialog


CDlgReg::CDlgReg(CWnd* pParent /*=NULL*/, CRSIDoc* pDoc)
	: CDialog(CDlgReg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgReg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// ��ȡ�ĵ���ָ��
	m_pDoc = pDoc;

	// ���ü���ͼ��λ�ñ�־λλFALSE
	m_bCalImgLoc = FALSE;

	// ���û�׼ͼ��Ϊԭʼ�򿪵�ͼ��
	m_pDibInit = pDoc->m_pDibInit;

	// ���ô���׼ͼ��
	m_pDibSamp = new CDib;

	// ����ѡȡ���������Ŀ��ʼֵ
	m_nChsFeatureNum = 0;
	//�����ֶ�����ѡȡ��־λΪTRUE
	handle_baseFeaturechosen = TRUE;

	// ����ѡȡ������ı�־λΪFALSE
	m_bChoseFeature = FALSE;
	len_init_x = 0.0;
	len_init_y = 0.0;
	len_reg_x = 0.0;
	len_reg_y = 0.0;
	alpha_init = 1.0;
	alpha_samp = 1.0;
	MOUSE_R = FALSE;
	
}

void CDlgReg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
	auto_Feature = TRUE;
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBOAUTO, m_comboauto);
	DDX_Control(pDX, IDC_COMBORMS, m_rms_show);
}

BEGIN_MESSAGE_MAP(CDlgReg, CDialog)
	//{{AFX_MSG_MAP(CDlgReg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_REG_OPEN, OnRegOpen)
	ON_BN_CLICKED(IDC_REG_REG, OnRegReg)
	ON_BN_CLICKED(IDC_REG_CHOSE_FEATURE, OnRegChoseFeature)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO1, &CDlgReg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgReg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDOK, &CDlgReg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgReg::OnBnClickedCancel)
	ON_CBN_DROPDOWN(IDC_COMBOAUTO, &CDlgReg::OnDropdownComboauto)
//	ON_CBN_SELCHANGE(IDC_COMBOAUTO, &CDlgReg::OnCbnSelchangeComboauto)
	
//ON_BN_CLICKED(IDC_BUTTONRMS, &CDlgReg::OnFeatureRms)
ON_BN_CLICKED(IDC_BUTTONRMS, &CDlgReg::OnFeatureRms)
ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReg message handlers

void CDlgReg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

					   // �����û�м���ͼ���λ�ã�����м���
	//if (!m_bCalImgLoc)
		CalImageLocation();

	CSize sizeDisplay;
	CPoint pointDisplay;

	// ��ʾ��׼ͼ��
	if (!m_pDibInit->IsEmpty())
	{
		
		sizeDisplay.cx = IMG_INIT_W;
		sizeDisplay.cy = IMG_INIT_H;
		pointDisplay.x = m_rectInitImage.left;
		pointDisplay.y = m_rectInitImage.top;
		//�����׼ͼ��ʾ����С���ڱ������ż���
		len_init_x = sizeDisplay.cx;
		len_init_y = sizeDisplay.cy;

		m_pDibInit->Draw(&dc, pointDisplay, sizeDisplay);
	}

	// ��ʾ����׼ͼ��
	if (!m_pDibSamp->IsEmpty())
	{
		sizeDisplay.cx = IMG_SAMP_W;
		sizeDisplay.cy = IMG_SAMP_H;

		pointDisplay.x = m_rectResltImage.left;
		pointDisplay.y = m_rectResltImage.top;

		//�������׼ͼ��ʾ����С���ڱ������ż���
		len_reg_x = sizeDisplay.cx;
		len_reg_y = sizeDisplay.cy;
		m_pDibSamp->Draw(&dc, pointDisplay, sizeDisplay);
	}


	// ��ʾ����������׼��������
	DrawFeature(&dc);
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
void CDlgReg::CalImageLocation()
{
	// ��ÿؼ�IDC_REG_INIT_IMAGE�ľ��������ÿؼ��ĳ�ʼλ����Ϣ
	CWnd* pWnd = GetDlgItem(IDC_REG_INIT_IMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement = new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);

	// ͼ����
	int nImageWidth1;
	nImageWidth1 = m_pDibInit->m_lpBMIH->biWidth;

	// ͼ���
	int nImageHeight1;
	nImageHeight1 = m_pDibInit->m_lpBMIH->biHeight;

	// �����ؼ�IDC_REG_INIT_IMAGE�Ĵ�Сλ�ã���ͬʱ������ʾ��׼ͼ���λ��
	
	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + IMG_INIT_H;
		m_rectInitImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectInitImage.top = winPlacement->rcNormalPosition.top;
	
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + IMG_INIT_W;
		m_rectInitImage.right = winPlacement->rcNormalPosition.right;
		m_rectInitImage.left = winPlacement->rcNormalPosition.left;


	// ����IDC_REG_INIT_IMAGE�ؼ��Ĵ�Сλ��
	pWnd->SetWindowPlacement(winPlacement);

	// �����ʾ��׼ͼ��ؼ����ұ�λ�ã��Ա�ȷ����ʾ����׼ͼ��ؼ���λ��
	int nIniImgRight;
	nIniImgRight = winPlacement->rcNormalPosition.right;

	int  nIniImgLeft;
	nIniImgLeft = winPlacement->rcNormalPosition.left;

	// ���IDC_REG_INIT_IMAGE�ؼ����±�λ�ã��Ա���������ؼ���λ��
	int nIniImgBottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;



	// ��ÿؼ�IDC_REG_RESLT_IMAGE�ľ��������ó�ʼλ����Ϣ
	pWnd = GetDlgItem(IDC_REG_RESLT_IMAGE);
	pWnd->GetWindowPlacement(winPlacement);

	int nImageHeight2, nImageWidth2;
	// �����δ�򿪴���׼ͼ�������ô���׼ͼ���С�ͻ�׼ͼ���С���
	if (!m_pDibSamp->IsEmpty())
	{
		nImageWidth2 = m_pDibSamp->m_lpBMIH->biWidth;
		nImageHeight2 = m_pDibSamp->m_lpBMIH->biHeight;
	}
	else
	{
		nImageHeight2 = nImageHeight1;
		nImageWidth2 = nImageWidth1;
	}
	// �����ؼ�IDC_REG_RESLT_IMAGE�Ĵ�Сλ�ã���ͬʱ������ʾ����׼ͼ���λ��

	// �ȵ����ؼ������λ�ã���IDC_REG_INIT_IMAGE�ؼ����15������
	winPlacement->rcNormalPosition.left = nIniImgRight + 15;

	
	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + IMG_SAMP_H;
		m_rectResltImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectResltImage.top = winPlacement->rcNormalPosition.top;
	
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + IMG_SAMP_W;
		m_rectResltImage.right = winPlacement->rcNormalPosition.right;
		m_rectResltImage.left = winPlacement->rcNormalPosition.left;
	
	// ����IDC_REG_RESLT_IMAGE�ؼ��Ĵ�Сλ��
	pWnd->SetWindowPlacement(winPlacement);

	if (nIniImgBottom < winPlacement->rcNormalPosition.bottom)
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;

	nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	nIniImgRight = winPlacement->rcNormalPosition.right;

	if (nImageHeight2<nImageHeight1)
	{
		// ��ÿؼ�IDC_REG_INIT_IMAGE�ľ��������ÿؼ��ĳ�ʼλ����Ϣ
		CWnd* pWnd = GetDlgItem(IDC_REG_INIT_IMAGE);
		WINDOWPLACEMENT *winPlacement;
		winPlacement = new WINDOWPLACEMENT;
		pWnd->GetWindowPlacement(winPlacement);
		if (nIniImgBottom < winPlacement->rcNormalPosition.bottom)
			nIniImgBottom = winPlacement->rcNormalPosition.bottom;
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	}
	// ���ÿؼ�IDC_STATIC_BASEREG��λ�ô�С
	pWnd = GetDlgItem(IDC_STATIC_BASEREG);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.left = (m_rectInitImage.right + m_rectInitImage.left) / 2 - 35;
	winPlacement->rcNormalPosition.right = (m_rectInitImage.right + m_rectInitImage.left) / 2 + 35;
	pWnd->SetWindowPlacement(winPlacement);

	// ���ÿؼ�IDC_STATIC_REG��λ�ô�С
	pWnd = GetDlgItem(IDC_STATIC_REG);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.left = (m_rectResltImage.right + m_rectResltImage.left) / 2 - 45;
	winPlacement->rcNormalPosition.right = (m_rectResltImage.right + m_rectResltImage.left) / 2 + 45;
	pWnd->SetWindowPlacement(winPlacement);
	

	// ���ÿؼ�IDOK��λ�ô�С
	pWnd = GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);

	// ���ÿؼ�IDCANCEL��λ�ô�С
	pWnd = GetDlgItem(IDCANCEL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);

	// ���ÿؼ�IDC_REG_OPEN��λ�ô�С
	pWnd = GetDlgItem(IDC_REG_OPEN);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);
	// ���ÿؼ�IDC_RADIO1��λ�ô�С
	pWnd = GetDlgItem(IDC_RADIO1);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 40;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 55;
	pWnd->SetWindowPlacement(winPlacement);
	int RA1_right = winPlacement->rcNormalPosition.right;
	//����IDC_COMBOAUTO��λ�ô�С
	pWnd = GetDlgItem(IDC_COMBOAUTO);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 40;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 55;
	winPlacement->rcNormalPosition.left = RA1_right + 10;
	pWnd->SetWindowPlacement(winPlacement);
	
	// ���ÿؼ�IDC_RADIO2��λ�ô�С
	pWnd = GetDlgItem(IDC_RADIO2);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 60;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 75;
	pWnd->SetWindowPlacement(winPlacement);
	// ���ÿؼ�IDC_REG_REG��λ�ô�С
	pWnd = GetDlgItem(IDC_REG_REG);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);
	// ���ÿؼ�IDC_BUTTONRMS��λ�ô�С
	pWnd = GetDlgItem(IDC_BUTTONRMS);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);
	// ���ÿؼ�IDC_COMBORMS��λ�ô�С
	pWnd = GetDlgItem(IDC_COMBORMS);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 40;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 55;
	pWnd->SetWindowPlacement(winPlacement);

	// ���ÿؼ�IDC_REG_CHOSE_FEATUR��λ�ô�С
	pWnd = GetDlgItem(IDC_REG_CHOSE_FEATURE);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);

	// �����˶Ի���Ĵ�С
	this->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 400;
	winPlacement->rcNormalPosition.left = nIniImgLeft - 20;
	winPlacement->rcNormalPosition.right = nIniImgRight + 50;
	this->SetWindowPlacement(winPlacement);

	// �ͷ��ѷ����ڴ�
	delete winPlacement;

	// ���ü���ͼ��ؼ�λ�ñ�־λΪTRUE
	m_bCalImgLoc = TRUE;
}


/*************************************************************************
* \�������ƣ�
*   DrawFeature()
* \�������:��
* \����ֵ:��
* \˵��:
*�ú���������ĳ�Ա����ȷ�����������Ŀ��λ�ã�����ͼ���н�����ʾ��
**************************************************************************/
void CDlgReg::DrawFeature(CDC* pDC)
{
	// ѭ������
	int i;
	CPoint pointTemp;
	// �뾶
	int nRadius;
	nRadius = 5;

	// ���û�ͼ����
	pDC->SelectStockObject(HOLLOW_BRUSH);

	// ��������
	CPen penWhite(PS_SOLID, 1, RGB(255, 0, 0));
	CPen *pOldPen;

	// ������ѡ�룬��������ǰ�Ļ���
	pOldPen = pDC->SelectObject(&penWhite);
//	if(!auto_Feature&&m_bChoseFeature == TRUE)
	if (handle_baseFeaturechosen == FALSE&&m_bChoseFeature == TRUE)
		m_nChsFeatureNum++;
	for (i = 0; i<m_nChsFeatureNum; i++)
	{
		// ������ʾ������

		// ȷ���˵����ʾλ��
		pointTemp.x =floor( m_pPointSampl[i].x / m_pDibSamp->m_lpBMIH->biWidth * IMG_SAMP_W + m_rectResltImage.left+0.5);
		pointTemp.y =floor( m_pPointSampl[i].y  / m_pDibSamp->m_lpBMIH->biHeight* IMG_SAMP_H + m_rectResltImage.top+0.5);

		// �����������㣬����԰�İ뾶ΪnRadius
		CRect rectSamp(pointTemp.x - nRadius, pointTemp.y - nRadius,
			pointTemp.x + nRadius, pointTemp.y + nRadius);
		pDC->Ellipse(rectSamp);

		// ����ʾ��׼������
		// ȷ���˵����ʾλ��
		if (handle_baseFeaturechosen == TRUE || handle_baseFeaturechosen == FALSE&&i<m_nChsFeatureNum - 1)
		{
			pointTemp.x =floor( m_pPointBase[i].x / m_pDibInit->m_lpBMIH->biWidth * IMG_INIT_W + m_rectInitImage.left+0.5);
			pointTemp.y = floor(m_pPointBase[i].y  / m_pDibInit->m_lpBMIH->biHeight* IMG_INIT_H + m_rectInitImage.top+0.5);
			// �����������㣬����԰�İ뾶ΪnRadius
			CRect rectBase(pointTemp.x - nRadius, pointTemp.y - nRadius,
				pointTemp.x + nRadius, pointTemp.y + nRadius);
			pDC->Ellipse(rectBase);
		}
	}
	if (handle_baseFeaturechosen == FALSE&&m_bChoseFeature == TRUE)
		m_nChsFeatureNum--;
	// �ظ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);
	penWhite.DeleteObject();
}

/*************************************************************************
* \�������ƣ�
*   OnRegOpen()
* \�������:��
* \����ֵ:��
* \˵��:
*�ú����򿪴���׼ͼ�񣬲���ͼ������m_pDibSamp�С�
**************************************************************************/
void CDlgReg::OnRegOpen()
{
	GetDlgItem(IDC_COMBORMS)->EnableWindow(FALSE); //RMS�򲻿���
	char  szFilter[] = "(*.dat)|*.dat|GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	// TODO: Add your control notification handler code here
	//CFileDialog dlg(TRUE,"bmp","*.bmp");
	CFileDialog dlg(TRUE, "tif", NULL, OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		CFile file;
		CString strPathName;
		strPathName = dlg.GetPathName();

		GDALAllRegister();//ע��GDAL��
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  //֧������·��
		GDALDataset *pDataSet;
		pDataSet = (GDALDataset *)GDALOpen(strPathName, GA_ReadOnly);
		//16λת8λ
		if (pDataSet->GetRasterBand(1)->GetRasterDataType() == GDT_UInt16 || pDataSet->GetRasterBand(1)->GetRasterDataType() == GDT_Int16)
		{
			GDALClose(pDataSet);
			GISImage GIS;
			pDataSet = GIS.Stretch_percent_16to8(strPathName);
		}
		//GDALDataType e=pDataSet->GetRasterBand(1)->GetRasterDataType();
		if (pDataSet != NULL)
		{
			nbands = pDataSet->GetRasterCount();
			lWidth = pDataSet->GetRasterXSize();
			lHeight = pDataSet->GetRasterYSize();
			//pData = new BYTE[lWidth*lHeight*nbands];
			pData = new WORD[lWidth*lHeight*nbands];
			pDataSet->RasterIO(GF_Read, 0, 0, lWidth, lHeight, pData, lWidth, lHeight, GDT_UInt16, nbands, 0, 0, 0, 0);
			//pDataSet->RasterIO(GF_Read, 0, 0, lWidth, lHeight, pData, lWidth, lHeight, GDT_Byte, nbands, 0, 0, 0, 0);
		}
		ChangeToBmp("myBmp2.bmp");// TODO: Add your specialized creation code here
		GDALClose(pDataSet);
		//CFile file1;
		//CFileException fe;
		//if (!file1.Open("myBmp2.bmp", CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
		//{
		//	return;
		//}
		//BOOL bSuccess = FALSE;

		//bSuccess = m_DIBt.Save(file1);
		////m_DIBt.Free();
		//file1.Close();
		//cv::Mat img = cv::imread("myBmp2.bmp", 0);
		//cv::imwrite("myBmp2.bmp", img);

		// ���ļ�
		if (!file.Open("myBmp2.bmp", CFile::modeRead | CFile::shareDenyWrite))
		{
			// ����	
			return;
		}

		// ����ģ��ͼ��
		if (!m_pDibSamp->Read(&file))
		{
			// �ָ������״
			EndWaitCursor();

			// ����ѷ����ڴ�
			m_pDibSamp->Empty();

			// ����
			return;
		}
	}

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ģ����׼�������Ŀ������ƣ�
	if (m_pDibSamp->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		

		// ����ѷ����ڴ�
		m_pDibSamp->Empty();

		// ����
		return;
	}

	// ������µĴ���׼�ļ�����ͼ��λ�����ñ�־λ��ΪFALSE���Ա��ٴε���λ��
	m_bCalImgLoc = FALSE;
	m_pDibSamp->Convert256toGray();
	// ������ʾ
	this->UpdateData();
	this->Invalidate();
}

DWORD CDlgReg::ChangeToBmp(const char* filename)
{
	bool flag = FALSE;;
	INT_PTR reselesct = IDCANCEL;
	ChoseShow show;
	cv::Mat img;
	do
	{
		m_DIBt.Free();
		BYTE * m_pBMI;
		if (nbands == 1)
		{
			m_pBMI = new BYTE[1064];
			if (m_pBMI == 0)
			{
				return NULL;
			}

			((BITMAPINFOHEADER*)m_pBMI)->biSize = 40;
			((BITMAPINFOHEADER*)m_pBMI)->biWidth = lWidth;
			((BITMAPINFOHEADER*)m_pBMI)->biHeight = lHeight;
			((BITMAPINFOHEADER*)m_pBMI)->biPlanes = 1;
			((BITMAPINFOHEADER*)m_pBMI)->biCompression = 0;
			((BITMAPINFOHEADER*)m_pBMI)->biXPelsPerMeter = 0;
			((BITMAPINFOHEADER*)m_pBMI)->biYPelsPerMeter = 0;
			((BITMAPINFOHEADER*)m_pBMI)->biClrUsed = 0;
			((BITMAPINFOHEADER*)m_pBMI)->biClrImportant = 0;
			((BITMAPINFOHEADER*)m_pBMI)->biBitCount = 8;
			((BITMAPINFOHEADER*)m_pBMI)->biSizeImage = (((lWidth * 8) + 31) / 32 * 4)*lHeight;


			int i, j;
			RGBQUAD rgb[256];
			for (i = 0; i<256; i++)
			{
				rgb[i].rgbBlue = (unsigned char)i;
				rgb[i].rgbGreen = (unsigned char)i;
				rgb[i].rgbRed = (unsigned char)i;
				rgb[i].rgbReserved = 0;
			}
			memcpy(m_pBMI + 40, rgb, 256 * 4);
			DWORD iWidthBytes = ((BITMAPINFOHEADER*)m_pBMI)->biSizeImage / lHeight;

			BYTE *p = new BYTE[lHeight*iWidthBytes];
			for (i = 0; i < lHeight; i++)
			{
				for (j = 0; j < lWidth; j++)
				{
					p[(lHeight - i - 1)*iWidthBytes + j] = pData[i*lWidth + j];
				}
				if (lWidth != iWidthBytes)
					for (j = lWidth; j<iWidthBytes; j++)
					{
						p[(lHeight - i - 1)*iWidthBytes + j] = 0;
					}

			}

			m_DIBt.SetBMI((LPBITMAPINFO)m_pBMI);
			m_DIBt.SetBits(p);
			reselesct = IDOK;
		}
		if (nbands >= 3)
		{
			int red, green, blue;
			CChooseband dlg;
			dlg.nbands = nbands;
			if (dlg.DoModal() == IDOK)
			{
				red = dlg.R;
				green = dlg.G;
				blue = dlg.B;
				flag = FALSE;
				reselesct = IDOK;
			}
			else
			{
				red = dlg.R;
				green = dlg.G;
				blue = dlg.B;
				flag = TRUE;
			}

			delete dlg;
			m_pBMI = new BYTE[40];

			if (m_pBMI == 0)
			{
				return NULL;
			}

			((BITMAPINFOHEADER*)m_pBMI)->biSize = 40;
			((BITMAPINFOHEADER*)m_pBMI)->biWidth = lWidth;
			((BITMAPINFOHEADER*)m_pBMI)->biHeight = lHeight;
			((BITMAPINFOHEADER*)m_pBMI)->biPlanes = 1;
			((BITMAPINFOHEADER*)m_pBMI)->biCompression = 0;
			((BITMAPINFOHEADER*)m_pBMI)->biXPelsPerMeter = 0;
			((BITMAPINFOHEADER*)m_pBMI)->biYPelsPerMeter = 0;
			((BITMAPINFOHEADER*)m_pBMI)->biClrUsed = 0;
			((BITMAPINFOHEADER*)m_pBMI)->biClrImportant = 0;
			((BITMAPINFOHEADER*)m_pBMI)->biBitCount = 24;
			//((BITMAPINFOHEADER*)m_pBMI)->biBitCount = 8;
			((BITMAPINFOHEADER*)m_pBMI)->biSizeImage = (((lWidth * 8 * 3) + 31) / 32 * 4)*lHeight;
			DWORD iWidthBytes = ((BITMAPINFOHEADER*)m_pBMI)->biSizeImage / lHeight;
			unsigned char *p;
			p = new unsigned char[lHeight*iWidthBytes];
			DWORD i, j;
			for (j = 0; j<lHeight; j++)
			{
				for (i = 0; i<lWidth; i++)
				{
					*(p + i * 3 + (lHeight - j - 1)*iWidthBytes) = *(pData + i + j*lWidth + (blue - 1)*lWidth*lHeight);
					*(p + i * 3 + 1 + (lHeight - j - 1)*iWidthBytes) = *(pData + i + j*lWidth + (green - 1)*lWidth*lHeight);
					*(p + i * 3 + 2 + (lHeight - j - 1)*iWidthBytes) = *(pData + i + j*lWidth + (red - 1)*lWidth*lHeight);

				}
				if (iWidthBytes != lWidth)
				{
					for (i = lWidth * 3; i < iWidthBytes; i++)
					{
						*(p + i + (lHeight - j - 1)*iWidthBytes) = 0;

					}

				}
			}


			m_DIBt.SetBMI((LPBITMAPINFO)m_pBMI);
			m_DIBt.SetBits(p);
		}

		m_DIBt.CreatePalette();
		//m_pDibtInit = m_DIBt;
		CFile file;
		CFileException fe;
		if (!file.Open(filename, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
		{

			return FALSE;
		}
		BOOL bSuccess = FALSE;

		bSuccess = m_DIBt.Save(file);
		file.Close();

		img = cv::imread(filename, 1);
		cv::imwrite(filename, img);
		if (flag)
		{
			show.filename = LPCTSTR(filename);
			reselesct = show.DoModal();
			flag = FALSE;

		}




	} while (reselesct == IDCANCEL);
	delete[]pData;
	delete show;
	img = cv::imread(filename, 0);
	cv::imwrite(filename, img);
	return 1;
}




/*************************************************************************
* \�������ƣ�
*   OnRegChoseFeatureOnRegReg()
* \�������:��
* \����ֵ:��
* \˵��:
*�ú�������ѡȡ�������־λ��Ȼ����ú����ڴ���׼ͼ����ѡȡ�����㣬��
*��׼��Щ�����㡣���������Ŀ����Ӧ��ѡȡ������
**************************************************************************/
void CDlgReg::OnRegChoseFeature()
{
	// �������׼ͼ����δ�򿪣����ܽ���������ѡȡ����
	if ((m_pDibSamp->IsEmpty())) {
		AfxMessageBox("��δ�򿪴���׼ͼ���ļ�����򿪴���׼ͼ��");
		return;
	}
	// ����ѡȡ�������־λ
	m_bChoseFeature = TRUE;
	if (!auto_Feature)
	{
		//handle_baseFeaturechosen == TRUE;
		AfxMessageBox("���ڴ���׼ͼ����ѡȡ������");
	}
		
	if (auto_Feature)//�Զ�
	{
		auto_Featurechosen();
		Invalidate();
	}
}

/*************************************************************************
* \�������ƣ�
*   OnLButtonUp()
* \�������:��
* \����ֵ:��
* \˵��:
*�ú�������������궨��λ�����������㡣Ȼ�����������׼������׼�������㡣
*���������Ŀ����Ӧ��ѡȡ������
**************************************************************************/
void CDlgReg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!auto_Feature)//�ֶ�
					  //if(auto_Feature==TRUE&&handle_baseFeaturechosen==TRUE)
					  //	auto_Featurechosen(nFlags,point);
					  //else
		handle_Featurechosen(nFlags, point);
	if(auto_Feature&&m_bChoseFeature)
		auto_Good(nFlags, point);
	
	// ������ʾ
	Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}
/*************************************************************************
* \�������ƣ�
*   auto_Featurechosen()
* \�������:��
* \����ֵ:��
* \˵��:���Զ�ѡȡ������
**************************************************************************/
void CDlgReg::auto_Featurechosen()
{
	// �������ѡȡ��־λΪTRUE��������������ѡȡ����׼�������˳�
	if (!m_bChoseFeature)
		return;
	if (m_comboauto.GetCurSel() == CB_ERR)
	{
		AfxMessageBox("��ѡ���Զ�ƥ���㷨��");
		return;
	}
		
	cv::Mat image1 = cv::imread("myBmp1.bmp", 0);
	cv::Mat image2 = cv::imread("myBmp2.bmp", 0);

	//�ؼ���
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;
	keypoints1.reserve(100000);
	keypoints2.reserve(100000);
	//������
	cv::Mat descriptors1, descriptors2;
	//����ƥ��
	vector< vector< cv::DMatch > > matches;
	matches.reserve(100000);
	//ɸѡ��ƥ��
	vector<cv::DMatch> goodMatches;
	double beta = 4;//��С���뱶��
	if (m_comboauto.GetCurSel() == SURF)
	{
		cv::SurfFeatureDetector surf(3000);
		surf.detect(image1, keypoints1);
		surf.detect(image2, keypoints2);
		//����������
		cv::SurfDescriptorExtractor surfDesc;
		surfDesc.compute(image1, keypoints1, descriptors1);
		surfDesc.compute(image2, keypoints2, descriptors2);
	}
	else if (m_comboauto.GetCurSel() == SIFT)
	{
		//��ȡ������    
		cv::SiftFeatureDetector siftDetector(2000);  // ����������ֵ��������������ȣ�ֵԽ���Խ�٣�Խ��׼ 

		siftDetector.detect(image1, keypoints1);
		siftDetector.detect(image2, keypoints2);

		//������������Ϊ�±ߵ�������ƥ����׼��    
		cv::SiftDescriptorExtractor SiftDescriptor;
		
		SiftDescriptor.compute(image1, keypoints1, descriptors1);
		SiftDescriptor.compute(image2, keypoints2, descriptors2);

	


	}

	else if (m_comboauto.GetCurSel() == ORB)
	{
		//��ȡ������    
		cv::OrbFeatureDetector OrbDetector(1000);  // ������������ȣ�ֵԽС��Խ�٣�Խ��׼ 
		
		OrbDetector.detect(image1, keypoints1);
		OrbDetector.detect(image2, keypoints2);

		//������������Ϊ�±ߵ�������ƥ����׼��    
		cv::OrbDescriptorExtractor OrbDescriptor;

		OrbDescriptor.compute(image1, keypoints1, descriptors1);
		OrbDescriptor.compute(image2, keypoints2, descriptors2);

		cv::flann::Index flannIndex(descriptors1, cv::flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);


		cv::Mat macthIndex(descriptors2.rows, 2, CV_32SC1), matchDistance(descriptors2.rows, 2, CV_32FC1);
		flannIndex.knnSearch(descriptors2, macthIndex, matchDistance, 2, cv::flann::SearchParams());

		// Lowe's algorithm,��ȡ����ƥ���
		for (int i = 0; i < matchDistance.rows; i++)
		{
			if (matchDistance.at<float>(i, 0) < 0.5 * matchDistance.at<float>(i, 1))
			{
				cv::DMatch dmatches(i, macthIndex.at<int>(i, 0), matchDistance.at<float>(i, 0));
				goodMatches.push_back(dmatches);
			}
		}
	}
	//else if (m_comboauto.GetCurSel() == HARRIS)
	//{
	//	//��ȡ������    
	//	cv::GoodFeaturesToTrackDetector Detector(500);  //������,ֵԽ�󣬵�Խ��
	//
	//	Detector.detect(image1, keypoints1);
	//	Detector.detect(image2, keypoints2);

	//	//������������Ϊ�±ߵ�������ƥ����׼��    
	//	cv::SiftDescriptorExtractor  Descriptor;
	//	
	//	Descriptor.compute(image1, keypoints1, descriptors1);
	//	Descriptor.compute(image2, keypoints2, descriptors2);

	//	cv::BruteForceMatcher<float> matcher;
	//	vector<vector<cv::DMatch> > matchePoints;
	//	vector<cv::DMatch> GoodMatchePoints;

	//	vector<cv::Mat> train_desc(1, imageDesc1);
	//	matcher.add(train_desc);
	//	matcher.train();
	//}
	else return; //����д����ɾ

	//
	if (m_comboauto.GetCurSel() != ORB)
	{
		//ʹ��FLANNƥ�����ӽ���ƥ��
		cv::FlannBasedMatcher matcher;

		vector<cv::Mat> train_desc(1, descriptors1);
		matcher.add(train_desc);
		matcher.train();

		matcher.knnMatch(descriptors1, descriptors2, matches, 2);

		// Lowe's algorithm,��ȡ����ƥ���
		for (unsigned int i = 0; i < matches.size(); i++)
		{
			if (matches[i][0].distance < 0.6*matches[i][1].distance)
			{
				goodMatches.push_back(matches[i][0]);
			}
		}
	}
	
	//������ؼ���֮���������ֵ����Сֵ
	double max_dist = 0.0; double min_dist = 100.0;//��С�����������
	for (unsigned j = 0; j < goodMatches.size(); j++)
	{
		double dist = goodMatches[j].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	//����ƥ�����С��beta*min_dist�ĵ��

	vector<cv::DMatch>::iterator it;
	for (it = goodMatches.begin(); it != goodMatches.end();)
	{
		if ((*it).distance > beta * min_dist)
		{
			it = goodMatches.erase(it);
		}
		else
		{
			it++;
		}
	}


	//���������ֲ�����
	vector<cv::Point2f> imagePointsBase;
	vector<cv::Point2f> imagePointsReg;

	//��ƥ��ɹ���ƥ����л�ȡ�ؼ���
	for (unsigned int i = 0; i < goodMatches.size(); i++)
	{
		imagePointsBase.push_back(keypoints1[goodMatches[i].queryIdx].pt);
		imagePointsReg.push_back(keypoints2[goodMatches[i].trainIdx].pt);
	}

	//
	m_nChsFeatureNum = goodMatches.size();

	for (int i = 0;i<m_nChsFeatureNum;i++)
	{
		m_pPointBase[i].x = imagePointsBase[i].x;
		m_pPointBase[i].y = imagePointsBase[i].y;
		m_pPointSampl[i].x= imagePointsReg[i].x;
		m_pPointSampl[i].y= imagePointsReg[i].y;

	}


}

/*************************************************************************
* \�������ƣ�
*   handle_Featurechosen()
* \�������:
* \����ֵ:��
* \˵��:�ֶ�ѡ��������
**************************************************************************/
void CDlgReg::handle_Featurechosen(UINT nFlags, CPoint point)
{
	// ѭ������
	int i, j;
	// �������ѡȡ��־λΪTRUE��������������ѡȡ����׼�������˳�
	if (!m_bChoseFeature)
		return;

	// ����׼ͼ�������ѡȡ����������ѡ���������ѡ������Ҫ��ͼ�����
	// ��СһȦ
	CRect rectChoose1;
	rectChoose1.bottom = m_rectResltImage.bottom - 5;
	rectChoose1.top = m_rectResltImage.top + 5;
	rectChoose1.left = m_rectResltImage.left + 5;
	rectChoose1.right = m_rectResltImage.right - 5;
	// ��׼ͼ�������ѡȡ����������ѡ���������ѡ������Ҫ��ͼ�����
	// ��СһȦ
	CRect rectChoose2;
	rectChoose2.bottom = m_rectInitImage.bottom - 5;
	rectChoose2.top = m_rectInitImage.top + 5;
	rectChoose2.left = m_rectInitImage.left + 5;
	rectChoose2.right = m_rectInitImage.right - 5;
	//��׼ͼ����
	CRect rectChoose3;
	rectChoose3.bottom = m_rectInitImage.bottom;
	rectChoose3.top = m_rectInitImage.top;
	rectChoose3.left = m_rectInitImage.left;
	rectChoose3.right = m_rectInitImage.right;
	//����׼ͼ����
	CRect rectChoose4;
	rectChoose4.bottom = m_rectResltImage.bottom;
	rectChoose4.top = m_rectResltImage.top;
	rectChoose4.left = m_rectResltImage.left;
	rectChoose4.right = m_rectResltImage.right;
	// �����������
	CRect rectFeature1;
	CRect rectFeature2;

	// ��־λ����ʾ�˵��Ƿ����Ѿ�ѡ���������
	BOOL bFlag1 = FALSE;
	BOOL bFlag2 = FALSE;

	if (handle_baseFeaturechosen == TRUE)
	{
		// �жϴ˵��Ƿ�Ϸ������жϴ˵��Ƿ��Ѿ�ѡ������ǣ���ȥ���˵�,����׼��
		if (rectChoose1.PtInRect(point))
		{
			// �����ѡ�������������ǰ�������㣬��ȥ���˵�
			for (i = 0; i<m_nChsFeatureNum; i++)
			{
				// ѡ�����������ʾ�����Ա�����������ȡ��,��ʵ���ڶԻ�������(������С)
				rectFeature1.bottom = floor(m_pPointSampl[i].y * IMG_SAMP_H / m_pDibSamp->m_lpBMIH->biHeight + 0.5) + m_rectResltImage.top + 5;
				rectFeature1.top = floor(m_pPointSampl[i].y* IMG_SAMP_H / m_pDibSamp->m_lpBMIH->biHeight + 0.5) + m_rectResltImage.top - 5;
				rectFeature1.left = floor(m_pPointSampl[i].x* IMG_SAMP_W / m_pDibSamp->m_lpBMIH->biWidth+0.5) + m_rectResltImage.left - 5;
				rectFeature1.right =floor( m_pPointSampl[i].x * IMG_SAMP_W / m_pDibSamp->m_lpBMIH->biWidth+0.5) + m_rectResltImage.left + 5;

				// �ж���ѡ����������Ƿ�Ϊԭ��ѡ���������
				// ����ǣ���ȥ����������
				if (rectFeature1.PtInRect(point))
				{
					// ���������������ǰ�ƶ�һλ��ȥ����ѡ��Ĵ�������
					for (j = i; j<m_nChsFeatureNum - 1; j++)
					{
						m_pPointSampl[j] = m_pPointSampl[j + 1];
						m_pPointBase[j] = m_pPointBase[j + 1];
					}

					// ��������ļ�����һ
					m_nChsFeatureNum--;
					// ������ʾ
					Invalidate();
					// ���ñ�־λ
					bFlag1 = TRUE;
					return;
				}
			}

			// ���ж��������Ƿ��Ѿ�ѡȡ����
			if (m_nChsFeatureNum >= 40)
			{
				AfxMessageBox("���Ѿ�ѡȡ��40�������㣬���Ҫ����ѡȡ�������ȥ����׼����ȷ���������ٽ���ѡȡ");
				return;
			}

			// ����˵�����Ҫѡȡ�ģ��������ز���
			if (!bFlag1)
			{
				// ���˴���׼������ѡȡ��ע�����������������ͼ������Ͻ�Ϊԭ��ȷ����,�����Ŵ�
				m_pPointSampl[m_nChsFeatureNum].x =  double(point.x - m_rectResltImage.left)*m_pDibSamp->m_lpBMIH->biWidth/ IMG_SAMP_W;
				m_pPointSampl[m_nChsFeatureNum].y = double(point.y  - m_rectResltImage.top) * m_pDibSamp->m_lpBMIH->biHeight/IMG_SAMP_H ;
				handle_baseFeaturechosen = FALSE;
				this->Invalidate();
				AfxMessageBox("��������׼ͼ����ѡȡ��Ӧ������");

			}
		}
	}
	else
	{
		// �жϴ˵��Ƿ�Ϸ������жϴ˵��Ƿ��Ѿ�ѡ������ǣ���ȥ���˵�
		if (rectChoose3.PtInRect(point))
		{
			if (!rectChoose2.PtInRect(point))
			{
				AfxMessageBox("����ͼ��5��������ѡȡ������");
				handle_baseFeaturechosen = TRUE;
				return;
			}
			// �����ѡ�������������ǰ�������㣬��ȥ���˵�
			for (i = 0; i<m_nChsFeatureNum; i++)
			{
				// ѡ�����������ʾ�����Ա�����������ȡ��
				rectFeature2.bottom = floor(m_pPointBase[i].y * IMG_INIT_H /m_pDibInit->m_lpBMIH->biHeight +0.5)+ m_rectInitImage.top + 5;
				rectFeature2.top =floor( m_pPointBase[i].y * IMG_INIT_H / m_pDibInit->m_lpBMIH->biHeight +0.5)+ m_rectInitImage.top - 5;
				rectFeature2.left =floor( m_pPointBase[i].x * IMG_INIT_W / m_pDibInit->m_lpBMIH->biWidth +0.5)+ m_rectInitImage.left - 5;
				rectFeature2.right =floor( m_pPointBase[i].x * IMG_INIT_W / m_pDibInit->m_lpBMIH->biWidth+0.5) + m_rectInitImage.left + 5;

				// �ж���ѡ����������Ƿ�Ϊԭ��ѡ���������
				// ����ǣ���ȥ����������
				if (rectFeature2.PtInRect(point))
				{
					// ���������������ǰ�ƶ�һλ��ȥ����ѡ��Ĵ�������
					for (j = i; j<m_nChsFeatureNum - 1; j++)
					{
						m_pPointSampl[j] = m_pPointSampl[j + 1];
						m_pPointBase[j] = m_pPointBase[j + 1];
					}

					// ��������ļ�����һ
					m_nChsFeatureNum--;

					// ������ʾ
					Invalidate();

					// ���ñ�־λ
					bFlag2 = TRUE;

					return;
				}
			}

			// ����˵�����Ҫѡȡ�ģ��������ز���
			if (!bFlag2)
			{
				// ���˴���׼������ѡȡ��ע�����������������ͼ������Ͻ�Ϊԭ��ȷ����
				m_pPointBase[m_nChsFeatureNum].x = double(point.x - m_rectInitImage.left) / IMG_INIT_W *m_pDibInit->m_lpBMIH->biWidth;
				m_pPointBase[m_nChsFeatureNum].y =double( point.y - m_rectInitImage.top) / IMG_INIT_H *m_pDibInit->m_lpBMIH->biHeight;
				// �������������һ
				m_nChsFeatureNum++;
				handle_baseFeaturechosen = TRUE;
			}
		}
	}
}
/*************************************************************************
* \�������ƣ�
*   auto_Good()
* \�������:
* \����ֵ:��
* \˵��:�Զ�ѡ��������ʱɸѡ������
**************************************************************************/
void CDlgReg::auto_Good(UINT nFlags, CPoint point)
{
	// ѭ������
	int i, j;
	// �������ѡȡ��־λΪTRUE��������������ѡȡ����׼�������˳�
	if (!m_bChoseFeature)
		return;

	// ����׼ͼ�������ѡȡ����������ѡ���������ѡ������Ҫ��ͼ�����
	// ��СһȦ
	CRect rectChoose1;
	rectChoose1.bottom = m_rectResltImage.bottom - 5;
	rectChoose1.top = m_rectResltImage.top + 5;
	rectChoose1.left = m_rectResltImage.left + 5;
	rectChoose1.right = m_rectResltImage.right - 5;
	// ��׼ͼ�������ѡȡ����������ѡ���������ѡ������Ҫ��ͼ�����
	// ��СһȦ
	CRect rectChoose2;
	rectChoose2.bottom = m_rectInitImage.bottom - 5;
	rectChoose2.top = m_rectInitImage.top + 5;
	rectChoose2.left = m_rectInitImage.left + 5;
	rectChoose2.right = m_rectInitImage.right - 5;

	CRect rectChoose3;
	rectChoose3.bottom = m_rectInitImage.bottom;
	rectChoose3.top = m_rectInitImage.top;
	rectChoose3.left = m_rectInitImage.left;
	rectChoose3.right = m_rectInitImage.right;
	// �����������
	CRect rectFeature1;
	CRect rectFeature2;

	// ��־λ����ʾ�˵��Ƿ����Ѿ�ѡ���������
	BOOL bFlag1 = FALSE;
	BOOL bFlag2 = FALSE;


		// �жϴ˵��Ƿ�Ϸ������жϴ˵��Ƿ��Ѿ�ѡ������ǣ���ȥ���˵�
		if (rectChoose3.PtInRect(point))
		{
			
			// �����ѡ�������������ǰ�������㣬��ȥ���˵�
			for (i = 0; i<m_nChsFeatureNum; i++)
			{
				// ѡ�����������ʾ�����Ա�����������ȡ��
				rectFeature2.bottom =floor( m_pPointBase[i].y * IMG_INIT_H / m_pDibInit->m_lpBMIH->biHeight +0.5)+ m_rectInitImage.top + 5;
				rectFeature2.top =floor( m_pPointBase[i].y * IMG_INIT_H / m_pDibInit->m_lpBMIH->biHeight +0.5)+ m_rectInitImage.top - 5;
				rectFeature2.left =floor( m_pPointBase[i].x * IMG_INIT_W / m_pDibInit->m_lpBMIH->biWidth+0.5) + m_rectInitImage.left - 5;
				rectFeature2.right =floor( m_pPointBase[i].x * IMG_INIT_W / m_pDibInit->m_lpBMIH->biWidth +0.5)+ m_rectInitImage.left + 5;

				// �ж���ѡ����������Ƿ�Ϊԭ��ѡ���������
				// ����ǣ���ȥ����������
				if (rectFeature2.PtInRect(point))
				{
					// ���������������ǰ�ƶ�һλ��ȥ����ѡ��Ĵ�������
					for (j = i; j<m_nChsFeatureNum - 1; j++)
					{
						m_pPointSampl[j] = m_pPointSampl[j + 1];
						m_pPointBase[j] = m_pPointBase[j + 1];
					}

					// ��������ļ�����һ
					m_nChsFeatureNum--;

					// ������ʾ
					Invalidate();

					// ���ñ�־λ
					bFlag2 = TRUE;

					return;
				}
			}


		}
		// �жϴ˵��Ƿ�Ϸ������жϴ˵��Ƿ��Ѿ�ѡ������ǣ���ȥ���˵�,����׼��
		if (rectChoose1.PtInRect(point))
		{
			// �����ѡ�������������ǰ�������㣬��ȥ���˵�
			for (i = 0; i<m_nChsFeatureNum; i++)
			{
				// ѡ�����������ʾ�����Ա�����������ȡ��,��ʵ���ڶԻ�������(������С)
				rectFeature1.bottom = floor(m_pPointSampl[i].y * IMG_SAMP_H / m_pDibSamp->m_lpBMIH->biHeight +0.5)+ m_rectResltImage.top + 5;
				rectFeature1.top = floor(m_pPointSampl[i].y* IMG_SAMP_H / m_pDibSamp->m_lpBMIH->biHeight +0.5)+ m_rectResltImage.top - 5;
				rectFeature1.left =floor( m_pPointSampl[i].x* IMG_SAMP_W / m_pDibSamp->m_lpBMIH->biWidth +0.5)+ m_rectResltImage.left - 5;
				rectFeature1.right =floor( m_pPointSampl[i].x * IMG_SAMP_W / m_pDibSamp->m_lpBMIH->biWidth +0.5)+ m_rectResltImage.left + 5;

				// �ж���ѡ����������Ƿ�Ϊԭ��ѡ���������
				// ����ǣ���ȥ����������
				if (rectFeature1.PtInRect(point))
				{
					// ���������������ǰ�ƶ�һλ��ȥ����ѡ��Ĵ�������
					for (j = i; j<m_nChsFeatureNum - 1; j++)
					{
						m_pPointSampl[j] = m_pPointSampl[j + 1];
						m_pPointBase[j] = m_pPointBase[j + 1];
					}

					// ��������ļ�����һ
					m_nChsFeatureNum--;
					// ������ʾ
					Invalidate();
					// ���ñ�־λ
					bFlag1 = TRUE;
					return;
				}
			}

		}
	
}
/*************************************************************************
* \�������ƣ�
*   OnLButtonMove()
* \�������:��
* \����ֵ:��
* \˵��:
*���������ѡȡ��־λΪTRUE����ú���������׼������������Ϊʮ����״��
*�Ա��ܸ���ȷ�Ķ�λ�����㡣
**************************************************************************/
static void on_mouse1(int event, int x, int y, int flags, void* ustc)
{
	//CDlgReg *p;
	int foo = 60;
	//IplImage* org = p->org;
	//IplImage* img = p->img;
	//IplImage* dst = p->dst;
	IplImage* org = cvLoadImage("myBmp2.bmp", 1);
	IplImage* dst = cvCreateImage(cvSize(foo * 4, foo * 4), org->depth, org->nChannels);
	IplImage* img = cvCloneImage(org);
	IplImage* img1 = cvCreateImage(cvSize(IMG_W, IMG_H), img->depth, img->nChannels);
	//cvResize(img, img1);
	CvPoint center;
	//int radius = 3;
	x = x * org->width / IMG_W;
	y = y * org->height / IMG_H;

	//int foo = p->foo;

	if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))
	{
		cvCopy(org, img);
		//cvReleaseImage(&org);
		CvPoint p0;
		CvPoint p1;
		if (x<foo)
		{
			if (y<foo)
			{
				p0 = cvPoint(0, 0);
				p1 = cvPoint(2 * foo, 2 * foo);
			}
			else if (y>img->height - foo)
			{
				p0 = cvPoint(0, img->height - 2 * foo);
				p1 = cvPoint(2 * foo, img->height);
			}
			else
			{
				p0 = cvPoint(0, y - foo);
				p1 = cvPoint(2 * foo, y + foo);
			}
		}
		else if (x>img->width - foo)
		{
			if (y<foo)
			{
				p0 = cvPoint(img->width - 2 * foo, 0);
				p1 = cvPoint(img->width, 2 * foo);
			}
			else if (y>img->height - foo)
			{
				p0 = cvPoint(img->width - 2 * foo, img->height - 2 * foo);
				p1 = cvPoint(img->width, img->height);
			}
			else
			{
				p0 = cvPoint(img->width - 2 * foo, y - foo);
				p1 = cvPoint(img->width, y + foo);
			}
		}
		else
		{
			if (y<foo)
			{
				p0 = cvPoint(x - foo, 0);
				p1 = cvPoint(x + foo, 2 * foo);
			}
			else if (y>img->height - foo)
			{
				p0 = cvPoint(x - foo, img->height - 2 * foo);
				p1 = cvPoint(x + foo, img->height);
			}
			else
			{
				p0 = cvPoint(x - foo, y - foo);
				p1 = cvPoint(x + foo, y + foo);
			}
		}

		center.x = 120;
		center.y = 120;
		//cvResize(img, dst);288,320
		cvRectangle(img, p0, p1, CV_RGB(0, 255, 0));
		cvResize(img, img1);
		//cvReleaseImage(&img);
		cvSetImageROI(org, cvRect(p0.x, p0.y, p1.x - p0.x, p1.y - p0.y));
		cvResize(org, dst);
		cvResetImageROI(org);
		cvShowImage("img", img1);
		cvNamedWindow("dst", 1);
		cvCircle(dst, center, 5, CV_RGB(255, 0, 0));
		//cvMoveWindow("dst", 200, 500);
		cvShowImage("dst", dst);
		//cvCircle(dst,center,5, CV_RGB(255, 0, 0));
		//cvCircle(dst, center, radius, cvScalar(255, 0,0), -1);
		cvReleaseImage(&org);
		cvReleaseImage(&img);
	}

	if (flags & CV_EVENT_FLAG_LBUTTON || flags & CV_EVENT_FLAG_RBUTTON)
	{//cvWaitKey(0);

		cvDestroyAllWindows();
		//cvReleaseImage(&org);
		//cvReleaseImage(&img);
		cvReleaseImage(&img1);
		cvReleaseImage(&dst);
	}

}

static void on_mouse2(int event, int x, int y, int flags, void* ustc)
{
	//CDlgReg *p;
	int foo = 60;
	//IplImage* org = p->org;
	//IplImage* img = p->img;
	//IplImage* dst = p->dst;
	IplImage* org = cvLoadImage("myBmp1.bmp", 1);
	IplImage* dst = cvCreateImage(cvSize(foo * 4, foo * 4), org->depth, org->nChannels);
	IplImage* img = cvCloneImage(org);
	IplImage* img1 = cvCreateImage(cvSize(IMG_W, IMG_H), img->depth, img->nChannels);
	//cvResize(img, img1);
	CvPoint center;
	//int radius = 3;
	x = x * org->width / IMG_W;
	y = y * org->height / IMG_H;

	//int foo = p->foo;

	if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))
	{
		cvCopy(org, img);
		//cvReleaseImage(&org);
		CvPoint p0;
		CvPoint p1;
		if (x<foo)
		{
			if (y<foo)
			{
				p0 = cvPoint(0, 0);
				p1 = cvPoint(2 * foo, 2 * foo);
			}
			else if (y>img->height - foo)
			{
				p0 = cvPoint(0, img->height - 2 * foo);
				p1 = cvPoint(2 * foo, img->height);
			}
			else
			{
				p0 = cvPoint(0, y - foo);
				p1 = cvPoint(2 * foo, y + foo);
			}
		}
		else if (x>img->width - foo)
		{
			if (y<foo)
			{
				p0 = cvPoint(img->width - 2 * foo, 0);
				p1 = cvPoint(img->width, 2 * foo);
			}
			else if (y>img->height - foo)
			{
				p0 = cvPoint(img->width - 2 * foo, img->height - 2 * foo);
				p1 = cvPoint(img->width, img->height);
			}
			else
			{
				p0 = cvPoint(img->width - 2 * foo, y - foo);
				p1 = cvPoint(img->width, y + foo);
			}
		}
		else
		{
			if (y<foo)
			{
				p0 = cvPoint(x - foo, 0);
				p1 = cvPoint(x + foo, 2 * foo);
			}
			else if (y>img->height - foo)
			{
				p0 = cvPoint(x - foo, img->height - 2 * foo);
				p1 = cvPoint(x + foo, img->height);
			}
			else
			{
				p0 = cvPoint(x - foo, y - foo);
				p1 = cvPoint(x + foo, y + foo);
			}
		}

		center.x = 120;
		center.y = 120;
		//cvResize(img, dst);288,320
		cvRectangle(img, p0, p1, CV_RGB(0, 255, 0));
		cvResize(img, img1);
		//cvReleaseImage(&img);
		cvSetImageROI(org, cvRect(p0.x, p0.y, p1.x - p0.x, p1.y - p0.y));
		cvResize(org, dst);
		cvResetImageROI(org);
		cvShowImage("img", img1);
		cvNamedWindow("dst", 1);
		cvCircle(dst, center, 5, CV_RGB(255, 0, 0));
		//cvMoveWindow("dst", 600, 500);
		cvShowImage("dst", dst);
		//cvCircle(dst, center, radius, cvScalar(255, 0,0), -1);
		cvReleaseImage(&org);
		cvReleaseImage(&img);
	}

	if (flags & CV_EVENT_FLAG_LBUTTON || flags & CV_EVENT_FLAG_RBUTTON)
	{//cvWaitKey(0);

		cvDestroyAllWindows();
		//cvReleaseImage(&org);
		//cvReleaseImage(&img);
		cvReleaseImage(&img1);
		cvReleaseImage(&dst);
	}

}

void CDlgReg::OnMouseMove(UINT nFlags, CPoint point)
{
	// ���������������ѡȡ״̬���򲻽�����صĲ���
	if (m_bChoseFeature)
	{
		// �������ڴ���׼ͼ�������У�����������״
		if (m_rectResltImage.PtInRect(point) && auto_Feature ==FALSE ||
			m_rectInitImage.PtInRect(point) && handle_baseFeaturechosen == FALSE
			|| m_rectResltImage.PtInRect(point) && auto_Feature == FALSE&&handle_baseFeaturechosen == TRUE
			|| m_rectResltImage.PtInRect(point) && auto_Feature &&m_bChoseFeature
			|| m_rectInitImage.PtInRect(point) && auto_Feature &&m_bChoseFeature)
			
		{
			::SetCursor(LoadCursor(NULL, IDC_CROSS));
			if (MOUSE_R)
			{
				if (m_rectResltImage.PtInRect(point) && auto_Feature == FALSE&&handle_baseFeaturechosen == TRUE)
				{
					//org = cvLoadImage("myBmp2.bmp",1);
					//img = cvCloneImage(org);
					//dst = cvCreateImage(CvSize(foo * 4, foo * 4), org->depth, org->nChannels);
					//cvNamedWindow("img", 1);
					cvNamedWindow("img", CV_WINDOW_AUTOSIZE);
					HWND hWnd = (HWND)cvGetWindowHandle("img");
					HWND hParent = ::GetParent(hWnd);
					::SetParent(hWnd, GetDlgItem(IDC_REG_RESLT_IMAGE)->m_hWnd);
					::ShowWindow(hParent, SW_HIDE);


					cvSetMouseCallback("img", on_mouse1, 0);

					//cvNamedWindow("dst", 1);
					/*cvWaitKey(0);
					cvDestroyAllWindows();
					cvReleaseImage(&org);
					cvReleaseImage(&img);
					cvReleaseImage(&dst);*/
				}
				if (m_rectInitImage.PtInRect(point) && handle_baseFeaturechosen == FALSE)
				{
					cvNamedWindow("img", CV_WINDOW_AUTOSIZE);
					HWND hWnd = (HWND)cvGetWindowHandle("img");
					HWND hParent = ::GetParent(hWnd);
					::SetParent(hWnd, GetDlgItem(IDC_REG_INIT_IMAGE)->m_hWnd);
					::ShowWindow(hParent, SW_HIDE);

					cvSetMouseCallback("img", on_mouse2, 0);
				}
			}
			else
			{
				cvDestroyAllWindows();
				cvReleaseImage;
			}
		}
		else
		{
			::SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

/*************************************************************************
* \�������ƣ�
*   FindMatchPoint()
* \�������:
*   CDib*	pDibBase	- ��׼ͼ��ָ��
*   CDib*	pDibSamp	- ����׼ͼ��ָ��
*   CPoint	pointSamp	- ����׼�������λ��
* \����ֵ:
*   CPoint			- �����ڻ�׼ͼ������׼���������λ��
* \˵��:
*�ú������ݴ�����׼ͼ���е�������λ���ڻ�׼ͼ����Ѱ����׼�����㣬����
*��׼��������λ�÷��ء�����׼�Ĺ����У���ȡ���ǿ���׼�ķ���������׼��������
*ѡȡ��Ĵ�СΪ7*7�������ķ���Ϊȫ��������
**************************************************************************/
CPoint CDlgReg::FindMatchPoint(CDib* pDibBase, CDib* pDibSamp,
	CPoint pointSamp)
{
	// ѭ������
	int i, j, m, n;

	// ��ʱ����
	int nX, nY;

	// ��׼������λ��
	CPoint pointBase;

	// ��׼���ݿ�ĳߴ�
	int nBlockLen = 7;
	int nBlockHalfLen = 3;

	// ��׼ͼ������ָ��	
	unsigned char* pBase;
	pBase = (unsigned char *)pDibBase->m_lpImage;

	// ����׼ͼ������ָ��
	unsigned char* pSamp;
	pSamp = (unsigned char *)pDibSamp->m_lpImage;

	// ������λ�õ�������׼��
	unsigned char* pUnchSampBlock;
	pUnchSampBlock = new unsigned char[nBlockLen*nBlockLen];

	// ��ʱ�����ڴ棬���ڴ����׼���ݿ�
	unsigned char* pUnchBaseBlock;
	pUnchBaseBlock = new unsigned char[nBlockLen*nBlockLen];

	// ���ƶ�
	double dbCor;

	// ������ƶ�
	double dbMaxCor = 0;

	// ��׼ͼ��Ĵ洢��С
	CSize sizeBaseImg;
	sizeBaseImg = pDibBase->GetDibSaveDim();

	// ����׼ͼ��Ĵ洢��С
	CSize sizeSampImg;
	sizeSampImg = pDibSamp->GetDibSaveDim();

	// �Ӵ���׼ͼ������ȡ��������Ϊ���ĵ�nBlockLen*nBlockLen0�����ݿ�
	for (i = -nBlockHalfLen; i <= nBlockHalfLen; i++)
	{
		for (j = -nBlockHalfLen; j <= nBlockHalfLen; j++)
		{
			// ����˵���ͼ���е�λ��
			nX = pointSamp.x + i;
			nY = sizeSampImg.cy - pointSamp.y + j + 1;

			// ��ȡͼ������
			pUnchSampBlock[(j + nBlockHalfLen)*nBlockLen + (i + nBlockHalfLen)] =
				pSamp[nY*sizeSampImg.cx + nX];
		}
	}

	// ��׼ͼ��ĸ߶ȺͿ��
	int nBaseImgHeight, nBaseImgWidth;
	nBaseImgHeight = pDibBase->m_lpBMIH->biHeight;
	nBaseImgWidth = pDibBase->m_lpBMIH->biWidth;

	// �ڻ�׼ͼ����Ѱ����׼�����㣬��ȡ����������Ϊȫ������
	for (m = nBlockHalfLen; m< nBaseImgHeight - nBlockHalfLen; m++)
	{
		for (n = nBlockHalfLen; n<nBaseImgWidth - nBlockHalfLen; n++)
		{
			// ��ȡ�Դ˵�Ϊ���ģ���СΪnBlockLen*nBlockLen�����ݿ�
			for (i = -nBlockHalfLen; i <= nBlockHalfLen; i++)
			{
				for (j = -nBlockHalfLen; j <= nBlockHalfLen; j++)
				{
					// ����˵���ͼ���д洢��λ��
					nX = n + i;
					nY = sizeBaseImg.cy - m + j + 1;

					// ��ȡͼ������
					pUnchBaseBlock[(j + nBlockHalfLen)*nBlockLen + (i + nBlockHalfLen)] =
						pBase[nY*sizeBaseImg.cx + nX];
				}
			}


			// �����������ݿ������׼���������ƶ�
			dbCor = CalCorrelation(pUnchBaseBlock, pUnchSampBlock, nBlockLen);

			// �ж��Ƿ�Ϊ������ƶȣ�����ǣ����¼�����ƶȺ���׼������λ��
			if (dbCor > dbMaxCor)
			{
				dbMaxCor = dbCor;
				pointBase.x = n;
				pointBase.y = m;
			}
		}
	}
	return pointBase;
}

/*************************************************************************
* \�������ƣ�
*   CalCorrelation()
* \�������:
*   unsigned char*	pBase		- ��׼ͼ������ָ��
*   unsigned char*	pSamp		- ����׼ͼ������ָ��
*   int		nBlockLen	- ��׼���ݿ�ĳ߶ȴ�С
* \����ֵ:
*   double			- �����������ݿ���׼�����ƶ�
* \˵��:
*�ú����Ը�����������СΪnBlockLen*nBlockLen�����ݿ飬��������֮���
*����׼���ƶȡ����У�ȥ����ֵ���������ȱ任��Ӱ�졣
**************************************************************************/
double CDlgReg::CalCorrelation(unsigned char* pBase, unsigned char* pSamp, int nBlockLen)
{
	// ��ʱ����
	double dbSelfBase = 0, dbSelfSamp = 0;

	// ���ƶ�
	double dbCor = 0;

	// ���ֵ
	double dbMeanBase = 0, dbMeanSamp = 0;

	// �����������ƽ��ֵ
	for (int i = 0;i<nBlockLen;i++)
		for (int j = 0;j<nBlockLen;j++)
		{
			dbMeanBase += pBase[j*nBlockLen + i];
			dbMeanSamp += pSamp[j*nBlockLen + i];
		}
	dbMeanBase = dbMeanBase / (nBlockLen*nBlockLen);
	dbMeanSamp = dbMeanSamp / (nBlockLen*nBlockLen);

	// ��ȡ���ƶ�
	for (int i = 0;i<nBlockLen;i++)
		for (int j = 0;j<nBlockLen;j++)
		{
			dbSelfBase += (pBase[j*nBlockLen + i] - dbMeanBase)*
				(pBase[j*nBlockLen + i] - dbMeanBase);
			dbSelfSamp += (pSamp[j*nBlockLen + i] - dbMeanSamp)*
				(pSamp[j*nBlockLen + i] - dbMeanSamp);
			dbCor += (pBase[j*nBlockLen + i] - dbMeanBase)*
				(pSamp[j*nBlockLen + i] - dbMeanSamp);
		}
	dbCor = dbCor / sqrt(dbSelfBase * dbSelfSamp);

	// �������ƶ�
	return dbCor;
}

/*************************************************************************
* \�������ƣ�
*   OnRegReg()
* \�������:��
* \����ֵ:��
* \˵��:
*�ú���������ͼ��m_pDibInit��m_pDibSamp������׼�����У���Ҫ��ȷ�������㣬
*������������Ҫѡȡ3�����ϣ���һ������Ҫ�ڡ�ѡȡ�����㡱�����н��С�����ѡȡ
*�������㣬����õ��ռ��������󣬽���׼������ͼ��ƴ��������ʾ��
***************************************************************************/
void CDlgReg::OnRegReg()
{
	//if (auto_Feature)//�Զ�
	//{
	//	auto_Featurechosen();
	//	m_bChoseFeature = TRUE;
	//}
	// ������׼֮ǰ���ж��Ƿ��Ѿ�ѡȡ������
	if (!m_bChoseFeature)  //��Ҫ�ֶ����ж�
	{
		AfxMessageBox("��û��ѡȡ�����㣬����ѡȡ������");
		return;
	}

	// ���ѡȡ����������Ŀ������Ҳ���ܽ��д���
	if (m_nChsFeatureNum < 3)
	{
		AfxMessageBox("��ѡ������3�������㣬�ٽ���ͼ����׼");
		return;
	}

	// �任ϵ��Ϊ6������ϵ��Ϊ�ӻ�׼ͼ�񵽴���׼ͼ��ı任ϵ��
	double *pDbBs2SpAffPara;
	pDbBs2SpAffPara = new double[2 * 3];

	// �任ϵ��Ϊ6������ϵ��Ϊ�Ӵ���׼ͼ�񵽻�׼ͼ��ı任ϵ��
	double *pDbSp2BsAffPara;
	pDbSp2BsAffPara = new double[2 * 3];



	// ����Ѿ�ѡȡ�������㣬���õ�����׼�������㣬����Լ���ռ�任ϵ����

		std::vector<cv::Point2f> imagePointsBase(m_nChsFeatureNum), imagePointsReg(m_nChsFeatureNum);


		for (int i = 0;i<m_nChsFeatureNum;i++)
		{
			imagePointsBase[i].x= m_pPointBase[i].x;
			imagePointsReg[i].x = m_pPointSampl[i].x;
			imagePointsBase[i].y = m_pPointBase[i].y;
			imagePointsReg[i].y = m_pPointSampl[i].y;
		}
		//�õ��������
		cv::Mat homoT, homoRT;
		if (m_nChsFeatureNum == 3)
		{
			homoT = cv::getAffineTransform(imagePointsBase, imagePointsReg);
			homoRT=cv::getAffineTransform(imagePointsReg, imagePointsBase);
		}
		else
		{
			homoT = cv::estimateRigidTransform(imagePointsBase, imagePointsReg, CV_RANSAC);
			homoRT = cv::estimateRigidTransform(imagePointsReg, imagePointsBase, CV_RANSAC);
		}

		homoT.copyTo(homo);
		homoRT.copyTo(homoR);


		//����RMS
		GetDlgItem(IDC_COMBORMS)->EnableWindow(TRUE);
		RMS_compute();
		


	// ������ֵ
	for(int r = 0;  r < 2; r++)
		for (int c = 0;c < 3;c++)
		{
			pDbBs2SpAffPara[r * 3 + c] = homo.at<double>(r, c);
			pDbSp2BsAffPara[r * 3 + c] = homoR.at<double>(r, c);
		}




	// ����ͼ�񾭹��ռ�任��ĳߴ��С
	CRect rectAftAff;
	rectAftAff = GetAftAffDim(pDbSp2BsAffPara);

	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth = rectAftAff.right - rectAftAff.left;
	nNewImgHeight = rectAftAff.bottom - rectAftAff.top;

	// ����׼ͼ������µ�ͼ����
	LPBYTE lpBaseImg;
	lpBaseImg = SetBaseImgAftAff(rectAftAff);

	// ������׼ͼ������µ�ͼ����
	LPBYTE lpSampImg;
	lpSampImg = SetSampImgAftAff(pDbBs2SpAffPara, rectAftAff);

	// ����ͼ����CDib���װ
	m_pDibResult = new CDib(CSize(nNewImgWidth, nNewImgHeight), 8);

	// ������ͼ��Ĵ洢��С�ߴ�
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// ������ɫ��
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// Ӧ�õ�ɫ��
	m_pDibResult->MakePalette();

	// �����ڴ���ϲ����ͼ��
	LPBYTE lpImgResult;
	lpImgResult = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	// ��ͼ����и�ֵ
	for (int i = 0; i<nNewImgWidth; i++)
		for (int j = 0; j<nNewImgHeight; j++)
		{
			int nX = i;
			int nY = sizeSaveResult.cy - j - 1;
			if (lpBaseImg[j*nNewImgWidth + i] != 0 && lpSampImg[j*nNewImgWidth + i] == 0)
				lpImgResult[nY*sizeSaveResult.cx + nX] = lpBaseImg[j*nNewImgWidth + i];
			if (lpBaseImg[j*nNewImgWidth + i] == 0 && lpSampImg[j*nNewImgWidth + i] != 0)
				lpImgResult[nY*sizeSaveResult.cx + nX] = lpSampImg[j*nNewImgWidth + i];
			if (lpBaseImg[j*nNewImgWidth + i] != 0 && lpSampImg[j*nNewImgWidth + i] != 0)
				lpImgResult[nY*sizeSaveResult.cx + nX] = (lpSampImg[j*nNewImgWidth + i] + lpBaseImg[j*nNewImgWidth + i]) / 2;
		}

	// ��ָ�븳ֵ��CDib�������
	m_pDibResult->m_lpImage = lpImgResult;

	// ��ʾ�ϲ����ͼ��
	CDlgAftReg* pDlg;
	pDlg = new CDlgAftReg(NULL, m_pDibResult);
	pDlg->DoModal();

	// ɾ������
	delete pDlg;

	// �ͷ��ѷ����ڴ�
	delete[]lpBaseImg;
	delete[]lpSampImg;
	delete[]pDbBs2SpAffPara;
	delete[]pDbSp2BsAffPara;
}


/*************************************************************************
* \�������ƣ�
*   GetAffinePara()
* \�������:
*   double  *pDbBs2SpAffPara	- ���ڴ�Ż�׼ͼ�񵽴���׼ͼ��Ŀռ�任ϵ��
*   double  *pDbSp2BsAffPara	- ���ڴ�Ŵ���׼ͼ�񵽻�׼ͼ��Ŀռ�任ϵ��
* \����ֵ:��
* \˵��:
*�ú������ݵõ���������׼�������㣬����ռ�任ϵ�����õ��Ŀռ�任ϵ��
*������������������ָ����ڴ��С�
**************************************************************************/
void CDlgReg::GetAffinePara(MyPoint* pPointBase, MyPoint* pPointSampl, double* pDbAffPara)
{
	// pDbBMatrix�д�ŵ��ǻ�׼ͼ��������������꣬
	// ��СΪ2*m_nChsFeatureNum��ǰm_nChsFeatureNumΪX����
	double *pDbBMatrix;
	pDbBMatrix = new double[2 * m_nChsFeatureNum];

	// pDbSMatrix�д�ŵ��Ǵ���׼ͼ�������������չ����
	// ��СΪ3*m_nChsFeatureNum������ǰm_nChsFeatureNumΪX����
	// �м�m_nChsFeatureNum��ΪY���꣬����m_nChsFeatureNumΪ1
	double *pDbSMatrix;
	pDbSMatrix = new double[3 * m_nChsFeatureNum];

	// pDbSMatrixT�д�ŵ�pDbSMatrix��ת�þ���
	// ��СΪm_nChsFeatureNum*3
	double *pDbSMatrixT;
	pDbSMatrixT = new double[m_nChsFeatureNum * 3];

	// pDbInvMatrixΪ��ʱ��������ŵ���pDbSMatrix*pDbSMatrixT����
	// ��СΪ3*3
	double *pDbInvMatrix;
	pDbInvMatrix = new double[3 * 3];

	// ��ʱ�ڴ�
	double *pDbTemp;
	pDbTemp = new double[2 * 3];

	// ѭ������
	int count;

	// ������ֵ
	for (count = 0; count<m_nChsFeatureNum; count++)
	{
		pDbBMatrix[0 * m_nChsFeatureNum + count] = pPointBase[count].x;
		pDbBMatrix[1 * m_nChsFeatureNum + count] = pPointBase[count].y;
		pDbSMatrix[0 * m_nChsFeatureNum + count] = pPointSampl[count].x;
		pDbSMatrix[1 * m_nChsFeatureNum + count] = pPointSampl[count].y;
		pDbSMatrix[2 * m_nChsFeatureNum + count] = 1;
		pDbSMatrixT[count*m_nChsFeatureNum + 0] = pPointSampl[count].x;
		pDbSMatrixT[count*m_nChsFeatureNum + 1] = pPointSampl[count].y;
		pDbSMatrixT[count*m_nChsFeatureNum + 2] = 1;
	}

	// ����pDbSMatrix*pDbSMatrixT�������������pDbInvMatrix��
	CalMatProduct(pDbSMatrix, pDbSMatrixT, pDbInvMatrix, 3, 3, m_nChsFeatureNum);

	// ����pDbInvMatrix�������
	CalInvMatrix(pDbInvMatrix, 3);

	// ����ռ�任ϵ��
	CalMatProduct(pDbBMatrix, pDbSMatrixT, pDbTemp, 2, 3, m_nChsFeatureNum);
	CalMatProduct(pDbTemp, pDbInvMatrix, pDbAffPara, 2, 3, 3);

	// �ͷ��ڴ�
	delete[]pDbBMatrix;
	delete[]pDbSMatrix;
	delete[]pDbSMatrixT;
	delete[]pDbInvMatrix;
	delete[]pDbTemp;

}

/*************************************************************************
* \�������ƣ�
*   CalMatProduct()
* \�������:
*   double  *pDbSrc1	- ָ����˾���1���ڴ�
*   double  *pDbSrc2	- ָ����˾���2���ڴ�
*   double  *pDbDest   - ��ž���������н�����ڴ�ָ��
*   int     nX		- ����ĳߴ磬����μ�����˵��
*   int     nY		- ����ĳߴ磬����μ�����˵��
*   int     nZ		- ����ĳߴ磬����μ�����˵��
* \����ֵ:��
* \˵��:
*�ú������������������ˣ�Ȼ����˵Ľ�������pDbDest�С�����pDbSrc1
*�Ĵ�СΪnX*nZ��pDbSrc2�Ĵ�СΪnZ*nY��pDbDest�Ĵ�СΪnX*nY
**************************************************************************/
void CDlgReg::CalMatProduct(double* pDbSrc1, double *pDbSrc2, double *pDbDest, int y, int x, int z)
{
	for (int i = 0;i<y;i++)
		for (int j = 0;j<x;j++)
		{
			pDbDest[i*x + j] = 0;
			for (int m = 0;m<z;m++)
				pDbDest[i*x + j] += pDbSrc1[i*z + m] * pDbSrc2[m*x + j];
		}
}

/*************************************************************************
* \�������ƣ�
*   CalInvMatrix()
* \�������:
*   double  *pDbSrc	- ָ������ָ��
*   int     nLen	- ����ĳߴ�
* \����ֵ:��
* \˵��:
*�ú����������pDbSrc�����������pDbSrc�Ĵ�СΪnLen*nLen
**************************************************************************/
BOOL CDlgReg::CalInvMatrix(double *pDbSrc, int nLen)
{
	int *is, *js, i, j, k;
	double d, p;
	is = new int[nLen];
	js = new int[nLen];
	for (k = 0;k<nLen;k++)
	{
		d = 0.0;
		for (i = k;i<nLen;i++)
			for (j = k;j<nLen;j++)
			{
				p = fabs(pDbSrc[i*nLen + j]);
				if (p>d)
				{
					d = p;
					is[k] = i;
					js[k] = j;
				}
			}
		if (d + 1.0 == 1.0)
		{
			delete is;
			delete js;
			return FALSE;
		}
		if (is[k] != k)
			for (j = 0;j<nLen;j++)
			{
				p = pDbSrc[k*nLen + j];
				pDbSrc[k*nLen + j] = pDbSrc[(is[k] * nLen) + j];
				pDbSrc[(is[k])*nLen + j] = p;
			}
		if (js[k] != k)
			for (i = 0; i<nLen; i++)
			{
				p = pDbSrc[i*nLen + k];
				pDbSrc[i*nLen + k] = pDbSrc[i*nLen + (js[k])];
				pDbSrc[i*nLen + (js[k])] = p;
			}

		pDbSrc[k*nLen + k] = 1.0 / pDbSrc[k*nLen + k];
		for (j = 0; j<nLen; j++)
			if (j != k)
			{
				pDbSrc[k*nLen + j] *= pDbSrc[k*nLen + k];
			}
		for (i = 0; i<nLen; i++)
			if (i != k)
				for (j = 0; j<nLen; j++)
					if (j != k)
					{
						pDbSrc[i*nLen + j] -= pDbSrc[i*nLen + k] * pDbSrc[k*nLen + j];
					}
		for (i = 0; i<nLen; i++)
			if (i != k)
			{
				pDbSrc[i*nLen + k] *= -pDbSrc[k*nLen + k];
			}
	}
	for (k = nLen - 1; k >= 0; k--)
	{
		if (js[k] != k)
			for (j = 0; j<nLen; j++)
			{
				p = pDbSrc[k*nLen + j];
				pDbSrc[k*nLen + j] = pDbSrc[(js[k])*nLen + j];
				pDbSrc[(js[k])*nLen + j] = p;
			}
		if (is[k] != k)
			for (i = 0; i<nLen; i++)
			{
				p = pDbSrc[i*nLen + k];
				pDbSrc[i*nLen + k] = pDbSrc[i*nLen + (is[k])];
				pDbSrc[i*nLen + (is[k])] = p;
			}
	}
	delete is;
	return TRUE;
}

/*************************************************************************
* \�������ƣ�
*  GetAftAffDim()
* \�������:
*   double  *pDbAffPara	- �ռ�任ϵ������
* \����ֵ:
*   CRect			- ���ش���׼ͼ�񾭿ռ�任�������
* \˵��:
*   �ú������ݿռ�任ϵ�����������׼ͼ��ռ�任���ͼ��ߴ��С
***************************************************************************/
CRect CDlgReg::GetAftAffDim(double* pDbAffPara)
{
	// ��׼ͼ��Ŀ�Ⱥ͸߶�
	int nBaseImgWidth, nBaseImgHeight;
	nBaseImgWidth = m_pDibInit->m_lpBMIH->biWidth;
	nBaseImgHeight = m_pDibInit->m_lpBMIH->biHeight;

	// ����׼ͼ��Ŀ�Ⱥ͸߶�
	int nSamplImgWidth, nSamplImgHeight;
	nSamplImgWidth = m_pDibSamp->m_lpBMIH->biWidth;
	nSamplImgHeight = m_pDibSamp->m_lpBMIH->biHeight;

	// ��׼ͼ���ԭʼ����
	CRect rectBase(0, 0, nBaseImgWidth, nBaseImgHeight);

	// ��ʱ����
	CPoint pointTemp;
	double tx, ty;

	// ͼ��Ķ˵�
	pointTemp.x = 0;
	pointTemp.y = 0;

	// �����pointTemp�����ռ�任�������
	tx = pDbAffPara[0 * 3 + 0] * pointTemp.x +
		pDbAffPara[0 * 3 + 1] * pointTemp.y + pDbAffPara[0 * 3 + 2];
	ty = pDbAffPara[1 * 3 + 0] * pointTemp.x +
		pDbAffPara[1 * 3 + 1] * pointTemp.y + pDbAffPara[1 * 3 + 2];

	// �ж�pointTemp�����ռ�任���Ƿ񳬳�ԭ���Ĵ�С
	if (tx<rectBase.left)
		rectBase.left = (int)tx;
	if (tx>rectBase.right)
		rectBase.right = (int)tx + 1;
	if (ty<rectBase.top)
		rectBase.top = (int)ty;
	if (ty>rectBase.bottom)
		rectBase.bottom = (int)ty + 1;

	// ����˵�(0, nSamplImgHeight)�任�������
	pointTemp.x = 0; pointTemp.y = nSamplImgHeight;
	tx = pDbAffPara[0 * 3 + 0] * pointTemp.x +
		pDbAffPara[0 * 3 + 1] * pointTemp.y + pDbAffPara[0 * 3 + 2];
	ty = pDbAffPara[1 * 3 + 0] * pointTemp.x +
		pDbAffPara[1 * 3 + 1] * pointTemp.y + pDbAffPara[1 * 3 + 2];

	// �ж��Ƿ�Խ��
	if (tx<rectBase.left)
		rectBase.left = (int)tx;
	if (tx>rectBase.right)
		rectBase.right = (int)tx + 1;
	if (ty<rectBase.top)
		rectBase.top = (int)ty;
	if (ty>rectBase.bottom)
		rectBase.bottom = (int)ty + 1;

	// ����˵�(nSamplImgWidth, nSamplImgHeight)�任�������
	pointTemp.x = nSamplImgWidth; pointTemp.y = nSamplImgHeight;
	tx = pDbAffPara[0 * 3 + 0] * pointTemp.x +
		pDbAffPara[0 * 3 + 1] * pointTemp.y + pDbAffPara[0 * 3 + 2];
	ty = pDbAffPara[1 * 3 + 0] * pointTemp.x +
		pDbAffPara[1 * 3 + 1] * pointTemp.y + pDbAffPara[1 * 3 + 2];

	// �ж��Ƿ�Խ��
	if (tx<rectBase.left)
		rectBase.left = (int)tx;
	if (tx>rectBase.right)
		rectBase.right = (int)tx + 1;
	if (ty<rectBase.top)
		rectBase.top = (int)ty;
	if (ty>rectBase.bottom)
		rectBase.bottom = (int)ty + 1;

	// ����˵�(nSamplImgWidth, 0)�任�������
	pointTemp.x = nSamplImgWidth; pointTemp.y = 0;
	tx = pDbAffPara[0 * 3 + 0] * pointTemp.x +
		pDbAffPara[0 * 3 + 1] * pointTemp.y + pDbAffPara[0 * 3 + 2];
	ty = pDbAffPara[1 * 3 + 0] * pointTemp.x +
		pDbAffPara[1 * 3 + 1] * pointTemp.y + pDbAffPara[1 * 3 + 2];

	// �ж��Ƿ�Խ��
	if (tx<rectBase.left)
		rectBase.left = (int)tx;
	if (tx>rectBase.right)
		rectBase.right = (int)tx + 1;
	if (ty<rectBase.top)
		rectBase.top = (int)ty;
	if (ty>rectBase.bottom)
		rectBase.bottom = (int)ty + 1;

	// ���ش���׼ͼ��任��������С
	return(rectBase);
}

/*************************************************************************
* \�������ƣ�
*   SetSampImgAftAff()
* \�������:
*   double  *pDbAffPara	- �ռ�任ϵ������
*   CRect   rectNewImg		- �任��ͼ��Ĵ�С�ߴ�
* \����ֵ:
*   LPBYTE			- ���ر任���ͼ��
* \˵��:
*�ú������ݿռ�任ϵ�����������׼ͼ��ռ�任���ͼ�񡣲����ش�ͼ��ָ��
*��ͼ��Ĵ�СΪrectNewImg
**************************************************************************/
LPBYTE CDlgReg::SetSampImgAftAff(double* pDbAffPara, CRect rectNewImg)
{
	// pUnchSect��4*4��С�ľ�������
	unsigned char *pUnchSect;
	pUnchSect = new unsigned char[4 * 4];

	// �µ�ͼ���Ⱥ͸߶�
	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth = rectNewImg.right - rectNewImg.left;
	nNewImgHeight = rectNewImg.bottom - rectNewImg.top;

	// ����׼ͼ��Ŀ�Ⱥ͸߶�
	int nSamplImgWidth, nSamplImgHeight;
	nSamplImgWidth = m_pDibSamp->m_lpBMIH->biWidth;
	nSamplImgHeight = m_pDibSamp->m_lpBMIH->biHeight;

	// ����׼ͼ��Ĵ洢���
	int nSampSaveWidth;
	nSampSaveWidth = m_pDibSamp->GetDibSaveDim().cx;

	// pUnchAftAffSamp��һ����СΪrectNewImg��С��ͼ��
	// ����rectNewImg��ʾ�任���ͼ���С
	unsigned char *pUnchAftAffSamp;
	pUnchAftAffSamp = new unsigned char[nNewImgWidth * nNewImgHeight];

	double tx, ty;

	// �����ڱ任���ͼ�������
	for (int i = 0;i<rectNewImg.bottom - rectNewImg.top;i++)
		for (int j = 0;j<rectNewImg.right - rectNewImg.left;j++)
		{
			tx = pDbAffPara[0 * 3 + 0] * (j + rectNewImg.left) +
				pDbAffPara[0 * 3 + 1] * (i + rectNewImg.top) + pDbAffPara[0 * 3 + 2];
			ty = pDbAffPara[1 * 3 + 0] * (j + rectNewImg.left) +
				pDbAffPara[1 * 3 + 1] * (i + rectNewImg.top) + pDbAffPara[1 * 3 + 2];

			for (int m = (int)ty - 1;m <= (int)ty + 2;m++)
				for (int n = (int)tx - 1;n <= (int)tx + 2;n++)
				{
					if (m<0 || m >= nSamplImgHeight || n<0 || n >= nSamplImgWidth)
						pUnchSect[(m - (int)ty + 1) * 4 + (n - (int)tx + 1)] = 0;
					else
						pUnchSect[(m - (int)ty + 1) * 4 + (n - (int)tx + 1)] =
						m_pDibSamp->m_lpImage[(nSamplImgHeight - m - 1)*nSampSaveWidth + n];
				}

			// ȷ���任������
			ty = ty - (int)ty + 1;
			tx = tx - (int)tx + 1;

			// ȷ���任����������ֵ
			pUnchAftAffSamp[i*nNewImgWidth + j] = CalSpline(pUnchSect, tx, ty);
		}
	// �ͷ��ڴ�
	delete[]pUnchSect;
	// ����ָ��
	return (LPBYTE)pUnchAftAffSamp;
}

/*************************************************************************
* \�������ƣ�
*   CalSpline()
* \�������:
*   unsigned char	*pUnchCorr	- ��ֵ�ĵ�
*   double		dX		- X����
*   double		dY		- Y����
* \����ֵ:
*   unsigned char			- ��ֵ���ֵ
* \˵��:
*�ú��������ڽ�λ�õ���ֵ���в�ֵ����ͼ��Ĵ�СΪrectNewImg
*************************************************************************/
unsigned char CDlgReg::CalSpline(unsigned char *pUnchCorr, double x, double y)
{
	double ret = 0, Cx, Cy;
	double Temp;

	for (int i = 0;i<4;i++)
		for (int j = 0;j<4;j++)
		{
			Temp = pUnchCorr[i * 4 + j];
			if (fabs(y - i)<1)
				Cy = 1 - 2 * fabs(y - i)*fabs(y - i) + fabs(y - i)*fabs(y - i)*fabs(y - i);
			if (fabs(y - i) >= 1)
				Cy = 4 - 8 * fabs(y - i) + 5 * fabs(y - i)*fabs(y - i) - fabs(y - i)*fabs(y - i)*fabs(y - i);
			if (fabs(x - j)<1)
				Cx = 1 - 2 * fabs(x - j)*fabs(x - j) + fabs(x - j)*fabs(x - j)*fabs(x - j);
			if (fabs(x - j) >= 1)
				Cx = 4 - 8 * fabs(x - j) + 5 * fabs(x - j)*fabs(x - j) - fabs(x - j)*fabs(x - j)*fabs(x - j);
			ret += Temp*Cy*Cx;
		}
	if (ret<0)
		ret = 0;
	if (ret>255)
		ret = 255;

	return (unsigned char)ret;
}

/*************************************************************************
* \�������ƣ�
*   SetBaseImgAftAff()
* \�������:
*   double  *pDbAffPara	- �ռ�任ϵ������
* \����ֵ:��
* \˵��:
*�ú������ݿռ�任ϵ���������׼ͼ��ռ�任���ͼ�񣬲����ش�Ŵ�
*���ݵ�ָ��
**************************************************************************/
LPBYTE CDlgReg::SetBaseImgAftAff(CRect rectNewImg)
{
	// ��ͼ��Ĵ�С
	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth = rectNewImg.right - rectNewImg.left;
	nNewImgHeight = rectNewImg.bottom - rectNewImg.top;

	// �任��ͼ��
	unsigned char *pUnchAftAffBase;
	pUnchAftAffBase = new unsigned char[nNewImgWidth*nNewImgHeight];

	// ��׼ͼ��ĸ߶ȺͿ��
	int nBaseWidth, nBaseHeight;
	nBaseWidth = m_pDibInit->m_lpBMIH->biWidth;
	nBaseHeight = m_pDibInit->m_lpBMIH->biHeight;

	// ��׼ͼ��Ĵ洢���
	int nBaseSaveWidth;
	nBaseSaveWidth = m_pDibInit->GetDibSaveDim().cx;//

	for (int i = 0;i<rectNewImg.bottom - rectNewImg.top;i++)
		for (int j = 0;j<rectNewImg.right - rectNewImg.left;j++)
		{
			if (i<-rectNewImg.top || i >= -rectNewImg.top + nBaseHeight || j<-rectNewImg.left || j >= -rectNewImg.left + nBaseWidth)
				pUnchAftAffBase[i*nNewImgWidth + j] = 0;
			else
				pUnchAftAffBase[i*nNewImgWidth + j] = m_pDibInit->m_lpImage[(nBaseHeight - (i + rectNewImg.top) - 1)*nBaseSaveWidth + (j + rectNewImg.left)];
		}

	// ����
	return (LPBYTE)pUnchAftAffBase;
}
void CDlgReg::RMS_compute()
{
	//����RMS
	double tx, ty;
	CString t;
	m_rms_show.ResetContent();
	for (int i = 0;i<m_nChsFeatureNum;i++)
	{
		tx = homoR.at<double>(0, 0)*m_pPointSampl[i].x +
			homoR.at<double>(0, 1)*m_pPointSampl[i].y + homoR.at<double>(0, 2);
		ty = homoR.at<double>(1, 0)*m_pPointSampl[i].x +
			homoR.at<double>(1, 1)*m_pPointSampl[i].y + homoR.at<double>(1, 2);

		RMS[i] = sqrt((tx - m_pPointBase[i].x)*(tx - m_pPointBase[i].x) + (ty - m_pPointBase[i].y)*(ty - m_pPointBase[i].y));
		//��ʾ
		
		
		t.Format("point %d: %f", i + 1,RMS[i]);
		m_rms_show.AddString(t);
		
		
	}
	UpdateData(FALSE);
	
	
}

void CDlgReg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//if (auto_Feature == FALSE&&handle_baseFeaturechosen == FALSE)
	//	AfxMessageBox("�����ڻ�׼ͼ����ѡȡ��������Ķ�Ӧ�㣬Ȼ�����л������Զ���ģʽ");
	GetDlgItem(IDC_COMBOAUTO)->EnableWindow(TRUE);
	auto_Feature = TRUE;
	
}

void CDlgReg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_COMBOAUTO)->EnableWindow(FALSE);
	auto_Feature = FALSE;
	
}

void CDlgReg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CDlgReg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}


void CDlgReg::OnDropdownComboauto()
{

	// TODO: �ڴ���ӿؼ�֪ͨ����������


	
}




void CDlgReg::OnFeatureRms()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_rms_show.GetCurSel();
	if (nIndex == CB_ERR)
	{
		AfxMessageBox("��ѡ��Ҫȥ���ĵ㣡");
		return;
	}
	for (int i = nIndex;i < m_nChsFeatureNum;i++)
	{
		m_pPointBase[i] = m_pPointBase[i + 1];
		m_pPointSampl[i] = m_pPointSampl[i + 1];
		RMS[i] = RMS[i + 1];
	}
	m_nChsFeatureNum--;
	CString t;
	m_rms_show.ResetContent();
	for (int j = 0;j<m_nChsFeatureNum;j++)
	{
	t.Format("point %d: %f", j + 1, RMS[j]);
	m_rms_show.AddString(t);
	}
	UpdateData(FALSE);


	Invalidate();
		
}


void CDlgReg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	MOUSE_R = !MOUSE_R;


	CDialog::OnRButtonUp(nFlags, point);
}
