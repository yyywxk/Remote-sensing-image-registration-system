//dib.h

#ifndef _INC_DIB
#define _INC_DIB

#define PALVERSION 0x300

#define DIB_HEADER_MARKER ((WORD) ('M'<<8)|'B')

#define RECTWIDTH(lpRect)	((lpRect)->right-(lpRect)->left)
#define RECTHEIGHT(lpRect)	((lpRect)->bottom-(lpRect)->top)
#define WIDTHBYTES(bits)	(((bits)+31)/32*4)

class CDibt: public CObject
{
	DECLARE_DYNAMIC(CDibt)

public:
	CDibt();
	LPBYTE m_pBits;
	LPBITMAPINFO m_pBMI;
public:
	CPalette * m_pPalette;

public:
	DWORD Width()		const;
	DWORD Height()		const;
	WORD  NumColors()	const;
	BOOL IsValid()		const { return (m_pBMI != NULL); }

public:
	BOOL Paint(HDC, LPRECT, LPRECT)	const;
	HGLOBAL CopyToHandle()			const;
	DWORD Save(CFile& file)			const;
	DWORD Read(CFile& file);
	DWORD ReadFromHandle(HGLOBAL hGlobal);
	BOOL CreatePalette();
	LPBYTE RETURN()
	{
		return m_pBits;
	}
	LPBYTE DD(LPBYTE m)
	{
		m_pBits = m;
		return m_pBits;
	}
	void Invalidate() { Free(); }
	virtual void Serialize(CArchive& ar);

public:
	virtual~CDibt();
	void Free();

protected:
	WORD PaletteSize() const;


public:
	void SetBits(LPBYTE a);
	void SetBMI(LPBITMAPINFO a);
	BITMAPFILEHEADER bmfHeader;
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CDibt& operator =(CDibt& dibt);
};

#endif//!_INC_DIB
#pragma once
