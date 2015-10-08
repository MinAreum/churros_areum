#pragma once


// ToolWnd

class ToolWnd : public CWnd
{
	DECLARE_DYNAMIC(ToolWnd)

public:
	ToolWnd();
	virtual ~ToolWnd();

public:
	CButton testButton;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnButtonTest();
};


