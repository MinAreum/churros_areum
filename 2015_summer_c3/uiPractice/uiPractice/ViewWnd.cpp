// ViewWnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "uiPractice.h"
#include "ViewWnd.h"


// ViewWnd

IMPLEMENT_DYNAMIC(ViewWnd, CWnd)

ViewWnd::ViewWnd()
{
	testString = L"�ȳ�";
}

ViewWnd::~ViewWnd()
{
}


BEGIN_MESSAGE_MAP(ViewWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// ViewWnd �޽��� ó�����Դϴ�.




int ViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}


void ViewWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


BOOL ViewWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CWnd::OnEraseBkgnd(pDC);
}


void ViewWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect clientRect;
	GetClientRect(clientRect);
	dc.FillSolidRect(clientRect, RGB(100,100,0));
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()��(��) ȣ������ ���ʽÿ�.
}
