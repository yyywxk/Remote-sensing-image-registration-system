// ChoseShow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ң��ͼ����׼ϵͳ.h"
#include "ChoseShow.h"
#include "afxdialogex.h"
#include "RSIDoc.h"


// ChoseShow �Ի���

IMPLEMENT_DYNAMIC(ChoseShow, CDialog)

ChoseShow::ChoseShow(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CHOSE_SHOW, pParent)
{

}

ChoseShow::~ChoseShow()
{
}

void ChoseShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
}


BEGIN_MESSAGE_MAP(ChoseShow, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ChoseShow ��Ϣ�������


void ChoseShow::OnPaint()
{
	/*
	// ��ÿؼ�IDC_REG_INIT_IMAGE�ľ��������ÿؼ��ĳ�ʼλ����Ϣ
	CWnd* pWnd = GetDlgItem(IDC_PICTURE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement = new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);
	// �����ؼ�IDC_COR_INIT_IMAGE�Ĵ�Сλ�ã���ͬʱ������ʾ��׼ͼ���λ��
	winPlacement->rcNormalPosition.top = 60;
	winPlacement->rcNormalPosition.right = 20;
	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 400;
	winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 400;
	pWnd->SetWindowPlacement(winPlacement);

	// �����˶Ի���Ĵ�С
	this->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = 500;
	winPlacement->rcNormalPosition.right = 440;
	this->SetWindowPlacement(winPlacement);

	// ���ÿؼ�IDOK��λ�ô�С
	pWnd = GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = 20;
	winPlacement->rcNormalPosition.bottom = 40;
	pWnd->SetWindowPlacement(winPlacement);

	// ���ÿؼ�IDCANCEL��λ�ô�С
	pWnd = GetDlgItem(IDCANCEL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = 20;
	winPlacement->rcNormalPosition.bottom = 40;
	pWnd->SetWindowPlacement(winPlacement);
	*/


	HBITMAP hbitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),filename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_picture.SetBitmap(hbitmap);
	
	//CPaintDC dc(this); // device context for painting

	//CRect  rect;
	//GetClientRect(&rect);
	//CDC  dcMem;   //����һ�������䣨�豸�����ģ�

	//dcMem.CreateCompatibleDC(&dc);///��������DC 
	//CBitmap  bmpBackground;   //λͼ����
	//bmpBackground.LoadBitmap(IDB_BITMAP1);   //IDB_BITMAP�����Լ���ͼ��Ӧ��ID  
	//BITMAP  bitmap;
	//bmpBackground.GetBitmap(&bitmap);  //�����󶨹�ϵ
	//CBitmap  *pbmpOld = dcMem.SelectObject(&bmpBackground);   //����ԭ��CDC���󣬲�ѡ����CDC������DC

	//dc.SetStretchBltMode(COLORONCOLOR);//��ֹbmpͼƬʧ��
	//dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	//dcMem.SelectObject(pbmpOld);
	//bmpBackground.DeleteObject();
	//dcMem.DeleteDC();

	//ChoseShow::OnPaint();  //���ŵ���

					   
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}
