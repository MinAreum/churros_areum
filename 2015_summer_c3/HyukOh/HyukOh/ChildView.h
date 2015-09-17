
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

	unsigned char * dibData; //dib ������
	unsigned char * dibImage; // dib���� ���� �������� ������
	unsigned char * srcData; // ���� ����
	unsigned char * dstData; // ���� ó����

	int				samplePerPixel, width, height, bitsAllocated, bitsStored;
	int				srcStep, dibStep; // step�� �ѷ� ��������.
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

