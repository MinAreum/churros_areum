// ToolWnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "uiPractice.h"
#include "ToolWnd.h"
#include "MainFrm.h"
#include "ViewWnd.h"


// ToolWnd

IMPLEMENT_DYNAMIC(ToolWnd, CWnd)

ToolWnd::ToolWnd()
{

}

ToolWnd::~ToolWnd()
{
}


BEGIN_MESSAGE_MAP(ToolWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(10000,OnButtonTest)
END_MESSAGE_MAP()



// ToolWnd �޽��� ó�����Դϴ�.




int ToolWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	testButton.Create(L"TEST", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(10,10,100,40),this, 10000);
	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}


void ToolWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


BOOL ToolWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CWnd::OnEraseBkgnd(pDC);
}


void ToolWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect clientRect;
	GetClientRect(clientRect);
	dc.FillSolidRect(clientRect, RGB(100,0,10));
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()��(��) ȣ������ ���ʽÿ�.
}

void ToolWnd::OnButtonTest()
{
	CMainFrame * pFrame = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	AfxMessageBox(pFrame->viewWnd->testString);
}