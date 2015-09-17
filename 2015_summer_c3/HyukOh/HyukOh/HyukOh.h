
// HyukOh.h : main header file for the HyukOh application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CHyukOhApp:
// See HyukOh.cpp for the implementation of this class
//

class CHyukOhApp : public CWinApp
{
public:
	CHyukOhApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHyukOhApp theApp;
