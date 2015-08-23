
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
   void GammaCorrection(double gamma);
   CScrollBar		scrollBar;


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
	afx_msg void OnLutAdd();
	afx_msg void OnUpdateLutAdd(CCmdUI *pCmdUI);
	afx_msg void OnLutNegative();
	afx_msg void OnUpdateLutNegative(CCmdUI *pCmdUI);
	afx_msg void OnLutGamma();
	afx_msg void OnUpdateLutGamma(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

