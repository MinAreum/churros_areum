
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "HelloWorld.h"
#include "ChildView.h"
#include "math.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	dibData = NULL;
    dstData = NULL;
	w_dstData = NULL;
	leftButtonDown = FALSE; 
	rightButtonDown = FALSE;
}

CChildView::~CChildView()
{
	if(dibData)
		delete[] dibData;
	if(dstData)
		delete[] dstData;
	if(w_dstData)
		delete[] w_dstData;
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
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
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

BOOL CChildView::CreateDIB() 
{
	int colorNum = 256;

	//Calculate DIB size
	int dibSize = sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*colorNum;
	
	//Allocate DIB memory
	if(dibData)
		delete[] dibData;
	dibData = new unsigned char[dibSize];
	bitmapInfo = (BITMAPINFO *) dibData;

	//Make BITMAPINFOHEADER
	bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo->bmiHeader.biWidth = imageWidth;
	bitmapInfo->bmiHeader.biHeight = imageHeight;
	bitmapInfo->bmiHeader.biPlanes = 1;
	bitmapInfo->bmiHeader.biBitCount = WORD(samplePerPixel*8);
	bitmapInfo->bmiHeader.biCompression = 0;
	bitmapInfo->bmiHeader.biSizeImage = imageStep*imageHeight;
	bitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	bitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	bitmapInfo->bmiHeader.biClrImportant = 0;
	bitmapInfo->bmiHeader.biClrUsed = colorNum;
	bitmapInfo->bmiHeader.biClrImportant = 0;
	if( Photometric == L"MONOCHROME1"){
		for(int i =0; i<colorNum; i++){
			bitmapInfo->bmiColors [i]. rgbRed = 255-i;
			bitmapInfo->bmiColors [i]. rgbGreen = 255-i;
			bitmapInfo->bmiColors [i]. rgbBlue = 255-i;
			bitmapInfo->bmiColors [i]. rgbReserved = 0;
		}
	}
	else if(Photometric == L"MONOCHROME2"){
		for(int i=0; i<colorNum; i++){
			bitmapInfo->bmiColors [i]. rgbRed = i;
			bitmapInfo->bmiColors [i]. rgbGreen = i;
			bitmapInfo->bmiColors [i]. rgbBlue = i;
			bitmapInfo->bmiColors [i]. rgbReserved = 0;
		}
	}

	return TRUE;
}


void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	/*CPen Pen;
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
	Pen.DeleteObject();*/
	
	//if(dibData == NULL)
	//	return;

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
	CString szFilter = _T ("Dicom Files (*.dcm)|*.dcm|bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*|");

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY |
		OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, szFilter, this);
		if(dlg.DoModal()==IDCANCEL)
		return;

	CString ext = dlg.GetFileExt();
	ext.MakeUpper();
	if(ext == L"BMP")
		OpenBMPFile(dlg .GetPathName());
	else if(ext == L"DCM")
		OpenDICOMFile(dlg .GetPathName()); 
}	

void CChildView::OpenBMPFile(CString path)
{
	// open File
	FILE *file;
	_wfopen_s(&file, path, L"rb");
	
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

//void CChildView::OpenDICOMFile(CString path)
//{
//	samplePerPixel = 1;
//	imageWidth = 1;
//	width = 512;
//	height = 512;
//	photometric = L"MPNOCHROME2";
//
//	CreateDIB();
//
//	if(dstData)
//		delete[] dstData;
//	dstData = new unsigned char[step*height];
//	memset(dstData, 100, step*imageHeight);
//
//	Invalidate(FALSE);
//
//}

void CChildView::OpenDICOMFile(CString path)
{
	KDicomDS*ds = new KDicomDS; 

	ds->LoadDS(path, FALSE);

	//Get Parameters
	KDicomElement*de;
	de = ds->GetElement(0x0028, 0x0002);
	if(de){
		 samplePerPixel = de->GetValueUS(0);
	}
	
	//Photometric Representation
	de = ds->GetElement(0x0028, 0x0004);
	if(de){
		Photometric = de->GetValueCS(0);
	}
	
	de = ds->GetElement (0x0028, 0x0011);
	if(de){
		imageWidth = de->GetValueUS(0);
	}

	de = ds->GetElement (0x0028, 0x0010);
	if(de){
		imageHeight = de->GetValueUS(0);
	}

	de = ds->GetElement (0x0028, 0x0100);
	if(de){
		BitsAllocated = de->GetValueUS(0);
	}

	de = ds->GetElement (0x0028, 0x0101);
	if(de){
		BitsStored = de->GetValueUS(0);
	}

	de = ds->GetElement (0x0028, 0x0103);
	if(de){
		PixelRepresentation = de->GetValueUS(0);
	}

	de = ds->GetElement (0x0028, 0x1051);
	if(de){
		WindowWidth = _wtof(de->GetValueDS(0));
	}

	de = ds->GetElement (0x0028, 0x1050);
	if(de){
		WindowCenter = _wtof(de->GetValueDS(0));
	}
	
	CreateDIB();

	imageStep = GetRealWidth(imageWidth);

	if(dstData)
		delete[] dstData;
	dstData = new unsigned char[imageStep * imageHeight];

	//Allocate source memory
	de = ds->GetElement(0x7FE0, 0x0010);
	if(de){
		unsigned char *buff = de->GetValueOB();
		srcData = new unsigned char [imageWidth*imageHeight*2];
		memcpy(srcData, buff, imageWidth*imageHeight*2);
	}


	if(w_dstData)
		delete[] w_dstData;
	w_dstData = new unsigned char[imageStep*imageHeight];
	
	//16bit to 8bit
	short * src = (short*)srcData;
	int low = WindowCenter - WindowWidth/2;
	int high = WindowCenter + WindowWidth/2;
	double ratio = 255/WindowWidth;
	short value;
	for(int i = 0; i<imageHeight; i++){
		for(int j=0; j<imageWidth; j++){
		value = srcData[i*imageWidth+j];
		if(value<low)
			dstData[(imageHeight-1-i)*imageWidth+j] = 0;
		else if(value>high)
			dstData[(imageHeight-1-i)*imageWidth+j] = 255;
		else
			w_dstData[(imageHeight-1-i)*imageWidth+j] = (value-low)*ratio;

			dstData = w_dstData;
		}
	}
	delete ds;
	Invalidate(FALSE);
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
			lut[i] = Clip(i+diff, 0, 255);
		}
		for(int i = 0; i<imageWidth*imageHeight; i++){
			dstData[i] = lut[srcData[i]];
		}
		Invalidate(FALSE);
	}
	if(rightButtonDown){

		int ydiff = rightButtonPoint.y -point.y;
		int xdiff = rightButtonPoint.x - point.x;

		WindowWidth = WindowWidth + xdiff/2;
		WindowCenter = WindowCenter + ydiff/2;

		Invalidate(FALSE);

	}
	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{

	rightButtonDown = TRUE;
	rightButtonPoint = point;

	CWnd::OnRButtonDown(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{

	rightButtonDown = FALSE;

	CWnd::OnRButtonUp(nFlags, point);

}