// RSIView.cpp : implementation of the CRSIView class
//

#include "stdafx.h"
#include "ң��ͼ����׼ϵͳ.h"

#include "RSIDoc.h"
#include "RSIView.h"
#include  "GlobalApi.h"
#include <complex>
using namespace std;



#include "DlgReg.h"
#include  "DlgCor.h"
#include  "DlgEnhance.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRSIView

IMPLEMENT_DYNCREATE(CRSIView, CScrollView)

BEGIN_MESSAGE_MAP(CRSIView, CScrollView)
	//{{AFX_MSG_MAP(CRSIView)
	ON_COMMAND(ID_REG_REG, OnRegReg)
	ON_COMMAND(ID_COR_COR, OnCorCor)
	ON_COMMAND(ID_ENHANCE_ENHANCE, OnEnhanceEnhance)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRSIView construction/destruction

CRSIView::CRSIView()
{
	// ΪС���任���õĲ���
	// ��ʱ���С���任ϵ���ڴ�
	m_pDbImage = NULL;	
	
	// ���õ�ǰ����
	m_nDWTCurDepth = 0;

	// ����С������֧������
	m_nSupp = 1;
}

CRSIView::~CRSIView()
{
	// �ͷ��ѷ����ڴ�
	if(m_pDbImage){
		delete[]m_pDbImage;
		m_pDbImage = NULL;
	}
}

BOOL CRSIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRSIView drawing

void CRSIView::OnDraw(CDC* pDC)
{
	CRSIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeDibDisplay;		

	
	

	if(!pDoc->m_pDibInit->IsEmpty()){
		sizeDibDisplay = pDoc->m_pDibInit->GetDimensions();
		pDoc->m_pDibInit->Convert256toGray();
		pDoc->m_pDibInit->Draw(pDC,CPoint(0,0),sizeDibDisplay);	
	}

	
	// TODO: add draw code for native data here
	/*if (pDoc->m_DIBt.IsValid())
	{
		int cxDIB = (int)pDoc->m_DIBt.Width();
		int cyDIB = (int)pDoc->m_DIBt.Height();
		///////////////////////////////
		CSize sizeTotal;
		sizeTotal.cx = cxDIB;
		sizeTotal.cy = cyDIB;
		SetScrollSizes(MM_TEXT, sizeTotal);
		//���»�ô��ڴ�С
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest;
		if (pDC->IsPrinting())
		{
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB*cxPage*cyInch) / ((double)cxDIB*cxInch));
			rcDest.right = cxPage;
		}
		else
		{
			rcDest = rcDIB;
		}
		pDoc->m_DIBt.Paint(pDC->m_hDC, &rcDest, &rcDIB);
	}*/

}

void CRSIView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CRSIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeTotal = pDoc->m_pDibInit->GetDimensions();
	SetScrollSizes(MM_TEXT, sizeTotal);

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();


	/*CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy =400;//100
	SetScrollSizes(MM_TEXT, sizeTotal);
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();*/
}

/////////////////////////////////////////////////////////////////////////////
// CRSIView printing


void CRSIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRSIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRSIView diagnostics

#ifdef _DEBUG
void CRSIView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CRSIView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CRSIDoc* CRSIView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRSIDoc)));
	return (CRSIDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRSIView message handlers







/*************************************************************************
 *
 * \�������ƣ�
 *   OnRegReg()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú���ʵ��ͼ�����׼
 *
 *************************************************************************
 */
void CRSIView::OnRegReg() 
{	
	// ����ĵ�����
	CRSIDoc* pDoc;
	pDoc = GetDocument();

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ��ͼ����׼�Ի���
	CDlgReg* pDlg=new CDlgReg(NULL,pDoc);

	pDlg->DoModal();

	delete pDlg;
}



/*************************************************************************
 *
 * \�������ƣ�
 *   OnCorCor()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú���ʵ��ͼ��ļ���У��
 *
 *************************************************************************
 */
void CRSIView::OnCorCor() 
{	
	// ����ĵ�����
	CRSIDoc* pDoc;
	pDoc = GetDocument();

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ��ͼ����׼�Ի���
	CDlgCor* pDlg=new CDlgCor(NULL,pDoc);

	pDlg->DoModal();

	delete pDlg;
}
BOOL CRSIView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	
	return CScrollView::OnPreparePrinting(pInfo);
}


/*************************************************************************
 *
 * \�������ƣ�
 *   OnENHANCEENHANCE()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú���ʵ��ͼ��ļ���У��
 *
 *************************************************************************
 */
void CRSIView::OnEnhanceEnhance() 
{	
	// ����ĵ�����
	CRSIDoc* pDoc;
	pDoc = GetDocument();

	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ��ͼ����׼�Ի���
	CDlgEnhance* pDlg=new CDlgEnhance(NULL,pDoc);

	pDlg->DoModal();

	delete pDlg;
}

