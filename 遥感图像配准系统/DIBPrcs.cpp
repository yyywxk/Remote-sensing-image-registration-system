/*************************************************************************
 * �ļ�DIBPrcs.cpp��������й�DIB�Ĳ�������
 * �ļ�ͷ�����GlobalApi.h��
 *************************************************************************
 */

#include "GlobalApi.h"
#include "stdafx.h"
#include "cdib.h"
#include "MyException.h"



/*************************************************************************
 *
 * \�������ƣ�
 *   GetSystemPalette()
 *
 * \�������:
 *   ��
 *
 * \����ֵ:
 *   HPALETTE				- ϵͳ��ɫ����
 *
 * \˵��:
 *   �ú�����õ�ǰ����ʹ�õ�ϵͳ��ɫ��ľ��
 *
 *************************************************************************
 */
HPALETTE GetSystemPalette() 
{ 
	// �豸������
	HDC hDC;                

	// ������ɫ������ָ�����ʱ����
	static HPALETTE hPal = NULL;   
	HANDLE hLogPal;         
	LPLOGPALETTE lpLogPal;  

	// ��ǰϵͳ��ɫ�����ɫ��
	int nColors;            
 
    	// ��õ�ǰϵͳ�豸������
	hDC = GetDC(NULL); 
	if (!hDC) 
		return NULL; 
 
	// ��õ�ǰϵͳ��ɫ�����ɫ��Ŀ
	nColors = ( 1 << (GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES)));
    
	// ����ɫ������ڴ�
	hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + nColors * 
		sizeof(PALETTEENTRY)); 
	if (!hLogPal) 
		return NULL; 
 
	// �õ���ɫ���ڴ�ָ��
	lpLogPal = (LPLOGPALETTE)GlobalLock(hLogPal); 
 
	// ���õ�ɫ����Ϣ
	lpLogPal->palVersion = 0x300; 
	lpLogPal->palNumEntries = nColors; 
 
	// ��ϵͳ�ĵ�ɫ�忽������ǰ���߼���ɫ����
	GetSystemPaletteEntries(hDC, 0, nColors, 
            (LPPALETTEENTRY)(lpLogPal->palPalEntry)); 
 
	// ����Windows��ɫ��
	hPal = CreatePalette(lpLogPal); 
 
	// �ͷ��ѷ����ڴ沢ɾ����ʱ����
	GlobalUnlock(hLogPal); 
	GlobalFree(hLogPal); 
	ReleaseDC(NULL, hDC); 
 
	// ����
	return hPal; 
} 
 

/*************************************************************************
 *
 * \�������ƣ�
 *   CopyPalette
 *
 * \�������:
 *   HPALETTE	hPalSrc			- ��Ҫ������Դ��ɫ����
 *
 * \����ֵ:
 *   HPALETTE				- ��������ɹ����򷵻ؿ����ĵ�ɫ����
 *
 * \˵��:
 *   �ú���������һ���µĵ�ɫ�壬����ָ���ĵ�ɫ�忽����ɫ������
 *
 *************************************************************************
 */
HPALETTE CopyPalette(HPALETTE hPalSrc)
{
	// ��ɫ��ָ�룬��ʱ����
	PLOGPALETTE     plogPal;

	// ����һ����ɫ������һ����ʱ���
	HPALETTE        hPalette;
	HANDLE		hTemp;

	// ��ɫ�������
	int             iNumEntries=0;
    
	// ��ȡ��ɫ���еı�����
	iNumEntries = GetPaletteEntries(hPalSrc, 0, iNumEntries, NULL);
	if (iNumEntries == 0) 
		return (HPALETTE) NULL;

	// �����ɫ���ڴ棬�õ����
	hTemp = GlobalAlloc(GHND, sizeof(DWORD) + sizeof(PALETTEENTRY)*iNumEntries);
	if (! hTemp)
		return (HPALETTE) NULL;

	// �õ���ɫ���ָ��
	plogPal = (PLOGPALETTE)GlobalLock(hTemp);
	if (! plogPal) 
		return (HPALETTE) NULL;

	// ���õ�ɫ�����Ϣ
	plogPal->palVersion = 0x300;
	plogPal->palNumEntries = (WORD) iNumEntries;
    
	// ��ȡ�߼���ɫ����ָ����Χ�ĵ�ɫ�����
	GetPaletteEntries(hPalSrc, 0, iNumEntries, plogPal->palPalEntry);

	// ����һ��Windows��ɫ��
	hPalette = CreatePalette(plogPal);

	// �ͷ��Է�����ڴ�
	GlobalUnlock( hTemp );
	GlobalFree  ( hTemp );

	return hPalette;
}
 



/*************************************************************************
 *
 * \�������ƣ�
 *   ReadDIB
 *
 * \�������:
 *   CFile*			pFile		- ��Ҫ�򿪵�DIB�ļ�
 *   LPBITMAPINFOHEADER*	pLpBMIH		- DIB��Ϣͷָ���ָ��
 *   LPBYTE*			pLpImage	- DIBλͼ���ݿ�ָ���ָ��
 *
 * \����ֵ:
 *   BOOL				- ��������ɹ����򷵻�TRUE
 *
 * \˵��:
 *   �ú�����ָ���ļ��е�DIB�ļ����룬������Ϣͷ�͵�ɫ�����*pLpBMIH��
 *   ͼ�����ݴ�ŵ�*pLpImage�С�
 *
 *************************************************************************
 */
 BOOL ReadDIB(CFile* pFile, LPBITMAPINFOHEADER* pLpBMIH, LPBYTE* pLpImage)
 {
	 // ��ʱ�����Ϣ�ı���
 	int nCount, nSize;
	BITMAPFILEHEADER bmfh;

	// ��Ϣͷָ��
	//LPBITMAPINFOHEADER lpBMIH;

	// DIBͼ������ָ��
	//LPBYTE lpImage;

	// ���ж�����
	try 
	{
		// ��ȡ�ļ�ͷ
		nCount = pFile->Read((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
		if(nCount != sizeof(BITMAPFILEHEADER)) {
			throw new CMyException;
		}

		// ����ļ����Ͳ�λ"BM"���򷵻ز�������Ӧ������
		if(bmfh.bfType != 0x4d42) {
			throw new CMyException;
		}

		// ������Ϣͷ���ϵ�ɫ��Ĵ�С����������Ӧ���ڴ�
		nSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
		*pLpBMIH = (LPBITMAPINFOHEADER) new char[nSize];

		// ��ȡ��Ϣͷ�͵�ɫ��
		nCount = pFile->Read(*pLpBMIH, nSize); 

		// ����ͼ�����ݴ�С�����õ�ɫ��ָ��
		if((*pLpBMIH)->biSize != sizeof(BITMAPINFOHEADER)) {
			TRACE("Not a valid Windows bitmap -- probably an OS/2 bitmap\n");
			throw new CMyException;
		}

		// ���ͼ�������ڴ��СΪ0�������¼���
		if((*pLpBMIH)->biSizeImage == 0) {
			DWORD dwBytes = ((DWORD) (*pLpBMIH)->biWidth * (*pLpBMIH)->biBitCount) / 32;
			if(((DWORD) (*pLpBMIH)->biWidth * (*pLpBMIH)->biBitCount) % 32) {
				dwBytes++;
			}
			dwBytes *= 4;
			(*pLpBMIH)->biSizeImage = dwBytes * (*pLpBMIH)->biHeight;	
		}
		
		// ����ͼ�������ڴ棬�����ļ��ж�ȡͼ������
		*pLpImage = (LPBYTE) new char[(*pLpBMIH)->biSizeImage];
		nCount = pFile->Read((*pLpImage), (*pLpBMIH)->biSizeImage); 
	}

	// ������
	catch(CMyException* pe) 
	{
		AfxMessageBox("Read error");
		pe->Delete();
		return FALSE;
	}

	// ��ָ�븳ֵ

	// ����
	return TRUE;


 }


 /*************************************************************************
 *
 * \�������ƣ�
 *   MakeDIBPalette()
 *
 * \�������:
 *   LPVOID		lpvColorTable	- ��ɫ��ָ��
 *   LPBITMAPINFOHEADER lpBMIH		- DIB��Ϣͷָ��
 *
 * \����ֵ:
 *   HPALETTE				- ����ɹ������ɫ����
 *
 * \˵��:
 *   �ú�������ȡ��ɫ��������һ��Windows��ɫ�壬�����ش˵�ɫ��ľ��
 *
 ************************************************************************
 */
HPALETTE MakeDIBPalette(LPVOID lpvColorTable,  LPBITMAPINFOHEADER lpBMIH)
{
	// ��ɫ����
	HPALETTE hPalette = NULL;				

	// ��ɫ����ɫ��
	int nColorTableEntries;
	
	// ����DIB�еĵ�ɫ��ָ��
	// lpvColorTable = (LPBYTE) lpBMIH + sizeof(BITMAPINFOHEADER);

	// �����ɫ��ı�����
	if(lpBMIH->biClrUsed == 0) {
		switch(lpBMIH->biBitCount) {
			case 1:
				nColorTableEntries = 2;
				break;
			case 4:
				nColorTableEntries = 16;
				break;
			case 8:
				nColorTableEntries = 256;
				break;
			case 16:
			case 24:
			case 32:
				nColorTableEntries = 0;
				break;
			default:
				break;
		}
	}

	// �����ɫ��ı����������õ�����ɫ��Ŀ
	else {
		nColorTableEntries = lpBMIH->biClrUsed;
	}

	ASSERT((nColorTableEntries >= 0) && (nColorTableEntries <= 256)); 

	// ��������ڵ�ɫ�壬�򷵻�FALSE
	if(nColorTableEntries == 0) return FALSE;

	// ���߼���ɫ������ڴ�
	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +
		nColorTableEntries * sizeof(PALETTEENTRY)];

	// �����߼���ɫ�����Ϣ
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = nColorTableEntries;

	// ����DIB�е���ɫ���߼���ɫ��
	LPRGBQUAD pDibQuad = (LPRGBQUAD) lpvColorTable;
	for(int i = 0; i < nColorTableEntries; i++) {
		pLogPal->palPalEntry[i].peRed = pDibQuad->rgbRed;
		pLogPal->palPalEntry[i].peGreen = pDibQuad->rgbGreen;
		pLogPal->palPalEntry[i].peBlue = pDibQuad->rgbBlue;
		pLogPal->palPalEntry[i].peFlags = 0;
		pDibQuad++;
	}

	// �����߼���ɫ��
	hPalette = ::CreatePalette(pLogPal);

	// ɾ����ʱ����
	delete pLogPal;

	// ���ص�ɫ����
	return hPalette;
}	

 /*************************************************************************
 *
 * \�������ƣ�
 *   PaintDIB()
 *
 * \�������:
 *   CDC*	pDC			- ָ��Ҫ����DIBͼ����豸������ָ�� 
 *   LPBITMAPINFOHEADER lpBMIH		- DIB��Ϣͷָ��
 *   LPBYTE		lpImage;	- DIBλͼ���ݿ��ַ 
 *   CPoint	origin			- ��ʾDIB���߼�����
 *   CSize	size			- ��ʾ���εĿ�Ⱥ͸߶�
 *   HPALETTE	hPalette		- ָ��DIB�ĵ�ɫ����
 *
 * \����ֵ:
 *   BOOL				- ����ɹ����򷵻�TRUE
 *
 * \˵��:
 *   �ú�����DIBͼ�������ʾ
 *
 ************************************************************************
 */
BOOL PaintDIB(CDC* pDC, LPBITMAPINFOHEADER lpBMIH, LPBYTE lpImage, CPoint origin, CSize size, HPALETTE hPalette)
{
	if(lpBMIH == NULL) return FALSE;

	// �����ɫ�岻Ϊ�գ��򽫵�ɫ��ѡ���豸������
	if(hPalette != NULL) {
		::SelectPalette(pDC->GetSafeHdc(), hPalette, TRUE);
	}

	// ������ʾģʽ
	pDC->SetStretchBltMode(COLORONCOLOR);

	// ���豸��originλ���ϻ�����СΪsize��ͼ��
	::StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y,size.cx,size.cy,
		0, 0, lpBMIH->biWidth, lpBMIH->biHeight,
		lpImage, (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS, SRCCOPY);

	// ����
	return TRUE;
}


/*************************************************************************
 *
 * \�������ƣ�
 *   ConvertDDBToDIB()
 *
 * \�������:
 *   HBITMAP	hBitmap			- ָ��Դ���ݵ�BITMAP���
 *   CDib*	pDibDst			- ָ��ת��Ŀ���CDib����ָ��
 *
 * \����ֵ:
 *   BOOL				- ��������ɹ����򷵻�TRUE
 *
 * \˵��:
 *   �ú�����ԴBITMAP��pDibSrc�е����ݿ�����pDibDst�У�������Ӧ�����ݳ�Ա��ֵ
 *
 *************************************************************************
 */

BOOL ConvertDDBToDIB(HBITMAP hBitmap, CDib* pDibDest, HPALETTE hPal)
{
	// ����һ��BITMAP�ṹ
	BITMAP bm;

	// �豸������
	HDC hDC;
	
	// ����λ��
	WORD biBitCount;
	
	// ��ɫ�������
	int nColorTableEntries;

	// ���hBitmap�����Ч���򷵻�
	if(!hBitmap){
		return FALSE;
	}

	// ���ͼ�����ݵ�bm�У��������һ��������ʾ�������ָ���Ķ����ָ��
	if(!GetObject(hBitmap,sizeof(BITMAP),(LPBYTE)&bm)){
		return FALSE;
	}

	// ��������λ��
	biBitCount=bm.bmPlanes*bm.bmBitsPixel;
	if(biBitCount<=1)
		biBitCount=1;
	else if(biBitCount<=4)
		biBitCount=4;
	else if(biBitCount<=8)
		biBitCount=8;
	else 
		biBitCount=24;

	// �����ɫ��ĳߴ�
	// ���biClrUsedΪ�㣬���õ�����ɫ��Ϊ2��biBitCount�η�
	switch(biBitCount) {
		case 1:
			nColorTableEntries = 2;
			break;
		case 4:
			nColorTableEntries = 16;
			break;
		case 8:
			nColorTableEntries = 256;
			break;
		case 16:
		case 24:
		case 32:
			nColorTableEntries = 0;
			break;
		default:
			ASSERT(FALSE);
	}	
	ASSERT((nColorTableEntries >= 0) && (nColorTableEntries <= 256)); 

	// ����DIB��Ϣͷ�͵�ɫ����ڴ�
	LPBITMAPINFOHEADER lpBMIH = (LPBITMAPINFOHEADER) new char
		[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorTableEntries];

	lpBMIH->biSize = sizeof(BITMAPINFOHEADER); 
	lpBMIH->biWidth = bm.bmWidth; 
	lpBMIH->biHeight = bm.bmHeight; 
	lpBMIH->biPlanes = 1; 
	lpBMIH->biBitCount = biBitCount; 
	lpBMIH->biCompression = BI_RGB; 
	lpBMIH->biSizeImage = 0; 
	lpBMIH->biXPelsPerMeter = 0; 
	lpBMIH->biYPelsPerMeter = 0; 
	lpBMIH->biClrUsed = nColorTableEntries;
	lpBMIH->biClrImportant = nColorTableEntries;	

	// ����豸�����ľ��
	hDC=GetDC(NULL);

	  // select and realize our palette 
 
	// ���û��ָ����ɫ�壬���ϵͳ�л�õ�ǰ��ϵͳ��ɫ��
	if(hPal==NULL){
		hPal = GetSystemPalette();
	}
	hPal = SelectPalette(hDC, hPal, FALSE); 
	RealizePalette(hDC); 
 

	// ����GetDIBits�����Ϣͷ�������ͼ�����ݵĳߴ硣ע������ͼ������ָ��ΪNULL
	GetDIBits( hDC, hBitmap, 0, (UINT)lpBMIH->biHeight, NULL, (LPBITMAPINFO)lpBMIH, DIB_RGB_COLORS);

	// ���û����ȷ�Ļ��ͼ�����ݳߴ磬�����¼���
	if( lpBMIH->biSizeImage == 0 ){
		lpBMIH->biSizeImage=(((bm.bmWidth*biBitCount) + 31) / 32 * 4)*bm.bmHeight;
	}

	// ������ͼ�����ݵ��ڴ�
	LPBYTE lpImage = (LPBYTE) new char[lpBMIH->biSizeImage];

	// ����GetDIBits����ͼ�����ݣ�ע�����������ͼ������ָ��

	// �������ͼ�����ݲ��ɹ������ͷ��Ѿ�������ڴ棬������FALSE
	if( GetDIBits( hDC, hBitmap, 0, (UINT)lpBMIH->biHeight, (LPBYTE)lpImage,
		(LPBITMAPINFO)lpBMIH, DIB_RGB_COLORS) == 0 ){
		//clean up and return NULL
		delete []lpImage;
		delete []lpBMIH;

		SelectPalette( hDC, hPal, TRUE );
		RealizePalette( hDC );
		ReleaseDC( NULL, hDC );
		
		return FALSE;
	}

	// ��ָ�뱣����CDib��������ݳ�Ա��
	pDibDest->m_lpBMIH  = lpBMIH;
	pDibDest->m_lpImage = lpImage;
	pDibDest->m_nBmihAlloc = pDibDest->m_nImageAlloc = pDibDest->crtAlloc;


	// ɾ����ʱ����
	SelectPalette(hDC, hPal, TRUE); 
	RealizePalette(hDC); 
	ReleaseDC(NULL, hDC); 
	return TRUE;
}


/*************************************************************************
 *
 * \�������ƣ�
 *   CopyDIB()
 *
 * \�������:
 *   CDib*	pDibSrc			- ָ��Դ���ݵ�CDib����ָ��
 *   CDib*	pDibDst			- ָ�򿽱�Ŀ���CDib����ָ��
 *
 * \����ֵ:
 *   BOOL				- ��������ɹ����򷵻�TRUE
 *
 * \˵��:
 *   �ú�����ԴCDib��pDibSrc�е����ݿ�����pDibDst�У�������Ӧ�����ݳ�Ա��ֵ
 *
 *************************************************************************
 */
BOOL CopyDIB(CDib* pDibSrc, CDib* pDibDst)
{
	// ��Ŀ��CDib�������
	pDibDst->Empty();
	
	// ������Ϣͷ���ϵ�ɫ��Ĵ�С����������Ӧ���ڴ�
	int nSizeHdr = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * pDibSrc->m_nColorTableEntries;

	pDibDst->m_lpBMIH = (LPBITMAPINFOHEADER) new char[nSizeHdr];
	pDibDst->m_nBmihAlloc = pDibDst->m_nImageAlloc = pDibDst->crtAlloc;

	try{
		
		// ������Ϣͷ�͵�ɫ��
		memcpy(pDibDst->m_lpBMIH,pDibSrc->m_lpBMIH,nSizeHdr);
		
		
		// ����ṹ�ĳ��Ȳ��ԣ�����д�����
		if(pDibDst->m_lpBMIH->biSize != sizeof(BITMAPINFOHEADER)) {
			TRACE("Not a valid Windows bitmap -- probably an OS/2 bitmap\n");
			throw new CMyException;
		}

		// ����ͼ�������ڴ��С��CDib��������ݳ�Ա��
		pDibDst->m_dwSizeImage = pDibDst->m_lpBMIH->biSizeImage;

		// ���ͼ�������ڴ��СΪ0�������¼���
		if(pDibDst->m_dwSizeImage == 0) {
			DWORD dwBytes = ((DWORD) pDibDst->m_lpBMIH->biWidth * pDibDst->m_lpBMIH->biBitCount) / 32;
			if(((DWORD) pDibDst->m_lpBMIH->biWidth * pDibDst->m_lpBMIH->biBitCount) % 32) {
				dwBytes++;
			}
			dwBytes *= 4;
			pDibDst->m_dwSizeImage = dwBytes * pDibDst->m_lpBMIH->biHeight; 
		}

		// ����DIB�еĵ�ɫ��ָ��
		pDibDst->m_lpvColorTable = (LPBYTE) pDibDst->m_lpBMIH + sizeof(BITMAPINFOHEADER);

		// �����ɫ��ı�����
		pDibDst->ComputePaletteSize(pDibDst->m_lpBMIH->biBitCount);

		// ���DIB�д��ڵ�ɫ�壬�򴴽�һ��Windows��ɫ��
		pDibDst->MakePalette();

		// ����ͼ�������ڴ棬������ͼ������
		pDibDst->m_lpImage = (LPBYTE) new char[pDibDst->m_dwSizeImage];
		memcpy(pDibDst->m_lpImage, pDibSrc->m_lpImage,pDibDst->m_dwSizeImage);
	}

	catch(CMyException* pe) 
	{
		AfxMessageBox("Copy DIB error");
		pDibDst->Empty();
		pe->Delete();
		return FALSE;
	}
	
	return TRUE;
}


/*************************************************************************
 *
 * \�������ƣ�
 *   CopyScreenToDIB
 *
 * \�������:
 *   LPRECT	lpRect			- ��Ҫ��������Ļ����
 *   CDib*	pDibDest		- ָ��Ŀ��CDib�����ָ��
 *
 * \����ֵ:
 *   BOOL				- ��������ɹ����򷵻�TRUE
 *
 * \˵��:
 *   �ú�����ָ������λ���ڵ���Ļ���ݿ�����DIB��ԴCDib��pDibSrc�е����ݿ�����pDibDst��
 *
 *************************************************************************
 */
BOOL CopyScreenToDIB(LPRECT lpRect, CDib* pDibDest)
{
	// ��Ļ�豸�����ĺ��ڴ��豸�����ľ��
	HDC         hScrDC, hMemDC;         

	// ����BITMAP��ʱ�������ǰ��BITMAP���
	HBITMAP     hBitmap, hOldBitmap;    

	// ��ɫ����
	HPALETTE    hPalette;       
    
	// ��ȡ�������������
	int         nX, nY, nX2, nY2;

	// DIBͼ��ĸ߶ȺͿ��
	int         nWidth, nHeight; 

	// ��Ļ�ֱ���
	int         xScrn, yScrn;    
 
        // ��������ľ�������Ϊ�գ��򲻽��н�һ���Ĵ���
	if (IsRectEmpty(lpRect)) 
		return FALSE; 
 
    	// �õ�һ����Ļ�豸������
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL); 

	// ��������Ļ�豸���ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC); 
 
	// �õ����ε���������
	nX = lpRect->left; 
	nY = lpRect->top; 
	nX2 = lpRect->right; 
	nY2 = lpRect->bottom; 
 
	// �õ���Ļ�ķֱ��ʣ��Ա������жϴ���
	xScrn = GetDeviceCaps(hScrDC, HORZRES); 
	yScrn = GetDeviceCaps(hScrDC, VERTRES); 
 
	// �жϾ��������Ƿ񳬳���Ļ
 
	if (nX < 0) 
		nX = 0; 
	if (nY < 0) 
	        nY = 0; 
	if (nX2 > xScrn) 
	        nX2 = xScrn; 
	if (nY2 > yScrn) 
	        nY2 = yScrn; 
 
	// ����DIBͼ��ĸ߶ȺͿ��
	nWidth = nX2 - nX; 
	nHeight = nY2 - nY; 
 
	// ����һ������Ļ�豸�����ļ��ݵ�DDBλͼ
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight); 
 
	// ��DDBλͼѡ���ڴ��豸������
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); 
 
	// ����Ļ��ָ�������ͼ���䵽�ڴ��豸��������
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY); 
 
	// Ȼ����ǰ��ͼ��ѡ�룬���õ���Ļ�����DDBͼ����
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap); 
 
	// ����ʱ���豸������ɾ��
	DeleteDC(hScrDC); 
	DeleteDC(hMemDC); 
 
	// �õ���ǰϵͳ��ɫ��
	hPalette = GetSystemPalette(); 
 
	// ��DDBͼ��ת��ΪDIBͼ��
	pDibDest->ConvertFromDDB(hBitmap,hPalette);
	
	// ɾ����ʱ����
	DeleteObject(hPalette); 
	DeleteObject(hBitmap); 
	
	return TRUE;
}
