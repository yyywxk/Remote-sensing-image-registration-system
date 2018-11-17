//dib.cpp

#include "StdAfx.h"
#include "CDibt.h"
#include "windowsx.h"
#include "afxadv.h"
#include "io.h"
#include "errno.h"

//////////////////////////////////////////////////////////////////////////////////////
//CDib
IMPLEMENT_DYNAMIC(CDibt, CObject)
CDibt::CDibt()
{
	m_pBMI = NULL;
	m_pBits = NULL;
	m_pPalette = NULL;

}
CDibt::~CDibt()
{
	Free();
}

void CDibt::Free()
{
	if (m_pBits)
	{
		GlobalFreePtr(m_pBits);
		m_pBits = NULL;
	}
	if (m_pBMI)
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
	}
	if (m_pPalette)
	{
		m_pPalette->DeleteObject();
		delete m_pPalette;
		m_pPalette = NULL;
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDibt::Paint(HDC hDC, LPRECT lpDCRect, LPRECT lpDIBRect) const
{
	if (!m_pBMI)
		return FALSE;

	HPALETTE hPal = NULL;
	HPALETTE hOldPal = NULL;

	if (m_pPalette != NULL)
	{
		hPal = (HPALETTE)m_pPalette->m_hObject;
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	::SetStretchBltMode(hDC, COLORONCOLOR);

	BOOL bSuccess;
	if ((RECTWIDTH(lpDCRect) == RECTWIDTH(lpDIBRect)) &&
		(RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC,
			lpDCRect->left,
			lpDCRect->top,
			RECTWIDTH(lpDCRect),
			RECTHEIGHT(lpDCRect),
			lpDIBRect->left,
			(int)Height() -
			lpDIBRect->top -
			RECTHEIGHT(lpDIBRect),
			0,
			(WORD)Height(),
			m_pBits,
			m_pBMI,
			DIB_RGB_COLORS);
	else
		bSuccess = ::StretchDIBits(hDC,
			lpDCRect->left,
			lpDCRect->top,
			RECTWIDTH(lpDCRect),
			RECTHEIGHT(lpDCRect),
			lpDIBRect->left,
			lpDIBRect->top,
			RECTWIDTH(lpDIBRect),
			RECTHEIGHT(lpDIBRect),
			m_pBits,
			m_pBMI,
			DIB_RGB_COLORS,
			SRCCOPY);
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}
	return bSuccess;
}
////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDibt::CreatePalette()
{
	if (!m_pBMI)
		return FALSE;
	WORD wNumColors = NumColors();

	if (wNumColors != 0)
	{
		HANDLE hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY)*wNumColors);

		if (hLogPal == 0)
			return FALSE;
		LPLOGPALETTE lpPal = (LPLOGPALETTE)::GlobalLock((HGLOBAL)hLogPal);
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)wNumColors;

		for (int i = 0; i<(int)wNumColors; i++)
		{
			lpPal->palPalEntry[i].peRed = m_pBMI->bmiColors[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = m_pBMI->bmiColors[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue = m_pBMI->bmiColors[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}

		if (m_pPalette)
		{
			m_pPalette->DeleteObject();
			delete m_pPalette;
		}
		m_pPalette = new CPalette;
		BOOL bResult = m_pPalette->CreatePalette(lpPal);
		::GlobalUnlock((HGLOBAL)hLogPal);
		::GlobalFree((HGLOBAL)hLogPal);
		return bResult;
	}
	return TRUE;
}
DWORD CDibt::Width() const
{
	if (!m_pBMI)
		return 0;
	return m_pBMI->bmiHeader.biWidth;
}
DWORD CDibt::Height() const
{
	if (!m_pBMI)
		return 0;
	return m_pBMI->bmiHeader.biHeight;

}
WORD CDibt::PaletteSize() const
{
	if (!m_pBMI)
		return 0;
	return NumColors() * sizeof(RGBQUAD);
}
WORD CDibt::NumColors() const
{
	if (!m_pBMI)
		return 0;
	WORD wBitCount;
	DWORD dwClrUsed;
	dwClrUsed = m_pBMI->bmiHeader.biClrUsed;
	if (dwClrUsed != 0)
		return(WORD)dwClrUsed;
	wBitCount = m_pBMI->bmiHeader.biBitCount;
	switch (wBitCount)
	{
	case 1:
		return 2;
	case 4:
		return 16;
	case 8:
		return 256;
	default:
		return 0;
	}
}
DWORD CDibt::Save(CFile&file) const
{
	BITMAPFILEHEADER bmfHdr; // 位图文件头指针
	DWORD dwDIBSize;

	if (m_pBMI == NULL)
	{
		return 0;
	}

	//写文件头


	//文件类型最初2bits要是‘BM’
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"

										//信息头和调色板大小
	dwDIBSize = *(LPDWORD)&m_pBMI->bmiHeader + PaletteSize();

	//计算图像大小

	if ((m_pBMI->bmiHeader.biCompression == BI_RLE8) || (m_pBMI->bmiHeader.biCompression == BI_RLE4))
	{
		//如果是RLE位图，不能计算图像尺寸，直接采用biSizeImage;
		dwDIBSize += m_pBMI->bmiHeader.biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize;  //位图数据块大小

							 //不是RLE位图，大小为Width（DWORD aligned）*Height
		dwBmBitsSize = WIDTHBYTES((m_pBMI->bmiHeader.biWidth)*((DWORD)m_pBMI->bmiHeader.biBitCount))
			*m_pBMI->bmiHeader.biHeight;

		dwDIBSize += dwBmBitsSize;

		m_pBMI->bmiHeader.biSizeImage = dwBmBitsSize;
	}

	//计算整个文件大小，并且书写图像文件头
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;


	//计算图像文件头到图像数字的偏移
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + m_pBMI->bmiHeader.biSize + PaletteSize();

	//文件头
	file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
	DWORD dwBytesSaved = sizeof(BITMAPFILEHEADER);

	//信息头
	UINT nCount = sizeof(BITMAPINFO) + (NumColors() - 1) * sizeof(RGBQUAD);
	dwBytesSaved += nCount;
	file.Write(m_pBMI, nCount);

	//数据部分
	DWORD dwbytes = m_pBMI->bmiHeader.biBitCount*Width();

	//计算每行像素数据
	if (dwbytes % 32 == 0)
	{
		dwbytes /= 8;
	}
	else
	{
		dwbytes = dwbytes / 8 + (32 - dwbytes % 32) / 8 + (((32 - dwbytes % 32) % 8>0) ? 1 : 0);
	}
	nCount = dwbytes * Height();
	dwBytesSaved += nCount;
	file.Write(m_pBits, nCount);

	return dwBytesSaved;

}

DWORD CDibt::Read(CFile&file)
{
	Free();
	//	BITMAPFILEHEADER bmfHeader;
	if (file.Read((LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER)) != sizeof(bmfHeader))
		return 0;
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
		return 0;
	DWORD dwReadBytes = sizeof(bmfHeader);

	m_pBMI = (LPBITMAPINFO)GlobalAllocPtr(GHND, bmfHeader.bfOffBits - sizeof(BITMAPFILEHEADER) + 256 * sizeof(RGBQUAD));
	if (m_pBMI == 0)
		return 0;

	if (file.Read(m_pBMI, bmfHeader.bfOffBits - sizeof(BITMAPFILEHEADER)) != (UINT)
		(bmfHeader.bfOffBits - sizeof(BITMAPFILEHEADER)))
	{
		GlobalFree(m_pBMI);
		m_pBMI = NULL;

		return 0;

	}
	dwReadBytes += bmfHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
	DWORD dwLength = file.GetLength();

	m_pBits = (LPBYTE)GlobalAllocPtr(GHND, dwLength - bmfHeader.bfOffBits);
	if (m_pBits == 0)
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
		return 0;
	}

	if (file.Read(m_pBits, dwLength - bmfHeader.bfOffBits) != (dwLength - bmfHeader.bfOffBits))
	{
		GlobalFreePtr(m_pBMI);
		m_pBMI = NULL;
		GlobalFreePtr(m_pBits);
		m_pBits = NULL;
		return 0;
	}
	dwReadBytes += dwLength - bmfHeader.bfOffBits;
	CreatePalette();
	return dwReadBytes;


}

#ifdef _DEBUG
void CDibt::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif
HGLOBAL CDibt::CopyToHandle() const
{
	CSharedFile file;
	try
	{
		if (Save(file) == 0)
			return 0;
	}
	catch (CFileException* e)
	{
		e->Delete();
		return 0;

	}
	return file.Detach();
}

DWORD CDibt::ReadFromHandle(HGLOBAL hGlobal)
{

	CSharedFile file;
	file.SetHandle(hGlobal, FALSE);
	DWORD dwResult = Read(file);
	file.Detach();
	return dwResult;

}

void CDibt::Serialize(CArchive& ar)
{
	CFile* pFile = ar.GetFile();
	ASSERT(pFile != NULL);
	if (ar.IsStoring())
	{
		Save(*pFile);
	}
	else
	{
		Read(*pFile);
	}
}

void CDibt::SetBMI(LPBITMAPINFO a)
{
	m_pBMI = a;
}

void CDibt::SetBits(LPBYTE a)
{
	m_pBits = a;
}
