#pragma once


// ViewWnd

class ViewWnd : public CWnd
{
	DECLARE_DYNAMIC(ViewWnd)

public:
	ViewWnd();
	virtual ~ViewWnd();

public:
	CString testString;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


