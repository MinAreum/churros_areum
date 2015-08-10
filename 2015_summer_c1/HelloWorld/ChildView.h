
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
   BITMAPINFO *bitmapInfo;
   unsigned char *dibData;
   unsigned char *dstData;
   unsigned char Clip (int value, int low, int high);
   unsigned char *srcData; 
   int samplePerPixel;
   int imageWidth;
   int imageHeight;
   int imageStep;
   int GetRealWidth(int width);

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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTestHelloworld();
	afx_msg void OnFileopen();
	afx_msg void OnArithmeticAdd();
	afx_msg void OnArithmeticSubtract();
	afx_msg void OnArithmeticMultiply();
	afx_msg void OnArithmeticDivide();
	afx_msg void OnArithmeticNegative();
	afx_msg void OnArithmeticBinary();
	afx_msg void OnGeometryFlipV();
	afx_msg void OnGeometryFlipH();
	afx_msg void OnGeometryRotateLeft();
	afx_msg void OnGeometryRotateRight();
};

