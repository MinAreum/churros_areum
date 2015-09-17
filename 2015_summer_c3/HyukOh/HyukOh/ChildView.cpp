
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "HyukOh.h"
#include "ChildView.h"
#include "ippi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	dibData     = NULL;
	dibImage    = NULL;

	dstData     = NULL;
	srcData     = NULL;
}

CChildView::~CChildView()
{
	if(dibData)
		delete[] dibData;

	if(dstData)
		delete[] dstData;
	if(srcData)
		delete[] srcData;
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)

	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_OPEN,			OnFileOpen)
	ON_COMMAND(ID_ARITHMETIC_ADD,		OnArithmeticAdd)
	ON_COMMAND(ID_ARITHMETIC_SUB,		OnArithmeticSub)
	ON_COMMAND(ID_ARITHMETIC_MULTIPLY,	OnArithmeticMul)
	ON_COMMAND(ID_ARITHMETIC_DIVIDE,	OnArithmeticDiv)
	ON_COMMAND(ID_GEOMETRY_FLIPV,		OnGeometryFlipV)
	ON_COMMAND(ID_GEOMETRY_FLIPH,		OnGeometryFlipH)
	ON_COMMAND(ID_GEOMETRY_ROTATELEFT,	OnGeometryRotateLeft)
	ON_COMMAND(ID_GEOMETRY_ROTATERIGHT,	OnGeometryRotateRight)
	ON_COMMAND(ID_FILTER_BLUR,			OnFilterBlur)
	ON_COMMAND(ID_FILTER_SHARPEN,		OnFilterSharpen)

	END_MESSAGE_MAP()

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
	CPaintDC dc(this);

	if(dibData == NULL)
		return;

	// Draw Image
	::SetDIBitsToDevice(dc.m_hDC,
		0, 0, width, height,    // Destination
		0, 0, 0, height,        // Source
		dibImage, bitmapInfo, DIB_RGB_COLORS);	
}

void CChildView::OnFileOpen()
{
	// Show File Dialog Box
	CString szFilter = _T("DICOM Files (*.dcm)|*.dcm|bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, szFilter,this);
	if(dlg.DoModal() == IDCANCEL)
		return;

	CString ext = dlg.GetFileExt();
	ext.MakeUpper();
	if(ext == L"DCM")
		OpenDCMFile(dlg.GetPathName());
}

void CChildView::OpenDCMFile(CString path)
{
	// Allocate KDicomDS object
	KDicomDS * dicomDS = new KDicomDS;

	// Load File
	dicomDS->LoadDS(path);

	// Get Parameters
	samplePerPixel       = dicomDS->m_nSamplePerPixel;
	photometric          = dicomDS->m_nPhotometric;
	width                = dicomDS->m_nWidth;
	height               = dicomDS->m_nHeight;
	bitsAllocated        = dicomDS->m_nBitsAllocated;
	bitsStored           = dicomDS->m_nBitsStored;
	pixelRepresentation  = dicomDS->m_nRepresentation;
	windowCenter         = dicomDS->m_dWindowCenter;
	windowWidth          = dicomDS->m_dWindowWidth;
	if(bitsAllocated == 8)
		srcStep = width * samplePerPixel;
	else
		srcStep = width * 2;

	// Allocate image processing source memory
	if(srcData)
		delete[] srcData;
	srcData  = new unsigned char[srcStep * height];
	dicomDS->GetImageData(srcData);

	// Allocate image processing dest memory
	if(dstData)
		delete[] dstData;
	dstData  = new unsigned char[srcStep * height];
	memcpy(dstData, srcData, srcStep * height);

	// Delete DicomDS
	delete dicomDS;

	CreateDIB();

	Trans16to8();
	
	Invalidate(FALSE);
}

BOOL CChildView::CreateDIB()
{
	int colorNum = 256;
	dibStep = GetRealWidth(width);

	// Calculate DIB size
	int dibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * colorNum + dibStep * height;

	// Allocate DIB memory
	if(dibData)
		delete[] dibData;
	dibData = new unsigned char[dibSize];

	bitmapInfo = (BITMAPINFO *) dibData;

	// Make BITMAPINFOHEADER
	bitmapInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	bitmapInfo->bmiHeader.biWidth			= width;
	bitmapInfo->bmiHeader.biHeight			= height;
	bitmapInfo->bmiHeader.biPlanes			= 1;
	bitmapInfo->bmiHeader.biBitCount		= WORD(samplePerPixel * 8);
	bitmapInfo->bmiHeader.biCompression		= 0;
	bitmapInfo->bmiHeader.biSizeImage		= dibStep * height;
	bitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
	bitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
	bitmapInfo->bmiHeader.biClrUsed			= colorNum;
	bitmapInfo->bmiHeader.biClrImportant	= 0;

	dibImage = dibData + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * colorNum;

	if(photometric == MONOCHROME1){
		for(int i=0;i<colorNum;i++){
			bitmapInfo->bmiColors[i].rgbRed			= 255-i;
			bitmapInfo->bmiColors[i].rgbGreen		= 255-i;
			bitmapInfo->bmiColors[i].rgbBlue		= 255-i;
			bitmapInfo->bmiColors[i].rgbReserved	= 0;
		}
	}
	else if(photometric == MONOCHROME2){
		for(int i=0;i<colorNum;i++){
			bitmapInfo->bmiColors[i].rgbRed			= i;
			bitmapInfo->bmiColors[i].rgbGreen		= i;
			bitmapInfo->bmiColors[i].rgbBlue		= i;
			bitmapInfo->bmiColors[i].rgbReserved	= 0;
		}
	}

	return TRUE;
}

void CChildView::Trans16to8()
{
	if(bitsAllocated == 8)
		return;

	short * src = (short *) dstData;
	int low  = windowCenter - windowWidth / 2;
	int high = windowCenter + windowWidth / 2;
	double ratio = 255 / windowWidth;
	short value;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			value = src[i*width + j];
			if(value < low)
				dibImage[(height - 1 - i)*dibStep + j] = 0;
			else if(value > high)
				dibImage[(height - 1 - i)*dibStep + j] = 255;
			else{
				dibImage[(height - 1 - i)*width + j] = (value - low) * ratio;
			}
		}
	}
}

int CChildView::GetRealWidth(int width)
{
	int real_width;
	div_t r;
	r = div(width,4);
	if(r.rem != 0){
		real_width = ((r.quot + 1) * 4);
		return real_width;
	}
	else
		return width;
}

void CChildView::OnArithmeticAdd()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;

	IppiSize roiSize;
	roiSize.height	= height;
	roiSize.width	= width;

	ippiAddC_16s_C1RSfs(src,srcStep,100,dst,srcStep,roiSize,0);
	Trans16to8();
	Invalidate(FALSE);
}

void CChildView::OnArithmeticSub()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;

	IppiSize roiSize;
	roiSize.height	= height;
	roiSize.width	= width;

	ippiSubC_16s_C1RSfs(src, srcStep, 100,dst, srcStep, roiSize, 0);
	Trans16to8();
	Invalidate(FALSE);
}

void CChildView::OnArithmeticMul()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;

	IppiSize roiSize;
	roiSize.height	= height;
	roiSize.width	= width;

	ippiMulC_16s_C1RSfs(src, srcStep, 2, dst, srcStep, roiSize, 0);
	Trans16to8();
	Invalidate(FALSE);
}

void CChildView::OnArithmeticDiv()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;

	IppiSize roiSize;
	roiSize.height	= height;
	roiSize.width	= width;

	ippiDivC_16s_C1RSfs(src, srcStep, 2,dst, srcStep, roiSize, 0);
	Trans16to8();
	Invalidate(FALSE);
}

void CChildView::OnGeometryFlipV()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;

	IppiSize roiSize;
	roiSize.height	= height;
	roiSize.width	= width;

	ippiMirror_16s_C1R(src, srcStep, dst, srcStep, roiSize, ippAxsVertical);
	Trans16to8();
	Invalidate(FALSE);

}

void CChildView::OnGeometryFlipH()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;

	IppiSize roiSize;
	roiSize.height	= height;
	roiSize.width	= width;

	ippiMirror_16s_C1R(src, srcStep, dst, srcStep, roiSize, ippAxsHorizontal);
	Trans16to8();
	Invalidate(FALSE);

}

void CChildView::OnGeometryRotateLeft()
{
	//if(dibData == NULL)
	//	return;

	//short * src = (short *) srcData;
	//short * dst = (short *) dstData;

	//IppiSize roiSize;
	//roiSize.height	= height;
	//roiSize.width	= width;

	//ippiRotate_16u_C1R();
	//Trans16to8();
	//Invalidate(FALSE);

}

void CChildView::OnGeometryRotateRight()
{
	//if(dibData == NULL)
	//	return;

	//short * src = (short *) srcData;
	//short * dst = (short *) dstData;

	//IppiSize roiSize;
	//roiSize.height	= height;
	//roiSize.width	= width;

	//ippiRotate_16u_C1R();
	//Trans16to8();
	//Invalidate(FALSE);

}

void CChildView::OnFilterBlur() // filter box
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;

	IppiSize roiSize;
	roiSize.height	= height;
	roiSize.width	= width;
	IppiSize maskSize;
	maskSize.height = 3;
	maskSize.width  = 3;
	IppiPoint anchor;
	anchor.x = 1;
	anchor.y = 1;

	ippiFilterBox_16s_C1R(src, srcStep, dst, srcStep, roiSize, maskSize,anchor);
	Trans16to8();
	Invalidate(FALSE);

}

void CChildView::OnFilterSharpen() //fixed filter
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;

	IppiSize roiSize;

	roiSize.height	= height;
	roiSize.width	= width;

	ippiFilterSharpen_16s_C1R(src, srcStep, dst, srcStep, roiSize);
	Trans16to8();
	Invalidate(FALSE);

}

