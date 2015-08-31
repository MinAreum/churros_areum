
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "HelloWorld.h"
#include "ChildView.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	dibData = NULL;
    dstData = NULL;
	leftButtonDown = FALSE; //추가해줘야해
}

CChildView::~CChildView()
{
	if(dibData)
		delete[] dibData;
	if(dstData)
		delete[] dstData;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_TEST_HELLOWORLD, &CChildView::OnTestHelloworld)
	ON_COMMAND(OnFileOpen, &CChildView::OnFileopen)
	ON_COMMAND(ID_ARITHMETIC_ADD, OnArithmeticAdd)
	ON_COMMAND(ID_ARITHMETIC_SUBTRACT, OnArithmeticSubtract)
	ON_COMMAND(ID_ARITHMETIC_MULTIPLY, OnArithmeticMultiply)
	ON_COMMAND(ID_ARITHMETIC_DIVIDE, OnArithmeticDivide)
	ON_COMMAND(ID_ARITHMETIC_NEGATIVE, OnArithmeticNegative)
	ON_COMMAND(ID_ARITHMETIC_BINARY, OnArithmeticBinary)
	ON_COMMAND(ID_GEOMETRY_FLIPV, OnGeometryFlipV)
	ON_COMMAND(ID_GEOMETRY_FLIPH, OnGeometryFlipH)
	ON_COMMAND(ID_GEOMETRY_ROTATELEFT, OnGeometryRotateLeft)
	ON_COMMAND(ID_GEOMETRY_ROTATERIGHT, OnGeometryRotateRight)
	ON_COMMAND(ID_LUT_ADD, &CChildView::OnLutAdd)
	ON_UPDATE_COMMAND_UI(ID_LUT_ADD, &CChildView::OnUpdateLutAdd)
	ON_COMMAND(ID_LUT_NEGATIVE, &CChildView::OnLutNegative)
	ON_UPDATE_COMMAND_UI(ID_LUT_NEGATIVE, &CChildView::OnUpdateLutNegative)
	ON_COMMAND(ID_LUT_GAMMA, &CChildView::OnLutGamma)
	ON_UPDATE_COMMAND_UI(ID_LUT_GAMMA, &CChildView::OnUpdateLutGamma)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_FILTER_BLUR, &CChildView::OnFilterBlur)
	ON_COMMAND(ID_FILTER_SHARPEN, &CChildView::OnFilterSharpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, RGB(0,0,255));
	CPen *pOldPen;
	pOldPen = dc.SelectObject(&Pen);

	CBrush Brush;
	Brush.CreateSolidBrush(RGB(0,255,0));
	CBrush *pOldBrush = dc.SelectObject(&Brush);
	{
		dc.MoveTo(0,0);
		dc.LineTo(100,100);
		dc.Rectangle(100,100,200,200);
	}
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
	
	Brush.DeleteObject();
	Pen.DeleteObject();
	
	if(dibData == NULL)
		return;

	 SetDIBitsToDevice(dc.m_hDC,
     0, 0, imageWidth, imageHeight,   
     0, 0, 0, imageHeight,            
     dstData, bitmapInfo, DIB_RGB_COLORS);	
}



void CChildView::OnTestHelloworld()
{
	AfxMessageBox(L"Hello World"); 
}


void CChildView::OnFileopen()
{
	// Show File Dialog Box
	CString szFilter = _T ("bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*|");

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY |
		OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, szFilter, this);
		if(dlg.DoModal()==IDCANCEL)
		return;

	// open File
	FILE *file;
	_wfopen_s(&file, dlg.GetPathName(), L"rb");
	
	// Read File Header
	BITMAPFILEHEADER bitmapFileHeader;
	fread(&bitmapFileHeader,sizeof(BITMAPFILEHEADER),1,file);

	// Calculate DIB size
	int dibSize = bitmapFileHeader .bfSize - sizeof (BITMAPFILEHEADER);

	// Allocate DIB size
	if( dibData)
		delete [] dibData;
	dibData = new unsigned char[ dibSize ];

	// Read DIB
	fread( dibData, dibSize, 1, file );
	
	// Important Variables
	bitmapInfo = (BITMAPINFO *) dibData;
	samplePerPixel = bitmapInfo->bmiHeader.biBitCount/8;
	imageWidth = bitmapInfo->bmiHeader.biWidth;
	imageHeight = bitmapInfo->bmiHeader.biHeight;
	imageStep = GetRealWidth (imageWidth);
	srcData = dibData + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bitmapInfo->bmiHeader.biClrUsed;

	// Allocate destination memory
	if( dstData )
		delete [] dstData;
	dstData = new unsigned char[ imageStep * imageHeight ];
	
	// Copy src to dst
	memcpy( dstData, srcData, imageStep * imageHeight );

	// Close file
	fclose( file );

	// Repaint client area
	Invalidate (FALSE);
}

int CChildView::GetRealWidth(int width)
{
   int real_width;
   div_t r;
   r = div(width,4);
   if(r .rem != 0){
      real_width = ((r.quot + 1) * 4);
      return real_width;
   }
   else
      return width;
}

unsigned char CChildView:: Clip (int value, int low, int high)
{
	if (value < low)
		return (unsigned char) low;
	else if (value > high)
		return (unsigned char) high;
	else
		return (unsigned char) value;
}


void CChildView::OnArithmeticAdd()
{
	if(dibData == NULL)
		return;
	for(int i=0; i<imageStep*imageHeight; i++){
		dstData[i] = Clip(srcData[i] + 100, 0, 255);
	}

	Invalidate(FALSE);
}

void CChildView::OnArithmeticSubtract()
{
	if(dibData == NULL)
		return;
	for(int i=0; i<imageStep*imageHeight; i++){
		dstData[i] = Clip(srcData[i] - 100, 0, 255);
	}

	Invalidate(FALSE);
}

void CChildView::OnArithmeticMultiply()
{
	if(dibData == NULL)
		return;
	for(int i=0; i<imageStep*imageHeight; i++){
		dstData[i] = Clip(srcData[i] * 2, 0, 255);
	}

	Invalidate(FALSE);
}

void CChildView::OnArithmeticDivide()
{
	if(dibData == NULL)
		return;
	for(int i=0; i<imageStep*imageHeight; i++){
		dstData[i] = Clip(srcData[i] / 2, 0, 255);
	}

	Invalidate(FALSE);
}

void CChildView::OnArithmeticNegative()
{
	if(dibData == NULL)
		return;
	for(int i=0; i<imageStep*imageHeight; i++){
		dstData[i] = (255 - srcData[i]);
	}

	Invalidate(FALSE);
}

void CChildView::OnArithmeticBinary()
{	  
	if(dstData == NULL)
		return;
	for(int i=0; i<imageStep*imageHeight; i++){
		if(srcData[i]<=120)
			dstData[i] = 0;
		else
			dstData[i] = 255;
}

	Invalidate(FALSE);

}

void CChildView::OnGeometryFlipV()
{
	memset(dstData, 0, imageStep*imageHeight); 	
	for(int i=0; i<imageHeight; i++){ //한줄씩 복사, 여긴 버티컬이라 줄단위로함 나머지는 호리존탈은 픽셀별로 해야함
		memcpy(dstData + imageStep*i,
		srcData + (imageHeight - i -1)*imageStep, //메모리 갖다 붙일때 겹치지 않게 하기 위해서
		imageStep);
}
    Invalidate (FALSE);;
}

void CChildView::OnGeometryFlipH()
{
	memset(dstData, 0, imageStep*imageHeight); 	
	for(int i=0; i<imageHeight; i++){
		for(int j=0; j<imageWidth; j++){
			dstData[(imageStep*i)+j] = srcData[(imageStep*i)+ imageWidth-j-1]; //1차원 영상에서 ibyj 픽셀로 접근하는 방법
// 다른 방법 쓰는것 *(dstData + imageStep*i +  j) = * 이런식으로 쓰기도 하뮤, 배열로 쓰는거임
}
	}
	Invalidate (FALSE);

}

void CChildView::OnGeometryRotateLeft()
{
	memset(dstData, 0, imageStep*imageHeight); 	
	for(int i=0; i<imageHeight; i++){
		for(int j=0; j<imageWidth; j++){
			dstData[imageStep*j + imageHeight -i -1] = srcData[imageStep*i + j];
		}
	}
		Invalidate (FALSE);
}

void CChildView::OnGeometryRotateRight()
{
	memset(dstData, 0, imageStep*imageHeight);
	for(int i=0; i<imageHeight; i++){
		for(int j=0; j<imageWidth; j++){
			dstData[imageStep*i +j] = srcData[imageStep*j + imageHeight -i -1];
		}	
		}
		Invalidate (FALSE);
}


void CChildView::OnLutAdd()
{
	unsigned char lut[256];
	for(int i=0; i<256; i++)
		lut[i]=Clip(i + 100,0,255);	
	for(int i=0; i<imageHeight; i++){
		dstData[i]=lut[srcData[i]];
	}
	Invalidate(FALSE);
}


void CChildView::OnUpdateLutAdd(CCmdUI *pCmdUI)
{
	if( dibData == NULL)
		pCmdUI->Enable(FALSE);
}


void CChildView::OnLutNegative()
{
	unsigned char lut[256];
	for(int i=0; i<256; i++)
		lut[i]=255 - i;	
	for(int i=0; i<imageHeight; i++){
		dstData[i]=lut[srcData[i]];
	}
	Invalidate(FALSE);
}


void CChildView::OnUpdateLutNegative(CCmdUI *pCmdUI)
{
	if( dibData == NULL)
		pCmdUI->Enable(FALSE);
}

 
void CChildView::OnLutGamma()
{
	GammaCorrection(2);
}

void CChildView::GammaCorrection(double gamma)
{
	double exp = 1./gamma;
	unsigned char lut [256];
	for (int i = 0; i<255; i++){
		lut[i] = unsigned char (pow (i/255., exp)*255);
	}
	for (int i =0; i<imageWidth*imageHeight; i++){
		dstData[i] = lut[srcData[i]];
		dstData[i] = unsigned char (pow(srcData[i]/255., exp)*255);
	}
	Invalidate(FALSE);
}

void CChildView::OnUpdateLutGamma(CCmdUI *pCmdUI) 
{
	if( dibData == NULL)
		pCmdUI->Enable(FALSE);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	scrollBar.Create(SBS_HORZ | WS_CHILD | WS_VISIBLE,CRect(0, 520, 500, 520 + 10),this, 9999);
	scrollBar.SetScrollRange(0, 100);
	scrollBar.SetScrollPos(0);

	return 0;
}


void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	double gamma = 1. - nPos/200.;
	GammaCorrection(gamma);

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CChildView::OnFilterBlur()
{
	double mask[9] = {1/9., 1/9., 1/9.,
					  1/9., 1/9., 1/9.,
					  1/9., 1/9., 1/9.};
	SpatialFilter3x3(mask);
}

void CChildView::OnFilterSharpen()
{
	double mask[9] = {0, -1, 0,
	                  -1, 5, -1,
	                  0, -1, 0};
	SpatialFilter3x3(mask);
}

void CChildView::SpatialFilter3x3(double*mask)
{
	double sum;
	for (int i = 1; i<imageHeight-1; i++){
		for (int j = 1; j<imageWidth-1; j++){
			sum = 0;
			for(int k = 0; k<3; k++){
				for(int l=0; l<3; l++){
					sum += srcData [(i+k-1)*imageStep + (j+l-1)]*mask[k*3+l];
				}
			}
			dstData[i*imageStep+j] = Clip(int(sum), 0, 255);
		}
	}
	Invalidate(FALSE);

}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	leftButtonDown = TRUE;
	leftButtonPoint = point;
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	leftButtonDown = FALSE;
	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(leftButtonDown){

		int diff = leftButtonPoint. y -point .y;
		unsigned char lut[256];
		for(int i = 0; i<255; i++){
		}
		for(int i = 0; i<imageWidth*imageHeight; i++){
			dstData[i] = lut[srcData[i]];
		}
		Invalidate(FALSE);
	}

	CWnd::OnMouseMove(nFlags, point);
}