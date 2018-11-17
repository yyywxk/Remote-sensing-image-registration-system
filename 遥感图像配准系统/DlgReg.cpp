// DlgReg.cpp : implementation file
//
#include "stdafx.h"
#include "遥感图像配准系统.h"
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

	// 获取文档类指针
	m_pDoc = pDoc;

	// 设置计算图像位置标志位位FALSE
	m_bCalImgLoc = FALSE;

	// 设置基准图像为原始打开的图像
	m_pDibInit = pDoc->m_pDibInit;

	// 设置待配准图像
	m_pDibSamp = new CDib;

	// 设置选取特征点的数目初始值
	m_nChsFeatureNum = 0;
	//设置手动特征选取标志位为TRUE
	handle_baseFeaturechosen = TRUE;

	// 设置选取特征点的标志位为FALSE
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

					   // 如果还没有计算图像的位置，则进行计算
	//if (!m_bCalImgLoc)
		CalImageLocation();

	CSize sizeDisplay;
	CPoint pointDisplay;

	// 显示基准图像
	if (!m_pDibInit->IsEmpty())
	{
		
		sizeDisplay.cx = IMG_INIT_W;
		sizeDisplay.cy = IMG_INIT_H;
		pointDisplay.x = m_rectInitImage.left;
		pointDisplay.y = m_rectInitImage.top;
		//计算基准图显示区大小用于比例缩放计算
		len_init_x = sizeDisplay.cx;
		len_init_y = sizeDisplay.cy;

		m_pDibInit->Draw(&dc, pointDisplay, sizeDisplay);
	}

	// 显示待配准图像
	if (!m_pDibSamp->IsEmpty())
	{
		sizeDisplay.cx = IMG_SAMP_W;
		sizeDisplay.cy = IMG_SAMP_H;

		pointDisplay.x = m_rectResltImage.left;
		pointDisplay.y = m_rectResltImage.top;

		//计算待配准图显示区大小用于比例缩放计算
		len_reg_x = sizeDisplay.cx;
		len_reg_y = sizeDisplay.cy;
		m_pDibSamp->Draw(&dc, pointDisplay, sizeDisplay);
	}


	// 显示特征点与配准的特征点
	DrawFeature(&dc);
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
void CDlgReg::CalImageLocation()
{
	// 获得控件IDC_REG_INIT_IMAGE的句柄，并获得控件的初始位置信息
	CWnd* pWnd = GetDlgItem(IDC_REG_INIT_IMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement = new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);

	// 图像宽度
	int nImageWidth1;
	nImageWidth1 = m_pDibInit->m_lpBMIH->biWidth;

	// 图像高
	int nImageHeight1;
	nImageHeight1 = m_pDibInit->m_lpBMIH->biHeight;

	// 调整控件IDC_REG_INIT_IMAGE的大小位置，并同时设置显示基准图像的位置
	
	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + IMG_INIT_H;
		m_rectInitImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectInitImage.top = winPlacement->rcNormalPosition.top;
	
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + IMG_INIT_W;
		m_rectInitImage.right = winPlacement->rcNormalPosition.right;
		m_rectInitImage.left = winPlacement->rcNormalPosition.left;


	// 设置IDC_REG_INIT_IMAGE控件的大小位置
	pWnd->SetWindowPlacement(winPlacement);

	// 获得显示基准图像控件的右边位置，以便确认显示待配准图像控件的位置
	int nIniImgRight;
	nIniImgRight = winPlacement->rcNormalPosition.right;

	int  nIniImgLeft;
	nIniImgLeft = winPlacement->rcNormalPosition.left;

	// 获得IDC_REG_INIT_IMAGE控件的下边位置，以便调整其他控件的位置
	int nIniImgBottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;



	// 获得控件IDC_REG_RESLT_IMAGE的句柄，并获得初始位置信息
	pWnd = GetDlgItem(IDC_REG_RESLT_IMAGE);
	pWnd->GetWindowPlacement(winPlacement);

	int nImageHeight2, nImageWidth2;
	// 如果还未打开待配准图像，则设置待配准图像大小和基准图像大小相等
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
	// 调整控件IDC_REG_RESLT_IMAGE的大小位置，并同时设置显示待配准图像的位置

	// 先调整控件的左边位置，和IDC_REG_INIT_IMAGE控件相隔15个象素
	winPlacement->rcNormalPosition.left = nIniImgRight + 15;

	
	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + IMG_SAMP_H;
		m_rectResltImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectResltImage.top = winPlacement->rcNormalPosition.top;
	
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + IMG_SAMP_W;
		m_rectResltImage.right = winPlacement->rcNormalPosition.right;
		m_rectResltImage.left = winPlacement->rcNormalPosition.left;
	
	// 设置IDC_REG_RESLT_IMAGE控件的大小位置
	pWnd->SetWindowPlacement(winPlacement);

	if (nIniImgBottom < winPlacement->rcNormalPosition.bottom)
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;

	nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	nIniImgRight = winPlacement->rcNormalPosition.right;

	if (nImageHeight2<nImageHeight1)
	{
		// 获得控件IDC_REG_INIT_IMAGE的句柄，并获得控件的初始位置信息
		CWnd* pWnd = GetDlgItem(IDC_REG_INIT_IMAGE);
		WINDOWPLACEMENT *winPlacement;
		winPlacement = new WINDOWPLACEMENT;
		pWnd->GetWindowPlacement(winPlacement);
		if (nIniImgBottom < winPlacement->rcNormalPosition.bottom)
			nIniImgBottom = winPlacement->rcNormalPosition.bottom;
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	}
	// 设置控件IDC_STATIC_BASEREG的位置大小
	pWnd = GetDlgItem(IDC_STATIC_BASEREG);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.left = (m_rectInitImage.right + m_rectInitImage.left) / 2 - 35;
	winPlacement->rcNormalPosition.right = (m_rectInitImage.right + m_rectInitImage.left) / 2 + 35;
	pWnd->SetWindowPlacement(winPlacement);

	// 设置控件IDC_STATIC_REG的位置大小
	pWnd = GetDlgItem(IDC_STATIC_REG);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.left = (m_rectResltImage.right + m_rectResltImage.left) / 2 - 45;
	winPlacement->rcNormalPosition.right = (m_rectResltImage.right + m_rectResltImage.left) / 2 + 45;
	pWnd->SetWindowPlacement(winPlacement);
	

	// 设置控件IDOK的位置大小
	pWnd = GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);

	// 设置控件IDCANCEL的位置大小
	pWnd = GetDlgItem(IDCANCEL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);

	// 设置控件IDC_REG_OPEN的位置大小
	pWnd = GetDlgItem(IDC_REG_OPEN);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);
	// 设置控件IDC_RADIO1的位置大小
	pWnd = GetDlgItem(IDC_RADIO1);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 40;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 55;
	pWnd->SetWindowPlacement(winPlacement);
	int RA1_right = winPlacement->rcNormalPosition.right;
	//设置IDC_COMBOAUTO的位置大小
	pWnd = GetDlgItem(IDC_COMBOAUTO);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 40;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 55;
	winPlacement->rcNormalPosition.left = RA1_right + 10;
	pWnd->SetWindowPlacement(winPlacement);
	
	// 设置控件IDC_RADIO2的位置大小
	pWnd = GetDlgItem(IDC_RADIO2);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 60;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 75;
	pWnd->SetWindowPlacement(winPlacement);
	// 设置控件IDC_REG_REG的位置大小
	pWnd = GetDlgItem(IDC_REG_REG);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);
	// 设置控件IDC_BUTTONRMS的位置大小
	pWnd = GetDlgItem(IDC_BUTTONRMS);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);
	// 设置控件IDC_COMBORMS的位置大小
	pWnd = GetDlgItem(IDC_COMBORMS);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 40;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 55;
	pWnd->SetWindowPlacement(winPlacement);

	// 设置控件IDC_REG_CHOSE_FEATUR的位置大小
	pWnd = GetDlgItem(IDC_REG_CHOSE_FEATURE);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom + 5;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 30;
	pWnd->SetWindowPlacement(winPlacement);

	// 调整此对话框的大小
	this->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 400;
	winPlacement->rcNormalPosition.left = nIniImgLeft - 20;
	winPlacement->rcNormalPosition.right = nIniImgRight + 50;
	this->SetWindowPlacement(winPlacement);

	// 释放已分配内存
	delete winPlacement;

	// 设置计算图像控件位置标志位为TRUE
	m_bCalImgLoc = TRUE;
}


/*************************************************************************
* \函数名称：
*   DrawFeature()
* \输入参数:无
* \返回值:无
* \说明:
*该函数根据类的成员变量确定特征点的数目和位置，并在图像中进行显示。
**************************************************************************/
void CDlgReg::DrawFeature(CDC* pDC)
{
	// 循环变量
	int i;
	CPoint pointTemp;
	// 半径
	int nRadius;
	nRadius = 5;

	// 设置画图类型
	pDC->SelectStockObject(HOLLOW_BRUSH);

	// 声明画笔
	CPen penWhite(PS_SOLID, 1, RGB(255, 0, 0));
	CPen *pOldPen;

	// 将画笔选入，并保存以前的画笔
	pOldPen = pDC->SelectObject(&penWhite);
//	if(!auto_Feature&&m_bChoseFeature == TRUE)
	if (handle_baseFeaturechosen == FALSE&&m_bChoseFeature == TRUE)
		m_nChsFeatureNum++;
	for (i = 0; i<m_nChsFeatureNum; i++)
	{
		// 首先显示特征点

		// 确定此点的显示位置
		pointTemp.x =floor( m_pPointSampl[i].x / m_pDibSamp->m_lpBMIH->biWidth * IMG_SAMP_W + m_rectResltImage.left+0.5);
		pointTemp.y =floor( m_pPointSampl[i].y  / m_pDibSamp->m_lpBMIH->biHeight* IMG_SAMP_H + m_rectResltImage.top+0.5);

		// 画出此特征点，其中园的半径为nRadius
		CRect rectSamp(pointTemp.x - nRadius, pointTemp.y - nRadius,
			pointTemp.x + nRadius, pointTemp.y + nRadius);
		pDC->Ellipse(rectSamp);

		// 再显示配准特征点
		// 确定此点的显示位置
		if (handle_baseFeaturechosen == TRUE || handle_baseFeaturechosen == FALSE&&i<m_nChsFeatureNum - 1)
		{
			pointTemp.x =floor( m_pPointBase[i].x / m_pDibInit->m_lpBMIH->biWidth * IMG_INIT_W + m_rectInitImage.left+0.5);
			pointTemp.y = floor(m_pPointBase[i].y  / m_pDibInit->m_lpBMIH->biHeight* IMG_INIT_H + m_rectInitImage.top+0.5);
			// 画出此特征点，其中园的半径为nRadius
			CRect rectBase(pointTemp.x - nRadius, pointTemp.y - nRadius,
				pointTemp.x + nRadius, pointTemp.y + nRadius);
			pDC->Ellipse(rectBase);
		}
	}
	if (handle_baseFeaturechosen == FALSE&&m_bChoseFeature == TRUE)
		m_nChsFeatureNum--;
	// 回复以前的画笔
	pDC->SelectObject(pOldPen);
	penWhite.DeleteObject();
}

/*************************************************************************
* \函数名称：
*   OnRegOpen()
* \输入参数:无
* \返回值:无
* \说明:
*该函数打开待配准图像，并将图像存放在m_pDibSamp中。
**************************************************************************/
void CDlgReg::OnRegOpen()
{
	GetDlgItem(IDC_COMBORMS)->EnableWindow(FALSE); //RMS框不可用
	char  szFilter[] = "(*.dat)|*.dat|GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	// TODO: Add your control notification handler code here
	//CFileDialog dlg(TRUE,"bmp","*.bmp");
	CFileDialog dlg(TRUE, "tif", NULL, OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		CFile file;
		CString strPathName;
		strPathName = dlg.GetPathName();

		GDALAllRegister();//注册GDAL库
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  //支持中文路径
		GDALDataset *pDataSet;
		pDataSet = (GDALDataset *)GDALOpen(strPathName, GA_ReadOnly);
		//16位转8位
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

		// 打开文件
		if (!file.Open("myBmp2.bmp", CFile::modeRead | CFile::shareDenyWrite))
		{
			// 返回	
			return;
		}

		// 读入模板图像
		if (!m_pDibSamp->Read(&file))
		{
			// 恢复光标形状
			EndWaitCursor();

			// 清空已分配内存
			m_pDibSamp->Empty();

			// 返回
			return;
		}
	}

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模板配准，其它的可以类推）
	if (m_pDibSamp->m_nColorTableEntries != 256)
	{
		// 提示用户
		

		// 清空已分配内存
		m_pDibSamp->Empty();

		// 返回
		return;
	}

	// 如果打开新的待配准文件，将图像位置设置标志位设为FALSE，以便再次调整位置
	m_bCalImgLoc = FALSE;
	m_pDibSamp->Convert256toGray();
	// 更新显示
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
* \函数名称：
*   OnRegChoseFeatureOnRegReg()
* \输入参数:无
* \返回值:无
* \说明:
*该函数设置选取特征点标志位，然后调用函数在待配准图像中选取特征点，并
*配准这些特征点。特征点的数目至少应该选取三个。
**************************************************************************/
void CDlgReg::OnRegChoseFeature()
{
	// 如果待配准图像尚未打开，则不能进行特征点选取工作
	if ((m_pDibSamp->IsEmpty())) {
		AfxMessageBox("尚未打开待配准图像文件，请打开待配准图像");
		return;
	}
	// 设置选取特征点标志位
	m_bChoseFeature = TRUE;
	if (!auto_Feature)
	{
		//handle_baseFeaturechosen == TRUE;
		AfxMessageBox("请在待配准图像中选取特征点");
	}
		
	if (auto_Feature)//自动
	{
		auto_Featurechosen();
		Invalidate();
	}
}

/*************************************************************************
* \函数名称：
*   OnLButtonUp()
* \输入参数:无
* \返回值:无
* \说明:
*该函数根据鼠标所标定的位置设置特征点。然后调用特征配准函数配准此特征点。
*特征点的数目至少应该选取三个。
**************************************************************************/
void CDlgReg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!auto_Feature)//手动
					  //if(auto_Feature==TRUE&&handle_baseFeaturechosen==TRUE)
					  //	auto_Featurechosen(nFlags,point);
					  //else
		handle_Featurechosen(nFlags, point);
	if(auto_Feature&&m_bChoseFeature)
		auto_Good(nFlags, point);
	
	// 更新显示
	Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}
/*************************************************************************
* \函数名称：
*   auto_Featurechosen()
* \输入参数:无
* \返回值:无
* \说明:半自动选取特征点
**************************************************************************/
void CDlgReg::auto_Featurechosen()
{
	// 如果特征选取标志位为TRUE，则进行特征点的选取和配准，否则退出
	if (!m_bChoseFeature)
		return;
	if (m_comboauto.GetCurSel() == CB_ERR)
	{
		AfxMessageBox("请选择自动匹配算法！");
		return;
	}
		
	cv::Mat image1 = cv::imread("myBmp1.bmp", 0);
	cv::Mat image2 = cv::imread("myBmp2.bmp", 0);

	//关键点
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;
	keypoints1.reserve(100000);
	keypoints2.reserve(100000);
	//描述子
	cv::Mat descriptors1, descriptors2;
	//所有匹配
	vector< vector< cv::DMatch > > matches;
	matches.reserve(100000);
	//筛选的匹配
	vector<cv::DMatch> goodMatches;
	double beta = 4;//最小距离倍数
	if (m_comboauto.GetCurSel() == SURF)
	{
		cv::SurfFeatureDetector surf(3000);
		surf.detect(image1, keypoints1);
		surf.detect(image2, keypoints2);
		//计算描述子
		cv::SurfDescriptorExtractor surfDesc;
		surfDesc.compute(image1, keypoints1, descriptors1);
		surfDesc.compute(image2, keypoints2, descriptors2);
	}
	else if (m_comboauto.GetCurSel() == SIFT)
	{
		//提取特征点    
		cv::SiftFeatureDetector siftDetector(2000);  // 海塞矩阵阈值，在这里调整精度，值越大点越少，越精准 

		siftDetector.detect(image1, keypoints1);
		siftDetector.detect(image2, keypoints2);

		//特征点描述，为下边的特征点匹配做准备    
		cv::SiftDescriptorExtractor SiftDescriptor;
		
		SiftDescriptor.compute(image1, keypoints1, descriptors1);
		SiftDescriptor.compute(image2, keypoints2, descriptors2);

	


	}

	else if (m_comboauto.GetCurSel() == ORB)
	{
		//提取特征点    
		cv::OrbFeatureDetector OrbDetector(1000);  // 在这里调整精度，值越小点越少，越精准 
		
		OrbDetector.detect(image1, keypoints1);
		OrbDetector.detect(image2, keypoints2);

		//特征点描述，为下边的特征点匹配做准备    
		cv::OrbDescriptorExtractor OrbDescriptor;

		OrbDescriptor.compute(image1, keypoints1, descriptors1);
		OrbDescriptor.compute(image2, keypoints2, descriptors2);

		cv::flann::Index flannIndex(descriptors1, cv::flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);


		cv::Mat macthIndex(descriptors2.rows, 2, CV_32SC1), matchDistance(descriptors2.rows, 2, CV_32FC1);
		flannIndex.knnSearch(descriptors2, macthIndex, matchDistance, 2, cv::flann::SearchParams());

		// Lowe's algorithm,获取优秀匹配点
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
	//	//提取特征点    
	//	cv::GoodFeaturesToTrackDetector Detector(500);  //最大点数,值越大，点越多
	//
	//	Detector.detect(image1, keypoints1);
	//	Detector.detect(image2, keypoints2);

	//	//特征点描述，为下边的特征点匹配做准备    
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
	else return; //其他写好再删

	//
	if (m_comboauto.GetCurSel() != ORB)
	{
		//使用FLANN匹配算子进行匹配
		cv::FlannBasedMatcher matcher;

		vector<cv::Mat> train_desc(1, descriptors1);
		matcher.add(train_desc);
		matcher.train();

		matcher.knnMatch(descriptors1, descriptors2, matches, 2);

		// Lowe's algorithm,获取优秀匹配点
		for (unsigned int i = 0; i < matches.size(); i++)
		{
			if (matches[i][0].distance < 0.6*matches[i][1].distance)
			{
				goodMatches.push_back(matches[i][0]);
			}
		}
	}
	
	//计算出关键点之间距离的最大值和最小值
	double max_dist = 0.0; double min_dist = 100.0;//最小距离和最大距离
	for (unsigned j = 0; j < goodMatches.size(); j++)
	{
		double dist = goodMatches[j].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	//存下匹配距离小于beta*min_dist的点对

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


	//定义两个局部变量
	vector<cv::Point2f> imagePointsBase;
	vector<cv::Point2f> imagePointsReg;

	//从匹配成功的匹配对中获取关键点
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
* \函数名称：
*   handle_Featurechosen()
* \输入参数:
* \返回值:无
* \说明:手动选择特征点
**************************************************************************/
void CDlgReg::handle_Featurechosen(UINT nFlags, CPoint point)
{
	// 循环变量
	int i, j;
	// 如果特征选取标志位为TRUE，则进行特征点的选取和配准，否则退出
	if (!m_bChoseFeature)
		return;

	// 待配准图像的特征选取区域，在这里选择特征点的选择区域要比图像的区
	// 域小一圈
	CRect rectChoose1;
	rectChoose1.bottom = m_rectResltImage.bottom - 5;
	rectChoose1.top = m_rectResltImage.top + 5;
	rectChoose1.left = m_rectResltImage.left + 5;
	rectChoose1.right = m_rectResltImage.right - 5;
	// 基准图像的特征选取区域，在这里选择特征点的选择区域要比图像的区
	// 域小一圈
	CRect rectChoose2;
	rectChoose2.bottom = m_rectInitImage.bottom - 5;
	rectChoose2.top = m_rectInitImage.top + 5;
	rectChoose2.left = m_rectInitImage.left + 5;
	rectChoose2.right = m_rectInitImage.right - 5;
	//基准图区域
	CRect rectChoose3;
	rectChoose3.bottom = m_rectInitImage.bottom;
	rectChoose3.top = m_rectInitImage.top;
	rectChoose3.left = m_rectInitImage.left;
	rectChoose3.right = m_rectInitImage.right;
	//待配准图区域
	CRect rectChoose4;
	rectChoose4.bottom = m_rectResltImage.bottom;
	rectChoose4.top = m_rectResltImage.top;
	rectChoose4.left = m_rectResltImage.left;
	rectChoose4.right = m_rectResltImage.right;
	// 特征点的区域
	CRect rectFeature1;
	CRect rectFeature2;

	// 标志位，表示此点是否是已经选择的特征点
	BOOL bFlag1 = FALSE;
	BOOL bFlag2 = FALSE;

	if (handle_baseFeaturechosen == TRUE)
	{
		// 判断此点是否合法，并判断此点是否已经选择，如果是，则去掉此点,待配准区
		if (rectChoose1.PtInRect(point))
		{
			// 如果所选择的特征点是以前的特征点，则去掉此点
			for (i = 0; i<m_nChsFeatureNum; i++)
			{
				// 选择特征点的显示区域，以便对特征点进行取舍,真实点在对话框坐标(比例缩小)
				rectFeature1.bottom = floor(m_pPointSampl[i].y * IMG_SAMP_H / m_pDibSamp->m_lpBMIH->biHeight + 0.5) + m_rectResltImage.top + 5;
				rectFeature1.top = floor(m_pPointSampl[i].y* IMG_SAMP_H / m_pDibSamp->m_lpBMIH->biHeight + 0.5) + m_rectResltImage.top - 5;
				rectFeature1.left = floor(m_pPointSampl[i].x* IMG_SAMP_W / m_pDibSamp->m_lpBMIH->biWidth+0.5) + m_rectResltImage.left - 5;
				rectFeature1.right =floor( m_pPointSampl[i].x * IMG_SAMP_W / m_pDibSamp->m_lpBMIH->biWidth+0.5) + m_rectResltImage.left + 5;

				// 判断所选择的特征点是否为原来选择的特征点
				// 如果是，则去掉此特征点
				if (rectFeature1.PtInRect(point))
				{
					// 将后面的特征点向前移动一位，去掉所选择的此特征点
					for (j = i; j<m_nChsFeatureNum - 1; j++)
					{
						m_pPointSampl[j] = m_pPointSampl[j + 1];
						m_pPointBase[j] = m_pPointBase[j + 1];
					}

					// 将特征点的计数减一
					m_nChsFeatureNum--;
					// 更新显示
					Invalidate();
					// 设置标志位
					bFlag1 = TRUE;
					return;
				}
			}

			// 在判断特征点是否已经选取够了
			if (m_nChsFeatureNum >= 40)
			{
				AfxMessageBox("你已经选取了40个特征点，如果要继续选取，你可以去掉配准不正确的特征点再进行选取");
				return;
			}

			// 如果此点是需要选取的，则进行相关操作
			if (!bFlag1)
			{
				// 将此待配准特征点选取，注意特征点的坐标是以图像的左上角为原点确定的,比例放大
				m_pPointSampl[m_nChsFeatureNum].x =  double(point.x - m_rectResltImage.left)*m_pDibSamp->m_lpBMIH->biWidth/ IMG_SAMP_W;
				m_pPointSampl[m_nChsFeatureNum].y = double(point.y  - m_rectResltImage.top) * m_pDibSamp->m_lpBMIH->biHeight/IMG_SAMP_H ;
				handle_baseFeaturechosen = FALSE;
				this->Invalidate();
				AfxMessageBox("请在左侧基准图像中选取对应特征点");

			}
		}
	}
	else
	{
		// 判断此点是否合法，并判断此点是否已经选择，如果是，则去掉此点
		if (rectChoose3.PtInRect(point))
		{
			if (!rectChoose2.PtInRect(point))
			{
				AfxMessageBox("请在图像5像素以内选取特征点");
				handle_baseFeaturechosen = TRUE;
				return;
			}
			// 如果所选择的特征点是以前的特征点，则去掉此点
			for (i = 0; i<m_nChsFeatureNum; i++)
			{
				// 选择特征点的显示区域，以便对特征点进行取舍
				rectFeature2.bottom = floor(m_pPointBase[i].y * IMG_INIT_H /m_pDibInit->m_lpBMIH->biHeight +0.5)+ m_rectInitImage.top + 5;
				rectFeature2.top =floor( m_pPointBase[i].y * IMG_INIT_H / m_pDibInit->m_lpBMIH->biHeight +0.5)+ m_rectInitImage.top - 5;
				rectFeature2.left =floor( m_pPointBase[i].x * IMG_INIT_W / m_pDibInit->m_lpBMIH->biWidth +0.5)+ m_rectInitImage.left - 5;
				rectFeature2.right =floor( m_pPointBase[i].x * IMG_INIT_W / m_pDibInit->m_lpBMIH->biWidth+0.5) + m_rectInitImage.left + 5;

				// 判断所选择的特征点是否为原来选择的特征点
				// 如果是，则去掉此特征点
				if (rectFeature2.PtInRect(point))
				{
					// 将后面的特征点向前移动一位，去掉所选择的此特征点
					for (j = i; j<m_nChsFeatureNum - 1; j++)
					{
						m_pPointSampl[j] = m_pPointSampl[j + 1];
						m_pPointBase[j] = m_pPointBase[j + 1];
					}

					// 将特征点的计数减一
					m_nChsFeatureNum--;

					// 更新显示
					Invalidate();

					// 设置标志位
					bFlag2 = TRUE;

					return;
				}
			}

			// 如果此点是需要选取的，则进行相关操作
			if (!bFlag2)
			{
				// 将此待配准特征点选取，注意特征点的坐标是以图像的左上角为原点确定的
				m_pPointBase[m_nChsFeatureNum].x = double(point.x - m_rectInitImage.left) / IMG_INIT_W *m_pDibInit->m_lpBMIH->biWidth;
				m_pPointBase[m_nChsFeatureNum].y =double( point.y - m_rectInitImage.top) / IMG_INIT_H *m_pDibInit->m_lpBMIH->biHeight;
				// 将特征点计数加一
				m_nChsFeatureNum++;
				handle_baseFeaturechosen = TRUE;
			}
		}
	}
}
/*************************************************************************
* \函数名称：
*   auto_Good()
* \输入参数:
* \返回值:无
* \说明:自动选择特征点时筛选特征点
**************************************************************************/
void CDlgReg::auto_Good(UINT nFlags, CPoint point)
{
	// 循环变量
	int i, j;
	// 如果特征选取标志位为TRUE，则进行特征点的选取和配准，否则退出
	if (!m_bChoseFeature)
		return;

	// 待配准图像的特征选取区域，在这里选择特征点的选择区域要比图像的区
	// 域小一圈
	CRect rectChoose1;
	rectChoose1.bottom = m_rectResltImage.bottom - 5;
	rectChoose1.top = m_rectResltImage.top + 5;
	rectChoose1.left = m_rectResltImage.left + 5;
	rectChoose1.right = m_rectResltImage.right - 5;
	// 基准图像的特征选取区域，在这里选择特征点的选择区域要比图像的区
	// 域小一圈
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
	// 特征点的区域
	CRect rectFeature1;
	CRect rectFeature2;

	// 标志位，表示此点是否是已经选择的特征点
	BOOL bFlag1 = FALSE;
	BOOL bFlag2 = FALSE;


		// 判断此点是否合法，并判断此点是否已经选择，如果是，则去掉此点
		if (rectChoose3.PtInRect(point))
		{
			
			// 如果所选择的特征点是以前的特征点，则去掉此点
			for (i = 0; i<m_nChsFeatureNum; i++)
			{
				// 选择特征点的显示区域，以便对特征点进行取舍
				rectFeature2.bottom =floor( m_pPointBase[i].y * IMG_INIT_H / m_pDibInit->m_lpBMIH->biHeight +0.5)+ m_rectInitImage.top + 5;
				rectFeature2.top =floor( m_pPointBase[i].y * IMG_INIT_H / m_pDibInit->m_lpBMIH->biHeight +0.5)+ m_rectInitImage.top - 5;
				rectFeature2.left =floor( m_pPointBase[i].x * IMG_INIT_W / m_pDibInit->m_lpBMIH->biWidth+0.5) + m_rectInitImage.left - 5;
				rectFeature2.right =floor( m_pPointBase[i].x * IMG_INIT_W / m_pDibInit->m_lpBMIH->biWidth +0.5)+ m_rectInitImage.left + 5;

				// 判断所选择的特征点是否为原来选择的特征点
				// 如果是，则去掉此特征点
				if (rectFeature2.PtInRect(point))
				{
					// 将后面的特征点向前移动一位，去掉所选择的此特征点
					for (j = i; j<m_nChsFeatureNum - 1; j++)
					{
						m_pPointSampl[j] = m_pPointSampl[j + 1];
						m_pPointBase[j] = m_pPointBase[j + 1];
					}

					// 将特征点的计数减一
					m_nChsFeatureNum--;

					// 更新显示
					Invalidate();

					// 设置标志位
					bFlag2 = TRUE;

					return;
				}
			}


		}
		// 判断此点是否合法，并判断此点是否已经选择，如果是，则去掉此点,待配准区
		if (rectChoose1.PtInRect(point))
		{
			// 如果所选择的特征点是以前的特征点，则去掉此点
			for (i = 0; i<m_nChsFeatureNum; i++)
			{
				// 选择特征点的显示区域，以便对特征点进行取舍,真实点在对话框坐标(比例缩小)
				rectFeature1.bottom = floor(m_pPointSampl[i].y * IMG_SAMP_H / m_pDibSamp->m_lpBMIH->biHeight +0.5)+ m_rectResltImage.top + 5;
				rectFeature1.top = floor(m_pPointSampl[i].y* IMG_SAMP_H / m_pDibSamp->m_lpBMIH->biHeight +0.5)+ m_rectResltImage.top - 5;
				rectFeature1.left =floor( m_pPointSampl[i].x* IMG_SAMP_W / m_pDibSamp->m_lpBMIH->biWidth +0.5)+ m_rectResltImage.left - 5;
				rectFeature1.right =floor( m_pPointSampl[i].x * IMG_SAMP_W / m_pDibSamp->m_lpBMIH->biWidth +0.5)+ m_rectResltImage.left + 5;

				// 判断所选择的特征点是否为原来选择的特征点
				// 如果是，则去掉此特征点
				if (rectFeature1.PtInRect(point))
				{
					// 将后面的特征点向前移动一位，去掉所选择的此特征点
					for (j = i; j<m_nChsFeatureNum - 1; j++)
					{
						m_pPointSampl[j] = m_pPointSampl[j + 1];
						m_pPointBase[j] = m_pPointBase[j + 1];
					}

					// 将特征点的计数减一
					m_nChsFeatureNum--;
					// 更新显示
					Invalidate();
					// 设置标志位
					bFlag1 = TRUE;
					return;
				}
			}

		}
	
}
/*************************************************************************
* \函数名称：
*   OnLButtonMove()
* \输入参数:无
* \返回值:无
* \说明:
*如果特征点选取标志位为TRUE，则该函数将待配准区域的鼠标设置为十字形状，
*以便能更精确的定位特征点。
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
	// 如果不是在特征点选取状态，则不进行相关的操作
	if (m_bChoseFeature)
	{
		// 如果鼠标在待配准图像区域中，则更改鼠标形状
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
* \函数名称：
*   FindMatchPoint()
* \输入参数:
*   CDib*	pDibBase	- 基准图像指针
*   CDib*	pDibSamp	- 待配准图像指针
*   CPoint	pointSamp	- 待配准特征点的位置
* \返回值:
*   CPoint			- 返回在基准图像中配准的特征点的位置
* \说明:
*该函数根据待待配准图像中的特征点位置在基准图像中寻找配准特征点，并将
*配准的特征点位置返回。在配准的过程中，采取的是块配准的方法进行配准，在这里
*选取块的大小为7*7。搜索的方法为全局搜索。
**************************************************************************/
CPoint CDlgReg::FindMatchPoint(CDib* pDibBase, CDib* pDibSamp,
	CPoint pointSamp)
{
	// 循环变量
	int i, j, m, n;

	// 临时变量
	int nX, nY;

	// 配准特征点位置
	CPoint pointBase;

	// 配准数据块的尺寸
	int nBlockLen = 7;
	int nBlockHalfLen = 3;

	// 基准图像数据指针	
	unsigned char* pBase;
	pBase = (unsigned char *)pDibBase->m_lpImage;

	// 待配准图像数据指针
	unsigned char* pSamp;
	pSamp = (unsigned char *)pDibSamp->m_lpImage;

	// 特征点位置的数据配准块
	unsigned char* pUnchSampBlock;
	pUnchSampBlock = new unsigned char[nBlockLen*nBlockLen];

	// 临时分配内存，用于存放配准数据块
	unsigned char* pUnchBaseBlock;
	pUnchBaseBlock = new unsigned char[nBlockLen*nBlockLen];

	// 相似度
	double dbCor;

	// 最大相似度
	double dbMaxCor = 0;

	// 基准图像的存储大小
	CSize sizeBaseImg;
	sizeBaseImg = pDibBase->GetDibSaveDim();

	// 待配准图像的存储大小
	CSize sizeSampImg;
	sizeSampImg = pDibSamp->GetDibSaveDim();

	// 从待配准图像中提取以特征点为中心的nBlockLen*nBlockLen0的数据块
	for (i = -nBlockHalfLen; i <= nBlockHalfLen; i++)
	{
		for (j = -nBlockHalfLen; j <= nBlockHalfLen; j++)
		{
			// 计算此点在图像中的位置
			nX = pointSamp.x + i;
			nY = sizeSampImg.cy - pointSamp.y + j + 1;

			// 提取图像数据
			pUnchSampBlock[(j + nBlockHalfLen)*nBlockLen + (i + nBlockHalfLen)] =
				pSamp[nY*sizeSampImg.cx + nX];
		}
	}

	// 基准图像的高度和宽度
	int nBaseImgHeight, nBaseImgWidth;
	nBaseImgHeight = pDibBase->m_lpBMIH->biHeight;
	nBaseImgWidth = pDibBase->m_lpBMIH->biWidth;

	// 在基准图像中寻找配准特征点，采取的搜索方法为全局搜索
	for (m = nBlockHalfLen; m< nBaseImgHeight - nBlockHalfLen; m++)
	{
		for (n = nBlockHalfLen; n<nBaseImgWidth - nBlockHalfLen; n++)
		{
			// 提取以此点为中心，大小为nBlockLen*nBlockLen的数据块
			for (i = -nBlockHalfLen; i <= nBlockHalfLen; i++)
			{
				for (j = -nBlockHalfLen; j <= nBlockHalfLen; j++)
				{
					// 计算此点在图像中存储的位置
					nX = n + i;
					nY = sizeBaseImg.cy - m + j + 1;

					// 提取图像数据
					pUnchBaseBlock[(j + nBlockHalfLen)*nBlockLen + (i + nBlockHalfLen)] =
						pBase[nY*sizeBaseImg.cx + nX];
				}
			}


			// 对这两个数据块进行配准，计算相似度
			dbCor = CalCorrelation(pUnchBaseBlock, pUnchSampBlock, nBlockLen);

			// 判断是否为最大相似度，如果是，则记录此相似度和配准特征点位置
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
* \函数名称：
*   CalCorrelation()
* \输入参数:
*   unsigned char*	pBase		- 基准图像数据指针
*   unsigned char*	pSamp		- 待配准图像数据指针
*   int		nBlockLen	- 配准数据块的尺度大小
* \返回值:
*   double			- 返回两个数据块配准的相似度
* \说明:
*该函数对给定的两个大小为nBlockLen*nBlockLen的数据块，计算两者之间的
*的配准相似度。其中，去掉均值以消除亮度变换的影响。
**************************************************************************/
double CDlgReg::CalCorrelation(unsigned char* pBase, unsigned char* pSamp, int nBlockLen)
{
	// 临时变量
	double dbSelfBase = 0, dbSelfSamp = 0;

	// 相似度
	double dbCor = 0;

	// 块均值
	double dbMeanBase = 0, dbMeanSamp = 0;

	// 计算两个块的平均值
	for (int i = 0;i<nBlockLen;i++)
		for (int j = 0;j<nBlockLen;j++)
		{
			dbMeanBase += pBase[j*nBlockLen + i];
			dbMeanSamp += pSamp[j*nBlockLen + i];
		}
	dbMeanBase = dbMeanBase / (nBlockLen*nBlockLen);
	dbMeanSamp = dbMeanSamp / (nBlockLen*nBlockLen);

	// 求取相似度
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

	// 返回相似度
	return dbCor;
}

/*************************************************************************
* \函数名称：
*   OnRegReg()
* \输入参数:无
* \返回值:无
* \说明:
*该函数对两幅图像m_pDibInit和m_pDibSamp进行配准。其中，需要先确定特征点，
*特征点至少需要选取3个以上，这一步骤需要在“选取特征点”步骤中进行。根据选取
*的特征点，计算得到空间参数。最后，将配准的两幅图像拼接起来显示。
***************************************************************************/
void CDlgReg::OnRegReg()
{
	//if (auto_Feature)//自动
	//{
	//	auto_Featurechosen();
	//	m_bChoseFeature = TRUE;
	//}
	// 进行配准之前，判断是否已经选取特征点
	if (!m_bChoseFeature)  //主要手动才判断
	{
		AfxMessageBox("还没有选取特征点，请先选取特征点");
		return;
	}

	// 如果选取的特征点数目不够，也不能进行处理
	if (m_nChsFeatureNum < 3)
	{
		AfxMessageBox("请选择至少3个特征点，再进行图像配准");
		return;
	}

	// 变换系数为6个。此系数为从基准图像到待配准图像的变换系数
	double *pDbBs2SpAffPara;
	pDbBs2SpAffPara = new double[2 * 3];

	// 变换系数为6个。此系数为从待配准图像到基准图像的变换系数
	double *pDbSp2BsAffPara;
	pDbSp2BsAffPara = new double[2 * 3];



	// 如果已经选取了特征点，并得到了配准的特征点，则可以计算空间变换系数了

		std::vector<cv::Point2f> imagePointsBase(m_nChsFeatureNum), imagePointsReg(m_nChsFeatureNum);


		for (int i = 0;i<m_nChsFeatureNum;i++)
		{
			imagePointsBase[i].x= m_pPointBase[i].x;
			imagePointsReg[i].x = m_pPointSampl[i].x;
			imagePointsBase[i].y = m_pPointBase[i].y;
			imagePointsReg[i].y = m_pPointSampl[i].y;
		}
		//得到仿射矩阵
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


		//计算RMS
		GetDlgItem(IDC_COMBORMS)->EnableWindow(TRUE);
		RMS_compute();
		


	// 给矩阵赋值
	for(int r = 0;  r < 2; r++)
		for (int c = 0;c < 3;c++)
		{
			pDbBs2SpAffPara[r * 3 + c] = homo.at<double>(r, c);
			pDbSp2BsAffPara[r * 3 + c] = homoR.at<double>(r, c);
		}




	// 计算图像经过空间变换后的尺寸大小
	CRect rectAftAff;
	rectAftAff = GetAftAffDim(pDbSp2BsAffPara);

	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth = rectAftAff.right - rectAftAff.left;
	nNewImgHeight = rectAftAff.bottom - rectAftAff.top;

	// 将基准图像放入新的图像中
	LPBYTE lpBaseImg;
	lpBaseImg = SetBaseImgAftAff(rectAftAff);

	// 将待配准图像放入新的图像中
	LPBYTE lpSampImg;
	lpSampImg = SetSampImgAftAff(pDbBs2SpAffPara, rectAftAff);

	// 将此图像用CDib类封装
	m_pDibResult = new CDib(CSize(nNewImgWidth, nNewImgHeight), 8);

	// 计算结果图像的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();

	// 分配内存给合并后的图像
	LPBYTE lpImgResult;
	lpImgResult = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	// 对图像进行赋值
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

	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpImgResult;

	// 显示合并后的图像
	CDlgAftReg* pDlg;
	pDlg = new CDlgAftReg(NULL, m_pDibResult);
	pDlg->DoModal();

	// 删除对象
	delete pDlg;

	// 释放已分配内存
	delete[]lpBaseImg;
	delete[]lpSampImg;
	delete[]pDbBs2SpAffPara;
	delete[]pDbSp2BsAffPara;
}


/*************************************************************************
* \函数名称：
*   GetAffinePara()
* \输入参数:
*   double  *pDbBs2SpAffPara	- 用于存放基准图像到待配准图像的空间变换系数
*   double  *pDbSp2BsAffPara	- 用于存放待配准图像到基准图像的空间变换系数
* \返回值:无
* \说明:
*该函数根据得到的三对配准的特征点，计算空间变换系数。得到的空间变换系数
*存放在两个输入参数所指向的内存中。
**************************************************************************/
void CDlgReg::GetAffinePara(MyPoint* pPointBase, MyPoint* pPointSampl, double* pDbAffPara)
{
	// pDbBMatrix中存放的是基准图像中特征点的坐标，
	// 大小为2*m_nChsFeatureNum，前m_nChsFeatureNum为X坐标
	double *pDbBMatrix;
	pDbBMatrix = new double[2 * m_nChsFeatureNum];

	// pDbSMatrix中存放的是待配准图像中特征点的扩展坐标
	// 大小为3*m_nChsFeatureNum，其中前m_nChsFeatureNum为X坐标
	// 中间m_nChsFeatureNum个为Y坐标，后面m_nChsFeatureNum为1
	double *pDbSMatrix;
	pDbSMatrix = new double[3 * m_nChsFeatureNum];

	// pDbSMatrixT中存放的pDbSMatrix的转置矩阵，
	// 大小为m_nChsFeatureNum*3
	double *pDbSMatrixT;
	pDbSMatrixT = new double[m_nChsFeatureNum * 3];

	// pDbInvMatrix为临时变量，存放的是pDbSMatrix*pDbSMatrixT的逆
	// 大小为3*3
	double *pDbInvMatrix;
	pDbInvMatrix = new double[3 * 3];

	// 临时内存
	double *pDbTemp;
	pDbTemp = new double[2 * 3];

	// 循环变量
	int count;

	// 给矩阵赋值
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

	// 计算pDbSMatrix*pDbSMatrixT，并将结果放入pDbInvMatrix中
	CalMatProduct(pDbSMatrix, pDbSMatrixT, pDbInvMatrix, 3, 3, m_nChsFeatureNum);

	// 计算pDbInvMatrix的逆矩阵
	CalInvMatrix(pDbInvMatrix, 3);

	// 计算空间变换系数
	CalMatProduct(pDbBMatrix, pDbSMatrixT, pDbTemp, 2, 3, m_nChsFeatureNum);
	CalMatProduct(pDbTemp, pDbInvMatrix, pDbAffPara, 2, 3, 3);

	// 释放内存
	delete[]pDbBMatrix;
	delete[]pDbSMatrix;
	delete[]pDbSMatrixT;
	delete[]pDbInvMatrix;
	delete[]pDbTemp;

}

/*************************************************************************
* \函数名称：
*   CalMatProduct()
* \输入参数:
*   double  *pDbSrc1	- 指向相乘矩阵1的内存
*   double  *pDbSrc2	- 指向相乘矩阵2的内存
*   double  *pDbDest   - 存放矩阵相乘运行结果的内存指针
*   int     nX		- 矩阵的尺寸，具体参见函数说明
*   int     nY		- 矩阵的尺寸，具体参见函数说明
*   int     nZ		- 矩阵的尺寸，具体参见函数说明
* \返回值:无
* \说明:
*该函数计算两个矩阵的相乘，然后将相乘的结果存放在pDbDest中。其中pDbSrc1
*的大小为nX*nZ，pDbSrc2的大小为nZ*nY，pDbDest的大小为nX*nY
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
* \函数名称：
*   CalInvMatrix()
* \输入参数:
*   double  *pDbSrc	- 指向矩阵的指针
*   int     nLen	- 矩阵的尺寸
* \返回值:无
* \说明:
*该函数计算矩阵pDbSrc的逆矩阵，其中pDbSrc的大小为nLen*nLen
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
* \函数名称：
*  GetAftAffDim()
* \输入参数:
*   double  *pDbAffPara	- 空间变换系数矩阵
* \返回值:
*   CRect			- 返回待配准图像经空间变换后的区域
* \说明:
*   该函数根据空间变换系数，计算待配准图像空间变换后的图像尺寸大小
***************************************************************************/
CRect CDlgReg::GetAftAffDim(double* pDbAffPara)
{
	// 基准图像的宽度和高度
	int nBaseImgWidth, nBaseImgHeight;
	nBaseImgWidth = m_pDibInit->m_lpBMIH->biWidth;
	nBaseImgHeight = m_pDibInit->m_lpBMIH->biHeight;

	// 待配准图像的宽度和高度
	int nSamplImgWidth, nSamplImgHeight;
	nSamplImgWidth = m_pDibSamp->m_lpBMIH->biWidth;
	nSamplImgHeight = m_pDibSamp->m_lpBMIH->biHeight;

	// 基准图像的原始区域
	CRect rectBase(0, 0, nBaseImgWidth, nBaseImgHeight);

	// 临时变量
	CPoint pointTemp;
	double tx, ty;

	// 图像的端点
	pointTemp.x = 0;
	pointTemp.y = 0;

	// 计算点pointTemp经过空间变换后的坐标
	tx = pDbAffPara[0 * 3 + 0] * pointTemp.x +
		pDbAffPara[0 * 3 + 1] * pointTemp.y + pDbAffPara[0 * 3 + 2];
	ty = pDbAffPara[1 * 3 + 0] * pointTemp.x +
		pDbAffPara[1 * 3 + 1] * pointTemp.y + pDbAffPara[1 * 3 + 2];

	// 判断pointTemp经过空间变换后是否超出原来的大小
	if (tx<rectBase.left)
		rectBase.left = (int)tx;
	if (tx>rectBase.right)
		rectBase.right = (int)tx + 1;
	if (ty<rectBase.top)
		rectBase.top = (int)ty;
	if (ty>rectBase.bottom)
		rectBase.bottom = (int)ty + 1;

	// 计算端点(0, nSamplImgHeight)变换后的坐标
	pointTemp.x = 0; pointTemp.y = nSamplImgHeight;
	tx = pDbAffPara[0 * 3 + 0] * pointTemp.x +
		pDbAffPara[0 * 3 + 1] * pointTemp.y + pDbAffPara[0 * 3 + 2];
	ty = pDbAffPara[1 * 3 + 0] * pointTemp.x +
		pDbAffPara[1 * 3 + 1] * pointTemp.y + pDbAffPara[1 * 3 + 2];

	// 判断是否越界
	if (tx<rectBase.left)
		rectBase.left = (int)tx;
	if (tx>rectBase.right)
		rectBase.right = (int)tx + 1;
	if (ty<rectBase.top)
		rectBase.top = (int)ty;
	if (ty>rectBase.bottom)
		rectBase.bottom = (int)ty + 1;

	// 计算端点(nSamplImgWidth, nSamplImgHeight)变换后的坐标
	pointTemp.x = nSamplImgWidth; pointTemp.y = nSamplImgHeight;
	tx = pDbAffPara[0 * 3 + 0] * pointTemp.x +
		pDbAffPara[0 * 3 + 1] * pointTemp.y + pDbAffPara[0 * 3 + 2];
	ty = pDbAffPara[1 * 3 + 0] * pointTemp.x +
		pDbAffPara[1 * 3 + 1] * pointTemp.y + pDbAffPara[1 * 3 + 2];

	// 判断是否越界
	if (tx<rectBase.left)
		rectBase.left = (int)tx;
	if (tx>rectBase.right)
		rectBase.right = (int)tx + 1;
	if (ty<rectBase.top)
		rectBase.top = (int)ty;
	if (ty>rectBase.bottom)
		rectBase.bottom = (int)ty + 1;

	// 计算端点(nSamplImgWidth, 0)变换后的坐标
	pointTemp.x = nSamplImgWidth; pointTemp.y = 0;
	tx = pDbAffPara[0 * 3 + 0] * pointTemp.x +
		pDbAffPara[0 * 3 + 1] * pointTemp.y + pDbAffPara[0 * 3 + 2];
	ty = pDbAffPara[1 * 3 + 0] * pointTemp.x +
		pDbAffPara[1 * 3 + 1] * pointTemp.y + pDbAffPara[1 * 3 + 2];

	// 判断是否越界
	if (tx<rectBase.left)
		rectBase.left = (int)tx;
	if (tx>rectBase.right)
		rectBase.right = (int)tx + 1;
	if (ty<rectBase.top)
		rectBase.top = (int)ty;
	if (ty>rectBase.bottom)
		rectBase.bottom = (int)ty + 1;

	// 返回待配准图像变换后的区域大小
	return(rectBase);
}

/*************************************************************************
* \函数名称：
*   SetSampImgAftAff()
* \输入参数:
*   double  *pDbAffPara	- 空间变换系数矩阵
*   CRect   rectNewImg		- 变换后图像的大小尺寸
* \返回值:
*   LPBYTE			- 返回变换后的图像
* \说明:
*该函数根据空间变换系数，计算待配准图像空间变换后的图像。并返回此图像指针
*此图像的大小为rectNewImg
**************************************************************************/
LPBYTE CDlgReg::SetSampImgAftAff(double* pDbAffPara, CRect rectNewImg)
{
	// pUnchSect是4*4大小的矩阵数组
	unsigned char *pUnchSect;
	pUnchSect = new unsigned char[4 * 4];

	// 新的图像宽度和高度
	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth = rectNewImg.right - rectNewImg.left;
	nNewImgHeight = rectNewImg.bottom - rectNewImg.top;

	// 待配准图像的宽度和高度
	int nSamplImgWidth, nSamplImgHeight;
	nSamplImgWidth = m_pDibSamp->m_lpBMIH->biWidth;
	nSamplImgHeight = m_pDibSamp->m_lpBMIH->biHeight;

	// 待配准图像的存储宽度
	int nSampSaveWidth;
	nSampSaveWidth = m_pDibSamp->GetDibSaveDim().cx;

	// pUnchAftAffSamp是一个大小为rectNewImg大小的图像，
	// 其中rectNewImg表示变换后的图像大小
	unsigned char *pUnchAftAffSamp;
	pUnchAftAffSamp = new unsigned char[nNewImgWidth * nNewImgHeight];

	double tx, ty;

	// 计算在变换后的图像的数据
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

			// 确定变换的坐标
			ty = ty - (int)ty + 1;
			tx = tx - (int)tx + 1;

			// 确定变换后此坐标的数值
			pUnchAftAffSamp[i*nNewImgWidth + j] = CalSpline(pUnchSect, tx, ty);
		}
	// 释放内存
	delete[]pUnchSect;
	// 返回指针
	return (LPBYTE)pUnchAftAffSamp;
}

/*************************************************************************
* \函数名称：
*   CalSpline()
* \输入参数:
*   unsigned char	*pUnchCorr	- 插值的点
*   double		dX		- X坐标
*   double		dY		- Y坐标
* \返回值:
*   unsigned char			- 插值后的值
* \说明:
*该函数根据邻近位置的数值进行插值。此图像的大小为rectNewImg
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
* \函数名称：
*   SetBaseImgAftAff()
* \输入参数:
*   double  *pDbAffPara	- 空间变换系数矩阵
* \返回值:无
* \说明:
*该函数根据空间变换系数，计算基准图像空间变换后的图像，并返回存放此
*数据的指针
**************************************************************************/
LPBYTE CDlgReg::SetBaseImgAftAff(CRect rectNewImg)
{
	// 新图像的大小
	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth = rectNewImg.right - rectNewImg.left;
	nNewImgHeight = rectNewImg.bottom - rectNewImg.top;

	// 变换后图像
	unsigned char *pUnchAftAffBase;
	pUnchAftAffBase = new unsigned char[nNewImgWidth*nNewImgHeight];

	// 基准图像的高度和宽度
	int nBaseWidth, nBaseHeight;
	nBaseWidth = m_pDibInit->m_lpBMIH->biWidth;
	nBaseHeight = m_pDibInit->m_lpBMIH->biHeight;

	// 基准图像的存储宽度
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

	// 返回
	return (LPBYTE)pUnchAftAffBase;
}
void CDlgReg::RMS_compute()
{
	//计算RMS
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
		//显示
		
		
		t.Format("point %d: %f", i + 1,RMS[i]);
		m_rms_show.AddString(t);
		
		
	}
	UpdateData(FALSE);
	
	
}

void CDlgReg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (auto_Feature == FALSE&&handle_baseFeaturechosen == FALSE)
	//	AfxMessageBox("请先在基准图像中选取此特征点的对应点，然后再切换至“自动”模式");
	GetDlgItem(IDC_COMBOAUTO)->EnableWindow(TRUE);
	auto_Feature = TRUE;
	
}

void CDlgReg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_COMBOAUTO)->EnableWindow(FALSE);
	auto_Feature = FALSE;
	
}

void CDlgReg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CDlgReg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CDlgReg::OnDropdownComboauto()
{

	// TODO: 在此添加控件通知处理程序代码


	
}




void CDlgReg::OnFeatureRms()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_rms_show.GetCurSel();
	if (nIndex == CB_ERR)
	{
		AfxMessageBox("请选择要去除的点！");
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MOUSE_R = !MOUSE_R;


	CDialog::OnRButtonUp(nFlags, point);
}
