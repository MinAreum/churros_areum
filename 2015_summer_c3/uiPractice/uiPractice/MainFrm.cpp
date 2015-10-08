
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "uiPractice.h"

#include "MainFrm.h"

#include "ViewWnd.h"
#include "ToolWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	viewWnd = NULL;
	toolWnd = NULL;
}

CMainFrame::~CMainFrame()
{
	if(viewWnd)
		delete viewWnd;
	if(toolWnd)
		delete toolWnd;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	viewWnd = new ViewWnd;
	// create a view to occupy the client area of the frame
	if (!viewWnd->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, 9998, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	toolWnd = new ToolWnd;
	// create a view to occupy the client area of the frame
	if (!toolWnd->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, 9999, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CFrameWnd::OnEraseBkgnd(pDC);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	GetClientRect(clientRect);

	viewRect = toolRect = clientRect;
	viewRect.right = toolRect.left = clientRect.Width()*2/3;

	viewWnd->MoveWindow(viewRect);
	toolWnd->MoveWindow(toolRect);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
