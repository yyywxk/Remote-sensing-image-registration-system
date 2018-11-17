// RSIDoc.cpp : implementation of the CRSIDoc class
//

#include "stdafx.h"
#include "ң��ͼ����׼ϵͳ.h"

#include "RSIDoc.h"
#include "GlobalApi.h"


#include "MainFrm.h"
#include "ChildFrm.h"
#include "RSIView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRSIDoc

IMPLEMENT_DYNCREATE(CRSIDoc, CDocument)

BEGIN_MESSAGE_MAP(CRSIDoc, CDocument)
	//{{AFX_MSG_MAP(CRSIDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_RELOAD, OnFileReload)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRSIDoc construction/destruction

CRSIDoc::CRSIDoc()
{
	// TODO: add one-time construction code here
	InitDocVariable();
}

CRSIDoc::~CRSIDoc()
{
	FreeDocVariable();
}

BOOL CRSIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRSIDoc serialization

void CRSIDoc::Serialize(CArchive& ar)
{
	//m_pDibInit->Serialize(ar);
	//m_DIBt.Serialize(ar);
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRSIDoc diagnostics

#ifdef _DEBUG
void CRSIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRSIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRSIDoc commands

int CRSIDoc::InitDocVariable()
{
	m_pDibInit = new CDib;
	//m_pDibtInit = new CDibt;
	// added by tanhc in 2002-7-30, used to test some function
	m_pDibTest = new CDib;
	//m_pDibtTest = new CDibt;

	m_nColorIndex = 0;

	//--------------------------------------------

	return 0;
}

int CRSIDoc::FreeDocVariable()
{
	delete m_pDibInit;
	//delete m_pDibtInit;
	m_pDibInit = NULL;
	//m_pDibtInit = NULL;

	// added by tanhc in 2002-7-30, used to test some function
	delete m_pDibTest ;
	//delete m_pDibtTest;
	m_pDibTest = NULL ;
	//m_pDibtTest = NULL;

	//--------------------------------------------

	return 0;
}

void CRSIDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	CString strSaveFileType = "λͼ�ļ� (*.bmp;*.dib)|*.bmp; *.dib|All Files (*.*)|*.*||";
	CFileDialog FileDlg(FALSE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strSaveFileType);

	CFile fileOpen ;
	if( FileDlg.DoModal() == IDOK ) {
		if(!fileOpen.Open( FileDlg.GetPathName() , CFile::modeCreate|CFile::modeWrite )){
			AfxMessageBox("cannot create the file to save");
			return;
		}
		if( !m_pDibInit->Write( &fileOpen ) ){
			return;
		}
		fileOpen.Close();
	}
	
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnFileSaveAs_result()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �Խ��ͼ����б���
 *
 *************************************************************************
 */
CString CRSIDoc::OnFileSaveAs_result ()
{
	
	// TODO: Add your command handler code here
	CString strSaveFileType = "λͼ�ļ� (*.bmp;*.dib)|*.bmp; *.dib|All Files (*.*)|*.*||";
	CString str;
	CFileDialog FileDlg(FALSE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strSaveFileType);
	CFile fileOpen ;
	if( FileDlg.DoModal() == IDOK ) {
		str=FileDlg.GetPathName();
		if(!fileOpen.Open( FileDlg.GetPathName() , CFile::modeCreate|CFile::modeWrite )){
			AfxMessageBox("cannot create the file to save");
			return str;
		}
		if( !m_pDibInit->Write( &fileOpen ) ){
			return str;
		}
		
		fileOpen.Close();
	}
	/*if(!fileOpen.Open( FileDlg.GetPathName() , CFile::modeRead ))
	{
		AfxMessageBox("cannot open the file");
		return;
		}
	fileOpen.Open( FileDlg.GetPathName() , CFile::modeRead );*/
	return str;
}
/*************************************************************************
 *
 * \�������ƣ�
 *   OnFileReload()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú�����ӦID_FILE_RELOAD�������Ϣ��������DIB
 *
 *************************************************************************
 */
void CRSIDoc::OnFileReload() 
{
	// �жϵ�ǰͼ���Ƿ��Ѿ����Ķ�
	if (IsModified())
	{
		// ��ʾ�û��ò�������ʧ���е�ǰ���޸�
		if (MessageBox(NULL, "���´�ͼ�񽫶�ʧ���иĶ����Ƿ������", "ϵͳ��ʾ", MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			// �û�ȡ��������ֱ�ӷ���
			return;
		}
	}
	CFile file;

	CString strPathName;
	CFileException fe;

	strPathName = GetPathName();

	// ���´��ļ�
	if( !file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// ʧ��
		ReportSaveLoadException(strPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);

		// ����
		return;
	}

	// ���Ĺ��
	BeginWaitCursor();

	// ���¶�ȡͼ��
	if(!m_pDibInit->Read(&file)){
		// �ָ������״
 		EndWaitCursor();
 				
 		 		
 		// ����
 		return;
	}

	// ��ʼ���ͱ��ΪFALSE
 	SetModifiedFlag(FALSE);
 	
 	// ˢ��
 	UpdateAllViews(NULL);
  	
 	// �ָ������״
 	EndWaitCursor();
	
 	// ����
 	return;
}


DWORD CRSIDoc::ChangeToBmp(const char* filename)
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
			ReportSaveLoadException(filename, &fe, TRUE, AFX_IDP_INVALID_FILENAME);
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
			reselesct=show.DoModal();
			flag = FALSE;

		}

		
		

	} while (reselesct==IDCANCEL);
	delete[]pData;
	delete show;
	img = cv::imread(filename, 0);
	cv::imwrite(filename, img);

	CFile file1(filename, CFile::modeRead);
	CArchive ar(&file1, CArchive::load);
	m_pDibInit->Read(ar.GetFile());
	file1.Close();

	return 1;
}



BOOL CRSIDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  �ڴ������ר�õĴ�������
	
	GDALAllRegister();//ע��GDAL��
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  //֧������·��
	GDALDataset *pDataSet;
	pDataSet = (GDALDataset *)GDALOpen(lpszPathName, GA_ReadOnly);
	//16λת8λ
	if (pDataSet->GetRasterBand(1)->GetRasterDataType() == GDT_UInt16 || pDataSet->GetRasterBand(1)->GetRasterDataType() == GDT_Int16)
	{
		GDALClose(pDataSet);
		GISImage GIS;
		pDataSet=GIS.Stretch_percent_16to8(lpszPathName);
	}

	if (pDataSet != NULL)
	{
		nbands = pDataSet->GetRasterCount();
		lWidth = pDataSet->GetRasterXSize();
		lHeight = pDataSet->GetRasterYSize();
	
		pData = new WORD[lWidth*lHeight*nbands];
		pDataSet->RasterIO(GF_Read, 0, 0, lWidth, lHeight, pData, lWidth, lHeight, GDT_UInt16, nbands, 0, 0, 0, 0);

	}
	ChangeToBmp("myBmp1.bmp");
	GDALClose(pDataSet);
	return TRUE;
	
}

