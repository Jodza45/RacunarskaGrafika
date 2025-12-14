
// lab2_novi.h : main header file for the lab2_novi application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Clab2noviApp:
// See lab2_novi.cpp for the implementation of this class
//

class Clab2noviApp : public CWinApp
{
public:
	Clab2noviApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clab2noviApp theApp;
