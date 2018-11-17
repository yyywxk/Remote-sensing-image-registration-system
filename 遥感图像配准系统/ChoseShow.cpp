// ChoseShow.cpp : 实现文件
//

#include "stdafx.h"
#include "遥感图像配准系统.h"
#include "ChoseShow.h"
#include "afxdialogex.h"
#include "RSIDoc.h"


// ChoseShow 对话框

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


// ChoseShow 消息处理程序


void ChoseShow::OnPaint()
{
	/*
	// 获得控件IDC_REG_INIT_IMAGE的句柄，并获得控件的初始位置信息
	CWnd* pWnd = GetDlgItem(IDC_PICTURE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement = new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);
	// 调整控件IDC_COR_INIT_IMAGE的大小位置，并同时设置显示基准图像的位置
	winPlacement->rcNormalPosition.top = 60;
	winPlacement->rcNormalPosition.right = 20;
	winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 400;
	winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 400;
	pWnd->SetWindowPlacement(winPlacement);

	// 调整此对话框的大小
	this->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = 500;
	winPlacement->rcNormalPosition.right = 440;
	this->SetWindowPlacement(winPlacement);

	// 设置控件IDOK的位置大小
	pWnd = GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = 20;
	winPlacement->rcNormalPosition.bottom = 40;
	pWnd->SetWindowPlacement(winPlacement);

	// 设置控件IDCANCEL的位置大小
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
	//CDC  dcMem;   //定义一个工具箱（设备上下文）

	//dcMem.CreateCompatibleDC(&dc);///建立关联DC 
	//CBitmap  bmpBackground;   //位图对象
	//bmpBackground.LoadBitmap(IDB_BITMAP1);   //IDB_BITMAP是你自己的图对应的ID  
	//BITMAP  bitmap;
	//bmpBackground.GetBitmap(&bitmap);  //建立绑定关系
	//CBitmap  *pbmpOld = dcMem.SelectObject(&bmpBackground);   //保存原有CDC对象，并选入新CDC对象入DC

	//dc.SetStretchBltMode(COLORONCOLOR);//防止bmp图片失真
	//dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	//dcMem.SelectObject(pbmpOld);
	//bmpBackground.DeleteObject();
	//dcMem.DeleteDC();

	//ChoseShow::OnPaint();  //最后才调用

					   
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialog::OnPaint()
}
