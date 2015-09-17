
// ChildView.h : interface of the CChildView class
//


#pragma once


// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	BITMAPINFO * bitmapInfo;

	unsigned char * dibData; //dib 포인터
	unsigned char * dibImage; // dib에서 영상 데이터의 포인터
	unsigned char * srcData; // 영상 원본
	unsigned char * dstData; // 영상 처리본

	int				samplePerPixel, width, height, bitsAllocated, bitsStored;
	int				srcStep, dibStep; // step을 둘로 나누었다.
	int				pixelRepresentation;
	double			windowWidth, windowCenter, windowWidthTemp, windowCenterTemp;
	KD_PHOTOMETRIC	photometric;
	//
	void OpenBMPFile(CString path);
	void OpenDCMFile(CString path);
	BOOL CreateDIB();
	int GetRealWidth(int width);
	void Trans16to8();

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg void OnFileOpen();
	afx_msg void OnArithmeticAdd();
	afx_msg void OnArithmeticSub();
	afx_msg void OnArithmeticMul();
	afx_msg void OnArithmeticDiv();
	afx_msg void OnGeometryFlipV();
	afx_msg void OnGeometryFlipH();
	afx_msg void OnGeometryRotateLeft();
	afx_msg void OnGeometryRotateRight();
	afx_msg void OnFilterBlur();
	afx_msg void OnFilterSharpen();

	DECLARE_MESSAGE_MAP()
};

