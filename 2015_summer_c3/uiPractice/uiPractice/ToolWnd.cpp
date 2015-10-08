// ToolWnd.cpp : 구현 파일입니다.
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



// ToolWnd 메시지 처리기입니다.




int ToolWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	testButton.Create(L"TEST", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(10,10,100,40),this, 10000);
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void ToolWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL ToolWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CWnd::OnEraseBkgnd(pDC);
}


void ToolWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect clientRect;
	GetClientRect(clientRect);
	dc.FillSolidRect(clientRect, RGB(100,0,10));
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
}

void ToolWnd::OnButtonTest()
{
	CMainFrame * pFrame = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	AfxMessageBox(pFrame->viewWnd->testString);
}